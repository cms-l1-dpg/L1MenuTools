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

## Script to extract emulator trigger counts from L1Ntuple

uGT_decision_counts contains a script that obtains the emulated trigger counts from L1Ntuple, and prints as output the L1 bit index, L1 seed name, and the trigger count.
