#include "win_man.h"
#include "vga_dr.h"
#include "gl.h"

//TODO: build a struct for the wm_memory
static void* 			wm_memory;
static screen_info* 	screen_arr[DEFAULT_SCREENS];

void _win_man_init( void ) {
	int i = 0;
	void* bPtrOffset = 0;
	_vga_init();
	_gl_init();
	
	//setup the memory for the arrays and 
	// this module
	wm_memory = (void*)(_vga_get_end_mem());
	screen_info = (screen_info*)(((Uint32) wm_memory) + WIN_MAN_MEM);
	bPtrOffset = (void*)( ( screen_info ) + ( DEFAULT_SCREENS * sizeof( screen_info ) ) );
	
	//fill out the default screens
	//TODO: bPtrOffset + (H * W * byte size of each pixel)
	for(i = 0; i < DEFAULT_SCREENS; i++) {
		screen_info[i].screen_num = i;
		screen_info[i].w = 0;
		screen_info[i].h = 0;
		screen_info[i].bPtr = 0;
	}
}
