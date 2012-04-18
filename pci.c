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
	
	// vars
	Uint16 val;

	// report that the PCI probe is starting
	c_puts("Probing for PCI Devices...\n");

	// Print out all found devices
	Uint16 bus;
	Uint16 device;
	Uint16 func;
	/*for(bus = 0; bus < 0xFF; bus++){
		for(device = 0;  device < 0xFF; device++){
			for(func = 0; func < 0xFF; func++){
				val = _pci_config_read(bus, device, func, 0);
				if(val != 0xFFFF && val != 0x0){
					c_printf("Probing Bus 0x%x, device 0x%x, function 0x%x...\n", 
							bus, device, func);
					c_printf("   Vendor ID: 0x%x.\n", val);
				}
			}
		}
	}*/

	c_puts("Printing information on the IDE Controller...\n");
	Uint8 valb = _pci_config_readb(0x00, 0x1F, 0x1, 11);
	c_printf("Class Code of Device 0x1F, Function 0x1: 0x%x.\n", valb);

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

	// The controller appears to only give back aligned by 2 words. So IF we
	// wanted the upper bytes (%2 = 1) then we'll outsmart the controller and
	// give the user back upper bytes only.
	Uint8 byte;
	if(offset % 2 == 0) {
		// Lower bytes
		byte = word & 0xFFFF;
	} else {
		// Upper bytes
		byte = word >> 8;
	}
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

/**
 * Writes a short to the PCI bus.
 * @param	Uint16	bus		The bus of the PCI device to write to
 * @param	Uint16	device	The device # of the device to write to
 * @param	Uint16	func	The function of the device to write to
 * @param	Uint16	offset	Offset of the register to write to. Must be aligned
 *							to 16-bits
 * @param	Uint16	payload	The short to write to the device
 */
void _pci_config_write( Uint16 bus, Uint16 device, Uint16 func, Uint16 offset,
		Uint16 payload ){
	
	// If we're writing to a non-2byte aligned address, I don't think it will
	// work right
	if(offset % 2 != 0) {
		__panic("Attempting to write to a non-2byte aligned PCI config reg");
	}

	// Get the address we're writing to and tell the controller we're writing
	// to it.
	Uint32 addr = _pci_config_get_address(bus, device, func, offset);
	__outl(CONFIG_ACCESS, addr);
	
	// Write to the data port
	__outw(CONFIG_DATA, payload);
}

/**
 * Writes a long to the PCI bus.
 * @param	Uint16	bus		The bus of the PCI device to write to
 * @param	Uint16	device	The device # of the device to write to
 * @param	Uint16	func	The function of the device to write to
 * @param	Uint16	offset	Offset of the register to write to. Must be aligned
 *							to 32-bits
 * @param	Uint32	payload	The long to write to the device
 */
void _pci_config_writel( Uint16 bus, Uint16 device, Uint16 func, Uint16 offset,
		Uint32 payload ) {
	
	// If we're writing to a non-4byte aligned address, I don't think it'll
	// work right
	if(offset % 4 != 0) {
		__panic("Attempting to write to a non-4byte aligned PCI config reg");
	}

	Uint32 addr = _pci_config_get_address(bus, device, func, offset);
	__outl(CONFIG_ACCESS, addr);
	__outl(CONFIG_DATA, payload);
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
