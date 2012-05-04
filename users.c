/*
** SCCS ID:	@(#)users.c	1.1	4/5/12
**
** File:	user.c
**
** Author:	4003-506 class of 20133
**
** Contributor:
**
** Description:	User routines.
*/

#include "headers.h"

#include "users.h"
#include "pci.h"

/*
** SYSTEM PROCESSES
*/


/*
** Initial process; it starts the other top-level user processes.
*/

void init( void ) {
	int i;
	Pid pid;
	Time time;
	Status status;

	c_puts( "Init started\n" );

	_pci_probe_devices();

	/*
	** And now we start twiddling our thumbs
	*/

	status = get_time( &time );
	if( status != SUCCESS ) {
		prt_status( "idle: get_time status %s\n", status );
	}
	c_printf( "init => idle at time %08x\n", time );

	status = set_priority( PRIO_IDLE );
	if( status != SUCCESS ) {
		prt_status( "idle: priority change status %s\n", status );
	}

	write( '.' );

	for(;;) {
		for( i = 0; i < DELAY_LONG; ++i )
			continue;
		write( '.' );
	}

	/*
	** SHOULD NEVER REACH HERE
	*/

	c_printf( "*** IDLE IS EXITING???\n" );
	exit();

}
