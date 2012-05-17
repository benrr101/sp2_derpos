#include "vmem_isr.h"

//aka falling down THREE flights of stairs
void _isr_vmem_general_protect( int vector, int code )
{
	c_printf("\n %x=%d %x=%d", vector, vector, code, code);
	__panic("MUST PROTECT");
}

void _isr_vmem_page_fault( int vector, int code )
{
	c_printf("\n %x=%d %x=%d\n", vector, vector, code, code);
	c_printf("Fautly address %x\n", _isr_vmem_getcr2() );
	__panic("fault");
}

