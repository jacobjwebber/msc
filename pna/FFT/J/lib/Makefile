###############################################################################
#
#  Makefile for jfftw library
#
###############################################################################

CC = cc
.SUFFIXES: .c .o .so
OBJS = jfftw_Wisdom.o jfftw_complex_Plan.o jfftw_complex_nd_Plan.o jfftw_real_Plan.o jfftw_real_nd_Plan.o
JAVAPATH = /usr/lib/jvm/java-1.7.0
INCLUDES = -I/$(JAVAPATH)/include/ -I/$(JAVAPATH)/include/linux/

jfftw: libjfftw.so

libjfftw.so: $(OBJS)
	$(CC) -shared -o libjfftw.so $(OBJS) -lfftw

.c.o:
	$(CC) -fpic -g $(INCLUDES) -c $<

clean:	
	/bin/rm -f *.o *.so

