#!/bin/bash
# Script for running the desired experiment in parallel
T=5000      # Number of seconds
MAX_N=1000  # Maximum swarm size
DELTA_N=50 # Delta swarm size
nseeds=20   # Number of seeds
L=20        # Environment size
N=$(seq $MAX_N -$DELTA_N $DELTA_N)
seeds=$(seq 31 1 $((30+$nseeds)))
controllers=(clw ilw)

# Specify paths
DIR="CollectiveLevyWalk"
DIRNAME="heavy_tailedness"
SHARED_DIR="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy"
OUTPUT_DIR="$SHARED_DIR/$DIR/output"

# Variables
COPY=true
BUILD=true 
RUN=true
ANALYZE=true

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
    ./experiments/create_heavy_tailedness_experiment_files.sh $T $L "${N[@]}" "${seeds[@]}"
    ARCHIVE="${SHARED_DIR}/${DIRNAME}_${L}.tar.gz"
    LOCAL_DIR="/users/jnauta/"
    # Archive the files necessary
    tar -C $SHARED_DIR --exclude='CollectiveLevyWalk/results' -czf $ARCHIVE $DIR
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
    parallel --nonall -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 'tar -xzf heavy_tailedness_{1}.tar.gz; cd CollectiveLevyWalk; if [[ ! -e build ]]; then mkdir build; fi; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make' ::: $L
fi

# MAIN PARALLEL SCRIPT
start=$SECONDS
if [ "$RUN" = true ]; then 
    # Parallelize only over the swarm size and seeds
    for CONTROLLER in "${controllers[@]}"; do
        # --controlmaster -S ensures that the paralellization happens across all specified nodes
        # Swarmsize, seed and the .argos file are specified
        # Within the .argos file, xmlstarlet changes the seed to the chosen value
        # The -l and -e flags when calling argos3 result in that the logs and errors
        # get written into non-existent files, which conveniently gets rid of them
        cd $SHARED_DIR/$DIR/
        parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.05 --delay 0.05 '
        L={1};
        CONTROLLER={2};
        SEED={3};
        SWARMSIZE={4};
        ARGOSDIR=experiments/heavy_tailedness_${L}/${CONTROLLER}/${SWARMSIZE}N/${SEED};
        ARGOSFILE=$ARGOSDIR/heavy_tailedness.argos;
        cd /users/jnauta/CollectiveLevyWalk/;
        argos3 -l $ARGOSDIR/log -e $ARGOSDIR/logerr -c $ARGOSFILE;
        echo Experiment done for $CONTROLLER $SWARMSIZE N and seed $SEED;
        ' ::: $L ::: $CONTROLLER ::: ${seeds[@]} ::: ${N[@]} 
        cd $SHARED_DIR/$DIR/
        duration=$(( SECONDS - start ))
        echo "Done executing $CONTROLLER, after $duration s"
    done
fi

duration=$(( SECONDS - start ))
echo "Simulations finished after approximately $duration s, archiving results..."

# ANALYZE THE HEAVY TAILEDNESS
if [ "$ANALYZE" = true ]; then
    cd $SHARED_DIR/$DIR
    echo "Analyzing step length distributions..."
    FILE="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/analyze.py"
    start=$SECONDS 
    parallel -S jnauta@node0,jnauta@node1,jnauta@node2,jnauta@node3,jnauta@node4 --sshdelay 0.05 --delay 0.05 '
    FILE={5};
    python -W ignore {5} --c {1} --N {2} --s {3} --L {4};
    ' ::: ${controllers[@]} ::: ${N[@]} ::: ${seeds[@]} ::: $L ::: $FILE
    duration=$(( SECONDS - start ))
    echo "Analysis of heavy tailedness finished after approximately $duration s"
fi 

# Copy the results to another directory
cd $SHARED_DIR/$DIR
RESULT_DIR="$SHARED_DIR/$DIR/results/"
tar -czf "$SHARED_DIR/heavy_tailedness_results_$L.tar.gz" -C "$RESULT_DIR" "${DIRNAME}_${L}"
tar -czf "$OUTPUT_DIR/${DIRNAME}_${L}.tar.gz" -C "$OUTPUT_DIR" "${DIRNAME}_${L}" 
echo "Done."
