# UTM scripts for the L1 Menu

## Dependencies

The major dependency of most of the scripts is the [UTM library](https://gitlab.cern.ch/cms-l1t-utm/utm). It might also be required to install *jinja2* and *requests*.

### Setup instructions
These instructions assume a standard Lxplus environment (log in with `ssh <user>@lxplus.cern.ch`) and set up a virtual environment with all the dependencies installed.

```
python3.6 -m venv utm-env
source utm-env/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
pip install git+https://github.com/cms-l1-globaltrigger/tm-python.git@0.7.3
```
If this virtual environment has been already set up in the past, it is sufficient to simply activate it with `source utm-env/bin/activate`.


## genCsv.py
- use genCsv.py for generating csv file to be used with the uGT emulator (needs python binding of the UTM library)

## menu2lib.py
- use menu2lib.py for generating c++ file to be used with L1Ntuple (needs python binding of the UTM library)
- usage: `python menu2lib.py --menu <path to menu xml> [--output <output c++ file name>]`

## json2xml.py
- use json2xml.py for producing XML snippet to be included in the Level-1 trigger menu XML file.
- usage: `python json2xml.py --input <path to serialised algorithms> [--output <output file for XML snippet]`
- follow the instruction in the test/00ReadMe.txt file to serialise algorithms then to produce xml snippet for the algorithms

## makeEmptyMenu.py
- use makeEmptyMenu.py for generating menu file with empty algorithm (needs python binding of the UTM library)
- usage: `python makeEmptyMenu.py --menu <path to L1 menu xml file>`
