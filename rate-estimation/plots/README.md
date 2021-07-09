# Useful plots for L1 Menu studies

## Rate VS pileup plot

To produce rate VS pileup plots, the output file from the rate estimation when running with the --doPrintPU argument is needed. 
See the [Menu Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToL1TriggerMenu#3_Run_3_setting) for full steps.
The code can be used for both MC and data ntuples. 
Note that when using data, you need the specific lumi csv file produced by running the GetLumi.py for the considered runs to run the rate estimation.

Provide the following arguments to run the script: 
* Name of the output folders: --outfolder
* Name of the csv file containing rates as a function of pileup: --csv

Default settings correspond to the following command provided as an example:
```
python CompPUDep.py --outfolder 20210702_Run2ZB_fill7005_Prescale_2022_v0_1_1 --csv Run3_ZeroBias_123_20212906_rateVSpileup_PU.csv
```

The script can be edited to modify the PatMap, which contains the L1 seeds for which rates will be plotted as a function of the pileup