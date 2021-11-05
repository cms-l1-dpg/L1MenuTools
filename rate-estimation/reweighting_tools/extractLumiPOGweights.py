#!/usr/bin/env python

# ***********************************
# usage: 
#    python extractLumiPOGweights.py
#
# ***********************************

import json, os, csv, re
import argparse
from array import *
import math
import ROOT
from ROOT import TFile, TH1, TH1F, TCanvas, gSystem, TAttFill, TLegend, TRatioPlot, TPad, TStyle, THStack, TPaveText, gStyle
from ROOT import kBlue, kRed, kBlack, kWhite, kAzure, kOrange
import pandas as pd
import numpy as np

ROOT.gROOT.SetBatch(True)


parser = argparse.ArgumentParser()
parser.add_argument('--outfolder',
        help='Specify details for the output folder',
        default='PU_REWEIGHTING',
        type=str)
parser.add_argument('--json',
        help='Name of the json file containing fraction of events as a function of pileup according to a certain Lumi POG model',
        default='pileup_distribution_2736_80mb_btb_PoissonStat_LumiWeighted_PU51.json',
        type=str)

args = parser.parse_args()
outputDir = args.outfolder
jsonfile = args.json

##############
# INPUT JSON # 
##############
f_jsonLumiPOG = open(jsonfile)
data = json.load(f_jsonLumiPOG)

v_frac = []
for f in data['fraction']:
    v_frac.append(f)

##############
# OUTPUT DIR # 
##############
gSystem.Exec("mkdir -p " + outputDir)
print "Output directory created!"

#############
# HISTOGRAM # 
#############
# Create the histo
h_PU_LumiPOG = TH1F('h_PU_LumiPOG', "Pileup gaussian distribution at the start of the fill", 100, 0, 100)

# Fill the histo
for b in range(0,100):
    h_PU_LumiPOG.Fill(b, v_frac[b])

# Save the histo in a root file 
print "Saving histo into root file ..."
outfile_histo = TFile.Open(outputDir + "/" + "h_LumiPOG_PU_gaussianStartOfTheFill.root", "RECREATE")
outfile_histo.cd()
h_PU_LumiPOG.Write()
outfile_histo.Close()
    
# Closing file
f_jsonLumiPOG.close()
