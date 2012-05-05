/**
 * File:	keyboard.c
 * Author:	Grant Kurtz
 *
 * Description:
 *		Handles processing PS/2 Keyboard input.  This module specifically only
 *		deals with buffered-blocking input.
 *
 */

#include "headers.h"
#include "ps2.h"
#include "startup.h"
#include "keyboard.h"

void _ps2_keyboard_init( void ){

	
	// install our ISR
	__install_isr( PS2_K_VEC, _ps2_keyboard_isr );

	// Disable the Keyboard Interface
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0xF5 );
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0xA7 );
	
	// clear the output buffer
	__inb( PS2_PORT );


	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0xAE );
	
	// Enable Interrupts
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0x20 );
	_ps2_keyboard_ready();
	Uint resp = ( __inb(0x60) | 1 );
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0x60 );
	_ps2_keyboard_clear();
	__outb( 0x60, resp );

	/*
	// Set delay
	_ps2_keyboard_clear();
	__outb( PS2_STAT, PS2_K_TYPD );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	_ps2_keyboard_ready();
	__outb( PS2_PORT, 20 );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	*/
	
	_ps2_keyboard_write( 0xF6 );
	_ps2_keyboard_read();  //Acknowledge

	// Enable
	_ps2_keyboard_clear();
	while( 1 )
		;
	__outb( PS2_STAT, 0xF4 );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	

	// Set Typematic Delay
	/*
	_ps2_keyboard_clear();
	__outb( PS2_STAT, PS2_K_TYPD );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	_ps2_keyboard_ready();
	__outb( PS2_PORT, 20 );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	*/

	// reset the keyboard
	/*
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0xFF );
	_ps2_keyboard_ready();
	c_printf("RCVD: 0x%x\n", __inb( PS2_PORT ));
	*/



	/*	
	// Try turning on the caps-lock light
	_ps2_keyboard_clear();
	__outb( PS2_K_LED, PS2_K_CAPS );
	c_puts( "Waiting for argument buffer to be clear...\n" );
	_ps2_keyboard_ready();
	c_printf("RCVD: 0x%x\n",__inb( PS2_PORT )); // Acknowledge
	_ps2_keyboard_ready();
	__outb( PS2_PORT, PS2_K_CAPS | PS2_K_NUML | PS2_K_SCRL );
	*/
}

void _ps2_keyboard_ready( void ){
	while( (__inb(PS2_STAT) & 0x1) == 1 )
		;
}

Uint _ps2_keyboard_read( void ){
	
	_ps2_keyboard_ready();
	return __inb( PS2_PORT );
}

void _ps2_keyboard_write( Uint command ){
	
	_ps2_keyboard_clear();

	// let the PS2 controller know we want to send a command to the mouse
	//c_printf("Letting Mouse know we are sending a command...\n");
	__outb( PS2_STAT, 0xD4 );
	_ps2_keyboard_clear();

	// Now, send the mouse our command
	//c_printf("Sending Command: 0x%x...\n", ( b & 0xFF) );
	__outb( PS2_PORT, command );
}

void _ps2_keyboard_clear( void ){
	while ( (__inb(PS2_STAT) & 16) == 0 )
		;
}

void _ps2_keyboard_isr( int vec, int code ){
	c_puts( "GOT AN NTERRUPTS!!!!!" );
}


