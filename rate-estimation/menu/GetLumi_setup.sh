#!/bin/sh
rm -f json_DCSONLY.txt
wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/DCSOnly/json_DCSONLY.txt
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH
