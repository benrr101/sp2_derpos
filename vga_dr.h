/*
** VGA Driver initialization code
*/

#ifndef _VGA_H
#define _VGA_H

#include "headers.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __ASM__20113__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
**
** From http://www.delorie.com/djgpp/doc/ug/graphics/vesa.
*/

typedef struct VESA_INFO
   { 
      unsigned char  VESASignature[4]     __attribute__ ((packed));
      unsigned short VESAVersion          __attribute__ ((packed));
      unsigned long  OEMStringPtr         __attribute__ ((packed));
      unsigned char  Capabilities[4]      __attribute__ ((packed));
      unsigned long  VideoModePtr         __attribute__ ((packed));
      unsigned short TotalMemory          __attribute__ ((packed));
      unsigned short OemSoftwareRev       __attribute__ ((packed));
      unsigned long  OemVendorNamePtr     __attribute__ ((packed));
      unsigned long  OemProductNamePtr    __attribute__ ((packed));
      unsigned long  OemProductRevPtr     __attribute__ ((packed));
      unsigned char  Reserved[222]        __attribute__ ((packed));
      unsigned char  OemData[256]         __attribute__ ((packed));
   } VESA_INFO;

typedef struct MODE_INFO
   {
	unsigned short 	ModeAttributes       __attribute__ ((packed));
	unsigned char  	WinAAttributes       __attribute__ ((packed));
	unsigned char  	WinBAttributes       __attribute__ ((packed));
	unsigned short 	WinGranularity       __attribute__ ((packed));
	unsigned short 	WinSize              __attribute__ ((packed));
	unsigned short 	WinASegment          __attribute__ ((packed));
	unsigned short 	WinBSegment          __attribute__ ((packed));
	unsigned long  	WinFuncPtr           __attribute__ ((packed));
	unsigned short 	BytesPerScanLine     __attribute__ ((packed));
	unsigned short 	XResolution          __attribute__ ((packed));
	unsigned short 	YResolution          __attribute__ ((packed));
	unsigned char  	XCharSize            __attribute__ ((packed));
	unsigned char  	YCharSize            __attribute__ ((packed));
	unsigned char  	NumberOfPlanes       __attribute__ ((packed));
	unsigned char  	BitsPerPixel         __attribute__ ((packed));
	unsigned char  	NumberOfBanks        __attribute__ ((packed));
	unsigned char  	MemoryModel          __attribute__ ((packed));
	unsigned char  	BankSize             __attribute__ ((packed));
	unsigned char  	NumberOfImagePages   __attribute__ ((packed));
	unsigned char  	Reserved_page        __attribute__ ((packed));
	unsigned char  	RedMaskSize          __attribute__ ((packed));
	unsigned char  	RedMaskPos           __attribute__ ((packed));
	unsigned char  	GreenMaskSize        __attribute__ ((packed));
	unsigned char  	GreenMaskPos         __attribute__ ((packed));
	unsigned char  	BlueMaskSize         __attribute__ ((packed));
	unsigned char  	BlueMaskPos          __attribute__ ((packed));
	unsigned char  	ReservedMaskSize     __attribute__ ((packed));
	unsigned char  	ReservedMaskPos      __attribute__ ((packed));
	unsigned char  	DirectColorModeInfo  __attribute__ ((packed));
	unsigned long  	PhysBasePtr          __attribute__ ((packed));
	unsigned long  	OffScreenMemOffset   __attribute__ ((packed));
	unsigned short	OffScreenMemSize     __attribute__ ((packed));
	unsigned short 	LinbytesPerScanLine;
	unsigned char 	BankNumberOfImagePages;
	unsigned char 	LinNumberOfImagePages;
	unsigned char 	LinRedMaskSize;
	unsigned char 	LinRedFieldPosition;
	unsigned char 	LingreenMaskSize;
	unsigned char 	LinGreenFieldPosition;
	unsigned char 	LinBlueMaskSize;
	unsigned char 	LinBlueFieldPosition;
	unsigned char 	LinRsvdMaskSize;
	unsigned char 	LinRsvdFieldPosition;
	unsigned char 	Reserved[194];
   } MODE_INFO;
/*
** Prototypes
*/

/*
** _vga_init()
**
** initialize the VGA driver module
*/

void _vga_init( void );

void vga256( void );

void draw( void );
void draw2( void );

void back2text( void );

#endif // ACM

#endif // VGA
