/*
** http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
*/
#include "vga_define.h"

	.text
.globl _get_vesa_info
_get_vesa_info:
	//get VESA_INFO
	pushl 	%eax
	pushl	%ebx

	movw	$VESA_INFO_ADDR, %ax
	movw	%ax, %es
	movw	$0x0, %di
	xorl 	%eax, %eax
	movw	$GET_VESA_INFO, %ax
	int 	$0x10
	
	popl	%ebx
    popl	%eax
    ret

.globl _get_mode_info
_get_mode_info:
	//get VESA_MODEINFO
	pushl 	%eax
	pushl	%ebx
	pushl	%ecx

	movw	$VGA__INFO_ADDR, %ax
	movw 	%ax, %es
	movw	$0x0, %di
	xorl 	%eax, %eax
	movw   	$V_MODE_800_600, %cx
	movw	$GET_MODE_INFO, %ax
	int		$0x10
	
	popl	%ecx
	popl	%ebx
    popl	%eax
    ret
	
	/* 
	** get mode info????
	** --->he had one more step I though it read as above
	** maybe setting the memory model from banks to linear buffer????????
	*/
.globl _select_vga_mode
_select_vga_mode:
	pushl 	%eax
	pushl	%ebx
	
	//select mode
	xorl   	%eax, %eax
    movw   	$V_MODE_800_600, %bx
    movw 	$SET_MODE, %ax
    int		$0x10
    
    popl	%ebx
    popl	%eax
    ret
    
.globl _select_text_mode
_select_text_mode:
	pushl 	%eax
	pushl	%ebx
	
	//select mode
	xorl   	%eax, %eax
    movw 	$V_MODE_TEXT, %ax
    int		$0x10
    
    popl	%ebx
    popl	%eax
    ret
