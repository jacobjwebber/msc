#----------------------#
#                      #
# Makefile for UPC     #
#                      #
#----------------------#
CC=	upc

# -X flag set the number of threads to 8
# remove this flag for dynamic environment
CFLAGS= -T=8

SRC= parallelSort.c \
	 quickSort.c \
	 cmpSplit.c

EXE= parallelSort.x

OBJ= $(SRC:.c=.o)

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o


all:	$(EXE)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ): $(MF)

clean:
	rm -f $(EXE) *.o 

