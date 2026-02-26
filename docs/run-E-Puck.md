# 🤖 Contrôleur E-Puck ROS 2 (C++) - Guide Complet

Ce projet permet de piloter un robot e-puck dans **Webots** en utilisant **ROS 2 Jazzy**. Le contrôleur utilise une logique d'évitement d'obstacles basée sur les capteurs de proximité et communique via des messages synchronisés (`TwistStamped`).

---

## 📁 Structure du Projet
* **Package :** `mon_robot_cpp`
* **Nœud :** `epuck_controller`
* **Type de message :** `geometry_msgs/msg/TwistStamped`
* **Langage :** C++17

---

## 🛠 1. Installation et Compilation

### Création du package (si non existant)
```bash
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_cmake mon_robot_cpp --dependencies rclcpp geometry_msgs sensor_msgs
```

## Compilation
Chaque fois que vous modifiez le code .cpp ou le CMakeLists.txt, exécutez ces commandes :

```bash
cd ~/ros2_ws
colcon build --packages-select mon_robot_cpp
source install/setup.bash
```

## 🚀 2. Lancement du Robot
Le fonctionnement nécessite deux terminaux actifs simultanément.

### Étape 1 : Lancer la simulation (Terminal 1)

```bash
source /opt/ros/jazzy/setup.bash
ros2 launch webots_ros2_epuck robot_launch.py
```

### Étape 2 : Lancer votre contrôleur (Terminal 2)
Note cruciale : Le paramètre use_sim_time:=true est obligatoire pour synchroniser l'horloge de votre programme avec celle de la simulation Webots.

```bash
cd ~/ros2_ws
source install/setup.bash
ros2 run mon_robot_cpp mon_cerveau_cpp --ros-args -p use_sim_time:=true
```

### 🔍 3. Outils de Debugging (Commandes utiles)

#### Vérifier la connexion entre les nœuds
Pour s'assurer que votre programme et Webots se "parlent" :

```bash
ros2 topic info /cmd_vel --verbose
```
**Succès si : Publisher count: 1 (votre code) et Subscription count: 1 (Webots).

#### Voir les flux de données
Ordres de vitesse sortants : ros2 topic echo /cmd_vel

Données des capteurs entrants : ros2 topic echo /ps0

#### Test forcé des moteurs
Envoyer une commande manuelle pour tester la réactivité du robot :**

### 💡 4. Concepts Clés
Pourquoi TwistStamped au lieu de Twist ?
Dans ROS 2 Jazzy, le contrôleur de mouvement de l'e-puck (diffdrive_controller) exige une structure Stamped.

Twist : Contient uniquement les vecteurs de vitesse.

TwistStamped : Contient les vecteurs + un Header avec l'heure exacte. Sans ce timestamp, le robot ignore la commande par sécurité.

Pourquoi use_sim_time ?
Si votre code utilise l'heure réelle (ex: 2026), Webots rejettera le message car il "vit" dans le passé (temps de simulation commençant à 0). Ce paramètre force votre code à lire l'horloge de Webots.

### 📄 5. Configuration CMakeLists.txt
Votre fichier doit impérativement lier ces dépendances pour compiler :

```bash
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(mon_cerveau_cpp src/intelligent_robot.cpp)
ament_target_dependencies(mon_cerveau_cpp rclcpp geometry_msgs sensor_msgs)

install(TARGETS mon_cerveau_cpp DESTINATION lib/${PROJECT_NAME})
```

### 6. Le suivi des capteurs

```bash
ros2 run rqt_plot rqt_plot
```
