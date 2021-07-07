# L1 Rate Visualization Tool

## Usage
To get the L1 menu rates visualized in form of a pie chart and a bar chart, run the following command:
```
bash run-visualize.sh --rateTable <rateTable.txt> --output <output name> --textOnBarPlot <select an option>

```
* `--rateTable`: Downloaded a rate table from which the rates should be visualized.
  The rate table is obtained by the L1MenuTools rate-estimation/ framework.
  (default: `example-inputs/output_Run2unpacked.csv`)
* `--rate`     : Type of rate to be plotted (must correspond to a column header of the
  input rate table file)
* `--output`   : Specify the name of pieChart and barPlot to be saved.

* `--textOnBarPlot`   : Specify what you want to plot on the bar plot. 
  You can choose from four options, "percentage","percentage+totalrate", "percentage+rates+totalrate" and "rates+totalrate".
  (default is `percentage+totalrate`)


It will save four plots, `output_pieChart.{png,pdf}` and `output_barPlot.{png,pdf}`.


## Seed Categorization Logic
**Seeds are categorized by their names.** Each category is formed by some commonalities in the names, e.g., the "isSingleMuSeed" category should include seeds which have "SingleMu" in the name but should exclude seeds containing "Jet", "EG", "Tau", "HTT" etc.

So, per category, *identifiers* and *vetoes* are defined, which define strings that should or should not be contained in the L1 seed name, respectively.

In addition, for some seeds with jets (e.g., "isMuJetSeed") other selection criteria are added on top of the identifiers and vetoes (e.g., "Jetsums", which will contain "Jet", "HTT","EMT" etc.)
If the seed has identifier(Mu) and any of the Jetsums in its name, that seed is categorized as "isMuJetSeed". 

Category | Identifiers | Vetoes | Additional requirements |
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
ZeroBias| ZeroBias| EG,Mu,Tau,Jet,ETM,HTT,ETT,ETMHF| |
Calibration| EG,Mu,Tau,Jet,ETM,HTT,ETT,ETMHF,ZeroBias| | |

"Calibration" seeds are all seeds with prescale value > 1.
