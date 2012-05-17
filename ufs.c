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
 * Flushes the internal sector buffer to the disk.
 * @param	FILE*	file	The file to flush
 * @return	FS_STATUS	Success/failure code.
 */
FS_STATUS fflush(FILE *file) {
	// Calculate which sector to write
	Uint32 sector = file->fp.mp->offset + file->fp.bufsect;
	
	// Write to the appropriate sector
	_ata_write_sector(*(file->fp.mp->device), sector, &(file->fp.buffer));

	//@TODO Error handling
	return FS_SUCCESS;
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

/**
 * Read bytes from the given file into the given buffer. This function checks
 * for end of the file and reads the number of bytes specified or until the end
 * of the file, whichever comes first. The internal file pointer is incremented
 * as the file is read.
 * @param	FILE*	file	The file to read from
 * @param	char*	buffer	The buffer to place read bytes
 * @param	Uint64	size	The maximum number of bytes to read from the file
 * @return	Uint64	The number of bytes read.
 */
Uint64 fread(FILE *file, char *buffer, Uint64 size) { 
	// @TODO Error check for dumb things

	// Start copying bytes into the buffer
	Uint64 bytes;
	Uint32 sectBytes = (Uint32)file->offset % FS_FILE_DATA_LENGTH;
	for(bytes = 0; bytes < size; bytes++, sectBytes++) {
		// Is this byte even in the file?
		if(sectBytes >= _sector_get_long(&(file->fp.buffer), FS_FILE_BYTE_OFF)) {
			// The next read would be outside the file
			return bytes;
		}

		// Does this byte require us to load a new buffer?
		if((Uint32)(file->offset + bytes) / FS_FILE_DATA_LENGTH > file->fp.bufindex) {
			// This read should be on the next sector of the file
			Uint32 nextSector = _sector_get_long(&(file->fp.buffer), FS_FILE_SECT_OFF);
			if(nextSector == FS_FILE_EOC) {
				// There is no next sector... (the file is % 500 = 0)
				return bytes;
			}
			
			// Store the new buffered sector
			ATASector newBuf;
			_ata_read_sector(*(file->fp.mp->device), 
				file->fp.mp->offset + nextSector, &newBuf);
			_fs_copy_sector(&newBuf, &(file->fp.buffer));

			// Increment the buffer index
			file->fp.bufindex++;
			file->fp.bufsect = nextSector;

			// Reset the bytes into the sector
			sectBytes = 0;
		}

		// Ok, copy the byte
		buffer[bytes] = file->fp.buffer[FS_FILE_DATA_OFF + sectBytes];
	}
	
	// Done!
	return bytes;
}

/**
 * Writes bytes to the given file from the given buffer. This function updates
 * the size of the file as the file is written. The number of written bytes are
 * returned. The internal buffer is flushed as a new buffer is required to be
 * read in. However, any overhang on the buffer will require fflush to be
 * called on the file.
 * @param	FILE*	file	The file to write to
 * @param	char*	buffer	The buffer to grab written bytes from
 * @param	Uint64	size	The maximum number of bytes to write to the file
 * @return	Uint64	The number of bytes written.
 */
Uint64 fwrite(FILE *file, char *buffer, Uint64 size) { 
	// @TODO Error check for dumb things

	// Start copying bytes into the buffer
	Uint64 bytes;
	Uint32 sectBytes = (Uint32)file->offset % FS_FILE_DATA_LENGTH;
	for(bytes = 0; bytes < size; bytes++, sectBytes++, file->offset++) {
		// Do we need to allocate a new sector?
		if(sectBytes >= FS_FILE_DATA_LENGTH) {
			// Do we need to allocate a new sector?
			Uint32 newSector = _sector_get_long(&(file->fp.buffer), FS_FILE_SECT_OFF);
			if(newSector == FS_FILE_EOC) {
				// Yep, we need to allocate a new sector
				newSector = _fs_find_empty_sector(file->fp.mp);
				
				// @TODO: Error check!
				_fs_allocate_sector(file->fp.mp, newSector);
				
				// Write the next sector to the file and flush the buffer
				_sector_put_long(&(file->fp.buffer), FS_FILE_SECT_OFF, newSector);
			}

			// Flush the file and load a new buffer
			fflush(file);

			// Load the newly minted sector into the file's buffer
			ATASector newSect;
			_ata_read_sector(*(file->fp.mp->device), 
				file->fp.mp->offset + newSector, &newSect);
			_fs_copy_sector(&newSect, &(file->fp.buffer));
			file->fp.bufsect = newSector;
			file->fp.bufindex++;

			// Reset the bytes into the sector
			sectBytes = 0;
		}

		// Ok, copy the byte
		file->fp.buffer[FS_FILE_DATA_OFF + sectBytes] = buffer[bytes];

		// Increment number of bytes in this sector
		Uint32 oldbytes = _sector_get_long(&(file->fp.buffer), FS_FILE_BYTE_OFF);
		oldbytes++;
		_sector_put_long(&(file->fp.buffer), FS_FILE_BYTE_OFF, oldbytes);
	}
	
	// Done!
	return bytes;
}
