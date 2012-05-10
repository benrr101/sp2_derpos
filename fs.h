////////////////////////////////////////////////////////////////////////////
// DERP_OS - DERP_FS Header
// 
// @file	fs.h
// @descrip	This header declares all constants needed for the DERP filesystem
//			includeing mod constants and size restrictions. This header also
//			declares functions necessary for the creation of a filesystem,
//			reading to the filesystem, and writing to the filesystem.
// @author	Benjamin Russell
////////////////////////////////////////////////////////////////////////////

#ifndef DERP_FS_H
#define DERP_FS_H

// CONSTANTS ///////////////////////////////////////////////////////////////
#define SECT_MBR				0x0

// Partition table info
#define FS_PART_TABLE_OFF		0x01BE
#define FS_PART_TABLE_SIZE		16				// bytes
#define FS_PART_TABLE_ENTRIES	0x04
#define FS_PART_ENTRY_PT		0x04			// Offset of the partition type
#define FS_PART_ENTRY_LBA		0x08			// Offset of LBA of first sect
#define FS_PART_ENTRY_SIZE		0x0C			// Offset of number of sectors

// DERP_FS identification info
#define FS_PARTITION_ID			0x44455250
#define FS_PARTITION_TYPE		0xDE			// Technically Dell diagnostics

#define	FS_SECTOR_SIZE			512
#define FS_SECT_PER_IB			112

// BitTable Definitions
#define FS_BT_SIZE				FS_SECT_PER_IB / 8
#define FS_BT_ALLOCATED			0x1
#define FS_BT_FREE				0x0

// TYPEDEFS ////////////////////////////////////////////////////////////////

/**
 * FSPointer - ABSOLUTE LBA of the sector where a file resides with respect
 * to the start of the partition.
 */
typedef Uint16 FSPointer;

/**
 * FileName - A filename. We're going OLD SKOOL, baby! Max size of 8 chars!
 */
typedef char FileName[8];

/**
 * BitTable - Bits to represent the allocation of the next FS_SECT_PER_IB
 * sectors of the partition (including the FSTable at currentsector-1)
 */
typedef	char BitTable[FS_BT_SIZE];

/**
 * The boot record of the device
 * Word-------- Definition---------
 * 0			Hard coded to 0x44455250 ("DERP")
 * 1			Hard coded to 0x00000000 for alignment
 * 3:2			Size in sectors (multiply by FS_SECTOR_SIZE for bytes)
 * 255:4		Reserved.
 */
typedef struct {
	Uint16	header;			// If it's DERP it should be DERP
	char	alignment[4];	// 0x00000000 for alignment in memory
	Uint32	size;			// Size in sectors
	char	reserved[496];	// Reserved for future use
} BootRecord;

/**
 * FSTable - A table that represents the next FS_SECT_PER_IB sectors INCLUDING
 * ITSELF and the next FS_SECT_PER_IB files.
 */
typedef struct {
	BitTable	sectorAllocation;	// Bits to represent the next
									// FS_MOD_INDEXBLK sectors
	char		reserved[2];		// Reserved.
	FSPointer	fsPointers[FS_SECT_PER_IB];	// Pointers to the next 112 files
} FSTable;

/**
 * MountPoint - Points to a ATADevice, the offset into the drive where the
 * partition starts at, and stores the BootRecord of the partition.
 */
typedef struct {
	ATADevice 	*device;	// Pointer to the device that the partition is on
	BootRecord	bootRecord;	// The boot record of the partition
	Uint32		offset;		// The offset into the drive where the partition
							// begins
	char		letter;		// The mount point. A-Z
} MountPoint;

// GLOBALS /////////////////////////////////////////////////////////////////
// We can only do A-Z (26) mount points
MountPoint mount_points[26];

// The current number of mount points
Uint8 mount_point_count;

// FUNCTIONS ///////////////////////////////////////////////////////////////
void _fs_probe(ATADevice *dev);

#endif
