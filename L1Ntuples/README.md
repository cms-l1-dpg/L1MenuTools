# L1Ntuples production recipe

The following L1Ntuples recipe presents the latest snapshot used to create L1Ntuples
for L1 menu studies.

The targeted environment is Lxplus and assumes a standard session:
```
ssh -XY <username>@lxplus.cern.ch
```

Follow the below instructions in the given order to produce your own set of L1Ntuples
on HTCondor.

## 1. Environment setup
Setup the environment according to the [official instructions](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati).

**NOTE**: the LUT file newSFHBHEOnp5_METPUM_211124.txt is not available in central cmssw yet. At the moment, it has to be included manually. See the last comment in [PR#63](https://github.com/cms-l1-dpg/L1MenuTools/pull/63) for the temporary recipe.
```
cmsrel CMSSW_12_0_2
cd CMSSW_12_0_2/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_12_0_2
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v110.0
git cms-addpkg L1Trigger/L1TCalorimeter
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

git cms-checkdeps -A -a

scram b -j 8
```

## 2. Download the L1MenuTools/L1Ntuples code
Clone this repository into your `$CMSSW_BASE/src/` folder. If you followed the
instructions above, all you need to do is
```
git clone https://github.com/cms-l1-dpg/L1MenuTools.git
```

## 3. Customization of the submission file
Go to the L1Ntuples subfolder
```
cd L1MenuTools/L1Ntuples
```
and customize the HTCondor submission script `condor_sub.py` according to your needs:
- `eosDir`: output folder for batch jobs (user must have write permissions)
- `nEvents`: number of events to process
- `nJobs`: number of jobs to run


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
- Input datasets: `/SingleNeutrino_Pt-2To20-gun/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_SNB_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW`,
`/SingleNeutrino_E10-gun/Run3Winter21DRMiniAOD-FlatPU30to80FEVT_SNB_112X_mcRun3_2021_realistic_v16-v2/GEN-SIM-DIGI-RAW`
- It includes the PFA1' Filter (https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalPileupMitigation#PFA1_Filter)
