#include "gl_print.h"
#include "gl.h"
#include "font_define.h"

static screen_info* scrn_info_arr;

static void gl_scroll( unsigned int l, screen_info* curr_si );
static void _gl_do_putchar( char c, screen_info* curr_si );
static void _gl_do_putchar_at( unsigned int x, unsigned int y, char c, screen_info* curr_si );
static void _gl_do_printf( int x, int y, char **f );
static int gl_pad( int x, int y, int extra, int padchar );
static int gl_padstr( int x, int y, char *str, int len, int width, int leftadjust, int padchar );


void gl_print_init( screen_info* ptr ) {
	scrn_info_arr = ptr;
}


static void gl_scroll( unsigned int l, screen_info* curr_si ) {
	char* to = curr_si->lines[0];
	char* from = curr_si->lines[1];
	int c_line = 1;
	int cpy = 0;
	//just clear the screan and make it go to the top.
	draw_rect(0,0,curr_si->w, curr_si->h, FONT_COLOR_BLACK);
	
	curr_si->curr_y = 0;
	
	for ( c_line = 1; c_line <= curr_si->y_max; c_line++ ) {
		to = curr_si->lines[c_line-1];
		
		//clear the lines
		for(cpy = 0; cpy < curr_si->x_max; cpy++)
			to[cpy] = '\0';
		from = curr_si->lines[c_line];
		//copy the lines
		for(cpy = 0; cpy < curr_si->x_max; cpy++) {
			to[cpy] = from[cpy];
		}
		to[cpy] = '\0';
		draw_string(to, 0, (c_line-1)*FONT_HEIGHT, FONT_COLOR);
	}
	//TODO: set cursor also mark dirty if we do that
}

//////////
// CHAR
//////////
void gl_putchar( char c ) {

	screen_info* curr_si;
	Pid pid = 0;
	Status s;

	s = get_pid( &pid );
	curr_si = ( get_screen_info( pid ) );
	_gl_do_putchar( c, curr_si );
}


static void _gl_do_putchar( char c, screen_info* curr_si ) {
	
	/*
	** If we're off the bottom of the screen, scroll the window.
	*/
	if( curr_si->curr_y > curr_si->y_max ){
		gl_scroll( 1, curr_si );
		curr_si->curr_y = curr_si->y_max;
	}
	
	switch( c ) {
		case '\n':
			curr_si->curr_x = 0;
			curr_si->curr_y++;
			break;
		case '\r':
			curr_si->curr_x = 0;
			break;
		case '\t':
			curr_si->curr_x +=4;
			break;
		default:
			curr_si->lines[curr_si->curr_y][curr_si->curr_x] = c;
			draw_character(c, curr_si->curr_x*FONT_WIDTH, curr_si->curr_y*FONT_HEIGHT, FONT_COLOR);
			curr_si->curr_x += 1;
			if( curr_si->curr_x > curr_si->x_max ){
				curr_si->curr_x = 0;
				curr_si->curr_y += 1;
			}
			break;
	}
	
	//TODO: set cursor
}

void gl_putchar_at( unsigned int x, unsigned int y, char c ) {
	screen_info* curr_si;
	Pid pid = 0;
	Status s;

	s = get_pid( &pid );
	curr_si = ( get_screen_info( pid ) );
	_gl_do_putchar_at( x, y, c, curr_si );
}

static void _gl_do_putchar_at( unsigned int x, unsigned int y, char c, screen_info* curr_si ) {
	/*
	** If x or y is too big or small, don't do any output.
	*/
	if( x <= curr_si->x_max && y <= curr_si->y_max ){
		curr_si->lines[y][x] = c;
		draw_character(c, x, y, FONT_COLOR);
	}
}

void gl_puts_at(  unsigned int x, unsigned int y, char *str ){
	unsigned int	ch;

	while( (ch = *str++) != '\0' ){
		gl_putchar_at( x, y, ch );
	}
}

void gl_puts( char *str ){
	unsigned int	ch;

	while( (ch = *str++) != '\0' ){
		gl_putchar( ch );
	}
}


