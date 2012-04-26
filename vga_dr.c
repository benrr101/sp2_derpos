/*
** VGA Driver initialization code
*/

#define	__KERNEL__20113__

#include "headers.h"
#include "vga_dr.h"
#include "vga_define.h"

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
static VESA_INFO *vga_vesa_info;
static MODE_INFO *vga_mode_info;

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
	//Check the contents of the information extracted from REAL MODE 
	vga_vesa_info = (VESA_INFO *)(VESA_INFO_ADDR << 1);
	vga_mode_info = (MODE_INFO *)(VGA__INFO_ADDR << 1);
	
	c_printf("\nVESA_INFO(%x):\n", vga_vesa_info);
	c_printf("-version: %d\n", vga_vesa_info->VESAVersion);
	c_printf("-VideoModePtr: %x\n", vga_vesa_info->VideoModePtr);
	c_printf("-TtlMem: %d, %d\n", vga_vesa_info->TotalMemory, (vga_vesa_info->TotalMemory * 64)/1024);
	c_printf("\nMODE_INFO(%x):\n", vga_mode_info);
	c_printf("-ModeAttr: %x\n", vga_mode_info->ModeAttributes);
	c_printf("-MemModel: %x\n", vga_mode_info->MemoryModel);
	c_printf("-WinSize: %d\n", vga_mode_info->WinSize);
	c_printf("-WinSize (X, Y): (%d, %d)\n", vga_mode_info->XResolution, vga_mode_info->YResolution);
	c_printf("-CharSize (X, Y): (%d, %d)\n", vga_mode_info->XCharSize, vga_mode_info->YCharSize);
	c_printf("-WinFuncPrt: %x\n", vga_mode_info->WinFuncPtr);
	c_printf("-PhysBasePrt: %x\n", vga_mode_info->PhysBasePtr);
	draw();
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
	Uint32 *ptr = (Uint32*)(vga_mode_info->PhysBasePtr << 1);
    int i = 0;
    int j = 0;

    for(i = 10; i < 40; i++){
        for( j = 0; j < 20; j++){
			ptr[i+(20*j)] = i+j;
        }
    }
}
