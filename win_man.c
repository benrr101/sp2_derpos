#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"

static win_man_vars*	wm_memory;
static screen_info* 	screen_info_arr;

/**
 * Initializes the gl library and the vga devices. Then initializes all of the 
 * memory required for the window manager, and sets all default values.
 * 
 */
void _win_man_init( void ) {
	int i = 0;
	int j = 0;
	void* bPtrOffset = 0;
	int dW, dH = 0;
	
	_vga_init();
	
	//setup the memory for the arrays and 
	// this module
	wm_memory = (void *)( _vga_get_end_mem() );
	//array of screen_infos
	screen_info_arr = (screen_info *)( wm_memory + WIN_MAN_MEM );
	
	_gl_init();
	
	//begining of buffers
	bPtrOffset = (void *)(screen_info_arr + ( (DEFAULT_SCREENS+1) * sizeof( struct screen_info ) ) );
	//TODO: give the VM_man_our addresses
	//
	//
	//
	
	//get screen info
	dW = vga_mode_info->XResolution / 2;
	dH = vga_mode_info->YResolution / 2;
	c_printf("info: %x -- %x \n\n", screen_info_arr, bPtrOffset );
	
	//fill out the default screens [[ * vga_mode_info->LinbytesPerScanLine)/8) ]]
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		screen_info_arr[i].buf_num = i;
		screen_info_arr[i].w = dW;
		screen_info_arr[i].h = dH;
		
		screen_info_arr[i].bPtr = (Uint32 *)( bPtrOffset + ( i * dH * dW ) );
		#ifdef WM_DEBUG
		c_printf("bufs: %d -- %x=%x \n", i, screen_info_arr[i].bPtr, (Uint32 *)( bPtrOffset + ( i * dH * dW ) ) );
		#endif
		
		screen_info_arr[i].pid = 0;
		screen_info_arr[i].active = 0;
		screen_info_arr[i].blocking = 0;
		
		#ifdef WM_DEBUG
		c_printf("%d  - %x || ", i, ( i * dH * dW) );
		#endif
	}
	#ifdef WM_DEBUG
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		c_printf("%d - (%d, %d) - %x || ", 
		screen_info_arr[i].buf_num, 
		screen_info_arr[i].w, 
		screen_info_arr[i].h, 
		screen_info_arr[i].bPtr);
	}
	#endif
	//set the default displayed screens
	wm_memory->screens[0] = 0;
	wm_memory->screens[1] = 1;
	wm_memory->screens[2] = 2;
	wm_memory->screens[3] = 3;
	
	//test
	for(i = 0; i < screen_info_arr[0].w; i++) {
		for(j = 0; j < screen_info_arr[0].h; j++) {
			screen_info_arr[0].bPtr[ ( j * screen_info_arr[0].w ) + i] = 0xee00ee00;
		}
	}
	for(i = 0; i < screen_info_arr[2].w; i++) {
		for(j = 0; j < screen_info_arr[2].h; j++) {
			screen_info_arr[2].bPtr[ ( j * screen_info_arr[2].w ) + i] = 0x00ee00ee;
		}
	}
}

/**
 * Gets the blocking flag for the specified buffer.
 * 
 * @param 	buf_num	The buffer number that is being checked for blocking.
 * @return			The current blocking flag.
 */
Uint8 get_blocking( Uint32 buf_num ) {
	return screen_info_arr[buf_num].blocking;	
}

/**
 * Sets the blocking flag for the specified buffer.
 * 
 * TODO: Rethink this function?
 * 
 * @param 	buf_num		The buffer number that is being set to blocking.
 * @param 	quadrant	The quadrant to set as the blocking flag.
 * @return				The blocking flag.
 */
Uint8 set_blocking( Uint32 buf_num, Uint8 quadrant) {
	screen_info_arr[buf_num].blocking = quadrant;
	return screen_info_arr[buf_num].blocking;
}

/**bPtrOffset
 * Grabs the currently active quadrant.
 * 
 * @return	The quadrant number.
 */
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

/**
 * Grabs the currently active screens pid.
 * 
 * @return	The pid of the active program.
 */
Pid get_active_pid( void ) {
	//grab the buffer number of the active quad
	Uint32 buf_num = wm_memory->screens[wm_memory->active_quad];
	return screen_info_arr[buf_num].pid;
}

/**
 * Grabs screen info array.
 * 
 * @return	The pointer to the screens info array.
 */
screen_info* get_screen_infos( void ) {
	return (screen_info_arr);
}

/**
 * Grabs the currently displayed screens array.
 * 
 * @return	The pointer to the array.
 */
Uint32* get_current_bufs( void ) {
	return (wm_memory->screens);
}

/**
 * Replaces the currently active quadrant with a process' buffer.
 * 
 * @param	buffer_num		The window (user process) to make active
 */

Uint8 replace_active( Uint32 buffer_num ) {
	return set_active( buffer_num, get_active() );
}

/**
 * Requests a screen buffer and reserves the buffer using the pid of 
 * process.
 * 
 * @param	pid		The pid of the user process makeing the request.
 * @return			SUCCESS if there was a free buffer.
 */
Status get_screen_buffer( Pid pid ) {
	int i = 0;
	Status ret = FAILURE;
	
	for( i = 0; i < DEFAULT_SCREENS; i++ ) {
		if( screen_info_arr[i].pid != 0 ) {
			screen_info_arr[i].pid = pid;
			c_printf("PID: %d reserved buffer %d. \n", pid, i);
			ret = SUCCESS;
		}
	}
	
	return ret;
}
//returns the screen info for the pid provided
/**
 * Requests a screen info struct of the pid provided.
 * 
 * @param	pid		The pid of the user process makeing the request.
 * @return			The pointer of the screen_info for the pid.
 */
screen_info* get_screen_info( Pid pid ) {
	int i = 0;
	
	for( i = 0; i < DEFAULT_SCREENS; i++ ) {
		if( screen_info_arr[i].pid == pid ) {
			return &(screen_info_arr[i]);
		}
	}
	return NULL;
}





