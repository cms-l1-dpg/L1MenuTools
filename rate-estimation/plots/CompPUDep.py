#!/usr/bin/env python
# encoding: utf-8

# File        : CompPUDep.py
# Author      : Zhenbin Wu
# Contact     : zhenbin.wu@gmail.com
# Date        : 2016 Aug 25
# Updated by  : Elisa Fontanesi (2021 Jul 1)
#
# Description : It allows to create rate vs PU plots starting csv files storing rate per PU information.
#               Fill 7005 is used as a test (runs 320673 and 320674). 
#               The csv file containing lumi and pileup information for these runs (run_lumi_fill7005.csv) is included here as an example
#               to test the basic running of the script.

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
        help='Name of the output folder',
        default='20210702_Run2ZB_fill7005_Prescale_2022_v0_1_1',
        type=str)
parser.add_argument('--csv',
        help='Name of the csv file containing rates as a function of pileup',
        default='Run3_ZeroBias_123_20212906_rateVSpileup_PU.csv',
        type=str)
args = parser.parse_args()

outfolder_details = args.outfolder
csvfile_rateVSpileup = args.csv

plot_min = 0
plot_max = 70
maxx = 75
maxy = 200
fit_min = 20
fit_max = 60

############ For 1866 bunches ############
#PU = 61		#61.00 for col 1.6
#PU = 57		#57.19 for col 1.5
#PU = 50		#49.56 for col 1.3
#PU = 38		#38.12 for col 1.0
############ For 2544 bunches ############
#PU = 62		#61.52 for col 2.2
#PU = 60
#PU = 56		#55.93 for col 2.0
#PU = 50		#50.33 for col 1.8
#PU = 40
##########################################

# Setup: Run 2 2018
freq = 11245.6
nBunches = 2544
unit = "kHz"
config = 2018

fitname = ROOT.TF1("fitname","[0]*x + [1]*x*x",0,70);
#fitname.SetParameters(0.1,0.001);
#fitname = "pol4"
#fitname = "expo"

pubins = np.arange(plot_min,plot_max, 1)
pumap = collections.defaultdict(list)
#print(type(pumap))

# The PatMap contains the names of the L1 seeds for which rates are plotted as a function of the pileup:
# by default, triggers used for physics are considered all together and are accessed with the label "L1APhysics"
PatMap = {  
    "L1APhysics" : "L1APhysics"}

def DrawPU(canvas, f, l1seed, count, key=None):
    list_fromDrawPU = [] # Making Python objects used here known within the DrawL1 function  
    df = f[(f.L1Seed == l1seed )]

    for i in range(0, len(pubins) -1):
        pumap[pubins[i]] = []
        pumap[pubins[i]].append(df[np.logical_and(df.PileUp > pubins[i], df.PileUp <= pubins[i+1])].Fired0.sum())
        pumap[pubins[i]].append(df[np.logical_and(df.PileUp > pubins[i], df.PileUp <= pubins[i+1])].Total.sum())

    x = []
    y = []
    yerr = []
    for k, v in pumap.iteritems():
        if v[1] != 0:
            x.append(k)
            if unit == "Hz":
                y.append(float(v[0])/v[1] * freq * nBunches )
                yerr.append( math.sqrt(float(v[0]))/v[1] * freq * nBunches )
            if unit == "kHz":
                y.append(float(v[0])/v[1] * freq * nBunches / 1000)
                yerr.append( math.sqrt(float(v[0]))/v[1] * freq * nBunches / 1000)

    ## Draw the plot
    graph = ROOT.TGraphErrors(len(x))
    list_fromDrawPU.append(graph)

    for i, (xx, yy, yee) in enumerate(zip(x, y, yerr)):
        # if yy != 0 and yee/yy >0.3:
            # continue
	#if i == 22 or i == 23 or i == 24:
	    # continue
        graph.SetPoint(i, xx, yy)
	#print (i,xx,yy,yee)
        #print "h1->SetBinContent( %d, %f);" %(xx,yy)
        #print "h1->SetBinError( %d, %f);" %(xx,yee)
        graph.SetPointError(i, 0, yee)

    graph.SetMarkerStyle(20+count)
    graph.SetMarkerSize(1.5)
    graph.SetMarkerColor(1+count)
    graph.SetLineColor(1+count)
    graph.SetLineWidth(2)
    tdrstyle.setTDRStyle()
    canvas.cd()
    canvas.Update()
    if count == 0:
        graph.Draw("AP")
        graph.GetXaxis().SetTitle("PileUp")
        graph.GetXaxis().SetLimits(plot_min, maxx)
        graph.GetYaxis().SetRangeUser(0, maxy)
        graph.GetYaxis().SetTitle("Rate (nBunches = %d) [%s]" % (nBunches, unit))
    else:
        graph.Draw("P")
    canvas.Update()
    leg.AddEntry(graph, l1seed, "p")

    result_ptr = graph.Fit(fitname, "SQ", "", fit_min, fit_max) 

    error_vec = result_ptr.GetConfidenceIntervals() 
    print ("error vec size = %d, fitted PU = %d" % (error_vec.size(), fit_max - fit_min + 1)) 
    if (fitname == "pol4" or fitname == "exp"):
        f2 = graph.GetFunction(fitname).Clone() 
    else:
        f2 = graph.GetFunction("fitname").Clone() 
    list_fromDrawPU.append(f2)

    f2.SetLineColor(1+count)
    f2.SetLineWidth(2)
    f2.SetRange(plot_min, fit_min)
    f2.SetLineStyle(5)
    #minChi = f2.GetChisquare() / f2.GetNDF()

    #fun = "Fit = %.2f + %.2f*x + %.3f*x^2" % (f2.GetParameter(0), f2.GetParameter(1), f2.GetParameter(2) )
    #fun = "Fit = %f*x + %f*x^2" % (f2.GetParameter(0), f2.GetParameter(1) )
    #print fun

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

    if (fitname == "pol4" or fitname == "exp"):
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
    #PatMap = {k:k for k in pd.unique(df.L1Seed)}

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
