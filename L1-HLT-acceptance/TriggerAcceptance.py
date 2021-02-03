#! /usr/bin/env python

##################################################################################################################
###  TriggerAcceptance.py : A script to compute the fraction of passing Level-1 Trigger paths accepted by HLT  ###
##################################################################################################################

import ROOT as R
R.gROOT.SetBatch(True)  ## Don't display histograms or canvases when drawn

import os
import sys
import math
import subprocess
import collections
import pdb

## User-defined constants
MAX_FILE = 1      ## Maximum number of input files to process
MAX_EVT  = 10000 ## Maximum number of events to process
PRT_EVT  = 10000  ## Print to screen every Nth event
VERBOSE  = False  ## Print extra info about each event

PU_MIN   = 50      ## Minimum number of good reconstructed vertices


def main():

###################
## Initialize files
###################
    ## Location of input files
    in_file_names = []
    in_dir = '/afs/cern.ch/user/m/mmatthew/public/'

    ## Loop through available input files
    for file_name in subprocess.check_output(['ls', in_dir]).splitlines():
        if not '.root' in file_name: continue
        in_file_names.append(in_dir+file_name)
        print 'Opening file: '+in_file_names[-1]
        if len(in_file_names) >= MAX_FILE: break

    ## Chain together trees from input files
    in_chains = []
    for i in range(len(in_file_names)):
        in_chains.append( R.TChain('Events') )
        in_chains[i].Add( in_file_names[i] )

    ## Name of output file and directory
    BASE_STR = 'TriggerAcceptance'

    ## Set output directories (create if they do not exist)
    if not os.path.exists('plots/png/%s/' % BASE_STR):
        os.makedirs('plots/png/%s/' % BASE_STR)
        
    out_file = R.TFile('plots/%s.root' % BASE_STR, 'recreate')
    png_dir  = 'plots/png/%s/' % BASE_STR

    
####################
## L1T and HLT paths
####################
    
    print(in_chains)
    ## List of branches in the tree
    branch_list = [key.GetName() for key in in_chains[0].GetListOfBranches()]
    ## All L1T and HLT paths in the tree
    L1T_paths = [path for path in branch_list if path.startswith('L1_')]
    HLT_paths = [path for path in branch_list if path.startswith('HLT_')]

    ## 49 unprescaled L1T paths with some unique rate in 2018
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

    ## Check that user-entered L1T paths exist in ZeroBias NanoAOD file
    for path in L1T_unpr_all:
        if not path in L1T_paths:
            print '\nSUPER WEIRD!!! Path %s from group %s not in TTree!!! Quitting.' % (path, group)
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

    ## Loop through input files
    for ch in in_chains:
        
        if iEvt >= MAX_EVT and MAX_EVT > 0: break

        ## Loop through events in each file
        for jEvt in range(ch.GetEntries()):

            if iEvt >= MAX_EVT and MAX_EVT > 0: break
            iEvt += 1
            if iEvt % PRT_EVT is 0: print 'Event #%d / %d' % (iEvt, MAX_EVT)

            ch.GetEntry(jEvt)
            
            if ch.PV_npvsGood < PU_MIN: continue
            iPass += 1

            ## if VERBOSE: print '\nIn event %d we find %d vertices (%d "Good", %d "Other")' % (iEvt, ch.PV_npvs, ch.PV_npvsGood, ch.nOtherPV)

            hists['nPV']     .Fill( min( max(ch.PV_npvs,     nPV_bins[1]+0.01), nPV_bins[2]-0.01) )
            hists['nPV_good'].Fill( min( max(ch.PV_npvsGood, nPV_bins[1]+0.01), nPV_bins[2]-0.01) )


            ## List of L1T paths which fired
            L1T_pass = []
            ## Dictionary of bools storing which L1T paths passed
            L1T_acc = collections.OrderedDict()
            
            ## Check which L1T paths fired in this event
            for path in L1T_unpr_all:
                if ch.GetLeaf(path).GetValue() == 1:
                    L1T_pass.append(path)
                    L1T_acc[path] = []
                    if VERBOSE: print 'In event %d, %s passes' % (iEvt, path)
            ## End loop: for path in L1T_unpr_all

            ## Can quit here if no L1T paths fired
            if len(L1T_pass) == 0: continue

            if VERBOSE:
                print 'Total list of L1T seeds which fire in this event:'
                print L1T_pass

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

                if VERBOSE: print 'In event %d, %s fired' % (iEvt, path)

                ## Check to see which fired L1T paths seeded this HLT path
                for seed in L1T_pass:
                    ###############################     FIXME!!!     ###############################
                    ###  Should have a conditional that this L1T path seeds the firing HLT path  ###
                    ###           e.g. if not seed in HLT_L1_seeds[path]: continue               ###
                    ################################################################################
                    L1T_acc       [seed].append(path)
                    HLT_seed_fired[path].append(seed)
                    if VERBOSE: print '  * %s also fired' % seed
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
                        print '\n\nBizzare error!!! nAcc_L1T = %d! Quitting.' % nAcc_L1T
                        sys.exit()

                    if VERBOSE: print '%s seeded an HLT path with %d other firing seeds' % (seed, nAcc_L1T)

                    hists['L1T_%s_acc_rate_total' % group].Fill(iL1T+1)
                    hists['L1T_%s_acc_rate_pure'  % group].Fill(iL1T+1, (nAcc_L1T == 1))
                    hists['L1T_%s_acc_rate_prop'  % group].Fill(iL1T+1, 1.0 / nAcc_L1T)

                ## End loop: for iL1T in range(len(L1T_unpr[group])):
            ## End loop: for group in L1T_unpr.keys():

                    
        ## End loop over events in chain (jEvt)
    ## End loop over chains (ch)

    print 'Finished with loop over %d events' % iEvt


