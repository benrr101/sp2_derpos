#include "gl.h"
#include "win_man.h"
#include "vga_dr.h"

static screen_info* scrn_info_arr;
static Uint32* video_mem_ptr;
static Uint32 buff_bytes_perline;
static Uint32 bytes_perline;

void _gl_init( void ) {
	scrn_info_arr = get_screen_infos( );
	video_mem_ptr = (Uint32 *)_vga_get_start_mem( );
	//buff_bytes_perline = (vga_mode_info->LinbytesPerScanLine/2);
	//bytes_perline = vga_mode_info->LinbytesPerScanLine;
}

void draw_active_screens() { 

	set_priority( PRIO_HIGH );
	
	while ( 1 ) {
		Uint32* 		displayed;
		Uint32*  		buffer_ptr;
		screen_info 	curr_si;
		int i = 0;
		int x = 0;
		int y = 0;
		int x_off = 0;
		int y_off = 0;
	
		displayed = get_current_bufs( );
	
		for(i = 0; i < 4; i++) {
			curr_si = (scrn_info_arr[displayed[i]]);
			write( 'd' );
			write( (char)displayed[i] );
			buffer_ptr = (Uint32 *) (curr_si.bPtr);
		
		
			//x_off = curr_si.w * (i % 2);
			if( i == 1 || i == 3 )
				x_off = curr_si.w;
			else
				x_off = 0;
			
			if( i == 2 || i == 3 )
				y_off = curr_si.h;
			else
				y_off = 0;
		
			//copy the buffer
			for(x = 0; x < curr_si.w; x++) {
				for(y = 0; y < curr_si.h; y++) {
					int pos1 = ((y + y_off) * vga_mode_info->XResolution) + (x + x_off);
					int pos2 = (y * curr_si.w) + x;
					
					video_mem_ptr	[pos1] = 
					buffer_ptr		[pos2];
				}
			}
		}
		sleep( 1 );
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
				p.a = 0x80;
				draw_pixel(x, y, p);
			}
		}
		
		sleep(1000);
	}
}

void draw_scr_2() { 
	
	int x = 0;
	int y = 0;
	pixel p;
	
	while ( 1 ) {
		
		for(x = 0; x < 180; x++) {
			for(y = 0; y < 180; y++) {
				p.r = 0xc0;
				p.b = 0xf0;
				p.g = 0xee;
				p.a = 0x80;
				draw_pixel(x, y, p);
			}
		}
		
		sleep(1000);
	}
}

void draw_scr_3() { 
	
	int x = 0;
	int y = 0;
	pixel p;
	
	while ( 1 ) {
		
		for(x = 0; x < 180; x++) {
			for(y = 0; y < 180; y++) {
				p.r = 0xc0;
				p.b = 0xf0;
				p.g = 0xee;
				p.a = 0x80;
				draw_pixel(x, y, p);
			}
		}
		
		sleep(1000);
	}
}


void draw_pixel(Uint32 x, Uint32 y, pixel p) {
	Uint32			pix = 0;
	Uint32*  		buffer_ptr;
	screen_info 	curr_si;
	curr_si = 		(scrn_info_arr[0]);
	buffer_ptr = 	(Uint32 *) (curr_si.bPtr);
	
	pix = p.r;
	pix = (pix << 8) | p.r;
	pix = (pix << 8) | p.g;
	pix = (pix << 8) | p.b;
	buffer_ptr[( y * (curr_si.w) ) + x] = pix;
}








