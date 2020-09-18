# Rate estimation

Rate estimation tools for L1 Menu operations in CMS

## Getting Started

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

#### menu2lib.py
Converts the XML menu to C++ to be used in testMenu2016 for rate estimation.
Do not run cmsenv before running this.

#### ntuple/makeFileList.py
Creates a file list for a given ntuple.

#### menu/GetLumi\_setup.sh and menu/GetLumi.py
Creates a lumi section information table.


