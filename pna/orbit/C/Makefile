#
# Makefile to compile the orbit program.
#

# Select the C compiler to use and set compilation flags.

CC     = cc
CFLAGS =
CLIB   = -lm

# How to make a .o from a .f

.c.o:
	$(CC) -c $(CFLAGS) $<

# Object files and source code to be compiled.

CSRC   = orbit.c init.c theory.c

# Make a list of all the .o files

COBJ=$(CSRC:.c=.o)

# This is where the code is compiled.

orbit:	$(COBJ)
	$(CC) $(CFLAGS) -o $@ $(COBJ) $(CLIB)

clean:
	\rm -f *.o orbit
