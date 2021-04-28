# L1+HLT acceptance

The scripts 'TriggerAcceptance.py' and 'seeds.py' can be used to perform acceptance studies in a two-stage process. The aim is to investigate the following quantities: total acceptance, pure acceptance, and proportional acceptance.

The total acceptance of an unprescaled L1T seed 's' is given as the fraction of the number of events passing any HLT path seeded by 's' and the total number of events passing 's'.

The pure acceptance of an unprescaled L1T seed 's' is given as the fraction of the number of events passing at least one HLT path seeded by ‘s’ and the total number of events passing only 's' and no other seed.

The proportional acceptance of an unprescaled L1T seed 's' is given as the fraction of the number of events passing an HLT path seeded by 's' where the events are weighted by the number of L1T seeds also firing the same HLT paths and the number of events passing 's' weighted by the number of L1T seeds also active in said event.


## Requirements

* active CMSSW environment: in CMSSW_XX_X/src call `cmsenv`
* valid grid proxy: `voms-proxy-init --voms cms`
* (dasgoclient)


## Input data

For the base usage one only requires a txt file 'example-inputs/fnames.txt' containing the location of the ROOT files to investigate, e.g.

'file=/store/data/Run2018D/ZeroBias/NANOAOD/02Apr2020-v2/230000/5613275E-184E-9A43-AA73-ACB61275D947.root'
                    
You can write the files to a txt file using the dasgoclient, e.g. to write the locations of all ROOT files stored at 'T2_IT_Legnaro' use

`dasgoclient -query="file dataset=/ZeroBias/Run2018D-02Apr2020-v2/NANOAOD site=T2_IT_Legnaro" > example-inputs/fnames.txt`

## Usage

* `python seeds.py`
* `python TriggerAcceptance.py`

## seeds.py

seeds.py creates HLT-L1 information via the hltGetConfiguration command found in a CMSSW environment. It loads ROOT files found in the txt file “example-inputs/fnames.txt” and extracts the run numbers which are used for the hltGetConfiguration command to create python HLT menus. The menus are then filtered for the HLT paths and L1T seeds. The information is then stored in “hlt_l1_seeds/hlt_l1.csv” with the following columns:

HLT path, Seeds (run #1), seeds (run #2), …

### Flags

`--FROM_MENU`: Use HLT python menus stored in MENUDIR instead of creating them from the ROOT files

`--MENUDIR`: Specify location of HLT python menus. Default: 'temp/'

`--ROOTFILES`: Specify location of the txt file. Default: example-inputs/fnames.txt’

`--TEMPDIR`: Specify the directory to which the HLT menus are stored. Default: ‘temp/’

`--KEEP_TEMPDIR`: Without this flag, the TEMPDIR is deleted after seeds.py has finished. Useful for testing as the HLT menu generation process can be skipped using the --FROM_MENU flag.

`--SAVEDIR`: Specify the directory to which the csv file is stored. Default: ‘hlt_l1_seeds’

`--REDIRECTOR`: Specify the redirector used to load data. Default: ‘root://xrootd-cms.infn.it/’

`--MAX_FILE`: Set the number of ROOT files to be processed. Default: 1

`--CSV_NAME`: Specify the name of the output ROOT file. Default: ‘hlt_l1.csv’

## TriggerAcceptance.py

TriggerAcceptance.py calculates the total, pure, and proportional rates and their accepted counterparts and stores histograms as a ROOT file and in pdf and png formats. It takes the hlt_l1_seeds.csv file and the txt file as input. It loads the events of the ROOT file and loops through each event to calculate the rates.

### Flags

`--ROOTFILES`: Specify location of the txt file. Default: example-inputs/fnames.txt’

`--HLT_L1_SEEDS`: Specify location of the csv file containing the HLT-L1T information. Default: ‘./hlt_l1_seeds/hlt_l1.csv’

`--MAX_FILE`: Set the number of ROOT files to be processed. Default: 1

`--REDIRECTOR`: Specify the redirector used to load data. Default: ‘root://xrootd-cms.infn.it/’

`--MAX_EVT`: Set the maximum number of events to process. Default: 500000

`--PRT_EVT`: Print information every nth event. Default 10000:
`--VERBOSE`: Print extra stuff. Useful for debugging
`--PU_MIN`: Minimum number of good reconstructed vertices. Default: 0

## Other sections

[consider adding further sections - some examples: Roadmap, release history,
guide for contributing, license,...]

