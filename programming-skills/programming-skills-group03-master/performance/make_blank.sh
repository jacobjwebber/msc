for i in $(seq 10 10 400)
do
    echo $(( ${i}*${i}/100 )) >> template_graph
done

