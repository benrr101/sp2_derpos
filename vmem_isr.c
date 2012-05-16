//aka falling down THREE flights of stairs
void _isr_general_protect( int vector, int code )
{
	c_printf("\n %x=%d %x=%d", vector, vector, code, code);
	__panic("MUST PROTECT");
}

void _isr_page_fault( int vector, int code )
{
	c_printf("\n %x=%d %x=%d", vector, vector, code, code);
	__panic("fault");
}

