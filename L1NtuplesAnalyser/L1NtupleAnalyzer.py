#!/usr/bin/env python3                                                                                
# ********************************                                                                                                
# usage:                                                                                                                    
#    python3 L1NtupleAnalyzer.py
# ********************************                                                                                              

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
# ZB2022 run357688 (5150338 events; 50 LS in LS = [35,84] for PU=52: 939081 events):
f_ZB2022 = "EphemeralZBHLTPhysics_data_run357688_part0-8.csv"
# ZB2018 run325097 (626090 events; 36 LS in LS = [58,93] for PU=52: 469101 events):
f_ZB2018 = "EphemeralZBHLTPhysics_data_run325097.csv"


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
n_SingleMu22 = 0
n_SingleMu25 = 0
n_SingleEG36_er2p5 = 0
n_SingleEG38_er2p5 = 0
n_SingleEG40_er2p5 = 0
n_DoubleLooseIsoEG22 = 0
n_DoubleLooseIsoEG24 = 0
n_SingleTau120_er2p1 = 0
n_SingleTau130_er2p1 = 0
n_DoubleIsoTau32 = 0
n_DoubleIsoTau34 = 0
n_DoubleIsoTau35 = 0
n_SingleJet120er2p5 = 0
n_SingleJet140er2p5 = 0
n_SingleJet160er2p5 = 0
n_SingleJet180er2p5 = 0
n_ETMHF70 = 0
n_ETMHF80 = 0
n_ETMHF90 = 0
n_ETMHF100 = 0

