#!/bin/bash

rm total_time.txt
generate_images() {
    for i in $(seq -f "%03g" 10 10 400)
    do
        convert -compress none *.pnm -resize ${i}% scaled/image_scaled${i}
    done

    for i in $(ls scaled)
    do
        ./pnm2dat scaled/${i} scaled_dat/{i}.dat
    done
}

mean() {
    mean_time=$(awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' $1)
}

std() {
    std="$(awk '{sum+=$1; sumsq+=$1*$1} END {print sqrt(sumsq/NR - (sum/NR)**2)}' $1)"
}

run_cliph() {
    for i in $(ls scaled_dat)
    do
        echo ${i}
        ./$1 scaled_dat/${i}
    done
    awk 'NF>1{print $NF}' total_time.txt | sed 's/s$//' > times_$1
    rm total_time.txt
    rm -r results*
}

make_graphs() {
    for i in $(ls times/)
    do
        paste template_graph times/${i} > graphs/${i}_graph
    done
}
        

#run_cliph pgi_cliphO1
#run_cliph pgi_cliphO2
#run_cliph pgi_cliphO3
#run_cliph pgi_cliph-none


#run_cliph gcc_cliphO1
#run_cliph gcc_cliphO2
#run_cliph gcc_cliphO3
#run_cliph gcc_cliph-none
run_cliph gcc_cliph-copy-reduce
#move results to times dir

#mkdir graphs
#make_graphs
