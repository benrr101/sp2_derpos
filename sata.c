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
#include "sata.h"

// DEFINES /////////////////////////////////////////////////////////////////

// FUNCTIONS ///////////////////////////////////////////////////////////////

void _testGetPCIInfo() {
	// Get the vendor id
	Uint16 vendor = _pci_config_read(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_VENDOR);
	c_printf("\nSATA Vendor: 0x%x\n", vendor);

	// Get the device id
	Uint16 device = _pci_config_read(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_DEVICE);
	c_printf("SATA Device: 0x%x\n", device);

	c_printf("Testing the PCI 8/32 shits...\n");
	// Writing the MAP to tell it to go into AHCI mode
	_pci_config_write(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_MAP, 0x40);
	c_printf("SATA MAP reg: 0x%x\n", _pci_config_readb(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_MAP));

	// Get the ABAR address?
	_pci_config_writel(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_BAR, 0xFF0);
	c_printf("SATA ABAR reg: 0x%x\n", _pci_config_readl(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_BAR));

    c_puts("Done doin shit with the SATA bus\n");
	__panic("HOLY FUCK.");
}

Uint16 _sata_get_bar(Uint16 bus, Uint16 device, Uint16 func, Uint16 offset) {
	// Read the register at the offset
	Uint32 reg = _pci_config_readl(bus, device, func, offset);
	// Return the address after bitmasking
	// The format of the register looks like:
	//	31:16	- Reserved
	//	15:3	- Base Address <- The stuff we want
	//	 2:1	- Reserved
	//	   0	- Hardwired to 1
	return (reg & 0xFFF8) >> 3;
}

void _sata_probe() {
	// Get the addresses that we'll be reading from
	Uint16 priCmdAddr = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE,
		SATA_PCI_FUNCTION, SATA_PCI_REG_PCMD);
	Uint16 priCtlAddr = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE,
		SATA_PCI_FUNCTION, SATA_PCI_REG_PCTRL);
	Uint16 secCmdAddr = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE,
		SATA_PCI_FUNCTION, SATA_PCI_REG_SCMD);
	Uint16 secCtlAddr = _sata_get_bar(SATA_PCI_BUS, SATA_PCI_DEVICE,
		SATA_PCI_FUNCTION, SATA_PCI_REG_SCTRL);
	c_printf("Primary SATA Command Port: 0x%x\n", priCmdAddr);
	c_printf("Primary SATA Control Port: 0x%x\n", priCtlAddr);
	c_printf("Secondary SATA Command Port: 0x%x\n", secCmdAddr);
	c_printf("Secondary SATA Control Port: 0x%x\n", secCtlAddr);
	__panic("HOT DAMN!");
}
