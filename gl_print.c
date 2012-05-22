#include "gl_print.h"

static void _do_printf( int x, int y, char** f ) { 
	screen_info* curr_si;
	Pid pid = 0;
	Status s;

	s = get_pid( &pid );
	curr_si = ( get_screen_info( pid ) );
	
}
