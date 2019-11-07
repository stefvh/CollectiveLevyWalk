#!/bin/bash
# Script for running the desired experiment in single
MAX_N=1000  # Maximum swarm size
DELTA_N=200  # Delta swarm size
nseeds=30   # Number of seeds
seeds=($(seq 1 1 $nseeds))

/bin/bash create_heavy_tailedness_experiment_files.sh $MAX_N $DELTA_N $nseeds
robot_controllers=(ilw clw)
experiment_upper_folder="experiments"
title="heavy_tailedness"

for robot_controller in "${robot_controllers[@]}"
do
  for swarm_size in $(seq $DELTA_N $DELTA_N $MAX_N)
  do
    for seed in "${seeds[@]}"
    do
      folder="${title}/${robot_controller}/${swarm_size}N/${seed}"
      experiment_title="${title}_${robot_controller}_${swarm_size}N_${seed}"
      experiment_file="${experiment_upper_folder}/${folder}/${experiment_title}.argos"

      argos3 -c $experiment_file
    done
  done
done