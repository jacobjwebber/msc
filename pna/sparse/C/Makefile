MF=Makefile

# Compile Flags

CC=cc
C_COMPILE_FLAGS= -g
C_LINK_FLAGS=
C_LIB_FLAGS=-lm

SRC = \
	coomatrix.c \
	csrmatrix.c \

INC	= \
	matrix.h \

OBJ = $(SRC:.c=.o)

.SUFFIXES: .c .o

.c.o: $(SRC) 
	$(CC) $(C_COMPILE_FLAGS) -c $<

csrtest: csrtest.o $(OBJ)
	$(CC) $(C_LINK_FLAGS) csrtest.o -o $@ $(OBJ) $(C_LIB_FLAGS)

pollutionModel: pollutionModel.o $(OBJ)
	$(CC) $(C_COMPILE_FLAGS) $(C_LINK_FLAGS) pollutionModel.o -o $@ $(OBJ) $(C_LIB_FLAGS)

clean:
	rm $(OBJ)
	rm *.o
	rm csrtest
	rm pollutionModel

depend:
	makedepend -f $(MF) $(SRC) 

# DO NOT DELETE THIS LINE -- make depend depends on it.

coomatrix.o: matrix.h /usr/include/stdio.h /usr/include/features.h
coomatrix.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
coomatrix.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
coomatrix.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
coomatrix.o: /usr/include/libio.h /usr/include/_G_config.h
coomatrix.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
coomatrix.o: /usr/include/bits/sys_errlist.h /usr/include/malloc.h
coomatrix.o: /usr/include/math.h /usr/include/bits/huge_val.h
coomatrix.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
coomatrix.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
coomatrix.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
csrmatrix.o: matrix.h /usr/include/stdio.h /usr/include/features.h
csrmatrix.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
csrmatrix.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
csrmatrix.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
csrmatrix.o: /usr/include/libio.h /usr/include/_G_config.h
csrmatrix.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
csrmatrix.o: /usr/include/bits/sys_errlist.h /usr/include/malloc.h
csrmatrix.o: /usr/include/math.h /usr/include/bits/huge_val.h
csrmatrix.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
csrmatrix.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
csrmatrix.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
