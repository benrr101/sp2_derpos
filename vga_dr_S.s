/*
** http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
*/

	.comm 	vga_vesa_info 512,32
	.comm 	vga_mode_info 256,32
	.text
.globl _get_vesa_info
_get_vesa_info:
	//get VESA_INFO
	pushl 	%eax
	pushl	%ebx

	leal	vga_vesa_info, %eax
	movwl	%eax, %es:(%di)
	xorl 	%eax, %eax
	movw	$0x4F00, %ax
	
	popl	%ebx
    popl	%eax
    ret

.globl _get_mode_info
_get_mode_info:
	//get VESA_MODEINFO
	pushl 	%eax
	pushl	%ebx
	pushl	%ecx

	leal	vga_mode_info, %eax
	movwl	%eax, %es:(%di)
	xorl 	%eax, %eax
	movw   	$0x0103, %cx
	movw	$0x4F01, %ax
	
	popl	%ecx
	popl	%ebx
    popl	%eax
    ret
	
	/* 
	** get mode info????
	** --->he had one more step I though it read as above
	*/
.globl _select_vga_mode
_select_vga_mode:
	pushl 	%eax
	pushl	%ebx
	
	//select mode
	xorl   	%eax, %eax
    movw   	$0x0103, %bx
    movw 	$0x4F02, %ax
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
    movw 	$0x03, %ax
    int		$0x10
    
    popl	%ebx
    popl	%eax
    ret
	
