#! /usr/bin/env python
#Andrew Loeliger
#adaptation of efe's script to find the rate/efficiency of certain
#bunch crossing schemes

#Should take as input a csv file that lists a series of files that contain L1 ntuple information
#Strictly, it could just be formatted as a text file with a list fof these files?
#The other piece of input necessary, is a JSON provided by the LPC filling scheme tool that describes the filling scheme
#used in the data: https://lpc.web.cern.ch/cgi-bin/filling_schemes.py
#this can be used to figure out where the trains exist by looking at the LHC collision clock

import argparse
import ROOT
import csv
from tqdm import trange
import json
import math


#The best thing I've found to show where trains/bunch positions are the LPC LHC filling scheme collision clock
#It seems like those bunches in a train that are not empty will show a decrease in the collision clock positive delay
#where empty bunches show increasing positive delay
#We can get thse JSONs from the above link
def parseBXJSON(bxJSON):
    data = None
    with open(bxJSON,'r') as readFile:
        data = json.load(readFile)
    timingInformation = data['IP1_5']['collisionTimePhase']['phaseshift']
    trains, emptyBunches = determineTrainLocations(timingInformation)
    segmentedTrains = parseToContiguousBunches(trains)
    segmentedEmptyBunches = parseToContiguousBunches(emptyBunches)

    #let's also get some length information on a bunch by bunch basis
    trainLength = [len(x) for x in segmentedTrains]
    emptyBunchLengths = [len(x) for x in segmentedEmptyBunches]

    return trains, emptyBunches, segmentedTrains, segmentedEmptyBunches, trainLength, emptyBunchLengths
#This will go through and determine which bunch crossings are part of trains,
#and which are empty, based on the positive shift upward or downward of the LHC collision clock
#This information Will then later need to processed to understand our actual train structure
#(i.e., how big the trains are, versus the empty sections)
def determineTrainLocations(timingInformation):
    emptyBunches = []
    trains = []
    for i in range(0, len(timingInformation)):
        if timingInformation[i] > timingInformation[i-1]:
            emptyBunches.append(i)
        else:
            trains.append(i)

    return trains, emptyBunches

#this should just take out empty bunches, and train bunches
#which are just lists of bx numbers, and segments them into 
#a list of lists, based on whether the BXs are adjacent
def parseToContiguousBunches(theList):
    completeList = []
    currentList = []

    #Have to do the first bunch manually
    currentList.append(theList[0])
    for i in range(1, len(theList)):
        if theList[i] == theList[i-1]+1:
            currentList.append(i)
        else:
            completeList.append(currentList)
            currentList = [i]
    #Whatever the current list is, append it to the complete list,
    #and then return tghe complete list
    completeList.append(currentList)
    return completeList

#given a list of now empty trains, determine the relative position of a given bunch with respect to the train
#WWe'll simply return a list for each train, and we can count each train, with irrelevant bunches pushed into over or underflow
#Should adjust bunches which are best considered "ahead" into "negative" positions in front of the train
#And those which are best considered "behind" into positive positions behind the end of the train
def positionInTrain(bx, trains, finalBXinOrbit):
    positions = []
    for train in trains:
        position = bx-train[0]
        if position < math.floor((-1*finalBXinOrbit)/2): #count going the other direction around the circle
            position+=finalBXinOrbit
        elif position > math.floor(finalBXinOrbit/2): #count going the other direction around the circle
            position-=finalBXinOrbit
        positions.append(position)
    return positions
    
    
def main(args):
    ROOT.gROOT.SetBatch(True)

    #parse the bx json to figure out where the trains are.
    trains, emptyBunches, segmentedTrains, segmentedEmptyBunches, trainLength, emptyBunchLengths = parseBXJSON(args.bunchFillingJSON)
    #print(trains)
    #print(emptyBunches)
    #print(segmentedTrains)
    #print(segmentedEmptyBunches)
    #print(trainLength)
    #print(emptyBunchLengths)
    finalBXinOrbit = max(trains[len(trains)-1], emptyBunches[len(emptyBunches)-1])
    #print(finalBXinOrbit)

    maxTrainLength = 0
    for length in trainLength:
        maxTrainLength = max(maxTrainLength, length)

    #create chains and load in the files in the csv
    evtTree = ROOT.TChain('l1EventTree/L1EventTree')
    ugtTree = ROOT.TChain('l1uGTTree/L1uGTTree')

    with open(args.inputCSV, mode='r') as csvFile:
        csvReader = csv.reader(csvFile)
        for row in csvReader:
            theFile = row[0]
            evtTree.Add(theFile)
            ugtTree.Add(theFile)
    #Figure out the L1 trigger bits that we have available to us.
    #print(list(branch.GetName() for branch in list(ugtTree.L1uGT.GetListOfBranches())))
    #L1uGT = ugtTree.GetBranch("L1uGT")
    #print(L1uGT)
    #print(type(L1uGT))
    #print(list(branch.GetName()) for branch in list(L1uGT.GetListOfBranches()))
    #print(list(branch.GetName() for branch in list(L1uGT.GetListOfBranches())))

    #print(L1uGT.m_algoDecisionFinal)
    #print(L1uGT.FindBranch('m_algoDecisionFinal'))
    #m_algoDecisionFinal = L1uGT.FindBranch('m_algoDecisionFinal')
    
    histograms = []
    overallEventsPerBX = ROOT.TH1D('overallEventsPerBX', 'overallEventsPerBX', maxTrainLength, -4, maxTrainLength+3) #give 4 bunches on either end of the train
    for i in trange(ugtTree.GetEntries()):
        ugtTree.GetEntry(i)
        evtTree.GetEntry(i)
        if i == 0:
            #Do some histogram setup
            #This is dependent on the chain actually loading some information, which is why it is done in the loop in this weird way
            numAlgoBits = len(ugtTree.L1uGT.getAlgoDecisionFinal())
            histograms = [ROOT.TH1D(f'eventsPerBXBit{x}', f'eventsPerBXBit{x}', maxTrainLength, -4, maxTrainLength+3) for x in range(numAlgoBits)]
        theBX = evtTree.Event.bx
        #Now we need to figure out this bunch crossing's position with respect to or trains 
        trainPositions = positionInTrain(theBX, segmentedTrains, finalBXinOrbit)
        #print(trainPositions)
        for position in trainPositions:
            overallEventsPerBX.Fill(position)
            for j in range(len(ugtTree.L1uGT.getAlgoDecisionFinal())):
                if not ugtTree.L1uGT.getAlgoDecisionFinal(j):
                    continue
                histograms[j].Fill(position)
    theFile = ROOT.TFile(args.outputFile, 'RECREATE')
    overallEventsPerBX.Write()
    for histogram in histograms:
        histogram.Write()
    theFile.Close()

if __name__=='__main__':
    parser =argparse.ArgumentParser(description = 'Create plots for rate as a function of position')
    parser.add_argument('--inputCSV',
                        required=True, 
                        nargs='?',
                        help='CSV with list of input files')
    parser.add_argument('--bunchFillingJSON',
                        required=True,
                        nargs='?',
                        help='JSON file that describes the filling scheme to examine')
    parser.add_argument('--outputFile',
                        default='bunchPositionAnalysisOutput.root',
                        help='File name to save the output histograms under')

    args = parser.parse_args()

    main(args)