# -------------------------
# Loop over events in TFile
# -------------------------
for iEvt in range(evt_tree.GetEntries()):
    if MAX_EVT > 0 and iEvt > MAX_EVT: break
    if iEvt % PRT_EVT is 0: print("Event #", iEvt)
    
    evt_tree.GetEntry(iEvt)
    if not ((evt_tree.Event.run == 357688 and (evt_tree.Event.lumi > 35 and evt_tree.Event.lumi < 85)) or (evt_tree.Event.run == 325097 and (evt_tree.Event.lumi > 58 and evt_tree.Event.lumi < 93))):
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
    i_mu22 = 0 # Counter to avoid to count twice an event because of two muons in the same event passing the requirement
    i_mu25 = 0 # Counter to avoid to count twice an event because of two muons in the same event passing the requirement
    for mu in range(preUGT_tree.nMuons):
        if (preUGT_tree.muonEt[mu] >= 22 and preUGT_tree.muonQual[mu] >= 12 and preUGT_tree.muonBx[mu] == 0 and i_mu22 == 0): #muonEt = 0.5*muonIEt
            i_mu22 += 1
            n_SingleMu22 += 1
            #print("muonEt = ", preUGT_tree.muonEt[mu])
            #print("muonIEt = ", preUGT_tree.muonIEt[mu])
        if (preUGT_tree.muonEt[mu] >= 25 and preUGT_tree.muonQual[mu] >= 12 and preUGT_tree.muonBx[mu] == 0 and i_mu25 == 0): #muonEt = 0.5*muonIEt
            i_mu25 += 1
            n_SingleMu25 += 1

    # -----------------
    #        EG       #
    # -----------------
    i_isoeg22 = 0 # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_isoeg24 = 0 # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_eg36 = 0 # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_eg38 = 0 # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    i_eg40 = 0 # Counter to avoid to count twice an event because of two eg objects in the same event passing the requirement
    for eg in range(preUGT_tree.nEGs):

        if (preUGT_tree.egEt[eg] >= 36 and abs(preUGT_tree.egEta[eg]) <= 2.5 and preUGT_tree.egBx[eg] == 0 and i_eg36 == 0): #egEt = 0.5*egIEt
            i_eg36 += 1
            n_SingleEG36_er2p5 += 1
            #print("egEt = ", preUGT_tree.egEt[eg])
            #print("egIEt = ", preUGT_tree.egIEt[eg])
        if (preUGT_tree.egEt[eg] >= 38 and abs(preUGT_tree.egEta[eg]) <= 2.5 and preUGT_tree.egBx[eg] == 0 and i_eg38 == 0): #egEt = 0.5*egIEt
            i_eg38 += 1
            n_SingleEG38_er2p5 += 1
        if (preUGT_tree.egEt[eg] >= 40 and abs(preUGT_tree.egEta[eg]) <= 2.5 and preUGT_tree.egBx[eg] == 0 and i_eg40 == 0): #egEt = 0.5*egIEt
            i_eg40 += 1
            n_SingleEG40_er2p5 += 1

        if (preUGT_tree.egEt[eg] >= 22 and abs(preUGT_tree.egEta[eg]) <= 2.1 and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_isoeg22 == 0): #tauEt = 0.5*tauIEt
            for eg2 in range(preUGT_tree.nEGs):
                if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= 22 and abs(preUGT_tree.egEta[eg2]) <= 2.1 and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #tauEt = 0.5*tauIEt
                    i_isoeg22 += 1
                    n_DoubleLooseIsoEG22 += 1
        if (preUGT_tree.egEt[eg] >=  24 and abs(preUGT_tree.egEta[eg]) <= 2.1 and (preUGT_tree.egIso[eg] == 2 or preUGT_tree.egIso[eg] == 3) and preUGT_tree.egBx[eg] == 0 and i_isoeg24 == 0): #tauEt = 0.5*tauIEt
            for eg2 in range(preUGT_tree.nEGs):
                if ((not eg2 == eg) and preUGT_tree.egEt[eg2] >= 24 and abs(preUGT_tree.egEta[eg2]) <= 2.1 and (preUGT_tree.egIso[eg2] == 2 or preUGT_tree.egIso[eg2] == 3) and preUGT_tree.egBx[eg2] == 0): #tauEt = 0.5*tauIEt
                    i_isoeg24 += 1
                    n_DoubleLooseIsoEG24 += 1

    # -----------------
    #       TAU       #
    # -----------------
    i_tau32 = 0 # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_tau34 = 0 # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_tau35 = 0 # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_tau120 = 0 # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    i_tau130 = 0 # Counter to avoid to count twice an event because of two taus in the same event passing the requirement
    for tau in range(preUGT_tree.nTaus):
        if (preUGT_tree.tauEt[tau] >= 120 and abs(preUGT_tree.tauEta[tau]) <= 2.1 and preUGT_tree.tauBx[tau] == 0 and i_tau120 == 0): #tauEt = 0.5*tauIEt
            i_tau120 += 1
            n_SingleTau120_er2p1 += 1
            #print("tauEt = ", preUGT_tree.tauEt[tau])
            #print("tauIEt = ", preUGT_tree.tauIEt[tau])
        if (preUGT_tree.tauEt[tau] >= 130 and abs(preUGT_tree.tauEta[tau]) <= 2.1 and preUGT_tree.tauBx[tau] == 0 and i_tau130 == 0): #tauEt = 0.5*tauIEt
            i_tau130 += 1
            n_SingleTau130_er2p1 += 1

        if (preUGT_tree.tauEt[tau] >= 32 and preUGT_tree.tauIso[tau] == 1 and preUGT_tree.tauBx[tau] == 0 and i_tau32 == 0): #tauEt = 0.5*tauIEt
            for tau2 in range(preUGT_tree.nTaus):
                if ((not tau2 == tau) and preUGT_tree.tauEt[tau2] >= 32 and preUGT_tree.tauIso[tau2] == 1 and preUGT_tree.tauBx[tau2] == 0):
                    i_tau32 += 1
                    n_DoubleIsoTau32 += 1
        if (preUGT_tree.tauEt[tau] >= 34 and preUGT_tree.tauIso[tau] == 1 and preUGT_tree.tauBx[tau] == 0 and i_tau34 == 0): #tauEt = 0.5*tauIEt
            for tau2 in range(preUGT_tree.nTaus):
                if ((not tau2 == tau) and preUGT_tree.tauEt[tau2] >= 34 and preUGT_tree.tauIso[tau2] == 1 and preUGT_tree.tauBx[tau2] == 0):
                    i_tau34 += 1
                    n_DoubleIsoTau34 += 1
        if (preUGT_tree.tauEt[tau] >= 35 and preUGT_tree.tauIso[tau] == 1 and preUGT_tree.tauBx[tau] == 0 and i_tau35 == 0): #tauEt = 0.5*tauIEt
            for tau2 in range(preUGT_tree.nTaus):
                if ((not tau2 == tau) and preUGT_tree.tauEt[tau2] >= 35 and preUGT_tree.tauIso[tau2] == 1 and preUGT_tree.tauBx[tau2] == 0):
                    i_tau35 += 1
                    n_DoubleIsoTau35 += 1

    # -----------------
    #       JET       #
    # -----------------
    i_jet120 = 0 # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet140 = 0 # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet160 = 0 # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    i_jet180 = 0 # Counter to avoid to count twice an event because of two jets in the same event passing the requirement
    for jet in range(preUGT_tree.nJets):
        if (preUGT_tree.jetEt[jet] >= 120 and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet120 == 0): #jetEt = 0.5*tauIEt
            i_jet120 += 1
            n_SingleJet120er2p5 += 1
            #print("jetEt = ", preUGT_tree.jetEt[jet])
            #print("jetIEt = ", preUGT_tree.jetIEt[jet])
        if (preUGT_tree.jetEt[jet] >= 140 and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet140 == 0): #jetEt = 0.5*tauIEt
            i_jet140 += 1
            n_SingleJet140er2p5 += 1
        if (preUGT_tree.jetEt[jet] >= 160 and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet160 == 0): #jetEt = 0.5*tauIEt
            i_jet160 += 1
            n_SingleJet160er2p5 += 1
        if (preUGT_tree.jetEt[jet] >= 180 and abs(preUGT_tree.jetEta[jet]) <= 2.5 and preUGT_tree.jetBx[jet] == 0 and i_jet180 == 0): #jetEt = 0.5*tauIEt
            i_jet180 += 1
            n_SingleJet180er2p5 += 1

    # -----------------
    #       MET       #
    # -----------------
    i_etmhf70 = 0 # Counter to avoid to count twice an event because of two met values in the same event passing the requirement
    i_etmhf80 = 0 # Counter to avoid to count twice an event because of two met values in the same event passing the requirement
    i_etmhf90 = 0 # Counter to avoid to count twice an event because of two met values in the same event passing the requirement
    i_etmhf100 = 0 # Counter to avoid to count twice an event because of two met values in the same event passing the requirement

    for met in range(preUGT_tree.nSums):
        if (not(preUGT_tree.sumType[met] == 8)): continue # 8 = 'kMissingEtHF'
        if (preUGT_tree.sumEt[met] >= 70 and preUGT_tree.sumBx[met] == 0 and i_etmhf70 == 0): #sumEt = 0.5*sumIEt
            i_etmhf70 += 1
            n_ETMHF70 += 1
            #print("sumEt = ", preUGT_tree.sumEt[met])
            #print("sumIEt = ", preUGT_tree.sumIEt[met])
        if (preUGT_tree.sumEt[met] >= 80 and preUGT_tree.sumBx[met] == 0 and i_etmhf80 == 0): #sumEt = 0.5*sumIEt
            i_etmhf80 += 1
            n_ETMHF80 += 1
        if (preUGT_tree.sumEt[met] >= 90 and preUGT_tree.sumBx[met] == 0 and i_etmhf90 == 0): #sumEt = 0.5*sumIEt
            i_etmhf90 += 1
            n_ETMHF90 += 1
        if (preUGT_tree.sumEt[met] >= 100 and preUGT_tree.sumBx[met] == 0 and i_etmhf100 == 0): #sumEt = 0.5*sumIEt
            i_etmhf100 += 1
            n_ETMHF100 += 1


