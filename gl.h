#ifndef _GL_H_
#define _GL_H_

#include "headers.h"

void _gl_init( void );

//main draw loop, copies from all active 
//buffers into the main lin buffer
void draw_active_screens( void );

#endif //_GL_H_
