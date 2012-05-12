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

#define TRUE    1
#define FALSE   0

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
	if( __get_end() >= PAGE_TABLE_SIZE - sizeof(Uint32) *2  ){
		__panic( "Houston we have a problem, the kernel is too fat");
	}

	_vmem_page_dir = (Uint32*)((__get_end() & PAGE_ADDRESS_LOC) + PAGE_SIZE);
	c_printf( "%x\n", (__get_end() & PAGE_ADDRESS_LOC) + PAGE_SIZE ) ;

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

	_vmem_page_dir[0] = (Uint32) pageTableStart | PAGE_DIR_READ | PAGE_DIR_PRESENT;
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
	
	_vmem_page_dir[1] = (Uint32) pageTableStart | PAGE_DIR_READ | PAGE_DIR_PRESENT;

	for( i = 0; i < (PAGE_TABLE_SIZE/32); i++ )
	{
		_vmem_bitmap_start[i] = 0;
	}

	for( i = 0; i < 0x400; i++)
	{
		_vmem_set_bit( _vmem_bitmap_start, i/32, i%32, 1);
	}

#ifdef _VMEM_DEBUG
	c_printf("Bitmap ending init addr: %x \n", _vmem_bitmap_start );
	c_printf(" %x : ", PAGE_TABLE_SIZE);
	c_printf(" %x : ", PAGE_SIZE);
	c_printf( "Bitmap table dir %x \n", pageTableStart);
	c_printf( "Ending of bitmap table is %x \n", addr);
	c_printf("0: %x \n", _vmem_page_dir[0] );
	c_printf("1: %x \n", _vmem_page_dir[1] );
	c_printf("3: %x %x\n", 0x34000, *((Uint32*)0x34000 ));
	c_printf("3: %x %x\n", 0x35000, *((Uint32*)0x35000 ));
	c_printf("3: %x %x\n", 0x36000, *((Uint32*)0x36000 ));
	c_printf("6: %x %x\n", 0x37000, *((Uint32*)0x37000 ));
	c_printf("7: %x %x\n", 0x400000, *((Uint32*)0x400000 ));
	c_printf( "Bitmap entry 0 is %x \n", _vmem_bitmap_start[0]);
	c_printf( "Bitmap entry 0 is %x \n", _vmem_read_bit( _vmem_bitmap_start, 0, 0));
	c_printf( "Bitmap entry 1 is %x \n", _vmem_read_bit( _vmem_bitmap_start, 0, 1));
	c_printf( "Bitmap entry 2 is %x \n", _vmem_read_bit( _vmem_bitmap_start, 0, 2));
	c_printf( "Bitmap entry n is %x \n", _vmem_read_bit( _vmem_bitmap_start, 31, 31));
	c_printf( "Bitmap entry n is %x \n", _vmem_read_bit( _vmem_bitmap_start, 32, 0));
#endif	
}

Int8 _vmem_read_bit( Uint32* address, Uint16 index, Int8 index2 )
{
	return ( address[index] >> index2 ) & 0x1;

}

void _vmem_set_bit( Uint32* address, Uint16 index, Int8 index2, Int8 value )
{
	address[index] |=  value << index2;
}

void _vmem_set_address( Uint32 address )
{
	Uint32 new = 0x0;
	new = address / 0x1000;
	Uint32 index = new / 32;
	Uint32 index2 = new % 32;
	c_printf( "Bitmap entry n is %x \n", 0x2000);
}
