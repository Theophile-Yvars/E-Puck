# Projet E-Puck : Autonomie avec ROS 2 Jazzy & Webots

Ce projet implémente un contrôleur de robot autonome en **C++** pour le robot e-puck dans le simulateur **Webots**. Il utilise l'architecture événementielle de **ROS 2 Jazzy** sur Ubuntu 24.04.

---

## 1. Prérequis & Installation

### Système requis
* **OS :** Ubuntu 24.04 LTS (Noble Numbat)
* **Simulateur :** Webots
* **Middleware :** ROS 2 Jazzy Jalisco

### Installation de Webots

https://cyberbotics.com/

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
```

## 3. Architecture logicielle
Le contrôleur repose sur une architecture asynchrone et événementielle :

Nœud : intelligent_robot_cpp

Entrées (Subscribers) : Écoute les capteurs de proximité /ps0 à /ps7.

Sorties (Publisher) : Envoie des vecteurs de vitesse sur /cmd_vel.

Logique : Utilise des Callbacks pour mettre à jour l'état interne du robot sans bloquer la boucle de contrôle principale.

## 4. Astuces Utiles
Ajouter ROS 2 au démarrage : echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc

Lister les capteurs actifs : ros2 topic list

Vérifier les données d'un capteur : ros2 topic echo /ps0

Pour s'assurer que votre programme et Webots se "parlent" (**Succès si : Publisher count: 1 (votre code) et Subscription count: 1 (Webots).) : ros2 topic info /cmd_vel --verbose

Le suivi des capteurs : ros2 run rqt_plot rqt_plot

Ordres de vitesse sortants : ros2 topic echo /cmd_vel
