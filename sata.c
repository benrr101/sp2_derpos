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

	// Get the ABAR address?
	Uint16 abar1 = _pci_config_read(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_ABAR);
	Uint16 abar2 = _pci_config_read(SATA_PCI_BUS, SATA_PCI_DEVICE, SATA_PCI_FUNCTION, SATA_PCI_REG_ABAR + 2);
	Uint32 abar = (abar2 << 32) | abar1;
	c_printf("SATA A(HCI)BAR: 0x%x | 0x%x -> 0x%x\n", abar1, abar2, abar);

    c_puts("Done doin shit with the SATA bus\n");
	__panic("HOLY FUCK.");
}

