import os
import csv
import utils.automate
import shutil
import argparse
import time

parser = argparse.ArgumentParser(description = 'Generate run specific HLT-L1 information from either ROOT files or HLT menus')
parser.add_argument('--FROM_ROOT', action= 'store_true', default = True, help = 'create HLT-L1 information from ROOT files (True) or from directory containing HLT menus (False)', dest = 'fromroot')
parser.add_argument('--ROOT_FILES', type = str, default = 'rootfiles/test.txt', dest = 'rootfiles', help = 'list of ROOT filenames from which run specific HLT menus are created')
parser.add_argument('--TEMPDIR', type = str, default = 'temp/', dest = 'tempdir', help = 'define temporary directory where HLT menus created from ROOT files are stored')
parser.add_argument('--KEEP_TEMPDIR', action = 'store_false', default=True, dest = 'keep', help = 'delete (False) or keep (True) the temporary directory')
parser.add_argument('--MENUDIR', type = str, default = 'temp/', dest = 'menudir', help = 'define directory with HLT menus from which the HLT-L1 information is extracted')
parser.add_argument('--SAVEDIR', type = str, default = 'hlt_l1_seeds/', dest = 'savedir', help = 'define directory where HLT-L1 information is stored as a csv file with the header: HLT, run1, run2, ...')
parser.add_argument('--REDIRECTOR', type = str, default = 'root://xrootd-cms.infn.it/', dest = 'redirector', help = 'set redirector to load ROOT files from')
parser.add_argument('--MAX_FILE', type = int, default = 10, dest = 'maxfiles', help = '# of root files to process from list')

args = parser.parse_args()

## For ease of debugging

fromroot = args.fromroot
tempdir = args.tempdir
menudir = args.menudir
fnames = args.rootfiles
savedir = args.savedir
keep = args.keep
redirector = args.redirector
maxfiles= args.maxfiles

## Create the HLT menus using the hlt_GetConfig command

if fromroot:
    directory = tempdir
    utils.automate.hlt_GetConfig(directory, fnames, maxfiles, redirector)
    files = os.listdir(directory)

## Load the HLT menus directly from menudir

if not fromroot:
    keep = True
    directory = menudir
    files = os.listdir(directory)

## Create nested dictionary containing the HLT-L1 info for corresonding runs

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

    # Create dictionary mapping HLT paths to L1 seeds

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


## Get all HLT paths

keys = set([hlt for run in runs for hlt in runs[run]])

if not os.path.exists(savedir):
    os.mkdir(savedir)

with open(savedir+"test.csv","w") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["HLT"] + runs.keys())

    for key in keys:
        # Count the max number of hltL1 structures for a single HLT path
        ls = []
        for run in runs.keys():
            try:
                ls.append(len(runs[run][key]))
            except KeyError:
                ls.append(len([]))

        m = max(ls)
        for i in range(m): # Iterate through all seeds in possible multiple hltL1 structures for a single HLT path
            ls = []
            ls.append(key)
            for run in runs.keys():
                try:
                    ls.append(runs[run][key][i])
                except KeyError:
                    ls.append(None)

            writer.writerow(ls)

print("HLT_L1_seeds generated.")

if not keep:
    shutil.rmtree(directory)
    print("Deleted temporary directory: " + directory)
print("Process finished.")


