# Set of tools to analyse L1Ntuples

This repository collects some additional useful tools for rate studies to get standalone results from the standard L1MenuTools rate-estimation code.

## 1. Run 2 VS Run 3 comparison based on HLTPhysics data as a function of the pT threshold
An example to compute L1 rates starting from L1Ntuples based on the HLTPhysics dataset is provided.
In fact, this collection of data requires a specific rate calculation. Moreover, we needed to compute the rates for the main seeds with more granularity with respect to the set of algorithms available in the xml menu.

The main script starts from the uGMT and Calo Layer 2 objects (`l1UpgradeTree/L1UpgradeTree`) and applies by hand the cuts to reproduce the design of the main algorithms for each type of object:
```L1NtupleAnalyzer_functionOfPt_etaSplitting.py```.
It returns the number of countings for each seed type and each threshold.

The list of L1Ntuples for the comparison can be specified as argument from the command line (`--ntuples`) together with the year of the scenario under consideration (`--y`).
The two lists of files used to compare the Run 2 and Run 3 rates per bunch-crossing are also provided as default files:
```
  * EphemeralZBHLTPhysics_data_run325097.csv
  * EphemeralZBHLTPhysics_data_run357688_part0-8.csv
```

A jupyter notebook is provided to compute the rates starting from the list of countings for each seed and each threshold under consideration taking into account the proper scaling.
The approach for the scaling is described in the notebook.
It includes also the plotting part.
```
triggerRate_functionOfPt_etaRanges.ipynb
```