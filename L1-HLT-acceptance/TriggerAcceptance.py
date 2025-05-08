#! /usr/bin/env python

##################################################################################################################
###  TriggerAcceptance.py : A script to compute the fraction of passing Level-1 Trigger paths accepted by HLT  ###
##################################################################################################################
## First development of the code: Andrew Brinkerhoff
## Accept. study 2021 adaptation: Mark Matthewman, Anna Stakia
##################################################################################################################

import ROOT as R
R.gROOT.SetBatch(True)  ## Don't display histograms or canvases when drawn

import os
import sys
import math
import subprocess
import collections
import pdb
import csv
import argparse

parser = argparse.ArgumentParser(description = 'A script to compute the fraction of passing Level-1 Trigger paths accepted by HLT')

parser.add_argument('--MAX_FILE', type = int, default = 1, help = 'Maximum number of input files to process')
parser.add_argument('--MAX_EVT', type = int, default = 500000, help = 'Maximum number of events to process')
parser.add_argument('--PRT_EVT', type = int, default = 10000, help = 'Print to screen every Nth event')
parser.add_argument('--VERBOSE', action = 'store_true', default = False, help = 'Print extra info about each event')
parser.add_argument('--PU_MIN', type = int, default = 0, help = 'Minimum number of good reconstructed vertices')
parser.add_argument('--ROOTFILES', type =str, default = 'example-inputs/fnames.txt', help = 'Location of .txt file containing .root file names' )
parser.add_argument('--REDIRECTOR', type = str, default = 'root://xrootd-cms.infn.it/', help = 'Redirector used to load .root files from')
parser.add_argument('--HLT_L1_SEEDS', type = str, default = './hlt_l1_seeds/hlt_l1.csv', help = 'Name of .csv file containing run specific HLT-L1 combinations')

args = parser.parse_args()

## User-defined constants
MAX_FILE = args.MAX_FILE    ## Maximum number of input .root files to process
MAX_EVT  = args.MAX_EVT     ## Maximum number of events to process
PRT_EVT  = args.PRT_EVT     ## Print to screen every Nth event
VERBOSE  = args.VERBOSE     ## Print extra info about each event
PU_MIN   = args.PU_MIN      ## Minimum number of good reconstructed vertices

def main():

###################
## Initialize files
###################

    ## List to store location of the input .root files
    in_file_names = []

    ## Loop over available input .root files existing in in_dir
    #in_dir = '/afs/cern.ch/<X>/<Y>/<ZZ>'
    #for file_name in subprocess.check_output(['ls', in_dir]).splitlines():
    #    if not '.root' in file_name: continue
    #    in_file_names.append(in_dir+file_name)
    #    print 'Opening file: '+in_file_names[-1]
    #    if len(in_file_names) >= MAX_FILE: break

    ## Read and store location of input .root files
    txtfile = args.ROOTFILES
    redirector = args.REDIRECTOR
    for ele in open(txtfile,'r'):
        in_file_names.append(redirector + ele.split('\n')[0])
        if len(in_file_names) >= MAX_FILE:break

    ## Chain together trees from input .root files
    in_chains = []
    for i in range(len(in_file_names)):
        in_chains.append( R.TChain('Events') )
        in_chains[i].Add( in_file_names[i] )

    ## Name of output file and directory
    BASE_STR = 'TriggerAcceptance'

    ## Set output directories for .pdf and .png files (create if not existing)
    if not os.path.exists('plots/png/%s/' % BASE_STR):
        os.makedirs('plots/png/%s/' % BASE_STR)
    if not os.path.exists('plots/pdf/%s/' % BASE_STR):
        os.makedirs('plots/pdf/%s/' % BASE_STR)

    out_file = R.TFile('plots/%s.root' % BASE_STR, 'recreate')
    png_dir  = 'plots/png/%s/' % BASE_STR
    pdf_dir  = 'plots/pdf/%s/' % BASE_STR