######################
## Save the histograms
######################

    ## Navigate to the output ROOT file
    out_file.cd()

    ## Create the canvas for drawing histograms
    c0 = R.TCanvas('c0')
    c0.cd()


    ## Draw the number of primary vertices
    hists['nPV_good'].SetLineWidth(2)
    hists['nPV_good'].SetLineColor(R.kBlue)
    hists['nPV_good'].Draw('hist')
    hists['nPV_good'].Write()
    hists['nPV'].SetLineWidth(2)
    hists['nPV'].SetLineColor(R.kBlack)
    hists['nPV'].Draw('histsame')
    hists['nPV'].Write()
    c0.SaveAs(png_dir+'h_nPV.png')


    ## Configure the rate histograms
    for hname in hists.keys():
        hists[hname].SetMinimum(0)
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
#            hists[hname].Scale(30000. / iPass)
            print("Re-insert Scaling!!!!")




    ## Draw the HLT rate histograms
    hists['HLT_rate_total'].SetLineWidth(2)
    hists['HLT_rate_total'].SetLineColor(R.kBlack)
    hists['HLT_rate_total'].Draw('hist')
    hists['HLT_rate_total'].Write()
    c0.SaveAs(png_dir+'h_HLT_rate.png')


    ## Draw the L1T rate and acceptance histograms for each group
    for group in L1T_unpr.keys():

        ## Raw L1T rate histograms, including purity w.r.t. other L1T seeds
        hists['L1T_%s_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_rate_total' % group].Draw('hist')
        hists['L1T_%s_rate_total' % group].Write()
        hists['L1T_%s_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_rate_prop'  % group].Draw('histsame')
        hists['L1T_%s_rate_prop'  % group].Write()
        hists['L1T_%s_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_rate_pure'  % group].Draw('histsame')
        hists['L1T_%s_rate_pure'  % group].Write()
        hists['L1T_%s_rate_total' % group].SetTitle('L1T rates')

        c0.SaveAs(png_dir+'h_L1T_%s_rate.png' % group)

        ## HLT acceptance rate histograms, including purity w.r.t. HLT acceptance
        hists['L1T_%s_acc_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('hist')
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histsame')
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histsame')
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T HLT-accepted rates')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_rate.png' % group)

        ## Total rate and HLT acceptance overlaid
        hists['L1T_%s_rate_total'     % group].Draw('hist')
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kViolet)
        hists['L1T_%s_acc_rate_total' % group].Draw('histsame')
        hists['L1T_%s_rate_total'     % group].SetTitle('L1T net and HLT-accepted total rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_total.png' % group)

        ## Proportional rate and HLT acceptance overlaid
        hists['L1T_%s_rate_prop'     % group].Draw('hist')
        hists['L1T_%s_acc_rate_prop' % group].SetLineColor(R.kGreen)
        hists['L1T_%s_acc_rate_prop' % group].Draw('histsame')
        hists['L1T_%s_rate_prop'     % group].SetTitle('L1T net and HLT-accepted proportional rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_prop.png' % group)

        ## Pure rate and HLT acceptance overlaid
        hists['L1T_%s_rate_pure'     % group].Draw('hist')
        hists['L1T_%s_acc_rate_pure' % group].SetLineColor(R.kMagenta)
        hists['L1T_%s_acc_rate_pure' % group].Draw('histsame')
        hists['L1T_%s_rate_pure'     % group].SetTitle('L1T net and HLT-accepted pure rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_pure.png' % group)

        ## HLT acceptance fraction histograms, including purity w.r.t. HLT acceptance
        hists['L1T_%s_acc_rate_total' % group].Divide  (hists['L1T_%s_rate_total' % group])
        hists['L1T_%s_acc_rate_total' % group].SetName ('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].SetTitle('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(R.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('histe')
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].Divide  (hists['L1T_%s_rate_prop' % group])
        hists['L1T_%s_acc_rate_prop'  % group].SetName ('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].SetTitle('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(R.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histesame')
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].Divide  (hists['L1T_%s_rate_pure' % group])
        hists['L1T_%s_acc_rate_pure'  % group].SetName ('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].SetTitle('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(R.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histesame')
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T fraction of HLT-accepted events')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_frac.png' % group)

    ## End loop: for group in L1T_unpr.keys()
    
    

    ## Delete the output ROOT file from local memory
    del out_file


## Define 'main' function as primary executable
if __name__ == '__main__':
    main()
