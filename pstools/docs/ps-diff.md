# PS-diff

Prescale table comparison tool

## Additional requirements

Optional requirement for colored output: [`colordiff`](https://www.colordiff.org/) (on Debian/Ubuntu-based systems, install via `sudo apt install colordiff`)


## Basic usage

Invoke a simple comparison of two existing prescale tables (in xlsx format):
```
python ps-diff.py PStable_left.xslx PStable_right.xslx
```

> The xslx files can be given as local paths or as URLs (if the URLs are downloadable via `wget`).


### Optional arguments:

* All valid `diff` options are allowed (execute `man diff` for a list of them) as long as they are passed without any dash prefixes (i.e., do not prepend "-" or "--").
* `--by-line`: perform a line-by-line comparison (default)
* `--by-name`: performs a name-by-name (i.e, seed-by-seed) comparison
* `--no-color`: do not colorize the terminal output

> For further information on usage, run `python ps-diff.py --help`

### Usage examples

Line-by-line comparison in two columns:
```
python ps-diff.py PStable_left.xslx PStable_right.xslx side-by-side
```

Name-by-name comparison in two columns:
```
python ps-diff.py PStable_left.xslx PStable_right.xslx side-by-side --by-name
```

Comparison in two columns, suppressing common lines:
```
python ps-diff.py PStable_left.xslx PStable_right.xslx side-by-side suppress-common-lines
```
