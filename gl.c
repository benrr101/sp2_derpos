#include "gl.h"
#include "win_man.h"
#include "vga_dr.h"

static screen_info* scrn_info_arr;
static pixel* video_mem_ptr;
static Uint32 buff_bytes_perline;
static Uint32 bytes_perline;

void _gl_init( void ) {
	scrn_info_arr = get_screen_infos( );
	video_mem_ptr = (pixel *)_vga_get_start_mem( );
	buff_bytes_perline = (vga_mode_info->LinbytesPerScanLine/2);
	bytes_perline = vga_mode_info->LinbytesPerScanLine;
}

void draw_active_screens() { 

	set_priority( PRIO_HIGH );
	
	while ( 1 ) {
		Uint32* 		displayed;
		pixel*  		buffer_ptr;
		screen_info* 	curr_si;
		int i = 0;
		int x = 0;
		int y = 0;
		int x_off = 0;
		int y_off = 0;
	
		displayed = get_current_bufs( );
	
		for(i = 0; i < 4; i++) {
			curr_si = &(scrn_info_arr[displayed[i]]);
			buffer_ptr = (pixel *) (curr_si->bPtr);
		
		
			x_off = curr_si->w * (i % 2);
			if( i == 2 || i == 3 )
				y_off = curr_si->h;
		
			//copy the buffer
			for(x = 0; x < curr_si->w; x++) {
				for(y = 0; y < curr_si->h; y++) {
					video_mem_ptr	[(	(y + y_off) * bytes_perline) + 	(x + x_off)] = 
					buffer_ptr		[( 	y * (buff_bytes_perline) ) + 	x];
				}
			}
		}
	}
}

void draw_scr_1() { 
	
	int x = 0;
	int y = 0;
	pixel p;
	
	while ( 1 ) {
		
		for(x = 0; x < 180; x++) {
			for(y = 0; y < 180; y++) {
				p.r = 0xc0;
				p.b = 0xf0;
				p.g = 0xee;
				draw_pixel(x, y, p);
			}
		}
		
		sleep(1000);
	}
}


void draw_pixel(Uint32 x, Uint32 y, pixel p) {
	pixel*  		buffer_ptr;
	screen_info* 	curr_si;
	curr_si = &(scrn_info_arr[0]);
	buffer_ptr = (pixel *) (curr_si->bPtr);
	buffer_ptr[( y * (buff_bytes_perline) ) + x] = p;
}








