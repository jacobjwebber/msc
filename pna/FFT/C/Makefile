###############################################################################
#
#  Makefile for FFTW C practical
#
###############################################################################

CC=gcc
CFLAGS=-O2

fftw1:	fftw1.o io.o
	$(CC) $(CFLAGS) -o fftw1 fftw1.o io.o -lsfftw -lm

.c.o:	
	$(CC) -c $<

clean:	
	/bin/rm -f *.o fftw1

