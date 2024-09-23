#!/usr/bin/env python
import os, re, sys, math, time, calendar

print('\nSTART\n')
ts = calendar.timegm(time.gmtime())

fileName = "L1Ntuple.root"
#jobName = "EphZB_2023D_run370293_13_3_0"
#jobName = "EphZB_2024F_run382250_all_14_0_10_menuv130"
jobName = "EphZB_2024G_run385127_all_14_0_13_menuv130"
jobScript = "cmsRun.sh"
#jobCfg = "mc.py"
jobCfg = "data.py"
rel = "CMSSW_14_0_13"
eosDir = "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/" + os.environ["USER"] + "/condor/" + jobName + "_" + str(ts) + "/"
rootDir = os.environ["CMSSW_BASE"] + "/src/L1MenuTools/L1Ntuples/"
jobDir = rootDir + jobName + "_" + str(ts) + "/"
ret = 0

### Settings to run on EphZB2023D (run 370293)
#fileList = rootDir + "EphZB_run370293_all.list"
#nEvents = 3148150
#nJobs = 735

### Settings to run on EphZB2024F (run 382250)
#fileList = rootDir + "EphZB_run382250_all.list"
#nEvents = 15621250
#nJobs = 3696 

### Settings to run on EphZB2024G (run 385127)
fileList = rootDir + "EphZB_run385127_all.list"
nEvents = 12800000
nJobs = 3096


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
   print(str(nJobs) + " jobs submitted.")
   print("\nYour jobs:")
   os.system("condor_q")
   print(sys.exit(0))

print("Submission failed.")
sys.exit(1)