####################
## L1T and HLT paths
####################

    print(in_chains)
    ## List of branches in the tree
    branch_list = [key.GetName() for key in in_chains[0].GetListOfBranches()]

    ## L1T_paths (HLT_paths): all the L1T (HLT) paths included in the tree, coming from the input .root files
    L1T_paths = [path for path in branch_list if path.startswith('L1_')]
    HLT_paths = [path for path in branch_list if path.startswith('HLT_')]

    ## L1T_unpr_all: Set of *unprescaled* L1T seeds that we use for the rate calculations on the input samples 
    ## Below are 49 unprescaled L1T paths with some unique rate in 2018
    ## Copied from L1T paper, Tables 2 and 3 (http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=TRG-17-001)
    ## 2018 prescales here: https://cmsoms.cern.ch/cms/triggers/prescale?cms_run=322079&cms_run_sequence=GLOBAL-RUN
    L1T_unpr = collections.OrderedDict()
    L1T_unpr['Muon']     = ['L1_SingleMu22', 'L1_DoubleMu_15_7', 'L1_DoubleMu_15_5_SQ', 'L1_DoubleMu8_SQ', 'L1_DoubleMu4p5er2p0_SQ_OS_Mass_Min7',
                            'L1_DoubleMu4_SQ_OS_dR_Max1p2', 'L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4', 'L1_TripleMu_5_3_3', 'L1_TripleMu3_SQ',
                            'L1_TripleMu_5_3p5_2p5_DoubleMu_5_2p5_OS_Mass_5to17',
                            'L1_TripleMu_5SQ_3SQ_0OQ_DoubleMu_5_3_SQ_OS_Mass_Max9']
    L1T_unpr['EG']       = ['L1_SingleEG60', 'L1_SingleEG36er2p5', 'L1_SingleLooseIsoEG28er2p5', 'L1_DoubleEG_25_12_er2p5',
                            'L1_DoubleEG_LooseIso22_12_er2p5', 'L1_TripleEG_18_17_8_er2p5', 'L1_TripleEG16er2p5']
    L1T_unpr['Tau']      = ['L1_SingleTau120er2p1', 'L1_DoubleIsoTau32er2p1']
    L1T_unpr['Jet']      = ['L1_SingleJet180', 'L1_DoubleJet150er2p5', 'L1_DoubleJet112er2p3_dEta_Max1p6',
                            'L1_DoubleJet_110_35_DoubleJet35_Mass_Min620',
                            'L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5', 'L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5']
    L1T_unpr['Sum']      = ['L1_ETMHF100', 'L1_HTT360er', 'L1_ETT2000']
    L1T_unpr['TwoObj']   = ['L1_Mu20_EG10er2p5', 'L1_Mu7_EG20er2p5', 'L1_Mu18er2p1_Tau24er2p1', 'L1_Mu6_HTT240er',
                            'L1_LooseIsoEG22er2p1_IsoTau26er2p1_dR_Min0p3', 'L1_LooseIsoEG28er2p1_Jet34er2p5_dR_Min0p3',
                            'L1_LooseIsoEG26er2p1_HTT100er', 'L1_IsoTau40er2p1_ETMHF90', 'L1_SingleJet140er2p5_ETMHF80']
    L1T_unpr['ThreeObj'] = ['L1_Mu12er2p3_Jet40er2p3_dR_Max0p4_DoubleJet40er2p3_dEta_Max1p6', 'L1_Mu3er1p5_Jet100er2p5_ETMHF40',
                            'L1_DoubleMu3_SQ_HTT220er', 'L1_DoubleMu0_dR_Max1p6_Jet90er2p5_dR_Max0p8', 'L1_DoubleMu5_SQ_EG9er2p5',
                            'L1_Mu6_DoubleEG12er2p5', 'L1_DoubleEG8er2p5_HTT300er']
    L1T_unpr['ManyObj']  = ['L1_DoubleMu3_OS_DoubleEG7p5Upsilon', 'L1_DoubleMu5Upsilon_OS_DoubleEG3',
                            'L1_DoubleMu3_SQ_ETMHF50_Jet60er2p5_OR_DoubleJet40er2p5', 'L1_HTT320er_QuadJet_70_55_40_40_er2p4']

    ## *** Excluded paths ***
    ## L1_SingleMuOpen_er1p4_NotBptxOR_3BX : Can't measure NotBptx rates with ZeroBias
    ## L1_SingleJet43er2p5_NotBptxOR_3BX   : Can't measure NotBptx rates with ZeroBias

    ## Make list of all unprescaled L1T paths
    L1T_unpr_all = []
    for group in L1T_unpr.keys():
        L1T_unpr_all += L1T_unpr[group]

    ## Check that user-entered L1T paths (L1T_unpr_all) exist in ZeroBias NanoAOD file inserted as input (L1T_paths). Their non-overlap is L1T_missing
    L1T_missing = []
    groups = list(L1T_unpr.keys())
    for path in L1T_unpr_all:
        if not path in L1T_paths:
            for group in groups:
                if path in L1T_unpr[group]: break
            print('\nUser defined unprescaled seed %s from group %s not in TTree!!! Program will continue.' % (path,group))
            L1T_missing.append(path)

    ## HLT_L1_seeds: HLT-L1T 'seed-mappings' based on whether a specific L1T path seeded a particular HLT path. Created through the output of seeds.py
    HLT_L1_seeds = {}
    HLT_missing = []
    path = args.HLT_L1_SEEDS
    with open(path,'r') as csvfile:
        reader = csv.reader(csvfile,delimiter=',')
        header = next(reader)
        for j in range(1,len(header)):
            HLT_L1_seeds[header[j]] = {}
        for row in reader:
            for j in range(1,len(row)):
                HLT_L1_seeds[header[j]][row[0]] = row[j].split(' OR ')


    ## Remove missing paths to avoid errors when calling ch.GetLeaf(path)
    for ele in L1T_missing:
        L1T_unpr_all.remove(ele)


    if L1T_unpr_all ==[]:
        print('None of the user defined unprescaled seeds can be found in TTree. System exit')
        sys.exit()

