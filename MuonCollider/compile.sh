#!/bin/bash

# Function to print messages in color
print_in_color() {
    local color="$1"
    local message="$2"
    case $color in
        red) echo -e "\033[31m${message}\033[0m" ;;
        green) echo -e "\033[32m${message}\033[0m" ;;
        yellow) echo -e "\033[33m${message}\033[0m" ;;
        blue) echo -e "\033[34m${message}\033[0m" ;;
        *) echo "${message}" ;;
    esac
}

# Create and move to install directory
print_in_color yellow "Creating install directory..."
mkdir -p exeInstall && cd exeInstall
InstallDir=$(pwd)
cd ..

# Create and move to build directory
print_in_color yellow "Creating build directory..."
mkdir -p buildTemp && cd buildTemp

# Run CMake and make
print_in_color yellow "Running CMake..."
cmake -DWITH_GEANT4_UIVIS=ON -DCMAKE_INSTALL_PREFIX="${InstallDir}" ..
print_in_color yellow "Building project..."
make -j4

# Install the project
print_in_color yellow "Installing project..."
make install
cd ..

# Optionally remove build directory
##rm -rf buildTemp

# Final message
print_in_color green "Now The execution file for GEANT4 muon collider simulation"
print_in_color green "At the [ Install directory : ${InstallDir} ]"

