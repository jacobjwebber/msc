#!/bin/bash

reps=2

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


manysched() {
    for ((j=0;j<=6;j++)); do
        size_of_block=$((2**j))
        export OMP_SCHEDULE="$1,${size_of_block}"
        do_loops
    done
}

export OMP_NUM_THREADS=4
export OMP_SCHEDULE="static"

do_loops

export OMP_SCHEDULE="auto"

do_loops

manysched static

manysched dynamic

manysched guided
