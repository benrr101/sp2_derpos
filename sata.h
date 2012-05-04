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
#define SATA_PCI_REG_PCMD	0x10
#define SATA_PCI_REG_PCTRL	0x14
#define SATA_PCI_REG_SCMD	0x18
#define SATA_PCI_REG_SCTRL	0x1C
#define SATA_PCI_REG_BMAST	0x20
#define SATA_PCI_REG_ABAR   0x24
#define SATA_PCI_REG_MAP	0x90	// 8 bit

// IDE Types
#define ATA_PORT_EMPTY		0x0
#define ATA_PORT_INUSE		0x1
#define ATA_PORT_CHANPRI	0x0
#define ATA_PORT_CHANSEC	0x1
#define ATA_PORT_CHANMAST	0x0
#define ATA_PORT_CHANSLAV	0x1

#define ATA_TYPE_ATA		0x0
#define ATA_TYPE_ATAPI		0x1

// Important offsets into the identify space
#define ATA_IDENT_MODEL		0x54

// Useful bitmasks for the status register
#define ATA_STATUS_BUSY		0x80
#define ATA_NOINT			0x02

// Determine if the device is ATAPI
#define ATAPI_LBA1			0x14
#define ATAPI_LBA2			0xEB
#define ATAPI_ALT_LBA1		0x69
#define ATAPI_ALT_LBA2		0x96


// TYPEDEFS ////////////////////////////////////////////////////////////////
typedef struct {
	Uint32	command;		// The command register (BAR)
	Uint32	control;		// The control register (should be BAR + 8)
	Uint32	busmast;		// The bus master register (PRI = BAR4, SEC = BAR4 + 8);
} ATAChannel;
typedef ATAChannel ATAController[2];

typedef enum {
	// Registers on the command
	IDE_REG_DATA		= 0x0,	// The data register
	IDE_REG_ERROR 		= 0x1,	// On read, error register.
	IDE_REG_FEATURES 	= 0x1,  // On write, features reg
	IDE_REG_SECCOUNT1	= 0x2,	// Number of sectors
	IDE_REG_LBALOW		= 0x3,	// LBA Low register
	IDE_REG_LBAMID		= 0x4,	// LBA Middle register
	IDE_REG_LBAHIGH		= 0x5,	// LBA High register
	IDE_REG_DRIVESEL	= 0x6,	// Drive selector register
	IDE_REG_STATUS		= 0x7,	// On read, status register.
	IDE_REG_COMMAND		= 0x7,	// On write, command reg
	
	// Registers on the control
	IDE_REG_SECCOUNT2	= 0x08,	// High byte of number of sectors
	IDE_REG_LBA3		= 0x09, // IDK.
	IDE_REG_LBA4		= 0x0A, // IDK.
	IDE_REG_LBA5		= 0x0B, // IDK.

	// Registers on the Bus Master
	IDE_REG_ALTSTATUS	= 0x0C, // On read, alternate status register
	IDE_REG_CONTROL		= 0x0C, // On write, control reg
	IDE_REG_DEVADDR		= 0x0D	// Device address?
} IDERegs;

typedef enum {
	IDE_CMD_IDENTIFY	= 0xEC,	// Tell the device to identify itself
	IDE_CMD_READSECE	= 0x24	// Tell the drive we want to read sectors
} IDECommands;

typedef struct {
	// VERY simple ATA device
	ATAChannel	channel;		// The channel the device is connected to
	Uint32		size;			// The size in sectors of the device
	Uint8		device;			// 0 for primary, 1 for secondary
	Uint8		type;			// 0 for ATA, 1 for ATAPI
	char		serial[21];		// The serial number of the drive
	char		model[41];		// The model number of the drive
} ATADevice;

// GLOBALS /////////////////////////////////////////////////////////////////
ATADevice ata_devices[10];
Uint8 ata_device_count; 
	
// FUNCTIONS ///////////////////////////////////////////////////////////////
void _testGetPCIInfo(void);
void _ata_probe(Uint16 bus, Uint16 device, Uint16 func);
void _ata_initialize(ATAController *cont, Uint16 bus, Uint16 device, Uint16 func);
Uint8 _ata_read_reg(ATAChannel channel, IDERegs reg);
void _ata_write_reg(ATAChannel channel, IDERegs reg, Uint8 payload);
Uint16 _ata_get_bar(Uint16 bus, Uint16 device, Uint16 func, Uint16 offset);
void _ata_read_sector(ATADevice dev, Uint64 lba);

void _ata_wait( void );
void _ata_wait_bsy( void );

#endif
