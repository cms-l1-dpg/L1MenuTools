import os
import json
import subprocess

era = "G"
run = "398183"

nevents = 0
nfiles = 0

for i in range(8):
    os.system('dasgoclient --query="file dataset="/EphemeralZeroBias'+str(i)+'/Run2025'+era+'-v1/RAW" run='+run+'" >> EphZB_run'+run+'.list')
    os.system('dasgoclient --query="file dataset="/EphemeralZeroBias'+str(i)+'/Run2025'+era+'-v1/RAW" run='+run+'" -json >> EphZB_run'+run+'_'+str(i)+'.json')

    # Get nEvents using json output of dasgoclient
    data = json.load(open('EphZB_run'+run+'_'+str(i)+'.json'))
    jtopy = json.dumps(data) #json.dumps take a dictionary as input and returns a string as output.
    dict_json = json.loads(jtopy) # json.loads take a string as input and returns a dictionary as output.


    for d in dict_json:
        nevents += d['file'][0]['nevents']

    nfiles += len(dict_json)
    os.system('rm EphZB_run'+run+'_'+str(i)+'.json')

print("there are " + str(nfiles) + " files for run " + run)
print("for the files in run "+run+", nevents = ", nevents)
