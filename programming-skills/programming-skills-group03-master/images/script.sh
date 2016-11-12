#!/bin/bash

for i in $(seq -f "%03g" 10 10 400)
do
        convert -compress none *.pnm -resize ${i}% scaled/image_scaled${i}
done

for i in $(ls scaled)
do
        ./pnm2dat scaled/${i} ${i}.dat
done
