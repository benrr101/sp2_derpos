////////////////////////////////////////////////////////////////////////////
// SysProgII OS (DERP OS) - SATA Controller Header
//
// @file	sata.h
// @descrip	Declares the functions for interfacing with the SATA bus.
// @author	Benjamin Russell
////////////////////////////////////////////////////////////////////////////

#ifndef	SATA_H
#define SATA_H

// #DEFINES ////////////////////////////////////////////////////////////////

// PCI Configuration Addressing
#define SATA_PCI_BUS        0
#define SATA_PCI_DEVICE     31
#define SATA_PCI_FUNCTION   2

// PCI Configuration Register Offsets
#define SATA_PCI_REG_VENDOR 0x00
#define SATA_PCI_REG_DEVICE 0x02
#define SATA_PCI_REG_HEADER 0x0E	// 16 bit
#define SATA_PCI_REG_BAR	0x20
#define SATA_PCI_REG_ABAR   0x24
#define SATA_PCI_REG_MAP	0x90	// 8 bit

// TYPEDEFS ////////////////////////////////////////////////////////////////
typedef struct {
	// Generic Host Controller Register Address Map
	Uint32 cap;		// Host capabilities
	Uint32 ghc;		// Global ICH7 control
	Uint32 is;		// Interrupt status
	Uint32 pi;		// Ports implemented
	Uint32 vs;		// AHCI Version		- Defaults to 0x00010100
	
	// Reserved bits

	// Port 1 DMA Registers
	Uint32 p0clb;	// Command List Lase Address
} SATA_AHCI;

// FUNCTIONS ///////////////////////////////////////////////////////////////
void _testGetPCIInfo(void);


#endif
