#!/bin/bash
# Creates the required argos experiment file(s) and output directories
# to analyse the heavy tailedness of the collective trajectory
# executed by a swarm existing of either ilw or clw robots

# Params from args:
N=$1      # Maximum swarmsize to be investigated
Ni=$2     # Increments in sequence of swarmsize up to N
seeds=$3  # Sequence of seeds to re-run single experiment

# Params default (file-related):
experiment_upper_folder="experiments"
results_upper_folder="results"
title="search_efficiency"
base_experiment_file="${experiment_upper_folder}/base/base.argos"

# Params default (experiment-related):
robot_controllers=(ilw clw acrw)
nested="true"
target_types=(sparse patchy)
target_positions_files=( "${target_types[@]/#/targets/targets_}" )
target_positions_files=( "${target_positions_files[@]/%/.txt}" )
save_step_lengths="false"

#    xmlstarlet ed --inplace -u 'argos-configuation//framework/experiment/@random_seed' -v $SEED $ARGOSFILE;

# Execute:
for target_type in "${target_types[@]}"
do
  for robot_controller in "${robot_controllers[@]}"
  do
    for swarm_size in $(seq $Ni $Ni $N)
    do
        for seed in "${seeds[@]}"
        do
        folder="${title}/${target_type}/${robot_controller}/${swarm_size}N/${seed}"
        printf -v folder_escaped "%q" "${folder}"
        mkdir -p "${experiment_upper_folder}/${folder}"
        mkdir -p "${results_upper_folder}/${folder}"

        experiment_title="${title}_${target_type}_${robot_controller}_${swarm_size}N_${seed}"
        experiment_file="${experiment_upper_folder}/${folder}/${experiment_title}.argos"
        cp "${base_experiment_file}" "${experiment_file}"

        xmlstarlet ed --inplace -u 'argos-configuation//loop_functions/swarm/@swarm_size' -v "${swarm_size}" "${experiment_file}"
        done
    done
  done
done