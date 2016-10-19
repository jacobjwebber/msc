#!/bin/bash

reps=6
rm temp
rm temploop1
rm temploop2

do_loops() {
    echo "SCHEDULE=${OMP_SCHEDULE}"

    for ((i=1;i<=reps+1;i++)); do
        ./loopspar_runtime >> temp
        printf "."
    done
    
    cat temp | sed -n '6~4p' > temploop1
    cat temp | sed -n '8~4p' > temploop2
 
    echo "loop 1 times"
    cat temploop1

    echo "loop 2 times"
    cat temploop2

    rm temp temploop1 temploop2
}

export OMP_NUM_THREADS=4
export OMP_SCHEDULE="static"

do_loops

export OMP_SCHEDULE="auto"

do_loops
