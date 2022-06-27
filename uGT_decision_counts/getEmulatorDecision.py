from ROOT import *
from algo_map import algo_map

import sys

helpstr = '\nUsage : python3 getEmulatorDecision.py <L1Ntuple>\n'
helpstr += '\n<L1Ntuple> : location of the L1Ntuple.root file\n'

if len(sys.argv) != 2 : sys.exit("\nError : Missing ntuple location\n"+helpstr)

# Input file name
file_in_name = sys.argv[1]
fin = TFile(file_in_name,"READ")
#fin = TFile("/afs/cern.ch/user/s/sonawane/private/ntuple_test/CMSSW_12_4_0_pre4/src/L1Ntuple.root","READ")

#t_L1UpgradeTree = fin.Get("l1UpgradeTree/L1UpgradeTree")
#t_l1uGTEmuTree.AddFriend("l1uGTTree/L1uGTTree")
t_l1uGTEmuTree = fin.Get("l1uGTEmuTree/L1uGTTree")

for bit, algo_name in algo_map:
        s_algoSelection = "m_algoDecisionFinal[" + str(bit) + "]==1"
        #print(s_algoSelection)
        nentriesPerBit = t_l1uGTEmuTree.GetEntries(s_algoSelection)
        print(bit, '\t', algo_name, '\t', nentriesPerBit)
