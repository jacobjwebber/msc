cc=pgcc -O3
LDFLAGS=-lm -mp

#cc=gcc -O3 
#LDFLAGS=-lm -fopenmp

#cc=icc -O3 
#LDFLAGS=-lm -qopenmp

OBJS=loops2.o

loops_affinity: $(OBJS)
	$(cc) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(cc) -c $^ -o $@ $(LDFLAGS)

.PHONY : clean
clean :
	rm -f loops_affinity
	rm -f *.o
