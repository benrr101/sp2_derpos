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
	
