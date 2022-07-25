# L1 Menu tools

Codebase for L1 Menu tools

## Available tools

### Rate estimation tools ([rate-estimation/](rate-estimation/))
Collection of tools used for rate estimation of a new menu.

### Rate visualization tool ([rate-visualization/](rate-visualization/))
Create pie charts and bar charts from L1 Menu rate tables.

### L1-HLT acceptance studies ([L1-HLT-acceptance/](L1-HLT-acceptance/))
Study and visualize the relations between L1 and HLT menus.

### L1Ntuples production recipe ([L1Ntuples/](L1Ntuples/))
Latest snapshot of a recipe to produce L1Ntuples on HTCondor.

### PS-tools ([pstools/](pstools/))
Collection of prescale table tools for offline L1 Menu operations in CMS. 

Contains the following tools:
* `ps-generate` (prescale table generation tool): create a new prescale table according to a L1 Menu, using information from an existing prescale table ([documentation](pstools/docs/ps-generate.md))
* `ps-diff` (prescale table comparison tool): create diffs between two PS table in various output formats ([documentation](pstools/docs/ps-diff.md))

### Script to extract emulator trigger counts from L1Ntuple ([uGT_decision_counts/](uGT_decision_counts/))

The MenuTools are intended to quickly produce rate estimates for L1 algorithms, allowing the flexibility to add new algorithms or modify existing ones. Thus the rate-estimation scripts use trigger primitives and objects to reproduce the trigger decision without having to re-produce L1Ntuples after re-run the emulator with the new menu over the raw data events. It is therefore important that the triggered event counts computed by the MenuTools matches the trigger counts given by the L1 emulator.

The trigger decision of interest for each L1 algorithm is stored in the L1Ntuples as m_algoDecisionFinal. The relevant CMSSW code may be found here : https://cmssdt.cern.ch/lxr/source/DataFormats/L1TGlobal/src/GlobalAlgBlk.cc

uGT_decision_counts contains a script that obtains the emulated trigger counts from L1Ntuple, and prints as output the L1 bit index, L1 seed name, and the trigger count. This is meant to be a validation test, and it is sufficient to produce an L1Ntuple based on the desired L1 menu.

Provided tools are :
* 'parse_algo_map.py' : Converts the given prescale table that corresponds to the emulated L1 menu into a list that maps the L1 bit index to the L1 seed name.
* 'algo_map.py' : Contains the list mapping the L1 bit index to the L1 seed name
* 'getEmulatorDecision.py' : script to obtain the emulated trigger counts (stored in m_algoDecisionFinal in the L1Ntuples) for each L1 bit/seed.
