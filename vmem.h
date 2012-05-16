/*
** File:	vmem.h
**
** Author:	Sean McDermott
**
** Contributor:
**
** Description:	Virtual Memory Definitions
*/

#ifndef _VMEM_H
#define _VMEM_H
//#define _VMEM_DEBUG

#include "headers.h"

#define PAGE_BITS 12
#define PAGE_SIZE (1 << PAGE_BITS )
#define PAGE_ADDRESS_LOC ( 0xFFFFFFFF << PAGE_BITS)
#define PAGE_TABLE_SIZE (PAGE_SIZE * 1024)
#define PAGE_RESERVE_AREA 0xFD800000
#define PAGE_RESERVE_ENTRIES ( 1024 - ( PAGE_RESERVE_AREA  / PAGE_TABLE_SIZE ) )
#define BITMAP_MAX 32768 //4GB

#define PAGE_DIR_4KB_ADDRESS 0xffff000
#define PAGE_DIR_4MB_ADDRESS 0xFFC0000
#define PAGE_DIR_RESERV 0x600
#define PAGE_DIR_IGNORE 0x100
#define PAGE_DIR_SIZE 0x80
#define PAGE_DIR_DIRTY 0x40
#define PAGE_DIR_ACCESS 0x20
#define PAGE_DIR_CACHE_DISABLE 0x10
#define PAGE_DIR_WRITE_THROUGH 0x8
#define PAGE_DIR_USER_MODE 0x4
#define PAGE_DIR_WRITE 0x2
#define PAGE_DIR_PRESENT 0x1

#define PAGE_TABLE_ADDRESS 0xffff800
#define PAGE_TABLE_RESERV 0x600
#define PAGE_TABLE_GLOBAL 0x100
#define PAGE_TABLE_ZERO 0x80
#define PAGE_TABLE_DIRTY 0x40
#define PAGE_TABLE_ACCESSED 0x20
#define PAGE_TABLE_CACHE_DISABLE 0x10
#define PAGE_TABLE_WRITE_THROUGH 0x8
#define PAGE_TABLE_USER_MODE 0x4
#define PAGE_TABLE_WRITE 0x2
#define PAGE_TABLE_PRESENT 0x1

extern Uint32* _vmem_page_dir;
/*
** _vmem_init()
**
*/
void _vmem_init( void );
Uint32 _vmem_first_4MB( void );
void _vmem_init_bitmap( Uint32 addr );

Int8 _vmem_read_bit( Uint32* address, Uint32 index, Int8 index2 );
void _vmem_set_bit( Uint32* address, Uint32 index, Int8 index2 );
void _vmem_clear_bit( Uint32* address, Uint32 index, Int8 index2);

void _vmem_set_address( Uint32 address );
void _vmem_set_4MB_address( Uint32 addres );
void _vmem_clear_address( Uint32 address );
void _vmem_clear_4MB_address( Uint32 address );

Uint32 _vmem_get_next_address(void);
Uint32 _vmem_get_next_reserve_address(void);
Uint32 _vmem_get_next_4MB_address(void);

void _vmem_address_calc( Uint32 address, Uint32* index, Uint8* index2 );
void _vmem_address_4MB_calc( Uint32 address, Uint32* i );
Uint32 _vmem_get_address( Uint32 index, Uint8 index2 );
Uint32 _vmem_get_4MB_address( Uint32 index); 


//assembly functions
void _vmem_turnon(Uint32 page);

int _vmem_getcr0(void);

int _vmem_bsf(Uint32 word);

#ifdef _VMEM_DEBUG
void _vmem_addresses_test(void);
void _vmem_address_test(Uint32 addr, Uint32 index, Uint8 index2);
void _vmem_address_4MB_test( Uint32 addr, Uint32 i );
#endif	
#endif
