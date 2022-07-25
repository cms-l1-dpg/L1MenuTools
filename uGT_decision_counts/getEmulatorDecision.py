from ROOT import *
from algo_map import algo_map

import sys
import os

helpstr = '\nUsage : python3 getEmulatorDecision.py\n'

# Input files
file_list = open("ntuples.list")
lines = file_list.readlines()

ntuple_list = []

for line in lines:
	ntuple_path = line.rstrip()
	if os.path.exists(ntuple_path) : ntuple_list.append(ntuple_path)

chain = {}

chain['uGTEmuTree'] =  TChain('l1uGTEmuTree/L1uGTTree')
#chain['UpgradeTree'] =  TChain('l1UpgradeTree/L1UpgradeTree')
#chain['uGTTree'] =  TChain('l1uGTTree/L1uGTTree')

for ntuple_path in ntuple_list :
	chain['uGTEmuTree'].Add(ntuple_path)
#	chain['UpgradeTree'].Add(ntuple_path)
#	chain['uGTTree'].Add(ntuple_path)

for bit, algo_name in algo_map:
        s_algoSelection = "m_algoDecisionFinal[" + str(bit) + "]==1"
       #print(s_algoSelection)
        nentriesPerBit = chain['uGTEmuTree'].GetEntries(s_algoSelection)
        print("{}  \t {:<40} \t {}".format(bit, algo_name, nentriesPerBit))
