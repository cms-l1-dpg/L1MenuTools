#!/usr/bin/env Python
# encoding: utf-8

# File        : CompPUDep.py
# Author      : Zhenbin Wu
# Contact     : zhenbin.wu@gmail.com
# Date        : 2016 Aug 25
# Updated by  : Elisa Fontanesi (2021 Jul 1)
#
# Description : It allows to create rate vs PU plots starting csv files storing rate per PU information.                                                                    
#               Two csv file containing lumi and pileup information (for ZeroBias and Run 3 MC) are provided to test the basic running of the script.                         
#               Fill 7005 is used as a test using ZeroBias (runs 320673 and 320674) with the corresponding csv file.                                                           
#               EXAMPLE COMMAND: python CompPUDep.py --outfolder Run3MC_Prescale_2022_v0_1_1 --csv Run3_ZeroBias_123_20212906_rateVSpileup_PU.csv  --reweightRun3 True --seed L1APhysics

import pandas as pd
import numpy as np
import glob
import math
import ROOT
import collections
import argparse
import os
import re
import tdrstyle
from matplotlib import pyplot as plt
from Config import DualMap, S1S2Map, S2S1Map

ROOT.gROOT.SetBatch(True)

parser = argparse.ArgumentParser()
parser.add_argument('--outfolder',
        help='Specify details for the output folder',
        default='Run3MC_Prescale_2022_v0_1_1',
        type=str)
parser.add_argument('--csv',
        help='Name of the csv file containing rates as a function of pileup',
        default='Run3_ZeroBias_123_20212906_rateVSpileup_PU.csv',
        type=str)
parser.add_argument('--seed',
        help='Names of the L1 seeds for which rates are plotted as a function of the pileup',
        nargs="*",
        default=['L1APhysics'],
        type=str)
parser.add_argument('--reweight2018',
        help='Specify if the 2018 reweighting is performed or not',
        default=False,
        type=bool)
parser.add_argument('--reweightRun3',
        help='Specify if the Run 3 reweighting is performed or not',
        default=False,
        type=bool)

args = parser.parse_args()
outfolder_details = args.outfolder
csvfile_rateVSpileup = args.csv
seedList = args.seed
isReweight2018 = args.reweight2018
isReweightRun3 = args.reweightRun3

print("List of L1 seeds considered: %r" % args.seed)

plot_min = 0
plot_max = 70
maxx = 75
maxy = 200
fit_min = 20
fit_max = 60

# Setup: Run 2 2018
freq = 11245.6
nBunches = 2544
unit = "kHz"

fitname = ROOT.TF1("fitname","[0]*x + [1]*x*x",0,70);
fitname.SetParameters(0.1,0.001);
#fitname = "pol4"
#fitname = "expo"

pubins = np.arange(plot_min,plot_max, 1)
pumap = collections.defaultdict(list)
#print(type(pumap))

# The PatMap contains the names of the L1 seeds for which rates are plotted as a function of the pileup:
# by default, triggers used for physics are considered all together and are accessed with the label "L1APhysics"
#PatMap = {seedName : seedName}
PatMap = {seedName:seedName for seedName in seedList}

if (isReweight2018 or isReweightRun3):
    isReweight = True

