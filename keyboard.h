#ifndef KEYBOARD_H_
#define KEYBOARD_H_

// Debug Print controls
#define DEBUG_G


// Commands
// Source: http://wiki.osdev.org/PS2_Keyboard
#define PS2_K_LED	0xED
#define PS2_K_TYPD	0xF3
#define PS2_K_SET_TYPEMATIC_DELAY	PS2_K_TYPD

// Argument Data
// LED Light Control
#define PS2_K_SCRL	0x01
#define PS2_K_NUML	0x02
#define PS2_K_CAPS	0x04

// Control Flags
#define PS2_K_IBH	0x10
#define PS2_K_Inhibited		PS2_K_IBH

// Special Keys
#define PS2_KEY_RELEASE		0x80
#define PS2_KEY_LSHIFT_P	0x2A
#define PS2_KEY_RSHIFT_P	0x36
#define PS2_KEY_LSHIFT_R	PS2_KEY_LSHIFT_P + PS2_KEY_RELEASE
#define PS2_KEY_RSHIFT_R	PS2_KEY_RSHIFT_P + PS2_KEY_RELEASE
#define PS2_KEY_CAPLCK_P	0x3A
#define PS2_KEY_NUMLCK_P	0x45
#define PS2_KEY_SROLCK_P	0x46
#define PS2_KEY_CAPLCK_R	PS2_KEY_CAPLCK_P + PS2_KEY_RELEASE
#define PS2_KEY_NUMLCK_R	PS2_KEY_NUMLCK_P + PS2_KEY_RELEASE
#define PS2_KEY_SROLCK_R	PS2_KEY_SROLCK_P + PS2_KEY_RELEASE

// IRQs
#define PS2_K_VEC	0x09

void _ps2_keyboard_init( void );
void _ps2_keyboard_ready( void );
void _ps2_keyboard_clear( void );
void _ps2_keyboard_isr( int vec, int code );
Uint _ps2_keyboard_read( void );
void _ps2_keyboard_write( Uint command );


#endif
