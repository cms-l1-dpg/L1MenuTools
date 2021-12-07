#!/usr/bin/env python

# ***************************************************
# usage: 
#    python LumiPOGweights_pileupComp_createHistos.py
#
# ***************************************************

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
        help='Specify the name of the output folder',
        default='PU_REWEIGHTING',
        type=str)
parser.add_argument('--mc',
        help='Run 3 MC file',
        default='/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/elfontan/condor/NuGun.root',
        type=str)
parser.add_argument('--histroot',
        help='Rootfile containing the Lumi POG Gaussian distribution stored in a histogram',
        default='h_LumiPOG_PU_gaussianStartOfTheFill.root',
        type=TFile)

args = parser.parse_args()
outputDir = args.outfolder
f_Run3MC = args.mc
f_lumiPOG_PU = args.histroot

##############
# OUTPUT DIR # 
##############
gSystem.Exec("mkdir -p " + outputDir)
print("Output directory created: ", outputDir)

##########
# HISTOS # 
##########
h_PU_lumiPOG = f_lumiPOG_PU.Get("h_PU_LumiPOG") 
h_nPV = TH1F('h_nPV', 'nPV', 100, 0, 100)
h_nPV_True = TH1F('h_nPV_True', 'nPV True', 100, 0, 100)

###########
# MC LOOP # 
###########
chains = {}
chains['Evt'] = []   # Event info                                                                                                                                           

chains['Evt'].append( ROOT.TChain('l1EventTree/L1EventTree') )
chains['Evt'][0].Add(f_Run3MC)
Evt_br = ROOT.L1Analysis.L1AnalysisEventDataFormat()
chains['Evt'][0].SetBranchAddress('Event', ROOT.AddressOf(Evt_br))

print("\nEntering loop over events for chain")
for jEvt in range(chains['Evt'][0].GetEntries()):
    chains['Evt'][0].GetEntry(jEvt)
    h_nPV.Fill(int(Evt_br.nPV))
    h_nPV_True.Fill(int(Evt_br.nPV_True))

h_PU_lumiPOG.Scale(h_nPV_True.Integral()/h_PU_lumiPOG.Integral())

# Save histograms in a root file 
print("Saving histograms into root file ...")
outfile_histos = TFile.Open(outputDir + "/" + "h_PUreweighting_LumiPOG_PU_gaussianStartOfTheFill.root", "RECREATE")
outfile_histos.cd()
h_PU_lumiPOG.Write()
h_nPV.Write()
h_nPV_True.Write()
outfile_histos.Close()
