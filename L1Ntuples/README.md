# L1Ntuples production recipe

The following L1Ntuples recipe presents the latest snapshot used to create L1Ntuples based on the latest conditions deployed during the 2024 data-taking.

The targeted environment is Lxplus and assumes a standard session:
```
ssh -XY <username>@lxplus.cern.ch
```

Follow the below instructions in the given order to produce your own set of L1Ntuples on HTCondor.

## 1. Environment setup
Setup the environment according to the [official instructions](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati), or the following instructions if the twiki is not updated.

```
cmsrel CMSSW_14_0_13
cd CMSSW_14_0_13/src
cmsenv
git cms-init
git cms-addpkg L1Trigger/L1TCalorimeter
git cms-addpkg L1Trigger/L1TNtuples
git cms-addpkg L1Trigger/Configuration
git cms-addpkg L1Trigger/L1TGlobal
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TZDC
mkdir L1Trigger/L1TZDC/data
cd L1Trigger/L1TZDC/data
wget https://raw.githubusercontent.com/cms-data/L1Trigger-L1TCalorimeter/master/zdcLUT_HI_v0_1.txt
cd -
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuRun3/master/development/L1Menu_Collisions2024_v1_3_0/L1Menu_Collisions2024_v1_3_0.xml
cd ../../../../../
sed -i 's/L1Menu_Collisions2022_v1_2_0.xml/L1Menu_Collisions2024_v1_3_0.xml/g' L1Trigger/Configuration/python/customiseUtils.py

git cms-checkdeps -A -a

scram b -j 8
```

## 2. Download the L1MenuTools/L1Ntuples code
Clone this repository into your `$CMSSW_BASE/src/` folder. 
If you followed the instructions above, all you need to do is
```
git clone https://github.com/cms-l1-dpg/L1MenuTools.git
```

## 3. Customization of the submission file
Go to the L1Ntuples subfolder
```
cd L1MenuTools/L1Ntuples
```
and customize the HTCondor submission script `condor_sub.py` according to your needs:
- `nEvents`: number of events to process
- `nJobs`: number of jobs for the splitting (corresponding to the number of files by default)
- `fileList`: list of GEN-SIM-RAW files
- `jobScript`: bash script containing the cmsRun command. NOTE: the default is `cmsRun.sh`.
- `eosDir`: output folder for batch jobs (user must have write permissions)
- `jobName`: name of the directory created in eosDir and locally


## 4. Submit the production
Some remaining steps before submitting:
```
scram b -j 8
voms-proxy-init --voms cms --valid 168:00 -out $HOME/private/.proxy
export X509_USER_PROXY=$HOME/private/.proxy
```

Finally, launch the production with
```
python3 condor_sub.py
```

## 5. Check the output directory
When all jobs finished to run, check the output directory, identify files corresponding to failed jobs looking at the size of the file, and delete them:
```
find . -name "*.root" -size -10k
find . -name "*.root" -size -10k -delete
```

  
