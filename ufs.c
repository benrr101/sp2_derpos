////////////////////////////////////////////////////////////////////////////
// DERP OS - User-level FS Abstraction
//
// @descrip	This file defines user-level interactions with FS abstraction. This
//			includes processing of file names, opening, creating and deleting
//			files housed on the system's disks.
// @file	ufs.c
// @author	Benjamin Russell
////////////////////////////////////////////////////////////////////////////

// INCLUDES ////////////////////////////////////////////////////////////////

#include "ufs.h"

// FUNCTIONS ///////////////////////////////////////////////////////////////

/**
 * Creates a new file pointer that can be used for all functions affecting the
 * user-accessible filesystem. If the file does not exist, it is created. If
 * the file already exists, then a pointer to the file is returned and the
 * internal pointer is set to address 0 of the file.
 * @param	char[10]	filename	The name of the file to open. Should follow
 * 									format: X:yyyyyyyy
 *									X = mountpoint letter
 *									y = filename up to 8 char in length.
 * @return	FILE	Pointer to the file in the filesystem.
 */
FILE fopen(char filepath[10]) {
	// Pre generate the FILE for output
	FILE f;

	// Process the filename X:yyyyyyyy
	// X = mount point letter
	// yyyyyyy = file name on mount point
	// verify a valid filename
	if(filepath[1] != ':') {
		f.code = FS_INVALID_FILENAME;
		return f;
	}

	char mountpoint = filepath[0];
	Uint8 i;
	for(i = 0; i < FS_NAME_SIZE; i++) {
		f.name[i] = filepath[2 + i];
	}
	
	// Determine which device we're reading
	MountPoint *mp = &mount_points[mountpoint - 0x41];

	// See if we can find the file
	FSPointer fp = _fs_find_file(mp, f.name);
	if(fp.mp == NULL) {
		// File not found -- so create it!
		f = _fs_create_file(mp, f.name);
		f.code = FS_SUCCESS;
		return f;
	}

	// File was found. Construct the FILE
	f.fp     = fp;
	f.code   = FS_SUCCESS;
	f.offset = 0;
	return f;
}

/**
 * Changes the internal file offset by adding, subtracting or replacing the
 * offset value provided.
 * @param	FILE*	file	The file to seek
 * @param	Uint64	offset	Position to seek to or +/- position
 * @param	FS_FILE_SEEK	dir	Direction to seek
 * FS_SEEK_ABS	- seeks to an absolute position into the file
 * FS_SEEK_REL	- seeks to a position relative to the current position (+ only)
 * FS_SEEK_REL_REV	- seeks to a position relative to the current position (-)
 * @return	FS_STATUS	status of the seek operation
 */
FS_STATUS fseek(FILE *file, Uint64 offset, FS_FILE_SEEK dir) {
	// We need the size of the file
	Uint64 filesize = _fs_get_file_size(file->fp);

	// Which way are we seeking
	if(dir == FS_SEEK_ABS) {
		// Seeking to absolute position into the disk
		if(offset >= filesize) {
			// We'd be going off the end of the file
			return FS_INVALID_OFFSET;
		}

		// Set the file offset to the offset
		file->offset = offset;

	} else if(dir == FS_SEEK_REL) {
		// Seeking to position relative to current position
		if(offset + file->offset >= filesize) {
			// We'd be going off the end of the file
			return FS_INVALID_OFFSET;
		}

		// Add to the file offset
		file->offset += offset;

	} else if(dir == FS_SEEK_REL_REV) {
		// Seeking to behind current position
		if(offset - file->offset > file->offset) {
			// We wrapped around... that's not good.
			return FS_INVALID_OFFSET;
		}
		
		// Subtract the offset
		file->offset -= offset;
	}

	// Success!
	return FS_SUCCESS;
}

Uint64 fread(FILE file, char *buffer, Uint64 size) { return 0x0; }
