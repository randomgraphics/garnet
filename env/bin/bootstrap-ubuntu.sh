#!/usr/bin/bash
set -eo pipefail

echo "This script installs garnet build dependencies to the current host Ubuntu 22.04+. To stop, press Ctrl-C now. Or press ENTER to continue..."
read

# install build dependencies 
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    clang \
    cmake \
    git \
    git-lfs \
    ninja-build \
    python3-dev \
    python3-pip \
    pkg-config \
    libxkbcommon-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
