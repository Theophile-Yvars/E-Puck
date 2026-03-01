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




