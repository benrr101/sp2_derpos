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
	_sector_put_word(&fsTable, FS_BT_END, 0xFFFF);

	// Write the header of the index block
	fsTable[0]=0x44; fsTable[1]=0x45; fsTable[2]=0x49; fsTable[3]=0x42;

	// These are written every FS_MOD_INDEXBLK sectors, offset by 1 for the mbr
	Uint32 i, count;
	for(i = 1; i < size; i += FS_SECT_PER_IB) {
		// Mark the first 3 sectors are in use
		Uint32 mask = 0x7;
		Uint32 bitfield = _sector_get_long(&fsTable, 13);
		bitfield |= mask;
		_sector_put_long(&fsTable, 13, bitfield);

		// Write the FSTable
		_ata_write_sector(*dev, lba + i, &fsTable);

		// Write the blank name table
		_ata_write_sector(*dev, lba + i + 1, &nameTable);
		_ata_write_sector(*dev, lba + i + 2, &nameTable);
		count++;
	} 

	return FS_SUCCESS;
}

Uint32 _fs_find_empty_sector(MountPoint *mp) {
	// Start checking index blocks
	Uint32 i, j, byte;
	Uint32 end = mp->offset + mp->bootRecord.size;
	ATASector s;

	// Loop from the first index block to the last index block
	for(i = mp->offset + 1; i < end; i += FS_SECT_PER_IB) {
		// Load the sector
		_ata_read_sector(*(mp->device), i, &s);
		
		// Start iterating over the bits in the bitfield
		for(j = 0; j < FS_BT_SIZE * sizeof(char); j++) {
			// Which byte do we need 
			byte = FS_BT_OFFSET + (FS_BT_END - (FS_BT_OFFSET + (j / 8)));
			byte -= 1;

			// Grab that byte and check if the bit we need is allocated
			if(((s[byte] >> (j % 8)) & FS_BT_ALLOCATED) == 0) {
				c_printf("Free bitfieldbyte is: 0x%x (byte %x)\n", s[byte], byte);
				// Sector isn't allocated. This is our golden nugget
				// Calculate the sector into the partition this is
				return (i - mp->offset) + j;
			}
		}
	}

	// If we make it here, there's no free sectors
	return 0x0;
}

FSPointer _fs_find_empty_fspointer(MountPoint *mp) {
	// Start checking index blocks
	Uint32 i, j;
	Uint32 end = mp->offset + mp->bootRecord.size;
	ATASector s;
	
	// Loop from the first index block to the last index block
	for(i = mp->offset + 1; i < end; i += FS_SECT_PER_IB) {
		// Load the sector
		_ata_read_sector(*(mp->device), i, &s);

		// Start iterating over the pointers in the table
		for(j = FS_FP_OFFSET; j < FS_FP_END; j += FS_FP_LENGTH) {
			if(_sector_get_long(&s, j) == FS_FP_FREE) {
				// This file pointer entry is free
				// Create a fs ptr to represent it and return it
				FSPointer f;
				f.mp      = mp;
				f.ib      = i;
				f.ibindex = (j - FS_FP_OFFSET) / FS_FP_LENGTH;
				return f;	
			}
		}
	}

	// If we make it here, there are no free pointers
	FSPointer f;
	f.mp = NULL;
	return f;
}
