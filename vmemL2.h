#ifndef _VMEML2_H
#define _VMEML2_H
#define _VMEML2_DEBUG

#include "headers.h"

//create pagedir
Uint32* _vmeml2_create_page_dir( void );

//create pagetable
Uint32* _vmeml2_create_page_table( Uint32* dir, Uint16* index, Uint32 vaddr );

//create page
void _vmeml2_create_page( Uint32* table, Uint32 vaddr );

//create 4MB page
Uint32* _vmeml2_create_4MB_page( Uint32* dir, Uint32 vaddr );

//release
void _vmeml2_release_page_dir( Uint32* dir );

void _vmeml2_static_dir_entry( Uint32 index);

Uint8 _vmeml2_is_empty_dir_entry( Uint32* dir, Uint32 index);

Uint8 _vmeml2_is_empty_page_entry( Uint32* table, Uint32 index);

void _vmeml2_change_page(Uint32 page);

void _vmeml2_test( void );

#endif
