#ifndef _WIN_MAN_H_
#define _WIN_MAN_H_

#include "headers.h"

// KB
#define WIN_MAN_MEM 		1024
#define DEFAULT_SCREENS		12

typedef struct screen_info
{
	Uint32	screen_num;
	Uint32 	w;
	Uint32	h;
	Uint32	bPtr;
}	screen_info;

void _win_man_init( void );

#endif //_WIN_MAN_H_
