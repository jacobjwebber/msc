Program PollutionModel

  use csrmatrix

  Implicit None

  integer, parameter :: M = 7   ! grid resolution

  integer, parameter      :: maxIter = 5000    ! Maximum iterations of BiCGSTAB 
  integer :: usedIter
  real(kind=8), parameter :: tol     = 1.0d-6  ! Relative BiCGSTAB tolerance

  ! Boundary conditions, -- see exercise sheet for notation
  real(kind=8), parameter :: y1 = 0.4d0
  real(kind=8), parameter :: y2 = 0.6d0
  integer, parameter :: p = 2

  ! k normalises the height of the chimney to 1
  real(kind=8), parameter :: k=(2.0d0/(y2-y1))**(2*p)


  ! indices
  integer :: index   ! index of current element
  integer :: i       ! current i coordinate
  integer :: j       ! current j coordinate

  real(kind=8) :: x, y  ! x and y data at current coordinate

  ! temporary store for coefficient data
  real(kind=8) :: coeffC, coeffN, coeffE, coeffS, coeffW

  ! local indices on interior mesh
  integer :: RC, RN, RE, RS, RW

  ! The linear system:
  integer, parameter :: noUnknowns = M*M
  integer, parameter :: noNonzeros = 5*(M-2)*(M-2) + 16*(M-2) + 12

  ! Coefficient matrix A in sparse CSR format
  type(CSR) :: A   
  
  ! right hand side
  real(kind=8), dimension(noUnknowns) :: b  

  ! solution vector
  real(kind=8), dimension(noUnknowns) :: u

  !------ Starting the code execution -------!

  ! Initialise the CSR matrix A and give it some memory
  Call setup_CSR(A, noUnknowns, noUnknowns, noNonzeros)

  ! initialise boundary condition
  b = 0.0d0
  
  
  ! In the example we will populate the matrix by hand
  ! stepping along each row of the u-field in x-direction
  ! column by column

  index=1
  do j = 1, M  ! looping over rows of the u-field 
     y = real(j, kind=8) / real(M+1, kind=8)

     do i = 1, M  ! looping over columns of the u-field
        x = real(i, kind=8) / real(M+1, kind=8)

        ! converting two-dimensional indices into 1 dimensional index
        RC = i + (j-1)*M

        ! Lookup table for north, east, south and west neighbour
        RN = RC + M
        RE = RC + 1
        RS = RC - M
        RW = RC - 1

        ! coefficient values
        coeffC = 4.0d0 * real( (M+1)*(M+1), kind=8)
        coeffN = - real( (M+1)*(M+1), kind=8)
        coeffE = - real( (M+1)*(M+1), kind=8)
        coeffS = - real( (M+1)*(M+1), kind=8)
        coeffW = - real( (M+1)*(M+1), kind=8)

        ! Set the row-start
        A%rowStart(RC) = index

        ! Now insert coefficients into the matrix, 
        ! modifying the allocation for boundary terms
        ! 
        ! There are nine different cases corresponding to
        ! the interior, the sides and corners of the u-field

        ! Interior point
        If ((i .gt. 1) .and. (i .lt. M) .and. (j .gt. 1) .and. (j .lt. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

        ! West boundary
        elseif ((i .eq. 1) .and. (j .gt. 1) .and. (j .lt. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

        ! East boundary
        elseif ((i .eq. M) .and. (j .gt. 1) .and. (j .lt. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

           ! we need to add the polution source to the bounday condition
           If ( (y .gt. y1) .and. ( y .lt. y2 ) ) then
              b(RC) = - coeffE * k * (( y2-y)*(y-y1))**p
           endif

        ! south bounday
        Elseif ((i .gt. 1) .and. (i .lt. M) .and. (j .eq. 1) ) then
           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

        ! north boundary
        Elseif ((i .gt. 1) .and. (i .lt. M) .and. (j .eq. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

        ! south west corner
        elseif ((i .eq. 1) .and. (j .eq. 1) ) then
           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

        ! south-east corner
        elseif ((i .eq. M) .and. (j .eq. 1)) then
           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RN
           A%value(index) = coeffN
           index = index + 1

        ! north-west corner
        elseif ((i .eq. 1) .and. (j .eq. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

           A%colIndex(index) = RE
           A%value(index) = coeffE
           index = index + 1

        ! North-east corner
        elseif ((i .eq. M) .and. (j .eq. M)) then
           A%colIndex(index) = RS
           A%value(index) = coeffS
           index = index + 1

           A%colIndex(index) = RW
           A%value(index) = coeffW
           index = index + 1

           A%colIndex(index) = RC
           A%value(index) = coeffC
           index = index + 1

        else
           print * ,"This should not be happen"
           print * , "Error in building matrix", i , j
           stop
        endif
     end do
  end do

  ! Check the correct number of elements have been allocated
  if ( index .eq. noNonzeros+1) then
     A%rowStart(A%nrow+1) = index
  else
     print *, "index = ", index, "  nnz = ", noNonzeros
     stop
  end if

  ! set initial solution to zero
  u =0.0d0

  ! Solve the system

  Call print_CSR(A)

  print *, 'Calling lincg_CSR'

  Call lincg_CSR(A, b, u, tol, maxIter, usedIter)

  print *, 'Number of iterations = ', usedIter

  ! Writing solution to disk

  print *, 'Writing solution to pollution.dat'

201 Format(f10.6,6x)
  Open(11, file="pollution.dat", status="unknown")
  Do j = 0, M+1
     y = real(j, kind=8) / real(M+1, kind=8)
     Do i = 0, M+1
        x = real(i, kind=8) / real(M+1, kind = 8)
        
        ! if interior point, write u-fields
        if ( (i .gt. 0) .and. (i .lt. M+1) .and. (j .gt. 0) .and. (j .lt. M+1) ) then
           RC = i + (j-1)*M
           write(11,201) u(RC)

        ! east boundary put the source
        elseif ( i .eq. M+1) then 
           if ((y .gt. y1) .and. ( y .lt. y2)) then
              write(11,201) k*((y-y2)*(y-y1))**p
           else
              write(11,201) 0.0d0
           endif
        else
           write(11,201) 0.0d0
        end if
     end Do
  end Do
  close(11)

  print *, 'Writing associated field file to pollution.general'

  !Writing an AVS field file
203 Format(80A)
  Open(12,file="pollution.general",status="unknown")
  write(12,203) "file = pollution.dat"
  write(12,*)"grid = ", M+2, " x ", M+2
  write(12,203) "format = ascii"
  write(12,203)"interleaving = record"
  write(12,203)"majority = row"
  write(12,203)"field = field0"
  write(12,203)"structure = scalar"
  write(12,203)"type = float"
  write(12,203)"dependency = positions"
  write(12,203)"positions = regular, regular, 0, 1, 0, 1"
  write(12,203)""
  write(12,203)"end"
  Close(12)


  ! Free the memory associated with A
  Call deallocate_csr(A)
  
end Program PollutionModel
