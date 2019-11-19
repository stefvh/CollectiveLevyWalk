#!/bin/bash
# Creates the required argos experiment files and result directories
# to analyse the heavy tailedness of the collective trajectory
# executed by a swarm existing of either ilw or clw robots

# Args:
T=$1      # Time length of experiment
L=$2      # Environment size
N=$3      # Sequence of swarm sizes
seeds=$4  # Sequence of seeds to re-run single experiment

# Experiment parameters:
robot_controllers=(ilw clw acrw)
target_types=(sparse patchy)
nested="true"
save_step_lengths="false"

# Directory parameters:
experiment_upper_folder="experiments"
results_upper_folder="/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/results"
mkdir -p $results_upper_folder
argos_name="search_efficiency"
dir_name="search_efficiency_${L}_nested${nested}"
mkdir -p $results_upper_folder/$dir_name
base_experiment_file="${experiment_upper_folder}/base/base.argos"

# Set the environment size
xmlstarlet ed --pf --inplace -u 'argos-configuration//arena/@size' -v "$L, $L, 2" "${base_experiment_file}"


# Execute:
for target_type in "${target_types[@]}"; do
  target_positions_file="targets/targets_${target_type}.txt"
  for robot_controller in "${robot_controllers[@]}"; do
    for swarm_size in $N; do
        for seed in $seeds; do
        folder="${dir_name}/${target_type}/${robot_controller}/${swarm_size}N/${seed}"
        mkdir -p "${experiment_upper_folder}/${folder}"
        mkdir -p "${results_upper_folder}/${folder}"
        # Specify experiment and output files
        experiment_file="${experiment_upper_folder}/${folder}/${argos_name}.argos"
        output_file="${results_upper_folder}/${folder}/${argos_name}"

        # Copy the base .argos file
        cp "${base_experiment_file}" "${experiment_file}"

        # Adapt the variables
        xmlstarlet ed --pf --inplace -u 'argos-configuration//framework/experiment/@random_seed' -v "${seed}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@robot_controller' -v "${robot_controller}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//framework/experiment/@length' -v "${T}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@swarm_size' -v "${swarm_size}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/swarm/@nested' -v "${nested}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/foraging/@target_positions_file' -v "${target_positions_file}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/output/@output_file' -v "${output_file}" "${experiment_file}"
        xmlstarlet ed --pf --inplace -u 'argos-configuration//loop_functions/output/@save_step_lengths' -v "${save_step_lengths}" "${experiment_file}"
        done
    done
  done
done