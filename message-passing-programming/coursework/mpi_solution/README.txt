README
=====

Included is a src dir containing all source files and a Makefile. Compile with
 "make".

Also included are intput and output dirs for reading and writing files.

There is also a run_experiment dir. Although it was not possible to run the 
experiment because the code was buggy, this gives some indication as to how it
 might have been done.

Also included is the serial program for correctness verification.

In src
- mp_functions.c contains all message passing functions
- imagenewmpi.c calls these and implements algorithm
- definitions.h defines constants.

other sources are imported.
