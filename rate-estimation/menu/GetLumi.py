#!/usr/bin/env python3
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
import io

runlist = [
    # High PU fill                                                                                                                                                                         
    362433, 362434, 362435, 362436, 362437, 362438, 362439
]

def Runcmd(run):
    cmd = "brilcalc lumi --byls -u '1e30/cm2s' "
    cmd += " --output-style csv -b 'STABLE BEAMS' "
    cmd += " -r %d " % run
    testcmd = cmd + " -i Cert_Collisions2022_355100_362760_eraBCDEFG_13p6TeV_DCSOnly_TkPx.json "
    pipe = subprocess.Popen(testcmd, shell=True, stdout=subprocess.PIPE)
    out, err = pipe.communicate()
    if len(out) == 221: # Empty output
        pipe = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        out, err = pipe.communicate()
    Pandas(out)

def Pandas(file):
    df = pd.read_csv(io.BytesIO(file), header=1, skipfooter=3, engine='python')
    df.loc[:,"run"] = df["#run:fill"].str.split(':').str[0]
    df.loc[:,"fill"] = df["#run:fill"].str.split(':').str[1]
    df.loc[:,"LS"] = df["ls"].str.split(':').str[0]
    df2=df[['fill','run','LS', 'avgpu']]
    df2.to_csv('run_lumi.csv', mode='a', header=False, index=False)

if __name__ == "__main__":
    for run in runlist:
        Runcmd(run)

