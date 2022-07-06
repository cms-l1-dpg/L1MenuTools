## uGT decision

Returns the emulated trigger counts in an ntuple for a given (L1_bit, L1_seed)  map.

The MenuTools are intended to quickly produce rate estimates for L1 algorithms, allowing the flexibility to add new algorithms or modify existing ones. Thus the rate-estimation scripts use trigger primitives and objects to reproduce the trigger decision without having to re-produce L1Ntuples after re-run the emulator with the new menu over the raw data events. It is therefore important that the triggered event counts computed by the MenuTools matches the trigger counts given by the L1 emulator. 

The trigger decision for each L1 algorithm is stored in the L1Ntuples as m_algoDecisionFinal. The relevant script in CMSSW may be found here : https://cmssdt.cern.ch/lxr/source/DataFormats/L1TGlobal/src/GlobalAlgBlk.cc

### parse_algo_map.py

Script to produce the algo map from a provided L1 Prescale Table. Please ensure that the Prescale Table version corresponds to the L1 menu used to produce the ntuples. This is because the L1 ntuple does not store the L1 seed names. This mapping of the L1 bit indices to the L1 seed names via this script/map is for convenience and readability.

## Usage

python3 parse_algo_map.py L1Menu_Collisions2022_v1_1_0.csv

## Obtaining L1 Prescale Table

The following command may be used to obtain an L1 prescale table :

wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuRun3/master/development/L1Menu_Collisions2022_v1_1_0/PrescaleTable/L1Menu_Collisions2022_v1_1_0.csv

### algo_map.py

Contains a list of the form [L1_bit, "L1_seed_name"], used to return the emulated counts as stored in the ntuple.

### getEmulatorDecision.py

Script that takes ntuple as input, using the provided algo map to obtain the emulated trigger counts.

The output lists the L1 bit index, L1 seed name, and the triggered counts.

## Usage

python3 getEmulatorDecision.py \<L1Ntuple\>

\<L1Ntuple\> : location of the L1Ntuple.root file

