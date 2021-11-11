![](https://img.shields.io/badge/version-0.2.0-blue.svg)

# PS-tools

Prescale table tools for L1 Menu operations in CMS


## Getting Started

These instructions will get you a copy of the project up and running on your
local machine.


### Available tools & usage guides

#### [`ps-generate`](./docs/ps-generate.md): prescale table generation tool

Prescale table generation tool: create a new prescale table according to a L1 Menu, using information from an existing prescale table.
&rarr; See the [documentation on `ps-generate`](./docs/ps-generate.md) for further details.

Example command:
```
cmsenv
bash run-ps-generate.py PStable.xlsx L1Menu.xml
```

#### [`ps-diff`](./docs/ps-diff.md): prescale table comparison tool

Prescale table comparison tool: create diffs between two PS table in various output formats.
&rarr; See the [documentation on `ps-diff`](./docs/ps-diff.md) for further details.

Example command:
```
cmsenv
bash run-ps-diff.py PStable_left.xslx PStable_right.xslx
```

### Presentations of PS-tools

See [this table](./docs/talks.md) for a collection of talks/slides about this software.


## Versioning

For versioning, [SemVer](http://semver.org/) is used. See the [tags on this repository](https://github.com/tempse/PS-generate/tags)
for the available versions of this software. 


## Authors

* **Sebastian Templ** - *Initial work* - [tempse](https://github.com/tempse)
* Developed in close collaboration with the [L1 Menu Team](https://github.com/cms-l1-dpg) of the CMS collaboration
