#!/usr/bin/env python3
#Andrew Loeliger, Nov 15, 2022
#Based on triggerAcceptance.py developed by Andrew Brinkerhoff, Mark Matthewman and Anna Stakia
#Takes output from seeds_L1.py, and a list of L1 ntuples, and makes the same plots as triggerAcceptance.py

import ROOT
import argparse
import json
import csv
import os
import re
import collections
from tqdm import trange

def main(args):
    #set-up chains and attach files to them
    eventChain = ROOT.TChain('l1EventTree/L1EventTree')
    uGTChain = ROOT.TChain('l1uGTTree/L1uGTTree')
    with open(args.ListOfFiles, mode='r') as csvFile:
        csvReader = csv.reader(csvFile)
        for row in csvReader:
            theFile = row[0]
            eventChain.Add(args.Redirector+theFile)
            uGTChain.Add(args.Redirector+theFile)
    eventChain.AddFriend(uGTChain) #let's us just deal with one chain in the end

    #Let's load up the HLT/L1 Seeds json
    with open(args.Seeds, 'r') as jsonFile:
        HLT_L1_seeds = json.load(jsonFile)

    #parse the L1 OMS text file to match up bit numbers to bit names
    #TODO: this solution with the text file is pretty horribly inelegant
    #A better solution would involve the actual OMS API, but that requires a lot of token
    #authentication I didn't want to write initially. That would be a good spot to upgrade
    runL1Mapping = {}
    for fileName in args.L1OMSOutput:
        runNumber = int(re.search('[0-9]+', fileName).group(0))
        with open(fileName, 'r') as theFile:
            data = theFile.read()
        groupedData = re.findall('([0-9]+)\n(L1_.*)\n', data)

        L1Mapping = {}
        for matchedData in groupedData:
            L1Mapping[int(matchedData[0])] = matchedData[1]
        runL1Mapping[runNumber] = L1Mapping
    
    #perform a similar set-up of the L1T and HLT paths as is done in triggerAcceptance.py
    #i.e. get a list of all the possible unique L1 paths between all of our runs
    L1T_Paths = []
    for run in runL1Mapping:
        L1T_Paths += list(runL1Mapping.keys())
    L1T_Paths = list(set(L1T_Paths))

    #getting the unique HLT paths we have a slightly more complex time
    #here we scroll throught the chain (disabling most branches for access speed)
    #and scan for unique entries in the HLT vector, and add that in to the list
    #This tends to pick up a bunch of zero bias or alca bits that we don't want
    #so we just remove any path that doesn't begin with "HLT", that isn't some zero bias bit, 
    #and ends in a version number
    HLT_Paths = []
    eventChain.SetBranchStatus("*", 0)
    eventChain.SetBranchStatus("hlt", 1)
    for i in range(eventChain.GetEntries()):
        eventChain.GetEntry(i)
        entryHLT = getattr(eventChain.Event, 'hlt')
        for path in entryHLT:
            if str(path) not in HLT_Paths:
                HLT_Paths.append(str(path))
    eventChain.SetBranchStatus("*", 1)
    cleanedHLT_Paths = []
    for path in HLT_Paths:
        reMatch = re.search('HLT(?!_Physics).*(?=_v[0-9]+)', path)
        if(reMatch):
            cleanedHLT_Paths.append(reMatch.group(0))
    HLT_Paths = cleanedHLT_Paths

    #prepare output space
    png_dir = f'{args.OutputDir}png/TriggerAcceptance/'
    pdf_dir = f'{args.OutputDir}pdf/TriggerAcceptance/'
    outputFile = ROOT.TFile(f'{args.OutputDir}TriggerAcceptance.root', 'RECREATE')
    if not os.path.exists(png_dir):
        os.makedirs(png_dir)
    if not os.path.exists(pdf_dir):
        os.makedirs(pdf_dir)

    #this is the same as trigger acceptance .py
    #We simply set up some collections of bits that will be useful to compare against and make plots for
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
    
    ## Make list of all unprescaled L1T paths
    L1T_unpr_all = []
    for group in L1T_unpr.keys():
        L1T_unpr_all += L1T_unpr[group]

    #Set-up histograms
    nPV_bins  = [101, -0.5, 100.5]
    nHLT_bins = [len(HLT_Paths), 0.5, len(HLT_Paths) + 0.5]
    nL1T_bins = collections.OrderedDict()
    for group in L1T_unpr.keys():
        nL1T_bins[group] = [len(L1T_unpr[group]), 0.5, len(L1T_unpr[group]) + 0.5]

    hists = collections.OrderedDict()  ## Dictionary of all histograms
    hists['nPV'] = ROOT.TH1D('h_nPV', 'h_nPV', nPV_bins[0], nPV_bins[1], nPV_bins[2])
    hists['nPV_good'] = ROOT.TH1D('h_nPV_good', 'h_nPV_good', nPV_bins[0], nPV_bins[1], nPV_bins[2])

    hists['HLT_rate_total'] = ROOT.TH1D('h_HLT_rate_total', 'h_HLT_rate_total', nHLT_bins[0], nHLT_bins[1], nHLT_bins[2])

    for group in L1T_unpr.keys():
        hists['L1T_%s_rate_total' % group] = ROOT.TH1D('h_L1T_%s_rate_total' % group, 'h_L1T_%s_rate_total' % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_rate_prop'  % group] = ROOT.TH1D('h_L1T_%s_rate_prop'  % group, 'h_L1T_%s_rate_prop'  % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_rate_pure'  % group] = ROOT.TH1D('h_L1T_%s_rate_pure'  % group, 'h_L1T_%s_rate_pure'  % group,
                                                   nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])

        hists['L1T_%s_acc_rate_total' % group] = ROOT.TH1D('h_L1T_%s_acc_rate_total' % group, 'h_L1T_%s_acc_rate_total' % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_acc_rate_prop'  % group] = ROOT.TH1D('h_L1T_%s_acc_rate_prop'  % group, 'h_L1T_%s_acc_rate_prop'  % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])
        hists['L1T_%s_acc_rate_pure'  % group] = ROOT.TH1D('h_L1T_%s_acc_rate_pure'  % group, 'h_L1T_%s_acc_rate_pure'  % group,
                                                        nL1T_bins[group][0], nL1T_bins[group][1], nL1T_bins[group][2])

    #This is the actual event loop. We start scrolling through everything in our chains
    iPass = eventChain.GetEntries() #TODO: in TriggerAcceptance.py there is PU checking, and this is the events that pass that
    for i in trange(eventChain.GetEntries(), desc='Making histograms'):
        eventChain.GetEntry(i)
        run = eventChain.Event.run

        #Fill our primary vertex 
        hists['nPV'].Fill(eventChain.Event.nPV)
        hists['nPV_good'].Fill(eventChain.Event.nPV_True) #TODO: check this, these may not actually be "good" vertices per se. We may not even have that info at L1

        passedBits = eventChain.L1uGT.getAlgoDecisionFinal()

        #Let's map this onto names
        L1T_pass = []
        L1T_to_HLT_Dict = {}
        for bitNum in range(len(passedBits)):
            if passedBits[bitNum]:
                L1T_pass.append(runL1Mapping[run][bitNum])  
        L1T_pass = [x for x in L1T_pass if x in L1T_unpr_all]  #let's narrow this down only to the unprescaled bits we're interested in
        for path in L1T_pass: #for each of these bits, we will check the HLT paths it seeded
            L1T_to_HLT_Dict[path] = []

        #If we don't have any of those, we're done
        if len(L1T_pass) == 0: continue

        #Check what HLT paths we have.
        #This is essentially the same cleaning step we performed earlier.
        #TODO: Could consider removing redundant code by making this check a function
        HLT_pass = []
        HLT_to_L1T_Dict = {} #for each HLT path, we can check what L1T paths are used
        for hltPath in eventChain.Event.hlt:
            hltPath = str(hltPath) #ROOT likes to store this as some TString format that is only vaguely python string related.
            reMatch = re.search('HLT(?!_Physics).*(?=_v[0-9]+)', hltPath)
            if (reMatch):
                HLT_pass.append(reMatch.group(0))
        for hltPath in HLT_pass:
            hists['HLT_rate_total'].Fill(HLT_Paths.index(hltPath))
            HLT_to_L1T_Dict[hltPath] = []
        
        #Okay. At this point, we now have a list of names HLT paths that have fired in this event
        #A list of names of L1 paths that have passed in the event
        #And a map that maps between what HLT paths are seeded by what bits
        #Now we just need to go through, and figure out, on a per bit basis, what L1T paths have HLT seeds in this event
        #And vice versa
        for hltPath in HLT_pass:
            #what bits make up this HLT path?
            try:
                L1Bits = HLT_L1_seeds[str(run)][hltPath]
            #theoretically, we might have missed something, if we did, just ignore it
            except KeyError:
                print(f'Failed to find a corresponding entry for Run: {run} and HLT Path: {hltPath} in the HLT-L1 bit mapping, continuing on...')
                continue
            L1Bits = [x for x in L1Bits if x in L1T_pass]
            HLT_to_L1T_Dict[hltPath] = L1Bits
            for bitName in L1Bits:
                if bitName in L1T_pass:
                    L1T_to_HLT_Dict[bitName].append(hltPath)
        
        #Okay, Now we start filling histograms
        for group in L1T_unpr.keys():
            for iL1T in range(len(L1T_unpr[group])):
                seed = L1T_unpr[group][iL1T]

                ## Require that this path fired
                if not seed in L1T_pass: continue

                hists['L1T_%s_rate_total' % group].Fill(iL1T+1)
                hists['L1T_%s_rate_pure'  % group].Fill(iL1T+1, (len(L1T_pass) == 1))
                hists['L1T_%s_rate_prop'  % group].Fill(iL1T+1, 1.0 / len(L1T_pass))

                if len(L1T_to_HLT_Dict[seed]) == 0: continue

                #Quote TriggerAcceptance.py:
                ## For each fired HLT path seeded by this L1T path, count the number of other
                ## L1T paths seeding this HLT path which also fired.  Find the minimum number.
                nAcc_L1T = 999
                for path in L1T_to_HLT_Dict[seed]:
                    nAcc_L1T = min(nAcc_L1T, len(HLT_to_L1T_Dict[path]))

                if nAcc_L1T == 0 or nAcc_L1T == 999:
                    print ('\n\nBizzare error!!! nAcc_L1T = %d! Quitting.' % nAcc_L1T)
                    sys.exit()
                    
                hists['L1T_%s_acc_rate_total' % group].Fill(iL1T+1)
                hists['L1T_%s_acc_rate_pure'  % group].Fill(iL1T+1, (nAcc_L1T == 1))
                hists['L1T_%s_acc_rate_prop'  % group].Fill(iL1T+1, 1.0 / nAcc_L1T)

    #This is just a straight rip from TriggerAcceptance.py to handle the final bit of output 
    #formatting and writing
    ## Configure the rate histograms
    for hname in hists.keys():
        hists[hname].SetStats(0)
        ## Label the L1T and HLT histogram axes
        if hname.startswith('HLT') and hists[hname].GetNbinsX() == len(HLT_Paths):
            for iHLT in range(len(HLT_Paths)):
                hists[hname].GetXaxis().SetBinLabel(iHLT+1, HLT_Paths[iHLT])
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
    outputFile.cd()

    ## Create the canvas for drawing histograms
    c0 = ROOT.TCanvas('c0')
    c0.cd()


    ## Draw the number of primary vertices
    legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
    hists['nPV_good'].SetLineWidth(2)
    hists['nPV_good'].SetLineColor(ROOT.kBlue)
    hists['nPV_good'].Draw('hist')
    legend.AddEntry(hists['nPV_good'], 'Good primary vertices', 'l')
    legend.Draw()
    hists['nPV_good'].Write()
    hists['nPV'].SetLineWidth(2)
    hists['nPV'].SetLineColor(ROOT.kBlack)
    hists['nPV'].Draw('histsame')
    legend.AddEntry(hists['nPV'], 'Primary vertices', 'l')
    legend.Draw()
    hists['nPV'].Write()
    c0.SaveAs(png_dir+'h_nPV.png')
    c0.SaveAs(pdf_dir+'h_nPV.pdf')


    ## Draw the HLT rate histograms

    hists['HLT_rate_total'].SetLineWidth(2)
    hists['HLT_rate_total'].SetLineColor(ROOT.kBlack)
    hists['HLT_rate_total'].Draw('hist')
    hists['HLT_rate_total'].Write()
    c0.SaveAs(png_dir+'h_HLT_rate.png')
    c0.SaveAs(pdf_dir+'h_HLT_rate.pdf')

    ## Draw the L1T rate and acceptance histograms for each group
    for group in L1T_unpr.keys():

        ## Raw L1T rate histograms, including purity w.r.t. other L1T seeds
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_rate_total' % group].SetLineColor(ROOT.kBlack)
        hists['L1T_%s_rate_total' % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_total' % group], 'L1T total rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_total' % group].Write()
        hists['L1T_%s_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_prop'  % group].SetLineColor(ROOT.kBlue)
        hists['L1T_%s_rate_prop'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_rate_prop' % group], 'L1T prop. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_prop'  % group].Write()
        hists['L1T_%s_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_rate_pure'  % group].SetLineColor(ROOT.kRed)
        hists['L1T_%s_rate_pure'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_rate_pure' % group], 'L1T pure rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_pure'  % group].Write()
        hists['L1T_%s_rate_total' % group].SetTitle('L1T rates')

        c0.SaveAs(png_dir+'h_L1T_%s_rate.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate.pdf' % group)

        ## HLT acceptance rate histograms, including purity w.r.t. HLT acceptance
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_acc_rate_total' % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(ROOT.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(ROOT.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].SetLineWidth(2)
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(ROOT.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T HLT-accepted rates')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_rate.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_acc_rate.pdf' % group)

        ## Total rate and HLT acceptance overlaid
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_total'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_total' % group], 'L1T total rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(ROOT.kViolet)
        hists['L1T_%s_acc_rate_total' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_total'     % group].SetTitle('L1T net and HLT-accepted total rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_total.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_total.pdf' % group)

        ## Proportional rate and HLT acceptance overlaid
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_prop'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop' % group].SetLineColor(ROOT.kGreen)
        hists['L1T_%s_acc_rate_prop' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_prop'     % group].SetTitle('L1T net and HLT-accepted proportional rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_prop.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_prop.pdf' % group)

        ## Pure rate and HLT acceptance overlaid
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_rate_pure'     % group].Draw('hist')
        legend.AddEntry(hists['L1T_%s_rate_pure' % group], 'L1T pure rate', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure' % group].SetLineColor(ROOT.kMagenta)
        hists['L1T_%s_acc_rate_pure' % group].Draw('histsame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure acc. rate', 'l')
        legend.Draw()
        hists['L1T_%s_rate_pure'     % group].SetTitle('L1T net and HLT-accepted pure rates')
        c0.SaveAs(png_dir+'h_L1T_%s_rate_acc_pure.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_rate_acc_pure.pdf' % group)

        ## HLT acceptance fraction histograms, including purity w.r.t. HLT acceptance
        legend = ROOT.TLegend(0.78,0.78,0.98,0.93)
        hists['L1T_%s_acc_rate_total' % group].Divide  (hists['L1T_%s_rate_total' % group])
        hists['L1T_%s_acc_rate_total' % group].SetName ('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].SetTitle('h_L1T_%s_acc_frac_total' % group )
        hists['L1T_%s_acc_rate_total' % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_total' % group].SetLineColor(ROOT.kBlack)
        hists['L1T_%s_acc_rate_total' % group].Draw('histe')
        legend.AddEntry(hists['L1T_%s_acc_rate_total' % group], 'L1T total fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_total' % group].Write()
        hists['L1T_%s_acc_rate_prop'  % group].Divide  (hists['L1T_%s_rate_prop' % group])
        hists['L1T_%s_acc_rate_prop'  % group].SetName ('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].SetTitle('h_L1T_%s_acc_frac_prop' % group )
        hists['L1T_%s_acc_rate_prop'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_prop'  % group].SetLineColor(ROOT.kBlue)
        hists['L1T_%s_acc_rate_prop'  % group].Draw('histesame')
        legend.AddEntry(hists['L1T_%s_acc_rate_prop' % group], 'L1T prop. fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_prop'  % group].Write()
        hists['L1T_%s_acc_rate_pure'  % group].Divide  (hists['L1T_%s_rate_pure' % group])
        hists['L1T_%s_acc_rate_pure'  % group].SetName ('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].SetTitle('h_L1T_%s_acc_frac_pure' % group )
        hists['L1T_%s_acc_rate_pure'  % group].GetYaxis().SetTitle('Fraction')
        hists['L1T_%s_acc_rate_pure'  % group].SetLineColor(ROOT.kRed)
        hists['L1T_%s_acc_rate_pure'  % group].Draw('histesame')
        legend.AddEntry(hists['L1T_%s_acc_rate_pure' % group], 'L1T pure fraction', 'l')
        legend.Draw()
        hists['L1T_%s_acc_rate_pure'  % group].Write()
        hists['L1T_%s_acc_rate_total' % group].SetTitle('L1T fraction of HLT-accepted events')
        c0.SaveAs(png_dir+'h_L1T_%s_acc_frac.png' % group)
        c0.SaveAs(pdf_dir+'h_L1T_%s_acc_frac.pdf' % group)

if __name__=='__main__':
    parser = argparse.ArgumentParser(description='A script to compute the fraction of passing Level-1 Trigger paths accepted by HLT')
    parser.add_argument(
        '--ListOfFiles',
        nargs='?',
        required=True,
        help='(CSV) List of L1Ntuple files to generate HLT-L1 bit corresponances with'
    )
    parser.add_argument(
        '--Redirector',
        nargs='?',
        default='',
        help='Redirector for the root files, if necessary'
    )
    parser.add_argument(
        '--Seeds',
        nargs='?',
        default='hlt_l1_seeds/hlt_l1.json',
        help='JSON containing L1+HLT information created in the first step'
    )
    parser.add_argument(
        '--OutputDir',
        nargs='?',
        default='plots/',
        help='Location to store script output'
    )
    parser.add_argument(
        '--L1OMSOutput',
        nargs='*',
        required=True,
        help='txt file containing the output of OMS L1 algo report to map L1 bits to their names (run number required in file name)'
    )

    args=parser.parse_args()

    main(args)