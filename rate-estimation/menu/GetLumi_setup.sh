#!/bin/sh
rm -f json_DCSONLY.txt
wget https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions18/13TeV/DCSOnly/json_DCSONLY.txt
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH
