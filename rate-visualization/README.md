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
python visualize.py --rateTable menu_v2_1_0_rate2.txt --output rates_menu_v210

```
* ` --rateTable`: Downloaded rate table from which the rates should be visualized.
* ` --output`   : Specify name of pieChart and barPlot to be saved.

It will save 4 plots, "output_pieChart"(png,pdf) and "outpu_barPlot"(png,pdf).




## Seed Categorization



Seeds are categorize by their names. Each category is formed by some commonalities in the names, e.g., isSingleMuSeed category should include seeds which have "SingleMu" in the name 
but should exclude the seeds which contain "Jet","EG", "Tau","HTT" etc in addition to "SingleMu".

So, per category, we define identifiers and vetoes, which as their names suggest help us define what the name should have and what we skip if found.

In addition, for some seeds with jets e.g., isMuJetSeed we add another selection criterion on top of identifiers and vetoes, Jetsums, which will contain "Jet", "HTT","EMT" etc. 
If the seed has identifier(Mu) and any of the Jetsums in it's name that seed is categorized as isMuJetSeed. 


