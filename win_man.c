#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"

//TODO: build a struct for the wm_memory
static void* 			wm_memory;
static screen_info* 	win_bufs;

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
	win_bufs = (screen_info *)( wm_memory + WIN_MAN_MEM );
	//begining of buffers
	bPtrOffset = (void *)(win_bufs + ( (DEFAULT_SCREENS+1) * sizeof( struct screen_info ) ) );
	//get screen info
	dW = vga_mode_info->XResolution / 2;
	dH = vga_mode_info->YResolution / 2;
	
	//fill out the default screens [[ * vga_mode_info->LinbytesPerScanLine)/8) ]]
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		win_bufs[i].screen_num = i;
		win_bufs[i].w = dW;
		win_bufs[i].h = dH;
		win_bufs[i].bPtr = (Uint32 *)(bPtrOffset + (( i * dH * dW )));
		c_printf("%d  - %x || ", i, ( i * dH * dW) );
	}
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		c_printf("%d - (%d, %d) - %x || ", 
		win_bufs[i].screen_num, 
		win_bufs[i].w, 
		win_bufs[i].h, 
		win_bufs[i].bPtr);
	}
}

Uint8 get_blocking( Uint32 screen_num ) {
	return win_bufs[screen_num].blocking;	
}

Uint8 set_blocking( Uint32 screen_num, Uint8 new_val) {
	win_bufs[screen_num].blocking = new_val;
	return win_bufs[screen_num].blocking;
}

//returns the position on the screen.
Uint8 get_active( Uint32 screen_num ) {
	return win_bufs[screen_num].active;
}

/**
 * Draws a given process' buffer to a given quadrant.
 *
 * @param	screen_num		The window (user process) to make active
 * @param	quadrant		The quadrant to draw the process' buffer to
 * @return					The quadrant of the active buffer
 */
Uint8 set_active( Uint32 buffer_num, Uint8 quadrant ) {

	// only offset the buffer-quadrant if it is a valid value
	if(quadrant < 4) {
		((win_man_vars *) wm_memory)->screens[quadrant] = buffer_num;
	}		
		
	win_bufs[screen_num].active = quadrant;
	return win_bufs[screen_num].active;
}

/**
 * Replaces the currently active quadrant with a process' buffer.
 * 
 * @param	buffer_num		The window (user process) to make active
 */
Uint8 replace_active( Uint32 buffer_num ){
	return set_active( buffer_num, get_active() );
}
