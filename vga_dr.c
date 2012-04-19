/*
** VGA Driver initialization code
*/

#define	__KERNEL__20113__

#include "headers.h"

#include "vga_dr.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
 */

/*
 ** PUBLIC FUNCTIONS
 */

/*
 ** _vga_init()
 **
 ** initialize the vga module
 */

//
//	if you change the 0 in the mode to a 4 you get the ($0x0103)->($0x4103)
//	linear buffer and from there you can draw tot he screen
// 	in the vga info struct there is  aPhysBaseAddr to draw too
//	
//

void _vga_init( void ) {
    //vga256();
    //draw();
    //draw2();
    //back2text() ;
}

void draw2( void ) {
    /*regs.h.ah = 0x0C;   function 0Ch = pixel plot
    regs.h.al = color;
    regs.x.cx = x;       x location, from 0..319  
    regs.x.dx = y;       y location, from 0..199  
    int86(0x10,&regs,&regs);
    */
    asm(
    "xorl %eax, %eax;"
    "movb $0x0C, %ah;"
    "movb $0x04, %al;"
    "movw $0x14, %cx;"
    "movw $0x14, %dx;"
    "int $0x10;"
    );
    return;
}
void back2text( void ) { 

    asm(
    "xorl   %eax, %eax;"
    "movw   $0x03, %ax;"
    "int    $0x10;"
    );
    return;
    /*  __dpmi_regs regs;
        regs.x.ax = 0x13; 
        Mode 0x13 is VGA 320x200x256, 0x3 is TEXT 80x25
        __dpmi_int(0x10, ); same as real-mode 
        return;
     */
}
void vga256( void ) {

    return;
    /*  __dpmi_regs regs;
        regs.x.ax = 0x13; 
        Mode 0x13 is VGA 320x200x256, 0x3 is TEXT 80x25
        __dpmi_int(0x10, ); same as real-mode 
        return;
     */
}

void draw( void ) {
    typedef unsigned char byte;
    byte *VGA = (byte*)0xA0000000L;     
    unsigned short offset;
    int i = 0;
    int j = 0;


    for(i = 10; i < 40; i++){
        for( j = 0; j < 20; j++){
            offset = 320*i + j;
            VGA[offset] = ((i+j)%15)+1;
        }
    }
}
