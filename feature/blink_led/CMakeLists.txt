cmake_minimum_required(VERSION 3.8)
project(mon_robot_cpp_blink_led)

# 1. Standard C++ (Requis pour ROS 2 Jazzy)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# 2. Recherche des packages ROS 2 nécessaires
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# 3. Création de l'exécutable
# On crée le programme "mon_cerveau_cpp_blink_led" à partir du fichier source
add_executable(mon_cerveau_cpp_blink_led src/blink_led.cpp)

# 4. Définition des dépendances cibles
# Cela lie ton code aux bibliothèques rclcpp, geometry_msgs (pour TwistStamped) et sensor_msgs (pour Range)
ament_target_dependencies(mon_cerveau_cpp_blink_led
  rclcpp
  geometry_msgs
  sensor_msgs
)

# 5. Installation de l'exécutable
# Permet à la commande 'ros2 run' de trouver ton programme après la compilation
install(TARGETS
  mon_cerveau_cpp_blink_led
  DESTINATION lib/${PROJECT_NAME}
)

# 6. Fin du package
ament_package()
