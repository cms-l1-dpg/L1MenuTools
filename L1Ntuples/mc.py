# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: cmsDriver.py l1Ntuple -s RAW2DIGI --python_filename=mc_126X.py -n -1 --no_output --era=Run3 --mc --conditions=131X_mcRun3_2023_realistic_v5 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAWSimHcalTP --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2023_v0_3 --filein=/store/mc/Run3Winter23Digi/TT_TuneCP5_13p6TeV_powheg-pythia8/GEN-SIM-RAW/126X_mcRun3_2023_forPU65_v1_ext1-v2/40002/cbcb2b23-174a-4e7f-a385-152d9c5c5b87.root

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

options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.suppressWarning = cms.untracked.vstring(
    'l1PhaseIITree', 'l1UpgradeTfMuonEmuTree', 'l1CaloTowerTree',
    'l1UpgradeTfMuonTree','l1UpgradeTree','l1HOTree', 'l1Phase2CaloTree')

# Input source                                                                                                                                  
process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(options.skipEvents), #added                                                          
    fileNames = cms.untracked.vstring(options.inputFiles),
    secondaryFileNames = cms.untracked.vstring()
)
# end ADDED  

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
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
process.GlobalTag = GlobalTag(process.GlobalTag, '131X_mcRun3_2023_realistic_v5', '')

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
from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParams_2023_v0_3 

#call to customisation function L1TSettingsToCaloParams_2023_v0_3 imported from L1Trigger.Configuration.customiseSettings
process = L1TSettingsToCaloParams_2023_v0_3(process)

# OMTF/EMTF Cross-cleaning parameters. This is "conservative"                                                  
process.gmtParams.FOPosMatchQualLUTMaxDR        = cms.double(0.2)
process.gmtParams.FOPosMatchQualLUTfEta         = cms.double(1)
process.gmtParams.FOPosMatchQualLUTfEtaCoarse   = cms.double(1)
process.gmtParams.FOPosMatchQualLUTfPhi         = cms.double(2)
process.gmtParams.FONegMatchQualLUTMaxDR        = cms.double(0.2)
process.gmtParams.FONegMatchQualLUTfEta         = cms.double(1)
process.gmtParams.FONegMatchQualLUTfEtaCoarse   = cms.double(1)
process.gmtParams.FONegMatchQualLUTfPhi         = cms.double(2)
process.gmtParams.fwVersion = cms.uint32(0x7000000)

# End of customisation functions


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
