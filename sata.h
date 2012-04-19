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
#define SATA_PCI_REG_PCMD	0x10
#define SATA_PCI_REG_PCTRL	0x14
#define SATA_PCI_REG_SCMD	0x18
#define SATA_PCI_REG_SCTRL	0x1C
#define SATA_PCI_REG_ABAR   0x24
#define SATA_PCI_REG_MAP	0x90	// 8 bit

// IDE Types
#define ATA_PORT_EMPTY		0x0
#define ATA_PORT_INUSE		0x1
#define ATA_PORT_CHANPRI	0x0
#define ATA_PORT_CHANSEC	0x1
#define ATA_PORT_CHANMAST	0x0
#define ATA_PORT_CHANSLAV	0x1
#define ATA_PORT_ATA		0x0
#define ATA_PORT_ATAPI		0x1


// TYPEDEFS ////////////////////////////////////////////////////////////////
typedef struct {
	Uint32	command;		// The command register (BAR)
	Uint32	control;		// The control register (should be BAR + 8)
} IDEChannel;

typedef struct {
	Uint8	data;			// The data register
	Uint8	errorFeatures;	// On read, error register. On write, features reg
	Uint8	sectorCount;	// Number of sectors
	Uint8	lbaLow;			// LBA Low register
	Uint8	lbaMid;			// LBA Middle register
	Uint8	lbaHigh;		// LBA High register
	Uint8	driveHead;		// Drive/Head register
	Uint8	statusCommand;	// On read, status register. On write, command reg
} IDECommandReg;

typedef struct {
	// Generic Host Controller Register Address Map
	Uint8	used;			// Whether the port is used
	Uint8	channel;		// Primary/Secondary
	Uint8	mastslav;		// Master/Slave
	Uint8	type;			// ATA/ATAPI
	Uint16	signature;
	Uint16	capabilities;
	Uint32	commandSets;
	Uint32	size;			// Size in sectors
	char	model[41];		// Model string
} IDEDevice;

// GLOBALS /////////////////////////////////////////////////////////////////
IDEChannel	ideChannels[2];
IDEDevice 	ideDevices[4];

// FUNCTIONS ///////////////////////////////////////////////////////////////
void _testGetPCIInfo(void);
void _sata_probe(void);
Uint16 _sata_get_bar(Uint16 bus, Uint16 device, Uint16 func, Uint16 offset);


#endif
