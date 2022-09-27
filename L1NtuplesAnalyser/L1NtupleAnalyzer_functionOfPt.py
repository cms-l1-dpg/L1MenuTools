#!/usr/bin/env python3                                                                                
# ********************************************                                                                                                
# usage:                                                                                                                    
#    python3 L1NtupleAnalyzer_functionOfPt.py  
# ********************************************                                                                                              

print("------> Setting Environment")

import sys
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
# ZB2022 run357688 (4819637 events):
f_ZB2022 = "/afs/cern.ch/work/e/elfontan/private/L1TMenu/13p6TeVcollisions_ntupleProduction/CMSSW_12_4_0/src/L1MenuTools/rate-estimation/ntuple/EphemeralZBHLTPhysics_data_run357688_part0-8.csv"
# ZB2018 run325097 (672287 events):
f_ZB2018 = "/afs/cern.ch/work/e/elfontan/private/L1TMenu/13p6TeVcollisions_ntupleProduction/CMSSW_12_4_0/src/L1MenuTools/rate-estimation/ntuple/EphemeralZBHLTPhysics_data_run325097.csv"

# -----------------
# L1Ntuple branches
# -----------------
evt_tree  = TChain('l1EventTree/L1EventTree')
if not USE_EMUL:
    L1_tree = TChain('l1UpgradeTree/L1UpgradeTree')
else:
    L1_tree = TChain('l1UpgradeEmuTree/L1UpgradeTree')

list_ZB2022 = []
list_ZB2018 = []

with open(f_ZB2022) as file:
    while (line := file.readline().rstrip()):
        list_ZB2022.append(line)

with open(f_ZB2018) as file:
    while (line := file.readline().rstrip()):
        list_ZB2018.append(line)

  
for f in range(len(list_ZB2022)):
    evt_tree.Add(list_ZB2022[f])
    #print("INTERMEDIATE Nentries evt_tree: ", evt_tree.GetEntries())
    L1_tree.Add(list_ZB2022[f])

    if f == MAX_FILE: break    

nEvt_outLSrange = 0

n_SingleMu_thr = [0, 0, 0, 0, 0, 0, 0, 0] #8 
mu_pt = [20, 21, 22, 23, 24, 25, 26, 27] 

n_SingleEG_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
eg_pt = [32, 33, 34, 35, 36, 37, 38, 39, 40]

n_DoubleLooseIsoEG_thr = [0, 0, 0, 0, 0, 0, 0, 0] #7
doubleeg_pt = [20, 21, 22, 23, 24, 25, 26]

n_SingleTau_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] #11
tau_pt = [120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130]

n_DoubleIsoTau_thr = [0, 0, 0, 0, 0, 0, 0] #7
doubleisotau_pt = [30, 31, 32, 33, 34, 35, 36]

n_SingleJet_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
jet_pt = [120, 125, 130, 135, 140, 145, 150, 155, 160]

n_ETMHF_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] #9
etmhf_pt = [70, 72, 75, 78, 80, 85, 90, 95, 100]

