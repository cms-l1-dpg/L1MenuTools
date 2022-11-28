#!/usr/bin/env python
import os, re, sys, commands, math, time, calendar

print '\nSTART\n'
ts = calendar.timegm(time.gmtime())

fileName = "L1Ntuple.root"
jobName = "menu_NuGun_12_6_X"
jobScript = "cmsRun.sh"
#jobScript = "NuGun_122X_cmsRun.sh" # New NuGun_E10 samples in 122X
jobCfg = "mc.py"
rel = "CMSSW_12_6_0_pre1"
eosDir = "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/" + os.environ["USER"] + "/condor/" + jobName + "_" + str(ts) + "/"
rootDir = os.environ["CMSSW_BASE"] + "/src/L1MenuTools/L1Ntuples/"
jobDir = rootDir + jobName + "_" + str(ts) + "/"
ret = 0

## NOTE for the 122X NuGun_E10 samples: For this submission, use NuGun_122X_cmsRun.sh as job script
fileList = rootDir + "122X_NuGun_E10_mcRun3_realisticPU.list" 
nEvents = 14262400 # New NuGun_E10 samples in 122X                                                                   
nJobs = 8914       # New NuGun_E10 samples in 122X - Tot files: nJobs*4=35656 (grouped in set of four for the submission) 
## NOTE for the 122X NuGun_Pt2-20 samples: For this submission, use NuGun_122X_cmsRun.sh as job script
#fileList = rootDir + "122X_NuGun_Pt2-20_mcRun3_realisticPU.list" 
#nEvents = 12764800 # New NuGun_Pt2-20 samples in 122X                                                                   
#nJobs = 7978       # New NuGun_Pt2-20 samples in 122X - Tot files: nJobs*4=31912 (grouped in set of four for the submission) 

#fileList = rootDir + "nugun_Pt2-20.list" 
#nEvents = 1000000  #NuGun_Pt_2-20                                                                         
#nJobs = 1250      #NuGun_Pt_2-20                                                                                                                             
#fileList = rootDir + "nugun_E10.list"
#nEvents = 1000000  #NuGun_E10                                                                                                                                                
#nJobs = 1250      #NuGun_E10  

while ret == 0:
   ret = os.system("mkdir " + jobDir)
   ret = os.system("mkdir " + jobDir + "out/")
   ret = os.system("mkdir " + jobDir + "err/")
   ret = os.system("mkdir " + jobDir + "log/")
   ret = os.system("mkdir " + eosDir)
   ret = os.chdir(os.environ["CMSSW_BASE"]+"/../")
   ret = os.system("tar --exclude='L1Ntuple.root' --exclude='ignore' --exclude='.git' " + 
                   "-zcf " + jobName + ".tgz " + rel)
   ret = os.system("mv " + jobName + ".tgz " + eosDir) 
   ret = os.chdir(rootDir)

   with open(jobDir + jobName + '.jdl', 'w') as jdl:
      jdl.write("universe = vanilla\n")
      jdl.write("x509userproxy = $ENV(X509_USER_PROXY)\n")
      jdl.write("Executable = " + jobScript + "\n")
      jdl.write("Should_Transfer_Files = YES\n")
      jdl.write("WhenToTransferOutput = ON_EXIT\n")
      jdl.write("Transfer_Input_Files = " + jobScript + ", " + jobCfg + "\n")
      jdl.write("Output = "    + jobDir + "out/$(ProcId).out\n")
      jdl.write("Error = "     + jobDir + "err/$(ProcId).err\n")
      jdl.write("Log = "       + jobDir + "log/$(ProcId).log\n")
      jdl.write("Arguments = " + eosDir + " " + jobName + " " + rel + " " + 
                " $(ProcId) " + str(nEvents) + " " + jobCfg + " " + fileList + " " + fileName + "\n")
      jdl.write("+MaxRuntime = 28800\n")
      jdl.write("on_exit_remove = (ExitBySignal == False) && (ExitCode == 0)\n")
      jdl.write("max_retries = 3\n")
      jdl.write("requirements = Machine =!= LastRemoteHost\n")
      jdl.write("Queue " + str(nJobs) + "\n")      

   os.system("condor_submit " + jobDir + jobName + ".jdl")
   print str(nJobs) + " jobs submitted."
   print "\nYour jobs:"
   os.system("condor_q")
   print
   sys.exit(0)

print("Submission failed.")
sys.exit(1)
