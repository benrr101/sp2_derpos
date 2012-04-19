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
	//	   0	- Hardwired to 1	<- Not needed, so and it off
	return (reg & 0xFFFFFFFE);
}

void _sata_initialize() {
	// Initialize the controller into IDE mode
	_pci_config_write(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_MAP, 0x00);

	// Store the info about the channels
	ideChannels[ATA_PORT_CHANPRI].command = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCMD);
	ideChannels[ATA_PORT_CHANPRI].control = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCTRL);
	ideChannels[ATA_PORT_CHANSEC].command = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_SCMD);
	ideChannels[ATA_PORT_CHANSEC].control = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_SCTRL);

	// Point the primary channel at a good spot for IO (address stolen from lspci)
	//_pci_config_writel(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCMD, 0x78701);
	//_pci_config_writel(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_PCTRL, 0x78681);

	// Point the secondary channel at a good spot for IO (address stolen from lspci)
	//_pci_config_writel(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_SCMD, 0x78581);
}

void _sata_probe() {

	// Initialize the sata and the channel info
	_sata_initialize();

	// Let's set up the primary master first as a test
	Uint8 device = 0;
	Uint8 channel;
	IDECommandReg q;
	for(channel = 0; channel <= ATA_PORT_CHANSEC; channel++) {
		// Let's try to read in some of a IDECommand struct?
		q.statusCommand = __inb(ideChannels[channel].command + 0x7);
		c_printf("SATA Channel %x Status Register: 0x%x\n", channel, q.statusCommand);
	}
	
	__panic("HOT DAMN!");
}
