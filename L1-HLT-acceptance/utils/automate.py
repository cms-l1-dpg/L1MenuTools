from subprocess import Popen,PIPE
import ROOT as R
import os
import time


def hlt_GetConfig(directory, rootfiles):
    for rootfile in rootfiles:

        runs = []

        f = R.TFile.Open(rootfile)
        branch = f.Get('Runs')
        for irun in range(branch.GetEntries()):
            branch.GetEntry(irun)
            runs.append(str(branch.GetLeaf('run').GetValue()).split('.')[0])

        if not os.path.exists(directory):
            os.makedirs(directory)

        commands = []

        for run in runs:
            command = 'hltGetConfiguration run:' + run + '>' + directory +run + '.py'
            commands.append(command)

        start = time.time()
        # run in parallel
        processes = [Popen(cmd, shell=True,stdout=PIPE, stderr=PIPE) for cmd in commands]

        # wait for completion

        for p in processes:
            p.wait()

	print('HLT menu generation process took ' + str(time.time()-start) + ' seconds')
        print('HLT menus stored in '+ directory)
