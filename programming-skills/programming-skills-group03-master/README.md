CLiPH
=====
##The Circle of Life for Pumas and Hares
### Programming Skills Group 03
#### C99
Authors: 
- Jacob Webber 
- Colm O'Shaughnessy
- Furqon Hensan Muttaqien
- Lidong Chen

Dependencies
- GCC
- CUnit

### For Users 

To run a simulation compile using 

```$ make```

then

```$ ./cliph input.txt```

where "input.txt" is an input map of land and sea areas. "input.txt", "input_mid.txt" and "input_big.txt" are included as
examples of the appropriate file format. The first line contains two integers which contain x and y, the dimensions of the map.
The remaining Ny lines of length Nx contain either 1s or 0s, with 1 denoting land and 0 denoting sea. If no file name is
specified by the user then the program will look for a file named "input.txt".

The simulation requires input parameters. These are read from a "params.dat" file. If this file does not exist they are
set to defaults. These params and defaults are:

- r - The birth rate of hares (0.04)
- a - The predation rate at which pumas eat hares (0.04)
- b - The birth rate of pumas per one hare eaten (0.02)
- m - The puma mortality rate (0.06)
- k - The diffusion rate for hares (0.2)
- l - The diffusion rate for pumas (0.2)
- dt - The time step (0.4)
- T - The number of iterations between ppm outputs (100)
- T_average - The iterations between averages (100) 

where the defaults are shown in brackets. Note that the time units are weeks. 

For land grid points, the population densities of the hares and pumas are randomly generated real numbers between 0 and 5.0.

The results of the simulation are outputted into a time-stamped results directory. The time taken to complete the
simulation is appended to a file entitled "program_time.txt".

If both dimenions of the map are less than 20, the results and input will be outputted to standard output. This may help developers with debugging.

### For Developers

The code is organised into modules. Each contains a .c and a .h file. These are made up of functions and derived data types
which are called or utilised by "main.c". The test directory contains a parallel directory structure to the src directory.
The test directory contains a test file for every src file. These test codes are written using the CUnit framework,
which is a dependency for testing.

Please write tests to cover any new code and run them using

```$ make test```

These tests will also be run by Travis CI when source code is committed to the "master" branch on the remote. Please see
the .travis.yml for the build script and add your email address to be notified of build success/failure. 

Below is a list of the models with a brief description of what they do and how they do it.

- params - This module contains a function that reads parameters from a file and assigns them to a struct. If the file does not exist it will add predefined defaults to this struct.

- model - This module contains functions which initialize map arrays and population arrays, calculate updates using the model for each timestep, and call the update fuction while outputting PPM files and average population densities at the specified intervals.

- input - This module contains a function that reads an input map file which provided from first argument while running the program. It sets the grid dimension (NX and NY), grid values, and also returns a pointer for the map grid.

- build_results_dir - creates a directory tree for the PPM files and averages. Head of tree is called "results-[timestamp]".

- output - This module provides functions to generate PPM files of population density and to print out total operation time in a text file. 

- arralloc - This code was authored by Adrian Jackson and used here with permission. It dynamically allocates an array with
            given dimensions. Note that no unit tests are included for this code as it was not written by us.

There is an automated build and test continuous integration process. Current build status is shown at by this
image

https://api.travis-ci.com/jacobjwebber/programming-skills-group03.svg?token=z1zVrpZxBDqxWvvduadE&branch=master

For security and plagiarism reasons the GitHub and travis-CI pages associated with this project are not public. The
authors can, however, assure whomever is marking this that they have been configured to an extremely high standard. 
For access please contact the authors.


