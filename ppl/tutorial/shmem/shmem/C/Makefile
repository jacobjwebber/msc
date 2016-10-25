PRG=	shmem

CC=	cc
LIBS=

# Leave alone below here !

.SUFFIXES:
.SUFFIXES: .c .o

MF=	Makefile

FFLAGS= -c
LFLAGS=	$(LIBS)

SRC=	cshmem.c

OBJ=	$(SRC:.c=.o  )

.c.o:
	$(CC) $(FFLAGS) $<

all:	$(PRG)

$(OBJ):	$(MF) 

$(PRG):	$(OBJ) $(MF)
	$(CC) -o $@ $(OBJ) $(LFLAGS)

clean:
	rm -f $(OBJ) $(PRG) core 
