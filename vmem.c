/*
** File:	vmem.c
**
** Author:	Sean McDermott
**
** Contributor:
**
** Description:
*/

#include "startup.h"
#include "vmem.h"


/*
** _vmem_init()
**
*/
void _vmem_init( void )
{
	c_printf( "\n\nEnd is at position %x \n", __get_end() );
	//strip the last 12 bytes then increment by page to get next valid page
	_vmem_page_dir = (Uint32*)(( ( __get_end() >> ( PAGE_BITS -1) ) << ( PAGE_BITS - 1) ) + PAGE_SIZE);

	c_printf( "Aligned address is %x \n", _vmem_page_dir);

	int i;
	for(i = 0; i < 1024; i++ )
	{
		_vmem_page_dir[i] = PAGE_DIR_READ;
	}

	Uint32* pageTableStart = (Uint32*) ( (Uint32)_vmem_page_dir + PAGE_SIZE);
	c_printf( "First page table address is %x \n", pageTableStart);

	//should check how much to idenity map

	//setting up and idenity page
	Uint32 address = 0;
	for( i = 0; i < 1024; i++ )
	{
		pageTableStart[i] = address | PAGE_TABLE_READ | PAGE_TABLE_PRESENT;
		address = address + PAGE_SIZE;
	}

	_vmem_page_dir[0] = (int) pageTableStart | PAGE_DIR_READ | PAGE_DIR_PRESENT;
	_vmem_turnon((Uint32)_vmem_page_dir);

	c_printf( "Cr0 is %x \n", _vmem_getcr0());
	

}

