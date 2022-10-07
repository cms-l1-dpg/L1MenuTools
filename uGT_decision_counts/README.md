## uGT decision

Returns the emulated trigger counts in an ntuple for a given (L1_bit, L1_seed)  map.

The MenuTools are intended to quickly produce rate estimates for L1 algorithms, allowing the flexibility to add new algorithms or modify existing ones. Thus the rate-estimation scripts use trigger primitives and objects to reproduce the trigger decision without having to re-produce L1Ntuples after re-running the emulator with the new menu over the raw data events. It is therefore important that the triggered event counts computed by the MenuTools matches the trigger counts given by the L1 emulator. 

The trigger decision of interest for each L1 algorithm is stored in the L1Ntuples as m_algoDecisionInitial. The relevant CMSSW code may be found here : https://cmssdt.cern.ch/lxr/source/DataFormats/L1TGlobal/src/GlobalAlgBlk.cc

### parse_algo_map.py

Script to produce the algo map from a provided L1 Prescale Table. Please ensure that the Prescale Table version corresponds to the L1 menu used to produce the ntuples. This is because the L1 ntuple does not store the L1 seed names. This mapping of the L1 bit indices to the L1 seed names via this script/map is for convenience and readability. Default map provided is for L1Menu_Collisions2022_v1_3_0.csv

## Usage

```
python3 parse_algo_map.py L1Menu_Collisions2022_v1_3_0.csv
```

## Obtaining L1 Prescale Table

The following command may be used to obtain an L1 prescale table :

```
wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuRun3/master/development/L1Menu_Collisions2022_v1_3_0/PrescaleTable/L1Menu_Collisions2022_v1_3_0.csv
```

### algo_map.py

Contains a list of the form [L1_bit, "L1_seed_name"], used to return the emulated counts as stored in the ntuple. Default map provided is for L1Menu_Collisions2022_v1_3_0.csv

### getuGTdecision.py

Script that takes ntuples as input, using the provided algo map to obtain the emulated trigger counts.

The output lists the L1 bit index, L1 seed name, and the triggered counts.

## Usage

```
python3 getuGTdecision.py --ntuples <ntuples.list> --selections <additional selections>
```
>NOTE : This runs over the list of ntuples given in, for example, ntuples.list. Therefore, the path to the ntuples must be listed in this file. A default list has been provided for reference, but in principle any list may be used. The recommended way is to do :
```
ls <path_to_ntuples_directory/*.root> > ntuple.list
```
### ntuples.list

Contains the locations of the ntuples to run over. The default ntuples provided are located here : 
```
/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/elfontan/condor/newCalibrations_startOfRun3_run3MC_NuGun_E10/
```
