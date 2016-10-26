#!/bin/bash
module load intel-compilers-16
reps=50
DATE=$(date +"%Y%m%d%H%M")
RESULTS_DIR_NAME=results/${DATE}
echo $RESULTS_DIR_NAME
mkdir ${RESULTS_DIR_NAME}
RESULTS_1_FILE_NAME=${RESULTS_DIR_NAME}/results_${DATE}_loop1.csv
RESULTS_2_FILE_NAME=${RESULTS_DIR_NAME}/results_${DATE}_loop2.csv


cat results_empty_1 > $RESULTS_1_FILE_NAME
cat results_empty_2 > $RESULTS_2_FILE_NAME

do_loops() {
    echo "SCHEDULE=${OMP_SCHEDULE}"

    for ((i=1;i<=reps+2;i++)); do
        ./loopspar_runtime >> temp
        printf "."
    done
    #skip first two repitions of program - these seem slower
    cat temp | sed -n '10~4p' | grep -oE '[^ ]+$'> temploop1
    cat temp | sed -n '12~4p' | grep -oE '[^ ]+$'> temploop2

    echo ""
    echo "Total time for 1000 reps of loop 1"
    cat temploop1 

    echo ""
    echo "Total time for 1000 reps of loop 2"
    cat temploop2
    
    mean temploop1
    mean_loop1=$mean_time

    std temploop1
    std_loop1=$std

    mean temploop2
    mean_loop2=$mean_time

    std temploop2
    std_loop2=$std

    echo "\"${OMP_SCHEDULE}\", ${OMP_NUM_THREADS}, $mean_loop1, $std_loop1" >> $RESULTS_1_FILE_NAME
    echo "\"${OMP_SCHEDULE}\", ${OMP_NUM_THREADS}, $mean_loop2, $std_loop2" >> $RESULTS_2_FILE_NAME

    rm temp temploop1 temploop2
}

mean() {
    cat $1 >> balls
    mean_time=$(awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' $1)
    echo $mean_time >> balls
}

std() {
    std="$(awk '{sum+=$1; sumsq+=$1*$1} END {print sqrt(sumsq/NR - (sum/NR)**2)}' $1)"
    echo ${std} >> balls
}

manysched() {
    for ((j=0;j<=6;j++)); do
        size_of_block=$((2**j))
        export OMP_SCHEDULE="$1,${size_of_block}"
        do_loops
        echo -e "$size_of_block \t ${mean_loop1} \t ${std_loop1}" >> ${RESULTS_DIR_NAME}/$1_loop1_graph
        echo -e "$size_of_block \t ${mean_loop2} \t ${std_loop2}" >> ${RESULTS_DIR_NAME}/$1_loop2_graph
    done
}

manythread() {
    values=( 1 2 4 6 8 12 16 )
    export OMP_SCHEDULE="$1,$2"
    for j in "${values[@]}"; do
        num_of_threads=$j
        echo on ${num_of_threads} threads
        export OMP_NUM_THREADS=${num_of_threads}
        do_loops
        echo -e "$num_of_threads \t ${mean_loop1}" >> ${RESULTS_DIR_NAME}/speed_$1$2_loop1_graph
        echo -e "$num_of_threads \t ${mean_loop2}" >> ${RESULTS_DIR_NAME}/speed_$1$2_loop2_graph
    done
    awk '{if(NR==1)Tone=$2; $2=Tone/$2}1' ${RESULTS_DIR_NAME}/speed_$1$2_loop1_graph >  ${RESULTS_DIR_NAME}/speedup_$1$2_loop1_graph

    awk '{if(NR==1)Tone=$2; $2=Tone/$2}1' ${RESULTS_DIR_NAME}/speed_$1$2_loop2_graph >  ${RESULTS_DIR_NAME}/speedup_$1$2_loop2_graph
}


export OMP_NUM_THREADS=4
export OMP_SCHEDULE="static"

do_loops

export OMP_SCHEDULE="auto"

do_loops

manysched static

manysched dynamic

manysched guided

manythread dynamic 8

manythread dynamic 16

cd $RESULTS_DIR_NAME

gnuplot ../../gnuplot_script
