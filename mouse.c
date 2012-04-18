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
	Uint test_resp = 0;

	// first, make sure the mouse has all the correct default setttings
	_ps2_write_mouse(PS2_M_RST);
	
	// make sure everything is a-OK
	test_resp = _ps2_read_mouse();
	if(test_resp != PS2_M_BATC){
		c_printf("Mouse did not pass BAT! Response: 0x%x\n", test_resp);
		return;
	}

	c_puts("BAT sucessfully passed and mouse now contains default settings!\n");

}

void _ps2_mouse_isr( int vec, int code ){


}

Uint _ps2_read_mouse(){	

	// We need to let the mouse know we want to read the next response
	c_printf("Letting Mouse know we are awaiting a new response...\n");
	__outb(PS2_STAT, PS2_M_SCOM);

	// Wait for the mouse to become ready
	_ps2_mouse_ready();
	return __inb(PS2_PORT);
}

void _ps2_write_mouse(char b){

	// vars
	Uint resp;

	_ps2_mouse_clear();

	// let the PS2 controller know we want to send a command to the mouse
	c_printf("Letting Mouse know we are sending a command...\n");
	__outb(PS2_STAT, PS2_M_SCOM);
	_ps2_mouse_clear();

	// Now, send the mouse our command
	c_printf("Sending Command: 0x%x...\n", ( b & 0xFF) );
	__outb(PS2_PORT, b);

	// Await ACK
	resp = _ps2_read_mouse();
	if(resp == PS2_M_ACK)
		c_printf("Command sent!\n");
	else
		c_printf("Strange response recieved, 0x%x???\n", resp);
}

void _ps2_mouse_clear( void ){
	
	// vars
	char b;

	c_printf("Waiting on Mouse to be clear...\n");
	// wait for the mouse to be clear for commands
	while( 1 ){
		b = __inb(PS2_STAT) & 0xFF;
		c_printf("CLEAR: Read a byte from Port 0x64, 0x%x...\n", b);
		if(b == 0x1C)
			return;
	}
}

void _ps2_mouse_ready( void ){
	
	// vars
	Uint b;

	c_printf("Waiting on Mouse to be ready...\n");
	// wait for the mouse to be ready for commands
	while( 1 ){
		b = __inb(PS2_STAT);
		c_printf("READY: Read a byte from Port 0x64, 0x%x...\n", b);
		if( b & (0x1 | 0x20) )
			return;
	}
}

