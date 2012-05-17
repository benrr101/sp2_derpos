/*
** File:	vmemA.S
**
** Author:	Sean McDermott
**
** Contributor:
**
** Description:	
*/

#define	__KERNEL__20113__
#define	__ASM__20113__

/*
** _vmem_turnon
**
*/

	.globl	_vmem_turnon
_vmem_turnon:
        enter   $0,$0
	movl 	%cr4, %ebx
	or 	$0x00000010, %ebx
	movl 	%ebx, %cr4
	movl    8(%ebp),%eax	// Get the address of the page directory into %eax,
	movl	%eax, %cr3
	movl	%cr0, %eax
	or	$0x80000000, %eax
	movl	%eax, %cr0
	leave			
	ret

	.globl	_vmem_getcr0
_vmem_getcr0:
	movl	%cr0, %eax
	ret

	.globl	_vmem_bsf
_vmem_bsf:
	enter   $0,$0
	movl    8(%ebp),%ebx
	bsf	%ebx, %eax
	leave
	ret

	.globl	_vmeml2_change_page
_vmeml2_change_page:
        enter   $0,$0
	movl    8(%ebp),%eax	// Get the address of the page directory into %eax,
	movl	%eax, %cr3
	leave			
	ret
	
	.globl	_isr_vmem_getcr2
_isr_vmem_getcr2:
	movl	%cr2, %eax
	ret
