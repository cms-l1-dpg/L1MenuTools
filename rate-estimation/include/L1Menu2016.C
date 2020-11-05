// ===========================================================================
// 
//       Filename:  L1Menu2016.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/13/2016 18:39:20
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "L1Menu2016.h"

//----------------------------------------------------------------------------
//       Class:  L1Menu2016
//      Method:  L1Menu2016
// Description:  constructor
//----------------------------------------------------------------------------
L1Menu2016::L1Menu2016 (std::string MenuName, std::string filelist):
  writefiles(true),writecsv(false),writeplots(true),
  menufilename(MenuName), 
  tuplefilename(filelist),
  scale(0),
  l1Plot(nullptr),
  l1TnP(nullptr),
  l1uGT(nullptr),
  l1unpackuGT(nullptr)
{
  InitConfig();
}  // -----  end of method L1Menu2016::L1Menu2016  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1Menu2016
//      Method:  ~L1Menu2016
// Description:  destructor
//----------------------------------------------------------------------------
L1Menu2016::~L1Menu2016 ()
{
  outfile->close();
  outcsv->close();
  outrootfile->Close();
  fChain->Reset();
  //delete outfile;
  //delete outcsv;
  //delete outrootfile;
}  // -----  end of method L1Menu2016::-L1Menu2016  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1Menu2016
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  L1Menu2016&
L1Menu2016::operator = ( const L1Menu2016 &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method L1Menu2016::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ConfigOutput
//  Description:  
// ===========================================================================
bool L1Menu2016::ConfigOutput(bool writetext_, bool writecsv_, bool writeplot_, 
    std::string outputdir_, std::string outputname_)
{
  writefiles = writetext_;
  writecsv = writecsv_;
  writeplots = writeplot_;
  outputdir = outputdir_;
  if (outputname_ == "Auto")
  {
    outputname = SetOutputName();
  }
  else
    outputname = outputname_;

  if (writefiles)
    outfile = new std::fstream( outputdir + "/" + outputname +".txt", std::fstream::out  );
  if (writecsv)
    outcsv = new std::fstream( outputdir + "/" + outputname +".csv", std::fstream::out  );
  if (writeplots)
  {
    std::string rootfilename = outputdir + "/" + outputname +".root";
    outrootfile =  new TFile( rootfilename.c_str(), "RECREATE");
  }
  return true;
}       // -----  end of function L1Menu2016::ConfigOutput  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::InitConfig
//  Description:  
// ===========================================================================
bool L1Menu2016::InitConfig()
{
  L1Config["SumJetET"]       = 0;
  L1Config["SumJetEta"]      = 999;
  L1Config["nBunches"]       = 2592; //default for 2017 nBunches
  L1Config["doPlotRate"]     = 0;
  L1Config["doPlotEff"]      = 0;
  L1Config["doPlotTest"]     = 0;
  L1Config["doPlotuGt"]      = 0;
  L1Config["doTnPMuon"]      = 0;
  L1Config["doPlotLS"]       = 0;
  L1Config["doPrintPU"]      = 0;
  L1Config["doPrintBX"]      = 0;
  L1Config["doCompuGT"]      = 0;
  L1Config["maxEvent"]       = -1;
  L1Config["SetMuonER"]      = -1;
  L1Config["SetNoPrescale"]  = 0;
  L1Config["IgnorePrescale"] = 0;
  L1Config["UseUpgradeLyr1"] = -1;
  L1Config["UseL1CaloTower"] = -1;
  L1Config["SelectFill"]     = -1;
  L1Config["SelectRun"]      = -1;
  L1Config["SelectEvent"]    = -1;
  L1Config["UsePFMETNoMuon"] = 0;
  L1Config["UseuGTDecision"] = 0;
  L1Config["UseUnpackTree"]  = 0;
  L1Config["doScanLS"]       = 0;
  L1Config["SetL1AcceptPS"]  = 0;
  L1Config["Select_BX_in_48b"]       = -1;
  L1Config["Select_BX_in_12b"]       = -1;
  L1Config["doBXReweight_1_to_6_47_48"]       = 0;
  L1Config["doBXReweight128"]       = 0;
  L1Config["doBXReweight34567"]       = 0;
  L1Config["doBXReweight_1_to_6_11_12"]       = 0;
  L1Config["doBXReweight_5_to_10"]       = 0;
  
  L1ConfigStr["SelectLS"] = "";
  L1ConfigStr["SelectBX"] = "";
  L1ConfigStr["Lumilist"] = "";
  L1ConfigStr["SelectCol"] = "";

  L1ObjectMap["Jet"]        = &L1Event.JetPt;
  L1ObjectMap["JetC"]       = &L1Event.JetCenPt;
  L1ObjectMap["Jeter3p0"]   = &L1Event.JetCenPt;
  L1ObjectMap["Tau"]        = &L1Event.TauPt;
  L1ObjectMap["Tauer"]      = &L1Event.TauerPt;
  L1ObjectMap["Tauer2p1"]   = &L1Event.TauerPt;
  L1ObjectMap["IsoTau"]     = &L1Event.IsoTauPt;
  L1ObjectMap["EG"]         = &L1Event.EGPt;
  L1ObjectMap["EGer"]       = &L1Event.EGerPt;
  L1ObjectMap["EGer2p1"]    = &L1Event.EGerPt;
  L1ObjectMap["IsoEG"]      = &L1Event.IsoEGPt;
  L1ObjectMap["IsoEGer"]    = &L1Event.IsoEGerPt;
  L1ObjectMap["IsoEGer2p1"] = &L1Event.IsoEGerPt;
  L1ObjectMap["Mu"]         = &L1Event.MuPt;
  L1ObjectMap["MuOpen"]     = &L1Event.MuOpenPt;
  L1ObjectMap["Muer"]       = &L1Event.MuerPt;
  L1ObjectMap["Muer2p1"]    = &L1Event.MuerPt;
  L1ObjectMap["HTT"]        = &L1Event.HTT;
  L1ObjectMap["HTTer"]      = &L1Event.HTT;
  L1ObjectMap["HTM"]        = &L1Event.HTM;
  L1ObjectMap["ETM"]        = &L1Event.ETM;
  L1ObjectMap["ETT"]        = &L1Event.ETT;
  L1ObjectMap["ETMHF"]      = &L1Event.ETMHF;
  L1ObjectMap["HTTHF"]      = &L1Event.HTTHF;


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Map to old func for now. ~~~~~
  // MutliJets
  L1SeedFun["L1_QuadJetC36_TauJet52"] = std::bind(&L1AlgoFactory::QuadJetCentral_TauJet, this, 36.,52., false, false);
  L1SeedFun["L1_QuadJetC36_Tau52"] = std::bind(&L1AlgoFactory::QuadJetCentral_TauJet, this, 36.,52., false, false);
  L1SeedFun["L1_QuadJet36er3p0_Tau52"] = std::bind(&L1AlgoFactory::QuadJetCentral_TauJet, this, 36.,52., false, false);

  // MultiMuon
  L1SeedFun["L1_DoubleMu0er1p6_dEta_Max1p8_OS"] = std::bind(&L1AlgoFactory::Onia2015, this, 0.,0.,true,true,18, 1.6);
  L1SeedFun["L1_DoubleMu0er1p6_dEta_Max1p8"] = std::bind(&L1AlgoFactory::Onia2015, this, 0.,0.,true,false,18, 1.6);
  L1SeedFun["L1_DoubleMu0er1p25_dEta_Max1p8_OS"] = std::bind(&L1AlgoFactory::Onia2015, this, 0.,0.,true,true,18, 1.25);
  L1SeedFun["L1_DoubleMu0er1p0_dEta_Max1p8_OS"] = std::bind(&L1AlgoFactory::Onia2015, this, 0.,0.,true,true,18, 1.25);
  L1SeedFun["L1_DoubleMu0er1p4_dEta_Max1p8_OS"] = std::bind(&L1AlgoFactory::Onia2015, this, 0.,0.,true,true,18, 1.4);
  L1SeedFun["L1_DoubleMu_10_0_dEta_Max1p8"] = std::bind(&L1AlgoFactory::Onia2015, this, 10.,0.,false,false,18, 1.6);
  L1SeedFun["L1_DoubleMuOpen"] = std::bind(&L1AlgoFactory::DoubleMuOpen, this, 0.);
  L1SeedFun["L1_DoubleMu_10_Open"] = std::bind(&L1AlgoFactory::DoubleMuXOpen, this, 10.);
  L1SeedFun["L1_DoubleMu0"]       = std::bind(&L1AlgoFactory::DoubleMu , this , 0.  , 0.  , 1 , false);
  L1SeedFun["L1_DoubleMu_10_0"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 10. , 0.  , 1 , false);
  L1SeedFun["L1_DoubleMu_10_3p5"] = std::bind(&L1AlgoFactory::DoubleMu , this , 10. , 3.5 , 1 , false);
  L1SeedFun["L1_DoubleMu_12_5"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 12. , 5.  , 1 , false);
  L1SeedFun["L1_DoubleMu_11_4"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 11. , 4.  , 1 , false);
  L1SeedFun["L1_DoubleMu_12_8"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 12. , 8.  , 1 , false);
  L1SeedFun["L1_DoubleMu_13_6"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 13. , 6.  , 1 , false);
  L1SeedFun["L1_DoubleMu_15_5"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 15. , 5.  , 1 , false);
  L1SeedFun["L1_TripleMu0"] = std::bind(&L1AlgoFactory::TripleMu, this, 0.,0.,0.,1);
  L1SeedFun["L1_TripleMuOpen"] = std::bind(&L1AlgoFactory::TripleMu, this, 0.,0.,0.,0);
  L1SeedFun["L1_TripleMu_5_5_3"] = std::bind(&L1AlgoFactory::TripleMu, this, 5.,5.,3.,1);
  L1SeedFun["L1_TripleMu_5_0_0"] = std::bind(&L1AlgoFactory::TripleMu, this, 5.,0.,0.,1);
  L1SeedFun["L1_TripleMu_3_0_0"] = std::bind(&L1AlgoFactory::TripleMu, this, 3.,0.,0.,1);
  L1SeedFun["L1_QuadMu0"] = std::bind(&L1AlgoFactory::QuadMu, this, 0.,0.,0.,0.,1);

  //Cross
  L1SeedFun["L1_IsoEG20er_Tau20er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 20.,20.,false);
  L1SeedFun["L1_IsoEG22er_Tau20er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 22.,20.,false);
  L1SeedFun["L1_IsoEG20er_Tau24er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 20.,24.,false);
  L1SeedFun["L1_IsoEG22er_IsoTau26er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 22.,26.,true);
  L1SeedFun["L1_IsoEG20er_IsoTau25er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 20.,25.,true);
  L1SeedFun["L1_IsoEG18er_IsoTau23er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 18.,23.,true);
  L1SeedFun["L1_IsoEG18er_IsoTau25er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 18.,25.,true);
  L1SeedFun["L1_IsoEG18er_IsoTau24er_dEta_Min0p2"] = std::bind(&L1AlgoFactory::IsoEGer_TauJetEta2p17, this, 18.,24.,true);
  L1SeedFun["L1_DoubleMu6_EG6"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 6.,6.,1, false);
  L1SeedFun["L1_DoubleMu6_EG16"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 6.,16.,1, false); // l1t-tsg-v3:  L1_DoubleMu6_EG6
  L1SeedFun["L1_DoubleMu7_EG7"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 7,7.,1, false);
  L1SeedFun["L1_DoubleMu7_EG14"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 7,14.,1, false);  // l1t-tsg-v3:  L1_DoubleMu7_EG7
  L1SeedFun["L1_Mu5_DoubleEG5"] = std::bind(&L1AlgoFactory::Mu_DoubleEG, this, 5., 5.);
  L1SeedFun["L1_Mu6_DoubleEG10"] = std::bind(&L1AlgoFactory::Mu_DoubleEG, this, 6., 10.);
  L1SeedFun["L1_Mu6_DoubleEG17"] = std::bind(&L1AlgoFactory::Mu_DoubleEG, this, 6., 17.); // l1t-tsg-v3:  L1_Mu6_DoubleEG10

  //MultiCross
  L1SeedFun["L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4"] = std::bind(&L1AlgoFactory::Mu_JetCentral_delta, this, 3.,16.);
  L1SeedFun["L1_Mu3_JetC52_dEta_Max0p4_dPhi_Max0p4"] = std::bind(&L1AlgoFactory::Mu_JetCentral_delta, this, 3.,52.);
  L1SeedFun["L1_Mu3_JetC60_dEta_Max0p4_dPhi_Max0p4"] = std::bind(&L1AlgoFactory::Mu_JetCentral_delta, this, 3.,60.);
  L1SeedFun["L1_Mu3_JetC120_dEta_Max0p4_dPhi_Max0p4"] = std::bind(&L1AlgoFactory::Mu_JetCentral_delta, this, 3.,120.);
  L1SeedFun["L1_DoubleJetC56_ETM60"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 56.,56.,60.);
  L1SeedFun["L1_DoubleJetC60_ETM60"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,60.);
  L1SeedFun["L1_DoubleJetC60_ETM70"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,70.);
  L1SeedFun["L1_DoubleEG6_HTT150"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 150.);
  L1SeedFun["L1_DoubleEG6_HTT255"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 255.); // l1t-tsg-v3:  L1_DoubleEG6_HTT150
  L1SeedFun["L1_Jet32_DoubleMuOpen_Mu10_dPhi_Jet_Mu0_Max1p05_dPhi_Mu_Mu_Min1p0"] = std::bind(&L1AlgoFactory::Jet_MuOpen_Mu_dPhiMuMu1, this, 32.,10., 0);
  L1SeedFun["L1_Jet32_DoubleMuOpen_Mu10_dPhi_Jet_Mu0_Max0p4_dPhi_Mu_Mu_Min1p0"] = std::bind(&L1AlgoFactory::Jet_MuOpen_Mu_dPhiMuMu1, this, 32.,10., 0);
  L1SeedFun["L1_Jet32_DoubleMu_10_0_dPhi_Jet_Mu0_Max0p4_dPhi_Mu_Mu_Min1p0"] = std::bind(&L1AlgoFactory::Jet_MuOpen_Mu_dPhiMuMu1, this, 32.,10., 1 );
  L1SeedFun["L1_Jet32_MuOpen_EG10_dPhi_Jet_Mu_Max1p05_dPhi_Mu_EG_Min1p05"] = std::bind(&L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1, this, 32.,10., 0);
  L1SeedFun["L1_Jet32_MuOpen_EG10_dPhi_Jet_Mu_Max0p4_dPhi_Mu_EG_Min1p0"] = std::bind(&L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1, this, 32.,10.,0);
  L1SeedFun["L1_Jet32_Mu0_EG10_dPhi_Jet_Mu_Max0p4_dPhi_Mu_EG_Min1p0"] = std::bind(&L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1, this, 32.,10.,1);
  L1SeedFun["L1_Jet32MuOpen_EG17_dPhiMu_EG1"] = std::bind(&L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1, this, 32.,17., 0); // l1t-tsg-v3:  L1_Jet32MuOpen_EG10_dPhiMu_EG1
  L1SeedFun["L1_DoubleMu0_ETM40"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 40, false); 
  L1SeedFun["L1_DoubleMu0_ETM50"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 50, false); 
  L1SeedFun["L1_DoubleMu0_ETM55"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 55, false); 
  L1SeedFun["L1_DoubleMu0_ETM60"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 60, false); 
  L1SeedFun["L1_DoubleMu0_ETM65"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 65, false); 
  L1SeedFun["L1_DoubleMu0_ETM70"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 70, false); 
  L1SeedFun["L1_DoubleMu0_ETM75"] = std::bind(&L1AlgoFactory::DoubleMu_ETM, this, 0, 0, 75, false); 

  L1SeedFun["L1_DoubleJet8_ForwardBackward"] = std::bind(&L1AlgoFactory::DoubleJet_ForwardBackward, this, 8., 8.); 
  L1SeedFun["L1_DoubleJet12_ForwardBackward"] = std::bind(&L1AlgoFactory::DoubleJet_ForwardBackward, this, 12., 12.); 
  L1SeedFun["L1_DoubleJet16_ForwardBackward"] = std::bind(&L1AlgoFactory::DoubleJet_ForwardBackward, this, 16., 16.); 
  L1SeedFun["L1_ETM60_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 60., 60., false); 
  L1SeedFun["L1_ETM70_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 70., 60., false); 
  L1SeedFun["L1_ETM75_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 75., 60., false); 
  L1SeedFun["L1_ETM85_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 85., 60., false); 
  L1SeedFun["L1_HTM60_HTT260"] = std::bind(&L1AlgoFactory::HTM_HTT, this, 60., 260.); 
  L1SeedFun["L1_HTM80_HTT220"] = std::bind(&L1AlgoFactory::HTM_HTT, this, 80., 220.); 
  L1SeedFun["L1_Mu3_JetC35"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 3., 35., false, 3.0); 
  L1SeedFun["L1_Mu3_JetC16"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 3., 16., false, 3.0); 
  L1SeedFun["L1_Mu3_JetC60"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 3., 60., false, 3.0); 
  L1SeedFun["L1_Mu3_JetC120"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 3., 120., false, 3.0); 
  L1SeedFun["L1_MU20_EG15"] = std::bind(&L1AlgoFactory::Mu_EG, this,20, 15, false, 2);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Mass Trigger ~~~~~
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_50_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 50., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_60_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 60., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_70_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 70., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_80_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 80., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_90_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 90., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_580"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 580); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_610"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 610); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_640"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 640); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_670"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 670); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_700"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 700); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_730"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 730); 
  L1SeedFun["L1_DoubleJet_90_30_Mj30j30_760"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 760); 


  L1SeedFun["L1_DoubleJet30_Mj30j30_360_Mu6"] = std::bind(&L1AlgoFactory::DoubleJetMass_Mu, this, 30, 30., false, 30, 30, false, 360, 6, false, 2); 
  L1SeedFun["L1_DoubleJet30_Mj30j30_360_Mu10"] = std::bind(&L1AlgoFactory::DoubleJetMass_Mu, this, 30, 30., false, 30, 30, false, 360, 10, false, 2); 
  L1SeedFun["L1_DoubleJet_40_30_Mj40j30_540_IsoEG12"] = std::bind(&L1AlgoFactory::DoubleJetMass_Mu, this, 40, 30., false, 40, 30, false, 540, 12, true, false); 

//**************************************************************************//
//                           2017 L1Menu Proposals                          //
//**************************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Top:Halil ~~~~~
  L1SeedFun["L1_IsoEG24er2p1_Jet26er3p0_dR_Min0p3"] = std::bind(&L1AlgoFactory::EG_Jet, this, 24., 26., 0.3, true, true, true);
  L1SeedFun["L1_IsoEG26er2p1_Jet34er3p0_dR_Min0p3"] = std::bind(&L1AlgoFactory::EG_Jet, this, 26,  30,  0.3, true, true, true);
  L1SeedFun["L1_IsoEG28er2p1_Jet34er3p0_dR_Min0p3"] = std::bind(&L1AlgoFactory::EG_Jet, this, 28,  34,  0.3, true, true, true);
  L1SeedFun["L1_IsoEG30er2p1_Jet34er3p0_dR_Min0p3"] = std::bind(&L1AlgoFactory::EG_Jet, this, 30,  34,  0.3, true, true, true);
  L1SeedFun["L1_IsoEG24er2p1_HTT100er"] = std::bind(&L1AlgoFactory::EG_HTT, this, 24, 100, true, true);
  L1SeedFun["L1_IsoEG26er2p1_HTT100er"] = std::bind(&L1AlgoFactory::EG_HTT, this, 26, 100, true, true);
  L1SeedFun["L1_IsoEG28er2p1_HTT100er"] = std::bind(&L1AlgoFactory::EG_HTT, this, 28, 100, true, true);
  L1SeedFun["L1_IsoEG30er2p1_HTT100er"] = std::bind(&L1AlgoFactory::EG_HTT, this, 30, 100, true, true);
  //L1SeedFun["L1_IsoEG24er_TripleJetC26"] = std::bind(&L1AlgoFactory::EGer_TripleJetCentral, this, 24, 26, true, true);
  L1SeedFun["L1_Mu18_HTT100er"] = std::bind(&L1AlgoFactory::Mu_HTT, this, 18, 100);
  L1SeedFun["L1_Mu18_Jet24er3p0"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 18, 24, false, true);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Top:Marina ~~~~~
  L1SeedFun["L1_HTT250er_QuadJet_70_55_40_35_er2p5"] = std::bind(&L1AlgoFactory::HTT_QuadJet, this, 250, 70, 55, 40, 35, 2.5);
  L1SeedFun["L1_HTT280er_QuadJet_70_55_40_35_er2p5"] = std::bind(&L1AlgoFactory::HTT_QuadJet, this, 280, 70, 55, 40, 35, 2.5);
  L1SeedFun["L1_HTT300er_QuadJet_70_55_40_35_er2p5"] = std::bind(&L1AlgoFactory::HTT_QuadJet, this, 300, 70, 55, 40, 35, 2.5);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Top:JIRA38 ~~~~~
  L1SeedFun["L1_DoubleJet100er2p3_dEta_Max1p6"] = std::bind(&L1AlgoFactory::DoubleJet_EtaRes_deltaEta, this, 100, 100, 2.3, 1.6);
  L1SeedFun["L1_Mu10er2p3_Jet32er2p3_dR_Max0p4_DoubleJet32er2p3_dEta_Max1p6"] = std::bind(&L1AlgoFactory::Mu_DoubleJet_Cor, this, 10, 32, 2.3, 0.4, 1.6);
  L1SeedFun["L1_Mu12er2p3_Jet40er2p3_dR_Max0p4_DoubleJet40er2p3_dEta_Max1p6"] = std::bind(&L1AlgoFactory::Mu_DoubleJet_Cor, this, 12, 40, 2.3, 0.4, 1.6);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EXO:JIRA48 ~~~~~
  L1SeedFun["L1_IsoTau40er_ETM80"] = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 80,  true, true, false);
  L1SeedFun["L1_IsoTau40er_ETM85"]  = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 85,  true, true, false);
  L1SeedFun["L1_IsoTau40er_ETM90"]  = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 90,  true, true, false);
  L1SeedFun["L1_IsoTau40er_ETM95"]  = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 95,  true, true, false);
  L1SeedFun["L1_IsoTau40er_ETM100"] = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 100, true, true, false);
  L1SeedFun["L1_IsoTau40er_ETMHF80"] = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 80,  true, true, true);
  L1SeedFun["L1_IsoTau40er_ETMHF90"] = std::bind(&L1AlgoFactory::Tau_ETM, this, 40, 90,  true, true, true);
  L1SeedFun["L1_QuadJet36er3p0_IsoTau52er2p1"] = std::bind(&L1AlgoFactory::QuadJetCentral_TauJet, this, 36, 52, true, true);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Higgs:Chiara ~~~~~
  L1SeedFun["L1_DoubleJet_90_30_DoubleJet30_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 90, 30., false, 30, 30, false, 620); 
  L1SeedFun["L1_DoubleJet_100_30_DoubleJet30_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 100, 30., false, 30, 30, false, 620); 
  L1SeedFun["L1_DoubleJet_100_35_DoubleJet35_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 100, 35., false, 35, 35, false, 620); 
  L1SeedFun["L1_DoubleJet_110_35_DoubleJet35_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 110, 35., false, 35, 35, false, 620); 
  L1SeedFun["L1_DoubleJet_110_40_DoubleJet40_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 110, 40., false, 35, 35, false, 620); 
  L1SeedFun["L1_DoubleJet_115_35_DoubleJet35_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 115, 35., false, 35, 35, false, 620); 
  L1SeedFun["L1_DoubleJet_115_40_DoubleJet40_Mass_Min620"] = std::bind(&L1AlgoFactory::DoubleJetMass, this, 115, 40., false, 40, 40, false, 620); 
  L1SeedFun["L1_DoubleJet30_Mass_Min400_Mu10"] = std::bind(&L1AlgoFactory::DoubleJetMass_Mu, this, 30, 30., false, 30, 30, false, 400, 10, false, 2); 
  L1SeedFun["L1_DoubleJet30_Mass_Min400_Mu6"] = std::bind(&L1AlgoFactory::DoubleJetMass_Mu, this, 30, 30., false, 30, 30, false, 400, 6, false, 2); 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BPH:Sergey ~~~~~
//
  L1SeedFun["L1_DoubleMu4_OS_EG12"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 4.,12.,1, true);
  L1SeedFun["L1_DoubleMu5_OS_EG12"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 5.,12.,1, true);
  L1SeedFun["L1_DoubleMu6_HighQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 6.  , 6. , 999, 2 , true, 999, 999);
  //
  L1SeedFun["L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 0   , 0   , 1.5, 2 , true, 1.4);
  L1SeedFun["L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 0   , 0   , 1.4, 2 , true, 1.4);
  L1SeedFun["L1_DoubleMu4_SQ_OS_dR_Max1p2"]      = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 4   , 4   , 999, 2 , true, 1.2);
  L1SeedFun["L1_DoubleMu4p5_SQ_OS_dR_Max1p2"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 4.5   , 4.5   , 999, 2 , true, 1.2);

  L1SeedFun["L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18"]  = std::bind(&L1AlgoFactory::DoubleMuMass  , this , 4.5 , 4.5 , 2.0, 2 , true, 7,    18);
  L1SeedFun["L1_DoubleMu5_SQ_OS_Mass7to18"]  = std::bind(&L1AlgoFactory::DoubleMuMass  , this , 5 , 5 , 999, 2 , true, 7,    18);
  L1SeedFun["L1_DoubleMu_20_2_SQ_Mass_Max20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 20.  , 2. , 999, 2 , false, 0, 20);

  L1SeedFun["L1_TripleMu_5_0_0_DoubleMu_5_0_OS_Mass_Max17"] = std::bind(&L1AlgoFactory::TripleMu_DoubleMuMass, this, 5, 0, 0, 1, 5, 0, true, 0, 17);
  L1SeedFun["L1_TripleMu_5_3_0_DoubleMu_5_3_OS_Mass_Max17"] = std::bind(&L1AlgoFactory::TripleMu_DoubleMuMass, this, 5, 3, 0, 1, 5, 3, true, 0, 17);
  L1SeedFun["L1_TripleMu_5_3p5_2p5_DoubleMu_5_2p5_OS_Mass_5to17"] = std::bind(&L1AlgoFactory::TripleMu_DoubleMuMass, this, 5, 3.5, 2.5, 1, 5, 2.5, true, 5, 17);
  L1SeedFun["L1_TripleMu_5_4_2p5_DoubleMu_5_2p5_OS_Mass_5to17"] = std::bind(&L1AlgoFactory::TripleMu_DoubleMuMass, this, 5, 4, 2.5, 1, 5, 2.5, true, 5, 17);
  

  L1SeedFun["L1_DoubleMu0_SQ"]    = std::bind(&L1AlgoFactory::DoubleMuMass , this , 0. , 0. , 999 , 2, false, 999, 999);
  L1SeedFun["L1_DoubleMu0_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 0. , 0. , 999 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu0er1p5_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 0. , 0. , 1.5061875 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu4_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4. , 4. , 999 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu4p5_SQ"]    = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4.5 , 4.5 , 999 , 2, false, 999, 999);
  L1SeedFun["L1_DoubleMu4p5_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4.5 , 4.5 , 999 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu4p5er2p0_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4.5 , 4.5 ,2.0 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu5_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 5 , 5 , 999 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu6_SQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 6 , 6 , 999 , 2, true,  999, 999);
  L1SeedFun["L1_DoubleMu8_SQ"]    = std::bind(&L1AlgoFactory::DoubleMuMass , this , 8. , 8. , 999 , 2, false, 999, 999);

  L1SeedFun["L1_TripleMu0_OQ"] = std::bind(&L1AlgoFactory::TripleMu, this, 0.,0.,0.,0);
  L1SeedFun["L1_TripleMu_5_3p5_2p5"] = std::bind(&L1AlgoFactory::TripleMu, this, 5.,3.5,2.5,1);
  L1SeedFun["L1_TripleMu_5_5_5_OQ_OS"] = std::bind(&L1AlgoFactory::TripleMuOS, this, 5., 5, 5, 0, 5, 5, true);
  L1SeedFun["L1_DoubleMu_12_8_SQ"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 12. , 8.  , 2 , false);
  L1SeedFun["L1_DoubleMu_15_5_SQ"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 15. , 5.  , 2 , false);

  //L1SeedFun["L1_DoubleMu_12_5_OS"]      = std::bind(&L1AlgoFactory::DoubleMuMass , this , 12. , 5. , 999, 1 , true, 999, 999);
  //L1SeedFun["L1_DoubleMu_9_5_HighQ_OS"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 9.  , 5. , 999, 2 , true, 999, 999);
  //L1SeedFun["L1_DoubleMu6_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 6.  , 6. , 999, 1 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu5_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 5.  , 5. , 999, 2 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu6_HighQ_OS_M0to16"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 6.  , 6. , 999, 2 , true, 0, 16);

  //L1SeedFun["L1_DoubleMu0er1p4_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 0.  , 0. , 1.4, 2 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu0er1p5_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 0.  , 0. , 1.5, 2 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu3er1p6_HighQ_OS_M6to20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 3.  , 3. , 1.6, 2 , true, 6, 20);
  //L1SeedFun["L1_DoubleMu3er1p8_HighQ_OS_M6to20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 3.  , 3. , 1.8, 2 , true, 6, 20);
  //L1SeedFun["L1_DoubleMu4er1p8_HighQ_OS_M6to20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4.  , 4. , 1.8, 2 , true, 6, 20);

  //L1SeedFun["L1_DoubleMu4p5_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 4.5  , 4.5 , 999, 2 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu6_HighQ_OS_M0to20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 6  , 6 , 999, 2 , true, 0, 20);
  //L1SeedFun["L1_DoubleMu_7_4_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 7  , 4 , 999, 2 , true, 0, 9);
  //L1SeedFun["L1_DoubleMu_7_5_HighQ_OS_M0to20"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 7  , 5 , 999, 2 , true, 0, 20);
  //L1SeedFun["L1_DoubleMu_9_2_HighQ_OS_M0to9"] = std::bind(&L1AlgoFactory::DoubleMuMass , this , 9  , 2 , 999, 2 , true, 0, 9);

  //L1SeedFun["L1_DoubleMu0er1p4_HighQ_OS_dR_Max1p3"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 0  , 0 , 1.4, 2 , true, 1.3);
  //L1SeedFun["L1_DoubleMu3er1p6_HighQ_OS_dR_Max2p0"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 3  , 3 , 1.6, 2 , true, 2.0);
  //L1SeedFun["L1_DoubleMu3er1p8_HighQ_OS_dR_Max2p0"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 3  , 3 , 1.8, 2 , true, 2.0);
  //L1SeedFun["L1_DoubleMu4er1p8_HighQ_OS_dR_Max2p0"] = std::bind(&L1AlgoFactory::DoubleMudRMax , this , 4  , 4 , 1.8, 2 , true, 2.0);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SUSY:Laurent ~~~~~
  L1SeedFun["L1_DoubleMu_15_7"]       = std::bind(&L1AlgoFactory::DoubleMu , this , 15. , 7. , 1 , false);
  L1SeedFun["L1_DoubleMu_15_7_SQ"] = std::bind(&L1AlgoFactory::DoubleMu , this , 15. , 7. , 2 , false);
  L1SeedFun["L1_DoubleMu_15_7_SQ_Mass_Min4"] = std::bind(&L1AlgoFactory::DoubleMuMass, this , 15. , 7. , 999, 2 , false, 4, 999);
  L1SeedFun["L1_TripleMu3"] = std::bind(&L1AlgoFactory::TripleMu, this, 3.,3.,3.,1);
  L1SeedFun["L1_TripleMu3_SQ"] = std::bind(&L1AlgoFactory::TripleMu, this, 3.,3.,3.,2);
  L1SeedFun["L1_TripleMu_5_3_3"] = std::bind(&L1AlgoFactory::TripleMu, this, 5.,3.,3.,1);
  L1SeedFun["L1_TripleMu_4_4_4"] = std::bind(&L1AlgoFactory::TripleMu, this, 4.,4.,4.,1);
  L1SeedFun["L1_DoubleMu7_SQ_EG7"] = std::bind(&L1AlgoFactory::DoubleMu_EG, this, 7,7.,2, false);

  L1SeedFun["L1_DoubleEG6_HTT240er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 240.);
  L1SeedFun["L1_DoubleEG6_HTT250er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 250.);
  L1SeedFun["L1_DoubleEG6_HTT255er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 255.);
  L1SeedFun["L1_DoubleEG6_HTT270er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 270.);
  L1SeedFun["L1_DoubleEG6_HTT300er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 6., 300.);
  L1SeedFun["L1_DoubleEG8_HTT300er"] = std::bind(&L1AlgoFactory::DoubleEG_HT, this, 8., 300.);

  L1SeedFun["L1_DoubleMu0_ETMHF40_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 0, 0, 1, false, 40, true, 60, 30);
  L1SeedFun["L1_DoubleMu0_ETMHF50_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 0, 0, 1, false, 50, true, 60, 30);
  L1SeedFun["L1_DoubleMu0_ETMHF60_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 0, 0, 1, false, 60, true, 60, 30);
  L1SeedFun["L1_DoubleMu0_ETMHF70_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 0, 0, 1, false, 70, true, 60, 30);
  L1SeedFun["L1_DoubleMu0_ETMHF80_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 0, 0, 1, false, 80, true, 60, 30);
  L1SeedFun["L1_DoubleMu3_SQ_ETMHF40_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 3, 3, 2, false, 40, true, 60, 30);
  L1SeedFun["L1_DoubleMu3_SQ_ETMHF50_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 3, 3, 2, false, 50, true, 60, 30);
  L1SeedFun["L1_DoubleMu3_SQ_ETMHF60_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 3, 3, 2, false, 60, true, 60, 30);
  L1SeedFun["L1_DoubleMu3_SQ_ETMHF70_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 3, 3, 2, false, 70, true, 60, 30);
  L1SeedFun["L1_DoubleMu3_SQ_ETMHF80_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::DoubleMu_ETMHF_Jets, this, 3, 3, 2, false, 80, true, 60, 30);


  L1SeedFun["L1_DoubleMu3_SQ_HTT100er"] = std::bind(&L1AlgoFactory::DoubleMu_HTT, this, 3, 3, 2, false, 100);
  L1SeedFun["L1_DoubleMu3_SQ_HTT200er"] = std::bind(&L1AlgoFactory::DoubleMu_HTT, this, 3, 3, 2, false, 200);
  L1SeedFun["L1_DoubleMu3_SQ_HTT220er"] = std::bind(&L1AlgoFactory::DoubleMu_HTT, this, 3, 3, 2, false, 220);
  L1SeedFun["L1_DoubleMu3_SQ_HTT240er"] = std::bind(&L1AlgoFactory::DoubleMu_HTT, this, 3, 3, 2, false, 240);
  //L1SeedFun["L1_DoubleMu3_SQ_HTT100er"] = std::bind(&L1AlgoFactory::DoubleMu_HTT, this, 0, 0, 2, false, 100);

  L1SeedFun["L1_ETMHF70_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 70 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF75_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 75 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF80_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 80 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF85_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 85 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF90_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 90 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF95_Jet60_OR_DoubleJet30"]  = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 95 , true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF100_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 100, true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF105_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 105, true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF110_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 110, true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF115_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 115, true, 60, 30, 0, false);
  L1SeedFun["L1_ETMHF120_Jet60_OR_DoubleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 120, true, 60, 30, 0, false);

  L1SeedFun["L1_ETMHF70_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 70, true, 90, 45, 30, false);
  L1SeedFun["L1_ETMHF80_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 80, true, 90, 45, 30, false);
  L1SeedFun["L1_ETMHF90_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 90, true, 90, 45, 30, false);
  L1SeedFun["L1_ETMHF100_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 100, true, 90, 45, 30, false);
  L1SeedFun["L1_ETMHF110_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 110, true, 90, 45, 30, false);
  L1SeedFun["L1_ETMHF120_Jet90_OR_DoubleJet45_OR_TripleJet30"] = std::bind(&L1AlgoFactory::ETM_JetsComb, this, 120, true, 90, 45, 30, false);

  L1SeedFun["L1_ETMHF90_HTT60er"]  = std::bind(&L1AlgoFactory::ETM_HTT, this, 90,  60, true);
  L1SeedFun["L1_ETMHF100_HTT60er"] = std::bind(&L1AlgoFactory::ETM_HTT, this, 100, 60, true);
  L1SeedFun["L1_ETMHF110_HTT60er"] = std::bind(&L1AlgoFactory::ETM_HTT, this, 110, 60, true);
  L1SeedFun["L1_ETMHF120_HTT60er"] = std::bind(&L1AlgoFactory::ETM_HTT, this, 120, 60, true);
  L1SeedFun["L1_DoubleJet30_Mass_Min300_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 300);
  L1SeedFun["L1_DoubleJet30_Mass_Min320_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 320);
  L1SeedFun["L1_DoubleJet30_Mass_Min340_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 340);
  L1SeedFun["L1_DoubleJet30_Mass_Min360_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 360);
  L1SeedFun["L1_DoubleJet30_Mass_Min380_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 380);
  L1SeedFun["L1_DoubleJet30_Mass_Min400_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, false, 1.5, 400);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min300_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 300);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min320_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 320);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min340_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 340);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min360_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 360);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min380_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 380);
  L1SeedFun["L1_DoubleLeadJet30_Mass_Min400_dEta_Max1p5"] = std::bind(&L1AlgoFactory::DoubleJet_dEtaMass, this, 30, 30, 999, true, 1.5, 400);

  L1SeedFun["L1_Mu3_Jet30er2p5"] = std::bind(&L1AlgoFactory::Mu_Jet, this, 3., 30., false, 2.523); 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HIG:Javier ~~~~~
  L1SeedFun["L1_ETM80_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 80., 60., false); 
  L1SeedFun["L1_ETM90_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 90., 60., false); 
  L1SeedFun["L1_ETM100_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 100., 60., false); 
  L1SeedFun["L1_ETM110_Jet60_dPhi_Min0p4"] = std::bind(&L1AlgoFactory::ETM_Jet, this, 110., 60., false); 
  L1SeedFun["L1_DoubleJet60er3p0_ETM60"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,60.);
  L1SeedFun["L1_DoubleJet60er3p0_ETM70"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,70.);
  L1SeedFun["L1_DoubleJet60er3p0_ETM80"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,80.);
  L1SeedFun["L1_DoubleJet60er3p0_ETM90"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,90.);
  L1SeedFun["L1_DoubleJet60er3p0_ETM100"] = std::bind(&L1AlgoFactory::DoubleJetCentral_ETM, this, 60.,60.,100.);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HIG:Sumit ~~~~~
  L1SeedFun["L1_DoubleEG_Iso23_10"] = std::bind(&L1AlgoFactory::DoubleEGIsoPer, this, 23.,10.,true, false, false );
  L1SeedFun["L1_DoubleEG_Iso24_10"] = std::bind(&L1AlgoFactory::DoubleEGIsoPer, this, 24.,10.,true, false, false );
  L1SeedFun["L1_TripleEG_Iso20_10_5"] = std::bind(&L1AlgoFactory::TripleEGIsoPer, this, 20, 10, 5, true, false, false);
  L1SeedFun["L1_DoubleIsoEG22er2p1"] = std::bind(&L1AlgoFactory::DoubleEGIsoPer, this, 22.,22.,true, true, true );
  L1SeedFun["L1_DoubleMu18er2p1"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 18. , 18.  , 1 , true);
  L1SeedFun["L1_DoubleMu22er2p1"]   = std::bind(&L1AlgoFactory::DoubleMu , this , 22. , 22.  , 1 , true);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ECal:Andrea ~~~~~
  L1SeedFun["L1_IsoEG33_Mt40"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 33.,40., 0, true, false);
  L1SeedFun["L1_IsoEG33_Mt43"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 33.,43., 0, true, false);
  L1SeedFun["L1_IsoEG33_Mt47"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 33.,47., 0, true, false);
  L1SeedFun["L1_IsoEG33_Mt48"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 33.,48., 0, true, false);
  L1SeedFun["L1_IsoEG33_Mt44"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 33.,44., 0, true, false);
  L1SeedFun["L1_IsoEG34_Mt46"] = std::bind(&L1AlgoFactory::SingleEGMT, this, 34.,46., 0, true, false);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Tracker:L1DPG74 ~~~~~
  L1SeedFun["L1_CDC_3_er1p2_TOP120_DPHI2p618_3p665"] = std::bind(&L1AlgoFactory::MuonCDC_dPhi, this, 3, 0, 1.2, 2.618, 3.665);
  L1SeedFun["L1_CDC_4_er1p2_TOP120_DPHI2p618_3p665"] = std::bind(&L1AlgoFactory::MuonCDC_dPhi, this, 4, 0, 1.2, 2.618, 3.665);

  L1SeedFun["L1_ZeroBias"] = [](){return true;};
  return true;

}       // -----  end of function L1Menu2016::InitConfig  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::BookHistogram
//  Description:  
// ===========================================================================
bool L1Menu2016::BookHistogram()
{
  for(auto col : ColumnMap)
  {
    col.second->BookHistogram();
  }
  return true;
}       // -----  end of function L1Menu2016::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::WriteHistogram
//  Description:  
// ===========================================================================
bool L1Menu2016::WriteHistogram() 
{
  for(auto col : ColumnMap)
  {
    col.second->WriteHistogram(outrootfile);
  }
  return true;
}       // -----  end of function L1Menu2016::WriteHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ReadMenu
//  Description:  /* cursor */
// ===========================================================================
bool L1Menu2016::ReadMenu()
{
  mL1Seed.clear();
  vL1Seed.clear();

  //Read the prescales table
  std::ifstream menufile(menufilename);
  if (!menufile)
  {
    std::cout << "MenuFile "<<menufilename<<" is not found !"<<std::endl;
    return false;
  }

  if (writefiles)
    *outfile <<  "---------------------------- Input Menu -------------------------" << std::endl;

  if (menufilename.find_last_of("txt")!= std::string::npos)
    ReadMenuTXT(menufile);
  else if (menufilename.find_last_of("csv")!= std::string::npos)
    ReadMenuCSV(menufile);
  else
  {
    std::cout << "Can not understand MenuFile "<<menufilename<<"! Please provide TXT or CSV format."<<std::endl;
    return false;
  }

  if (writefiles)
    *outfile <<  "---------------------------- Input Menu -------------------------" <<std::endl << std::endl;

  return true;
}       // -----  end of function L1Menu2016::ReadMenu  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ReadMenuTXT
//  Description:  
// ===========================================================================
bool L1Menu2016::ReadMenuTXT(std::ifstream &menufile)
{
  std::string line;

  while (std::getline(menufile, line))
  {
    if (line.empty()) continue;
    if (writefiles)
      *outfile << line <<std::endl;
    if (line.at(0) == '#')
      continue;
    if (line.at(0) == '%')
    {
      std::cout << "Parsing config from menu: " << line  << std::endl;
      ParseConfig(line);
      continue;
    }

    std::size_t commentpos = line.find_first_of("#");
    std::string goodline = "";
    std::string comline = "";

    if (commentpos != std::string::npos)
    {
      goodline = line.substr(0, commentpos);
      comline = line.substr(commentpos, line.length() - commentpos);
    }
    else
      goodline = line;
    std::istringstream iss(goodline);

    std::string seed;
    int bit;
    int prescale;
    std::string pog, pag;

    iss >> seed >> bit >> prescale >> pog >> pag;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Form L1Seed ~~~~~
    L1Seed temp;
    temp.name = seed;
    vL1Seed.push_back(seed);
    temp.bit = bit;
    temp.comment = comline;
    temp.prescale = prescale;

    if (L1Config["doCompuGT"] || L1Config["SetNoPrescale"])
      temp.prescale = 1;

    if (L1Config["IgnorePrescale"] && temp.prescale > 1 )
      temp.prescale = 0;

    if (pog.length() != 0)
      temp.POG = TokenGroups(pog);
    if (pag.length() != 0)
      temp.PAG = TokenGroups(pag);
    mL1Seed[seed] = temp;
  }
  return true;
}       // -----  end of function L1Menu2016::ReadMenuTXT  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ReadMenuCSV
//  Description:  To be finished
// ===========================================================================
bool L1Menu2016::ReadMenuCSV(std::ifstream &menufile)
{
  // Get the first line
  std::string line;
  while (std::getline(menufile, line))
  {
    line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
    if (line.empty()) continue;
    if (line.find_first_not_of(", ") == std::string::npos) continue;
    if (line.at(0) == '#')
      continue;
    break; // Get the first line
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the header  ~~~~~
  boost::char_separator<char> sep(",", "", boost::keep_empty_tokens);
  tokenizer tokens(line, sep);
  std::map<int, std::string> premap;
  std::map<int, std::string> infomap;
  int j = 0;
  for(auto i : tokens)
  {
    i.erase(std::remove_if(i.begin(), i.end(), ::isspace), i.end());
    try
    {
      boost::lexical_cast<double>(i);
      premap[j] = i;
    }
    catch (const boost::bad_lexical_cast &) {
      infomap[j] = i;
    };
    j++;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Select the prescale column ~~~~~
  int targetcol = -1;
  if (premap.size() == 0)
  {
    std::cout << "No prescale column found in " << menufilename<<". Exiting"  << std::endl;
    exit(1);
  }

  if (premap.size() == 1)
  {
    targetcol = premap.begin()->first;
  }
  else{
    if (premap.size() > 1 && L1ConfigStr["SelectCol"] == "")
    {
      std::cout << "Select prescale columns from: ";
      for(const auto &i : premap)
        std::cout << i.second <<", ";
      std::cout << std::endl;
      exit(1);
    }

    for(const auto &i : premap)
    {
      if (i.second == L1ConfigStr["SelectCol"] )
      {
        targetcol = i.first;
        break;
      }
    }
    if (targetcol == -1)
    {
      std::cout << "Can not find " << L1ConfigStr["SelectCol"] <<" in ";
      for(const auto &i : premap)
        std::cout << i.second <<", ";
      std::cout <<"Exiting" << std::endl;
      exit(1);
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Finally reading in the menu ~~~~~
  std::vector<int> outidx;
  std::stringstream ss;
  ss << std::left;
  infomap[targetcol] = "Prescale";
  for(const auto &k : infomap)
  {
    tokenizer::iterator t = tokens.begin();
    std::advance(t, k.first);
    std::string it = std::regex_replace(*t, std::regex("^ +| +$|( ) +"), "$1");
    ss << it <<" "; 
  }
  if (writefiles)
  {
    assert(outfile != nullptr);
    *outfile << ss.str() <<std::endl;
  }

  while (std::getline(menufile, line))
  {
    line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
    if (line.empty()) continue;
    if (line.find_first_not_of(", ") == std::string::npos) continue;
    if (line.at(0) == '#')
      continue;
    if (line.at(0) == '%')
    {
      std::cout << "Parsing config from menu: " << line  << std::endl;
      ParseConfig(line);
      continue;
    }

    ss.str("");
    L1Seed temp;

    tokenizer tokens(line, sep);
    for(const auto &k : infomap)
    {
      tokenizer::iterator t = tokens.begin();
      std::advance(t, k.first);
      std::string it = std::regex_replace(*t, std::regex("^ +|\t+$| +$|( ) +"), "$1");
      
      if (k.second == "n")
      {
        ss << std::setw(4) << it <<" "; 
        try
        {
          temp.bit = boost::lexical_cast<int>(it);
        }
        catch (const boost::bad_lexical_cast &)
        {
          std::cout << "Can't cast bit " << it<< " to int type in line: " << line << std::endl;
        }
      }
      if (k.second == "L1AlgoName")
      {
        ss << std::setw(65) << it <<" "; 
        temp.name = it;
        vL1Seed.push_back(it);
      }
      if (k.second == "Prescale")
      {
        ss << std::setw(5) << it <<" "; 
        try
        {
          temp.prescale = boost::lexical_cast<int>(it);
        }
        catch (const boost::bad_lexical_cast &)
        {
          std::cout << "Can't cast prescale " << it<< " to int type in line: " << line <<"; set to disable" << std::endl;
          temp.prescale = 0;
        }
      }
      if (k.second == "Comment")
      {

        ss << it <<" "; 
        temp.comment = it;
      }
      if (k.second == "POG")
      {
        ss << std::setw(15) << it <<" "; 
        temp.POG = TokenGroups(it);
      }
      if (k.second == "PAG")
      {
        ss << std::setw(15) << it <<" "; 
        temp.PAG = TokenGroups(it);
      }

      if (L1Config["doCompuGT"] || L1Config["SetNoPrescale"])
        temp.prescale = 1;

      if (L1Config["IgnorePrescale"] && temp.prescale > 1 )
        temp.prescale = 0;
    }

    if (writefiles)
      *outfile << ss.str() <<std::endl;
    mL1Seed[temp.name] = temp;
  }

  return true;
}       // -----  end of function L1Menu2016::ReadMenuCSV  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::FormPrescaleColumns
//  Description:  
// ===========================================================================
bool L1Menu2016::FormPrescaleColumns()
{
  std::vector<std::string> VarSeeds;
  for(auto &seed : mL1Seed)
  {
    if (seed.second.prescale < 0)
    {
      VarSeeds.push_back(seed.first);
      std::cout << "Varing " << seed.first << std::endl;
    }
  }
 
  const int varSize = VarSeeds.size();
  int varCounts = 1 << varSize;
  for (int i = 0; i < varCounts; ++i)
  {
    std::map<std::string, L1Seed> tempL1Seed = mL1Seed;
    for (int j = 0; j < varSize; ++j)
    {
      tempL1Seed.at(VarSeeds.at(j)).prescale = (i & 1 << j) > 0 ; 
    }
    ColumnMap[i] = new PreColumn(i, tempL1Seed);
    ColumnMap[i]->PassRelation( vL1Seed, BitMap, POGMap, PAGMap);
  }

  std::cout << "In total ColumnMap "<< ColumnMap.size() << std::endl;
  return true;
}       // -----  end of function L1Menu2016::FormPrescaleColumns  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ReadFilelist
//  Description:  
// ===========================================================================
bool L1Menu2016::OpenWithList(std::string filelist)
{
  L1Ntuple::SelectTree(L1Config["UseUnpackTree"]);
  if (filelist.find(".root") != std::string::npos)
  {
    L1Ntuple::Open(filelist);
    return false;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OpenNtupleList ~~~~~

  std::ifstream flist(filelist.c_str());
  if (!flist)
  {
    std::cout << "File "<<filelist<<" is not found !"<<std::endl;
    return false;
  }

  std::string line;
  while (std::getline(flist, line))
  {
    if (line.empty()) continue;
    if (line.at(0) == '#')
      continue;
    if (line.at(0) == '%')
    {
      std::cout << "Parsing config from ntuple list: " << line  << std::endl;
      ParseConfig(line);
      continue;
    }
    if (!flist.fail())
    {
      listNtuples.push_back(line);
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OpenNtupleList ~~~~~
  if (!CheckFirstFile())      exit(0);
  if (!OpenWithoutInit())     exit(0);

  std::cout.flush();
  std::cout<<"Going to init the available trees..."<<std::endl;
  std::cout.flush();
  Init();

  return true;
}       // -----  end of function L1Menu2016::ReadFilelist  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseConfig
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseConfig(const std::string line)
{
  if (line.find('=') == std::string::npos)
  {
    std::cout << "Wrong config: " << line << std::endl;
    return false;
  }

  std::istringstream iss(line);

  std::string key, sign;
  double value;

  iss >> sign >> key >> sign >> value;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Parse for double inputs ~~~~~
  bool parDouble = iss.fail();
  if (!parDouble && L1Config.find(key) != L1Config.end())
  {
    L1Config[key] = value;
    return true;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Parse for string inputs ~~~~~
  bool parString = true;
  std::istringstream isss(line);
  key=sign="";
  std::string valstr="";

  if (parDouble)
  {
    isss >> sign >> std::ws >> key >>std::ws >> sign >>std::ws >> valstr;
    parString = isss.fail();
  }

  if (!parString && L1ConfigStr.find(key) != L1ConfigStr.end())
  {
    L1ConfigStr[key] = valstr;
    return true;
  }

  if (parDouble && parString)
    std::cout<<"\033[0;31mCan't parse config:\033[0m "<<line<< std::endl; 
  else
    std::cout << "Not reconfiguzed config key " << key<< std::endl;
  
  return false;
}       // -----  end of function L1Menu2016::ParseConfig  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::GetRunConfig
//  Description:  Get the running time config from command line
// ===========================================================================
bool L1Menu2016::GetRunConfig(std::map<std::string, float> &config, 
    std::map<std::string, std::string> &configstr)
{

  for(auto c : config)
  {
    if (L1Config.find(c.first) != L1Config.end())
    {
      L1Config[c.first] = c.second;
    }
  }

  for(auto c : configstr)
  {
    if (L1ConfigStr.find(c.first) != L1ConfigStr.end())
    {
      L1ConfigStr[c.first] = c.second;
    }
  }
  return true;
}       // -----  end of function L1Menu2016::GetRunConfig  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PrintConfig
//  Description:  
// ===========================================================================
bool L1Menu2016::PrintConfig() const
{
  std::cout << "Printing configuration ___________________________ " << std::endl;
  for(auto &x: L1Config)
    std::cout << std::setw(20) <<x.first <<" : " << x.second << std::endl;
  for(auto &x: L1ConfigStr)
    std::cout << std::setw(20) <<x.first <<" : " << x.second << std::endl;
  std::cout << "Printed configuration ============================ " << std::endl;
  return true;
}       // -----  end of function L1Menu2016::PrintConfig  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PreLoop
//  Description:  
// ===========================================================================
bool L1Menu2016::PreLoop(std::map<std::string, float> &config, std::map<std::string, std::string> &configstr)
{
  GetRunConfig(config, configstr);
  OpenWithList(tuplefilename);

  //Prepare Menu
  std::cout << "Preparing menus __________________________________ " << std::endl;
  ReadMenu();
  BuildRelation();
  L1SeedFunc();
  FormPrescaleColumns();

  PrintConfig();
  BookHistogram();
  
  if (writeplots)
  {
    GlobalAlgBlk *l1uGTsel_ = l1uGT_;
    TChain       *fl1uGTsel = fl1uGT;
    
    if (L1Config["UseUnpackTree"]){
      l1uGTsel_ = l1unpackuGT_;
      fl1uGTsel = fl1unpackuGT;      
    }
    
    l1Plot = new L1Plot(outrootfile, event_, upgrade_, recoJet_,
			recoSum_, recoEle_, recoMuon_, recoTau_, recoFilter_, l1CaloTower_, recoVtx_, l1uGTsel_);
    l1Plot->SetTodo(L1Config);
    //std::cout << "line 1004 start" << std::endl;
    l1Plot->PreRun(&L1Event, &mL1Seed, L1Ntuple::GetuGTAlias(fl1uGTsel));
    //l1Plot->PreRun(&L1Event, &mL1Seed, {});		//temp solution to Aaron's ntuple, sould be commented
    //std::cout << "line 1004 end" << std::endl;
  }

  if (L1Config["doPrintPU"] || L1Config["SelectFill"] != -1 )
  {
    ReadDataPU();
  }
    
  if (L1Config["doTnPMuon"])
  {
    l1TnP = new L1TnP(outrootfile, event_, upgrade_, recoJet_,
		      recoSum_, recoEle_, recoMuon_, recoTau_, recoFilter_, l1CaloTower_, recoVtx_, l1uGT_);
    if (L1Config["doTnPMuon"])
      l1TnP->DoMuonTnP();
  }

  if (l1unpackuGT_ != NULL)
  {
    l1unpackuGT = new L1uGT( outrootfile, event_, l1unpackuGT_, &L1Event, &mL1Seed);
    //std::cout << "line 1025 start" << std::endl;
    l1unpackuGT->GetTreeAlias(L1Ntuple::GetuGTAlias(fl1unpackuGT));
    //std::cout << "line 1025 end" << std::endl;
  }

  if (L1Config["doCompuGT"] || L1Config["UseuGTDecision"] || L1Config["doPlotuGt"])
  {
    assert(l1uGT_ != NULL);
    l1uGT = new L1uGT( outrootfile, event_, l1uGT_, &L1Event, &mL1Seed);
    std::cout << "line 1034 start" << std::endl;
    l1uGT->GetTreeAlias(L1Ntuple::GetuGTAlias(fl1uGT));
    std::cout << "line 1034 end" << std::endl;
  }


  if (L1Config["SetMuonER"] != -1) SetMuonER(L1Config["SetMuonER"]);
  if (L1Config["UseUpgradeLyr1"] != -1) SetUseUpgradeLyr1(L1Config["UseUpgradeLyr1"]);
  if (L1Config["UseL1CaloTower"] != -1) SetUseL1CaloTower(L1Config["UseL1CaloTower"]);

  if (L1ConfigStr["SelectLS"] != "") 
    ParseRanges("SelectLS", pLS);

  if (L1ConfigStr["SelectBX"] != "") 
    ParseRanges("SelectBX", pBX);
  return true;
}       // -----  end of function L1Menu2016::PreLoop  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::GetL1Event
//  Description:  
// ===========================================================================
bool L1Menu2016::GetL1Event()
{
  L1Event = {};

  //Jet
  L1AlgoFactory::SingleJetPt(L1Event.JetPt,false);
  L1AlgoFactory::SingleJetPt(L1Event.JetCenPt,true);

  //EG
  L1AlgoFactory::SingleEGPt(L1Event.EGPt,false, false);
  L1AlgoFactory::SingleEGPt(L1Event.EGerPt,false, true);
  L1AlgoFactory::SingleEGPt(L1Event.IsoEGPt,true, false);
  L1AlgoFactory::SingleEGPt(L1Event.IsoEGerPt,true, true);

  //Tau
  L1AlgoFactory::SingleTauPt(L1Event.TauPt, false, false); 
  L1AlgoFactory::SingleTauPt(L1Event.TauerPt, true, false);
  L1AlgoFactory::SingleTauPt(L1Event.IsoTauPt, false, true);

  //Mu
  L1AlgoFactory::SingleMuPt(L1Event.MuPt, false, 2);
  L1AlgoFactory::SingleMuPt(L1Event.MuOpenPt, false, 0);
  L1AlgoFactory::SingleMuPt(L1Event.MuerPt, true, 2);

  //Sum
  if (L1Config["SumJetET"] != 0 || L1Config["SumJetEta"] != 999)
  {
    CalLocalHT(L1Event.HTT, false);
    CalLocalHT(L1Event.HTTHF, true);
    CalLocalHTM(L1Event.HTM);
  } else {
    L1AlgoFactory::HTTVal(L1Event.HTT);
    L1AlgoFactory::HTTHFVal(L1Event.HTTHF); // Not in L1Ntuple yet
    L1AlgoFactory::HTMVal(L1Event.HTM);
  }

  if (L1Config["UseL1CaloTower"])
  {
    CalLocalETM(L1Event.ETM, false);
    CalLocalETM(L1Event.ETMHF, true);
  }
  else
  {
    L1AlgoFactory::ETMVal(L1Event.ETM);
    L1AlgoFactory::ETMHFVal(L1Event.ETMHF);
  }

  L1AlgoFactory::ETTVal(L1Event.ETT);

  // Mulit
  float dummy = 0;
  L1AlgoFactory::DoubleMuPt(L1Event.doubleMuPt1,L1Event.doubleMuPt2, true, false);
  L1AlgoFactory::Onia2015Pt(L1Event.oniaMuPt1, L1Event.oniaMuPt2,true, false, 18);
  L1AlgoFactory::DoubleJetPt(L1Event.dijetPt1,L1Event.dijetPt2);
  L1AlgoFactory::DoubleJetPt(L1Event.diCenjetPt1,L1Event.diCenjetPt2,true);
  L1AlgoFactory::DoubleTauJetEta2p17Pt(dummy,L1Event.ditauPt, false);
  L1AlgoFactory::DoubleTauJetEta2p17Pt(dummy,L1Event.diIsotauPt, true);
  L1AlgoFactory::QuadJetPt(dummy,dummy,dummy,L1Event.quadjetPt);
  L1AlgoFactory::QuadJetPt(dummy,dummy,dummy,L1Event.quadjetCPt,true);
  L1AlgoFactory::DoubleEGPt(L1Event.diEG1,L1Event.diEG2,false);
  L1AlgoFactory::DoubleEGPt(L1Event.diIsolEG1,L1Event.diIsolEG2,true);

  return true;
}       // -----  end of function L1Menu2016::GetL1Event  -----
// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::Loop
//  Description:  
// ===========================================================================
bool L1Menu2016::Loop()
{
  ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initial ~~~~~
  //Int_t nevents = fChain->GetEntriesFast();//GetEntries();
  unsigned int currentLumi(-1);
  nZeroBiasevents = 0.;
  int i = -1;
  nLumi.clear();
  bool skipLS = false;

  while(true)
  {
    i++;
    Long64_t ientry = LoadTree(i); 
    if (ientry < 0) break;
    GetEntry(i);
    if (L1Config["maxEvent"] != -1 && i > L1Config["maxEvent"]) break;

    if (event_ != NULL )
    {
      if (L1Config["SelectRun"] != -1 && event_->run != L1Config["SelectRun"])
        continue;

      if (L1Config["SelectFill"] != -1 && (DataFillNO.find(event_->run) == DataFillNO.end() 
            || DataFillNO.at(event_->run)  !=  L1Config["SelectFill"] ))
          continue;

      if (L1Config["SelectEvent"] != -1 && event_->event != L1Config["SelectEvent"])
        continue;

      if(event_ -> lumi != currentLumi){
        currentLumi = event_ -> lumi;
        //skipLS      = CheckLS(currentLumi);
	if (L1ConfigStr["SelectLS"] != "" || L1Config["doScanLS"])
	  skipLS      = CheckLS(currentLumi);
        if (!skipLS)
          nLumi.insert(currentLumi);
      } 

      if (L1ConfigStr["SelectLS"] != "" && skipLS)
        continue;

      if (L1ConfigStr["SelectBX"] != "" && CheckBX(event_->bx) )
        continue;

      if (L1Config["Select_BX_in_48b"] != -1 && !Found_BX_in_48b(L1Config["Select_BX_in_48b"], event_->bx))
        continue;
      if (L1Config["Select_BX_in_12b"] != -1 && !Found_BX_in_12b(L1Config["Select_BX_in_12b"], event_->bx))
        continue;
      if (L1Config["doBXReweight_1_to_6_47_48"] && !BXReweight_is_1_to_6_47_48(event_->bx))
        continue;
      if (L1Config["doBXReweight128"] && !BXReweight_is128(event_->bx))
        continue;
      if (L1Config["doBXReweight34567"] && !BXReweight_is34567(event_->bx))
        continue;
      if (L1Config["doBXReweight_1_to_6_11_12"] && !BXReweight_is_1_to_6_11_12(event_->bx))
        continue;
      if (L1Config["doBXReweight_5_to_10"] && !BXReweight_is_5_to_10(event_->bx))
        continue;

      if (L1Config["doPrintBX"])
	std::cout << "BX = " << event_->bx << std::endl;

      if (L1Config["doScanLS"])
        continue;
    }

    if (i % 1000 == 0)
    {
      std::cout << "Processed " << i << " events." << std::endl;
    }

    //Use Final decision by default, unless for PlotLS
    //if (l1unpackuGT != NULL && !l1unpackuGT->GetuGTDecision("L1_ZeroBias", L1Config["doPlotLS"]))
    //In case using L1Accept, don't count the Zerobias Event
    if (L1Config["SetL1AcceptPS"] ==0 && l1unpackuGT != NULL && !l1unpackuGT->GetuGTDecision("L1_ZeroBias", L1Config["doPlotLS"])) 
      continue;

    nZeroBiasevents++;

    GetL1Event();
    RunMenu();

    if (L1Config["doPlotLS"])
      FillLumiSection(currentLumi);

    if (L1Config["doPrintPU"] && event_ != NULL)
      FillPileUpSec();

    if (l1Plot != NULL)
      l1Plot->RunPlot();

    if (l1TnP != NULL)
      l1TnP->RunTnP();

    if (L1Config["doCompuGT"])
      l1uGT->CompEvents();
  }

  std::cout << "Total Event: " << i <<" ZeroBias Event: " << nZeroBiasevents << std::endl;
  return true;
}       // -----  end of function L1Menu2016::Loop  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PostLoop
//  Description:  
// ===========================================================================
bool L1Menu2016::PostLoop()
{
  scale = CalScale(0, 0, true);

  std::cout << "==================================== Summary" << std::endl;

  for(auto col : ColumnMap)
  {
    col.second->CalRate(scale);
    col.second->FillDefHist1D(scale);
    col.second->FillDefHist2D(scale);
  }
  
  if (ColumnMap.size() == 1)
    PrintRates(std::cout);
  else{
    for(auto col : ColumnMap)
      col.second->PrintMenuRate(scale);
  }
  if (writefiles)
    PrintRates(*outfile);
  PrintCSV(*outcsv);

  if (l1Plot != NULL)
  {
    l1Plot->PostRun(scale);
    if (L1Config["doPlotLS"])
      l1Plot->PlotRatePerLS(L1LSCount, L1Config["nBunches"]);
  }

  if (l1TnP != NULL)
    l1TnP->PostRun();

  if (L1Config["doPrintPU"])
  {
    PrintPUCSV();
  }
  WriteHistogram();
  return true;
}       // -----  end of function L1Menu2016::PostLoop  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PrintPUCSV
//  Description:  
// ===========================================================================
bool L1Menu2016::PrintPUCSV()
{
  //const int nBunches = 2736;
  std::fstream pucsv (outputdir + "/" + outputname+"_PU" +".csv", std::fstream::out );

  // L1Seeds
  std::vector<std::string> csvout;
  for(auto col : ColumnMap)
  {
    col.second->PrintPUCSV(csvout);
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Writing out to file ~~~~~
  for(auto i : csvout)
  {
    pucsv << i <<std::endl;
  }
  pucsv.close();

  return true;
}       // -----  end of function L1Menu2016::PrintPUCSV  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PrintRates
//  Description:  
// ===========================================================================
bool L1Menu2016::PrintRates(std::ostream &out)
{
  for(auto col : ColumnMap)
  {
    col.second->PrintRates(out, scale);
  }
  return true;
}       // -----  end of function L1Menu2016::PrintRates  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::BuildRelation
//  Description:  
// ===========================================================================
bool L1Menu2016::BuildRelation()
{
  int misbit= -1;
  for(auto &l1 : mL1Seed)
  {

    // bit
    if (BitMap.find(l1.second.bit) != BitMap.end())
    {
      std::cout << "Duplicate bit number at " << l1.second.bit <<" for " 
        << BitMap[l1.second.bit] <<" and " << l1.first 
        << "; setting " << l1.first  << " to bit " << misbit << std::endl;
      l1.second.bit = misbit;
      BitMap[l1.second.bit] = l1.first;
      misbit--;
    } else BitMap[l1.second.bit] = l1.first;

    for(auto &pog : l1.second.POG)
    {
      if (POGMap.find(pog) != POGMap.end())
      {
        POGMap[pog] = {};
        assert(POGMap[pog].size() == 0);
      } else POGMap[pog].push_back(l1.second.bit);
    }

    for(auto &pag : l1.second.PAG)
    {
      if (PAGMap.find(pag) != PAGMap.end())
      {
        PAGMap[pag] = {};
        assert(PAGMap[pag].size() == 0);
      } else PAGMap[pag].push_back(l1.second.bit);
    }
  }

  return true;
}       // -----  end of function L1Menu2016::BuildRelation  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::L1SeedFunc
//  Description:  
// ===========================================================================
bool L1Menu2016::L1SeedFunc()
{
#ifdef UTM_MENULIB
  std::map<std::string, std::function<bool()>> L1SeedFun_temp;
  addFuncFromName(L1SeedFun_temp, upgrade_, l1CaloTower_);
  //addFuncFromName(L1SeedFun_temp, upgrade_);
#endif
    
  for(auto &L1Seed : mL1Seed)
  {
/*    if (L1SeedFun.find(L1Seed.first) != L1SeedFun.end())
      continue;

    if(ParseL1Seed(L1Seed.first))
      continue;
*/	//this part is commented, so that seeds' algo are always added from menulib, NOT L1AlgoFactory
    if (L1SeedFun_temp.find(L1Seed.first) != L1SeedFun_temp.end())
    {
      std::cout << "Add from Menulib " << L1Seed.first << std::endl;
      L1SeedFun[L1Seed.first] = L1SeedFun_temp[L1Seed.first];
      continue;
    }

    std::cout << "No function call for " << L1Seed.first <<"; setting to no fire"<< std::endl;
  }

  return true;
}       // -----  end of function L1Menu2016::L1SeedFunc  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CheckL1Seed
//  Description:  /* cursor */
// ===========================================================================
bool L1Menu2016::CheckL1Seed(const std::string L1Seed)
{
  if (L1SeedFun.find(L1Seed) != L1SeedFun.end())
  {
    return L1SeedFun[L1Seed]();
  }
  return false;
}       // -----  end of function L1Menu2016::CheckL1Seed  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::RunMenu
//  Description:  
// ===========================================================================
bool L1Menu2016::RunMenu()
{
  for(auto col : ColumnMap)
  {
    col.second->EventReset();
  }

  for(auto& seed: mL1Seed)
  {
    bool IsFired = false;
    if (L1Config["UseuGTDecision"])
    {
      assert(l1uGT != NULL);
      IsFired = l1uGT->GetuGTDecision(seed.first);
    }
    else
      IsFired = CheckL1Seed(seed.first);

    for(auto col : ColumnMap)
    {
      col.second->InsertInMenu(seed.first, IsFired);
    }
  }

  for(auto col : ColumnMap)
  {
    col.second->CheckCorrelation();
  }

  return true;
}       // -----  end of function L1Menu2016::RunMenu  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CalScale
//  Description:  
// ===========================================================================
double L1Menu2016::CalScale(int nEvents_, int nBunches_, bool print) 
{
  double scale = 0.0;
  int nEvents = nEvents_ == 0 ? nZeroBiasevents : nEvents_;
  double nBunches = nBunches_ == 0 ?  L1Config["nBunches"] : nBunches_;

  if (L1Config["nBunches"] < 0)
  {
    //scale = (-1.*L1Config["nBunches"])/(nLumi.size()*23.31);
    scale = (-1.*L1Config["nBunches"] * L1Config["SetL1AcceptPS"])/(nLumi.size()*23.31);
    if (print)
      //std::cout << "Scale by "   << "("<< -1. * L1Config["nBunches"] <<")/(nLumi*23.31) with nLumi = " << nLumi.size()      << std::endl;
      std::cout << "Scale by "   << "("<< -1. * L1Config["nBunches"] <<"* L1AcceptPS )/(nLumi*23.31) with L1AcceptPS = "<< L1Config["SetL1AcceptPS"] <<" nLumi = " << nLumi.size()  << std::endl;
  } else {
    scale = 11246.; // ZB per bunch in Hz
    //scale /= nZeroBiasevents*1000.; // in kHz
    scale /= nEvents; // in Hz
    scale *= nBunches;
    if (print)
      std::cout << "Scale by "   << " 11246 / nZeroBiasevents * NumberOfBunches, with nZeroBiasevents = " 
        << nEvents    <<" NumberOfBunches = " << nBunches << std::endl;
  }
  return scale;
}       // -----  end of function L1Menu2016::CalScale  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::SetOutputName
//  Description:  
// ===========================================================================
std::string L1Menu2016::SetOutputName() const
{
  boost::filesystem::path menupath(menufilename);
  boost::filesystem::path flistpath(tuplefilename);
  std::stringstream ss;
  ss << menupath.stem().string() << "-" << flistpath.stem().string();
  return ss.str();
}       // -----  end of function L1Menu2016::SetOutputName  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseL1Seed
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseL1Seed(const std::string SeedName)
{
  if (SeedName.find("L1_") == std::string::npos)
  {
    return false;
  }

  if (ParseSingleObject(SeedName)) return true;

  // Jets
  if (ParseDoubleJet(SeedName)) return true;
  if (ParseTripleJetVBF(SeedName)) return true;
  if (ParseQuadJet(SeedName)) return true;
  // EG
  if (ParseDoubleEG(SeedName)) return true;
  if (ParseTripleEG(SeedName)) return true;
  // Tau
  if (ParseDoubleTau(SeedName)) return true;

  // Mu_Tau
  if (ParseMuerTauer(SeedName)) return true;
  // Mu_EG
  if (ParseMuEG(SeedName)) return true;
  // Mu_Sum
  if (ParseMuSum(SeedName)) return true;

  // EG_Sum
  if (ParseEGSum(SeedName)) return true;

  if (ParseEGStrategy(SeedName)) return true;

  if (ParseETMJetdPhi(SeedName)) return true;

  if (ParseComplexSingleMu(SeedName)) return true;
  // EGMass
  //if (ParseMultiEGMass(SeedName)) return true;

  return false;
}       // -----  end of function L1Menu2016::ParseL1Seed  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseSingleObject
//  Description:  /* cursor */
// ===========================================================================
bool L1Menu2016::ParseSingleObject(const std::string SeedName)
{
  std::string L1object ="";
  std::string postfix = "";
  int pt = -10;

  boost::char_separator<char> sep("_");
  tokenizer tokens(SeedName, sep);
  if (std::distance(tokens.begin(), tokens.end()) < 2) return false;
  boost::tokenizer<boost::char_separator<char> >::iterator tokenit = tokens.begin();
  tokenit++;
  std::string Seedtoken(*tokenit);

  std::smatch base_match;
  std::regex integerobj("Single([^0-9]+)([0-9]+)(er|er2p1|)");
  std::regex integerSum("(ETMHF|ETM|HTT|ETT|HTM)([0-9]+)(er|)");
  if (std::regex_match(Seedtoken, base_match, integerobj))
  {
	// The first sub_match is the whole string; the next
	// sub_match is the first parenthesized expression.
    L1object = base_match[1].str();
    pt = std::stoi(base_match[2].str(), nullptr);
    postfix = base_match[3].str();
  }else if (std::regex_match(Seedtoken, base_match, integerSum))
  {
	// The first sub_match is the whole string; the next
	// sub_match is the first parenthesized expression.
    L1object = base_match[1].str();
    pt = std::stoi(base_match[2].str(), nullptr);
    postfix = base_match[3].str();
  } else if(Seedtoken == "SingleMuOpen")
  {
    L1object = "Mu";
    postfix = "Open";
    pt = 0;
  }
    

  L1object += postfix;
  mL1Seed[SeedName].singleObj = L1object;
  std::vector<std::function<bool()>> funs;

  //std::cout <<  std::distance(tokenit, tokens.end())<< std::endl;
  if (std::distance(tokenit, tokens.end()) > 1) 
  {
    tokenit++;
    Seedtoken = *tokenit;
    if (Seedtoken == "NotBptxOR" || Seedtoken == "BptxAND")
    {
      funs.push_back(ParseBptx(Seedtoken));
    }
    else return false;
  }

  if (L1ObjectMap.find(L1object) != L1ObjectMap.end())
  {
    //funs.push_back(std::bind(&SingleObjPt, L1ObjectMap[L1object], pt));
    // No idea for the funs vector 
    L1SeedFun[SeedName] = std::bind(&SingleObjPt, L1ObjectMap[L1object], pt);
  } else return false;

  return true;
}       // -----  end of function L1Menu2016::ParseSingleObject  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseBptx
//  Description:  /* cursor */
// ===========================================================================
std::function<bool()> L1Menu2016::ParseBptx(const std::string /*Seedtoken*/)
{
  return [](){return true;};
}       // -----  end of function L1Menu2016::ParseBptx  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseDoubleJet
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseDoubleJet(const std::string& SeedName)
{
  std::smatch base_match;
  std::regex integer("L1_DoubleJet(C|)([0-9]+)(er3p0|)");
  if (std::regex_match(SeedName, base_match, integer))
  {
    bool isCentral = base_match.length(1) == 1 || (base_match.length(3) == 5 );
    unsigned int pt = std::stoi(base_match[2].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::DoubleJet, this, pt, pt, isCentral);
    return true;
  }
  else return false;
}       // -----  end of function L1Menu2016::ParseDoubleJet  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseDoubleTau
//  Description:  /* cursor */
// ===========================================================================
bool L1Menu2016::ParseDoubleTau(const std::string& SeedName) 
{
  std::smatch base_match;
  std::regex integer("L1_Double(Iso|)Tau([0-9]+)er(2p1|)");
  if (std::regex_match(SeedName, base_match, integer))
  {
    bool isIso = base_match.length(1) == 3;
    unsigned int pt = std::stoi(base_match[2].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::DoubleTauJetEta2p17, this, pt, pt, isIso);
    return true;
  }
  else return false;
}       // -----  end of function L1Menu2016::ParseDoubleTau  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseTripleJetVBF
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseTripleJetVBF(const std::string& SeedName)
{
  const int jetclass = 0; 
  std::smatch base_match;
  std::regex integer("L1_TripleJet_([0-9]+)_([0-9]+)_([0-9]+)_VBF");
  if (std::regex_match(SeedName, base_match, integer))
  {
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::TripleJet_VBF, this, 
        std::stoi(base_match[1].str(), nullptr),
        std::stoi(base_match[2].str(), nullptr),
        std::stoi(base_match[3].str(), nullptr), jetclass);
    return true;
  }
  else return false;
}       // -----  end of function L1Menu2016::ParseTripleJetVBF  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseQuadJet
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseQuadJet(const std::string& SeedName)
{
  int pt1 = -10;
  int pt2 = -10;
  int pt3 = -10;
  int pt4 = -10;
  bool isCentral = false;

  std::smatch base_match;
  std::regex integer_sys("L1_QuadJet([C]*)([0-9]+)(er3p0|)");
  std::regex integer_asys("L1_QuadJet([C]*)_([0-9]+)_([0-9]+)_([0-9]+)_([0-9]+)");
  if (std::regex_match(SeedName, base_match, integer_sys))
  {
    isCentral = base_match.length(1) == 1 || base_match.length(3) == 5;
    pt1 = std::stoi(base_match[2].str(), nullptr);
    pt2 = pt3 = pt4 = pt1;
  } else if (std::regex_match(SeedName, base_match, integer_asys))
  {
    isCentral = base_match.length(1) == 1;
    pt1 = std::stoi(base_match[2].str(), nullptr);
    pt2 = std::stoi(base_match[3].str(), nullptr);
    pt3 = std::stoi(base_match[4].str(), nullptr);
    pt4 = std::stoi(base_match[5].str(), nullptr);
  }

  if (pt1 != -10 && pt2 != -10 && pt3 != -10 && pt4 != -10)
  {
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::QuadJet, this, pt1, pt2, pt3, pt4, isCentral);
    return true;
  }
  else return false;
}       // -----  end of function L1Menu2016::ParseQuadJet  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseDoubleEG
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseDoubleEG(const std::string& SeedName)
{
  int pt1 = -10;
  int pt2 = -10;
  bool isIso = false;

  std::smatch base_match;
  std::regex integer_sys("L1_Double(Iso|)EG([0-9]+)");
  std::regex integer_asys("L1_Double(Iso|)EG_([0-9]+)_([0-9]+)");
  if (std::regex_match(SeedName, base_match, integer_sys))
  {
    isIso = base_match.length(1) == 3;
    pt1 = std::stoi(base_match[2].str(), nullptr);
    pt2 = pt1;
  }else if (std::regex_match(SeedName, base_match, integer_asys))
  {
    isIso = base_match.length(1) == 3;
    pt1 = std::stoi(base_match[2].str(), nullptr);
    pt2 = std::stoi(base_match[3].str(), nullptr);
  }

  if (pt1 != -10 && pt2 != -10)
  {
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::DoubleEG, this, pt1, pt2, isIso);
    return true;
  }
  else
    return false;
}       // -----  end of function L1Menu2016::ParseDoubleEG  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseTripleEG
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseTripleEG(const std::string& SeedName)
{
  int pt1 = -10;
  int pt2 = -10;
  int pt3 = -10;

  std::smatch base_match;
  std::regex integer_sys("L1_TripleEG([0-9]+)");
  std::regex integer_asys("L1_TripleEG_([0-9]+)_([0-9]+)_([0-9]+)");
  if (std::regex_match(SeedName, base_match, integer_sys))
  {
    pt1 = std::stoi(base_match[1].str(), nullptr);
    pt2 = pt1;
    pt3 = pt1;
  }else if (std::regex_match(SeedName, base_match, integer_asys))
  {
    pt1 = std::stoi(base_match[1].str(), nullptr);
    pt2 = std::stoi(base_match[2].str(), nullptr);
    pt3 = std::stoi(base_match[3].str(), nullptr);
  }

  if (pt1 != -10 && pt2 != -10 && pt3 != -10)
  {
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::TripleEG, this, pt1, pt2, pt3);
    return true;
  }
  else
    return false;
}       // -----  end of function L1Menu2016::ParseTripleEG  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseEGSum
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseEGSum(const std::string& SeedName)
{
  int EGpt = -10;
  int Sumpt = -10;
  bool isIsoEG= false;

  std::smatch base_match;
  std::regex integerEGerHTT("L1_(Iso|)EG([0-9]+)er(2p1|)_HTT([0-9]+)(er|)");
  if (std::regex_match(SeedName, base_match, integerEGerHTT))
  {
    isIsoEG = base_match.length(1) == 3;
    EGpt =  std::stoi(base_match[2].str(), nullptr);
    Sumpt = std::stoi(base_match[4].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::SingleEG_Eta2p1_HTT, this, EGpt, Sumpt, isIsoEG);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseEGSum  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseEGStrategy
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseEGStrategy(const std::string & SeedName)
{
  int pt = -10;

  std::smatch base_match;
  std::regex EGStrategy("L1_StrategyEG([0-9]+)");
  if (std::regex_match(SeedName, base_match, EGStrategy))
  {
    pt =  std::stoi(base_match[1].str(), nullptr);
    L1SeedFun[SeedName] = 
      boost::bind(&SingleObjPt, L1ObjectMap["EG"], pt) ||
      boost::bind(&SingleObjPt, L1ObjectMap["IsoEG"], pt-2) ||
      boost::bind(&SingleObjPt, L1ObjectMap["IsoEGer"], pt-4);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseEGStrategy  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseETMJetdPhi
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseETMJetdPhi(const std::string & SeedName)
{
  int ETMpt = -10;
  int Jetpt = -10;
  bool isCentral = false;

  std::smatch base_match;
  std::regex ETMJetdPhi("L1_ETM([0-9]+)_Jet([C]*)([0-9]+)_dPhi_Min0p4");
  if (std::regex_match(SeedName, base_match, ETMJetdPhi))
  {
    ETMpt =  std::stoi(base_match[1].str(), nullptr);
    isCentral = base_match.length(2) == 1;
    Jetpt =  std::stoi(base_match[3].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::ETM_Jet, this, ETMpt, Jetpt, isCentral);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseETMJetdPhi  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseMultiEGMass
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseMultiEGMass(const std::string& SeedName)
{
  
  int pts = -10;
  int pt1 = -10;
  int pt2 = -10;
  int pt3 = -10;
  int pt4 = -10;
  int Mcut = -10;
  bool isIso = false;
  bool isER = false;
  int EGcount = 0;

  std::map<std::string, int> CountMap;
  CountMap["Single"] = 1;
  CountMap["Double"] = 2;
  CountMap["Triple"] = 3;
  CountMap["Quad"] = 4;


  std::smatch base_match;
  std::regex integer("L1_(Single|Double|Triple|Quad)(Iso|)EG([0-9]*)(er|)(_{0,1}[0-9]*)(_{0,1}[0-9]*)(_{0,1}[0-9]*)(_{0,1}[0-9]*)_M([0-9]+)");
  if (std::regex_match(SeedName, base_match, integer))
  {
    EGcount = CountMap[base_match[1].str()];
    isIso = base_match.length(2) != 0;
    if (base_match[3].str() != "")
      pts =std::stoi(base_match[3].str(), nullptr);
    isER = base_match.length(4) != 0;
    if (base_match[5].str() != "")
      pt1 = std::stoi(base_match[5].str().erase(0, 1), nullptr);
    if (base_match[6].str() != "")
      pt2 = std::stoi(base_match[6].str().erase(0, 1), nullptr);
    if (base_match[7].str() != "")
      pt3 = std::stoi(base_match[7].str().erase(0, 1), nullptr);
    if (base_match[8].str() != "")
      pt4 = std::stoi(base_match[8].str().erase(0, 1), nullptr);
    Mcut = std::stoi(base_match[9].str(), nullptr);
  }


  std::vector<int> Ptcuts;
  if (pts > 0) // Set to all legs
  {
    pt1 = pt2 = pt3 = pt4 = pts;
    for (int i = 0; i < 4; ++i)
    {
      if (i <= EGcount)
        Ptcuts.push_back(pts);
      else
        Ptcuts.push_back(-10);
    }
  } else{
    Ptcuts.push_back(pt1);
    Ptcuts.push_back(pt2);
    Ptcuts.push_back(pt3);
    Ptcuts.push_back(pt4);
    std::sort(Ptcuts.begin(), Ptcuts.end(), std::greater<int>());
  }

  assert(EGcount == std::count_if(Ptcuts.begin(), Ptcuts.end(), [](int i){return i > 0;}));
  L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::MultiEGMass, this, Ptcuts.at(0), 
      Ptcuts.at(1), Ptcuts.at(2),  Ptcuts.at(3), Mcut, isIso, isER);

  return true;
}       // -----  end of function L1Menu2016::ParseMultiEGMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseCrossMu
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseCrossMu(const std::string& /*SeedName*/)
{
  //std::smatch base_match;
  //std::regex integer("L1_QuadJet([C]*)([0-9]+)");
  //if (std::regex_match(SeedName, base_match, integer))
  //{
    //if (base_match.size() != 2) return false;
    //bool isCentral = base_match.length(1) == 1;
    //unsigned int pt = std::stoi(base_match[2].str(), nullptr);
    //L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::QuadJet, this, pt, pt, pt, pt, isCentral);
    //return true;
  //}
  //else return false;
  return false;
}       // -----  end of function L1Menu2016::ParseCrossMu  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseMuerTauer
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseMuerTauer(const std::string& SeedName)
{
  int Mupt = -10;
  int Taupt = -10;
  bool isIsoTau = false;

  std::smatch base_match;
  std::regex integer("L1_Mu([0-9]+)(er|er2p1)_(Iso|)Tau(Jet|)([0-9]+)(er|er2p1)");
  if (std::regex_match(SeedName, base_match, integer))
  {
    Mupt =  std::stoi(base_match[1].str(), nullptr);
    isIsoTau = base_match.length(3) == 3;
    Taupt = std::stoi(base_match[5].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::Muer_TauJetEta2p17, this, Mupt, Taupt, isIsoTau);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseMuerTauer  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseMuEG
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseMuEG(const std::string& SeedName)
{
  const int muonQual = 2;
  int Mupt = -10;
  int EGpt = -10;
  bool isIsoEG = false;

  std::smatch base_match;
  std::regex integer("L1_Mu([0-9]+)_(Iso|)EG([0-9]+)");
  if (std::regex_match(SeedName, base_match, integer))
  {
    Mupt =  std::stoi(base_match[1].str(), nullptr);
    isIsoEG = base_match.length(2) == 3;
    EGpt = std::stoi(base_match[3].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::Mu_EG, this, Mupt, EGpt, isIsoEG, muonQual);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseMuEG  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseMuSum
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseMuSum(const std::string& SeedName)
{
  int Mupt = -10;
  int Sumpt = -10;

  std::smatch base_match;
  std::regex integerMuHTT("L1_Mu([0-9]+)_HTT([0-9]+)(er|)");
  std::regex integerMuerETM("L1_Mu([0-9]+)(er|er2p1)_ETM([0-9]+)");
  if (std::regex_match(SeedName, base_match, integerMuHTT))
  {
    Mupt =  std::stoi(base_match[1].str(), nullptr);
    Sumpt = std::stoi(base_match[2].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::Mu_HTT, this, Mupt, Sumpt);
    return true;
  }

  if (std::regex_match(SeedName, base_match, integerMuerETM))
  {
    Mupt =  std::stoi(base_match[1].str(), nullptr);
    assert(base_match.length(2) > 0);
    Sumpt = std::stoi(base_match[3].str(), nullptr);
    L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::Muer_ETM, this, Mupt, Sumpt);
    return true;
  }

  return false;
}       // -----  end of function L1Menu2016::ParseMuSum  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseComplexSingleMu
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseComplexSingleMu(const std::string& SeedName) 
{
  // This function overlap wiht ParseSingleObject. The code shouldn't come
  // here for simple singleMu trigger. Check for safety
  if (L1SeedFun.find(SeedName)!= L1SeedFun.end())
    return false;

  std::smatch base_match;
  std::regex integer("L1_Single(Mu)([0-9]+)([^0-9_]*)(_(EMTF|OMTF|BMTF))*(_Bx([-+0-9]+))*(_(Open|LowQ|HighQ))*");
  std::string L1object ="";
  std::string postfix = "";
  std::string muonType = "";
  std::string muonQual = "";
  int muonBx = 0;
  int imuonQual = 2;
  int imuonType = 0;
  float muonpt = -10;

  if (std::regex_match(SeedName, base_match, integer))
  {
    L1object = base_match[1];
    muonpt = std::stoi(base_match[2], nullptr);
    postfix =  base_match[3];
    if (base_match[4] != "")
    {
      muonType = base_match[5];
    }
    if (base_match[6]!="")
    {
      muonBx = std::stoi(base_match[7], nullptr);
    }
    if (base_match[8] != "")
    {
      muonQual = base_match[9];
    }
  } else return false;

  if (!muonQual.empty())
  {
    if (muonQual == "Open") imuonQual = 0;
    if (muonQual == "LowQ") imuonQual = 1;
    if (muonQual == "HighQ") imuonQual = 2;
  }

  if (!muonType.empty())
  {
    if (muonType == "BMTF") imuonType = 1;
    if (muonType == "OMTF") imuonType = 2;
    if (muonType == "EMTF") imuonType = 3;
  }
  assert(L1object == "Mu");
  bool isER = postfix=="er";
  L1SeedFun[SeedName] = std::bind(&L1AlgoFactory::ComplexSingleMu, this, muonpt,  isER, imuonQual, imuonType, muonBx);
  return true;
}       // -----  end of function L1Menu2016::ParseComplexSingleMu  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::FillLumiSection
//  Description:  
// ===========================================================================
bool L1Menu2016::FillLumiSection(int currentLumi)
{
  if (currentLumi == -1) return false;

  for(auto l1 : mL1Seed)
  {
    if(L1LSCount[l1.first].find(currentLumi) == L1LSCount[l1.first].end())
    {
      L1LSCount[l1.first][currentLumi] = 0;
    }
    if (l1.second.eventfire)
    {
      L1LSCount[l1.first][currentLumi]++;
    }
  }

  L1LSCount["Count"][currentLumi]++;
  
  return true;
}       // -----  end of function L1Menu2016::FillLumiSection  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::FillPileUpSec
//  Description:  
// ===========================================================================
bool L1Menu2016::FillPileUpSec()
{
  float pu = -1;
  //bool eFired = false;
  // Data
  if (event_->run > 1 && DataLSPU.find(event_->run) != DataLSPU.end())
  {
    if (DataLSPU[event_->run].find(event_->lumi) != DataLSPU[event_->run].end())
    {
      pu = DataLSPU[event_->run][event_->lumi];
    }
  }

  // MC
  if (event_->run == 1)
  {
    pu = event_->nPV;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Fill Rate per PU ~~~~~
  for(auto col : ColumnMap)
  {
    col.second->FillPileUpSec(pu);
  }

  return true;
}       // -----  end of function L1Menu2016::FillPileUpSec  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::PrintCSV
//  Description:  
// ===========================================================================
bool L1Menu2016::PrintCSV(std::ostream &out)
{
  if (!writecsv) return false;

  // L1Seeds
  std::vector<std::string> csvout;
  std::stringstream ss;
  ss << "L1Bit"         
    << "," << "L1SeedName"<< "," ;
  csvout.push_back(ss.str());
  for(auto sed : vL1Seed)
  {
    auto seed = mL1Seed[sed];
    ss.str("");
    ss << seed.bit
      << "," << sed<< "," ;
    csvout.push_back(ss.str());
  }

  // POG
  ss.str("");
  csvout.push_back(ss.str());

  int idx = 1000;
  for(auto pog : POGMap)
  {
    ss.str("");
    ss << idx++ << "," << pog.first <<",";
    csvout.push_back(ss.str());
  }

  // PAG
  ss.str("");
  csvout.push_back(ss.str());

  idx = 2000;
  for(auto pag : PAGMap)
  {
    ss.str("");
    ss << idx++ << "," << pag.first <<",";
    csvout.push_back(ss.str());
  }

  // Total
  csvout.push_back("");
  csvout.push_back("9999,Total rate,");

  for(auto col : ColumnMap)
  {
    col.second->PrintCSV(csvout, scale);
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Print out additional info ~~~~~
  std::vector<std::string>::iterator csvt = csvout.begin();
  ss.str("");
  ss <<"Type"<<","<<"PAG"<<","<<"Comment";
  csvt++->append(ss.str());
  // L1Seeds
  for(auto sed : vL1Seed)
  {
    auto seed = mL1Seed[sed];
    ss.str("");
    std::copy(seed.POG.begin(), seed.POG.end(), std::ostream_iterator<std::string>(ss, "|"));
    ss.seekp(ss.str().length()-1); //remove trailing |
    ss<<",";
    std::copy(seed.PAG.begin(), seed.PAG.end(), std::ostream_iterator<std::string>(ss, "|"));
    ss.seekp(ss.str().length()-1); //remove trailing |
    ss<<","<<seed.comment;
    csvt++->append(ss.str());
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Writing out to file ~~~~~
  for(auto i : csvout)
  {
    out << i <<std::endl;
  }
  return true;
}       // -----  end of function L1Menu2016::PrintCSV  -----


// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ReadDataPU
//  Description:  
// ===========================================================================
bool L1Menu2016::ReadDataPU() 
{
  const std::string pucsv = L1ConfigStr["Lumilist"];
  std::ifstream csvfile(pucsv);
  if (!csvfile)
  {
    std::cout << "Data PU CSV File "<<pucsv<<" is not found !"<<std::endl;
    return false;
  }

  std::string line;
  DataLSPU.clear();
  DataFillNO.clear();
  std::getline(csvfile, line); // Skip the first line;
  while (std::getline(csvfile, line))
  {
    std::istringstream iss(line);
    std::string seed;
    char c;
    int Fill, Run, LS;
    float pileup;
    iss >> Fill >> c >> Run >> c >> LS >> c >> pileup;
    DataLSPU[Run][LS] = pileup;
    DataFillNO[Run] = Fill;
  }

  return true;
}       // -----  end of function L1Menu2016::ReadDataPU  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CalLocalHT
//  Description:  
// ===========================================================================
void L1Menu2016::CalLocalHT(float &HTTcut, bool withHF)
{
  float sumJetHt= 0;
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    Float_t eta = upgrade_->jetEta.at(ue);
    if (pt >= L1Config["SumJetET"])
    {
      if (withHF)
      {
        sumJetHt += pt;
      }
      else{
        if ( fabs(eta) <= L1Config["SumJetEta"] )
          sumJetHt += pt;
      }
    }
  }
  HTTcut = sumJetHt;
  return;

}       // -----  end of function L1Menu2016::CalLocalHT  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CalLocalHTM
//  Description:  
// ===========================================================================
void L1Menu2016::CalLocalHTM(float &HTMcut)
{
  
  TLorentzVector temp(0, 0,0,0);
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    Float_t eta = upgrade_->jetEta.at(ue);
    if (pt >= L1Config["SumJetET"] && fabs(eta) <= L1Config["SumJetEta"] )
    {
      TLorentzVector jet(0, 0,0,0);
      jet.SetPtEtaPhiE(upgrade_->jetEt.at(ue),
          upgrade_->jetEta.at(ue),
          upgrade_->jetPhi.at(ue), 
          0);
      temp -= jet;
    }
  }
  HTMcut = temp.Pt();
}       // -----  end of function L1Menu2016::CalLocalHTM  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::ParseRanges
//  Description:  
// ===========================================================================
bool L1Menu2016::ParseRanges(const std::string Config,  std::vector<std::pair<unsigned int, unsigned int> >& Container)
{
  if (L1ConfigStr[Config] == "") return false;
  
  std::regex pattern("\\[\\s*([0-9]+,\\s*[0-9]+)\\s*\\]");
  std::regex bounds("([0-9]+),\\s*([0-9]+)");
  std::smatch base_match;

  for (std::sregex_token_iterator i(L1ConfigStr[Config].begin(), L1ConfigStr[Config].end(), pattern, 1); 
      i != std::sregex_token_iterator(); ++i) 
  {
	std::string match_str = i->str(); 
    if (std::regex_match(match_str, base_match, bounds))
    {
      unsigned lead =  std::stoi(base_match[1], nullptr);
      unsigned sec =  std::stoi(base_match[2], nullptr);
      assert( sec >= lead );
      Container.push_back(std::make_pair(lead, sec));
    }
  }

  return true;
}       // -----  end of function L1Menu2016::ParseRanges  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CheckLS
//  Description:  Return whether to skip this LS
// ===========================================================================
bool L1Menu2016::CheckLS(unsigned int currentLumi) const
{
  for(auto p : pLS)
  {
    if (currentLumi >= p.first && currentLumi <= p.second)
    {
      if (L1Config.at("doScanLS"))
        std::cout << "LS"<<currentLumi << " within "<< L1ConfigStr.at("SelectLS") << " % "<< fChain->GetCurrentFile()->GetName()  << std::endl;
      return false;
    }
  }
  return true;
}       // -----  end of function L1Menu2016::CheckLS  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CheckBX
//  Description:  Return whether to skip this BX
// ===========================================================================
bool L1Menu2016::CheckBX(unsigned int currentBX) const
{
  for(auto p : pBX)
  {
    if (currentBX >= p.first && currentBX <= p.second)
    {
      return false;
    }
  }
  return true;
}       // -----  end of function L1Menu2016::CheckBX  -----

// this function selects the first 6 and last 2 bx in 2018 48b
bool L1Menu2016::BXReweight_is_1_to_6_47_48(int currentBX)
{
	int a[] = {62,63,64,65,66,67,108,109,117,118,119,120,121,122,163,164,196,197,198,199,200,201,242,243,251,252,253,254,255,256,297,298,306,307,308,309,310,311,352,353,385,386,387,388,389,390,431,432,440,441,442,443,444,445,486,487,495,496,497,498,499,500,541,542,574,575,576,577,578,579,620,621,629,630,631,632,633,634,675,676,684,685,686,687,688,689,730,731,767,768,769,770,771,772,813,814,822,823,824,825,826,827,868,869,901,902,903,904,905,906,947,948,956,957,958,959,960,961,1002,1003,1011,1012,1013,1014,1015,1016,1057,1058,1090,1091,1092,1093,1094,1095,1136,1137,1145,1146,1147,1148,1149,1150,1191,1192,1200,1201,1202,1203,1204,1205,1246,1247,1279,1280,1281,1282,1283,1284,1325,1326,1334,1335,1336,1337,1338,1339,1380,1381,1389,1390,1391,1392,1393,1394,1435,1436,1468,1469,1470,1471,1472,1473,1514,1515,1523,1524,1525,1526,1527,1528,1569,1570,1578,1579,1580,1581,1582,1583,1624,1625,1661,1662,1663,1664,1665,1666,1707,1708,1716,1717,1718,1719,1720,1721,1762,1763,1795,1796,1797,1798,1799,1800,1841,1842,1850,1851,1852,1853,1854,1855,1896,1897,1905,1906,1907,1908,1909,1910,1951,1952,1984,1985,1986,1987,1988,1989,2030,2031,2039,2040,2041,2042,2043,2044,2085,2086,2094,2095,2096,2097,2098,2099,2140,2141,2173,2174,2175,2176,2177,2178,2219,2220,2228,2229,2230,2231,2232,2233,2274,2275,2283,2284,2285,2286,2287,2288,2329,2330,2362,2363,2364,2365,2366,2367,2408,2409,2417,2418,2419,2420,2421,2422,2463,2464,2472,2473,2474,2475,2476,2477,2518,2519,2555,2556,2557,2558,2559,2560,2601,2602,2610,2611,2612,2613,2614,2615,2656,2657,2689,2690,2691,2692,2693,2694,2735,2736,2744,2745,2746,2747,2748,2749,2790,2791,2799,2800,2801,2802,2803,2804,2845,2846,2878,2879,2880,2881,2882,2883,2924,2925,2933,2934,2935,2936,2937,2938,2979,2980,2988,2989,2990,2991,2992,2993,3034,3035,3067,3068,3069,3070,3071,3072,3113,3114,3122,3123,3124,3125,3126,3127,3168,3169,3177,3178,3179,3180,3181,3182,3223,3224,3256,3257,3258,3259,3260,3261,3302,3303,3311,3312,3313,3314,3315,3316,3357,3358,3366,3367,3368,3369,3370,3371,3412,3413};
	for(int i = 0; i < 424; i++)
	{
		if(currentBX == a[i])
		return true;
	}
return false;
}

//this function selects bx 1,2 and 8 in 8b4e in 2017
bool L1Menu2016::BXReweight_is128(int currentBX)
{
	int a[] = {46,47,53,58,59,65,70,71,77,82,83,89,121,122,128,133,134,140,145,146,152,157,158,164,172,173,179,184,185,191,196,197,203,208,209,215,223,224,230,235,236,242,247,248,254,259,260,266,274,275,281,286,287,293,298,299,305,310,311,317,349,350,356,361,362,368,373,374,380,385,386,392,400,401,407,412,413,419,424,425,431,436,437,443,451,452,458,463,464,470,475,476,482,487,488,494,502,503,509,514,515,521,526,527,533,538,539,545,577,578,584,589,590,596,601,602,608,613,614,620,628,629,635,640,641,647,652,653,659,664,665,671,679,680,686,691,692,698,703,704,710,715,716,722,730,731,737,742,743,749,754,755,761,766,767,773,826,827,833,838,839,845,850,851,857,862,863,869,877,878,884,889,890,896,901,902,908,913,914,920,928,929,935,940,941,947,952,953,959,964,965,971,1003,1004,1010,1015,1016,1022,1027,1028,1034,1039,1040,1046,1054,1055,1061,1066,1067,1073,1078,1079,1085,1090,1091,1097,1105,1106,1112,1117,1118,1124,1129,1130,1136,1141,1142,1148,1156,1157,1163,1168,1169,1175,1180,1181,1187,1192,1193,1199,1231,1232,1238,1243,1244,1250,1255,1256,1262,1267,1268,1274,1282,1283,1289,1294,1295,1301,1306,1307,1313,1318,1319,1325,1333,1334,1340,1345,1346,1352,1357,1358,1364,1369,1370,1376,1384,1385,1391,1396,1397,1403,1408,1409,1415,1420,1421,1427,1459,1460,1466,1471,1472,1478,1483,1484,1490,1495,1496,1502,1510,1511,1517,1522,1523,1529,1534,1535,1541,1546,1547,1553,1561,1562,1568,1573,1574,1580,1585,1586,1592,1597,1598,1604,1612,1613,1619,1624,1625,1631,1636,1637,1643,1648,1649,1655,1720,1721,1727,1732,1733,1739,1744,1745,1751,1756,1757,1763,1771,1772,1778,1783,1784,1790,1795,1796,1802,1807,1808,1814,1822,1823,1829,1834,1835,1841,1846,1847,1853,1858,1859,1865,1897,1898,1904,1909,1910,1916,1921,1922,1928,1933,1934,1940,1948,1949,1955,1960,1961,1967,1972,1973,1979,1984,1985,1991,1999,2000,2006,2011,2012,2018,2023,2024,2030,2035,2036,2042,2050,2051,2057,2062,2063,2069,2074,2075,2081,2086,2087,2093,2125,2126,2132,2137,2138,2144,2149,2150,2156,2161,2162,2168,2176,2177,2183,2188,2189,2195,2200,2201,2207,2212,2213,2219,2227,2228,2234,2239,2240,2246,2251,2252,2258,2263,2264,2270,2278,2279,2285,2290,2291,2297,2302,2303,2309,2314,2315,2321,2353,2354,2360,2365,2366,2372,2377,2378,2384,2389,2390,2396,2404,2405,2411,2416,2417,2423,2428,2429,2435,2440,2441,2447,2455,2456,2462,2467,2468,2474,2479,2480,2486,2491,2492,2498,2506,2507,2513,2518,2519,2525,2530,2531,2537,2542,2543,2549,2614,2615,2621,2626,2627,2633,2638,2639,2645,2650,2651,2657,2665,2666,2672,2677,2678,2684,2689,2690,2696,2701,2702,2708,2716,2717,2723,2728,2729,2735,2740,2741,2747,2752,2753,2759,2791,2792,2798,2803,2804,2810,2815,2816,2822,2827,2828,2834,2842,2843,2849,2854,2855,2861,2866,2867,2873,2878,2879,2885,2893,2894,2900,2905,2906,2912,2917,2918,2924,2929,2930,2936,2944,2945,2951,2956,2957,2963,2968,2969,2975,2980,2981,2987,3019,3020,3026,3031,3032,3038,3043,3044,3050,3055,3056,3062,3070,3071,3077,3082,3083,3089,3094,3095,3101,3106,3107,3113,3121,3122,3128,3133,3134,3140,3145,3146,3152,3157,3158,3164,3172,3173,3179,3184,3185,3191,3196,3197,3203,3208,3209,3215,3247,3248,3254,3259,3260,3266,3271,3272,3278,3283,3284,3290,3298,3299,3305,3310,3311,3317,3322,3323,3329,3334,3335,3341,3349,3350,3356,3361,3362,3368,3373,3374,3380,3385,3386,3392,3400,3401,3407,3412,3413,3419,3424,3425,3431,3436,3437,3443};

	for(int i = 0; i < 696; i++)
	{
		if(currentBX == a[i])
		return true;
	}
return false;
}

//this function selects bx 3 to 7 in 8b4e in 2017
bool L1Menu2016::BXReweight_is34567(int currentBX)
{
	int a[] = {48,49,50,51,52,60,61,62,63,64,72,73,74,75,76,84,85,86,87,88,123,124,125,126,127,135,136,137,138,139,147,148,149,150,151,159,160,161,162,163,174,175,176,177,178,186,187,188,189,190,198,199,200,201,202,210,211,212,213,214,225,226,227,228,229,237,238,239,240,241,249,250,251,252,253,261,262,263,264,265,276,277,278,279,280,288,289,290,291,292,300,301,302,303,304,312,313,314,315,316,351,352,353,354,355,363,364,365,366,367,375,376,377,378,379,387,388,389,390,391,402,403,404,405,406,414,415,416,417,418,426,427,428,429,430,438,439,440,441,442,453,454,455,456,457,465,466,467,468,469,477,478,479,480,481,489,490,491,492,493,504,505,506,507,508,516,517,518,519,520,528,529,530,531,532,540,541,542,543,544,579,580,581,582,583,591,592,593,594,595,603,604,605,606,607,615,616,617,618,619,630,631,632,633,634,642,643,644,645,646,654,655,656,657,658,666,667,668,669,670,681,682,683,684,685,693,694,695,696,697,705,706,707,708,709,717,718,719,720,721,732,733,734,735,736,744,745,746,747,748,756,757,758,759,760,768,769,770,771,772,828,829,830,831,832,840,841,842,843,844,852,853,854,855,856,864,865,866,867,868,879,880,881,882,883,891,892,893,894,895,903,904,905,906,907,915,916,917,918,919,930,931,932,933,934,942,943,944,945,946,954,955,956,957,958,966,967,968,969,970,1005,1006,1007,1008,1009,1017,1018,1019,1020,1021,1029,1030,1031,1032,1033,1041,1042,1043,1044,1045,1056,1057,1058,1059,1060,1068,1069,1070,1071,1072,1080,1081,1082,1083,1084,1092,1093,1094,1095,1096,1107,1108,1109,1110,1111,1119,1120,1121,1122,1123,1131,1132,1133,1134,1135,1143,1144,1145,1146,1147,1158,1159,1160,1161,1162,1170,1171,1172,1173,1174,1182,1183,1184,1185,1186,1194,1195,1196,1197,1198,1233,1234,1235,1236,1237,1245,1246,1247,1248,1249,1257,1258,1259,1260,1261,1269,1270,1271,1272,1273,1284,1285,1286,1287,1288,1296,1297,1298,1299,1300,1308,1309,1310,1311,1312,1320,1321,1322,1323,1324,1335,1336,1337,1338,1339,1347,1348,1349,1350,1351,1359,1360,1361,1362,1363,1371,1372,1373,1374,1375,1386,1387,1388,1389,1390,1398,1399,1400,1401,1402,1410,1411,1412,1413,1414,1422,1423,1424,1425,1426,1461,1462,1463,1464,1465,1473,1474,1475,1476,1477,1485,1486,1487,1488,1489,1497,1498,1499,1500,1501,1512,1513,1514,1515,1516,1524,1525,1526,1527,1528,1536,1537,1538,1539,1540,1548,1549,1550,1551,1552,1563,1564,1565,1566,1567,1575,1576,1577,1578,1579,1587,1588,1589,1590,1591,1599,1600,1601,1602,1603,1614,1615,1616,1617,1618,1626,1627,1628,1629,1630,1638,1639,1640,1641,1642,1650,1651,1652,1653,1654,1722,1723,1724,1725,1726,1734,1735,1736,1737,1738,1746,1747,1748,1749,1750,1758,1759,1760,1761,1762,1773,1774,1775,1776,1777,1785,1786,1787,1788,1789,1797,1798,1799,1800,1801,1809,1810,1811,1812,1813,1824,1825,1826,1827,1828,1836,1837,1838,1839,1840,1848,1849,1850,1851,1852,1860,1861,1862,1863,1864,1899,1900,1901,1902,1903,1911,1912,1913,1914,1915,1923,1924,1925,1926,1927,1935,1936,1937,1938,1939,1950,1951,1952,1953,1954,1962,1963,1964,1965,1966,1974,1975,1976,1977,1978,1986,1987,1988,1989,1990,2001,2002,2003,2004,2005,2013,2014,2015,2016,2017,2025,2026,2027,2028,2029,2037,2038,2039,2040,2041,2052,2053,2054,2055,2056,2064,2065,2066,2067,2068,2076,2077,2078,2079,2080,2088,2089,2090,2091,2092,2127,2128,2129,2130,2131,2139,2140,2141,2142,2143,2151,2152,2153,2154,2155,2163,2164,2165,2166,2167,2178,2179,2180,2181,2182,2190,2191,2192,2193,2194,2202,2203,2204,2205,2206,2214,2215,2216,2217,2218,2229,2230,2231,2232,2233,2241,2242,2243,2244,2245,2253,2254,2255,2256,2257,2265,2266,2267,2268,2269,2280,2281,2282,2283,2284,2292,2293,2294,2295,2296,2304,2305,2306,2307,2308,2316,2317,2318,2319,2320,2355,2356,2357,2358,2359,2367,2368,2369,2370,2371,2379,2380,2381,2382,2383,2391,2392,2393,2394,2395,2406,2407,2408,2409,2410,2418,2419,2420,2421,2422,2430,2431,2432,2433,2434,2442,2443,2444,2445,2446,2457,2458,2459,2460,2461,2469,2470,2471,2472,2473,2481,2482,2483,2484,2485,2493,2494,2495,2496,2497,2508,2509,2510,2511,2512,2520,2521,2522,2523,2524,2532,2533,2534,2535,2536,2544,2545,2546,2547,2548,2616,2617,2618,2619,2620,2628,2629,2630,2631,2632,2640,2641,2642,2643,2644,2652,2653,2654,2655,2656,2667,2668,2669,2670,2671,2679,2680,2681,2682,2683,2691,2692,2693,2694,2695,2703,2704,2705,2706,2707,2718,2719,2720,2721,2722,2730,2731,2732,2733,2734,2742,2743,2744,2745,2746,2754,2755,2756,2757,2758,2793,2794,2795,2796,2797,2805,2806,2807,2808,2809,2817,2818,2819,2820,2821,2829,2830,2831,2832,2833,2844,2845,2846,2847,2848,2856,2857,2858,2859,2860,2868,2869,2870,2871,2872,2880,2881,2882,2883,2884,2895,2896,2897,2898,2899,2907,2908,2909,2910,2911,2919,2920,2921,2922,2923,2931,2932,2933,2934,2935,2946,2947,2948,2949,2950,2958,2959,2960,2961,2962,2970,2971,2972,2973,2974,2982,2983,2984,2985,2986,3021,3022,3023,3024,3025,3033,3034,3035,3036,3037,3045,3046,3047,3048,3049,3057,3058,3059,3060,3061,3072,3073,3074,3075,3076,3084,3085,3086,3087,3088,3096,3097,3098,3099,3100,3108,3109,3110,3111,3112,3123,3124,3125,3126,3127,3135,3136,3137,3138,3139,3147,3148,3149,3150,3151,3159,3160,3161,3162,3163,3174,3175,3176,3177,3178,3186,3187,3188,3189,3190,3198,3199,3200,3201,3202,3210,3211,3212,3213,3214,3249,3250,3251,3252,3253,3261,3262,3263,3264,3265,3273,3274,3275,3276,3277,3285,3286,3287,3288,3289,3300,3301,3302,3303,3304,3312,3313,3314,3315,3316,3324,3325,3326,3327,3328,3336,3337,3338,3339,3340,3351,3352,3353,3354,3355,3363,3364,3365,3366,3367,3375,3376,3377,3378,3379,3387,3388,3389,3390,3391,3402,3403,3404,3405,3406,3414,3415,3416,3417,3418,3426,3427,3428,3429,3430,3438,3439,3440,3441,3442};

	for(int i = 0; i < 1160; i++)
	{
		if(currentBX == a[i])
		return true;
	}
return false;
}

// this function selects the first 6 and last 2 bx in 2018 12b (Fill 7358)
bool L1Menu2016::BXReweight_is_1_to_6_11_12(int currentBX)
{
	int a[] = {750, 751, 752, 753, 754, 755, 760, 761, 1644, 1645, 1646, 1647, 1648, 1649, 1654, 1655};
	for(int i = 0; i < 16; i++)
	{
		if(currentBX == a[i])
		return true;
	}
return false;
}

// this function selects bx 5 to 10 in 2018 12b (Fill 7358)
bool L1Menu2016::BXReweight_is_5_to_10(int currentBX)
{
	int a[] = {754, 755, 756, 757, 758, 759, 1648, 1649, 1650, 1651, 1652, 1653};
	for(int i = 0; i < 12; i++)
	{
		if(currentBX == a[i])
		return true;
	}
return false;
}

// this function selects target BX in 2018 48b (Fill 7118)
bool L1Menu2016::Found_BX_in_48b(int targetBX, int currentBX)
{
	int a[] = {62,117,196,251,306,385,440,495,574,629,684,767,822,901,956,1011,1090,1145,1200,1279,1334,1389,1468,1523,1578,1661,1716,1795,1850,1905,1984,2039,2094,2173,2228,2283,2362,2417,2472,2555,2610,2689,2744,2799,2878,2933,2988,3067,3122,3177,3256,3311,3366};
	for(int i = 0; i < 53; i++)
	{
		if(currentBX == a[i] + targetBX)
		return true;
	}
return false;
}

// this function selects target BX in 2018 12b (Fill 7358)
bool L1Menu2016::Found_BX_in_12b(int targetBX, int currentBX)
{
	int a[] = {750, 1644};
	for(int i = 0; i < 2; i++)
	{
		if(currentBX == a[i] + targetBX)
		return true;
	}
return false;
}

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::CalLocalETM
//  Description:  
// ===========================================================================
void L1Menu2016::CalLocalETM(float &ETMcut, bool withHF)
{

  if (!l1CaloTower_) return;

  TVector2 revec(0,0);
  
  // Ignore Tower 28
  int ietamax = 29;
  if (withHF)
    ietamax = 99;
  float metX =0;
  float metY =0;


  for(int jTower=0; jTower< l1CaloTower_ ->nTower; ++jTower){
    Int_t ieta = l1CaloTower_->ieta[jTower];
    Int_t iphi = l1CaloTower_->iphi[jTower];
    Int_t iet  = l1CaloTower_->iet[jTower];
    Double_t phi = (Double_t)iphi * TMath::Pi()/36.;
    Double_t et = 0.5 * (Double_t)iet;
    if (abs(ieta) == 28) continue;
    if(abs(ieta) < ietamax){
      metX -= et * TMath::Cos(phi);
      metY -= et * TMath::Sin(phi);
    }
  }

  revec.Set(metX, metY);
  ETMcut = revec.Mod();
}       // -----  end of function L1Menu2016::CalLocalETM  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Menu2016::TokenGroups
//  Description:  
// ===========================================================================
std::vector<std::string> L1Menu2016::TokenGroups(std::string instring) const
{
  std::vector<std::string>  temp;
  if (instring.empty()) return temp;

  boost::char_separator<char> sep(",.;|- ");
  tokenizer tokens(instring, sep);
  for(auto &t : tokens)
  {
    temp.push_back(t);
  }
  return temp;
}       // -----  end of function L1Menu2016::TokenGroups  -----
