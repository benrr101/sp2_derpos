#ifndef KEYBOARD_H
#define KEYBOARD_H

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


// IRQs
#define PS2_K_VEC	0x09

void _ps2_keyboard_init( void );
void _ps2_keyboard_ready( void );
void _ps2_keyboard_clear( void );
void _ps2_keyboard_isr( int vec, int code );
Uint _ps2_keyboard_read( void );
void _ps2_keyboard_write( Uint command );


#endif
