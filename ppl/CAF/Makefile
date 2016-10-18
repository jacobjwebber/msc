MF=	Makefile

#COMPILER
FC=	ifort
FFLAGS=	-O3 -coarray
LFLAGS=	-Wl,-rpath,/lustre/sw/intel/compilers_and_libraries_2016.3.223/linux/mpi/intel64/lib/

EXE=	ex1c

FSRC= \
	ex1c.f90 \
	pgmio.f90

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .f90 .c .o

OBJ=	$(FSRC:.f90=.o) $(CSRC:.c=.o)

.f90.o:
	$(FC) $(FFLAGS) -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

ex1c.o: pgmio.o

$(EXE):	$(OBJ)
	$(FC) $(FFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF)

clean:
	rm -f $(OBJ) $(EXE) core
