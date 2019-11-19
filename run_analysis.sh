#!/bin/bash
# Parallelize the analysis of the powerlaw fits
MAX_N=500  # Maximum swarm size
MIN_N=300
DELTA_N=50 # Delta swarm size
nseeds=20   # Number of seeds
L=170        # Environment size
N=$(seq $MAX_N -$DELTA_N $MIN_N)
seeds=$(seq 1 1 $nseeds)
controllers=(acrw)
environments=(patchy sparse)

DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/"

start=$SECONDS
# -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.05 
parallel --delay 0.05 '
    DIR={6};
    cd $DIR;
    python -W ignore analyze.py --c {1} --N {2} --s {3} --L {4} --e {5};
    ' ::: ${controllers[@]} ::: ${N[@]} ::: ${seeds[@]} ::: $L ::: ${environments[@]} ::: ${DIR}

duration=$(( SECONDS - start ))
echo "Analysis of search efficiency finished after approximately $duration s"
