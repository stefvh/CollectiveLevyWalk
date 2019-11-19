#!/bin/bash
# Script for running search efficiency experiments in parallel
T=5000      # Number of seconds
MIN_N=50
MAX_N=1000  # Maximum swarm size
DELTA_N=50  # Delta swarm size
L=170       # Environment size
nseeds=30   # Number of seeds
N=$(seq $MAX_N -$DELTA_N $MIN_N)
seeds=$(seq 1 1 $nseeds)
# seeds=$(seq 31 1 $((30+$nseeds)))
controllers=(clw)
distributions=(sparse patchy)

# Specify paths
DIR="CollectiveLevyWalk"
DIRNAME="search_efficiency"
SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy"
OUTPUT_DIR="$SHARED_DIR/$DIR/output"
HOME_DIR="/users/jnauta/"

# Variables
COPY=true
BUILD=true
RUN=true
ARCHIVE=false

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
    ARCHIVE="${SHARED_DIR}/${DIRNAME}_${L}.tar.gz"
    LOCAL_DIR="/users/jnauta"
    # Archive the files
    tar -C $SHARED_DIR --exclude='CollectiveLevyWalk/results' -czf $ARCHIVE $DIR
    while IFS= read -r dest; do 
        # Copy the archive to each node
        echo $dest; scp -q $ARCHIVE $dest:$LOCAL_DIR
    done <nodes.txt
fi 

if [ "$BUILD" = true ]; then 
    echo "Building on all nodes..."
    parallel --nonall -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 'tar -xzf search_efficiency_{1}.tar.gz; cd CollectiveLevyWalk; if [[ ! -e build ]]; then mkdir build; fi; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make' ::: $L
fi

## MAIN PARALLEL SCRIPT
start=$SECONDS 
if [ "$RUN" = true ]; then 
    for CONTROLLER in "${controllers[@]}"; do 
        cd $HOME_DIR/$DIR
        parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.05 --delay 0.05 '
        L={1};
        CONTROLLER={2};
        TARGET={3};
        SEED={4};
        SWARMSIZE={5};
        DIR_STRUCT=search_efficiency_${L}_nestedtrue_${CONTROLLER}/${TARGET}/${CONTROLLER}/${SWARMSIZE}N/${SEED}
        ARGOSDIR=experiments/$DIR_STRUCT;
        RESULTSDIR=results/$DIR_STRUCT
        ARGOSFILE=$ARGOSDIR/search_efficiency.argos;
        cd /users/jnauta/CollectiveLevyWalk/;
        cp $ARGOSFILE /groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/$RESULTSDIR;
        argos3 -l $ARGOSDIR/log -e $ARGOSDIR/logerr -c $ARGOSFILE;
        echo Experiment done for $CONTROLLER $SWARMSIZE N and seed $SEED within $TARGET environment;
        ' ::: $L ::: $CONTROLLER ::: ${distributions[@]} ::: ${seeds[@]} ::: ${N[@]}
        cd $HOME_DIR/$DIR
        duration=$((SECONDS - start ))
        echo "Done executing $CONTROLLER, after $duration s"
    done 
fi 

duration=$((SECONDS - start ))
echo "Simulations finished after approximately $duration s."

# Archive the results
if [ "$ARCHIVE" = true ]; then 
    echo "Archiving results..."
    cd $SHARED_DIR/$DIR
    RESULT_DIR="$SHARED_DIR/$DIR/results/"
    tar -czf "$SHARED_DIR/search_efficiency_results_${L}.tar.gz" -C "$RESULT_DIR" "${DIRNAME}_${L}"
    echo "Done."
fi

