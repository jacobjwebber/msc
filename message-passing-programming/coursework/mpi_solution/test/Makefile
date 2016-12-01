MF=	Makefile

CC=	mpicc
CFLAGS=

LFLAGS=	-lm -Wall

EXE= imagempi	

SRC= \
	pgmio.c \
	arralloc.c \
	mp_functions.c \
	imagenewmpi.c

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF)

clean:
	rm -f $(OBJ) $(EXE) core
