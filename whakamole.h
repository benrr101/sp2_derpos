#ifndef _WHAKAMOLE_H
#define _WHAKAMOLE_H

// Game Constants
#define MOLE_APPEAR_DELAY	750
#define MOLE_COUNT			5

// Game Draw Constants
#define MOLE_DIST			20
#define MOLE_ROWS			3
#define MOLE_COLS			3
#define MOLE_WIDTH			15
#define MOLE_HEIGHT			10
#define MOLE_START_X		50
#define MOLE_START_Y		30
#define MOLE_END_X			MOLE_START_X + (MOLE_ROWS * MOLE_WIDTH + MOLE_ROWS * MOLE_DIST)
#define MOLE_END_Y			MOLE_START_Y + (MOLE_COLS * MOLE_HEIGHT + MOLE_COLS * MOLE_DIST)


void whakamole_start( void );
void mouse_h( Uint8 x, Uint8 y, char left, char right, char middle);




#endif