#############
## Histograms
#############

    ## Histogram bins: [# of bins, minimum x, maximum x]
    nPV_bins  = [101, -0.5, 100.5]                            ## Bins for number of primary vertices
    nHLT_bins = [len(HLT_paths), 0.5, len(HLT_paths) + 0.5]  ## Bins for High Level Trigger paths
    nL1T_bins = collections.OrderedDict()                     ## Dictionary of Level-1 Trigger "groups"
    for group in L1T_unpr.keys():
        nL1T_bins[group] = [len(L1T_unpr[group]), 0.5, len(L1T_unpr[group]) + 0.5]  ## Bins for Level-1 Trigger paths by group

    ## Book 1D histograms
    ## Important to use '1D' instead of '1F' when dealing with large numbers of entries, and weighted events (higher precision)

    hists = collections.OrderedDict()  ## Dictionary of all histograms

    hists['nPV']      = R.TH1D('h_nPV'     , 'h_nPV'     , nPV_bins[0], nPV_bins[1], nPV_bins[2])
    hists['nPV_good'] = R.TH1D('h_nPV_good', 'h_nPV_good', nPV_bins[0], nPV_bins[1], nPV_bins[2])

    hists['HLT_rate_total'] = R.TH1D('h_HLT_rate_total', 'h_HLT_rate_total', nHLT_bins[0], nHLT_bins[1], nHLT_bins[2])

    for group in L1T_unpr.keys():
        hists['L1T_%s_rate_total' % group] = R.TH1D('h_L1T_%s_rate_total' % group, 'h_L1T_%s_rate_total' % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_rate_prop'  % group] = R.TH1D('h_L1T_%s_rate_prop'  % group, 'h_L1T_%s_rate_prop'  % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_rate_pure'  % group] = R.TH1D('h_L1T_%s_rate_pure'  % group, 'h_L1T_%s_rate_pure'  % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])

        hists['L1T_%s_acc_rate_total' % group] = R.TH1D('h_L1T_%s_acc_rate_total' % group, 'h_L1T_%s_acc_rate_total' % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_acc_rate_prop'  % group] = R.TH1D('h_L1T_%s_acc_rate_prop'  % group, 'h_L1T_%s_acc_rate_prop'  % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_acc_rate_pure'  % group] = R.TH1D('h_L1T_%s_acc_rate_pure'  % group, 'h_L1T_%s_acc_rate_pure'  % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])


