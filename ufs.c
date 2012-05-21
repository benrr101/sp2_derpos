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
FILE *fopen(char filepath[10]) {
	// Pre generate the FILE for output
	FILE f;

	// Process the filename X:yyyyyyyy
	// X = mount point letter
	// yyyyyyy = file name on mount point
	// verify a valid filename
	if(filepath[1] != ':') {
		f.code = FS_INVALID_FILENAME;
		return NULL;
	}

	char mountpoint = filepath[0];
	Uint8 i;
	for(i = 0; i < FS_NAME_FILENAME; i++) {
		f.name[i] = filepath[2 + i];
	}
	
	// Determine which device we're reading
	MountPoint *mp = &mount_points[mountpoint - 0x41];
	// @TODO: Make sure there is a mountpoint

	// See if we can find the file
	f = _fs_find_file(mp, f.name);
	if(f.code == FS_ERR_FILENOTFOUND) {
		// Make sure there is a free filepointer first
		if(!_fs_is_free_filepointer()) {
			// No file pointers, so blow up.
			return NULL;
		}

		// File not found -- so create it!
		f = _fs_create_file(mp, f.name);
		f.code = FS_SUCCESS_NEWFILE;
	} else {
		// The file does exist. Is it open already
		if(_fs_file_inuse(&f) == FS_ERR_FILE_INUSE) {
			// File is in use, so we can't use it again
			return NULL;
		}

		f.code = FS_SUCCESS;
	}

	// Allocate a file pointer
	FILE *file;
	if(_fs_allocate_filepointer(&f, &file) == FS_ERR_NO_FP) {
		return NULL;
	}

	// The file was found, so package it up and send it back to the user
	// Set the status code and initialize the offset
	file->offset = 0;

	return file;
}

FS_STATUS fclose(FILE *file) {
	// Flush the file
	fflush(file);

	// Unallocate the pointer
	_fs_unallocate_filepointer(file);

	return 0x0;
}

FS_STATUS fdelete(FILE *file) {
	// Mark the file pointer as available

	
	// Tear apart the file and send it to the deleter
	return _fs_delete_file(file);
}

/**
 * Flushes the internal sector buffer to the disk.
 * @param	FILE*	file	The file to flush
 * @return	FS_STATUS	Success/failure code.
 */
FS_STATUS fflush(FILE *file) {
	// Calculate which sector to write
	Uint32 sector = file->mp->offset + file->bufsect;
	
	// Write to the appropriate sector
	_ata_write_sector(file->mp->device, sector, &(file->buffer));

	//@TODO Error handling
	return FS_SUCCESS;
}

/**
 * Changes the internal file offset by adding, subtracting or replacing the
 * offset value provided.
 * @param	FILE*	file	The file to seek
 * @param	Uint32	offset	Position to seek to or +/- position
 * @param	FS_FILE_SEEK	dir	Direction to seek
 * FS_SEEK_ABS	- seeks to an absolute position into the file
 * FS_SEEK_REL	- seeks to a position relative to the current position (+ only)
 * FS_SEEK_REL_REV	- seeks to a position relative to the current position (-)
 * @return	FS_STATUS	status of the seek operation
 */
