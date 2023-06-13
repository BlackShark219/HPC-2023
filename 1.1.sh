#!/bin/bash

n=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<name>.*</name>' | wc -l)
echo "Number of nodes: $n"

g_b_m=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<status>.*</status>' | grep -o 'totmem=.*' | cut -d',' -f1 | sort -u | wc -l)
echo "$g_b_m groups by memory"

g_b_p=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<np>.*</np>' | sort -u | wc -l)
echo "$g_b_p groups by processor number"

n_p=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<np>.*</np>' | cut -d'>' -f2 | cut -d'<' -f1 | tr '\n' ' ' | awk '{sum += $1} END {print sum}')
tot_mem_gb=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<status>.*</status>' | grep -o 'totmem=.*' | cut -d',' -f1 | cut -d'=' -f2 | sed 's/kb//g' | awk '{sum += $1} END {print sum / (1024*1024)}')
mem_per_proc=$(echo "$tot_mem_gb $n_p" | awk '{print $1 / $2}')
echo "Average memory per processor: $mem_per_proc"

n_gpu=$(pbsnodes -x | sed 's/<Node>/\n/g' | grep -o '<gpus>.*</gpus>' | cut -d'>' -f2 | cut -d'<' -f1 | tr '\n' ' ' | awk '{sum += $1} END {print sum}')
echo "Total GPUs: $n_gpu"
