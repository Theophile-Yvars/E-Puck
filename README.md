# E-Puck

![Demo](assets/demo.gif)

## Documentation sur le e-puck

https://cyberbotics.com/doc/guide/epuck?version=R2022b

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

## Installation

### Créer le workspace et cloner le projet
Le projet utilise l'outil colcon pour la compilation C++.

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/Theophile-Yvars/ROS2-E-Puck.git E-Puck
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

## Astuces Utiles

Ajouter ROS 2 au démarrage : echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc

Lister les capteurs actifs : ros2 topic list

Vérifier les données d'un capteur : ros2 topic echo /ps0

Pour s'assurer que votre programme et Webots se "parlent" (**Succès si : Publisher count: 1 (votre code) et Subscription count: 1 (Webots).) : ros2 topic info /cmd_vel --verbose

Le suivi des capteurs : ros2 run rqt_plot rqt_plot

Ordres de vitesse sortants : ros2 topic echo /cmd_vel

## Architecture logicielle

Le contrôleur repose sur une architecture asynchrone et événementielle :

Nœud : intelligent_robot_cpp

Entrées (Subscribers) : Écoute les capteurs de proximité /ps0 à /ps7.

Sorties (Publisher) : Envoie des vecteurs de vitesse sur /cmd_vel.

Logique : Utilise des Callbacks pour mettre à jour l'état interne du robot sans bloquer la boucle de contrôle principale.
