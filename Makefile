#
# SCCS ID: @(#)Makefile	1.14	4/5/12
#
# Makefile to control the compiling, assembling and linking of
# standalone programs in the DSL.  Used for both 4003-406 and
# 4003-506 (with appropriate tweaking).
#

#
# User supplied files
#
U_C_SRC = clock.c klibc.c pcbs.c queues.c scheduler.c sio.c stacks.c syscalls.c system.c ulibc.c users.c fs.c ata.c pci.c
U_C_OBJ = clock.o klibc.o pcbs.o queues.o scheduler.o sio.o stacks.o syscalls.o system.o ulibc.o users.o fs.o ata.o pci.o
U_S_SRC = klibs.S ulibs.S
U_S_OBJ = klibs.o ulibs.o
U_LIBS	=

#
# User compilation/assembly definable options
#
#	ISR_DEBUGGING_CODE	include context restore debugging code
#	CLEAR_BSS_SEGMENT	include code to clear all BSS space
#	SP2_CONFIG		enable SP2-specific startup variations
#	REPORT_MYSTERY_INTS	print a message on interrupt 0x27
#
USER_OPTIONS = -DCLEAR_BSS_SEGMENT -DSP2_CONFIG -DISR_DEBUGGING_CODE

#
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!!!
#
# Compilation/assembly control
#

#
# We only want to include from the current directory and ~wrc/include
#
INCLUDES = -I. -I/home/fac/wrc/include

#
# Compilation/assembly/linking commands and options
#
CPP = cpp
# CPPFLAGS = $(USER_OPTIONS) -nostdinc -I- $(INCLUDES)
CPPFLAGS = $(USER_OPTIONS) -nostdinc $(INCLUDES)

CC = gcc
CFLAGS = -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)

AS = as
ASFLAGS =

LD = ld

#		
# Transformation rules - these ensure that all compilation
# flags that are necessary are specified
#
# Note use of 'cpp' to convert .S files to temporary .s files: this allows
# use of #include/#define/#ifdef statements. However, the line numbers of
# error messages reflect the .s file rather than the original .S file. 
# (If the .s file already exists before a .S file is assembled, then
# the temporary .s file is not deleted.  This is useful for figuring
# out the line numbers of error messages, but take care not to accidentally
# start fixing things by editing the .s file.)
#

.SUFFIXES:	.S .b

.c.s:
	$(CC) $(CFLAGS) -S $*.c

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S

.S.o:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S
	$(AS) -o $*.o $*.s -a=$*.lst
	$(RM) -f $*.s

.s.b:
	$(AS) -o $*.o $*.s -a=$*.lst
	$(LD) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

# Binary/source file for system bootstrap code

BOOT_OBJ = bootstrap.b
BOOT_SRC = bootstrap.S

# Assembly language object/source files

S_OBJ = startup.o isr_stubs.o $(U_S_OBJ)
S_SRC =	startup.S isr_stubs.S $(U_S_SRC)

# C object/source files

C_OBJ =	c_io.o support.o $(U_C_OBJ)
C_SRC =	c_io.c support.c $(U_C_SRC)

# Collections of files

OBJECTS = $(S_OBJ) $(C_OBJ)

SOURCES = $(BOOT_SRC) $(S_SRC) $(C_SRC)

#
# Targets for remaking bootable image of the program
#
# Default target:  usb.image
#

usb.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
	./BuildImage -d usb -o usb.image -b bootstrap.b prog.b 0x10000

floppy.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
	./BuildImage -d floppy -o floppy.image -b bootstrap.b prog.b 0x10000

prog.out: $(OBJECTS)
	$(LD) -o prog.out $(OBJECTS)

prog.o:	$(OBJECTS)
	$(LD) -o prog.o -Ttext 0x10000 $(OBJECTS) $(U_LIBS)

prog.b:	prog.o
	$(LD) -o prog.b -s --oformat binary -Ttext 0x10000 prog.o

#
# Targets for copying bootable image onto boot devices
#

floppy:	floppy.image
	dd if=floppy.image of=/dev/fd0

usb:	usb.image
	dd if=usb.image of=/local/devices/disk

#
# Special rule for creating the modification and offset programs
#
# These are required because we don't want to use the same options
# as for the standalone binaries.
#

BuildImage:	BuildImage.c
	$(CC) -o BuildImage BuildImage.c

Offsets:	Offsets.c
	$(CC) $(INCLUDES) -o Offsets Offsets.c

#
# Clean out this directory
#

clean:
	rm -f *.nl *.lst *.b *.o *.image *.dis BuildImage Offsets

#
# Create a printable namelist from the prog.o file
#
# options to 'nm':
#	B	use (traditional) BSD format
#	n	sort by addresses, not by names
#	g	only global symbols
#

prog.nl: prog.o
#	nm -Bng prog.o | pr -w80 -3 > prog.nl
	nm -Bn prog.o | pr -w80 -3 > prog.nl

#
# Generate a disassembly
#
# options to 'objdump':
#	d	disassemble all text sections
#

prog.dis: prog.o
#	dis prog.o > prog.dis
	objdump -d prog.o > prog.dis

#
#       makedepend is a program which creates dependency lists by
#       looking at the #include lines in the source files
#

depend:
	makedepend $(INCLUDES) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

bootstrap.o: bootstrap.h
startup.o: bootstrap.h
isr_stubs.o: bootstrap.h
ulibs.o: syscalls.h headers.h queues.h /home/fac/wrc/include/x86arch.h
c_io.o: c_io.h startup.h support.h /home/fac/wrc/include/x86arch.h
support.o: startup.h support.h c_io.h /home/fac/wrc/include/x86arch.h
support.o: bootstrap.h
clock.o: headers.h /home/fac/wrc/include/x86arch.h startup.h clock.h pcbs.h
clock.o: stacks.h queues.h scheduler.h sio.h syscalls.h
klibc.o: headers.h
pcbs.o: headers.h queues.h pcbs.h clock.h stacks.h
queues.o: headers.h pcbs.h clock.h stacks.h queues.h
scheduler.o: headers.h scheduler.h pcbs.h clock.h stacks.h queues.h
sio.o: headers.h sio.h queues.h pcbs.h clock.h stacks.h scheduler.h system.h
sio.o: startup.h /home/fac/wrc/include/uart.h /home/fac/wrc/include/x86arch.h
stacks.o: headers.h queues.h stacks.h
syscalls.o: headers.h pcbs.h clock.h stacks.h scheduler.h queues.h sio.h
syscalls.o: syscalls.h /home/fac/wrc/include/x86arch.h system.h startup.h
system.o: headers.h system.h pcbs.h clock.h stacks.h bootstrap.h syscalls.h
system.o: queues.h /home/fac/wrc/include/x86arch.h sio.h scheduler.h users.h
system.o: ulib.h types.h
ulibc.o: headers.h
users.o: headers.h users.h
