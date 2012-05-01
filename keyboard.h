#ifndef KEYBOARD_H
#define KEYBOARD_H

// Commands
// Source: http://wiki.osdev.org/PS2_Keyboard
#define PS2_K_LED	0xED

// Argument Data
#define PS2_K_SHFT	0x00
#define PS2_K_NUML	0x01
#define PS2_K_CAPS	0x02


// IRQs
#define PS2_K_VEC	0x09

void _ps2_keyboard_init();


#endif
