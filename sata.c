/*
** SCCS ID: @(#)users.c 1.1 4/5/12
**
** File:    sata.c
** Author:  Benjamin Russell
** Description: User routines.
*/

// REQUIRED FILES //////////////////////////////////////////////////////////

#include "headers.h"
#include "pci.h"
#include "startup.h"
#include "sata.h"

// DEFINES /////////////////////////////////////////////////////////////////

// FUNCTIONS ///////////////////////////////////////////////////////////////

Uint16 _sata_get_bar(Uint16 bus, Uint16 device, Uint16 func, Uint16 offset) {
	// Read the register at the offset
	Uint32 reg = _pci_config_readl(bus, device, func, offset);
	// Return the address after bitmasking
	// The format of the register looks like:
	//	31:16	- Reserved
	//	15:3	- Base Address		
	//	 2:1	- Reserved
	//	   0	- Hardwired to 1	<- Not needed, so AND it off
	return (reg & 0xFFFFFFFE);
}


/**
 * Reads a byte from the specified ide channel register. THAR BE MAGIC IN HERE.
 * @param	IDEChannel channel	The channel to read from
 * @param	IDEReg reg			The register on the channel to read from
 * @source	http://wiki.osdev.org/IDE
 */
Uint8 _sata_read_reg(IDEChannel channel, IDERegs reg) {
	Uint8 result;
	if(reg > 0x07 && reg < 0x0C) {
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x80);
	}

	if(reg < 0x08) {
		result = __inb(channel.command + reg - 0x00);
	} else if(reg < 0x0C) {
		result = __inb(channel.command  + reg - 0x06);
	} else if(reg < 0x0E) {
		result = __inb(channel.control  + reg - 0x0A);
	} else if (reg < 0x16) {
		result = __inb(channel.busmast + reg - 0x0E);
	}
   
	if(reg > 0x07 && reg < 0x0C) {
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x00);
	}
   
	return result;
}

/**
 * Writes a byte to the specified ide channel reg. THAR BE MAGIC HERE.
 * @param	IDEChannel channel	The channel to write to
 * @param	IDERegs reg			The register on the channel to write to
 * @param	Uint8 payload		The byte to write the the register
 * @source	http://wiki.osdev.org/IDE
 */
void _sata_write_reg(IDEChannel channel, IDERegs reg, Uint8 payload) {
	if(reg > 0x07 && reg < 0x0C) {
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x80);
	} 

	if(reg < 0x08) {
		__outb(channel.command  + reg - 0x00, payload);
	} else if(reg < 0x0C) {
		__outb(channel.command  + reg - 0x06, payload);
	} else if(reg < 0x0E) {
		__outb(channel.control  + reg - 0x0A, payload);
    } else if(reg < 0x16) {
		__outb(channel.busmast + reg - 0x0E, payload);
	}

	if(reg > 0x07 && reg < 0x0C) {
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x0);
	}
}

void _sata_initialize() {
	// Initialize the controller into IDE mode
	_pci_config_write(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_MAP, 0x00);

	// Store the info about the channels
	ideChannels[ATA_PORT_CHANPRI].command = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCMD);
	ideChannels[ATA_PORT_CHANPRI].control = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCTRL);
	ideChannels[ATA_PORT_CHANSEC].command = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_SCMD);
	ideChannels[ATA_PORT_CHANSEC].control = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_SCTRL);
	ideChannels[ATA_PORT_CHANPRI].busmast = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_BMAST);
	ideChannels[ATA_PORT_CHANSEC].busmast = ideChannels[ATA_PORT_CHANPRI].busmast + 0x8;
}

void _sata_wait() {
	volatile int i;
	for(i = 0; i < 1000000; i++);
}

void _sata_probe() {

	// Initialize the sata and the channel info
	_sata_initialize();

	// Let's probe the devices on the channels
	Uint8 dev;
	Uint8 chan;

	// Turn off IRQ's for the channels
	_sata_write_reg(ideChannels[ATA_PORT_CHANPRI], IDE_REG_CONTROL, 2);
	_sata_write_reg(ideChannels[ATA_PORT_CHANSEC], IDE_REG_CONTROL, 2);

	// Iterate over the channels to probe
	for(chan = ATA_PORT_CHANPRI; chan <= ATA_PORT_CHANSEC; chan++) {
		// Iterate over each device on the channel
		for(dev = ATA_PORT_CHANMAST; dev <= ATA_PORT_CHANSLAV; dev++) {
			// Tell the controller which drive we want
			_sata_write_reg(ideChannels[chan], IDE_REG_DRIVESEL, 0xA0 | (dev << 4));
			_sata_wait();

			_sata_write_reg(ideChannels[chan], IDE_REG_COMMAND, IDE_CMD_IDENTIFY);
			_sata_wait();
			
			// Read in the status register
			c_printf("SATA Channel: %x, Device: %x, Status: 0x%x, Sectors: %d\n",
				chan, dev, _sata_read_reg(ideChannels[chan], IDE_REG_STATUS),
				_sata_read_reg(ideChannels[chan], IDE_REG_SECCOUNT1));
		}	
	}
	
	__panic("HOT DAMN!");
}
