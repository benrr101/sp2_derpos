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

	// we need to compute the address of the device
	Uint32 addr = _pci_config_get_address(bus, slot, func, offset);

	// select the device
	__outl(CONF_ACC, addr);

	// read the data, we want the first word of the 32-bit register
	return (Uint16) ((__inl(CONF_DAT) >> ((offset & 2) * 8)) & 0xffff);
}

/**
 * Read a byte from the PCI bus. IE read a word from the bus but AND off the
 * upper byte such that a single byte is returned.
 */
Uint8 _pci_config_readb( Uint16 bus, Uint16 device, Uint16 func, 
		Uint16 offset ) {
	Uint16 word = _pci_config_read(bus, device, func, offset);
	Uint8 byte = word & 0xFFFF;	
	return byte;
}


/**
 * Read a long from the PCI bus. IE read a short from the bus, read another
 * short, shift the second's bits and OR it together.
 */
Uint32 _pci_config_readl( Uint16 bus, Uint16 device, Uint16 func, 
		Uint16 offset ) {
	// Read the two shorts
	Uint16 word1 = _pci_config_read(bus, device, func, offset);
	Uint16 word2 = _pci_config_read(bus, device, func, offset + 2);

	// OR them together
	Uint32 word3 = word2 << 16;
	Uint32 dword = word1 | word3;

	return dword;
}

void _pci_config_writeb( Uint16 bus, Uint16 device, Uint16 func, Uint16 offset,
		Uint8 payload ){
		
	// Get the address we're writing to
	Uint32 addr = _pci_config_get_address(bus, device, func, offset);
}


/**
 * Calculates the address for the PCI config space register.
 * How the address is computed:
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
 */
Uint32 _pci_config_get_address( Uint16 bus, Uint16 device, Uint16 func, 
		Uint16 offset ){

		// Turn the 16-bit values into 32-bit values
		Uint32 lbus    = (Uint32) bus;
		Uint32 ldevice = (Uint32) device;
		Uint32 lfunc   = (Uint32) func;

		// Calculate and return the address
		return (Uint32) ((lbus << 16) | (ldevice << 11) | (lfunc << 8) |
			(offset & 0xFC) | ((Uint32) 0x80000000));
}