# -------------------------
# Loop over events in TFile
# -------------------------
for iEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and iEvt > MAX_EVT: break
    if iEvt % PRT_EVT is 0: print("Event #", iEvt)
    
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
    i_mu_thr = [0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two mu objects in the same event passing the requirement
    for mu in range(preUGT_tree.nMuons):
        for thr in range(len(mu_pt)):
            if (preUGT_tree.muonEt[mu] >= mu_pt[thr] and preUGT_tree.muonQual[mu] >= 12 and preUGT_tree.muonBx[mu] == 0 and i_mu_thr[thr] == 0): #muonEt = 0.5*muonIEt
                i_mu_thr[thr] += 1
                n_SingleMu_thr[thr] += 1

    # -----------------
    #        EG       #
    # -----------------
    i_eg_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_doubleeg_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    for eg in range(preUGT_tree.nEGs):
        for thr in range(len(eg_pt)):
            if (preUGT_tree.egEt[eg] >= eg_pt[thr] and abs(preUGT_tree.egEta[eg]) <= 2.5 and preUGT_tree.egBx[eg] == 0 and i_eg_thr[thr] == 0): #egEt = 0.5*egIEt
                i_eg_thr[thr] += 1
                n_SingleEG_thr[thr] += 1

        for thr in range(len(doubleeg_pt)):
            if (preUGT_tree.egEt[eg] >= doubleeg_pt[thr] and abs(preUGT_tree.egEta[eg]) <= 2.1 and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_doubleeg_thr[thr] == 0): #egEt = 0.5*egIEt
                for eg2 in range(preUGT_tree.nEGs):
                    if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= doubleeg_pt[thr] and abs(preUGT_tree.egEta[eg2]) <= 2.1 and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #egEt = 0.5*egIEt
                        i_doubleeg_thr[thr] += 1
                        n_DoubleLooseIsoEG_thr[thr] += 1


    # -----------------
    #       TAU       #
    # -----------------
    i_tau_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_doubleisotau_thr = [0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    for tau in range(preUGT_tree.nTaus):
        for thr in range(len(tau_pt)):
            if (preUGT_tree.tauEt[tau] >= tau_pt[thr] and abs(preUGT_tree.tauEta[tau]) <= 2.1 and preUGT_tree.tauBx[tau] == 0 and i_tau_thr[thr] == 0): #tauEt = 0.5*tauIEt
                i_tau_thr[thr] += 1
                n_SingleTau_thr[thr] += 1

        for thr in range(len(doubleisotau_pt)):
            if (preUGT_tree.tauEt[tau] >= doubleisotau_pt[thr] and preUGT_tree.tauIso[tau] == 1 and preUGT_tree.tauBx[tau] == 0 and i_doubleisotau_thr[thr] == 0): #tauEt = 0.5*tauIEt
                for tau2 in range(preUGT_tree.nTaus):
                    if ((not tau2 == tau) and preUGT_tree.tauEt[tau2] >= doubleisotau_pt[thr] and preUGT_tree.tauIso[tau2] == 1 and preUGT_tree.tauBx[tau2] == 0):
                        i_doubleisotau_thr[thr] += 1
                        n_DoubleIsoTau_thr[thr] += 1


    # -----------------
    #       JET       #
    # -----------------
    i_jet_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    for jet in range(preUGT_tree.nJets):
        for thr in range(len(jet_pt)):
            if (preUGT_tree.jetEt[jet] >= jet_pt[thr] and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet_thr[thr] == 0): #jetEt = 0.5*tauIEt
                i_jet_thr[thr] += 1
                n_SingleJet_thr[thr] += 1


    # -----------------
    #       MET       #
    # -----------------
    i_etmhf_thr = [0, 0, 0, 0, 0, 0, 0, 0, 0] # Counter to avoid to count twice an event because of two met values in the same event passing the requirement
    for met in range(preUGT_tree.nSums):
        for thr in range(len(etmhf_pt)):
            if (not(preUGT_tree.sumType[met] == 8)): continue # 8 = 'kMissingEtHF'
            if (preUGT_tree.sumEt[met] >= etmhf_pt[thr] and preUGT_tree.sumBx[met] == 0 and i_etmhf_thr[thr] == 0): #sumEt = 0.5*sumIEt
                i_etmhf_thr[thr] += 1
                n_ETMHF_thr[thr] += 1


print("-------------------------")
print("-------- SUMMARY --------")
print("-------------------------")
print("FINAL Nentries evt_tree: ", evt_tree.GetEntries())
print("FINAL Nentries evt_tree in LS range: ", (evt_tree.GetEntries() - nEvt_outLSrange))
print("-------------------------")

print("# MU #")
print("mu pt thresholds = ", mu_pt)
print("n_SingleMu_Countings = ", n_SingleMu_thr)

print("# EG #")
print("eg pt thresholds = ", eg_pt)
print("n_SingleEG_Countings = ", n_SingleEG_thr)
print("doubleeg pt thresholds = ", doubleeg_pt)
print("n_DoubleLooseIsoEG_Countings = ", n_DoubleLooseIsoEG_thr)

print("# TAU #")
print("tau pt thresholds = ", tau_pt)
print("n_SingleTau_Countings = ", n_SingleTau_thr)
print("doubleisotau pt thresholds = ", doubleisotau_pt)
print("n_DoubleIsoTau_Countings = ", n_DoubleIsoTau_thr)

print("# JET #")
print("jet pt thresholds = ", jet_pt)
print("n_SingleJet_Countings = ", n_SingleJet_thr)

print("# MET #")
print("met thresholds = ", etmhf_pt)
print("n_ETMHF_Countings = ", n_ETMHF_thr)
