#!/bin/bash
# Script for running search efficiency experiments in parallel
T=5000      # Number of seconds
MAX_N=750   # Maximum swarm size
DELTA_N=50  # Delta swarm size
L=170       # Environment size
nseeds=30   # Number of seeds
N=$(seq $MAX_N -$DELTA_N $DELTA_N)
seeds=$(seq 1 1 $nseeds)

# Specify paths
DIR="CollectiveLevyWalk"
DIRNAME="search_efficiency"
SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy"
OUTPUT_DIR="$SHARED_DIR/$DIR/output"

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

if [ "$COPY" = true ]; then 
    echo "Copying/distributing code across available nodes..."
    # Generate output directories
    ./experiments/create_search_efficiency_experiment_files.sh $T $L "${N[@]}" "${seeds[@]}"
    ARCHIVE="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/$DIRNAME.tar.gz"
    LOCAL_DIR="/users/jnauta"
    # Archive the files
    tar -C $SHARED_DIR --exclude='CollectiveLevyWalk/results' --exclude='CollectiveLevyWalk/build/CMakeCache.txt' -czf $ARCHIVE $DIR
    while IFS= read -r dest; do 
        # Copy the archive to each node
        echo $dest; scp -q $ARCHIVE $dest:$LOCAL_DIR
    done <nodes.txt
fi 

if [ "$BUILD" = true ]; then 
    echo "Building on all nodes..."
    parallel --nonall -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 'tar -xzf search_efficiency.tar.gz; cd CollectiveLevyWalk; if [[ ! -e build ]]; then mkdir build; fi; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make'
fi

## MAIN PARALLEL SCRIPT
start=$SECONDS 
if [ "$RUN" = true ]; then 
    controllers=(acrw clw ilw)
    distributions=(sparse patchy)
    for CONTROLLER in "${controllers[@]}"; do 
        cd $SHARED_DIR/$DIR/
        parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 1 --delay 0.25 '
        CONTROLLER={1};
        TARGET={2};
        SEED={3};
        SWARMSIZE={4};
        ARGOSDIR=experiments/search_efficiency/${TARGET}/${CONTROLLER}/${SWARMSIZE}N/${SEED};
        ARGOSFILE=$ARGOSDIR/search_efficiency.argos;
        cd /users/jnauta/CollectiveLevyWalk/;
        argos3 -l $ARGOSDIR/log -e $ARGOSDIR/logerr -c $ARGOSFILE;
        echo Experiment done for $CONTROLLER $SWARMSIZE N and seed $SEED within $TARGET environment;
        ' ::: $CONTROLLER ::: ${distributions[@]} ::: ${seeds[@]} ::: ${N[@]}
        cd $SHARED_DIR/$DIR/
        duration=$((SECONDS - start ))
        echo "Done executing $CONTROLLER, after $duration s"
    done 
fi 

duration=$((SECONDS - start ))
echo "Simulations finished after approximately $duration s, archiving results..."

# Archive the results
RESULT_DIR="$SHARED_DIR/$DIR/results/"
tar -czf "$SHARED_DIR/search_efficiency_results_${L}.tar.gz" -C "$RESULT_DIR" "${DIRNAME}_${L}"
echo "Done."

