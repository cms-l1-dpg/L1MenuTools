# Useful plots for L1 Menu studies

## Rate VS pileup plot

To produce rate VS pileup plots, the output file from the rate estimation when running with the --doPrintPU argument is needed. 
See the [Menu Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToL1TriggerMenu#3_Run_3_setting) for full steps.
The code can be used for both MC and data ntuples. 
Note that when using data, you need the specific lumi csv file produced by running the GetLumi.py for the considered runs to run the rate estimation.
Then use the following script:
```
python CompPUDep.py
```

Before running it, edit the script modifying at least:

* outfolder_details
* csvfile_rateVSpileup
* PatMap to provide the L1 seeds for which rates will be plotted as a function of the pileup