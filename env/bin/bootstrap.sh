#!/usr/bin/bash

echo "This script installs garnet build dependencies to the current host Ubuntu 22.04. To stop, press Ctrl-C now. Or press ENTER to continue..."
read

# Setup Vulkan SDK repo
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.275-jammy.list https://packages.lunarg.com/vulkan/1.3.275/lunarg-vulkan-1.3.275-jammy.list

# install build dependencies 
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    git-lfs \
    ninja-build \
    vulkan-sdk \
    clang-15 \
    clang-format-12 

# install python modules
python3 -m pip install --upgrade pip
python3 -m pip install --upgrade termcolor