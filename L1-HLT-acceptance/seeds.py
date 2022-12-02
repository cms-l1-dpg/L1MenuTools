#################################################################################################################
############  seeds.py : A script to map the Level-1 Trigger paths to the seeded by them HLT paths  #############
#################################################################################################################
## First development of the code (Accept. study 2021): Mark Matthewman
##################################################################################################################

import os
import csv
import utils.automate
import shutil
import argparse
import time

parser = argparse.ArgumentParser(description = 'Generate run specific HLT-L1 information from either ROOT files or HLT menus')
parser.add_argument('--FROM_MENU', action= 'store_true', help = 'Create HLT-L1 information from HLT menus stored as python files in MENUDIR')
parser.add_argument('--ROOTFILES', type =str, default = 'example-inputs/fnames.txt', help = 'txt file containing ROOT fnames')
parser.add_argument('--TEMPDIR', type = str, default = 'temp/', help = 'Temporary directory where HLT menus are stored as python files')
parser.add_argument('--KEEP_TEMPDIR', action = 'store_true', help = 'Keep the temporary directory with HLT menus as python files')
parser.add_argument('--MENUDIR', type = str, default = 'temp/', help = 'Directory with HLT menus as python files')
parser.add_argument('--SAVEDIR', type = str, default = 'hlt_l1_seeds/', help = 'Directory to save csv file to.')
parser.add_argument('--REDIRECTOR', type=str, default='root://xrootd-cms.infn.it/', help='Redirector used to access ROOT files')
parser.add_argument('--MAX_FILE', type = int, default = 1, help = 'Maximum number of input files to process')
parser.add_argument('--CSV_NAME', type = str, default = 'hlt_l1.csv',  help = 'Name of csv file where run specific HLT-L1 combinations will be stored')

args = parser.parse_args()

## For ease of debugging

frommenu = args.FROM_MENU
tempdir = args.TEMPDIR
menudir = args.MENUDIR
fnames = args.ROOTFILES
savedir = args.SAVEDIR
keep = args.KEEP_TEMPDIR
redirector = args.REDIRECTOR
maxfiles = args.MAX_FILE
csvname = args.CSV_NAME

## Case1: *Create* the HLT menus applying the hlt_GetConfig command (run by automate.py) on the input .root files 

if not frommenu:
    directory = tempdir
    utils.automate.hlt_GetConfig(directory, fnames, maxfiles, redirector)
    files = os.listdir(directory)

## Case2: *Load* the HLT menus directly from menudir

if frommenu:
    keep = True
    directory = menudir
    files = os.listdir(directory)

## Create nested dictionary containing the HLT-L1 info for the corresponding runs

runs = {}
for file in files:
    with open(directory + file,"r") as f:
        data = f.read()

    # Create dictionary with HLT paths as keys and hltL1 objects as values

    HLT_paths={}
    for item in data.split('\n'):
        if "cms.Path(" in item and "process.hltL1" in item:
            hlt = item.split("_v")[0].split("process.")[1]
            HLT_paths[hlt] = []
            hltl1s = item.split("process.hltL1")
            for i in range(1,len(hltl1s)): # An HLT path can contain multiple hltL1 structures
                hltl1 = item.split("process.hltL1")[i].split(" +")[0]
                HLT_paths[hlt].append(hltl1)

    # hltL1 combine multiple L1 seeds. Create mapping between hltl1 objects to all L1 seeds

    L1_seeds={}
    for ele in data.split("\n)\n"):
        if " = cms.EDFilter" in ele:
            for hltl1s in HLT_paths.values(): 
                for hltl1 in hltl1s: # An HLT path can contain multiple hltL1 structures
                    if hltl1 in ele and "cms.string( " in ele:
                        L1_seeds[hltl1] = ele.split('cms.string( "')[1].split('" ),\n')[0]

    # Create dictionary mapping HLT paths to L1T seeds

    combined = {}
    for key in HLT_paths.keys():
        if key not in combined.keys():
            combined[key] = []
        for path in HLT_paths[key]:
            try:
                combined[key].append(L1_seeds[path])
            except:
                pass

    runs[file.split(".py")[0]] = combined


## Get all HLT paths. Mapping info in .csv file, which is then fed to TriggerAcceptance.py

keys = set([hlt for run in runs for hlt in runs[run]])

if not os.path.exists(savedir):
    os.mkdir(savedir)

with open(savedir+csvname,"w") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["HLT"] + list(runs))

    for key in keys:
        # Count the max number of hltL1 structures for a single HLT path
        ls = []
        for run in list(runs):
            try:
                ls.append(len(runs[run][key]))
            except KeyError:
                ls.append(len([]))

        m = max(ls)
        for i in range(m): # Iterate through all seeds in possible multiple hltL1 structures for a single HLT path
            ls = []
            ls.append(key)
            for run in list(runs):
                try:
                    ls.append(runs[run][key][i])
                except KeyError:
                    ls.append(None)

            writer.writerow(ls)

print("HLT_L1_seeds generated: {}".format(savedir + csvname))


if not keep:
    shutil.rmtree(directory)
    print("Deleted temporary directory: " + directory)
print("Process finished.")


