#
# Makefile to compile the orbit program.
#

# Select the Fortran compiler to use and set compilation flags.

FC     = gfortran
FFLAGS =
FLIB   =


# How to make a .mod from a .f90
# How to make a .o from a .f90

%.mod: %.f90
	$(FC) -c $(FFLAGS) $<

%.o: %.f90
	$(FC) -c $(FFLAGS) $<


# Object files and source code to be compiled.

FSRC = orbit.f90 init.f90 theory.f90


# Make a list of all the .o files

FOBJ := $(FSRC:.f=.o)
FOBJ := $(FOBJ:.f90=.o)


# This is where the code is compiled.

orbit: $(FOBJ)
	$(FC) $(FFLAGS) -o $@ $(FOBJ) $(FLIB)


# Define dependency of orbit.f90 and theory.f90 on init module

orbit.f90: init.mod
theory.f90: init.mod


# Get rid of any junk.

clean:
	\rm -f *.o *.mod orbit *.agr
