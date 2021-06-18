# L1T+HLT Acceptance Studies

## 1. Aim
Evaluation of the total, pure and proportional acceptance of L1T seeds wrt HLT paths on NanoAOD samples, via the calculation of the corresponding rates. See [1] for more information on these quantities and on the workflow implemented.

## 2. Setup
* `ssh -Y <USERNAME>@lxplus.cern.ch`
* `voms-proxy-init -voms cms`
* `cmsrel CMSSW_<X_X_X>`
* `cd CMSSW_<X_X_X>/src`
* `cmsenv`
* `git cms-init`
* `git clone https://github.com/cms-l1-dpg/L1MenuTools.git`
* `scram b -j 8`
* `cd L1-HLT-acceptance`

## 3. Execution

### 3a. Input data

For the base usage, only a .txt file 'example-inputs/fnames.txt' is required, which shall contain the location of the .root files on which the acceptance rates are to be calculated, e.g.
'file=/store/data/Run2018D/ZeroBias/NANOAOD/02Apr2020-v2/230000/5613275E-184E-9A43-AA73-ACB61275D947.root'
This can be done by using the dasgoclient command.
e.g. to write the locations of all .root files stored at 'T2_IT_Legnaro' use:
`dasgoclient -query="file dataset=/ZeroBias/Run2018D-02Apr2020-v2/NANOAOD site=T2_IT_Legnaro" > example-inputs/fnames.txt`

### 3b. Code

The scripts 'TriggerAcceptance.py' and 'seeds.py' can be used to perform acceptance studies in a two-stage process.
Execution commands:

* `python seeds.py`
* `python TriggerAcceptance.py`

#### 3bi. seeds.py

Creates HLT-L1 information via the hltGetConfiguration* command: It loads .root files found in the txt file “example-inputs/fnames.txt”, and extracts the run numbers which are used for the hltGetConfiguration command to create python HLT menus. The menus are then filtered for the HLT paths and L1T seeds. The information is then stored in “hlt_l1_seeds/hlt_l1.csv” with the following columns:

HLT path, Seeds (run #1), seeds (run #2), …

*which requires the CMSSW environment

##### Flags

`--FROM_MENU`: Use HLT python menus stored in MENUDIR instead of creating them from the .root files

`--MENUDIR`: Specify location of HLT python menus. Default: 'temp/'

`--ROOTFILES`: Specify location of the .txt file. Default: example-inputs/fnames.txt’

`--TEMPDIR`: Specify the directory to which the HLT menus are stored. Default: ‘temp/’

`--KEEP_TEMPDIR`: Without this flag, the TEMPDIR is deleted after seeds.py has finished. Useful for testing as the HLT menu generation process can be skipped using the --FROM_MENU flag.

`--SAVEDIR`: Specify the directory to which the csv file is stored. Default: ‘hlt_l1_seeds’

`--REDIRECTOR`: Specify the redirector used to load data. Default: ‘root://xrootd-cms.infn.it/’

`--MAX_FILE`: Set the number of .root files to be processed. Default: 1

`--CSV_NAME`: Specify the name of the output .root file. Default: ‘hlt_l1.csv’

#### 3bii. TriggerAcceptance.py

Calculates the total, pure, and proportional rates and their accepted counterparts, and stores histograms as a .root file and also in pdf and png formats. It takes the hlt_l1_seeds.csv file and the .txt file as input. It loads the events of the .root file and loops over each event to calculate the rates.

##### Flags

`--ROOTFILES`: Specify location of the .txt file. Default: example-inputs/fnames.txt’

`--HLT_L1_SEEDS`: Specify location of the .csv file containing the HLT-L1T information. Default: ‘./hlt_l1_seeds/hlt_l1.csv’

`--MAX_FILE`: Set the number of .root files to be processed. Default: 1

`--REDIRECTOR`: Specify the redirector used to load data. Default: ‘root://xrootd-cms.infn.it/’

`--MAX_EVT`: Set the maximum number of events to process. Default: 500000

`--PRT_EVT`: Print information every nth event. Default: 10000

`--VERBOSE`: Print extra stuff. Useful for debugging

`--PU_MIN`: Minimum pileup required. Default: 0

---------

[1]: https://indico.cern.ch/event/1032503/contributions/4340838/attachments/2237852/3793533/20210503_L1Tacceptance_DPG_Mark_Anna.pdf
