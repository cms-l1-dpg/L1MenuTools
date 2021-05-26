# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple -s RAW2DIGI --python_filename=mc.py -n -1 --no_output --era=Run3 --mc --conditions=112X_mcRun3_2021_realistic_v15 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAWSimHcalTP --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_3 --filein=/store/mc/Run3Winter20DRPremixMiniAOD/Nu_E10-pythia8-gun/GEN-SIM-RAW/SNB_110X_mcRun3_2021_realistic_v6-v1/10000/51FBC4DC-D5C7-824F-9AFA-3025F04F96FA.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3
import FWCore.ParameterSet.VarParsing as VarParsing # ADDED
import FWCore.Utilities.FileUtils as FileUtils # ADDED

process = cms.Process('RAW2DIGI',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# ADDED 

options = VarParsing.VarParsing ('analysis')
# get and parse the command line arguments

options.register('skipEvents',
                 0,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")
options.register('outFile',
                 'L1Ntuple.root',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Output file')
options.register('inputFile',
                 'file:ttMTDPU200.root',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 'Input file')
                                  

options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.suppressWarning = cms.untracked.vstring(
    'l1PhaseIITree', 'l1UpgradeTfMuonEmuTree', 'l1CaloTowerTree', 
    'l1UpgradeTfMuonTree','l1UpgradeTree','l1HOTree', 'l1Phase2CaloTree')

#fileList = FileUtils.loadListFromFile(options.inputFile)
#readFiles = cms.untracked.vstring(*fileList)

# Input source
process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(options.skipEvents), #added
    fileNames = cms.untracked.vstring(options.inputFile),
    secondaryFileNames = cms.untracked.vstring()
)

# end ADDED

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(1)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1Ntuple nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '112X_mcRun3_2021_realistic_v15', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulMCFromRAWSimHcalTP 

#call to customisation function L1TReEmulMCFromRAWSimHcalTP imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulMCFromRAWSimHcalTP(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMU 

#call to customisation function L1NtupleRAWEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleRAWEMU(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseSettings
from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParams_2018_v1_3 

#call to customisation function L1TSettingsToCaloParams_2018_v1_3 imported from L1Trigger.Configuration.customiseSettings
process = L1TSettingsToCaloParams_2018_v1_3(process)

# End of customisation functions


# Customisation from command line
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

process.simHcalTriggerPrimitiveDigis.numberOfFilterPresamplesHBQIE11 = 1
process.simHcalTriggerPrimitiveDigis.numberOfFilterPresamplesHEQIE11 = 1
process.simHcalTriggerPrimitiveDigis.weightsQIE11 = {
    "ieta1" :  [-0.47, 1.0],
    "ieta2" :  [-0.47, 1.0],
    "ieta3" :  [-0.47, 1.0],
    "ieta4" :  [-0.47, 1.0],
    "ieta5" :  [-0.47, 1.0],
    "ieta6" :  [-0.47, 1.0],
    "ieta7" :  [-0.47, 1.0],
    "ieta8" :  [-0.47, 1.0],
    "ieta9" :  [-0.47, 1.0],
    "ieta10" : [-0.47, 1.0],
    "ieta11" : [-0.47, 1.0],
    "ieta12" : [-0.47, 1.0],
    "ieta13" : [-0.47, 1.0],
    "ieta14" : [-0.47, 1.0],
    "ieta15" : [-0.47, 1.0],
    "ieta16" : [-0.47, 1.0],
    "ieta17" : [-0.47, 1.0],
    "ieta18" : [-0.47, 1.0],
    "ieta19" : [-0.47, 1.0],
    "ieta20" : [-0.47, 1.0],
    "ieta21" : [-0.43, 1.0],
    "ieta22" : [-0.43, 1.0],
    "ieta23" : [-0.43, 1.0],
    "ieta24" : [-0.43, 1.0],
    "ieta25" : [-0.43, 1.0],
    "ieta26" : [-0.43, 1.0],
    "ieta27" : [-0.43, 1.0],
    "ieta28" : [-0.43, 1.0]
}

process.HcalTPGCoderULUT.contain1TSHB = True
process.HcalTPGCoderULUT.contain1TSHE = True

# Pick one of the pairs of lines below based on the intended scenario for running
process.HcalTPGCoderULUT.containPhaseNSHB = 1.0 # For Run3 MC
process.HcalTPGCoderULUT.containPhaseNSHE = 1.0 # For Run3 MC

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
