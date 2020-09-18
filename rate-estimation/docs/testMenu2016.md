# testMenu2016

Rate estimation tool

## Setup
Make sure you are in a CMSSW environment and have menulib.cc and menulib.hh in your directory.
To get menulib.cc and menulib.hh, see menu2lib.py.

Then ensure you have all requirements available:
* C++ conversion of a menu
* Ntuple file list
* Lumi section information table
* Prescale table

Make the directories objs/ and objs/include/ if they do not exist.
Finally, run `make -j 4`.

## Test run
Components (apart from menulib.hh and menulib.cc) are provided to run a short test.
`./testMenu2016 -u menu/run_lumi.csv -m menu/Prescale_2018_v2_1_0_Col_2.0.txt -l ntuple/fill_7118_nanoDST_shifter_lxplus_test.list -o test -b 2544 --doPlotRate --doPlotEff --UseUnpackTree`
This will take only a few minutes and output test.csv, test.root, and test.txt into the results/ directory.
Make sure you have set up the virtual environment as specified in the rate-estimation README, are in a CMSSW environment (ie- you have run `cmsenv`), and have run `make -j 4` since acquiring menulib.hh and menulib.cc.

## Arguments

### -u: LS information table
See menu/GetLumi.py.
An example table (run_lumi.csv) is provided.

### -m: PS table
An example table (menu/Prescale_2018_v2_1_0_Col_2.0.txt) is provided.

### -l: ntuple list
An example list (fill_7118_nanoDST_shifter_lxplus.list) is provided.

### -o: Output name
Output files will be saved to results/[output name].*

### -b: Number of bunches.

### --UseUnpackTree
Switch to using UnpackTree; default is EmuTree.

### --SelectRun
Select a run number; default is full list.

### --SelectLS '[startLS,endLS]'
Select lumi sections to run over; default is whole LS provided.


