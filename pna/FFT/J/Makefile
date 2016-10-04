###############################################################################
#
#  Makefile for FFTW Java practical
#
###############################################################################

JAVAC=javac
.SUFFIXES: .java .class

default: FFT_example.class lib/libjfftw.so

.java.class:
	$(JAVAC) $<

lib/libjfftw.so:
	cd lib ; make

clean:	
	/bin/rm -f *.class *.pgm
	cd lib; make clean

