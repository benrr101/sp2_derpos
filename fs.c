////////////////////////////////////////////////////////////////////////////
// DERP_OS - DERP_FS
//
// @file	fs.c
// @descrip	This file defines all the functions needed for writing to and from
//			a DERP_FS. This also includes all necessary initialization tools.
// @author	Benjamin Russell
////////////////////////////////////////////////////////////////////////////

// REQUIRED FILES //////////////////////////////////////////////////////////

#include "headers.h"
#include "pci.h"
#include "startup.h"
#include "ata.h"
#include "fs.h"

// FUNCTIONS ///////////////////////////////////////////////////////////////

/**
 * Probes the given ata device for any DERP_FS partitions. If a partition is
 * found, the partition is mounted to the mount_points global variable and the
 * value of mount_point_count is incremented.
 * @param	ATADevice*	dev		The device to scan for DERP_FS partitions
 */
void _fs_probe(ATADevice *dev) {
	// Create a blank sector for reading in the mbr
	ATASector mbr;
	_ata_blank_sector(&mbr);

	// Read in the MBR
	_ata_read_sector(*dev, SECT_MBR, &mbr);
	
	// Go to the partition table and iterate over each entry
	Uint16 i;
	Uint16 endOfTable = FS_PART_TABLE_OFF + (FS_PART_TABLE_SIZE * FS_PART_TABLE_ENTRIES);
	for(i = FS_PART_TABLE_OFF; i < endOfTable; i += FS_PART_TABLE_SIZE) {
		// What type is the partiton?
		if(mbr[i + FS_PART_ENTRY_PT] == FS_PARTITION_TYPE) {
			// We found a DERP_FS partition!!
			// Read the first sector of the partiton to see if it's derp
			ATASector br;
			Uint32 brAddr = _sector_get_long(&mbr, i + FS_PART_ENTRY_LBA);
			_ata_read_sector(*dev, brAddr, &br);

			// Is it DERP?
			Uint32 header = br[0]<<24|br[1]<<16|br[2]<<8|br[3];
			Uint32 size   = _sector_get_long(&br, FS_BR_SIZE);
			if(header == FS_PARTITION_ID) {
				// Increment the count of mount points and set up the mp
				Uint8 mp_id = mount_point_count;
				mount_point_count++;
				MountPoint mp;
				
				// Build the MountPoint
				mp.device = dev;
				mp.bootRecord.header = header;
				mp.bootRecord.size = size;
				mp.offset = brAddr;
				mp.letter = mp_id + 0x41;

				// Store the MountPoint
				mount_points[mp_id] = mp;
			}
		}
	}
}

FS_STATUS _fs_create_partition(ATADevice *dev, Uint32 start, Uint32 size, Uint8 index) {
	// Detect stupidity errors
	if(index >= FS_PART_TABLE_ENTRIES) {
 		// Can't have > 4 partitions
		return FS_ERR_BADINDEX; 
	}
	if(size >= dev->size || size + start >= dev->size) { 
		// Can't partition off more sectors that the drive can hold
		return FS_ERR_TOOBIG; 
	}
	if(size <= 4) {
		// Can't have less sectors than required for metadata
		return FS_ERR_TOOSMALL;
	}
	if(start == SECT_MBR) {
		// Can't place a partition over the MBR
		return FS_ERR_BADSECT;
	}

	// Make the size divisible by 112
	if(size % FS_SECT_PER_IB != 0) {		
		// Size of partition is not divisible by 112. Make it so.
		size += FS_SECT_PER_IB - (size % FS_SECT_PER_IB);
		size++; // Making room for the BootRecord
	}

	// Read in the current MBR
	ATASector mbr;
	_ata_read_sector(*dev, SECT_MBR, &mbr);

	// Verify that it's actually an MBR
	if(!(mbr[510] == 0x55 && mbr[511] == 0xAA)) {
		// It's not a valid MBR. We need to rewrite it.
		_ata_blank_sector(&mbr);
		mbr[510] = 0x55;	// The MBR signature
		mbr[511] = 0xAA;
	}

	// Now jump to the index of the partition
	Uint32 indexAddr = FS_PART_TABLE_OFF + index * FS_PART_TABLE_SIZE;

	// Write the type of the partition
	mbr[indexAddr + FS_PART_ENTRY_PT] = FS_PARTITION_TYPE;

	// Write the geometry of the partition
	_sector_put_long(&mbr, indexAddr + FS_PART_ENTRY_LBA, start);
	_sector_put_long(&mbr, indexAddr + FS_PART_ENTRY_SIZE, size);

	// Write the mbr back to the disk
	_ata_write_sector(*dev, SECT_MBR, &mbr);
	return FS_SUCCESS;
}

