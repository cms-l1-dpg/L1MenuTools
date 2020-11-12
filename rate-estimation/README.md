# Rate estimation

Rate estimation tools for L1 Menu operations in CMS

> See the [official twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/HowToL1TriggerMenu) for updated instructions!

## Overview

### Setting up the environment

In order to prepare a development environment, run
```
virtualenv -p python3 env
source env/bin/activate
pip install --upgrade pip
pip install jinja2
pip install git+https://github.com/cms-l1-globaltrigger/tm-python.git@0.7.3
```
This creates a virtual environment and installs all the necessary dependencies.

Please do not run cmsenv between activating the environment and running menu2lib.py.
Doing so will lead to an error stating that there is no tmGrammar module found.
Other tools do expect to be run in a CMSSW environment, and you must run cmsenv before setting up testMenu2016.

### Available tools

#### testMenu2016
Primary tool for performing rate estimation. The rest of the tools in this directory are used for setting up the components required.
Requires:
* C++ conversion of a menu
* Ntuple file list
* Lumi section information table
* Prescale table
See docs/testMenu2016.md for more information.

#### menu2lib/menu2lib.py
Converts the XML menu to C++ to be used in testMenu2016 for rate estimation.
Do not run cmsenv before running this.

#### ntuple/makeFileList.py
Creates a file list for a given ntuple.

#### menu/GetLumi\_setup.sh and menu/GetLumi.py
Creates a lumi section information table.

## Getting Started
After setting up the environment as discussed above, prepare the set of components required to run testMenu2016 for rate estimation.

### C++ conversion of a L1 Menu
Place a copy of your L1 Menu XML in the rate-estimation directory. Make sure you are in your virtual environment and have not run `cmsenv`.
Then run the command:
```
python menu2lib.py --menu <your_menu.xml>
```
This will produce the files menulib.hh and menulib.cc.

The latest L1 Menu XML can be found at https://github.com/cms-l1-dpg/L1Menu2018/tree/master/official/XMLs .

Once these files have been produced, it is safe to run cmsenv.

### Ntuple file list
A ZeroBias ntuple is required to run rate estimation.
An example for fill 7118 has been provided in ntuple/fill\_7118\_nanoDST\_shifter\_lxplus.list,
with a subset of that list provided in ntuple/fill\_7118\_nanoDST\_shifter\_lxplus\_test.list to be used for short tests of the environment.

The script ntuple/makeFileList.py can be used to create a file list for other ntuples:
```
cd ntuple
./makeFileList.py <your_ntuple_location>
cd ..
```

### Lumi section information table
This table is used to avoid bad LS. It also includes LS vs PU information, which can be useful for plotting rate vs PU dependencies.
An example csv is included in menu/run\_lumi.csv , including fills 7118, 7131, and 7358.
To produce your own, modify the beginning of GetLumi.py to list the run number of your ntuple, then:
```
cd menu
source GetLumi\_setup.sh
./GetLumi.py
cd ..
```

### Prescale table
While the official PS tables are in .xlsx format for better presentation, ensure that the prescale table you use has been converted to
either a tab-separated .txt file or comma-separated .csv file.
An example table has been provided for column 2.0e34 in menu/Prescale\_2018\_v2\_1\_0\_Col\_2.0.txt .
The 2018 official prescale tables can be found at https://github.com/cms-l1-dpg/L1Menu2018/tree/master/official/PrescaleTables .

### Rate estimation
Ensure you are in a CMSSW 10 or 11 environment (ie- that you have run cmsenv).
If the directories objs/ and objs/include/ do not exist, create them.
Then run `make -j 4`.

#### Test run
Components have been provided to run a short test.
```
./testMenu2016 -u menu/run_lumi.csv -m menu/Prescale_2018_v2_1_0_Col_2.0.txt -l ntuple/fill_7118_nanoDST_shifter_lxplus_test.list -o test -b 2544 --doPlotRate --doPlotEff --UseUnpackTree
```
This will take only a few minutes and output test.csv, test.root, and test.txt into the results/ directory.

#### Arguments
The arguments used in the test are as follows:
* -u: LS information table
* -m: PS table
* -l: ntuple list
* -o: Output name. Output files will be saved to results/[output name].\*
* -b: Number of bunches.
* --UseUnpackTree. Switch to using UnpackTree; default is EmuTree.
* --SelectRun. Select a run number; default is full list.
* --SelectLS '[startLS,endLS]'. Select lumi sections to run over; default is whole LS provided.

Running `./testMenu2016 --help` will show all arguments with a brief description. Also see docs/testMenu2016.md .