def ExtractPileUpWeight(pu):
    # WEIGHTS obtained as the ratio between the 2018 pileup profile and the Run 3 MC nPV_True distribution: 
    # see here -> https://elfontan.web.cern.ch/elfontan/Run3_MENU/PileupReweighting/weights_nPV_True.png.
    # A weight equal to 0 is set in bins where the number of events in data is less than 100.
    h_weights_2018 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7805557154345164, 1.8060490539875227, 1.8429444310772023, 1.8847969284653239, 1.9023685309193448, 1.9237248769457562, 1.9052231983237566, 1.9323838791307304, 1.9085059563417344, 1.8603791701197574, 1.7603108386627149, 1.6756647663900748, 1.5370617605401784, 1.3999688210755514, 1.259781632746541, 1.113578483437248, 0.9458259616433159, 0.8004237934065365, 0.67859967863879, 0.5416839609316458, 0.4312760960624466, 0.3410534695481281, 0.2670644366287844, 0.20005146208632094, 0.15028893320205652, 0.10953206021410954, 0.08057347625153384, 0.05889674651099118, 0.04287951951844378, 0.03136484854119283, 0.022045797907650135, 0.015338620880659437, 0.011042294804776033, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    h_weights_Run3 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.11768066610316973, 0.15659943608123963, 0.2062485504625545, 0.268258220098052, 0.33984423319498697, 0.42640469375359685, 0.5188836993090095, 0.6415574459968488, 0.7679292883090271, 0.9038989080521145, 1.0312039036982403, 1.184185989240607, 1.3135745255235602, 1.452683767877649, 1.595818724793107, 1.7332340493078218, 1.8220240159036707, 1.9232725225809801, 2.0500456898039943, 2.0735607093466237, 2.107312361581553, 2.1410569727220117, 2.1657353540686484, 2.1041608280359783, 2.055486270892838, 1.9497829996617755, 1.865648229655712, 1.770365292692509, 1.6681387392525582, 1.573366538275714, 1.4204671249736036, 1.2648609312008994, 1.1619530797268465, 1.032957960295197, 0.9106977649201128, 0.8043353997543611, 0.7051914455399683, 0.5998090800948392, 0.522684322280154, 0.43992380960221283, 0.36786673008123166, 0.31065622561951234, 0.2568458813937691, 0.20984708112111544, 0.1707654623586224, 0.13792878579949355, 0.11263581096823445, 0.08921848542040954, 0.07125932229597162, 0.05617010906326074, 0.04314936365485068, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    if (isReweight2018):
        weight = h_weights_2018[pu]
    elif (isReweightRun3):
        weight = h_weights_Run3[pu]
    return weight

def DrawPU(canvas, f, l1seed, count, key=None):
    list_fromDrawPU = [] # Making Python objects used here known within the DrawL1 function  
    df = f[(f.L1Seed == l1seed )]
    #print "\n Dataframe:\n", df
    for i in range(0, len(pubins) -1):
        pumap[pubins[i]] = []
        pumap[pubins[i]].append(df[np.logical_and(df.PileUp >= pubins[i], df.PileUp < pubins[i+1])].Fired0.sum() )
        pumap[pubins[i]].append(df[np.logical_and(df.PileUp >= pubins[i], df.PileUp < pubins[i+1])].Total.sum() )

    x = []
    y = []
    yerr = []
    for k, v in pumap.iteritems():
        if v[1] != 0:
            x.append(k)
            if unit == "Hz":
                y.append(float(v[0])/v[1] * freq * nBunches )
                if (isReweight):
                    w = ExtractPileUpWeight(k)
                    yerr.append( math.sqrt(float(v[0])*w)/v[1] * freq * nBunches )
                else:
                    yerr.append( math.sqrt(float(v[0]))/v[1] * freq * nBunches )                    
            if unit == "kHz":
                y.append(float(v[0])/v[1] * freq * nBunches / 1000)
                if (isReweight):
                    w = ExtractPileUpWeight(k)
                    yerr.append( math.sqrt(float(v[0])*w)/v[1] * freq * nBunches / 1000 )
                else:
                    yerr.append( math.sqrt(float(v[0]))/v[1] * freq * nBunches / 1000 )

    ## Draw the plot
    graph = ROOT.TGraphErrors(len(x))
    list_fromDrawPU.append(graph)

    for i, (xx, yy, yee) in enumerate(zip(x, y, yerr)):
        graph.SetPoint(i, xx, yy)
        graph.SetPointError(i, 0, yee)
    
    graph.SetMarkerStyle(20+count)
    graph.SetMarkerSize(1.5)
    graph.SetMarkerColor(1+count)
    graph.SetLineColor(1+count)
    graph.SetLineWidth(2)
    tdrstyle.setTDRStyle()
    canvas.cd()

    if count == 0:
        graph.Draw("AP")
        graph.GetXaxis().SetTitle("PileUp")
        graph.GetXaxis().SetLimits(plot_min, maxx)
        graph.GetYaxis().SetRangeUser(0, maxy)
        graph.GetYaxis().SetTitle("Rate (nBunches = %d) [%s]" % (nBunches, unit))
    else:
        graph.Draw("P")

    leg.AddEntry(graph, l1seed, "p")
    
    result_ptr = graph.Fit(fitname, "SQ", "", fit_min, fit_max) 
    checkVariable = result_ptr.Get() # TFitResult * TFitResultPtr::Get ( ) const => Return contained pointer 
    if (not(checkVariable)):
        print("TFitResult is empty, returned null pointer: checkVariable =", checkVariable)
    else:
        error_vec = result_ptr.GetConfidenceIntervals() 
    
        print ("error vec size = %d, fitted PU = %d" % (error_vec.size(), fit_max - fit_min + 1)) 
        if (fitname == "pol4" or fitname == "expo"):
            f2 = graph.GetFunction(fitname).Clone() 
        else:
            f2 = graph.GetFunction("fitname").Clone() 
        list_fromDrawPU.append(f2)
        
        f2.SetLineColor(1+count)
        f2.SetLineWidth(2)
        f2.SetRange(plot_min, fit_min)
        f2.SetLineStyle(5)
        #minChi = f2.GetChisquare() / f2.GetNDF()
        
        f2.Draw("same")
        f2_2 = f2.Clone("dashline2")
        list_fromDrawPU.append(f2_2)
        f2_2.SetRange(fit_max, plot_max)
        f2_2.Draw("same")
        key = ""
        
        if key is not None:
            tex = ROOT.TLatex(0.2, 0.85, key)
            tex.SetNDC()
            tex.SetTextFont(61)
            tex.SetTextSize(0.055)
            tex.SetTextColor(ROOT.kGreen+2)
            tex.SetLineWidth(2)
            tex.Draw()            
    return list_fromDrawPU
            
