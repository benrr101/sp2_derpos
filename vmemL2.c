#include "vmem.h"
#include "vmemL2.h"

static Uint32 _vmeml2_all_pages_tables[80];
static Uint8 _vmeml2_all_pages_tables_size = 0;

Uint32* _vmeml2_create_page_dir( void )
{
	Uint32 addrRaw = _vmem_get_next_reserve_address();
	_vmem_set_address( addrRaw );
	
	Uint32* addr = (Uint32*) addrRaw;

	int i;
	for(i = 0; i < 1024; i++ )
	{
		addr[i] = PAGE_DIR_WRITE;
	}

	for( i=0; i < _vmeml2_all_pages_tables_size; i++ )
	{
		addr[_vmeml2_all_pages_tables[i]] = _vmem_page_dir[_vmeml2_all_pages_tables[i]];
	}
	return addr;
}

Uint32* _vmeml2_create_page_table( Uint32* dir, Uint16 index )
{
	if( !_vmeml2_is_empty_dir_entry( dir, index) )
	{
		return 0x00;
	}
	
	Uint32 tableAddr = _vmem_get_next_reserve_address();
	_vmem_set_address(tableAddr);
	
	Uint32* tAddr = (Uint32*)tableAddr;
	int i;
	for( i=0; i < 1024; i++ )
	{
		tAddr[i] = PAGE_TABLE_WRITE;
	}

	tAddr[index] = tableAddr | PAGE_DIR_PRESENT | PAGE_DIR_WRITE | PAGE_DIR_SIZE;
	return tAddr;
}

void _vmeml2_create_page( Uint32* table, Uint16 index )
{
	if( !_vmeml2_is_empty_page_entry(table, index))
	{
		__panic("Page already maped");
	}

	Uint32 page = _vmem_get_next_address();
	_vmem_set_address(page);
	
	table[index] = page | PAGE_TABLE_WRITE | PAGE_TABLE_PRESENT;
}

Uint32* _vmeml2_create_4MB_page( Uint32* dir, Uint16 index )
{
	if( !_vmeml2_is_empty_dir_entry( dir, index) )
	{
		return 0x00;
	}

	Uint32 page = _vmem_get_next_4MB_address();
	_vmem_set_4MB_address(page);
	
	dir[index] = page | PAGE_DIR_PRESENT | PAGE_DIR_WRITE | PAGE_DIR_SIZE;
	return (Uint32*)page;
}

void _vmeml2_release_page_dir( Uint32* dir )
{
}

void _vmeml2_static_address( Uint32 addr1, Uint32 addr2, Uint8 mark)
{
	//c_printf( "|%x,%x,%x|", addr1, PAGE_TABLE_SIZE, (addr1 % PAGE_TABLE_SIZE));
	if( addr1 % PAGE_TABLE_SIZE != 0 ) 
	{
		__panic("Static address not aligned on 4MB boundary");
	}

	Uint32 tempAddr;
	Uint32 index;
	for( tempAddr = addr1; tempAddr < addr2 && tempAddr != 0; tempAddr = tempAddr + PAGE_TABLE_SIZE )
	{
		if( mark )
		{
			_vmem_set_4MB_address( tempAddr);
		}
		index = tempAddr / PAGE_TABLE_SIZE;
		_vmeml2_static_dir_entry( index);
		_vmem_page_dir[index] = tempAddr | PAGE_DIR_PRESENT | PAGE_DIR_WRITE | PAGE_DIR_SIZE;
		//c_printf("Index: %x Address: %x \n", index, tempAddr);
	}
}

void _vmeml2_static_dir_entry( Uint32 index)
{
		_vmeml2_all_pages_tables[_vmeml2_all_pages_tables_size] = index;
		_vmeml2_all_pages_tables_size++;
}

Uint8 _vmeml2_is_empty_dir_entry( Uint32* dir, Uint32 index)
{
	Uint32 present = dir[index] & PAGE_DIR_PRESENT;
	if( present == PAGE_DIR_PRESENT )
	{
		return TRUE;
	}
	return FALSE;
}

Uint8 _vmeml2_is_empty_page_entry( Uint32* table, Uint32 index)
{
	Uint32 present = table[index] & PAGE_TABLE_PRESENT;
	if( present == PAGE_TABLE_PRESENT )
	{
		return TRUE;
	}
	return FALSE;
}

void _vmeml2_init(void)
{
	_vmeml2_all_pages_tables[0] = 0;
	_vmeml2_all_pages_tables[1] = 1;
	_vmeml2_all_pages_tables_size = 2;
	_vmeml2_static_address(PAGE_RESERVE_AREA, 0xFFFFFFFF, FALSE);
	Uint32* dirTable = _vmeml2_create_page_dir(); 
//	_vmeml2_change_page( (Uint32) dirTable );
	
#ifdef _VMEM_DEBUG
	_vmeml2_static_address(0xD0000000, ( 0xD0000000 + PAGE_TABLE_SIZE ), TRUE );
	c_printf( "%x \n", _vmem_page_dir[0]); 
	c_printf( "%x \n", _vmem_page_dir[0x340]);
	c_printf("Info %x \n", *((Uint32*)0xD0000000) );
#endif
	c_puts(" vmeml2" );
}
