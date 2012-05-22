#include "headers.h"
#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"
#include "c_io.h"
#include "vmemL2.h"

//TODO: build a struct for the wm_memory
static win_man_vars*	wm_memory;
static screen_info* 	screen_info_arr;

// Mouse Data
static Uint8 abs_cursor_x = 0;
static Uint8 abs_cursor_y = 0;

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
	wm_memory = (void *)(0x20000000 );
	//array of screen_infos
	screen_info_arr = (screen_info *)( wm_memory + WIN_MAN_MEM );
	
	
	//begining of buffers
	bPtrOffset = (void *)(screen_info_arr + ( (DEFAULT_SCREENS+1) * sizeof( struct screen_info ) ) );
	_vmeml2_static_address( (Uint32) _vga_get_start_mem(), (Uint32) _vga_get_end_mem(), TRUE );
	_vmeml2_static_address( ( (Uint32) wm_memory ), (Uint32) ( (Uint32) wm_memory ) + ( DEFAULT_SCREENS * dH * dW * 4 ), TRUE ); 
	//TODO: give the VM_man_our addresses
	//
	//
	//

	_gl_init();
	
	//get screen info
	dW = vga_mode_info->XResolution / 2;
	dH = vga_mode_info->YResolution / 2;
	
	//fill out the default screens [[ * vga_mode_info->LinbytesPerScanLine)/8) ]]
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		screen_info_arr[i].buf_num = i;
		screen_info_arr[i].w = dW;
		screen_info_arr[i].h = dH;
		screen_info_arr[i].bPtr = (Uint32 *)(bPtrOffset + (( i * dH * dW * 4 )));
		screen_info_arr[i].pid = 0;
		screen_info_arr[i].active = 0;
		screen_info_arr[i].blocking = 0;
		
		#ifdef WM_DEBUG
		c_printf("%d  - %x || ", i, ( i * dH * dW * 4) );
		#endif
	}
	//clear buffer mem
	_kmemclr(screen_info_arr[0].bPtr, ( DEFAULT_SCREENS * dH * dW * 4 ) );
	
	#ifdef WM_DEBUG
	c_printf("\n-Total Memory(B): %d blocks(x64KB), %d\n", vga_vesa_info->TotalMemory, (vga_vesa_info->TotalMemory * 64)*1024);
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
			screen_info_arr[2].bPtr[ ( j * screen_info_arr[2].w ) + i] = 0xffffffff;
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
		if( screen_info_arr[i].pid == 0 && screen_info_arr[i].pid != pid ) {
			screen_info_arr[i].pid = pid;
			for(i = 0; i < 12; i++) { 
				write('A'+i);
				write(':');
				write(' ');
				write_x(screen_info_arr[i].pid);
				write(' ');
			}
			write('\n');
			write('\r');
			
			#ifdef WM_DEBUG
			c_printf("PID: %d reserved buffer %d. \n", pid, i);
			#endif
			ret = SUCCESS;
			return ret;
		}
	}
	
	return ret;
}

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


