# Rate estimation

Rate estimation tools for L1 Menu operations in CMS

> See the [official twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/HowToL1TriggerMenu) for updated instructions!

## Overview

### Available tools

#### testMenu2016
Primary tool for performing rate estimation. The rest of the tools in this
directory are used for setting up the components required.

Requires:
* C++ conversion of a menu
* Ntuple file list
* Lumi section information table
* Prescale table
See [docs/testMenu2016.md]() for more information.

#### menu2lib/menu2lib.py
Converts the XML menu to C++ to be used in testMenu2016 for rate estimation.
Do not run cmsenv before running this.

#### ntuple/makeFileList.py
Creates a file list for a given ntuple.

#### menu/GetLumi_setup.sh and menu/GetLumi.py
Creates a lumi section information table.

## Getting Started
After setting up the environment as discussed above, prepare the set of
components required to run testMenu2016 for rate estimation.

### C++ conversion of a L1 Menu
Configure rate-estimation using a L1 Menu XML file using `configure.sh`.
```bash
./configure.sh <your_menu.xml>
```
This will produce the files menulib.hh and menulib.cc.

The latest L1 Menu XML can be found [here](https://github.com/cms-l1-dpg/L1Menu2018/tree/master/official/XMLs).

Once these files have been produced, it is safe to run `cmsenv`.

### Ntuple file list
A set of ntuples is required to run the rate estimation code.
To test the code, a list containing Run 3 MC ntuples is provided in
`ntuple/Run3_NuGun_MC_ntuples.list`.
You can run the code running on a subset of events using the option `--maxEvent`.

The script `ntuple/makeFileList.py` can be used to create a file list for other
ntuples:
```bash
cd ntuple
./makeFileList.py <your_ntuple_location>
cd ..
```

### Lumi section information table
In case you are using data ntuples, this table is used to avoid bad lumi sections (LS). 
It also includes LS vs PU information, which can be useful for plotting rate vs PU dependencies. 
An example csv is included in `menu/run_lumi_RunA.csv`, including fill at high pileup.
To produce your own, modify the beginning of `GetLumi.py` to list the run number
of your ntuple, then:
```bash
cd menu
source GetLumi_setup.sh
./GetLumi.py
cd ..
```

### Prescale table
While the official PS tables are in .xlsx format for better presentation, ensure
that the prescale table you use has been converted to
either a tab-separated .txt file or comma-separated .csv file.
An example table has been provided for column 2.0e34 in
`menu/Prescale_2022_v0_1_1.csv`.
The latest 2018 official prescale tables can be found 
[here](https://github.com/cms-l1-dpg/L1Menu2018/tree/master/official/PrescaleTables).

### Rate estimation
Ensure you are in a CMSSW_120X environment.
If the directories objs/ and objs/include/ do not exist, create them.
Then run `make -j 4`.

#### Test run
Components have been provided to run a short test.
```bash
./testMenu2016 \
-m menu/Prescale_2022_v0_1_1.csv -l ntuple/Run3_NuGun_MC_ntuples.list \
-o test -b 2748 --doPlotRate --doPlotEff --SelectCol 2E+34 \    
--doPrintPU --allPileUp --doReweightingRun3 --maxEvent 200000
```
This will take only a few minutes and output test.csv, test.root, test.txt, and test_PU.txt into the results/ directory.

#### Temporary remark:
Please note that currently in the context of the rate estimation studies for the Trigger Review and preparation of the Run 3 menu, a selection on the pileup window is applied by default. 
A pileup range from 48 to 58, corresponding to the expected average PU during the lumi levelling period in Run 3, is taken into account. 
If you want to run the rates for the full PU distribution available in the sample, please use the option `--allPileUp`.
Moreover, the possibility to apply a reweighting procedure has been included in order to take into account a realistic pileup model according to Run 3 Lumi POG predictions.
The recommended approach at the moment is to use the options `--allPileUp --doReweightingRun3` to get results for the incoming Trigger Review. 

#### Arguments
The arguments that you can use are as follows:

|Option | Description |
|-------|-------------|
|`-u`   | LS information table. |
|`-m`   | PS table. |
|`-l`   | ntuple list. |
|`-o`   | Output name. Output files will be saved to `results/[output name].\*` |
|`-b`   | Number of bunches. |
|`--allPileUp` | Consider the full pileup profile instead if a default pileup window around 53 |
|`--doReweightingRun3` | Apply pileup reweighting of the Run 3 flat pileup distribution according to Run 3Lumi POG model (use with `--allPileUp` option!) |
|`--doReweighting2018` | Apply pileup reweighting of the Run 3 flat pileup distribution to match the Run 2 pileup profile (use with `--allPileUp` option!) |
|`--SelectRun` | Select a run number; default is full list. |
|`--SelectLS [startLS,endLS]` | Select lumi sections to run over; default is whole LS provided. |
|`--SelectCol` | Select the column in the PS table. |
|`--UseUnpackTree` | Switch to using UnpackTree; default is EmuTree. |
|`--maxEvent` | Run on a subset of the available events. |
|`--doPrintPU` | Evaluate and save the rates for different pileup values |

Running `./testMenu2016 --help` will show all arguments with a brief description. Also see [docs/testMenu2016.md]().

#### Important note:
Please remember that when you use the --SelectCol option, you just select a specific PS column from the table which contains the information about which seeds are prescaled or unprescaled and about the prescale values. It does not allow to select a certain luminosity. In order to look at rates using a specific luminosity scenario, you would need to use lumi-range-skimmed datasets or to select a specific pileup window when running the rates. 
(See [here](https://github.com/cms-l1-dpg/L1MenuTools/blob/master/rate-estimation/include/L1Menu2016.C#L1239) for the part of the code that has to be modified in case you need to select the pileup range of interest. Then remember to compile the rate-estimation directory!).
