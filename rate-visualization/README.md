# L1 Rate Visualization Tool

## Setup

```
python3 -m venv env
source env/bin/activate
pip install -U pip
pip install -r requirements.txt

```

## User Guideline



To get the rates visualized in form of a pie chart and a bar chart, run the following command:
```
python visualize.py --rateTable <rateTable.txt> --output <output name>

```
* ` --rateTable`: Downloaded rate table from which the rates should be visualized. The rate table is obtained by the L1MenuTools rate-estimation framework.
		 menu_v2_1_0_rate2.txt is an example rate table obtained from late 2018 menu (L1Menu_Collisions2018_v2_1_0).
* ` --output`   : Specify name of pieChart and barPlot to be saved.

It will save 4 plots, "output_pieChart"(png,pdf) and "output_barPlot"(png,pdf).




## Seed Categorization



Seeds are categorize by their names. Each category is formed by some commonalities in the names, e.g., isSingleMuSeed category should include seeds which have "SingleMu" in the name 
but should exclude the seeds which contain "Jet","EG", "Tau","HTT" etc in addition to "SingleMu".

So, per category, we define identifiers and vetoes, which as their names suggest help us define what the name should have and what we skip if found.

In addition, for some seeds with jets e.g., isMuJetSeed we add another selection criterion on top of identifiers and vetoes, Jetsums, which will contain "Jet", "HTT","EMT" etc. 
If the seed has identifier(Mu) and any of the Jetsums in it's name that seed is categorized as isMuJetSeed. 

Category | Identifiers | Vetoes | additional requirements |
---|---|---|---|
SingleMu | SingleMu | Jet,EG,Tau,ETM,HTT,ETT,ETMHF,ZeroBias | |
MultiMu | DoubleMu, TripleMu, QuadMu |Jet,EG,Tau,ETM,HTT,ETT,ETMHF,ZeroBias | |
MuEG | Mu,EG | Jet,Tau,ETM,HTT,ETT,ETMHF,ZeroBias | |
MuJet | Mu | EG,Tau,ZeroBias| Jet,ETM,HTT,ETMHF|
SingleEG| SingleEG,SingleIsoEG,SingleLooseEG,SingleLooseIsoEG,IsoEG| Jet,Mu,Tau,ETM,HTT,ETT,ETMHF,ZeroBias,Double| |
MultiEG| DoubleEG,TripleEG,DoubleIsoEG,DoubleLooseIsoEG| Jet,Mu,Tau,ETM,HTT,ETT,ETMHF,ZeroBias,Double| |
EGJet| EG| Mu,Tau,ZeroBias| Jet,ETM,HTT,ETT,ETMHF|
Jet| Jet| EG,Mu,Tau,ETM,HTT,ETT,ETMHF,ZeroBias| |
Tau| Tau| EG,Mu,Jet,ETM,HTT,ETT,ETMHF,ZeroBias| |
LepJet| Tau| ZeroBias| EG,Mu,Jet,ETM,HTT,ETT,ETMHF|
Sums| | EG,Mu,Tau,ZeroBias| ETM,HTT,ETT,ETMHF|
|ZeroBias| ZeroBias| EG,Mu,Tau,Jet,ETM,HTT,ETT,ETMHF| |

For Calibration Seed, we take everything with prescale value > 1.


