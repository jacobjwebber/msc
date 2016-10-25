PRG=	shmem

FC=	ftn
LIBS=

# Leave alone below here !

.SUFFIXES:
.SUFFIXES: .f90 .o

MF=	Makefile

FFLAGS= -c
LFLAGS=	$(LIBS)

SRC=	fshmem.f90 

OBJ=	$(SRC:.f90=.o  )

.f90.o:
	$(FC) $(FFLAGS) $<

all:	$(PRG)

$(OBJ):	$(MF) 

$(PRG):	$(OBJ) $(MF)
	$(FC) -o $@ $(OBJ) $(LFLAGS)

clean:
	rm -f $(OBJ) $(PRG) core 
