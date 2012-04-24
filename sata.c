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

Uint8 _sata_read_reg(IDEChannel channel, IDERegs reg) {
	if(reg < IDE_REG_SECCOUNT1) {
		// Reading from command
		return __inb(channel.command + reg);
	} else if(reg < IDE_REG_ALTSTATUS) {
		// Reading from special command
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x80);
		Uint8 r = __inb(channel.command + reg - 0x06);
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x0);
		return r;
	} else if(reg <= IDE_REG_DEVADDR) {
		// Reading from control
		return __inb(channel.control + reg - 0x0A);
	} else {
		// Reading from bus master
		return __inb(channel.busmast + reg - 0x0E);
	}
}

void _sata_write_reg(IDEChannel channel, IDERegs reg, Uint8 payload) {
	if(reg < IDE_REG_SECCOUNT1) {
		// Reading from command
		__outb(channel.command + reg, payload);
	} else if(reg < IDE_REG_ALTSTATUS) {
		// Reading from special command
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x80);
		__outb(channel.command + reg - 0x06, payload);
		_sata_write_reg(channel, IDE_REG_CONTROL, 0x0);
	} else if(reg <= IDE_REG_DEVADDR) {
		// Reading from control
		__outb(channel.control + reg - 0x0A, payload);
	} else {
		// Reading from bus master
		__outb(channel.busmast + reg - 0x0E, payload);
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
			int i;
			for(i = 0; i < 200000000; i++) { continue; }

			_sata_write_reg(ideChannels[chan], IDE_REG_COMMAND, IDE_CMD_IDENTIFY);
			for(i = 0; i < 200000000; i++) { continue; }
			
			// Read in the status register
			c_printf("SATA Channel: %x, Device: %x, Status: 0x%x, Sectors: %d\n",
				chan, dev, _sata_read_reg(ideChannels[chan], IDE_REG_STATUS),
				_sata_read_reg(ideChannels[chan], IDE_REG_SECCOUNT1));
		}	
	}
	
	__panic("HOT DAMN!");
}
