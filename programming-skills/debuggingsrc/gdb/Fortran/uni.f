      REAL FUNCTION UNI()
      IMPLICIT NONE
C ***   FIRST CALL RSTART(I,J,K,L)
C ***   WITH I,J,K,L INTEGERS
C ***   FROM 1...168 NOT ALL 1
      REAL U(97)
      INTEGER IU,JU
      REAL C, CM, CD
      COMMON /SET1/ U,IU,JU,C
      SAVE /SET1/
C      DATA I,J/97,33/
      CD = (7654321./16777216.) 
      CM = (16777213./16777216.)
      UNI=U(IU)-U(JU)
      IF(UNI.LT.0.) UNI=UNI+1.
      U(IU)=UNI
      IU=IU-1
      IF(IU.EQ.0) IU=97
      JU=JU-1
      IF(JU.EQ.0) JU=97
      C=C-CD
      IF(C.LT.0.) C=C+CM
      UNI=UNI-C
      IF(UNI.LT.0.) UNI=UNI+1
      RETURN
      END

C
C  The algorithm from James RMARIN
C  to generate the 4 seeds from and
C  INTEGER in the range 0 <= seed <= 900 000 000
C
        SUBROUTINE RINIT( IJKL )
      IMPLICIT NONE
        INTEGER IJKL, IJ, KL, I, J, K, L

        IJ=IJKL/30082
        KL=IJKL - 30082*IJ
        I = MOD(IJ/177,177)+2
        J=MOD(IJ,177)+2
        K=MOD(KL/169,178)+1
        L=MOD(KL,169)

        CALL RSTART(I,J,K,L)

        RETURN
        END


      SUBROUTINE RSTART(IP,JP,KP,LP)
      IMPLICIT NONE
      REAL U(97)
      INTEGER I,J,K,L
      INTEGER IP, JP, KP, LP
      INTEGER IU,JU
      REAL C, S, T
      COMMON /SET1/ U,IU,JU,C
      SAVE /SET1/
      INTEGER II, JJ, M

      I = IP
      J = JP
      K = KP
      L = LP
      DO 2 II=1,97
      S=0.
      T=.5
      DO 3 JJ=1,24
        M=MOD(MOD(I*J,179)*K,179)
        I=J
        J=K
        K=M
        L=MOD(53*L+1,169)
        IF(MOD(L*M,64).GE.32) S=S+T
        T=.5*T
3     CONTINUE
      U(II)=S
2     CONTINUE
      C=362436./16777216.
C      CD=7654321./16777216.
C      CM=16777213./16777216.
      IU=97
      JU=33
      RETURN
      END

