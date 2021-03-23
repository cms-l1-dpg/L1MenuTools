from subprocess import Popen,PIPE
import ROOT as R
import os
import time


def hlt_GetConfig(directory, txtfile, MAXFILES, redirector):
    '''
    Creates menus using the hltGetConfiguration command of the cmsenv

    directory: directory where the output python files are written to
    txtfile: textfile containing the names of the ROOT files to extract runs from
    MAXFILES: number of files to loop through
    redirector: the redirector with which the ROOT files are loaded
    '''

    start = time.time()

    ## Gather ROOT file names from txtfile

    rootfiles = []
    for fdir in open(txtfile,'r'):
        rootfiles.append(fdir.split('\n')[0])
        if len(rootfiles)>MAXFILES:break
    
    ## Get runs from ROOT files

    runs = []
    for rootfile in rootfiles:
        f = R.TFile.Open(redirector + rootfile)
        branch = f.Get('Runs')
        for irun in range(branch.GetEntries()):
            branch.GetEntry(irun)
            runs.append(str(branch.GetLeaf('run').GetValue()).split('.')[0])

    runs = set(runs)

    ## Create temporary directory if necessary

    if not os.path.exists(directory):
        os.makedirs(directory)
    
    ## Create list of hltGetConfiguration commands

    commands = []
    for run in runs:
        command = 'hltGetConfiguration run:' + run + '>' + directory +run + '.py'
        commands.append(command)

    ## run in parallel

    processes = [Popen(cmd, shell=True,stdout=PIPE, stderr=PIPE) for cmd in commands]

    ## wait for completion

    for p in processes:
        p.wait()

    print('HLT menu generation process took ' + str(time.time()-start) + ' seconds')
    print('HLT menus stored in '+ directory)
