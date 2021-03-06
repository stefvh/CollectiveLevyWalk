#!/bin/bash
# Creates the required argos experiment files and result directories
# to analyse the heavy tailedness of the collective trajectory
# executed by a swarm existing of either ilw or clw robots

# Args:
T=$1      # Time length of experiment
N=$2      # Sequence of swarm sizes
seeds=$3  # Sequence of seeds to re-run single experiment

# Directory parameters:
experiment_upper_folder="experiments"
results_upper_folder="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/results"
title="heavy_tailedness"
base_experiment_file="${experiment_upper_folder}/base/base.argos"

# Experiment parameters:
robot_controllers=(ilw clw)
nested="false"
save_step_lengths="true"

# Execute:
for robot_controller in "${robot_controllers[@]}"; do
  # Remove all 
  for swarm_size in $N; do
    for seed in $seeds; do
      folder="${title}/${robot_controller}/${swarm_size}N/${seed}"
      mkdir -p "${experiment_upper_folder}/${folder}"
      mkdir -p "${results_upper_folder}/${folder}"

      experiment_title="${title}"
      experiment_file="${experiment_upper_folder}/${folder}/${experiment_title}.argos"

      # Copy the base .argos file
      cp "${base_experiment_file}" "${experiment_file}"

      # Adapt variables
      xmlstarlet ed --pf --inplace -u 'argos-configuration//framework/experiment/@random_seed' -v "${seed}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@robot_controller' -v "${robot_controller}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//framework/experiment/@length' -v "${T}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@swarm_size' -v "${swarm_size}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@nested' -v "${nested}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/output/@output_file' -v "${results_upper_folder}/${folder}/${experiment_title}" "${experiment_file}"
      xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/output/@save_step_lenghts' -v "${save_step_lenghts}" "${experiment_file}"
    done
  done 
done

