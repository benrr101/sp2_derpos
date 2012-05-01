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
	vga_vesa_info = (VESA_INFO *)(VESA_INFO_ADDR);
	vga_mode_info = (MODE_INFO *)(VGA__INFO_ADDR);
	
	c_printf("\n(2)VESA_INFO: %x", vga_vesa_info);
	c_printf("\nVer.  ModePtr    ttlmem    Cap");
	c_printf("\n%x | %x | %d - %d | %d %d %d %d",
		vga_vesa_info->VESAVersion, 
		vga_vesa_info->VideoModePtr,  
		vga_vesa_info->TotalMemory, 
		((vga_vesa_info->TotalMemory * 64)/1024),
		vga_vesa_info->Capabilities[0],
		vga_vesa_info->Capabilities[1],
		vga_vesa_info->Capabilities[2],
		vga_vesa_info->Capabilities[3]
		);
	c_printf("\nMODE_INFO(%x):", vga_mode_info);
	c_printf("\nModeAttr memModel  WinSize  Xres YRes XChar YChar");
	c_printf("\n%x | %x | %x | (%d, %d) | (%d, %d)", 
		vga_mode_info->ModeAttributes, 
		vga_mode_info->MemoryModel, 
		vga_mode_info->WinSize, 
		vga_mode_info->XResolution, 
		vga_mode_info->YResolution,
		vga_mode_info->XCharSize,
		vga_mode_info->YCharSize
		);
	c_printf("\n-WinFuncPrt: %x\n", vga_mode_info->WinFuncPtr);
	c_printf("-PhysBasePrt: %x\n", vga_mode_info->PhysBasePtr);
	c_printf("\nLin B/sl L #page  Rs  RFp Gs GFp Bs BFp Rsvds RsvdFp");
	c_printf("\n%d | %d         | %x | %x | %x | %x | %x | %x | %x | %x ", 
		vga_mode_info->LinbytesPerScanLine, 
		vga_mode_info->LinNumberOfImagePages, 
		vga_mode_info->LinRedMaskSize, 
		vga_mode_info->LinRedFieldPosition,
		vga_mode_info->LinGreenMaskSize,
		vga_mode_info->LinGreenFieldPosition, 
		vga_mode_info->LinBlueMaskSize, 
		vga_mode_info->LinBlueFieldPosition,
		vga_mode_info->LinRsvdMaskSize,
		vga_mode_info->LinRsvdFieldPosition
		);

	draw();
}

void draw2( void ) {
    return;
}
void back2text( void ) { 
    return;

}
void vga256( void ) {
    return;
}

void draw( void ) {
	Uint32 *ptr = (Uint32*)(vga_mode_info->PhysBasePtr);
    int i = 0;
    int j = 0;

    for(i = 10; i < 40; i++){
        for( j = 0; j < 20; j++){
			ptr[i+(20*j)] = i+j;
        }
    }
}
