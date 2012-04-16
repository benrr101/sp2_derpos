

/**
 * Port information for talking to the PCI bus
 * Source: http://wiki.osdev.org/PCI#Configuration_Mechanism_.231
 */
#define CONF_ACC	0xCF8
#define CONFIG_ACCESS	CONF_ACC
#define	CONF_DAT	0xCFC
#define CONFIG_DATA		CONF_DAT

/**
 * Class Code information for identifying the tpye of device found
 * Source: http://wiki.osdev.org/PCI#Configuration_Mechanism_.231
 */
#define NO_CLASS	0x00
#define	MASS_STOR	0x01
#define NET_CONT	0x02
#define DISP_CONT	0x03

void _pci_probe_devices( void );
unsigned short _pci_config_read( unsigned short bus, unsigned short slot, 
		unsigned short func, unsigned short offset);
