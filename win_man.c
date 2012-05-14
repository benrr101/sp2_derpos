#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"

//TODO: build a struct for the wm_memory
static void* 			wm_memory;
static screen_info* 	screen_arr;

void _win_man_init( void ) {
	int i = 0;
	void* bPtrOffset = 0;
	int dW, dH = 0;
	
	_vga_init();
	_gl_init();	
	
	//setup the memory for the arrays and 
	// this module
	wm_memory = (void *)( _vga_get_end_mem() );
	//array of screen_infos
	screen_arr = (screen_info *)( wm_memory + WIN_MAN_MEM );
	//begining of buffers
	bPtrOffset = (void *)(screen_arr + ( (DEFAULT_SCREENS+1) * sizeof( struct screen_info ) ) );
	//get screen info
	dW = vga_mode_info->XResolution / 2;
	dH = vga_mode_info->YResolution / 2;
	
	//fill out the default screens [[ * vga_mode_info->LinbytesPerScanLine)/8) ]]
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		screen_arr[i].screen_num = i;
		screen_arr[i].w = dW;
		screen_arr[i].h = dH;
		screen_arr[i].bPtr = (Uint32 *)(bPtrOffset + (( i * dH * dW )));
		c_printf("%d  - %x || ", i, ( i * dH * dW) );
	}
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		c_printf("%d - (%d, %d) - %x || ", 
		screen_arr[i].screen_num, 
		screen_arr[i].w, 
		screen_arr[i].h, 
		screen_arr[i].bPtr);
	}
}

Uint8 get_blocking( Uint32 screen_num ) {
	return screen_arr[screen_num].blocking;	
}

Uint8 set_blocking( Uint32 screen_num, Uint8 new_val) {
	screen_arr[screen_num].blocking = new_val;
	return screen_arr[screen_num].blocking;
}

//returns the position on the screen.
Uint8 get_active( Uint32 screen_num ) {
	return screen_arr[screen_num].active;
}

//pass into new val the position on the screen you want it at.
Uint8 set_active( Uint32 screen_num, Uint8 new_val) {
	if(new_val < 4) {
		((win_man_vars *) wm_memory)->screens[new_val] = screen_num;
	}		
		
	screen_arr[screen_num].active = new_val;
	return screen_arr[screen_num].active;
}
