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
	Uint32*	bPtr;
	Uint8	active;
	Uint8	blocking;
}	screen_info;

//to be 1024b
typedef struct win_man_vars
{
	Uint32	screens[4];
	char	reserved[992];
	//don't need to have the array here I just did t oshow the rest of the space
} 	win_man_vars;

/*
Screen positions
+------------+------------+
|            |            |
|     0      |      1     |
|            |            |
+------------+------------+
|            |            |
|     2      |      3     |
|            |            |
+------------+------------+

and there are 12 screens in the background
*/
void _win_man_init( void );

Uint8 get_blocking( Uint32 screen_num );
Uint8 set_blocking( Uint32 screen_num, Uint8 new_val);

//returns the position on the screen.
Uint8 get_active( Uint32 screen_num );
//pass into new val the position on the screen you want it at.
Uint8 set_active( Uint32 screen_num, Uint8 new_val);

#endif //_WIN_MAN_H_
