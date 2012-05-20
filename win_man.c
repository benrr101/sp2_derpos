#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"

//TODO: build a struct for the wm_memory
static win_man_vars*	wm_memory;
static screen_info* 	screen_info_arr;

// Mouse Data
static Uint8 abs_cursor_x = 0;
static Uint8 abs_cursor_y = 0;

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

/**
 * Draws a given process' buffer to a given quadrant.
 *
 * @param	screen_num		The window (user process) to make active
 * @param	quadrant		The quadrant to draw the process' buffer to
 * @return					The quadrant of the active buffer
 */
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

/**
 * Replaces the currently active quadrant with a process' buffer.
 * 
 * @param	buffer_num		The window (user process) to make active
 */
Uint8 replace_active( Uint32 buffer_num ){
	return set_active( buffer_num, get_active() );
}

Uint8 switch_active( Uint8 quadrant ){
	Uint8 prev_quad = wm_memory->active_quad; 
	if( quadrant < 4 )
		wm_memory->active_quad = quadrant;
	else
		prev_quad = -1;
	return prev_quad;
}

/**
 * Updates the window managers current cursor position.
 * Note, in the future this function should be updated to redraw the cursor
 * as well.
 * 
 * @param	x_pos		The new x-coordinate of the mouse pointer, in character widths.
 * @param	y_pos		The new y-coordinate of the mouse_pointer, in character widths.
 */
void update_cursor_pos( Uint8 x_pos, Uint8 y_pos ){
	abs_cursor_x = x_pos;
	abs_cursor_y = y_pos;
}

/**
 * Any mouse button changes are passed to the active user program. Note, if the
 * user did not register a mouse handler call-back, then the mouse data is
 * ignored.
 *
 * @param	left		1 if the left mouse button is held down, otherwise 0.
 * @param	right		1 if the right mouse button is held down, otherwise 0.
 * @param	middle		1 if the middle mouse button is held down, otherwise 0.
 */
void update_mouse_button( char left, char right, char middle ){
	
	// temp vars
	Uint8 x = abs_cursor_x, y = abs_cursor_y;

	// translate coordinates to relative window coordinates
	// TODO: Need to determine actual screen width values
	x = x % 120;
	y = y % 60;

	// call the active screen's callback with the updated mouse data
	// Note: the below callback is a huge security vulnerability, but not much
	// we can do without signals or resorting to user programs polling for
	// mouse data
	mouse_handler mh = screen_info_arr[ wm_memory->active_quad ].handler;
	if( mh != 0 )
		mh( x, y, left, right, middle );
}


