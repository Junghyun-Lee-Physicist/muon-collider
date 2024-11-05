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

echo ""
print_in_color yellow "GEANT4 Environment Setup --------------------"
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8.3.0/x86_64-centos7/setup.sh
export CXX=`which g++`
export CC=`which gcc`
source /cvmfs/geant4.cern.ch/geant4/10.7.p02/x86_64-centos7-gcc8-optdeb/CMake-setup.sh
export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.14.2/Linux-x86_64/bin:$PATH
G4VersionCheck=`geant4-config --version`
print_in_color green "GEANT4 version [ $G4VersionCheck ] is ready to simulate"

echo ""
print_in_color yellow "ROOT Environment Setup ----------------------"
source /cvmfs/sft.cern.ch/lcg/releases/LCG_96b/ROOT/6.18.04/x86_64-centos7-gcc8-opt/ROOT-env.sh
ROOTVersionCheck=`root-config --version`
print_in_color green "ROOT version [ $ROOTVersionCheck ] is ready to use"

