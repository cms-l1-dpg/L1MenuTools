#!/usr/bin/env python
# encoding: utf-8

# File        : GetLumi.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2016 Aug 10
#
# Description :

import subprocess
import pandas as pd
import sys
if sys.version_info[0] < 3:
    from StringIO import StringIO
else:
    from io import StringIO

runlist = [

#	two runs in Fill 7118
	322079,
	322088
]

def Runcmd(run):
    # cmd = "brilcalc lumi --byls"
    cmd = "brilcalc lumi --byls -u '1e30/cm2s' "
    cmd += " --output-style csv -b 'STABLE BEAMS' "
    cmd += " -r %d " % run
    #testcmd = cmd + " --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json "
    #testcmd = cmd + " -i /afs/cern.ch/user/d/deguio/public/Certification/Cert_13TeV_2017_HCAL_DCS_GOOD.txt "
    testcmd = cmd + " -i json_DCSONLY.txt "
    pipe = subprocess.Popen(testcmd, shell=True, stdout=subprocess.PIPE)
    out, err = pipe.communicate()
    if len(out) == 221: # Empty output
        pipe = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        out, err = pipe.communicate()
    Pandas(out)

def Pandas(file):
    # df = pd.read_csv(file, header=1, skipfooter=3)
    # df = pd.read_csv(StringIO(file), header=1, skipfooter=3)
    df = pd.read_csv(StringIO(file), header=1, skipfooter=3, engine='python')
    df.loc[:,"run"] = df["#run:fill"].str.split(':').str[0]
    df.loc[:,"fill"] = df["#run:fill"].str.split(':').str[1]
    df.loc[:,"LS"] = df["ls"].str.split(':').str[0]
    df2=df[['fill','run','LS', 'avgpu']]
    df2.to_csv('run_lumi.csv', mode='a', header=False, index=False)
    # df2.to_csv('run_lumi_old.csv', mode='a', header=False, index=False)
    # df.to_csv('lumi_new.csv', mode='a', header=False, index=False)

if __name__ == "__main__":
    for run in runlist:
        Runcmd(run)

