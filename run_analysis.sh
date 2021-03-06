#!/bin/bash
# Parallelize the analysis of the powerlaw fits
MAX_N=1000  # Maximum swarm size
DELTA_N=100 # Delta swarm size
nseeds=16   # Number of seeds
L=20        # Environment size
N=$(seq $MAX_N -$DELTA_N $DELTA_N)
seeds=$(seq 1 1 $nseeds)
controllers=(ilw clw)

FILE="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/analyze.py"

start=$SECONDS
parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.1 --delay 0.05 '
    FILE={5};
    python -W ignore {5} --c {1} --N {2} --s {3} --L {4};
    ' ::: ${controllers[@]} ::: ${N[@]} ::: ${seeds[@]} ::: $L ::: $FILE

duration=$(( SECONDS - start ))
echo "Analysis of heavy tailedness finished after approximately $duration s"