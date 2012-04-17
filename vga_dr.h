/*
** VGA Driver initialization code
*/

#ifndef _VGA_H
#define _VGA_H

#include "headers.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __ASM__20113__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

/*
** _vga_init()
**
** initialize the VGA driver module
*/

void _vga_init( void );

void vga256( void );

void draw( void );
void draw2( void );

void back2text( void );

#endif // ACM

#endif // VGA
