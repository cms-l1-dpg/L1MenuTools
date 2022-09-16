#!/bin/sh
rm -f json_DCSONLY.txt
wget https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/DCSOnly_JSONS/Cert_Collisions2022_355100_357900_13p6TeV_DCSOnly_TkPx.json
export PATH=$HOME/.local/bin:/cvmfs/cms-bril.cern.ch/brilconda/bin:$PATH
