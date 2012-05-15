#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"

//TODO: build a struct for the wm_memory
static win_man_vars*	wm_memory;
static screen_info* 	screen_info_arr;

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
	screen_info_arr = (screen_info *)( wm_memory + WIN_MAN_MEM );
	//begining of buffers
	bPtrOffset = (void *)(screen_info_arr + ( (DEFAULT_SCREENS+1) * sizeof( struct screen_info ) ) );
	//TODO: give the VM_man_our addresses
	//
	//
	//
	
	//get screen info
	dW = vga_mode_info->XResolution / 2;
	dH = vga_mode_info->YResolution / 2;
	
	//fill out the default screens [[ * vga_mode_info->LinbytesPerScanLine)/8) ]]
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		screen_info_arr[i].buf_num = i;
		screen_info_arr[i].w = dW;
		screen_info_arr[i].h = dH;
		screen_info_arr[i].bPtr = (Uint32 *)(bPtrOffset + (( i * dH * dW )));
		c_printf("%d  - %x || ", i, ( i * dH * dW) );
	}
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		c_printf("%d - (%d, %d) - %x || ", 
		screen_info_arr[i].buf_num, 
		screen_info_arr[i].w, 
		screen_info_arr[i].h, 
		screen_info_arr[i].bPtr);
	}
	//set the default displayed screens
	wm_memory->screens[0] = 0;
	wm_memory->screens[1] = 1;
	wm_memory->screens[2] = 2;
	wm_memory->screens[3] = 3;
}


Uint8 get_blocking( Uint32 buf_num ) {
	return screen_info_arr[buf_num].blocking;	
}

Uint8 set_blocking( Uint32 buf_num, Uint8 quadrant) {
	screen_info_arr[buf_num].blocking = quadrant;
	return screen_info_arr[buf_num].blocking;
}

//returns the active quadrant
Uint8 get_active( ) {
	return wm_memory->active_quad;
}

//Set quadrant to a buffer
Uint8 set_active( Uint32 buf_num, Uint8 quadrant) {
	if(quadrant < 4) {
		wm_memory->screens[quadrant] = buf_num;
	}else{ 
		//bad screen
		return -1;	
	}	
		
	screen_info_arr[buf_num].active = quadrant;
	return screen_info_arr[buf_num].active;
}

//returns the active quadrants pid
Pid get_active_pid( void ) {
	//grab the buffer number of the active quad
	Uint32 buf_num = wm_memory->screens[wm_memory->active_quad];
	return screen_info_arr[buf_num].pid;
}

//return the screen_info structs
screen_info* get_screen_infos( void ) {
	return (screen_info_arr);
}

//grabs the screens array
Uint32* get_current_bufs( void ) {
	return (wm_memory->screens);
}








