![](https://img.shields.io/badge/version-0.2.0-blue.svg)

# PS-tools

Prescale table tools for L1 Menu operations in CMS


## Getting Started

These instructions will get you a copy of the project up and running on your
local machine.


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


### Available tools & usage guides

#### [`ps-generate`](./docs/ps-generate.md): prescale table generation tool

Prescale table generation tool: create a new prescale table according to a L1 Menu, using information from an existing prescale table.

&rarr; see the [documentation on `ps-generate`](./docs/ps-generate.md) for further details.


#### [`ps-diff`](./docs/ps-diff.md): prescale table comparison tool

Prescale table comparison tool: create diffs between two PS table in various output formats.

&rarr; see the [documentation on `ps-diff`](./docs/ps-diff.md) for further details.


### Presentations of PS-tools

See [this table](./docs/talks.md) for a collection of talks/slides about this software.


## Versioning

For versioning, [SemVer](http://semver.org/) is used. See the [tags on this repository](https://github.com/tempse/PS-generate/tags)
for the available versions of this software. 


## Authors

* **Sebastian Templ** - *Initial work* - [tempse](https://github.com/tempse)
* Developed in close collaboration with the [L1 Menu Team](https://github.com/cms-l1-dpg) of the CMS collaboration
