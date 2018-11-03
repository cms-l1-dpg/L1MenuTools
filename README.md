![](https://img.shields.io/badge/version-0.1.0--dev-blue.svg)

# PS-generate

Prescale table generation tool for L1 Menu operations in CMS


## Getting Started

These instructions will get you a copy of the project up and running on your
local machine.

> This software is being developed and tested on Ubuntu 18.04 LTS.


### Setting up the environment

In order to prepare a development environment, run
```
./setup.sh
```

This creates a virtual environment and installs all the necessary dependencies
from the `requirements.txt` file.

In order to activate the new environment, run
```
source env/bin/activate
```


### Using the tool

Use the tool by running the Python script and passing both an existing prescale
table and a given L1 menu (see example below).

```
python psgenerate.py PStable.xlsx L1Menu.xml
```

#### Optional arguments:
* `-output`: Specify the name of the output file, without file extension
  (*default: "new_PStable"*)

If run successfully, the tool creates a new xlsx file ("PStable_new.xlsx") which
contains the seeds of the passed L1 menu XML with their corresponding prescale
values (if they existed in the passed PS table).


## Deployment

The preferred way of creating an executable binary for deployment is by using
[pyinstaller](https://www.pyinstaller.org/) (version 3.4+).

In order to create a binary with *pyinstaller*, run
```
pyinstaller --onefile psgenerate.py
```

## Versioning

For versioning, [SemVer](http://semver.org/) is used. See the [tags on this repository](https://github.com/tempse/PS-generate/tags)
for the available versions of this software. 


## Authors

* **Sebastian Templ** - *Initial work* - [tempse](https://github.com/tempse)

