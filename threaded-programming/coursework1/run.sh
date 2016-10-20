#!/bin/bash

reps=2

do_loops() {
    echo "SCHEDULE=${OMP_SCHEDULE}"

    for ((i=1;i<=reps+2;i++)); do
        ./loopspar_runtime >> temp
        printf "."
    done
    #skip first two repitions of program - these seem slower
    cat temp | sed -n '10~4p' > temploop1
    cat temp | sed -n '12~4p' > temploop2

    echo ""
    echo "Total time for 1000 reps of loop 1"
    cat temploop1 | grep -oE '[^ ]+$'

    echo ""
    echo "Total time for 1000 reps of loop 2"
    cat temploop2 | grep -oE '[^ ]+$'

    rm temp temploop1 temploop2
}


manysched() {
    for ((j=0;j<=4;j++)); do
        size_of_block=$((2**j))
        export OMP_SCHEDULE="$1,${size_of_block}"
        do_loops
    done
}

manythread() {
    export OMP_SCHEDULE="$1,$2"
    for ((j=0;j<=6;j++)); do
        num_of_threads=$((2**j))
        echo on ${num_of_threads} threads
        export OMP_NUM_THREADS=${num_of_threads}
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

manythread guided 4
