/**
 *
 *
 */

#include "headers.h"
#include "mouse.h"


void _ps2_init( void ){
	
	// Check if the mouse is there
	_ps2_mouse_init();
}

void _ps2_mouse_init( void ){
	
	// vars
	char test_resp;

	// first, make sure the mouse has all the correct default setttings
	_ps2_write(PS2_M_RST);
	
	// make sure everything is a-OK
	test_resp = _ps2_read();
	if(test_resp != PS2_M_BATC){
		c_puts("Mouse did not pass BAT! Mouse initialization disabled.\n");
		return;
	}

}

void _ps2_mouse_isr( int vec, int code ){


}

char _ps2_read(){
	
}

void _ps2_write(char b){
	
}

