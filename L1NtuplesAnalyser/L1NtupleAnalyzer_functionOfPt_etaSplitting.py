#!/usr/bin/env python3                                                                                
# ********************************************************                                                                                                
# usage:                                                                                                                    
#    python3 L1NtupleAnalyzer_functionOfPt_etaSplitting.py 
# ********************************************************                                                                                              

print("------> Setting Environment")

import sys, os
import argparse
import math
from subprocess import Popen,PIPE
from ROOT import *
import numpy as np
from array import *
import ROOT, array, random, copy
from ROOT import TCanvas, TFile, TH1, TH1F, TF1, gSystem

ROOT.gROOT.SetBatch()
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
gSystem.Load('libRooFit')

argparser = argparse.ArgumentParser(description='Parser used for non default arguments', formatter_class=argparse.ArgumentDefaultsHelpFormatter, add_help=True)
argparser.add_argument('--ntuples',
        help='Specify list of L1Ntuples',
        default='EphemeralZBHLTPhysics_data_run357688_part0-8.csv',
        type=str)
argparser.add_argument('--y',
        help='Specify the scenario under test',
        default='2022',
        type=str)
args = argparser.parse_args()
year = args.y
# ----------------------
# Configuration settings
# ----------------------
USE_EMUL = False    ## Use emulated L1T muons instead of unpacked
MAX_FILE = -1       ## Maximum number of input files (use "-1" for unlimited)
MAX_EVT  = -1       ## Maximum number of events to process (use "-1" for ALL)
PRT_EVT  = 1000     ## Print every Nth event

# ----------------
# List on ntuples:
# ----------------
if (year == '2022'):
    f_ZB2022 = args.ntuples 
    list_ZB2022 = []
    with open(f_ZB2022) as file:
        while (line := file.readline().rstrip()):
            list_ZB2022.append(line)
if (year == '2018'):
    f_ZB2018 = args.ntuples 
    list_ZB2018 = []
    with open(f_ZB2018) as file:
        while (line := file.readline().rstrip()):
            list_ZB2018.append(line)

# -----------------# 
# L1Ntuple branches
# -----------------
evt_tree  = TChain('l1EventTree/L1EventTree')
if not USE_EMUL:
    L1_tree = TChain('l1UpgradeTree/L1UpgradeTree')
else:
    L1_tree = TChain('l1UpgradeEmuTree/L1UpgradeTree')

if (year == '2022'):  
    for f in range(len(list_ZB2022)):
        evt_tree.Add(list_ZB2022[f])
        L1_tree.Add(list_ZB2022[f])
        if f == MAX_FILE: break    
if (year == '2018'):  
    for f in range(len(list_ZB2018)):
        evt_tree.Add(list_ZB2018[f])
        L1_tree.Add(list_ZB2018[f])
        if f == MAX_FILE: break    

nEvt_outLSrange = 0

n_SingleMu_thresholds = [20, 21, 22, 23, 24, 25, 26, 27] 
n_SingleMu_countings = [0, 0, 0, 0, 0, 0, 0, 0] #8 
n_SingleMu_countings_B = [0, 0, 0, 0, 0, 0, 0, 0] #8 
n_SingleMu_countings_O = [0, 0, 0, 0, 0, 0, 0, 0] #8 
n_SingleMu_countings_E = [0, 0, 0, 0, 0, 0, 0, 0] #8 

n_SingleEG_thresholds = [32, 33, 34, 35, 36, 37, 38, 39, 40]
n_SingleEG_countings = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
n_SingleEG_countings_B = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
n_SingleEG_countings_E = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9

n_DoubleLooseIsoEG_thresholds = [20, 21, 22, 23, 24, 25, 26]
n_DoubleLooseIsoEG_countings = [0, 0, 0, 0, 0, 0, 0] #7
n_DoubleLooseIsoEG_countings_B = [0, 0, 0, 0, 0, 0, 0] #7
n_DoubleLooseIsoEG_countings_E = [0, 0, 0, 0, 0, 0, 0] #7

n_SingleTau_countings = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] #11
n_SingleTau_thresholds = [120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130]

n_DoubleIsoTau_countings = [0, 0, 0, 0, 0, 0, 0] #7
n_DoubleIsoTau_thresholds = [30, 31, 32, 33, 34, 35, 36]

n_SingleJet_thresholds = [140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200]
n_SingleJet_countings = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0] #13
n_SingleJet_countings_B = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0] #13
n_SingleJet_countings_E = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0] #13
n_SingleJet_countings_F = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0] #13

n_ETMHF_countings = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
n_ETMHF_thresholds = [70, 72, 75, 78, 80, 85, 90, 95, 100]

