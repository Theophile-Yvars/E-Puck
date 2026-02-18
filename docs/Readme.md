# Projet E-Puck : Autonomie avec ROS 2 Jazzy & Webots

Ce projet implémente un contrôleur de robot autonome en **C++** pour le robot e-puck dans le simulateur **Webots**. Il utilise l'architecture événementielle de **ROS 2 Jazzy** sur Ubuntu 24.04.

---

## 1. Prérequis & Installation

### Système requis
* **OS :** Ubuntu 24.04 LTS (Noble Numbat)
* **Simulateur :** Webots
* **Middleware :** ROS 2 Jazzy Jalisco

### Installation de ROS 2 et des outils C++
Exécutez ces commandes pour configurer les dépôts et installer l'écosystème de build :

```bash
# Configuration des dépôts ROS 2
sudo apt update && sudo apt install curl gnupg2 lsb-release -y
sudo curl -sSL [https://raw.githubusercontent.com/ros/rosdistro/master/ros.key](https://raw.githubusercontent.com/ros/rosdistro/master/ros.key) -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] [http://packages.ros.org/ros2/ubuntu](http://packages.ros.org/ros2/ubuntu) $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Installation de ROS 2 Jazzy et des outils de compilation
sudo apt update
sudo apt install ros-jazzy-desktop ros-dev-tools build-essential cmake -y
```

### Installation du driver Webots

```bash
sudo apt install ros-jazzy-webots-ros2-epuck -y
```

## 2. Configuration du Workspace
Le projet utilise l'outil colcon pour la compilation C++.

```bash
# Créer l'espace de travail
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src

# Créer le package avec les dépendances nécessaires
ros2 pkg create --build-type ament_cmake mon_robot_cpp --dependencies rclcpp geometry_msgs sensor_msgs

# Compiler
cd ~/ros2_ws
colcon build
```

## 3. Lancement du Projet
Chaque terminal doit être "sourcé" pour reconnaître les commandes ROS 2.

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
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 run mon_robot_cpp mon_cerveau_cpp
```

4. Architecture logicielle
Le contrôleur repose sur une architecture asynchrone et événementielle :

Nœud : intelligent_robot_cpp

Entrées (Subscribers) : Écoute les capteurs de proximité /ps0 à /ps7.

Sorties (Publisher) : Envoie des vecteurs de vitesse sur /cmd_vel.

Logique : Utilise des Callbacks pour mettre à jour l'état interne du robot sans bloquer la boucle de contrôle principale.

5. Astuces Utiles
Ajouter ROS 2 au démarrage : echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc

Lister les capteurs actifs : ros2 topic list

Vérifier les données d'un capteur : ros2 topic echo /ps0
