#include "vmem_ref.h"
#include "vmemL2.h"

static Uint32* ref = (Uint32*) REF_START;

void _vmem_ref_init( void )
{
	_vmeml2_static_address( REF_START, REF_END, TRUE);
	int i;
	for( i =0; i < REF_SIZE; i++ )
	{
		ref[i] = REF_EMPTY;	
	}
	
	c_puts( " vmem_ref"); 
#ifdef _VMEM_REF_DEBUG
	c_printf("Should be 0 %x\n", _vmem_ref_get_count(0x400000, TRUE));
	c_printf("Should be 0 %x\n", _vmem_ref_get_count(0x400000, FALSE));
	_vmem_ref_inc_count(0x400000, TRUE);
	c_printf("Should be 1 %x\n", _vmem_ref_get_count(0x400000, TRUE));
	c_printf("Should be 0 %x\n", _vmem_ref_get_count(0x400000, FALSE));
	_vmem_ref_inc_count(0x400000, TRUE);
	_vmem_ref_inc_count(0x400000, FALSE);
	c_printf("Should be 2 %x\n", _vmem_ref_get_count(0x400000, TRUE));
	c_printf("Should be 1 %x\n", _vmem_ref_get_count(0x400000, FALSE));
	_vmem_ref_dec_count(0x400000, TRUE);
	c_printf("Should be 1 %x\n", _vmem_ref_get_count(0x400000, TRUE));
	_vmem_ref_dec_count(0x400000, TRUE);
	c_printf("Should be 0 %x\n", _vmem_ref_get_count(0x400000, TRUE));

	for( ;; )
	{
		c_printf("Count %d\n", _vmem_ref_get_count(0x400000, TRUE));
		_vmem_ref_inc_count(0x400000, TRUE);
	}
#endif
}

Uint32 _vmem_ref_get_count( Uint32 address, Uint8 is4Mb )
{
	Uint32 addr;
	Uint32 temp;
	Uint32 mb;
	int i;
	for( i =0; i < REF_SIZE; i++ )
	{
		if( ref[i] == 0 )
		{
			continue;
		}
		temp = ref[i];
		addr = temp & REF_ADDRESS;
		mb = temp & REF_4MB;

		if( addr == address && mb == is4Mb )
		{
			return (temp & REF_COUNT) >> 2;
		}

	}
	return 0x00;
}

void _vmem_ref_inc_count( Uint32 address, Uint8 is4Mb )
{
	Uint32 addr;
	Uint32 temp;
	Uint32 mb;
	Uint32 value;
	int i;
	for( i =0; i < REF_SIZE; i++ )
	{
		if( ref[i] == 0 )
		{
			continue;
		}
		
#ifdef _VMEM_REF_DEBUG
		c_printf("Found non-empty index %d %x \n", i, ref[i] );
#endif
		temp = ref[i];
		addr = temp & REF_ADDRESS;
		mb = temp & REF_4MB;

		if( addr == address && mb == is4Mb )
		{
			value = temp & REF_COUNT;
			if( value == REF_COUNT )
			{
				__panic("Vmem Ref page shared to many times");
			}				

			value = value + REF_COUNT_ADD;

			ref[i] = addr | value | REF_VALID | mb;
			
			return; 
		}

	}
	
	for( i =0; i < REF_SIZE; i++ )
	{
		if( ref[i] != 0 )
		{
			continue;
		}
		
		ref[i] = ( address & REF_ADDRESS) | (is4Mb & REF_4MB) | REF_COUNT_ADD | REF_VALID;
#ifdef _VMEM_REF_DEBUG
		c_printf("Make new entry for addr %x %x\n", address, ref[i] );
#endif
		return;
	}
	
	__panic( "Vmem Ref out of places for shared pages");
}

void _vmem_ref_dec_count( Uint32 address, Uint8 is4Mb )
{
	Uint32 addr;
	Uint32 temp;
	Uint32 mb;
	Uint32 value;
	int i;
	for( i =0; i < REF_SIZE; i++ )
	{
		if( ref[i] == 0 )
		{
			continue;
		}
		temp = ref[i];
		addr = temp & REF_ADDRESS;
		mb = temp & REF_4MB;

		if( addr == address && mb == is4Mb )
		{
			value = temp & REF_COUNT;
			if( value == REF_COUNT_ADD )
			{	
#ifdef _VMEM_REF_DEBUG
				c_printf("Index %d was delted\n", i);
#endif
				ref[i] = REF_EMPTY;
				return;
			}
			
#ifdef _VMEM_REF_DEBUG
			c_printf("Vmem ref %d %d %d\n", value >> 2, ( value >>2) -1, (( value >>2) -1) <<2 );
#endif
			value = value >> 2;
			value--;
			value = value << 2;
			
			ref[i] = addr | (value & REF_COUNT ) | REF_VALID | mb;

			return; 
		}
	}
	__panic( "Vmem no page to decrement");

}

