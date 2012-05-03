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
	Uint32 address = _vmem_first_4mb();
	_vmem_init_bitmap( address );
}

Uint32 _vmem_first_4mb( void )
{
	//strip the last 12 bytes then increment by page to get next valid page
	_vmem_page_dir = (Uint32*)(( ( __get_end() >> ( PAGE_BITS -1) ) << ( PAGE_BITS - 1) ) + PAGE_SIZE);

	int i;
	for(i = 0; i < 1024; i++ )
	{
		_vmem_page_dir[i] = PAGE_DIR_READ;
	}

	Uint32* pageTableStart = (Uint32*) ( (Uint32)_vmem_page_dir + PAGE_SIZE);

	//maps the first 4 MBs 0x400000
	//setting up and idenity page
	Uint32 address = 0;
	for( i = 0; i < 1024; i++ )
	{
		pageTableStart[i] = address | PAGE_TABLE_READ | PAGE_TABLE_PRESENT;
		address = address + PAGE_SIZE;
	}

	_vmem_page_dir[0] = (int) pageTableStart | PAGE_DIR_READ | PAGE_DIR_PRESENT;
	_vmem_turnon((Uint32)_vmem_page_dir);

#ifdef _VMEM_DEBUG
	c_printf( "\n\nEnd is at position %x \n", __get_end() );
	c_printf( "Aligned address is %x \n", _vmem_page_dir);
	c_printf( "First page table address is %x \n", pageTableStart);
	c_printf( "Ending of identify map is %x \n", address);
	c_printf( "Cr0 is %x \n", _vmem_getcr0());
	c_printf( "Cr0 High is %x \n", ( (_vmem_getcr0()>>31) & 0x01));
#endif	
	return address;
}

void _vmem_init_bitmap( Uint32 addr )
{
	if ( addr != PAGE_TABLE_SIZE )
	{
		__panic( "vmem: bitmap start is not where expected" );
	}

	Uint32* pageTableStart = (Uint32*) ( (Uint32)_vmem_page_dir + PAGE_SIZE + PAGE_SIZE);

	//maps the next 4 MBs
	//setting up and idenity page
	int i = 0;
	for( i = 0; i < 1024; i++ )
	{
		pageTableStart[i] = addr | PAGE_TABLE_READ | PAGE_TABLE_PRESENT;
		addr = addr + PAGE_SIZE;
	}
	
	_vmem_page_dir[1] = (int) pageTableStart | PAGE_DIR_READ | PAGE_DIR_PRESENT;

	for( i = 0; i < 1024; i++ ){
		_vmem_bitmap_start[i] = 0;
	}

	_vmem_bitmap_start[0] = 0xC0000000;

#ifdef _VMEM_DEBUG
	c_printf(" %x : ", PAGE_TABLE_SIZE);
	c_printf(" %x : ", PAGE_SIZE);
	c_printf( "Bitmap table dir %x \n", pageTableStart);
	c_printf( "Ending of bitmap table is %x \n", addr);
	c_printf( "Bitmap entry 0 is %x \n", _vmem_bitmap_start[0]);
	c_printf( "Bitmap entry 1 is %x \n", _vmem_bitmap_start[1]);
#endif	
}
