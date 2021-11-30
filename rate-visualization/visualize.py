import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import tempfile
import argparse
import wget
import random
#plt.rcParams.update({
#    "text.usetex": True,
#    "font.family": "sans-serif",
#    "font.sans-serif": ["Helvetica"]})
# define CLI elements
parser = argparse.ArgumentParser()
parser.add_argument('--rateTable',
	help='Existing rate table',
	type=str)
parser.add_argument('--output',
	help='Name of created output file',
	default='output_RateViz',
	type=str)
parser.add_argument('--rate',
	help='Type of rate used in plots',
	default='propotional0',
	type=str)
parser.add_argument('--textOnBarPlot',
	help='What should be diplayed on the bar-plot',
	default='percentage+totalrate',
	action = "store",
	choices = ("percentage","percentage+totalrate","percentage+rates+totalrate","rates+totalrate"),
	type=str)
args = parser.parse_args()

csv_rate_table = args.rateTable
outputFileName = args.output
rateType       = args.rate 
text	       = args.textOnBarPlot
if not os.path.exists(csv_rate_table):
	os.system( "wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuTools/master/piechart-tool/{}".format(csv_rate_table) ) 
else:
	print("Rate table exists, no need to download it")

# reading the rate table
df = pd.read_csv(csv_rate_table, sep=",")



# preparing a dataframe with PS==1 seeds only (only for the sake of this example), using some Boolean slicing of the df dataframe
df_PS1 = df[df["pre-scale0"] >= 1]
df_PS = df[df["pre-scale0"] > 1]

