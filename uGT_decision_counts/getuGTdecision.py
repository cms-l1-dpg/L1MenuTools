from ROOT import *
from algo_map import algo_map

import argparse
import sys
import os

argparser = argparse.ArgumentParser(description='Parser used for non default arguments', formatter_class=argparse.ArgumentDefaultsHelpFormatter, add_help=True)
argparser.add_argument('--ntuples'      , dest='ntuple_list'     , default='ntuples.list'   , help='List of ntuples')
argparser.add_argument('--selections'   , dest='selections'      , default=''               , help='Additional selections')

args = argparser.parse_args()
 
selection_str = args.selections

# Input files
file_list = open(args.ntuple_list)
lines = file_list.readlines()

outfile = open("output.txt","w")

ntuple_list = []

for line in lines:
	ntuple_path = line.rstrip()
	if os.path.exists(ntuple_path) : ntuple_list.append(ntuple_path)

chain = {}

#chain['uGTEmuTree'] =  TChain('l1uGTEmuTree/L1uGTTree')                # Emulated trigger decision
#chain['UpgradeTree'] =  TChain('l1UpgradeTree/L1UpgradeTree')
chain['uGTTree'] =  TChain('l1uGTTree/L1uGTTree')                       # Actual trigger decision saved in L1Ntuple
chain['EventTree'] =  TChain('l1EventTree/L1EventTree')                 # Event information saved in this tree, allows to add selections on Run, event and lumi

for ntuple_path in ntuple_list :
#	chain['uGTEmuTree'].Add(ntuple_path)
#	chain['UpgradeTree'].Add(ntuple_path)
	chain['uGTTree'].Add(ntuple_path)
	chain['EventTree'].Add(ntuple_path)

chain['uGTTree'].AddFriend(chain['EventTree'])

tot_str = "Total Events : "+ str(chain['uGTTree'].GetEntries())
tot_sel_str = "Total Events ({}) : {}".format("No selection" if not selection_str else selection_str, chain['uGTTree'].GetEntries(selection_str))

print(tot_str)
print(tot_sel_str)
outfile.write(tot_str+"\n")
outfile.write(tot_sel_str+"\n")

for bit, algo_name in algo_map:
       #print(s_algoSelection)

        s_algoSelection = "m_algoDecisionInitial[" + str(bit) + "]"
        if selection_str : s_algoSelection+= "&&"+selection_str

        nentriesPerBit = chain['uGTTree'].GetEntries(s_algoSelection)
        print("{}  {}  {}".format(bit, algo_name, nentriesPerBit))
        outfile.write("{}  {}  {}\n".format(bit, algo_name, nentriesPerBit))
