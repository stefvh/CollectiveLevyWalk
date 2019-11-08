#!/bin/bash
# Script for running the desired experiment in parallel
MAX_N=1000  # Maximum swarm size
DELTA_N=200  # Delta swarm size
nseeds=30   # Number of seeds
N=$(seq $DELTA_N $DELTA_N $MAX_N)
seeds=$(seq 1 1 $nseeds)
# GNU parallel magic
sudo localedef -i en_US -f UTF-8 en_US.UTF-8;
export LANGUAGE=en_US.UTF-8;
export LANG=en_US.UTF-8;
export LC_ALL=en_US.UTF-8;
sudo locale-gen en_US.UTF-8;

COPY=$1
if [ "$COPY" == "copy" ]; then 
    # Generate output directories for data to be stored
    ./create_heavy_tailedness_experiment_files.sh $MAX_N $DELTA_N "${seeds[@]}"
    # Distribute the codes over the available nodes
    SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/"
    LOCAL_DIR="/users/jnauta/CollectiveLevyWalk/"
    echo "Distributing code across nodes..."
    while IFS= read -r dest; do 
        scp -q -r [!.]* $SHARED_DIR $dest:$LOCAL_DIR
    done <nodes.txt
fi 

# MAIN PARALLEL SCRIPT
# o todo: Test this script
robotControllers=(ilw clw)
# Parallelize only over the swarm size and seeds
for CONTROLLER in "${robotControllers[@]}"; do
    # --controlmaster -S ensures that the paralellization happens across all specified nodes
    # Swarmsize, seed and the .argos file are specified
    # Within the .argos file, xmlstarlet changes the seed to the chosen value
    # The -l and -e flags when calling argos3 result in that the logs and errors
    # get written into non-existent files, which conveniently gets rid of them
    parallel --controlmaster -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.05 --delay 0.1 '
    CONTROLLER={1};
    SWARMSIZE={2};
    SEED={3};
    ARGOSDIR=experiments/heavy_tailedness/$CONTROLLER/${SWARMSIZE}N/$SEED;
    ARGOSFILE=$ARGOSDIR/heavy_tailedness_$CONTROLLER_${SWARMSIZE}N_$SEED.argos;
    argos3 -l $ARGOSDIR/log -e $ARGOSDIR/logerr -c $ARGOSFILE;
    ' ::: $CONTROLLER ::: ${N[@]} ::: ${seeds[@]}
done