# definition of one of the categorization functions
# (mind the extra @np.vectorize decorator - this is necessary to parallelize the function application call for entire columns later on)
@np.vectorize
def isSingleMuSeed(seedname):
    identifiers = ["SingleMu"]
    vetoes = ["Jet", "EG", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias", "upt"]
    return all([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )

@np.vectorize
def isLLPSeed(seedname):
    identifiers = ["MuShower", "upt"]
    vetoes = ["Jet", "EG", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias"]
    return ( 
	any([identifier in seedname for identifier in identifiers]) 
        and not any([veto in seedname for veto in vetoes])
        
    )

@np.vectorize
def isMultiMuSeed(seedname):
    identifiers = ["DoubleMu", "TripleMu", "QuadMu"]
    vetoes = ["Jet", "EG", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias", "upt"]
    return any([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isMuEGSeed(seedname):
    identifiers = ["Mu", "EG"]
    vetoes = ["Jet", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias", "upt"]
    return all([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isMuJetSeed(seedname):
    identifiers = ["Mu"]
    Jetsums = ["Jet", "ETM", "HTT", "ETT", "ETMHF"]
    vetoes = ["EG", "Tau", "ZeroBias", "upt"]
    return (
        all([identifier in seedname for identifier in identifiers])
        and any([Sum in seedname for Sum in Jetsums])
        and not any([veto in seedname for veto in vetoes])
    )


@np.vectorize
def isSingleEGSeed(seedname):
    identifiers = [
        "SingleEG",
        "SingleIsoEG",
        "SingleLooseEG",
        "SingleLooseIsoEG",
        "IsoEG",
    ]
    # EGsums = ["Single","Iso"]
    vetoes = ["Jet", "Mu", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias", "Double"]
    return any([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isMultiEGSeed(seedname):
    identifiers = ["DoubleEG", "TripleEG", "DoubleIsoEG", "DoubleLooseIsoEG"]
    vetoes = ["Jet", "Mu", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias"]
    return any([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isEGJetSeed(seedname):
    identifiers = ["EG"]
    Jetsums = ["Jet", "ETM", "HTT", "ETT", "ETMHF"]
    vetoes = ["Mu", "Tau", "ZeroBias"]
    return (
        all([identifier in seedname for identifier in identifiers])
        and any([Sum in seedname for Sum in Jetsums])
        and not any([veto in seedname for veto in vetoes])
    )


@np.vectorize
def isJetSeed(seedname):
    identifiers = ["Jet"]
    vetoes = ["EG", "Mu", "Tau", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias"]
    return all([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isTauSeed(seedname):
    identifiers = ["Tau"]
    vetoes = ["EG", "Mu", "Jet", "ETM", "HTT", "ETT", "ETMHF", "ZeroBias"]
    return all([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isLepJetSeed(seedname):
    identifiers = ["Tau"]
    Jetsums = ["Jet", "Mu", "EG", "ETM", "HTT", "ETT", "ETMHF"]
    vetoes = ["ZeroBias"]
    return (
        all([identifier in seedname for identifier in identifiers])
        and any([Sum in seedname for Sum in Jetsums])
        and not any([veto in seedname for veto in vetoes])
    )


@np.vectorize
def isSumsSeed(seedname):
    identifiers = ["Jet"]
    Jetsums = ["ETM", "HTT", "ETT", "ETMHF"]
    vetoes = ["EG", "Mu", "Tau", "ZeroBias"]
    return any([Sum in seedname for Sum in Jetsums]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isZeroBiasSeed(seedname):
    identifiers = ["ZeroBias"]
    vetoes = ["EG", "Mu", "Tau", "Jet", "ETM", "HTT", "ETT", "ETMHF"]
    return all([identifier in seedname for identifier in identifiers]) and not any(
        [veto in seedname for veto in vetoes]
    )


@np.vectorize
def isCalibrationSeed(seedname):
    # function is intended for use with PS > 1 seeds only
    identifiers = [
        "EG",
        "Mu",
        "Tau",
        "Jet",
        "ETM",
        "HTT",
        "ETT",
        "ETMHF",
        "ZeroBias",
        "Bunch",
        "Train",
    ]
    return any([identifier in seedname for identifier in identifiers])


functions = {
    "isSingleMuSeed": isSingleMuSeed,
    "isMultiMuSeed": isMultiMuSeed,
    "isMuEGSeed": isMuEGSeed,
    "isSingleEGSeed": isSingleEGSeed,
    "isMuJetSeed": isMuJetSeed,
    "isSingleEGSeed": isSingleEGSeed,
    "isMultiEGSeed": isMultiEGSeed,
    "isEGJetSeed": isEGJetSeed,
    "isJetSeed": isJetSeed,
    "isTauSeed": isTauSeed,
    "isLepJetSeed": isLepJetSeed,
    "isSumsSeed": isSumsSeed,
    "isZeroBiasSeed": isZeroBiasSeed,
    "isCalibrationSeed": isCalibrationSeed,
    "isLLPSeed": isLLPSeed,
}
# ... define the rest of the functions here, one for each category...
# extend the dataframe with new columns that contain the return values of the specific categorization functions
# thanks to the no.vectorize decorator in the function definition, pandas knows to run this "row-by-row" internally
# if df_PS1["PS"].all() == 1:
for category in [
    "isSingleMu",
    "isMultiMu",
    "isMuEG",
    "isMuJet",
    "isSingleEG",
    "isMultiEG",
    "isEGJet",
    "isJet",
    "isTau",
    "isLepJet",
    "isSums",
    "isZeroBias",
    "isLLP",
]:  # can be later extended ["isSingleMu", "isMultiMu",...]
    func = category + "Seed"
    function = functions[func]
    df_PS1[category] = function(df_PS1["L1SeedName"])
df_PS["isCalibration"] = isCalibrationSeed(df_PS["L1SeedName"])
# if df_PS1["PS"].all() > 1:
# df_PS1["isCalibration"] = isCalibrationSeed(df_PS1["L1SeedName"])

# inspect the newly added column(s)
#df_PS1

# Eventually, one should also automatically check and make sure that there is
# only one "True" value per row for each row in the dataframe (ie, each seed
# can only be in a single category)!

# Example: inspect all (PS==1) seeds that have been identified as SingleMu seeds
#df_PS1[df_PS1["isSingleMu"] == True]
#df_PS1[df_PS1["isSingleEG"] == True]
#df_PS[df_PS["isCalibration"] == True]

# extracting the sum of proportional rates for all seeds which have isSingleMu == True
# in this dummy example, we should add up 3960.02 + 2610.46 = 6570.48
sums = {}
sorted_sums={}
#for rate_type in ["rate0", "pure0", "propotional0"]:
for rate_type in ["{}".format(rateType)]:
    sums[rate_type] = {}
    sorted_sums[rate_type] = {}
    # calculating calibration for PS_rate as done in Aloke's script
    # sums[rate_type]["isCalibration"] = (df_PS[df_PS["isCalibration"] == True][rate_type]).sum()
    sums[rate_type]["isCalibration"] = (
        df_PS[df_PS["isCalibration"] == True]["rate0"]
    ).sum()
    for category in [
        "isSingleMu",
        "isMultiMu",
        "isMuEG",
        "isMuJet",
        "isSingleEG",
        "isMultiEG",
        "isEGJet",
        "isJet",
        "isTau",
        "isLepJet",
        "isSums",
        "isZeroBias",
	"isLLP",
    ]:  # can be extended later: ["isSingleMu", "isMultiMu",...]
        sums[rate_type][category] = (df_PS1[df_PS1[category] == True][rate_type]).sum()
sumCalib = {}
for rate_type in ["{}".format(rateType)]:
#for rate_type in ["rate0", "pure0", "propotional0"]:
    sumCalib[rate_type] = {}
    sumCalib[rate_type]["isCalibration"] = (
        df_PS[df_PS["isCalibration"] == True][rate_type]
    ).sum()
# check the results
#from pprint import pprint

 #pprint(sums)
# pprint(sumCalib)

# once all rates have been extracted per category and (probably best) stored in
# a separate data structure (like in the dictionary above), one can go to the
# plotting step...
sorted_sums[rateType] = sorted(sums[rateType].items(), key=lambda x: x[1], reverse=True)
#print (sorted_sums)
categories={}

categories = {
    "isSingleMu": {"texName":'Single $\mu$'},
    "isMultiMu": {"texName":'Multi $\mu$'},
    "isMuEG":    {"texName":'$\mu$ + e/$\gamma$'},
    "isMuJet":   {"texName": '$\mu$ + Jets or Energy sums'},
    "isLLP":   {"texName": 'Long Lived Particle'},
    "isSingleEG":{"texName":'Single e/$\gamma$'},
    "isMultiEG": {"texName":'Multi e/$\gamma$'},
    "isEGJet":{"texName":'e/$\gamma$ + Jets or Energy sums'},
    "isJet":{"texName":'Single or Multi Jets'},
    "isTau":{"texName":'Single or Multi $\\tau$'},
    "isLepJet":{"texName":'$\\tau$ + $\mu$ or e/$\gamma$ or Jets or Energy sums'},
    "isSums":{"texName":'Energy Sums'},
    "isZeroBias":{"texName":'Zero Bias'},
}
labels = []
rates = []
percnt = []
for x in sorted_sums[rateType]:
	for cat in categories.keys():
		#print (cat)
		if x[0]	== cat and x[1]>0:
			
			print (x[1])
			print(categories[cat]['texName']) 
			print ('********')
			labels.append(categories[cat]['texName'])
			rates.append(x[1])			

#for x, y in sorted_sums["{}".format(rateType)].items():
#    print("{}: ".format(rateType),x, y)
#    labels.append(x[2:])
#    rates.append(y)
##for x, y in sums["rate0"].items():
##    print("pure rate: ", x, y)

plt.rc('text', usetex=True, )
plt.rc('font', family='serif')
plt.rc('font', family='helvetica')
#colours = dict(zip(labels, plt.cm.tab20.colors[:len(labels)]))
#for i,l in enumerate(labels):
#	colours[l] = "C{}".format(i)
#fix colors per category
#colours={
#
#    "Single $\mu$": "C0",
#    "Multi $\mu$": "C1",
#    "$\mu$ + e/$\gamma$":    "C2",
#    "$\mu$ + Jets or Energy sums":   "C3",
#    "Single e/$\gamma$":"C4",
#    "Multi e/$\gamma$": "C5",
#    "e/$\gamma$ + Jets or Energy sums":"C6",
#    "Single or Multi Jets":"C7",
#    "Single or Multi $\\tau$":"C8",
#    "$\\tau$ + $\mu$ or e/$\gamma$ or Jets or Energy sums":"C9",
#    "Energy Sums":"cyan",
#    "Zero Bias":"yellow",
#}

colours={

    "Single $\mu$": "orange",
    "Multi $\mu$": "cornflowerblue",
    "$\mu$ + e/$\gamma$":    "brown",
    "$\mu$ + Jets or Energy sums":   "red",
    "Single e/$\gamma$":"green",
    "Multi e/$\gamma$": "pink",
    "e/$\gamma$ + Jets or Energy sums":"magenta",
    "Single or Multi Jets":"grey",
    "Single or Multi $\\tau$":"yellow",
    "$\\tau$ + $\mu$ or e/$\gamma$ or Jets or Energy sums":"lime",
    "Energy Sums":"cyan",
    "Zero Bias":"teal",
    "Long Lived Particle":"silver",

}
wedges, lab, pct_text=plt.pie(rates, labels=labels, autopct="%1.1f\%%", labeldistance=1.0, pctdistance=0.6, rotatelabels=True, colors=[colours[key] for key in labels])
#wedges, lab, pct_text=plt.pie(rates, labels=labels, autopct="%1.1f\%%", labeldistance=1.0, pctdistance=0.6, rotatelabels=True, colors=plt.cm.tab20.colors)
#wedges, lab, pct_text=plt.pie(rates, labels=labels, autopct="%1.1f%%", labeldistance=1.0, pctdistance=0.6, rotatelabels=True, colors=mcolors.TABLEAU_COLORS)
for label, pct_text in zip(lab, pct_text):
	pct_text.set_rotation(label.get_rotation())
	
plt.axis("equal")
fig=plt.gcf()
plt.tight_layout()
plt.show()
ext=['png','pdf']
for e in ext:
	fig.savefig("{}_pieChart.{}".format(outputFileName,e),transparent=True)
	print ("File saved: {}_pieChart.{}".format(outputFileName,e))

plt.rc('font', size=16, weight='bold')
## for getting percentages
tot = sum(rates)
#print ("sum of all rate: ", tot)
highRate = False
for rate in rates:
	if (rate*0.001)>100:
		highRate = True
		break
print( "high rate? : ", highRate)
figbar, ax = plt.subplots(figsize=(12,6)) 
ax.barh(labels, rates, edgecolor='black',color='None',align='center', alpha=0.5)
figbar.subplots_adjust(left= 0.24)
#figbar.subplots_adjust(right=1/15.0, left=1-1/15.0, top=1/4.0, bottom=1-1/4.0, wspace=0.2)
for i, v in enumerate(rates):
	#print ("y-location: ", v)
	
	if text == "percentage+totalrate" or text == "percentage":
		#ax.text(v*1.1, i, '{:.1f}{}'.format((v/tot)*100,'\%'),horizontalalignment='left', color='black', va='center', fontweight='bold')
		#if tot*0.001 <300:
		if highRate:
			ax.text(v+3000, i, '  {:.1f}{}'.format((v/tot)*100,'\%'),horizontalalignment='left', color='black', va='center', fontweight='bold')
		else:
			ax.text(v+300, i, '  {:.1f}{}'.format((v/tot)*100,'\%'),horizontalalignment='left', color='black', va='center', fontweight='bold')
	if text == "percentage+rates+totalrate" :
		if highRate:
			ax.text(v+3000 , i, "                    "+'{:.1f}{}      [{:.1f}{}]'.format((v/tot)*100,'\%',v*0.001,' kHz'),horizontalalignment='left', color='black', va='center')
		else:
			ax.text(v+300 , i, "                    "+'{:.1f}{}      [{:.1f}{}]'.format((v/tot)*100,'\%',v*0.001,' kHz'),horizontalalignment='left', color='black', va='center')
	if text == "rates+totalrate":
		if highRate:
			ax.text(v+3000 , i,'{:.1f}{}'.format(v*0.001,' kHz'), horizontalalignment='left',color='black', va='center')
		else:
			ax.text(v+300 , i,'{:.1f}{}'.format(v*0.001,' kHz'), horizontalalignment='left',color='black', va='center')

plt.ylabel('Seed Categories', size=15)
plt.xlabel('Fractions of the {:.1f} kHz rate allocation for single- and multi-object triggers and cross triggers'.format(df_PS1.iloc[-1,3]), size=15)
if "totalrate" in text:
	#plt.title('L1 Rates for Seed Categories\n(Total rate: {:.1f} kHz)'.format(df_PS1.iloc[-1,3]) )
	ax.set_title('L1 Rates for Seed Categories\n(Total rate: {:.1f} kHz)'.format(df_PS1.iloc[-1,3]) , loc = 'left')
else:
	#plt.title('L1 Rates for Seed Categories')
	ax.set_title('L1 Rates for Seed Categories', loc= 'left')
plt.box(False)
ax.get_xaxis().set_ticks([])
ax.tick_params(axis='both', which='both', length=0)
barfig = plt.gcf()
plt.tight_layout()
plt.show()
for e in ext:
	barfig.savefig("{}_{}_barPlot.{}".format(outputFileName,text,e),transparent=True)
	print ("File saved: {}_barPlot.{}".format(outputFileName,text,e))

#print ("last line of df: ", df_PS1.iloc[-1,3])
# df_PS1[df_PS1["isSingleEG"] == True]
df_PS1[df_PS1["isMultiMu"] == True]
## check which seed is in which category
#print (df_PS1[df_PS1["isMultiEG"]==True])
#print (df_PS1[df_PS1["isSingleMu"]==True])
#print (df_PS1[df_PS1["isMultiMu"]==True])
#print (df_PS1[df_PS1["isLepJet"]==True])
#print (df_PS1[df_PS1["isMuJet"]==True])
print (df_PS1[df_PS1["isLLP"]==True])

print (df_PS[df_PS["isCalibration"]==True])
# make sure that each seed is in only one category
for i in range(len(df_PS1)):
    if list((df_PS1.iloc[i][7:])).count(True) > 1:
    #if list((df_PS1.iloc[i][7:])).count(True) == 0:
        #print ( (df_PS1.iloc[i][7:]) )
        print(
            "Inconsistent categorization: {} PS1: {}, prop_rate: {}".format(
                (df_PS1.iloc[i, 1]), df_PS1.iloc[i, 2], df_PS1.iloc[i, 6]
            )
        )
