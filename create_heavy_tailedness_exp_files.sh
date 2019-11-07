#!/bin/bash
N=$1
seeds=$2
baseConfigurationFileName="experiments/base/base.argos"

echo "Configuring heavy-tailedness experiments"
heavyTailednessRobotControllers=(ilw clw)
nested="false"
target_positions_file=""
save_trajectory="true"

for i in "${heavyTailednessRobotControllers[@]}"; do
  for j in $N; do
    for s in $seeds; do 
      experimentFolderName="experiments/heavy_tailedness/${i}/${j}N/${s}"
      mkdir -p "$experimentFolderName"
      outputFolderName="output/heavy_tailedness/${i}/${j}N/${s}"
      mkdir -p "$outputFolderName/step_lengths"
      mkdir -p "$outputFolderName/state_counters"

      experimentName="heavy_tailedness_${i}_${j}N_${s}"
      configurationFileName="${experimentFolderName}/${experimentName}.argos"
      cp "$baseConfigurationFileName" "$configurationFileName"

      sed -i "s/\(robot_controller=\"\)[^\"]*\"/\1${i}\"/" "${configurationFileName}"
      sed -i "s/\(swarm_size=\"\)[^\"]*\"/\1${j}\"/" "${configurationFileName}"
      sed -i "s/\(nested=\"\)[^\"]*\"/\1${nested}\"/" "${configurationFileName}"
      sed -i "s/\(target_positions_file=\"\)[^\"]*\"/\1${target_positions_file}\"/" "${configurationFileName}"
      sed -i "s/\(output_file=\"\)[^\"]*\"/\1${experimentName}\"/" "${configurationFileName}"
      sed -i "s/\(output_step_lengths_folder=\"\)[^\"]*\"/\1output\/heavy_tailedness\/${i}\/${j}N\/${s}\/step_lengths\/\"/" "${configurationFileName}"
      sed -i "s/\(output_state_counters_folder=\"\)[^\"]*\"/\1output\/heavy_tailedness\/${i}\/${j}N\/${s}\/state_counters\/\"/" "${configurationFileName}"
      sed -i "s/\(output_target_findings_folder=\"\)[^\"]*\"/\1\"/" "${configurationFileName}"
      sed -i "s/\(save_trajectory=\"\)[^\"]*\"/\1${save_trajectory}\"/" "${configurationFileName}"
  done 
done


