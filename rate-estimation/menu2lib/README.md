# Creation of menulib files for the L1 Menu (via UTM)

## Dependencies
The major dependency of most of the scripts is the [UTM library](https://gitlab.cern.ch/cms-l1t-utm/utm). It is also necessary to install *jinja2* and *requests*.

## Setup instructions
These instructions assume a standard Lxplus environment (log in with `ssh <user>@lxplus.cern.ch`) and set up a virtual environment with all the dependencies installed.

```
python3.6 -m venv utm-env
source utm-env/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
pip install git+https://github.com/cms-l1-globaltrigger/tm-python.git@0.7.3
```
If this virtual environment has been already set up in the past, it is sufficient to simply activate it with `source utm-env/bin/activate`.

## Running menu2lib.py
- use menu2lib.py for generating c++ file to be used with L1Ntuple (needs python binding of the UTM library)
- usage: `python menu2lib.py --menu <path to menu xml> [--output <output c++ file name>]`
