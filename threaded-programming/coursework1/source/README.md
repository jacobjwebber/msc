README

B024703

Included is loopspar.c, an optimally parallelised OpenMP version of the loops.c program.
Also included is a run-experiment dir. This includes:

 - A run.sh file which will run the loopspar_runtime program with various 
environment variables and takemeasurements. It then calculates means and standard
deviations and plots using gnuplot.

 - The loopspar_runtime is a compiled version of the code where the schedules were set to
"runtime" meaning they can be set by environment variables.

 - Other files are used by the bash script to form csv outputs etcetera. Please take a look
at this bash script. It falls slightly outside the remit of this work but is a bit of a 
work of art.
