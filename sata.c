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

