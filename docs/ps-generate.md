# PS-generate

Prescale table generation tool

## Basic usage

Use this tool to create a new prescale table according to a new L1 Menu, using information from an existing prescale table.

### Working with local files

Use the tool by running the Python script and passing both an existing prescale
table and a given L1 menu (see example below).

```
python psgenerate.py PStable.xlsx L1Menu.xml
```

### Download files on-the-fly

It is also possible to pass URLs instead of the local filepaths of a prescale
table or an XML menu, as the following example demonstrates.

```
python psgenerate.py https://github.com/cms-l1-dpg/L1Menu2018/raw/master/official/PrescaleTables/PrescaleTable-1_L1Menu_Collisions2018_v2_1_0.xlsx https://raw.githubusercontent.com/cms-l1-dpg/L1Menu2018/master/official/XMLs/L1Menu_CollisionsHeavyIons2018_v4_0_0.xml
```

The files are stored in temporary directories and are
removed by the operating system automatically. No need for manual cleanup.

> The given URLs must refer to files that are downloadable via `wget`.


### Optional arguments:
* `-output`: Specify the name of the output file, without file extension
  (*default: "new_PStable"*)

If run successfully, the tool creates a new xlsx file ("PStable_new.xlsx") which
contains the seeds of the passed L1 menu XML with their corresponding prescale
values (if they existed in the passed PS table).


