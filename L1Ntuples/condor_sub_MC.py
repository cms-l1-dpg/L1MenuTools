#!/usr/bin/env python
import os, re, sys, math, time, calendar

print('\nSTART\n')
ts = calendar.timegm(time.gmtime())

fileName = "L1Ntuple.root"
jobName = "NuGun_E10_Winter25_15_0_3"
#jobName = "EphZB_2024I_run386593_14_0_13_menuv130_HCALtest"
jobScript = "cmsRun.sh"
jobCfg = "mc.py"
rel = "CMSSW_15_0_3"
eosDir = "/eos/cms/store/group/dpg_trigger/comm_trigger/L1Trigger/" + os.environ["USER"] + "/condor/" + jobName + "_" + str(ts) + "/"
rootDir = os.environ["CMSSW_BASE"] + "/src/L1MenuTools/L1Ntuples/"
jobDir = rootDir + jobName + "_" + str(ts) + "/"
ret = 0

### Settings to run on NuGun E10 Winter25 MC campaign
fileList = rootDir + "NuGun_E10_Winter25.list"
nEvents = 9967200
nJobs = 4183

### Settings to run on EphZB2024I (run 386593)
#fileList = rootDir + "EphZB_run386593_short.list"
#nEvents = 12072 
#nJobs = 3
#nEvents = 10719500
#nJobs = 2664

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
