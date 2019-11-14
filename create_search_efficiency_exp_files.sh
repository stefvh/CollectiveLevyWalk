#!/bin/bash
N=$1
seeds=$2
baseConfigurationFileName="experiments/base/base.argos"

echo "Configuring search efficiency experiments"
targetSearchRobotControllers=(ilw clw acrw)
targetEnvironments=(sparse patchy)
nested="true"
save_trajectory="false"

for k in "${targetEnvironments[@]}"; do 
  for i in "${targetSearchRobotControllers[@]}"; do
    for j in $N; do
      for s in $seeds; do 
        experimentFolderName="experiments/search_efficiency/${k}/${i}/${j}N/${s}"
        mkdir -p "$experimentFolderName"
        outputFolderName="output/search_efficiency/${k}/${i}/${j}N/${s}"
        mkdir -p "$outputFolderName/step_lengths"
        mkdir -p "$outputFolderName/state_counters"
        mkdir -p "$outputFolderName/target_findings"

        experimentName="search_efficiency_${k}_${i}_${j}N_${s}"
        configurationFileName="${experimentFolderName}/${experimentName}.argos"
        cp "$baseConfigurationFileName" "$configurationFileName"

        sed -i "s/\(robot_controller=\"\)[^\"]*\"/\1${i}\"/" "${configurationFileName}"
        sed -i "s/\(swarm_size=\"\)[^\"]*\"/\1${j}\"/" "${configurationFileName}"
        sed -i "s/\(nested=\"\)[^\"]*\"/\1${nested}\"/" "${configurationFileName}"
        sed -i "s/\(target_positions_file=\"\)[^\"]*\"/\1targets\/targets_${k}.txt\"/" "${configurationFileName}"
        sed -i "s/\(output_file=\"\)[^\"]*\"/\1${experimentName}\"/" "${configurationFileName}"
        sed -i "s/\(output_step_lengths_folder=\"\)[^\"]*\"/\1output\/search_efficiency\/${i}\/${j}N\/step_lengths\/\"/" "${configurationFileName}"
        sed -i "s/\(output_state_counters_folder=\"\)[^\"]*\"/\1output\/search_efficiency\/${i}\/${j}N\/state_counters\/\"/" "${configurationFileName}"
        sed -i "s/\(output_target_findings_folder=\"\)[^\"]*\"/\1output\/search_efficiency\/${i}\/${j}N\/target_findings\/\"/" "${configurationFileName}"
        sed -i "s/\(save_trajectory=\"\)[^\"]*\"/\1${save_trajectory}\"/" "${configurationFileName}"
    done 
  done
done