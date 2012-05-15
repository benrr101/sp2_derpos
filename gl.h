#ifndef _GL_H_
#define _GL_H_

#include "headers.h"

typedef struct pixel {
	Uint8 b;
	Uint8 g;
	Uint8 r;
} pixel;

void _gl_init( void );

//main draw loop, copies from all active 
//buffers into the main lin buffer
void draw_active_screens( void );

void draw_scr_1( void );

void draw_pixel(Uint32 x, Uint32 y, pixel p);

#endif //_GL_H_
