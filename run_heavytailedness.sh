#!/bin/bash
# Script for running the desired experiment in parallel
T=5000      # Number of seconds
MAX_N=1000  # Maximum swarm size
DELTA_N=100 # Delta swarm size
nseeds=30   # Number of seeds
N=$(seq $MAX_N -$DELTA_N $DELTA_N)
seeds=$(seq 1 1 $nseeds)

# Specify paths
DIR="CollectiveLevyWalk"
DIRNAME="heavy_tailedness"
SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy"

# Variables
COPY=true
BUILD=true
RUN=true

# GNU parallel magic
sudo localedef -i en_US -f UTF-8 en_US.UTF-8;
export LANGUAGE=en_US.UTF-8;
export LANG=en_US.UTF-8;
export LC_ALL=en_US.UTF-8;
sudo locale-gen en_US.UTF-8;

# Distribute the codes over the available nodes
if [ "$COPY" = true ]; then     
    echo "Copying/distributing code across nodes..."
    # Generate output directories for data to be stored
    ./create_heavy_tailedness_experiment_files.sh $T "${N[@]}" "${seeds[@]}"
    ARCHIVE="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/$DIRNAME.tar.gz"
    LOCAL_DIR="/users/jnauta/"
    # Archive the files necessary
    tar -C $SHARED_DIR -czf $ARCHIVE $DIR
    while IFS= read -r dest; do 
        # Copy the archive
        echo $dest 
        scp -q $ARCHIVE $dest:$LOCAL_DIR
    done <nodes.txt
fi 

#,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4
if [ "$BUILD" = true ]; then 
    echo "Building on all nodes..."
    # Build the project on each node
    parallel --nonall -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 'rm -rf heavy_tailedness; tar -xzf heavy_tailedness.tar.gz; cd heavy_tailedness; if [[ ! -e build ]]; then mkdir build; fi; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make'
fi

# MAIN PARALLEL SCRIPT
start=$SECONDS
if [ "$RUN" = true ]; then 
    # o todo: Test this script
    robotControllers=(ilw clw)
    # Parallelize only over the swarm size and seeds
    for CONTROLLER in "${robotControllers[@]}"; do
        # --controlmaster -S ensures that the paralellization happens across all specified nodes
        # Swarmsize, seed and the .argos file are specified
        # Within the .argos file, xmlstarlet changes the seed to the chosen value
        # The -l and -e flags when calling argos3 result in that the logs and errors
        # get written into non-existent files, which conveniently gets rid of them
        cd $SHARED_DIR/$DIR/
        parallel -S 12/jnauta@node0,12/jnauta@node1,12/jnauta@node2,12/jnauta@node3,12/jnauta@node4 --sshdelay 0.25 --delay 0.5 '
        CONTROLLER={1};
        SEED={2};
        SWARMSIZE={3};
        ARGOSDIR=experiments/heavy_tailedness/${CONTROLLER}/${SWARMSIZE}N/${SEED};
        ARGOSFILE=$ARGOSDIR/heavy_tailedness.argos;
        cd /users/jnauta/CollectiveLevyWalk/;
        argos3 -l $ARGOSDIR/log -e $ARGOSDIR/logerr -c $ARGOSFILE;
        echo Experiment done for $CONTROLLER $SWARMSIZE N and seed $SEED;
        ' ::: $CONTROLLER ::: ${seeds[@]} ::: ${N[@]} 
        cd $SHARED_DIR/$DIR/
        # Print out some time indication
        duration=$((SECONDS - start ))
        echo "Finished $CONTROLLER, after $duration s"
    done
fi 

duration=$(( SECONDS - start ))
echo "Simulations finished after approximately $duration s, archiving results..."

# Copy the results to another directory
RESULT_DIR="$SHARED_DIR/$DIR/"
tar -C $RESULT_DIR -czf $SHARED_DIR/results.tar.gz "$RESULT_DIR/results/"
echo "Done."