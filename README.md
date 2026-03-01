# E-Puck

![Demo](assets/demo.gif)

## Installation de l'environnement

Tous les détails se trouvent dans ce [Document](./docs/Readme.md)

## E-Puck ROS 2 Controller

Ce dépôt contient le package ROS 2 pour le robot e-puck. 
**Attention :** Pour compiler ce projet, il doit impérativement être placé dans un dossier `src/` au sein d'un workspace ROS 2.

### Structure attendue
Pour que `colcon build` fonctionne, votre dossier doit ressembler à ceci :
```text
ros2_ws/
└── src/
    └── E-Puck/ (Ce dépôt)
        ├── CMakeLists.txt
        ├── package.xml
        └── src/
            └── intelligent_robot.cpp
```

## Installation Rapide
### Créer le workspace et cloner le projet :

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone [https://github.com/Theophile-Yvars/ROS2-E-Puck.git](https://github.com/Theophile-Yvars/ROS2-E-Puck.git) E-Puck
```
### Compiler le projet :

```bash
cd ~/ros2_ws
colcon build --packages-select mon_robot_cpp
source install/setup.bash
```

### Lancement
Terminal 1 : La Simulation
Lance Webots et le bridge ROS 2 :

```bash
source /opt/ros/jazzy/setup.bash
ros2 launch webots_ros2_epuck robot_launch.py
```

Terminal 2 : Le Contrôleur C++
Compile et lance ton code :

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run mon_robot_cpp mon_cerveau_cpp --ros-args -p use_sim_time:=true
```

### Debugging
Si le robot ne bouge pas, vérifiez la correspondance des messages :

Topic : /cmd_vel

Type : geometry_msgs/msg/TwistStamped
