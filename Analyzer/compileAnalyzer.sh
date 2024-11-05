#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/releases/LCG_94/ROOT/6.14.04/x86_64-centos7-gcc7-opt/ROOT-env.sh

g++ -std=c++17 AnalysisScript.cpp `root-config --cflags --libs` -o AnalyzeNtuple
./AnalyzeNtuple

rm -rf AnalyzeNtuple
