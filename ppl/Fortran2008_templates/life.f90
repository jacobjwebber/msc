PROGRAM life
IMPLICIT NONE

!     This code performs MAXLOOP iterations of an NxN life board
!
INTEGER, PARAMETER :: N=8, MAXLOOP=10
INTEGER loop
CHARACTER*(10) picfile

! 1)  Declare main arrays 
!
INTEGER, DIMENSION(N, N) :: BOARD
INTEGER, DIMENSION(N, N) :: NCOUNT

! 2)  Initialise board
!
BOARD = 0
BOARD(:,4) = 1
BOARD(4,:) = 1

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
DO loop = 1, MAXLOOP


! 4)  Count number of neighbours
!
NCOUNT = 0
NCOUNT = CSHIFT(BOARD, SHIFT = 1, DIM = 2)
NCOUNT = CSHIFT(BOARD, SHIFT = -1, DIM = 2) + NCOUNT
NCOUNT = CSHIFT(BOARD, SHIFT = 1, DIM = 1) + NCOUNT
NCOUNT = CSHIFT(CSHIFT(BOARD, SHIFT = 1, DIM =2), SHIFT = 1, DIM = 1) + NCOUNT
NCOUNT = CSHIFT(CSHIFT(BOARD, SHIFT = -1, DIM =2), SHIFT = 1, DIM = 1) + NCOUNT
NCOUNT = CSHIFT(CSHIFT(BOARD, SHIFT = 1, DIM =2), SHIFT = -1, DIM = 1) + NCOUNT
NCOUNT = CSHIFT(CSHIFT(BOARD, SHIFT = -1, DIM =2), SHIFT = -1, DIM = 1) + NCOUNT
! 5)  Calculate new generation
!
WHERE(NCOUNT == 3)
    BOARD = 1
ELSEWHERE
    BOARD = 0
END WHERE

!     Write out new state of board to a file
!
  WRITE(picfile, FMT='(''life'', i2.2, ''.pgm'')') loop
  OPEN(UNIT=10, FILE=picfile)
  WRITE(10, FMT='(''P2'',/,i3,2x,i3,/,i3)') N, N, 1
  WRITE(10,FMT='(8(i1,1x))') board
  CLOSE(UNIT=10)

END DO

END PROGRAM
