# 
#Makefile
#

MF= Makefile

EXE= intro

SRC= intro.cu

ARCH=sm_21
ifeq ($(HOSTNAME),gpulab2.hydra)
ARCH=sm_35
endif


CC=	nvcc
CFLAGS= -O3 -arch=$(ARCH)




.SUFFIXES:
.SUFFIXES: .cu  .o

OBJ=	$(SRC:.cu=.o)


.cu.o:
	$(CC) $(CFLAGS) -dc -c $<

default: $(EXE)

$(EXE):	$(OBJ) 
	$(CC) $(CFLAGS) -o $@ $(OBJ) 

$(OBJ):	$(MF) 

clean:
	rm -f $(OBJ) $(EXE) core
