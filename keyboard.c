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
#include "ulib.h"
#include "keyboard.h"

// Struct to handle IO-Request information
typedef struct ps2_io_req{
	Pid pid;
	char **buf;
	int size;
	int index;
} ps2_io_req;

// Array of IO Requests currently outstanding
static ps2_io_req *requests [ TOTAL_IO_REQS ];

// Scan Code Set #1
// (copied from c_io.c)
unsigned char _ps2_scan_code[ 2 ][ 128 ] = {
	{
/* 00-07 */	'\377',	'\033',	'1',	'2',	'3',	'4',	'5',	'6',
/* 08-0f */	'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
/* 10-17 */	'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
/* 18-1f */	'o',	'p',	'[',	']',	'\n',	'\377',	'a',	's',
/* 20-27 */	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
/* 28-2f */	'\'',	'`',	'\377',	'\\',	'z',	'x',	'c',	'v',
/* 30-37 */	'b',	'n',	'm',	',',	'.',	'/',	'\377',	'*',
/* 38-3f */	'\377',	' ',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 40-47 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'7',
/* 48-4f */	'8',	'9',	'-',	'4',	'5',	'6',	'+',	'1',
/* 50-57 */	'2',	'3',	'0',	'.',	'\377',	'\377',	'\377',	'\377',
/* 58-5f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 60-67 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 68-6f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 70-77 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 78-7f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377'
	},

	{
/* 00-07 */	'\377',	'\033',	'!',	'@',	'#',	'$',	'%',	'^',
/* 08-0f */	'&',	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
/* 10-17 */	'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
/* 18-1f */	'O',	'P',	'{',	'}',	'\n',	'\377',	'A',	'S',
/* 20-27 */	'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
/* 28-2f */	'"',	'~',	'\377',	'|',	'Z',	'X',	'C',	'V',
/* 30-37 */	'B',	'N',	'M',	'<',	'>',	'?',	'\377',	'*',
/* 38-3f */	'\377',	' ',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 40-47 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'7',
/* 48-4f */	'8',	'9',	'-',	'4',	'5',	'6',	'+',	'1',
/* 50-57 */	'2',	'3',	'0',	'.',	'\377',	'\377',	'\377',	'\377',
/* 58-5f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 60-67 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 68-6f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 70-77 */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',
/* 78-7f */	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377',	'\377'
	}
};

// Key Modifiers
static char shift_pressed = 0;
static char ctrl_pressed = 0;
static char alt_pressed = 0;
static char num_lock = 1;
static char caps_lock = 0;
static char scroll_lock = 0;


void _ps2_keyboard_init( void ){

	// For simplicity, null-out the io-requests
	ps2_io_req *req;
	for( int i = 0; i < TOTAL_IO_REQS; i++){
		req = requests[i];
		req->pid = 0;
		req->buf = 0;
		req->size = -1;
		req->index = -1;
	}

	// Try turning on the caps-lock light
	/*
	_ps2_keyboard_clear();
	__outb( PS2_PORT, PS2_K_LED );
	c_puts( "Waiting for argument buffer to be clear...\n" );
	while( (__inb( PS2_PORT ) & 1) == 1 )
		;
	__outb( PS2_STAT, PS2_K_CAPS | PS2_K_NUML | PS2_K_SCRL );
	*/

	// install our ISR
	__install_isr( PS2_K_VEC, _ps2_keyboard_isr );

	// Disable the Keyboard Interface
#ifdef DEBUG_G
	//c_puts( "Disabling Keyboard Interface...\n" );
#endif
	//_ps2_keyboard_clear();
	//__outb( PS2_STAT, 0xF5 );
	//_ps2_keyboard_clear();
	//__outb( PS2_STAT, 0xA7 );

	// clear the output buffer
#ifdef DEBUG_G
	//c_puts( "Clearing Input Buffer...\n" );
#endif
	//__inb( PS2_PORT );

	
	//_ps2_keyboard_clear();
	//__outb( PS2_STAT, 0xAE );
	

	// Enable Interrupts
	
#ifdef DEBUG_G
	//c_puts( "Enabling Keyboard Interrupts...\n" );
#endif
	/*
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0x20 );
	_ps2_keyboard_ready();
	while( (__inb( PS2_STAT ) & 1) == 1)
		;
	Uint resp = ( __inb(0x60) | 1 );
	c_printf( "Val Read: 0x%x\n", resp );
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0x60 );
//	_ps2_keyboard_clear();
	_ps2_keyboard_ready();
	__outb( 0x60, resp );
	*/

	/*
	// Set delay
	_ps2_keyboard_clear();
	__outb( PS2_STAT, PS2_K_TYPD );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	_ps2_keyboard_ready();
	__outb( PS2_PORT, 20 );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	*/
	
#ifdef DEBUG_G
	//c_puts( "Setting Defaults...\n" );
#endif
	
	//_ps2_keyboard_clear();
	//_ps2_keyboard_write( 0xF6 );
	//_ps2_keyboard_read();  //Acknowledge
	
	// Set Typematic Delay
	/*
	_ps2_keyboard_clear();
	__outb( PS2_STAT, PS2_K_TYPD );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	_ps2_keyboard_ready();
	__outb( PS2_PORT, 20 );
	c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	*/
	
	// Enable
	//_ps2_keyboard_clear();
	//while( 1 )
		//;
	//__outb( PS2_STAT, 0xF4 );
	//c_printf("RCVD: 0x%x\n", _ps2_keyboard_read()); // Acknowledge
	


	// reset the keyboard
	/*	
	_ps2_keyboard_clear();
	__outb( PS2_STAT, 0xFF );
	_ps2_keyboard_ready();
	c_printf("RCVD: 0x%x\n", __inb( PS2_PORT ));
	*/



		
	
}

