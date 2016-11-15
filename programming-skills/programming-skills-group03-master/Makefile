#This is a make file

cc=gcc $(LDFLAGS)

LDFLAGS= -ftree-copy-prop -lcunit -lm 

INC=-Isrc -Isrc/params -Isrc/input -Isrc/model -Isrc/output -Isrc/arralloc -Isrc/build_results_dir
TESTINC=-Itest -Itest/params -Itest/input -Itest/model -Itest/output


OBJS=arralloc.o input.o matrix_model_functions.o params.o output_functions.o build_results_dir.o
TESTS=params_test.o matrix_model_functions_test.o input_test.o output_test.o

cliph: $(OBJS) main.o 
	$(cc) -o $@ $^ $(INC) $(LDFLAGS)

run_tests: $(TESTS) $(OBJS) test_runner.o
	$(cc) -o $@ $^ $(TNC) $(LDFLAGS) 


%.o : src/%.c
	$(cc) -c $^ -o $@ $(INC)
%.o : src/arralloc/%.c
	$(cc) -c $^ -o $@ $(INC)
%.o : src/input/%.c
	$(cc) -c $^ -o $@ $(INC)
%.o : src/model/%.c
	$(cc) -c $^ -o $@ $(INC)
%.o : src/output/%.c
	$(cc) -c $^ -o $@ $(INC) 
%.o : src/params/%.c
	$(cc) -c $^ -o $@ $(INC)
%.o : src/build_results_dir/%.c
	$(cc) -c $^ -o $@ $(INC)

%.o : test/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)
%.o : test/arralloc/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)
%.o : test/input/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)
%.o : test/model/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)
%.o : test/output/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)
%.o : test/params/%.c
	$(cc) -c $^ -o $@ $(INC) $(TESTINC)

.PHONY : test
test : run_tests
	./$<

.PHONY : clean
clean : 
	rm -f run_tests
	rm -f *.o
	rm -f cliph 
	rm -f *.ppm
