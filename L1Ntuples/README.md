# L1Ntuples production recipe

The following L1Ntuples recipe presents the latest snapshot used to create L1Ntuples for L1 menu studies.

The targeted environment is Lxplus and assumes a standard session:
```
ssh -XY <username>@lxplus.cern.ch
```

Follow the below instructions in the given order to produce your own set of L1Ntuples
on HTCondor.

## 1. Environment setup
Setup the environment according to the [official instructions](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati).

```
cmsrel CMSSW_12_3_0_pre6
cd CMSSW_12_3_0_pre6/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_12_3_0_pre6
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v129.0
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

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
- `jobScript`: bash script containing the cmsRun command. NOTE: the default is `cmsRun.sh`, but if you need to run on the latest 122X NuGun sample, the number of files is very large so the script needs some changes (included in `NuGun_122X_cmsRun.sh`)for a successful condor submission
- `eosDir`: output folder for batch jobs (user must have write permissions)
- 'jobName': name of the directory created in eosDir and locally


## 4. Submit the production
Some remaining steps before submitting:
```
scram b -j 8
voms-proxy-init --voms cms --valid 168:00 -out $HOME/private/.proxy
export X509_USER_PROXY=$HOME/private/.proxy
```

Finally, launch the production with
```
python condor_sub.py
```

## Additional notes about the current recipe
- It includes the PFA1' Filter (https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalPileupMitigation#PFA1_Filter)
- Input datasets (120X): `/SingleNeutrino_Pt-2To20-gun/Run3Summer21DRPremix-SNB_120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW`,
`/SingleNeutrino_E-10-gun/Run3Summer21DRPremix-SNB_120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW`
- Input datasets (122X - Realistic PU distribution with an average PU of 55): `/SingleNeutrino_E-10-gun/Run3Winter22DR-L1TPU0to99FEVT_SNB_122X_mcRun3_2021_realistic_v9-v2/GEN-SIM-DIGI-RAW`, `/SingleNeutrino_Pt-2To20-gun/Run3Winter22DR-L1TPU0to99FEVT_SNB_122X_mcRun3_2021_realistic_v9-v2/GEN-SIM-DIGI-RAW` 
