###############################################################################
#
#  Makefile for FFTW F90 practical
#
###############################################################################

F90=gfortran
FFLAGS=-O2
.SUFFIXES: .f90 .o

fftw1:	fftw1.o
	$(F90) $(FFLAGS) -o fftw1 fftw1.o -lsfftw

.f90.o:
	$(F90) -c $<

clean:	
	/bin/rm -f *.o fftw1

