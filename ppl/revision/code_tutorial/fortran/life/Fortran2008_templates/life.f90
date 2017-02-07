PROGRAM life
IMPLICIT NONE

!     This code performs MAXLOOP iterations of an NxN life board
!
INTEGER, PARAMETER :: N=8, MAXLOOP=10
INTEGER loop
CHARACTER*(10) picfile

! 1)  Declare main arrays 
!



! 2)  Initialise board
!


!     Leave this bit as is
!     Print starting config (array board) to file life00.pgm
!
WRITE(picfile, FMT='(''life'', i2.2, ''.pgm'')') 0

OPEN(UNIT=10, FILE=picfile)
WRITE(10, FMT='(''P2'',/,i3,2x,i3,/,i3)') N, N, 1
WRITE(10,FMT='(8(i1,1x))') board
CLOSE(UNIT=10)

! 3)  Perform MAXLOOP updates
!     Start of a DO loop   
!



! 4)  Count number of neighbours
!



! 5)  Calculate new generation
!



!     Leave this bit as is
!     Write out new state of board to a file
!
  WRITE(picfile, FMT='(''life'', i2.2, ''.pgm'')') loop
  OPEN(UNIT=10, FILE=picfile)
  WRITE(10, FMT='(''P2'',/,i3,2x,i3,/,i3)') N, N, 1
  WRITE(10,FMT='(8(i1,1x))') board
  CLOSE(UNIT=10)

END DO
END PROGRAM
