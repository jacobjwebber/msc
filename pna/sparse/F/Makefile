CF90=gfortran
# compile with array bound checking on!
F90FLAGS= -C

.SUFFIXES: .f90 .o .f .mod

.f90.o:
	${CF90} ${F90FLAGS} -c $*.f90

.f90.mod:
	${CF90} ${F90FLAGS} -c $*.f90


default: csrtest

csrtest.o: coomatrix.mod csrmatrix.mod
pollutionModel.o: csrmatrix.mod


csrtest: csrtest.o coomatrix.o csrmatrix.o
	${CF90} ${F90FLAGS} -o csrtest csrtest.o coomatrix.o csrmatrix.o

csrtest_answer: clean csrmatrix_answer.o csrtest.o coomatrix.o
	${CF90} ${F90FLAGS} -o csrtest_answer csrtest.o coomatrix.o csrmatrix_answer.o

pollutionModel: pollutionModel.o csrmatrix.o
	${CF90} ${F90FLAGS} -o pollutionModel pollutionModel.o csrmatrix.o

pollutionModel_answer: clean csrmatrix_answer.o pollutionModel.o 
	${CF90} ${F90FLAGS} -o pollutionModel pollutionModel.o csrmatrix_answer.o

# clearing up
clean:
	/usr/bin/rm -f csrtest pollutionModel *.o *.mod
