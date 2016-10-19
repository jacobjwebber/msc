#!/bin/bash

export OMP_SCHEDULE="static"

echo "SCHEDULE=STATIC"

for i in {1..10}
do
    ./loopspar_runtime >> temp

    cat temp | sed -n '2~4p' > temploop1
    cat temp | sed -n '4~4p' > temploop2
done

echo "loop 1 times"
cat temploop1

echo "loop 2 times"
cat temploop2


