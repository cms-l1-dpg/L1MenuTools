# PS XML converter
Prescale table conversion tool from `csv` to `xml`.

## Basic usage
Convert the file `PStable.csv` to an `xml` file:
```
cmsenv
python3 prescale_xml_converter.py -c PStable.csv -o PStable.xml
```
### Arguments
- `-c` or `--csv_table`: The path to the `csv` version of the prescale table that should be converted.
- `-o` or `--output`: Path to the output file. If the file exists already, it will be overwritten. If directories in the path do not exit, they will be created. Directories that exist already will be used.

### Checks
The first column of the prescale table has to be labeled with `Index` and the second column with `Name`, otherwise it does not match the required format of a prescale table. In that case the input would be invalid and the conversion is aborted.