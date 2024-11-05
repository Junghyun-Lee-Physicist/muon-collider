#!/bin/bash

# Unpack the tar.gz file
tar -xzf "MuonCollider.tar.gz"

cd "MuonCollider"

# Source the environment setup script
source "envSetup.sh"

# Run the compile script
source "compile.sh"

# Navigate to the install directory
cd "exeInstall"

# Run the simulation with the appropriate seed number
seed=$1
echo "Setted Seed [ ${seed} ]"
./ExeMuonCollider -m run.mac -s $seed

# Move the output file to the storage directory
output_file="ntupleMuonCollider_${seed}.root"
Analyzer_outout_file="analyzerResult_${seed}.root"

mv ${Analyzer_outout_file} "/u/user/jhlee/Storage/MuonCollider240620/"