FS_STATUS fseek(FILE *file, Uint32 offset, FS_FILE_SEEK dir) {
	// We need the size of the file
	Uint32 filesize = _fs_get_file_size(file);

	// Which way are we seeking
	if(dir == FS_SEEK_ABS) {
		// Seeking to absolute position into the disk
		if(offset >= filesize) {
			// We'd be going off the end of the file
			return FS_INVALID_OFFSET;
		}

		// Calculate the INDEX of the sector (not the sector number)
		Uint32 nextIndex = offset / FS_FILE_DATA_LENGTH;

		// Will we be seeking into a different sector?
		if(nextIndex != file->bufindex) {
			// Yes, we will be seeking into a new sector
			// Flush the current buffer
			fflush(file);
	
			// Grab the index block to get the address of the first sector
			// We'll trash the buffer for temp usage
			_ata_read_sector(file->mp->device, file->mp->offset + file->ib, &(file->buffer));
			Uint32 sector = _sector_get_long(&(file->buffer), 
				FS_FP_OFFSET + (file->ibindex * FS_FP_LENGTH));
			
			// Start from the first sector and follow the chain until we get to
			// the sector we're looking for
			Uint32 i;
			for(i = 0; i  <= nextIndex; i++) {
				// Load the next sector
				file->bufsect = sector;	// Must happen here otherwise we
										// are off by one
				_ata_read_sector(file->mp->device, file->mp->offset + sector, &(file->buffer));
				sector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			}

			// Store the sector as the buffer
			file->bufindex = nextIndex;
		}

		// Set the file offset to the offset
		file->offset = offset;

	} else if(dir == FS_SEEK_REL) {
		// Will we be seeking off of the file?
		if(offset + file->offset >= filesize) {
			// We'd be going off the end of the file
			return FS_INVALID_OFFSET;
		}

		// Calculate the INDEX of the next sector
		Uint32 nextSect = (file->offset + offset) / FS_FILE_DATA_LENGTH;

		// Will we be seeking into a different sector?
		if(nextSect != file->bufsect) {
			// Yes, we will be seeking into a new sector
			// Flush the current buffer so we can trash it
			fflush(file);

			// How many sectors jumps do we need to make?
			Uint32 seekSect = nextSect - file->bufindex;	

			Uint32 i;
			Uint32 sector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			for(i = 0; i < seekSect; i++) {
				// Load the next sector
				file->bufsect = sector;	// Must happen here otherwise we
										// are off by one
				_ata_read_sector(file->mp->device, file->mp->offset + sector, &(file->buffer));
				sector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			}

			// Store the sector as the buffer
			file->bufindex += seekSect;
		}

		// Add to the file offset
		file->offset += offset;

	} else if(dir == FS_SEEK_REL_REV) {
		// Seeking to behind current position
		if(file->offset - offset > file->offset) {
			// We wrapped around... that's not good.
			return FS_INVALID_OFFSET;
		}

		Uint32 nextSect = (file->offset - offset) / FS_FILE_DATA_LENGTH;
		// Will we be seeking into a different sector?
		if(nextSect != file->bufsect) {
			// Yes, we will be seeking into a new sector
			// Flush the buffer so we can trash it
			fflush(file);
	
			// Grab the index block to get the address of the first sector
			_ata_read_sector(file->mp->device, file->mp->offset + file->ib, &(file->buffer));
			Uint32 sector = _sector_get_long(&(file->buffer), 
				FS_FP_OFFSET + (file->ibindex * FS_FP_LENGTH));
			
			// Start from the first sector and follow the chain until we get to
			// the sector we're looking for
			Uint32 i;
			for(i = 0; i  <= nextSect; i++) {
				// Load the next sector
				file->bufsect = sector;	// Must happen here otherwise we
										// are off by one
				_ata_read_sector(file->mp->device, file->mp->offset + sector, &(file->buffer));
				sector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			}

			// Store the sector as the buffer
			file->bufindex = nextSect;
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
 * @param	Uint32	size	The maximum number of bytes to read from the file
 * @return	Uint32	The number of bytes read.
 */
Uint32 fread(FILE *file, char *buffer, Uint32 size) { 
	// Make sure the file is in a good state for reading
	if(file->code == FS_AVAILABLE) {
		// This is an uninitalized file pointer
		return 0;
	}

	// Start copying bytes into the buffer
	Uint32 bytes;
	Uint32 sectBytes = file->offset % FS_FILE_DATA_LENGTH;
	for(bytes = 0; bytes < size; bytes++, sectBytes++) {
		// Is this byte even in the file?
		if(sectBytes >= _sector_get_long(&(file->buffer), FS_FILE_BYTE_OFF)) {
			// The next read would be outside the file
			break;
		}

		// Does this byte require us to load a new buffer?
		if((file->offset + bytes) / FS_FILE_DATA_LENGTH > file->bufindex) {
			// Flush the current buffer
			fflush(file);

			// This read should be on the next sector of the file
			Uint32 nextSector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			if(nextSector == FS_FILE_EOC) {
				// There is no next sector... (the file is % 500 = 0)
				return bytes;
			}
			
			// Store the new buffered sector
			_ata_read_sector(file->mp->device, 
				file->mp->offset + nextSector, &(file->buffer));

			// Increment the buffer index
			file->bufindex++;
			file->bufsect = nextSector;

			// Reset the bytes into the sector
			sectBytes = 0;
		}

		// Ok, copy the byte
		buffer[bytes] = file->buffer[FS_FILE_DATA_OFF + sectBytes];
	}

	// Increment the internal file offset
	file->offset += bytes;
	
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
 * @param	Uint32	size	The maximum number of bytes to write to the file
 * @return	Uint32	The number of bytes written.
 */
Uint32 fwrite(FILE *file, char *buffer, Uint32 size) { 
	// @TODO Error check for dumb things

	// Start copying bytes into the buffer
	Uint32 bytes;
	Uint32 sectBytes = file->offset % FS_FILE_DATA_LENGTH;
	for(bytes = 0; bytes < size; bytes++, sectBytes++, file->offset++) {
		// Do we need to allocate a new sector?
		if(sectBytes >= FS_FILE_DATA_LENGTH) {
			// Do we need to allocate a new sector?
			Uint32 newSector = _sector_get_long(&(file->buffer), FS_FILE_SECT_OFF);
			if(newSector == FS_FILE_EOC) {
				// Yep, we need to allocate a new sector
				newSector = _fs_find_empty_sector(file->mp);
				
				// @TODO: Error check!
				_fs_allocate_sector(file->mp, newSector);
				
				// Write the next sector to the file and flush the buffer
				_sector_put_long(&(file->buffer), FS_FILE_SECT_OFF, newSector);
			}

			// Flush the file and load a new buffer
			fflush(file);

			// Load the newly minted sector into the file's buffer
			_ata_read_sector(file->mp->device, 
				file->mp->offset + newSector, &(file->buffer));
			file->bufsect = newSector;
			file->bufindex++;

			// Reset the bytes into the sector
			sectBytes = 0;
		}

		// Ok, copy the byte
		file->buffer[FS_FILE_DATA_OFF + sectBytes] = buffer[bytes];

		// Increment number of bytes in this sector
		Uint32 oldbytes = _sector_get_long(&(file->buffer), FS_FILE_BYTE_OFF);
		if(sectBytes >= oldbytes) {
			oldbytes++;
			_sector_put_long(&(file->buffer), FS_FILE_BYTE_OFF, oldbytes);
		}
	}
	
	// Done!
	return bytes;
}
