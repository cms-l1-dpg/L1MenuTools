#!/usr/bin/bash
#Andrew Loeliger, Nov 15, 2022
#Based on seeds.py originally created by Mark Matthewman and provided by Anna Stakia
#Script for taking L1 ntuples, getting all availble runs
#And mapping L1 bits to HLT paths

import argparse
import csv
import ROOT
from utils.L1NtupleUtils import *
import json
import re
import shutil

def main(args):
    #first task, let's chain the files together

    theChain = ROOT.TChain('l1EventTree/L1EventTree')

    with open(args.ListOfFiles, mode='r') as csvFile:
        csvReader = csv.reader(csvFile)
        for row in csvReader:
            theFile = row[0]
            theChain.Add(args.Redirector+theFile)
    #and figure out the list of unique runs that we have
    uniqueRuns = getListOfUniqueEntries(theChain=theChain, branchName='run')

    #After we have a list of unique runs, we need to get the HLT
    #cnofiguration for all of these things
    createHLTConfigurations(uniqueRuns, args.TempDir)

    #This is going to be the thing we serialize to json at the end of this
    #I would expect the basic structure should be
    #runInfo[run][hltPath] = list of L1 bit configurations
    runInfo = {} 

    #Once we have the configurations, for each of them, we go through
    for run in uniqueRuns:
        print('Run: ', run)
        with open(f'{args.TempDir}/{run}.py', 'r') as theFile:
            configuration = theFile.read()
        #Now we find all lines where there is a process.HLT ... cms.Path,
        #this will let us map HLT paths to HLTL1 seeds
        hltPathLines = re.findall('process\.HLT.*v[0-9]+.*cms\.Path.*hltL1.*(?=\n)', configuration)
        hltPaths = {}
        for hltPath in hltPathLines:
            pathName = re.search('(?<=process\.)HLT.*(?=_v[0-9]+)',hltPath).group(0)
            hltL1Seeds = re.findall('(?<=process\.hltL1s)\w*(?=\s?\+?)', hltPath)
            hltPaths[pathName] = hltL1Seeds
        #Now, we have to go through and find the HLTL1Seeds EDFilters that correspond
        #to each hltL1seed we have. First we get a unique list of those
        uniqueHLTL1Paths = list(set([y for x in hltPaths.values() for y in x])) #not the best way to do this I think
        L1Seeds = {}
        for hltL1Path in uniqueHLTL1Paths:
            #get the text of the filter. Anything that has this thing's name in it, up to the next thing being added to the process
            theFilter = re.search('process\.hltL1s'+hltL1Path+'.*?(?=\nprocess)', configuration, flags=re.DOTALL).group(0)
            #then we get the seed. This has a very specific form that we can just test for
            theSeed = re.search('(?<=L1SeedsLogicalExpression = cms.string\( \").*(?=\")',theFilter).group(0)
            theSeed = re.split('\sOR\s', theSeed) #split the seed up into component L1 seeds if it's an OR
            L1Seeds[hltL1Path] = theSeed
        #Now we just go through and replace the original hlt -> hltL1seed mapping
        #with hlt -> l1 bit mapping
        for hltPath in hltPaths:
            flatPaths = []
            for i in range(len(hltPaths[hltPath])):
                #hltPaths[hltPath][i] = L1Seeds[hltPaths[hltPath][i]]
                flatPaths += L1Seeds[hltPaths[hltPath][i]]
            hltPaths[hltPath]=flatPaths
        #Now we map this information onto the run, and we're done with the run
        runInfo[run]=hltPaths
    if not os.path.exists(args.OutputDir):
        os.mkdir(args.OutputDir)
    with open(args.OutputDir+'hlt_l1.json','w') as outputFile:
        json.dump(runInfo, outputFile)
    shutil.rmtree(args.TempDir)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Generate run specific HLT')
    parser.add_argument(
        '--ListOfFiles',
        nargs='?',
        required=True,
        help='(CSV) List of L1Ntuple files to generate HLT-L1 bit corresponances with'
    )
    parser.add_argument(
        '--TempDir',
        nargs='?',
        default='temp/',
        help='Location to store HLT configurations'
    )
    parser.add_argument(
        '--Redirector',
        nargs='?',
        default='',
        help='Redirector for the root files, if necessary'
    )
    parser.add_argument(
        '--OutputDir',
        nargs='?',
        default='hlt_l1_seeds/',
        help='Directory to put the script output in'
    )

    args = parser.parse_args()

    main(args)