print("###### MU ######")
print("n_SingleMu22 = ", n_SingleMu22)
print("n_SingleMu25 = ", n_SingleMu25)
print("###### EG ######")
print("n_SingleEG36_er2p5 = ", n_SingleEG36_er2p5)
print("n_SingleEG38_er2p5 = ", n_SingleEG38_er2p5)
print("n_SingleEG40_er2p5 = ", n_SingleEG40_er2p5)
print("n_DoubleLooseIsoEG22 = ", n_DoubleLooseIsoEG22)
print("n_DoubleLooseIsoEG24 = ", n_DoubleLooseIsoEG24)
print("###### TAU ######")
print("n_SingleTau120_er2p1 = ", n_SingleTau120_er2p1)
print("n_SingleTau130_er2p1 = ", n_SingleTau130_er2p1)
print(" n_DoubleIsoTau32 = ", n_DoubleIsoTau32)
print(" n_DoubleIsoTau34 = ", n_DoubleIsoTau34)
print(" n_DoubleIsoTau35 = ", n_DoubleIsoTau35)
print("###### JET ######")
print("n_SingleJet120er2p5 = ", n_SingleJet120er2p5)
print("n_SingleJet140er2p5 = ", n_SingleJet140er2p5)
print("n_SingleJet160er2p5 = ", n_SingleJet160er2p5)
print("n_SingleJet180er2p5 = ", n_SingleJet180er2p5)
print("###### MET ######")
print("n_ETMHF70 = ", n_ETMHF70)
print("n_ETMHF80 = ", n_ETMHF80)
print("n_ETMHF90 = ", n_ETMHF90)
print("n_ETMHF100 = ", n_ETMHF100)
print("###### ... ######")


print("--------------------------")
print("FINAL Nentries evt_tree: ", evt_tree.GetEntries())
print("FINAL Nentries evt_tree in LS range: ", (evt_tree.GetEntries() - nEvt_outLSrange))
print("--------------------------")
