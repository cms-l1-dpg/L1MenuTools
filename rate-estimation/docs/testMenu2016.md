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

### -h: help
Produces the help message, listing all arguments.

### -u: LS information table
See menu/GetLumi.py.
An example table (run\_lumi.csv) is provided.

### -m: PS table
An example table (menu/Prescale\_2018\_v2\_1\_0\_Col\_2.0.txt) is provided.

### -l: ntuple file list
An example list (fill\_7118\_nanoDST\_shifter\_lxplus.list) is provided.

### -b: Number of bunches.

### -n: Max events
Only run this number of events (-1 for all).

### --UseUnpackTree
Switch to using UnpackTree; default is EmuTree.

### --SelectRun
Select a run number; default is full list.

### --SelectLS '[startLS,endLS]'
Select lumi sections to run over; default is whole LS provided.

### --SelectFill
Select specific fill based on lumi list.

### --SelectEvent
Select specific event

### --SelectBX
Select specific BX ranges.

### --SelectCol
Select prescale column from input csv menu.

### -d: Output directory
Directory to put results in. Default is results/

### -o: Output name
Output files will be saved to [output directory]/[output name].\*

### -t: Text
Writes the rate to the output in text format.

### -c: CSV format
Writes the rate to the output in CSV format.

### -p: Plot
Writes plot to the output.

### --doPlotRate
Saves a rate plot to the output.

### --doPlotEff
Saves an efficiency plot to the output.

### --doPlotTest
Saves testing plot to the output.

### --doPlotuGt
Saves uGT histograms to the output.

### --doPlotLS
Saves count per LS plot to the output.

### --doTnPMuon
Uses tag & probe method for muon efficiency.

### --doPrintPU
Prints out rate per PU to file.

### --doPrintBX
Prints out bunch crossings.

### --doCompuGT
Compares emulation with uGT tree.

### --doScanLS
Quickly scan files for selected LS from --SelectLS option.

### --Select\_BX\_in\_48b
Selects target bunch crossings in 48b

### --Select\_BX\_in\_12b
Selects target bunch crossings in 12b

### --doBXReweight\_1\_to\_6\_47\_48
Selects bx 1-6 and 47,48 in 48b. These are often used to emulate or compare to 8b4e.

### --doBXReweight128
Selects bx 1,2, and 8 in 8b4e.

### --doBXReweight34567
Selects bx 3-7 in 8b4e

### --doBXReweight\_1\_to\_6\_11\_12
Selects bx 1-6, 11, and 12 in 12b

### --doBXReweight\_5\_to\_10
Selects bx 5-10 in 12b

### --SumJetET
PT threshold of Jet for HT

### --SumJetEta
Eta threshold of Jet for HT

### --SetMuonER
Set the ER in eta for muons

### --SetL1AcceptPS
Set the L1Accept Prescale at HLT for this run

### --SetNoPrescale
Set all prescales to 1

### --IgnorePrescale
Set all prescales of prescaled seeds to 0

### --UseUpgradeLyr1
Use Upgrade Layer1 Tree

### --UseL1CaloTower
Use Layer1 CaloTower Tree

### --UsePFMETNoMuon
Use PFMET no Muon in SingleMu sample

### --UseuGTDecision
Trigger seeds fired by uGT

