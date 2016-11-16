cc=pgcc -O3 $(LDFLAGS)

LDFLAGS=-lm -mp

OBJS=loops2.o

loopspar: $(OBJS)
	$(cc) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(cc) -c $^ -o $@ $(LDFLAGS)

.PHONY : clean
clean :
	rm -f loopspar
	rm -f *.o
