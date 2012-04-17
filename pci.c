/**
 * File:	pci.c
 * Author:	Grant Kurtz
 *
 * Description:
 *		The PCI module is for interfacing with PCI devices, such as finding,
 * identifying, and communicating configuration data with PCI devices.
 */

#include "headers.h"
#include "startup.h"
#include "pci.h"

/**
 * Scans for all PCI devices, and printing information about discovered devices
 * to the terminal.
 */
void _pci_probe_devices(){
	
	// report that the PCI probe is starting
	c_puts("Probing for PCI Devices...\n");

	// just test out the _pci_config_read() function
	c_printf("Probing Bus 0, device 0, function 0: '%u'.\n", 
			_pci_config_read(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF));

	// report that the PCI probe completed with no issues
	c_puts("Done.\n");
}

/**
 * Reads a single word (2-bytes) of data from the specified device's config.
 *
 * For reading config data from a device, the following specification details
 * how the address is computed:
 * 	Bit  31:		Enable Bit (always on)
 *	Bits 30 - 24:	Reserved (always off)
 *	Bits 23 - 16:	Bus Number
 *	Bits 15 - 11:	Device Number
 *	Bits 10 - 08:	Function Number
 *	Bits 07 - 02:	Register Number
 *	Bits 01 - 00:	Empty (always off)
 * 
 * @param	bus		The bus the device is on.
 * @param	slot	Which slot on the bus the device is on.
 * @param	func	Which function of the device to use.  Note, not all devices
 *					are multifunction.
 * @param	offset	The amount to shift the final address by.
 * @return			The word that was read back from the device.  Note, that a
 *					return value of 0xFFFF means an invalid device selection.
 *
 * @source			http://wiki.osdev.org/PCI#Configuration_Mechanism_.231
 */
Uint16 _pci_config_read( Uint16 bus, Uint16 slot, 
		Uint16 func, Uint16 offset){

	// convert the provided values to longs for easier assembly later
	Uint32 addr = 0;
	Uint32 lbus = (Uint32) bus;
	Uint32 lslot = (Uint32) slot;
	Uint32 lfunc = (Uint32) func;

	// we need to compute the address of the device
	addr = (Uint32) ((lbus << 16) | (lslot << 11) | (lfunc << 8) | 
			(offset & 0xfc) | ((Uint32)0x80000000));

	// select the device
	__outl(CONF_ACC, addr);

	// read the data, we want the first word of the 32-bit register
	return (Uint16) ((__inl(CONF_DAT) >> ((offset & 2) * 8)) & 0xffff);
}