void _ps2_keyboard_ready( void ){
	while( (__inb(PS2_STAT) & 1) != 0 )
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
	while ( 1 ){
		Uint b = __inb(PS2_STAT);
		if( (b & 16) != 0 )
			return;
	}
}

void _ps2_keyboard_isr( int vec, int code ){
	Uint key = __inb( PS2_PORT );

	switch( key ){
		case PS2_KEY_LSHIFT_P:
		case PS2_KEY_RSHIFT_P:
			shift_pressed = 1;
			return;
		case PS2_KEY_LSHIFT_R:
		case PS2_KEY_RSHIFT_R:
			shift_pressed = 0;
			return;
		case PS2_KEY_CAPLCK_P:
			caps_lock = 1;
			return;
		case PS2_KEY_CAPLCK_R:
			caps_lock = 0;
			return;
	}
	if( key < 0x80 ){
		c_printf( "%c", _ps2_scan_code[ shift_pressed ][ key ] );
		_ps2_write_to_active( _ps2_scan_code[ shift_pressed ][ key ] );
	}
	else if( key >= 0x80 && key <= 0xD8){
		// key released!
	}
	else{
		//c_printf( "?" );
	}

	__outb( 0xA0, 0x20 );
	__outb( 0x20, 0x20 );
}

int _ps2_get_io_req( void ){
	int index = -1;
	for( int i = 0; i < TOTAL_IO_REQS; i++){
		if( requests[i]->index == -1 ){
			index = i;
			break;
		}
	}
	return index;
}

int _ps2_write_to_active( char c ){
	
	// grab focused process
	Pid active_p = 0;

	// Throw away the character if there is no focused process
	if( active_p == 0 ){
		return;
	}

	// Find the IO-request for this process
	int index = 0;
	while( index < TOTAL_IO_REQS ){
		if( requests[ index ]->pid == active_p )
			break;
		index ++;
	}
	if( index == TOTAL_IO_REQS )
		return; // The focused process does not want input
	
	// Write the character
	requests[ index ]->buf[ requests[ index ]->index++ ] = c;
	if( requests[ index ]->index == requests[ index ]->size ){
		// pull from IO-blocking queue
	}
}

/**
 * Performs a blocking buffered read for keyboard input.
 *
 * Keystrokes are stored in the buffer and will not stop filling the buffer
 * until 'size' characters has been read. If there is an error when processing
 * the read request, the buffer will be left untouched.  Otherwise undefined
 * behavior is defined for errors after the read request is processed. Note,
 * only user programs which have focus will recieve characters.
 *
 * @param	buf		The buffer to fill with character input from the keyboard.
 * @param	size	The number of characters to read.
 */
void buf_read( char* buf, int size ){
	
	// get pid of running process
	Pid pid;
	Status status = get_pid( &pid );
	if( status != SUCCESS ){
		prt_status( "keyboard: can't get pid of IO-requester: %s\n", status );
		return;
	}

	// Create an IO-Request block
	int index = _ps2_get_io_req();
	if( index == -1 ){
		c_printf( "keyboard: IO-request pool full.\n" );
		return;
	}

	// Initialize IO-request
	requests[index]->pid = pid;
	requests[index]->buf = &buf;
	requests[index]->size = size;
	requests[index]->index = 0;

	// Add to IO-blocking queue


}




