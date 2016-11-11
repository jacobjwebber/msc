cc=gcc -O3 $(LDFLAGS)

LDFLAGS=-lm -fopenmp

OBJS=

loopspar: $(OBJS) loops2.o
	$(cc) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(cc) -c $^ -o $@

.PHONY : clean
clean :
	rm -f loopspar
	rm -f *.o
