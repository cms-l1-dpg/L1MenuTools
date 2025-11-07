#!/usr/bin/env python
import os, re, sys, math, time, calendar

print('\nSTART\n')
ts = calendar.timegm(time.gmtime())

fileName = "L1Ntuple.root"
jobName = "Nominal_EphZB_2025G_run398183_15_0_10_menu2025_v130"
jobScript = "cmsRun.sh"
#jobCfg = "mc.py"
jobCfg = "data.py"
#jobCfg = "data.py"
rel = os.environ.get("CMSSW_VERSION", "Unknown")
eosDir = "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/" + os.environ["USER"] + "/condor/" + jobName + "_" + str(ts) + "/"
rootDir = os.environ["CMSSW_BASE"] + "/src/L1MenuTools/L1Ntuples/"
jobDir = rootDir + jobName + "_" + str(ts) + "/"
ret = 0

### Settings to run on EphZB2024I (run 386593)
#fileList = rootDir + "EphZB_run386593_all.list"
#nEvents = 10719500
#nJobs = 2664

### Settings to run on EphZB2025C (run 392295)
# fileList = rootDir + "EphZB_run392295.list"
# nEvents = 10854637
# nJobs = 3812

### Settings to run on EphZB2025D (run 394595)
# fileList = rootDir + "EphZB_run394959.list"
# nEvents = 11206301
# nJobs = 3878

# Settings to run on EphZB2025D (run 394595)
# fileList = rootDir + "EphZB_run396102.list"
# nEvents = 10363769
# nJobs = 3677

fileList = rootDir + "EphZB_run398183.list"
nEvents = 10523867
nJobs = 3702

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