#############
## Event loop
#############


    iEvt  = 0  ## Event index
    iPass = 0  ## Number of events passing pileup cut 

    ## Loop over input .root files
    for ch in in_chains:

        if iEvt >= MAX_EVT and MAX_EVT > 0: break

        ## Loop through events in each file
        for jEvt in range(ch.GetEntries()):

            if iEvt >= MAX_EVT and MAX_EVT > 0: break
            iEvt += 1
            if iEvt % PRT_EVT == 0: print('Event #%d / %d' % (iEvt, MAX_EVT))

            ch.GetEntry(jEvt)
            run = str(ch.GetLeaf('run').GetValue()).split('.')[0]

            ## Pileup check
            if ch.PV_npvsGood < PU_MIN: continue
            iPass += 1

            ## if VERBOSE: print('\nIn event %d we find %d vertices (%d "Good", %d "Other")' % (iEvt, ch.PV_npvs, ch.PV_npvsGood, ch.nOtherPV))

            hists['nPV']     .Fill( min( max(ch.PV_npvs,     nPV_bins[1]+0.01), nPV_bins[2]-0.01) )
            hists['nPV_good'].Fill( min( max(ch.PV_npvsGood, nPV_bins[1]+0.01), nPV_bins[2]-0.01) )


            ## List of L1T paths which fired in jEvt
            L1T_pass = []
            ## Dictionary of bools storing which L1T paths passed
            L1T_acc = collections.OrderedDict()

            ## Check which L1T paths fired in jEvt
            for path in L1T_unpr_all:
                if ch.GetLeaf(path).GetValue() == 1:
                    L1T_pass.append(path)
                    L1T_acc[path] = []
                    if VERBOSE: print('In event %d, %s passes' % (iEvt, path))
            ## End loop: for path in L1T_unpr_all

            ## Can quit here if no L1T paths fired
            if len(L1T_pass) == 0: continue

            if VERBOSE:
                print('Total list of L1T seeds which fire in this event:')
                print(L1T_pass)

            ## Dictionary of lists of fired L1T paths seeding HLT paths which pass
            HLT_seed_fired = collections.OrderedDict()

            ## Check which HLT paths fired in this event
            for iHLT in range(len(HLT_paths)):
                path = HLT_paths[iHLT]
                ## Require that this path fired
                if ch.GetLeaf(path).GetValue() != 1: continue
                ## Exclude monitoring paths
                if 'ZeroBias' in path or path == 'HLT_Random': continue

                hists['HLT_rate_total'].Fill(iHLT+1)
                HLT_seed_fired[path] = []

                if VERBOSE: print('In event %d, %s fired' % (iEvt, path))

                ## Check to see which fired L1T paths seeded this HLT path
                for seed in L1T_pass:
                    try:
                        if not seed in HLT_L1_seeds[run][path]:
                            print('%s not in %s!!!' % (seed,path))
                            continue

                        ## Conditional that this L1T path seeded the firing HLT path
                        L1T_acc[seed].append(path)
                        HLT_seed_fired[path].append(seed)
                        print('  * %s fires %s' % (seed,path))
                        if VERBOSE: print('  * %s fires %s' % (seed,path))

                    except KeyError:
                        HLT_missing.append(path)
                        print('\n%s not in user defined HLT-L1-seeds!!! Program will continue.' % (path))

                ## End loop: for seed in L1T_pass:
            ## End loop: for iHLT in range(len(HLT_paths))


            ## Fill L1T rate and acceptance plots
            for group in L1T_unpr.keys():
                for iL1T in range(len(L1T_unpr[group])):
                    seed = L1T_unpr[group][iL1T]

                    ## Require that this path fired
                    if not seed in L1T_pass: continue

                    hists['L1T_%s_rate_total' % group].Fill(iL1T+1)
                    hists['L1T_%s_rate_pure'  % group].Fill(iL1T+1, (len(L1T_pass) == 1))
                    hists['L1T_%s_rate_prop'  % group].Fill(iL1T+1, 1.0 / len(L1T_pass))


                    ## Require that L1T path seeded at least one HLT path which fired
                    if len(L1T_acc[seed]) == 0: continue
                    ## For each fired HLT path seeded by this L1T path, count the number of other
                    ## L1T paths seeding this HLT path which also fired.  Find the minimum number.
                    nAcc_L1T = 999
                    for path in L1T_acc[seed]:
                        nAcc_L1T = min(nAcc_L1T, len(HLT_seed_fired[path]))

                    if nAcc_L1T == 0 or nAcc_L1T == 999:
                        print('\n\nBizzare error!!! nAcc_L1T = %d! Quitting.' % nAcc_L1T)
                        sys.exit()

                    if VERBOSE: print('%s seeded an HLT path with %d other firing seeds' % (seed, nAcc_L1T))

                    hists['L1T_%s_acc_rate_total' % group].Fill(iL1T+1)
                    hists['L1T_%s_acc_rate_pure'  % group].Fill(iL1T+1, (nAcc_L1T == 1))
                    hists['L1T_%s_acc_rate_prop'  % group].Fill(iL1T+1, 1.0 / nAcc_L1T)


                ## End loop: for iL1T in range(len(L1T_unpr[group])):
            ## End loop: for group in L1T_unpr.keys():


        ## End loop over events in chain (jEvt)
    ## End loop over chains (ch)

    print('Finished with loop over %d events' % iEvt)

    ## Configure the rate histograms
    for hname in hists.keys():
        hists[hname].SetStats(0)
        ## Label the L1T and HLT histogram axes
        if hname.startswith('HLT') and hists[hname].GetNbinsX() == len(HLT_paths):
            for iHLT in range(len(HLT_paths)):
                hists[hname].GetXaxis().SetBinLabel(iHLT+1, HLT_paths[iHLT])
                hists[hname].GetYaxis().SetTitle('Rate (kHz)')
        for group in L1T_unpr.keys():
            if hname.startswith('L1T_%s' % group) and hists[hname].GetNbinsX() == len(L1T_unpr[group]):
                for iL1T in range(len(L1T_unpr[group])):
                    hists[hname].GetXaxis().SetBinLabel(iL1T+1, L1T_unpr[group][iL1T])

                    hists[hname].GetYaxis().SetTitle('Rate (kHz)')


        ## Scale counts by 30 MHz to get trigger rate
        if (hname.startswith('HLT') or hname.startswith('L1T')) and '_rate_' in hname:
            hists[hname].Scale(30000. / iPass)
