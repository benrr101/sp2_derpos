#ifndef _VMEM_REF_H
#define _VMEM_REF_H
//#define _VMEM_REF_DEBUG

#include "headers.h"

#define REF_EMPTY	0x0
#define REF_ADDRESS 	0xFFFF000
#define REF_VALID	0x02
#define REF_4MB		0x01
#define REF_COUNT	0xFFC //means a page can only be shared 1024 times
#define REF_COUNT_ADD	0x04
#define REF_START	0x5000000
#define REF_END		0x5400000
#define REF_SIZE	1048576 //4MB / 4 bytes

void _vmem_ref_init( void );

Uint32 _vmem_ref_get_count( Uint32 address, Uint8 is4Mb );

//void _vmem_ref_set count( Uint32 address, Uint8 is4Mb );

void _vmem_ref_inc_count( Uint32 address, Uint8 is4Mb );

void _vmem_ref_dec_count( Uint32 address, Uint8 is4Mb );

#endif
