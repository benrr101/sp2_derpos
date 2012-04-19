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

#include "headers.h"

#define PAGE_BITS 12
#define PAGE_SIZE (1 << PAGE_BITS )

#define PAGE_DIR_ADDRESS 0xffff800
#define PAGE_DIR_RESERV 0x600
#define PAGE_DIR_IGNORE 0x100
#define PAGE_DIR_SIZE 0x80
#define PAGE_DIR_ZERO 0x40
#define PAGE_DIR_ACCESS 0x20
#define PAGE_DIR_CACHE_DISABLE 0x10
#define PAGE_DIR_WRITE_THROUGH 0x8
#define PAGE_DIR_USER_MODE 0x4
#define PAGE_DIR_READ 0x2
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
#define PAGE_TABLE_READ 0x2
#define PAGE_TABLE_PRESENT 0x1

static Uint32* _vmem_page_dir = (Uint32*)0xFFFFFFFF;

/*
** _vmem_init()
**
*/

void _vmem_init( void );

void _vmem_turnon(Uint32 page);

int _vmem_getcr0(void);

#endif
