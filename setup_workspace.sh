#!/bin/bash

# 1. Définition du nom du workspace
WS_NAME="ros2_ws"

echo "Configuration du Workspace ROS 2 : $WS_NAME"

# 2. Création de l'arborescence (le dossier src est la clé !)
cd ~
mkdir -p $WS_NAME/src

# 3. Déplacement du package dans le dossier src
# On suppose que l'utilisateur a cloné le repo dans son dossier perso
if [ -d "~/E-Puck" ]; then
    mv ~/E-Puck ~/$WS_NAME/src/
    echo "Package déplacé dans $WS_NAME/src/"
fi