FS_STATUS _fs_format(MountPoint *mp, ATADevice *dev, Uint8 index) {
	// Load up the mbr of the device
	ATASector mbr;
	_ata_read_sector(*dev, SECT_MBR, &mbr);

	// Verify that the partition is DERP
	Uint32 indexAddr = FS_PART_TABLE_OFF + index * FS_PART_TABLE_SIZE;
	if(mbr[indexAddr + FS_PART_ENTRY_PT] != FS_PARTITION_TYPE) {
		return FS_ERR_NOTDERP;
	}

	// BOOT RECORD /////////////////////////////////////////////////////////
	// Grab the LBA for the partition
	Uint32 lba  = _sector_get_long(&mbr, indexAddr + FS_PART_ENTRY_LBA);
	Uint32 size = _sector_get_long(&mbr, indexAddr + FS_PART_ENTRY_SIZE);

	// Build a boot record for the partition
	ATASector br;
	_ata_blank_sector(&br);

	// Write the header of the br
	br[0]=0x44; br[1]=0x45; br[2]=0x52; br[3]=0x50;

	// Write the size of the partition to the br
	_sector_put_long(&br, FS_BR_SIZE, size);

	// Write out the bootrecord
	_ata_write_sector(*dev, lba, &br);
	
	// INDEX BLOCKS ////////////////////////////////////////////////////////
	// Build an index block
	ATASector fsTable;
	ATASector nameTable;
	_ata_blank_sector(&fsTable);
	_ata_blank_sector(&nameTable);
	
	// Set the reserved bits of the BitTable
	_sector_put_word(&fsTable, FS_BT_RESERVED, 0xFFFF);

	// These are written every FS_MOD_INDEXBLK sectors, offset by 1 for the mbr
	Uint32 i, count;
	for(i = 1; i < size; i += FS_SECT_PER_IB) {
		// Do we need to block off some sectors of the next sector?
		/*if(i + FS_SECT_PER_IB > size) {
			// Yep we'll be going off the partition with this set
			// Mark the allocation of the sectors that do not belong to this
			// partition set to allocated.
			Uint32 j;
			for(j = i + FS_SECT_PER_IB; j > size; j--) {
				// Mark the bit for the sector
				Uint16 mask     = 0x1 << ( (j - i + FS_SECT_PER_IB) % 16);
				Uint16 bitfield = _sector_get_word(&fsTable, (j - (i + FS_SECT_PER_IB)) / 16);
				bitfield |= mask;
				_sector_put_long(&fsTable, (j - (i + FS_SECT_PER_IB)) / 16, bitfield);
			}
		}*/
		

		// Mark the first 3 sectors are in use
		Uint32 mask = 0x7;
		Uint32 bitfield = _sector_get_word(&fsTable, 12);
		bitfield |= mask;
		_sector_put_long(&fsTable, 12, bitfield);

		// Write the FSTable
		_ata_write_sector(*dev, lba + i, &fsTable);

		// Write the blank name table
		_ata_write_sector(*dev, lba + i + 1, &fsTable);
		_ata_write_sector(*dev, lba + i + 2, &fsTable);
		count++;
	} 
	c_printf("Format wrote:0x%x times\n", count);

	return FS_SUCCESS;
}
