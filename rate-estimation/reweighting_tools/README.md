# reweighting_tools

Set of scripts useful in order to retrieve weights from LumiPOG models for the reweighting of the pileup distribution according to Run 3 predictions. 

> Find here the [Lumi POG tool](https://cmslumipog.web.cern.ch/cgi-bin/pileupDistribution.py) and related documentation!

## Overview
The possibility to apply a reweighting procedure to the flat Run 3 MC PU distribution has been included in order to take into account a realistic PU modelling according to Run 3 Lumi POG predictions. 
In fact, triggers are expected to behave in a different way under different PU conditions, so it is important to have a reliable description of PU to get reasonable predictions of rates. 

### Available scripts

#### extractLumiPOGweights.py
This script starts from a json file provided as output of the Lumi POG tool that contains the fraction of events for each PU value from 0 to 100.
It is used to fill a histogram of 100 bins that contains the fraction of events for each PU value from 1 to 100. 
Example command:
```
python3 extractLumiPOGweights.py --outfolder PU_REWEIGHTING --json pileup_distribution_2736_80mb_btb_PoissonStat_PU51.json
```
Two json files describing the gaussian PU distribution according to Lumi POG models are provided as an example:
* Average PU 53 = `pileup_distribution_53_2700_80mb_noPoisson_noLumiWeighted.json`
* Average PU 51 = `pileup_distribution_2736_80mb_btb_PoissonStat_LumiWeighted_PU51.json`
The second one is used to produce rootfiles provided as an example.
An example of the rootfile produced as output of this step is provided: `h_weights_puReweighting_LumiPOG_PU_gaussianStartOfTheFill.root`.

#### LumiPOGweights_pileupComp_createHistos.py
This script creates and saves histograms of the Run 3 pileup distribution and the flat Run 3 nPV_True distribution with the same binning and normalized to the same number of entries.
Example command:
```
python3 LumiPOGweights_pileupComp_createHistos.py --outfolder PU_REWEIGHTING --mc /eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/elfontan/condor/NuGun.root --histroot h_LumiPOG_PU_gaussianStartOfTheFill.root
```
An example of the rootfile produced as output of this step is provided: `h_LumiPOG_PU_gaussianStartOfTheFill.root`.

#### LumiPOGweights_pileupComp_plotter.py
This script compute and print weights as the bin per bin ratio between histograms saved in the previous step.
This list of weights can be then included in the rate-estimation code.
Example command:
```
python3 LumiPOGweights_pileupComp_plotter.py --outfolder PU_REWEIGHTING --histos h_PUreweighting_LumiPOG_PU_gaussianStartOfTheFill.root
```
An example of the rootfile produced as output of this step is provided: `h_PUreweighting_LumiPOG_PU_gaussianStartOfTheFill.root`.