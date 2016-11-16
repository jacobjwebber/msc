Program CsrTest

  use coomatrix
  use csrmatrix

  Implicit None

  ! COO-matrix A:
  type(coo) :: A

  ! CSR-matrix B:
  type(csr) :: B

  ! vectors: 
  real(kind=8), dimension(:), allocatable :: v, w1, w2

  ! file name
  character fileNum
  character*80 matrix_file

  Integer index

  !!!!! Setting up COO-matrix A and vectors v and w1 !!!!!!!!!!!!!!!!

  ! ask for input file
  print *,   "Please specify the input file, 1/2/3"
  read (*,*) fileNum
  matrix_file="../COOInput/random_matrix"//fileNum//".dat"
  print *, "Reading", matrix_file

  ! read COO-matrix from input file
  Call read_Coo(matrix_file,A)

  ! allcoate the vectors and read them as well
  allocate(v(A%ncol))
  allocate(w1(A%nrow))


  matrix_file="../COOInput/vector"//fileNum//".dat"
  print *, "Reading",  matrix_file
  open(15,file=matrix_file)
  Do index = 1, A%nrow
     read(15,*)v(index), w1(index)
  enddo
  close(15)

  Call print_coo(A)

  !!!!!! finished with set up work on COO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


  !!!!!! Working with CSR-matrix B !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  ! dimension matrix B that it can hold the contents of A
  Call setup_CSR(B, A%nrow, A%ncol, A%nnz)

  ! put the data into matrix B
  Call populate_CSR(B, A%rowIndex, A%colIndex, A%value, A%nnz) 

  ! let us see what we have got
  Call print_CSR(B)

  ! lets go ahead, multiply B and v, put the result into w2
  allocate(w2(B%nrow))

  Call times_vector_CSR(B, v, w2)
  
  ! lets check the results and print w1 and w2 to compare

  print 102, "w1", "w2"
  do index = 1, A%nrow
     print 101, w1(index), w2(index)
  enddo

101 Format(2(x,F8.2))
102 Format(2(2x,A4,3x))

  Call deallocate_coo(A)
  Call deallocate_csr(B)

  deallocate(v)
  deallocate(w1)
  deallocate(w2)

end Program CsrTest
