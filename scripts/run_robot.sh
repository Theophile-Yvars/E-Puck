#!/bin/bash

# Compiler le projet
echo "Compilation du package..."
cd ~/ros2_ws
colcon build --packages-select mon_robot_cpp

# Sourcer l'environnement
source install/setup.bash

# Lancer le contrôleur avec les bons arguments
echo "Lancement du contrôleur C++..."
ros2 run mon_robot_cpp mon_cerveau_cpp --ros-args -p use_sim_time:=true
