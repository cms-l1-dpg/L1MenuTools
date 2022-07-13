#!/bin/bash
echo "Starting job on " `date` #Date/time of start of job
echo "Running on: `uname -a`" #Condor job is running on this node
echo "System software: `cat /etc/redhat-release`" #Operating System on that node
source /cvmfs/cms.cern.ch/cmsset_default.sh  ## if a bash script, use .sh instead of .csh
### for case 1. EOS have the following line, otherwise remove this line in case 2.
#Arguments 1)eosDir 2)jobName 3)rel 4)ProcId 5)nEvents 6)jobCfg 7)fileList 8) fileName
cp ${1}${2}.tgz .
tar -xf ${2}.tgz
rm ${2}.tgz
export $SCRAM_ARCH=slc7_amd64_gcc700
cd ${3}/src/
scramv1 b ProjectRename
eval `scramv1 runtime -sh` # cmsenv is an alias not on the workers
skipEvents=$((${4}*${5}))
line=$((${4}+1))
inFile=$(awk "NR == ${line}" ${7})
cmsRun L1MenuTools/L1Ntuples/${6} inputFiles=${inFile} #maxEvents=${5} #skipEvents=${skipEvents}
cp ${8} ${1}/${4}.root
rm ${8}
cd ${_CONDOR_SCRATCH_DIR}
rm -rf ${3}
