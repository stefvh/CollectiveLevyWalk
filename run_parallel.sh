#!/bin/bash
# Script for running the desired experiment in parallel
T=50      # Number of seconds
MAX_N=100  # Maximum swarm size
DELTA_N=50 # Delta swarm size
nseeds=5   # Number of seeds
N=$(seq $DELTA_N $DELTA_N $MAX_N)
seeds=$(seq 1 1 $nseeds)

# Variables
COPY="true"
BUILD="true"
RUN="true"

# GNU parallel magic
sudo localedef -i en_US -f UTF-8 en_US.UTF-8;
export LANGUAGE=en_US.UTF-8;
export LANG=en_US.UTF-8;
export LC_ALL=en_US.UTF-8;
sudo locale-gen en_US.UTF-8;

if [ "$COPY" == "true" ]; then     
    echo "Copying/distributing code across nodes..."
    # Generate output directories for data to be stored
    ./create_heavy_tailedness_experiment_files.sh $T "${N[@]}" "${seeds[@]}"
    # Distribute the codes over the available nodes
    DIR="CollectiveLevyWalk"
    SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/"
    ARCHIVE="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/$DIR.tar.gz"
    LOCAL_DIR="/users/jnauta/"
    # Archive the files necessary
    tar -C $SHARED_DIR -czf $ARCHIVE $DIR
    while IFS= read -r dest; do 
        # Copy the archive
        echo $dest 
        scp -q $ARCHIVE $dest:$LOCAL_DIR
    done <nodes.txt
fi 

if [ "$BUILD" == "true" ]; then 
    echo "Building on all nodes..."
    # Build the project on each node
    parallel --nonall -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 'tar -xzf CollectiveLevyWalk.tar.gz; cd CollectiveLevyWalk; if [[ ! -e build ]]; then mkdir build; fi; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make'
fi

# MAIN PARALLEL SCRIPT
if [ "$RUN" == "true" ]; then 
    # o todo: Test this script
    robotControllers=(ilw clw)
    # Parallelize only over the swarm size and seeds
    for CONTROLLER in "${robotControllers[@]}"; do
        # --controlmaster -S ensures that the paralellization happens across all specified nodes
        # Swarmsize, seed and the .argos file are specified
        # Within the .argos file, xmlstarlet changes the seed to the chosen value
        # The -l and -e flags when calling argos3 result in that the logs and errors
        # get written into non-existent files, which conveniently gets rid of them
        parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.1 --delay 0.05 '
        CONTROLLER={1};
        SWARMSIZE={2};
        SEED={3};
        ARGOSDIR=experiments/heavy_tailedness/${CONTROLLER}/${SWARMSIZE}N/${SEED};
        ARGOSFILE=$ARGOSDIR/heavy_tailedness_${CONTROLLER}_${SWARMSIZE}N_${SEED}.argos;
        cd /users/jnauta/CollectiveLevyWalk/;
        argos3 -c $ARGOSFILE;
        ' ::: $CONTROLLER ::: ${N[@]} ::: ${seeds[@]}
    done
fi 
echo "Done!"