# -------------------------
# Loop over events in TFile
# -------------------------
for iEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and iEvt > MAX_EVT: break
    if iEvt % PRT_EVT == 0: print("Event #", iEvt)
    
    evt_tree.GetEntry(iEvt)
    if not ((evt_tree.Event.run == 357688 and (evt_tree.Event.lumi >= 35 and evt_tree.Event.lumi < 85)) or (evt_tree.Event.run == 325097 and (evt_tree.Event.lumi >= 58 and evt_tree.Event.lumi < 94))):
        nEvt_outLSrange += 1
        continue

    L1_tree.GetEntry(iEvt)

    ## -----------------------------------
    ## pre UGT tree: muon and calo objects
    ## -----------------------------------
    preUGT_tree = L1_tree.L1Upgrade
  
    # ----------------- 
    #        MU       #
    # -----------------
    #-----------#
    # ETA range #
    # ----------#
    # MU: split per track finder i.e. |eta| in 0-0.83-1.24-2.4

    i_mu_thr = [0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two mu objects in the same event passing the requirement
    i_mu_thr_B = [0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two mu objects in the same event passing the requirement
    i_mu_thr_O = [0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two mu objects in the same event passing the requirement
    i_mu_thr_E = [0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two mu objects in the same event passing the requirement
    for mu in range(preUGT_tree.nMuons):
        for thr in range(len(n_SingleMu_thresholds)):
            if (preUGT_tree.muonEt[mu] >= n_SingleMu_thresholds[thr] and preUGT_tree.muonQual[mu] >= 12 and abs(preUGT_tree.muonBx[mu]) == 0 and i_mu_thr[thr] == 0): #muonEt = 0.5*muonIEt
                i_mu_thr[thr] += 1
                n_SingleMu_countings[thr] += 1
            if (preUGT_tree.muonEt[mu] >= n_SingleMu_thresholds[thr] and preUGT_tree.muonQual[mu] >= 12 and (abs(preUGT_tree.muonEta[mu]) <= 0.83) and abs(preUGT_tree.muonBx[mu]) == 0 and i_mu_thr_B[thr] == 0): #muonEt = 0.5*muonIEt
                i_mu_thr_B[thr] += 1
                n_SingleMu_countings_B[thr] += 1
            if (preUGT_tree.muonEt[mu] >= n_SingleMu_thresholds[thr] and preUGT_tree.muonQual[mu] >= 12 and (abs(preUGT_tree.muonEta[mu]) > 0.83 and abs(preUGT_tree.muonEta[mu]) <= 1.24) and preUGT_tree.muonBx[mu] == 0 and i_mu_thr_O[thr] == 0): #muonEt = 0.5*muonIEt
                i_mu_thr_O[thr] += 1
                n_SingleMu_countings_O[thr] += 1
            if (preUGT_tree.muonEt[mu] >= n_SingleMu_thresholds[thr] and preUGT_tree.muonQual[mu] >= 12 and (abs(preUGT_tree.muonEta[mu]) > 1.24 and abs(preUGT_tree.muonEta[mu]) <= 2.4) and preUGT_tree.muonBx[mu] == 0 and i_mu_thr_E[thr] == 0): #muonEt = 0.5*muonIEt
                i_mu_thr_E[thr] += 1
                n_SingleMu_countings_E[thr] += 1

    # -----------------
    #        EG       #
    # -----------------
    #-----------#
    # ETA range #
    # ----------#
    # EG: 0-1.479-2.5
    i_eg_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_eg_thr_B = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_eg_thr_E = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_doubleeg_thr = [0, 0, 0, 0, 0, 0, 0]
    i_doubleeg_thr_B = [0, 0, 0, 0, 0, 0, 0]
    i_doubleeg_thr_E = [0, 0, 0, 0, 0, 0, 0]
    for eg in range(preUGT_tree.nEGs):
        for thr in range(len(n_SingleEG_thresholds)):
            if (preUGT_tree.egEt[eg] >= n_SingleEG_thresholds[thr] and abs(preUGT_tree.egEta[eg]) <= 2.5 and preUGT_tree.egBx[eg] == 0 and i_eg_thr[thr] == 0): #egEt = 0.5*egIEt
                i_eg_thr[thr] += 1
                n_SingleEG_countings[thr] += 1
            if (preUGT_tree.egEt[eg] >= n_SingleEG_thresholds[thr] and abs(preUGT_tree.egEta[eg]) <= 1.479 and preUGT_tree.egBx[eg] == 0 and i_eg_thr_B[thr] == 0): #egEt = 0.5*egIEt
                i_eg_thr_B[thr] += 1
                n_SingleEG_countings_B[thr] += 1
            if (preUGT_tree.egEt[eg] >= n_SingleEG_thresholds[thr] and (abs(preUGT_tree.egEta[eg]) > 1.479 and abs(preUGT_tree.egEta[eg]) <= 2.5) and preUGT_tree.egBx[eg] == 0 and i_eg_thr_E[thr] == 0): #egEt = 0.5*egIEt
                i_eg_thr_E[thr] += 1
                n_SingleEG_countings_E[thr] += 1

        for thr in range(len(n_DoubleLooseIsoEG_thresholds)):
            if (preUGT_tree.egEt[eg] >= n_DoubleLooseIsoEG_thresholds[thr] and abs(preUGT_tree.egEta[eg]) <= 2.1 and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_doubleeg_thr[thr] == 0): #egEt = 0.5*egIEt
                for eg2 in range(preUGT_tree.nEGs):
                    if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= n_DoubleLooseIsoEG_thresholds[thr] and abs(preUGT_tree.egEta[eg2]) <= 2.1 and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #egEt = 0.5*egIEt
                        i_doubleeg_thr[thr] += 1
                        n_DoubleLooseIsoEG_countings[thr] += 1

            if (preUGT_tree.egEt[eg] >= n_DoubleLooseIsoEG_thresholds[thr] and abs(preUGT_tree.egEta[eg]) <= 1.479 and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_doubleeg_thr_B[thr] == 0): #egEt = 0.5*egIEt
                for eg2 in range(preUGT_tree.nEGs):
                    if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= n_DoubleLooseIsoEG_thresholds[thr] and abs(preUGT_tree.egEta[eg2]) <= 1.479 and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #egEt = 0.5*egIEt
                        i_doubleeg_thr_B[thr] += 1
                        n_DoubleLooseIsoEG_countings_B[thr] += 1

            if (preUGT_tree.egEt[eg] >= n_DoubleLooseIsoEG_thresholds[thr] and (abs(preUGT_tree.egEta[eg]) > 1.479 and abs(preUGT_tree.egEta[eg]) <= 2.1) and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_doubleeg_thr_E[thr] == 0): #egEt = 0.5*egIEt
                for eg2 in range(preUGT_tree.nEGs):
                    if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= n_DoubleLooseIsoEG_thresholds[thr] and (abs(preUGT_tree.egEta[eg]) > 1.479 and abs(preUGT_tree.egEta[eg]) <= 2.1) and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #egEt = 0.5*egIEt
                        i_doubleeg_thr_E[thr] += 1
                        n_DoubleLooseIsoEG_countings_E[thr] += 1


    # -----------------
    #       TAU       #
    # -----------------
    i_tau_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_doubleisotau_thr = [0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    for tau in range(preUGT_tree.nTaus):
        for thr in range(len(n_SingleTau_thresholds)):
            if (preUGT_tree.tauEt[tau] >= n_SingleTau_thresholds[thr] and abs(preUGT_tree.tauEta[tau]) <= 2.1 and preUGT_tree.tauBx[tau] == 0 and i_tau_thr[thr] == 0): #tauEt = 0.5*tauIEt
                i_tau_thr[thr] += 1
                n_SingleTau_countings[thr] += 1

        for thr in range(len(n_DoubleIsoTau_thresholds)):
            if (preUGT_tree.tauEt[tau] >= n_DoubleIsoTau_thresholds[thr] and abs(preUGT_tree.tauEta[tau]) <= 2.1 and preUGT_tree.tauIso[tau] == 1 and preUGT_tree.tauBx[tau] == 0 and i_doubleisotau_thr[thr] == 0): #tauEt = 0.5*tauIEt
                for tau2 in range(preUGT_tree.nTaus):
                    if ((not tau2 == tau) and preUGT_tree.tauEt[tau2] >= n_DoubleIsoTau_thresholds[thr] and abs(preUGT_tree.tauEta[tau2]) <= 2.1 and preUGT_tree.tauIso[tau2] == 1 and preUGT_tree.tauBx[tau2] == 0):
                        i_doubleisotau_thr[thr] += 1
                        n_DoubleIsoTau_countings[thr] += 1


    # -----------------
    #       JET       #
    # -----------------
    #-----------#
    # ETA range #
    # ----------#
    # JET: 0-1.3-2.5-3-5
    i_jet_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet_thr_B = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet_thr_E = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet_thr_F = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    for jet in range(preUGT_tree.nJets):
        for thr in range(len(n_SingleJet_thresholds)):
            if (preUGT_tree.jetEt[jet] >= n_SingleJet_thresholds[thr] and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet_thr[thr] == 0): #jetEt = 0.5*tauIEt
                i_jet_thr[thr] += 1
                n_SingleJet_countings[thr] += 1
            if (preUGT_tree.jetEt[jet] >= n_SingleJet_thresholds[thr] and abs(preUGT_tree.jetEta[jet]) <= 1.3 and preUGT_tree.jetBx[jet] == 0 and i_jet_thr_B[thr] == 0): #jetEt = 0.5*tauIEt
                i_jet_thr_B[thr] += 1
                n_SingleJet_countings_B[thr] += 1
            if (preUGT_tree.jetEt[jet] >= n_SingleJet_thresholds[thr] and (abs(preUGT_tree.jetEta[jet]) > 1.3 and abs(preUGT_tree.jetEta[jet]) <= 2.5) and preUGT_tree.jetBx[jet] == 0 and i_jet_thr_E[thr] == 0): #jetEt = 0.5*tauIEt
                i_jet_thr_E[thr] += 1
                n_SingleJet_countings_E[thr] += 1
            if (preUGT_tree.jetEt[jet] >= n_SingleJet_thresholds[thr] and (abs(preUGT_tree.jetEta[jet]) > 2.5 and abs(preUGT_tree.jetEta[jet]) <= 3.5) and preUGT_tree.jetBx[jet] == 0 and i_jet_thr_F[thr] == 0): #jetEt = 0.5*tauIEt
                i_jet_thr_F[thr] += 1
                n_SingleJet_countings_F[thr] += 1


    # -----------------
    #       MET       #
    # -----------------
    i_etmhf_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two met values in the same event passing the requirement
    for met in range(preUGT_tree.nSums):
        for thr in range(len(n_ETMHF_thresholds)):
            if (not(preUGT_tree.sumType[met] == 8)): continue # 8 = 'kMissingEtHF'
            if (preUGT_tree.sumEt[met] >= n_ETMHF_thresholds[thr] and preUGT_tree.sumBx[met] == 0 and i_etmhf_thr[thr] == 0): #sumEt = 0.5*sumIEt
                i_etmhf_thr[thr] += 1
                n_ETMHF_countings[thr] += 1


print("-------------------------")
print("-------- SUMMARY --------")
print("-------------------------")
print("FINAL Nentries evt_tree: ", evt_tree.GetEntries())
print("FINAL Nentries evt_tree in LS range: ", (evt_tree.GetEntries() - nEvt_outLSrange))
print("-------------------------")

print("# MU #")
print("mu pt thresholds = ", n_SingleMu_thresholds)
print("n_SingleMu_Countings = ", n_SingleMu_countings)
print("n_SingleMu_Countings_B = ", n_SingleMu_countings_B)
print("n_SingleMu_Countings_O = ", n_SingleMu_countings_O)
print("n_SingleMu_Countings_E = ", n_SingleMu_countings_E)

print("# EG #")
print("eg pt thresholds = ", n_SingleEG_thresholds)
print("n_SingleEG_Countings = ", n_SingleEG_countings)
print("n_SingleEG_Countings_B = ", n_SingleEG_countings_B)
print("n_SingleEG_Countings_E = ", n_SingleEG_countings_E)
print("doubleeg pt thresholds = ", n_DoubleLooseIsoEG_thresholds)
print("n_DoubleLooseIsoEG_Countings = ", n_DoubleLooseIsoEG_countings)
print("n_DoubleLooseIsoEG_Countings_B = ", n_DoubleLooseIsoEG_countings_B)
print("n_DoubleLooseIsoEG_Countings_E = ", n_DoubleLooseIsoEG_countings_E)

print("# TAU #")
print("tau pt thresholds = ", n_SingleTau_thresholds)
print("n_SingleTau_Countings = ", n_SingleTau_countings)
print("doubleisotau pt thresholds = ", n_DoubleIsoTau_thresholds)
print("n_DoubleIsoTau_Countings = ", n_DoubleIsoTau_countings)

print("# JET #")
print("jet pt thresholds = ", n_SingleJet_thresholds)
print("n_SingleJet_Countings = ", n_SingleJet_countings)
print("n_SingleJet_Countings_B = ", n_SingleJet_countings_B)
print("n_SingleJet_Countings_E = ", n_SingleJet_countings_E)
print("n_SingleJet_Countings_F = ", n_SingleJet_countings_F)

print("# MET #")
print("met thresholds = ", n_ETMHF_thresholds)
print("n_ETMHF_Countings = ", n_ETMHF_countings)
