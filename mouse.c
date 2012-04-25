/**
 *
 *
 */

#include "headers.h"
#include "startup.h"
#include "mouse.h"


void _ps2_init( void ){
	
	// Check if the mouse is there
	_ps2_mouse_init();
}

void _ps2_mouse_init( void ){
	
	// vars
	Uint resp = 0;

	/*
	_ps2_write_mouse(PS2_M_RST);
	resp = _ps2_read_mouse() | 2;
	_ps2_debug( "RESET", resp );
	resp = _ps2_read_mouse() | 2;
	_ps2_debug( "RESET 2", resp );
	*/

	// Make sure the aux port is enabled
	/*_ps2_write_mouse(PS2_M_ENAB);
	resp = _ps2_read_mouse();
	_ps2_debug("ENAB", resp);*/
	/*resp = _ps2_read_mouse();
	if( resp != PS2_M_ACK){
		_ps2_nonack(resp);
		return;
	}
	*/

	_ps2_mouse_clear();
	__outb( 0x64, 0xA8 );

	//Enable the interrupts
	_ps2_mouse_clear();
	__outb( 0x64, 0x20 );
	_ps2_mouse_ready();
	resp = ( __inb(0x60) | 2 );
	_ps2_mouse_clear();
	__outb( 0x64, 0x60 );
	_ps2_mouse_clear();
	__outb( 0x60, resp );

	//Tell the mouse to use default settings
	_ps2_write_mouse( 0xF6 );
	_ps2_read_mouse();  //Acknowledge

	//Enable the mouse
	_ps2_write_mouse( 0xF4 );
	_ps2_read_mouse();  //Acknowledge

	// Finally, hook in our interrupt vector
	__install_isr( PS2_M_VEC, _ps2_mouse_isr );

	c_puts( "Mouse driver successfully attached!\n" );
}

void _ps2_mouse_isr( int vec, int code ){
	//c_printf("Got an interrupt! Code: 0x%x\n", code);
	static int byte_c = 0;
	static char m_bytes[3];
	m_bytes[byte_c++] = __inb(PS2_PORT);
	//c_printf( "Byte 0x%x\n", byte_c );

	// Only dump information once we have everything the mouse sent
	if(byte_c == 3){
		c_printf( "Overflow Byte: 0x%x\n", m_bytes[0] );
		c_printf( "X Offset Byte: 0x%x\n", m_bytes[1] );
		c_printf( "Y Offset Byte: 0x%x\n", m_bytes[2] );
		byte_c = 0;
	}
}

Uint _ps2_read_mouse(){	

	Uint ret_char;

	// Wait for the mouse to become ready
	_ps2_mouse_ready();
	ret_char = __inb(PS2_PORT);

	return ret_char;
}

void _ps2_write_mouse(char b){

	// vars
	//Uint resp;

	_ps2_mouse_clear();

	// let the PS2 controller know we want to send a command to the mouse
	c_printf("Letting Mouse know we are sending a command...\n");
	__outb(PS2_STAT, PS2_M_SCOM);
	_ps2_mouse_clear();

	// Now, send the mouse our command
	c_printf("Sending Command: 0x%x...\n", ( b & 0xFF) );
	__outb(PS2_PORT, b);
}

void _ps2_mouse_clear( void ){
	
	// vars
	char b;

	c_printf("Waiting on Mouse to be clear...\n");
	// wait for the mouse to be clear for commands
	while( 1 ){
		b = __inb(PS2_STAT);
		//c_printf("CLEAR: Read a byte from Port 0x64, 0x%x...\n", b);
		if( (b & 2) == 0 )
			return;
	}
}

Uint _ps2_mouse_ready( void ){
	
	// vars
	Uint b;

	c_printf("Waiting on Mouse to be ready...\n");
	// wait for the mouse to be ready for commands
	while( 1 ){
		b = __inb(PS2_STAT);
		//c_printf("READY: Read a byte from Port 0x64, 0x%x...\n", b);
		if( (b & 1) == 1 )
			return b;
	}
}

void _ps2_setup_isr( void ){
/*	
	_ps2_write_mouse(PS2_M_CMPQ);
	_ps2_write
	__install_isr(PS2_M_VEC, _ps2_mouse_isr);
*/
}

void _ps2_nonack( Uint resp ){
	c_printf( "Did not recieve ACK (0xFF) from MOUSE, instead got: '0x%x'.\n",
			resp );
	__panic( "YOU SHALL NOT CYCLE!!!\n" );
}

void _ps2_debug( const char * comp, Uint resp ){
	c_printf("DEBUG: %s, RCVD: '0x%x'\n", comp, resp);
}

