#ifndef _GL_H_
#define _GL_H_

#include "headers.h"

//#define GL_DEBUG

typedef struct pixel {
	Uint8 b;
	Uint8 g;
	Uint8 r;
	Uint8 a;
}  __attribute__ ((packed)) pixel ;

void _gl_init( void );

//main draw loop, copies from all active
//buffers into the main lin buffer
void draw_active_screens( void );

void draw_scr_0( void );
void draw_scr_1( void );
void draw_scr_2( void );
void draw_scr_3( void );

void draw_pixel(Uint32 x, Uint32 y, pixel p);
void draw_line(Uint32 x, Uint32 y, Uint32 x2, Uint32 y2, pixel p);
void set_pixel(Uint32 x, Uint32 y, pixel p, screen_info* curr_si);

//draw char, make the x component count down so that we can flip the characters.
void draw_character( char c, Uint32 x, Uint32 y, pixel p);
//string is easy, make a loop that is the strlen and then draw one bit line at a
//time using the outer loop for the character height and the inner loop as the
//character selector (a,b,c,d....etc)
void draw_string(char* str, Uint32 x, Uint32 y, pixel p);

#endif //_GL_H_
