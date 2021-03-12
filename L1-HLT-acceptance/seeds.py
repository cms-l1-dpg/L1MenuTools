import os
import csv
import utils.automate
import shutil
import argparse
import time

parser = argparse.ArgumentParser(description = 'Generate run specific HLT-L1 information from either ROOT files or HLT menus')
parser.add_argument('--from_menus', action= 'store_false', default = True, help = 'create HLT-L1 information from ROOT file (True) or from directory containing HLT menus (False)', dest = 'fromroot')
parser.add_argument('--ROOT_files', type = list, default = ['/afs/cern.ch/user/m/mmatthew/public/5613275E-184E-9A43-AA73-ACB61275D947.root'], dest = 'rootfiles', help = 'list of root files from which run specific HLT menus are created')
parser.add_argument('--temp_dir', type = str, default = 'temp/', dest = 'rootdir', help = 'define temporary directory where HLT menus created from root files are stored')
parser.add_argument('--keep_temp_dir', action = 'store_false', default=True, dest = 'delete', help = 'set False to keep the temporary directory')
parser.add_argument('--dir_menus', type = str, default = 'temp/', dest = 'menudir', help = 'define directory with HLT menus from which the HLT-L1 information is extracted')
parser.add_argument('--dir_hlt_l1', type = str, default = 'hlt_l1_seeds/', dest = 'savedir', help = 'define directory where HLT-L1 information is stored as a csv file with the header: HLT, run1, run2, ...')

args = parser.parse_args()

fromroot = args.fromroot
rootdir = args.rootdir
menudir = args.menudir
rootfiles = args.rootfiles
savedir = args.savedir
delete = args.delete

if not fromroot:
    delete = False

if fromroot:
    directory = rootdir
    utils.automate.hlt_GetConfig(directory, rootfiles)
    files = os.listdir(directory)

if not fromroot:
    directory = menudir
    files = os.listdir(directory)

runs = {}


for file in files:
    with open(directory + file,"r") as f:
        
        data = f.read()

    # Create dictionary with HLT paths as key and hltL1 objects as values

    HLT_paths={}
    for item in data.split('\n'):
        if "cms.Path(" in item and "process.hltL1" in item:
            hlt = item.split("_v")[0].split("process.")[1]
            HLT_paths[hlt] = []
            temp = item.split("process.hltL1")
            for i in range(1,len(temp)): # An HLT path can contain multiple hltL1 structures
                l1 = item.split("process.hltL1")[i].split(" +")[0]
                HLT_paths[hlt].append(l1)

    # hltL1 combine multiple L1 seeds. Create mapping between hltl1 objects to all L1 seeds

    L1_seeds={}
    for ele in data.split("\n)\n"):
        if " = cms.EDFilter" in ele:
            for seed in HLT_paths.values():
                for ding in seed:
                    if ding in ele and "cms.string( " in ele:
                        L1_seeds[ding] = ele.split('cms.string( "')[1].split('" ),\n')[0]

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

keys = set([ele for run in runs for ele in runs[run]])

if not os.path.exists(savedir):
    os.mkdir(savedir)

with open(savedir+"test.csv","w") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(["HLT"] + runs.keys())

    for key in keys:
        # Count the max number of hltL1 structures for single HLT path
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

if delete:
    shutil.rmtree(directory)
    print("Deleted temporary directory: " + directory)
print("Process finished.")


