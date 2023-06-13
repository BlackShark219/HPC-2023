#!/bin/bash
n=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<name>.*</name>' | wc -l)
echo Number of nodes: $n
# Count the number of nodes with the same number of cores
echo "Nodes with the same number of cores:"
pbsnodes -a | awk '/np =/{print $3}' | sort | uniq -c | awk 
'{printf "%d nodes with %d cores\n", $1, $2}'
# Count the number of nodes with the same amount of memory
echo "Nodes with the same amount of memory:"
pbsnodes -a | awk '/physmem =/{print $10}' | sort | uniq -c | awk
'{printf "%d nodes with %d GB of memory\n", $1, $2/$3}'
n_p=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<np>.*</np>' | cut -d'>' -f2 | cut -d'<' -f1 | tr '\n' ' ' | awk '{sum = 0; for (i=1; i<=NF; i++) {sum+=$i} print sum}')
tot_mem_gb=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<status>.*</status>' | grep -o 'totmem=.*' | cut -d',' -f1 | cut -d'=' -f2 | sed 's/kb//g' | tr '\n' ' '| awk '{sum = 0; for (i=1; i<=NF; i++) {sum+=$i}; print sum/(1024*1024)}')
mem_per_proc=$(echo $tot_mem_gb $n_p | awk '{print $1/$2}')
echo Average memory per processor: $mem_per_proc
n_gpu=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<gpus>.*</gpus>' | cut -d'>' -f2 | cut -d'<' -f1 | tr '\n' ' ' | awk '{sum = 0; for (i=1; i<=NF; i++) {sum+=$i} print sum}')
echo Total GPUs: $n_gpu