#            print("Re-insert Scaling!!!!")

        hists[hname].SetMinimum(0) # Necessary to set at end otherwise command is ignored due to scaling

    ## Two 2D Tables

    from prettytable import PrettyTable

    x = PrettyTable()
    y = PrettyTable()

    x.field_names = ['group', 'total rate', 'prop rate', 'pure rate']
    y.field_names = ['group', 'total acc rate', 'prop acc rate', 'pure acc rate']

    for group in L1T_unpr.keys():

        total_x = hists['L1T_%s_rate_total' % group].Integral()
        total_y = hists['L1T_%s_acc_rate_total' % group].Integral()

        prop_x = hists['L1T_%s_rate_prop' % group].Integral()
        prop_y = hists['L1T_%s_acc_rate_prop' % group].Integral()

        pure_x = hists['L1T_%s_rate_pure' % group].Integral()
        pure_y = hists['L1T_%s_acc_rate_pure' % group].Integral()

        x.add_row([group, total_x, prop_x, pure_x])
        y.add_row([group, total_y, prop_y, pure_y])

    print(x)
    print(y)

######################
## Save the histograms
######################

    ## Navigate to the output ROOT file
    out_file.cd()

    ## Create the canvas for drawing histograms
    c0 = R.TCanvas('c0')
    c0.cd()


    ## Draw the number of primary vertices
    legend = R.TLegend(0.78,0.78,0.98,0.93)
    hists['nPV_good'].SetLineWidth(2)
    hists['nPV_good'].SetLineColor(R.kBlue)
    hists['nPV_good'].Draw('hist')
    legend.AddEntry(hists['nPV_good'], 'Good primary vertices', 'l')
    legend.Draw()
    hists['nPV_good'].Write()
    hists['nPV'].SetLineWidth(2)
    hists['nPV'].SetLineColor(R.kBlack)
    hists['nPV'].Draw('histsame')
    legend.AddEntry(hists['nPV'], 'Primary vertices', 'l')
    legend.Draw()
    hists['nPV'].Write()
    c0.SaveAs(png_dir+'h_nPV.png')
    c0.SaveAs(pdf_dir+'h_nPV.pdf')


    ## Draw the HLT rate histograms

    hists['HLT_rate_total'].SetLineWidth(2)
    hists['HLT_rate_total'].SetLineColor(R.kBlack)
    hists['HLT_rate_total'].Draw('hist')
    hists['HLT_rate_total'].Write()
    c0.SaveAs(png_dir+'h_HLT_rate.png')
    c0.SaveAs(pdf_dir+'h_HLT_rate.pdf')

    ## Draw the L1T rate and acceptance histograms for each group
    for group in L1T_unpr.keys():

        ## Raw L1T rate histograms, including purity w.r.t. other L1T seeds
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_rate_total' % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_total' % group], 'L1T total rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_total' % group].Write()
        hists['L1T_%s_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_rate_prop'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_rate_prop' % group], 'L1T prop. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_prop'  % group].Write()
        hists['L1T_%s_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_rate_pure'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_rate_pure' % group], 'L1T pure rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_pure'  % group].Write()
        hists['L1T_%s_rate_total' % group].SetTitle('L1T rates')

        c0.SaveAs(png_dir+'h_L1T_%s_rate.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate.pdf' % group)

        ## HLT acceptance rate histograms, including purity w.r.t. HLT acceptance
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_acc_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T HLT-accepted rates')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_rate.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_acc_rate.pdf' % group)

        ## Total rate and HLT acceptance overlaid
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_total'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_total' % group], 'L1T total rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kViolet)
        hists['L1T_%s_acc_rate_total' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_total'     % group].SetTitle('L1T net and HLT-accepted total rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_total.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_total.pdf' % group)

        ## Proportional rate and HLT acceptance overlaid
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_prop'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop' % group].SetLineColor(R.kGreen)
        hists['L1T_%s_acc_rate_prop' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_prop'     % group].SetTitle('L1T net and HLT-accepted proportional rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_prop.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_prop.pdf' % group)

        ## Pure rate and HLT acceptance overlaid
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_pure'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_pure' % group], 'L1T pure rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure' % group].SetLineColor(R.kMagenta)
        hists['L1T_%s_acc_rate_pure' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_pure'     % group].SetTitle('L1T net and HLT-accepted pure rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_pure.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_pure.pdf' % group)

        ## HLT acceptance fraction histograms, including purity w.r.t. HLT acceptance
        legend = R.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_acc_rate_total' % group].Divide  (hists['L1T_%s_rate_total' % group])
        hists['L1T_%s_acc_rate_total' % group].SetName ('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].SetTitle('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('histe')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].Divide  (hists['L1T_%s_rate_prop' % group])
        hists['L1T_%s_acc_rate_prop'  % group].SetName ('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].SetTitle('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histesame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].Divide  (hists['L1T_%s_rate_pure' % group])
        hists['L1T_%s_acc_rate_pure'  % group].SetName ('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].SetTitle('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histesame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T fraction of HLT-accepted events')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_frac.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_acc_frac.pdf' % group)

    ## End loop: for group in L1T_unpr.keys()


    ## Display the unprescaled seeds defined by the user which are not present in the .root file again
    if L1T_missing !=[]:
        print('\nThe missing seeds are:')
        for ele in L1T_missing:
            print(ele)

    if HLT_missing !=[]:
        print('\nThe missing paths are:')
        for ele in HLT_missing:
            print(ele)


    ## Delete the output ROOT file from local memory
    del out_file


## Define 'main' function as primary executable
if __name__ == '__main__':
    main()
