# L1 Menu tools

Codebase for L1 Menu tools. The targeted environment is Lxplus and assumes a standard session:
```
ssh -XY <username>@lxplus.cern.ch
```
Check all the details about the setup and running of the tools in the [Menu twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToL1TriggerMenu#4_Run_3_settings). Find below some general instructions for the first setup.
```
cmsrel CMSSW_14_0_13
cd CMSSW_14_0_13/src
git clone --depth 1 https://github.com/cms-l1-dpg/L1MenuTools.git
cd L1MenuTools/rate-estimation
wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuRun3/master/development/L1Menu_Collisions2024_v1_3_0/L1Menu_Collisions2024_v1_3_0.xml # alternatively: place your custom menu XML here
bash configure.sh  L1Menu_Collisions2024_v1_3_0.xml # alternatively: provide your custom menu XML
cmsenv
mkdir -p objs/include
make -j 8
```

## Available tools
Find listed below all the available tools for menu studies in this repository. 

* **Rate estimation tools** ([rate-estimation](rate-estimation/)): Collection of tools used for rate estimation of a new menu.
 
* **Rate visualization tool** ([rate-visualization](rate-visualization/)): Create pie charts and bar charts from L1 Menu rate tables.
 
* **L1-HLT acceptance studies** ([L1-HLT-acceptance](L1-HLT-acceptance/)): Study and visualize the relations between L1 and HLT menus.

* **L1Ntuples production recipe** ([L1Ntuples](L1Ntuples/)): Latest snapshot of a recipe to produce L1Ntuples on HTCondor.

* **PS-tools** ([pstools](pstools/)): Collection of prescale table tools for offline L1 Menu operations in CMS.  It contains the following tools:
  * `ps-generate` (prescale table generation tool): create a new prescale table according to a L1 Menu, using information from an existing prescale table ([documentation](pstools/docs/ps-generate.md)).
  * `ps-diff` (prescale table comparison tool): create diffs between two PS table in various output formats ([documentation](pstools/docs/ps-diff.md)).

* **Emulated trigger counts** ([uGT_decision_counts](uGT_decision_counts/)): Tool to extract the emulated trigger counts from the L1Ntuples. Useful for validating that the rate-estimation tools and the emulator work in the same way. Provided tools are :
  * `parse_algo_map.py` : Converts the given prescale table that corresponds to the emulated L1 menu into a list that maps the L1 bit index to the L1 seed name.
  * `algo_map.py` : Contains the list mapping the L1 bit index to the L1 seed name.
  * `getuGTdecision.py` : script to obtain the emulated trigger counts (stored in *m_algoDecisionInitial* in the L1Ntuples) for each L1 bit/seed.