///////////
// PRINTF
///////////
/*void gl_printf( char *fmt, ... ){
	_gl_do_printf( -1, -1, &fmt );
}

static void _gl_do_printf( int x, int y, char **f ){
	char	*fmt = *f;
	int	*ap;
	char	buf[ 12 ];
	char	ch;
	char	*str;
	int	leftadjust;
	int	width;
	int	len;
	int	padchar;
	screen_info* curr_si;
	Pid pid = 0;
	Status s;

	s = get_pid( &pid );
	curr_si = ( get_screen_info( pid ) );

	*
	** Get characters from the format string and process them
	*
	ap = (int *)( f + 1 );
	while( (ch = *fmt++) != '\0' ){
		*
		** Is it the start of a format code?
		*
		if( ch == '%' ){
			*
			** Yes, get the padding and width options (if there).
			** Alignment must come at the beginning, then fill,
			** then width.
			*
			leftadjust = 0;
			padchar = ' ';
			width = 0;
			ch = *fmt++;
			if( ch == '-' ){
				leftadjust = 1;
				ch = *fmt++;
			}
			if( ch == '0' ){
				padchar = '0';
				ch = *fmt++;
			}
			while( ch >= '0' && ch <= '9' ){
				width *= 10;
				width += ch - '0';
				ch = *fmt++;
			}

			*
			** What data type do we have?
			*
			switch( ch ){
			case 'c':
				// ch = *( (int *)ap )++;
				ch = *ap++;
				buf[ 0 ] = ch;
				buf[ 1 ] = '\0';
				x = gl_padstr( x, y, buf, 1, width, leftadjust, padchar );
				break;

			case 'd':
				// len = gl_cvtdec( buf, *( (int *)ap )++ );
				len = gl_cvtdec( buf, *ap++ );
				x = gl_padstr( x, y, buf, len, width, leftadjust, padchar );
				break;

			case 's':
				// str = *( (char **)ap )++;
				str = (char *) (*ap++);
				x = gl_padstr( x, y, str, -1, width, leftadjust, padchar );
				break;

			case 'x':
				// len = gl_cvthex( buf, *( (int *)ap )++ );
				len = gl_cvthex( buf, *ap++ );
				x = gl_padstr( x, y, buf, len, width, leftadjust, padchar );
				break;

			case 'o':
				// len = gl_cvtoct( buf, *( (int *)ap )++ );
				len = gl_cvtoct( buf, *ap++ );
				x = gl_padstr( x, y, buf, len, width, leftadjust, padchar );
				break;

			}
		}
		else {
			if( x != -1 || y != -1 ){
				_gl_do_putchar_at( x, y, ch, curr_si );
				switch( ch ){
				case '\n':
					y += 1;
					* FALL THRU *

				case '\r':
					x = 0;
					break;

				default:
					x += 1;
				}
			}
			else {
				_gl_do_putchar( ch, curr_si );
			}
		}
	}
}

void gl_printf( char *fmt, ... ){
	_gl_do_printf( -1, -1, &fmt );
}


char * gl_cvtdec0( char *buf, int value ){
	int	quotient;

	quotient = value / 10;
	if( quotient < 0 ){
		quotient = 214748364;
		value = 8;
	}
	if( quotient != 0 ){
		buf = gl_cvtdec0( buf, quotient );
	}
	*buf++ = value % 10 + '0';
	return buf;
}

int gl_cvtdec( char *buf, int value ){
	char	*bp = buf;

	if( value < 0 ){
		*bp++ = '-';
		value = -value;
	}
	bp = gl_cvtdec0( bp, value );
	*bp = '\0';

	return bp - buf;
}

char gl_hexdigits[] = "0123456789ABCDEF";

int gl_cvthex( char *buf, int value ){
	int	i;
	int	chars_stored = 0;
	char	*bp = buf;

	for( i = 0; i < 8; i += 1 ){
		int	val;

		val = ( value & 0xf0000000 );
		if( i == 7 || val != 0 || chars_stored ){
			chars_stored = 1;
			val >>= 28;
			val &= 0xf;
			*bp++ = gl_hexdigits[ val ];
		}
		value <<= 4;
	}
	*bp = '\0';

	return bp - buf;
}

int gl_cvtoct( char *buf, int value ){
	int	i;
	int	chars_stored = 0;
	char	*bp = buf;
	int	val;

	val = ( value & 0xc0000000 );
	val >>= 30;
	for( i = 0; i < 11; i += 1 ){

		if( i == 10 || val != 0 || chars_stored ){
			chars_stored = 1;
			val &= 0x7;
			*bp++ = gl_hexdigits[ val ];
		}
		value <<= 3;
		val = ( value & 0xe0000000 );
		val >>= 29;
	}
	*bp = '\0';

	return bp - buf;
}

static int gl_pad( int x, int y, int extra, int padchar ){
	while( extra > 0 ){
		if( x != -1 || y != -1 ){
			gl_putchar_at( x, y, padchar );
			x += 1;
		}
		else {
			gl_putchar( padchar );
		}
		extra -= 1;
	}
	return x;
}

static int gl_padstr( int x, int y, char *str, int len, int width, int leftadjust, int padchar ){
	int	extra;

	if( len < 0 ){
		while(str[len] != '\0')
			len++;
	}
	extra = width - len;
	if( extra > 0 && !leftadjust ){
		x = gl_pad( x, y, extra, padchar );
	}
	if( x != -1 || y != -1 ){
		c_puts_at( x, y, str );
		x += len;
	}
	else {
		c_puts( str );
	}
	if( extra > 0 && leftadjust ){
		x = gl_pad( x, y, extra, padchar );
	}
	return x;
}
*/

