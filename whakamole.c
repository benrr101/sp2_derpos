#include "win_man.h"
#include "sio.h"
#include "gl.h"
#include "whakamole.h"

static Uint8 mole_pos[ MOLE_COUNT ][ 2 ] = {
	{0, 0},
	{2, 2},
	{0, 1},
	{1, 2}, 
	{0, 0}
};


void whakamole_start( void ){
	
	pixel p;
	p.r = 0xFF;
	p.b = 0;
	p.g = 0;
	p.a = 0;

	// Render the field, init objects
	draw_box( MOLE_START_X, MOLE_START_Y, MOLE_END_X, MOLE_END_Y, 
			get_color( gl_BLUE ) );
	//draw_rect( 0, 0, 100, 100, p );

	// register with the mouse so we can get click data
	register_mouse_listener( mouse_h );

	// Start the main game-loop

	while( 1 ){

	}
}

void mouse_h( Uint8 x, Uint8 y, char left, char right, char middle ){
	//_sio_writes( "GOT A MOUSE CLICK OMFG IT WORKED IT WOOOOOOOOORKED!!!!\n", 30 );
}


