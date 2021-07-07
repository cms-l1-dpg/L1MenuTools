# L1 Menu tools

Codebase for L1 Menu tools

## Available tools

### Rate estimation tools ([rate-estimation/](rate-estimation/))
Collection of tools used for rate estimation of a new menu.

### Rate visualization tool ([rate-visualization/](rate-visualization/))
Create pie charts and bar charts from L1 Menu rate tables.

### L1Ntuples production recipe ([L1Ntuples/](L1Ntuples/))
Latest snapshop of a recipe to produce L1Ntuples on HTCondor.

### PS-tools ([pstools/](pstools/))
Collection of prescale table tools for offline L1 Menu operations in CMS. 

Contains the following tools:
* `ps-generate` (prescale table generation tool): create a new prescale table according to a L1 Menu, using information from an existing prescale table ([documentation](pstools/docs/ps-generate.md))
* `ps-diff` (prescale table comparison tool): create diffs between two PS table in various output formats ([documentation](pstools/docs/ps-diff.md))