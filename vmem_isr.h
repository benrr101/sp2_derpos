/*
** File:	vmem.h
**
** Author:	Sean McDermott
**
** Contributor:
**
** Description:	Virtual Memory Definitions
*/

#ifndef _VMEM_ISR_H
#define _VMEM_ISR_H
#define _VMEM_ISR_DEBUG

#include "headers.h"

//aka falling down THREE flights of stairs
void _isr_general_protect( int vector, int code );

void _isr_page_fault( int vector, int code );

#endif