def DrawL1(key, pattern):
    c1.Clear()
    leg.Clear()

    inputlist = []
    pat = re.compile('^%s$' % pattern)
    for x in [x for x in pd.unique(df.L1Seed)]:
        if pat.match(x):
            inputlist.append(x)
    #print key, " : ",  inputlist
    print key,

    for i, seed in enumerate(inputlist):
        pbuf = DrawPU(c1, df, seed, i)
    leg.Draw()

    # Add vertical lines 
    #v_line = ROOT.TLine(50, 0, 50, maxy)
    #v_line.SetLineColor(2)
    #v_line.SetLineWidth(2)
    #v_line.Draw()

    c1.SetGrid()
    box = ROOT.TBox(10, 8, 70, 12)
    box.SetFillColor(38)
    box.SetFillStyle(3002)
    c1.Update()

    if (fitname == "pol4" or fitname == "expo"):
        c1.SaveAs("Plots_%s/%s_%s.png" % (outfolder_details, key, fitname))
        c1.SaveAs("Plots_%s/%s_%s.pdf" % (outfolder_details, key, fitname))
        c1.SaveAs("Plots_%s/%s_%s.root" % (outfolder_details, key, fitname))
    else:
        c1.SaveAs("Plots_%s/%s.png" % (outfolder_details, key))
        c1.SaveAs("Plots_%s/%s.pdf" % (outfolder_details, key))
        c1.SaveAs("Plots_%s/%s.root" % (outfolder_details, key))

if __name__ == "__main__":
    allfiles = glob.glob(csvfile_rateVSpileup)
    if not os.path.exists("Plots_%s" % (outfolder_details)):
        os.mkdir("Plots_%s" % (outfolder_details))
    
    df = pd.DataFrame()
    flist = [ ]
    for file_ in allfiles:
        df_ = pd.read_csv(file_, index_col=None, header=0)
        flist.append(df_)
    df = pd.concat(flist)

    ## PatMap can be redifined to take into account each L1Seed present in the dataframe
    if (seedList[0] == 'all'):
        PatMap = {k:k for k in pd.unique(df.L1Seed)}

    ROOT.gStyle.SetOptStat(000000000)
    tdrstyle.setTDRStyle()
    c1 = ROOT.TCanvas("fd","Fdf", 1200, 1000)
    leg = ROOT.TLegend(0.15,0.7,0.45,0.75)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextFont(62)
    leg.SetTextSize(0.05)
    for k, v in PatMap.items():
        DrawL1(k, v)
