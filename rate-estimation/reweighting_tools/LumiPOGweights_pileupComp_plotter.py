#!/usr/bin/env python

# **********************************************
# usage: 
#    python LumiPOGweights_pileupComp_plotter.py
#
# **********************************************
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
parser.add_argument('--histos',
        help='Rootfile containing Lumi POG and Run 3 MC pilupe distributions',
        default='h_PUreweighting_LumiPOG_PU_gaussianStartOfTheFill.root',
        type=TFile)

args = parser.parse_args()
outputDir = args.outfolder
h_input = args.histos

###############
# READ HISTOS # 
###############
print 'Reading file', h_input.GetName(),'...'

h_PU_lumiPOG = h_input.Get("h_PU_LumiPOG")
h_nPV = h_input.Get("h_nPV")
h_nPV_True = h_input.Get("h_nPV_True")
h_weight_nPV_True = TH1F ("h_weight_nPV_True", "h_weight_nPV_True", h_nPV_True.GetSize() - 2, h_nPV_True.GetXaxis().GetXmin(), h_nPV_True.GetXaxis().GetXmax())
l_weights_nPV_True = []

for bin in range (h_nPV_True.GetSize() - 2):
    if (h_nPV_True.GetBinContent(bin) == 0):
        h_weight_nPV_True.SetBinContent(bin, 0)
        h_weight_nPV_True.SetBinError  (bin, 0)
        l_weights_nPV_True.append(0)
    elif (h_PU_lumiPOG.GetBinContent(bin) == 0 or h_PU_lumiPOG.GetBinContent(bin) < 100):
        h_weight_nPV_True.SetBinContent(bin, 0)
        h_weight_nPV_True.SetBinError  (bin, 0)
        l_weights_nPV_True.append(0)
    else:
        ratio_nPV_True = h_PU_lumiPOG.GetBinContent(bin)/h_nPV_True.GetBinContent(bin)
        h_weight_nPV_True.SetBinContent(bin, float(ratio_nPV_True))
        h_weight_nPV_True.SetBinError  (bin, 1./(h_PU_lumiPOG.GetBinContent(bin))**0.5)
        l_weights_nPV_True.append(ratio_nPV_True)

print "\nnPV_True weights:"
print(l_weights_nPV_True)
 
# Save histograms in a root file                                                                                                                   
print "Saving histo weights into root file ..."
outfile_weights = TFile.Open(outputDir + "/" + "h_weights_puReweighting_LumiPOG_PU_gaussianStartOfTheFill.root", "RECREATE")
outfile_weights.cd()
h_weight_nPV_True.Write()
outfile_weights.Close()

c_nPV_True = TCanvas ("c_nPV_True", "c_nPV_True", 1000, 800)
c_nPV_True.cd()
gStyle.SetOptStat(000)
h_weight_nPV_True.SetTitle("Run 3 pileup weights")
h_weight_nPV_True.GetXaxis().SetTitle("nPV_True")
h_weight_nPV_True.GetYaxis().SetTitle("Run 3 model / nPV_True")
h_weight_nPV_True.GetYaxis().SetLabelFont(43)
h_weight_nPV_True.GetYaxis().SetLabelSize(15)
h_weight_nPV_True.GetXaxis().SetLabelFont(43)
h_weight_nPV_True.GetXaxis().SetLabelSize(15)
h_weight_nPV_True.SetLineWidth(2)
h_weight_nPV_True.SetLineColor(kOrange-3)
h_weight_nPV_True.Draw()
c_nPV_True.SaveAs(outputDir + "/" + "weights_nPVTrue_Run3PU.pdf")
c_nPV_True.SaveAs(outputDir + "/" + "weights_nPVTrue_Run3PU.png")

#########
# PLOTS #
#########
h_PU_lumiPOG.SetMarkerColor(kBlack)
h_PU_lumiPOG.SetLineColor(kBlack)
h_PU_lumiPOG.SetMarkerStyle(20)
h_PU_lumiPOG.SetMarkerSize(0.6)
h_nPV.SetFillColor(kAzure-2)
h_nPV.SetLineColor(kBlack)
h_nPV_True.SetFillColor(kOrange-3)
h_nPV_True.SetLineColor(kBlack)

c2 = TCanvas("c2", "c2", 800, 800)

gStyle.SetOptStat(0)
h_nPV_True.SetTitle("")
h_nPV_True.GetXaxis().SetLabelFont(43)
h_nPV_True.GetXaxis().SetLabelSize(15)
h_nPV_True.GetXaxis().SetTitle("nPV_True")
h_nPV_True.GetYaxis().SetLabelFont(43)
h_nPV_True.GetYaxis().SetLabelSize(15)
h_nPV_True.GetYaxis().SetTitle("Events (A.U.)")
h_nPV_True.GetYaxis().SetRangeUser(0, 38000)
h_nPV_True.Draw("histo") 
h_PU_lumiPOG.Draw("sameP hist")
        
# Legend
l2 = TLegend(0.42,0.78,0.9,0.9)
l2.AddEntry(h_nPV_True,"nPV_True in NuGun Run 3 MC", "f")
l2.AddEntry(h_PU_lumiPOG,"Pileup distributionin Run 3 LumiPOG models","p")
l2.SetFillColor(kWhite)
l2.SetTextFont(43)
l2.SetTextSize(20)
l2.Draw()

c2.SaveAs(outputDir + "/" + "puDistributios_nPVTrue_Run3PU.pdf")
c2.SaveAs(outputDir + "/" + "puDistributios_nPVTrue_Run3PU.png")
