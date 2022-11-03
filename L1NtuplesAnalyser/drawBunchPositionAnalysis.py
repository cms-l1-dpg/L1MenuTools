#!/usr/bin/env python3
#Andrew Loeliger
#Script for drawing the output of the bunch position drawing script
#Should take as input the output file of "bunchPositionAnalysis.py"
#and an already existing (probably empty) directory location to put plots in.

import ROOT
import argparse
import re

#Generic drawing function for all bits
def drawHistogram(theHistogram, yAxisTitle, outputLocation):
    theHistogram.SetLineColor(ROOT.kBlack)
    theHistogram.SetTitle('')
    theHistogram.SetMarkerColor(ROOT.kBlack)
    theHistogram.SetLineWidth(1)
    theHistogram.SetMarkerStyle(20)
    theHistogram.SetMarkerSize(1)
    theHistogram.GetXaxis().SetTitle("Global BX (Train starting at 0)")
    theHistogram.GetYaxis().SetTitle(yAxisTitle)
    theHistogram.GetYaxis().SetTitleOffset(1.45)

    theCanvas = ROOT.TCanvas(f'Canvas_{theHistogram.GetName()}', '')

    theHistogram.Draw('pe')
    
    label = ROOT.TLatex()
    label.SetNDC(True)
    label.DrawLatex(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary}}")

    theCanvas.SaveAs(f'{args.outputLocation}/{theHistogram.GetName()}.png')

def main(args):
    ROOT.gStyle.SetOptStat(0)
    theFile = ROOT.TFile(args.inputFile)
    listOfHistograms = [x.GetName() for x in theFile.GetListOfKeys()]
    
    overallHistograms = []
    otherHistograms = []
    for nameIndex in range(len(listOfHistograms)):
        if 'overallEvents' in listOfHistograms[nameIndex]:
            overallHistograms.append(listOfHistograms[nameIndex])
        else:
            otherHistograms.append(listOfHistograms[nameIndex])
    listOfHistograms = otherHistograms

    for name in overallHistograms:
        drawHistogram(getattr(theFile, name), 'Overall #L1As', args.outputLocation)

    for histogramName in listOfHistograms:
        bitSearch = re.search("(?<=Bit)[0-9]+", histogramName) #Find the bit number. It is a number preceded by the word "Bit"
        bitNo = int(bitSearch.group(0))+1 #The bit is accidentially listed as the index, which is one too low due to starting at 0.
        drawHistogram(getattr(theFile, histogramName), f'# of Bit {bitNo} Accepts', args.outputLocation)
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Script for drawing the output of the bunch position analysis script.')
    parser.add_argument('--inputFile', 
                        required=True, 
                        nargs='?',
                        help='ROOT output file of the analysis script to draw')
    parser.add_argument('--outputLocation',
                        default='./',
                        nargs='?',
                        help='Location to put the drawn histograms')

    args = parser.parse_args()

    main(args)
