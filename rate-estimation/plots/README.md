# Useful plots for L1 Menu studies

## Rate VS pileup plot

To produce rate VS pileup plots, the output file from the rate estimation when running with the --doPrintPU argument is needed. 
See the [Menu Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToL1TriggerMenu#3_Run_3_setting) for full steps.
The code can be used for both MC and data ntuples. 
Note that when using data, you need the specific lumi csv file produced by running the GetLumi.py for the considered runs to run the rate estimation.

The following arguments can be provided to run the script: 
* Name of the output folders: --outfolder
* Name of the csv file containing rates as a function of pileup: --csv
* Name of the L1 seeds for which rates are plotted as a function of the pileup: --seed 
* Boolean choice (False or True) if the reweighting is performed or not: --reweightRun3 or --reweight2018

Default settings correspond to the following command provided as an example:
```
python3 CompPUDep.py --outfolder 20210702_Run2ZB_fill7005_Prescale_2022_v0_1_1 --csv Run3_ZeroBias_123_20212906_rateVSpileup_PU.csv --seed L1APhysics --reweightRun3 False
```

NOTES: 
* It is possible to provide a list of L1 seed names as an argument, for example --seed L1APhysics L1_DoubleMu_12_5 L1_DoubleEG8er2p5_HTT300er (no comma needed)
* The option "--seed all" redefines the PatMap to take into account each L1Seed present in the dataframe  
