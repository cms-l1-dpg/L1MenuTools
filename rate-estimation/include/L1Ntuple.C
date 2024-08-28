#include "L1Ntuple.h"

Long64_t L1Ntuple::GetEntries()
{
  return nentries_;
}
 
L1Ntuple::L1Ntuple()
{
  doEvent         = true;
  domuonreco      = true;
  dol1extra       = true;
  dol1emuextra    = true;
  dol1menu        = true;
  doBitWiseLayer1 = true;
  dol1CaloTower   = true;
  dol1uGT         = true;
  dol1unpackuGT   = true;
  doRecoJet       = true;
  doRecoMet       = true;
  doRecoEle       = true;
  doRecoMuon      = true;
  doRecoTau       = true;
  doRecoFilter    = true;
  doRecoVtx       = true;

  fChain             = nullptr;
  fEvent             = nullptr;
  fl1uGT             = nullptr;
  fl1unpackuGT       = nullptr;
  ftreeEvent         = nullptr;
  ftreemuon          = nullptr;
  ftreeExtra         = nullptr;
  ftreeCaloTower     = nullptr;
  ftreeMenu          = nullptr;
  ftreeEmuExtra      = nullptr;
  ftreeRecoJet       = nullptr;
  ftreeRecoMet       = nullptr;
  ftreeRecoEle       = nullptr;
  ftreeRecoMuon      = nullptr;
  ftreeRecoTau       = nullptr;
  ftreeRecoFilter    = nullptr;
  ftreeRecoVtx       = nullptr;
  ftreeUpgradeLayer1 = nullptr;

  event_          = nullptr;
  upgrade_        = nullptr;
  l1CaloTower_    = nullptr;
  upgrade_lyr1_   = nullptr;  
  recoJet_        = nullptr;
  recoSum_        = nullptr;
  recoEle_        = nullptr;
  recoMuon_       = nullptr;
  recoTau_        = nullptr;
  recoFilter_     = nullptr;
  recoVtx_        = nullptr;
  l1uGT_          = nullptr;
  l1unpackuGT_    = nullptr;
  MainTreePath = "l1UpgradeEmuTree/L1UpgradeTree";
  CaloTreePath = "l1CaloTowerEmuTree/L1CaloTowerTree";
  uGTTreePath  = "l1uGTEmuTree/L1uGTTree";
  EventTreePath = "l1EventTree/L1EventTree";
}

L1Ntuple::L1Ntuple(const std::string & fname)
{
  L1Ntuple();
  Open(fname);
}

bool L1Ntuple::OpenWithList(const std::string & fname)
{
  if (!OpenNtupleList(fname)) exit(0);
  if (!CheckFirstFile())      exit(0);
  if (!OpenWithoutInit())     exit(0);

  std::cout.flush();std::cout<<"Going to init the available trees..."<<std::endl;std::cout.flush();
  Init();

  return true;
}

bool L1Ntuple::Open(const std::string & fname)
{
  listNtuples.push_back(fname);

  if (!CheckFirstFile())  exit(0);
  if (!OpenWithoutInit()) exit(0);

  std::cout.flush();std::cout<<"Going to init the available trees..."<<std::endl;std::cout.flush();
  Init();

  return true;
}

bool L1Ntuple::OpenNtupleList(const std::string & fname)
{
  std::ifstream flist(fname.c_str());
  if (!flist)
  {
    std::cout << "File "<<fname<<" is not found !"<<std::endl;
    return false;
  }

  while(!flist.eof())
  {
    std::string str;
    getline(flist,str);
    if (!flist.fail())
    {
      if (str!="") listNtuples.push_back(str);
    }
  }

  return true;
}

bool L1Ntuple::CheckFirstFile()
{
  if (listNtuples.size()==0) return false;

  rf = TFile::Open(listNtuples[0].c_str());

  if (rf==0) return false;
  if (rf->IsOpen()==0) return false;

  TTree * myChain           = (TTree*) rf->Get(MainTreePath.c_str());
  TTree * mytreel1CaloTower = (TTree*) rf->Get(CaloTreePath.c_str());
  TTree * mytreel1uGT       = (TTree*) rf->Get(uGTTreePath.c_str());
  TTree * mytreel1unpackuGT = (TTree*) rf->Get("l1uGTTree/L1uGTTree");
  TTree * mytreeEvent    = (TTree*) rf->Get(EventTreePath.c_str());
  TTree * mytreemuon     = (TTree*) rf->Get("l1MuonRecoTreeProducer/MuonRecoTree");
  TTree * mytreeExtra    = (TTree*) rf->Get("l1ExtraTreeProducer/L1ExtraTree");
  TTree * mytreeEmuExtra = (TTree*) rf->Get("l1EmulatorExtraTree/L1ExtraTree");
  TTree * mytreeMenu     = (TTree*) rf->Get("l1MenuTreeProducer/L1MenuTree");
  TTree * mytreeRecoJet  = (TTree*) rf->Get("l1JetRecoTree/JetRecoTree");
  TTree * mytreeRecoMet  = nullptr;
  //TTree * mytreeRecoMet  = (TTree*) rf->Get("l1MetFilterRecoTree/MetFilterRecoTree");
  TTree * mytreeRecoEle  = (TTree*) rf->Get("l1ElectronRecoTree/ElectronRecoTree");
  TTree * mytreeRecoMuon = (TTree*) rf->Get("l1MuonRecoTree/Muon2RecoTree");
  TTree * mytreeRecoTau  = (TTree*) rf->Get("l1TauRecoTree/TauRecoTree");
  TTree * mytreeRecoFilter  = (TTree*) rf->Get("l1MetFilterRecoTree/MetFilterRecoTree");
  TTree * mytreeRecoVertex  = (TTree*) rf->Get("l1RecoTree/RecoTree");
  TTree * mytreeUpgradeLayer1 = (TTree*) rf->Get("l1UpgradeBitwiseTree/L1UpgradeTree");
  
  if (!myChain) {
    std::cout<<"L1Tree not found .... "<<std::endl;
    return false;
  } else {
    std::cout<<"Main tree is found at " << MainTreePath <<std::endl;
  }

  if (!mytreeUpgradeLayer1) {
    std::cout<<"L1TreeUpgradeTree w BitwiseLayer1 not found .... "<<std::endl;
    doBitWiseLayer1 = false;
  } else {
    std::cout<<"L1TreeUpgradeTree w BitwiseLayer1 is found .." <<std::endl;
  }

  if (!mytreel1CaloTower) {
    std::cout<<"L1CaloTower tree not found .... "<<std::endl;
    dol1CaloTower = false;
  } else {
    std::cout<<"L1CaloTower tree is found at " << CaloTreePath <<std::endl;
  }

  if (!mytreel1uGT) {
    std::cout<<"L1uGTTree tree not found .... "<<std::endl;
    dol1uGT = false;
  } else {
    std::cout<<"L1uGTTree tree is found at " << uGTTreePath <<std::endl;
  }

  if (!mytreel1unpackuGT) 
    dol1unpackuGT = false;
  
  if (!mytreeEvent) {
    std::cout<<"EventTree not found, it will be skipped..."<<std::endl;
    doEvent=false;
  } else
  {
    std::cout << "EventTree is found ..."<<std::endl;
  }

  if (!mytreeRecoJet) {
    std::cout<<"RecoJet not found, it will be skipped..."<<std::endl;
    doRecoJet=false;
  } else
  {
    std::cout << "RecoJet is found ..."<<std::endl;
  }

  if (!mytreeRecoMet) {
    std::cout<<"RecoMet not found, it will be skipped..."<<std::endl;
    doRecoMet=false;
  } else
  {
    std::cout << "RecoMet is found ..."<<std::endl;
  }
  doRecoMet=false;

  if (!mytreeRecoEle) {
    std::cout<<"RecoEle not found, it will be skipped..."<<std::endl;
    doRecoEle=false;
  } else
  {
    std::cout << "RecoEle is found ..."<<std::endl;
  }

  if (!mytreeRecoMuon) {
    std::cout<<"RecoMuon not found, it will be skipped..."<<std::endl;
    doRecoMuon=false;
  } else
  {
    std::cout << "RecoMuon is found ..."<<std::endl;
  }

  if (!mytreeRecoTau) {
    std::cout<<"RecoTau not found, it will be skipped..."<<std::endl;
    doRecoTau=false;
  } else
  {
    std::cout << "RecoTau is found ..."<<std::endl;
  }

  if (!mytreeRecoFilter) {
    std::cout<<"RecoFilter not found, it will be skipped..."<<std::endl;
    doRecoFilter=false;
  } else
  {
    std::cout << "RecoTau is found ..."<<std::endl;
  }

  if (!mytreeRecoVertex) {
    std::cout<<"recoVertex not found, it will be skipped..."<<std::endl;
    doRecoVtx=false;
  } else
  {
    std::cout << "RecoVertex is found ..."<<std::endl;
  }

  if (!mytreemuon) {
    std::cout<<"MuonRecoTree not found, it will be skipped..."<<std::endl;
    domuonreco=false;
  }
  else
  {
    std::cout << "MuonRecoTree is found ..."<<std::endl;
  }

  if (!mytreeExtra) {
    std::cout<<"L1ExtraTree not found, it will be skipped..."<<std::endl;
    dol1extra=false;
  }
  else
  {
    std::cout << "L1ExtraTree is found ..."<<std::endl;
  }
  if(!mytreeEmuExtra){
    std::cout<<"L1EmuExtraTree not found, it will be skipped..."<<std::endl;
    dol1emuextra=false;
  }
  else{
    std::cout << "L1EmuExtraTree is found ..."<<std::endl;
  }

  if (!mytreeMenu) {
    std::cout<<"L1MenuTree not found, it will be skipped..."<<std::endl;
    dol1menu=false;
  }
  else
  {
    std::cout << "L1MenuTree is found ..."<<std::endl;
  }

  return true;
}


bool L1Ntuple::OpenWithoutInit()
{
  fChain             = new TChain(MainTreePath.c_str());
  ftreeEvent         = new TChain(EventTreePath.c_str());
  ftreemuon          = new TChain("l1MuonRecoTreeProducer/MuonRecoTree");
  ftreeExtra         = new TChain("l1ExtraTreeProducer/L1ExtraTree");
  ftreeEmuExtra      = new TChain("l1EmulatorExtraTree/L1ExtraTree");
  ftreeMenu          = new TChain("l1MenuTreeProducer/L1MenuTree");
  ftreeRecoJet       = new TChain("l1JetRecoTree/JetRecoTree");
  //ftreeRecoMet     = new TChain("l1MetFilterRecoTree/MetFilterRecoTree");
  ftreeCaloTower     = new TChain(CaloTreePath.c_str());
  ftreeRecoEle       = new TChain("l1ElectronRecoTree/ElectronRecoTree");
  ftreeRecoMuon      = new TChain("l1MuonRecoTree/Muon2RecoTree");
  ftreeRecoTau       = new TChain("l1TauRecoTree/TauRecoTree");
  ftreeRecoFilter    = new TChain("l1MetFilterRecoTree/MetFilterRecoTree");
  ftreeRecoVtx       = new TChain("l1RecoTree/RecoTree");
  ftreeUpgradeLayer1 = new TChain("l1UpgradeBitwiseTree/L1UpgradeTree");
  fl1uGT             = new TChain(uGTTreePath.c_str());
  fl1unpackuGT       = new TChain("l1uGTTree/L1uGTTree");

  for (unsigned int i=0;i<listNtuples.size();i++)
  {
    std::cout << " -- Adding " << listNtuples[i] << std::endl;
    fChain->Add(listNtuples[i].c_str());

    if (doEvent)         ftreeEvent         -> Add(listNtuples[i].c_str());
    if (domuonreco)      ftreemuon          -> Add(listNtuples[i].c_str());
    if (dol1extra)       ftreeExtra         -> Add(listNtuples[i].c_str());
    if (dol1emuextra)    ftreeEmuExtra      -> Add(listNtuples[i].c_str());
    if (dol1menu)        ftreeMenu          -> Add(listNtuples[i].c_str());
    if (doRecoJet)       ftreeRecoJet       -> Add(listNtuples[i].c_str());
    if (doRecoMet)       ftreeRecoMet       -> Add(listNtuples[i].c_str());
    if (doRecoEle)       ftreeRecoEle       -> Add(listNtuples[i].c_str());
    if (doRecoMuon)      ftreeRecoMuon      -> Add(listNtuples[i].c_str());
    if (doRecoTau)       ftreeRecoTau       -> Add(listNtuples[i].c_str());
    if (doRecoFilter)    ftreeRecoFilter    -> Add(listNtuples[i].c_str());
    if (doRecoVtx)       ftreeRecoVtx       -> Add(listNtuples[i].c_str());
    if (doBitWiseLayer1) ftreeUpgradeLayer1 -> Add(listNtuples[i].c_str());
    if (dol1CaloTower)   ftreeCaloTower     -> Add(listNtuples[i].c_str());
    if (dol1uGT)         fl1uGT             -> Add(listNtuples[i].c_str());
    if (dol1unpackuGT)   fl1unpackuGT       -> Add(listNtuples[i].c_str());
  }

  if (doEvent)         fChain->AddFriend(ftreeEvent);
  if (domuonreco)      fChain->AddFriend(ftreemuon);
  if (dol1extra)       fChain->AddFriend(ftreeExtra);
  if (dol1emuextra)    fChain->AddFriend(ftreeEmuExtra);
  if (dol1menu)        fChain->AddFriend(ftreeMenu);
  if (doRecoJet)       fChain->AddFriend(ftreeRecoJet);
  if (doRecoMet)       fChain->AddFriend(ftreeRecoMet);
  if (doRecoEle)       fChain->AddFriend(ftreeRecoEle);
  if (doRecoMuon)      fChain->AddFriend(ftreeRecoMuon);
  if (doRecoTau)       fChain->AddFriend(ftreeRecoTau);
  if (doRecoFilter)    fChain->AddFriend(ftreeRecoFilter);
  if (doRecoVtx)       fChain->AddFriend(ftreeRecoVtx);
  if (doBitWiseLayer1) fChain->AddFriend(ftreeUpgradeLayer1);
  if (dol1CaloTower)   fChain->AddFriend(ftreeCaloTower);
  if (dol1uGT)         fChain->AddFriend(fl1uGT);
  if (dol1unpackuGT)   fChain->AddFriend(fl1unpackuGT, "UnpackuGT_ZB");
  return true;
}

L1Ntuple::~L1Ntuple()
{
  if (rf)                 delete rf;
  if (fChain)             delete fChain;
  if (fEvent)             delete fEvent;
  if (ftreeUpgradeLayer1) delete ftreeUpgradeLayer1;
  if (ftreeEvent)         delete ftreeEvent;
  if (ftreemuon)          delete ftreemuon;
  if (ftreeExtra)         delete ftreeExtra;
  if (ftreeMenu)          delete ftreeMenu;
  if (ftreeEmuExtra)      delete ftreeEmuExtra;
  if (ftreeRecoJet)       delete ftreeRecoJet;
  if (ftreeRecoMet)       delete ftreeRecoMet;
  if (ftreeRecoEle)       delete ftreeRecoEle;
  if (ftreeRecoMuon)      delete ftreeRecoMuon;
  if (ftreeRecoTau)       delete ftreeRecoTau;
  if (ftreeRecoFilter)    delete ftreeRecoFilter;
  if (ftreeRecoVtx)       delete ftreeRecoVtx;
  if (ftreeCaloTower)     delete ftreeCaloTower;
  if (fl1uGT)             delete fl1uGT;
  if (fl1unpackuGT)       delete fl1unpackuGT;
}



Int_t L1Ntuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t L1Ntuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);

   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
   }
   return centry;
}

void L1Ntuple::Init()
{
   if (!fChain) return;
   fCurrent = -1;
   /*
   fChain->SetMakeClass(1);
   ftreemuon->SetMakeClass(1);
   ftreereco->SetMakeClass(1);
   ftreeExtra->SetMakeClass(1); */

   std::cout << "Estimate the number of entries ... " <<std::endl;

   upgrade_      = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
   std::cout<<"Setting branch addresses for L1Upgrade tree...  "<<std::endl;
   if(doNano && !UseuGTDecision){ // since it's a lot of i/o, only use if necessary
     printf("Using nano config for L1Upgrade tree\n");
     SetNanoBranchAddresses();
   }
   else{
     fChain->SetBranchAddress("L1Upgrade", &upgrade_ );
   }

   if (doBitWiseLayer1){
     std::cout<<"Setting branch addresses for L1 Upgrade tree w/ Bitwise Emul...  "<<std::endl;
     upgrade_lyr1_ = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
     ftreeUpgradeLayer1->SetBranchAddress("L1Upgrade", &upgrade_lyr1_ );
     fChain-> AddFriend(ftreeUpgradeLayer1);     
   }
   
   if (doEvent){
     std::cout<<"Setting branch addresses for Event tree..."<<std::endl;
     event_        = new L1Analysis::L1AnalysisEventDataFormat();
     
     if(doNano){
       printf("Using nano config for Event tree\n");
       ftreeEvent->SetBranchAddress("event", &event_->event );
       ftreeEvent->SetBranchAddress("run", &event_->run );
       ftreeEvent->SetBranchAddress("luminosityBlock", &event_->lumi, &b_eventLumi);
       ftreeEvent->SetBranchAddress("bunchCrossing", &event_->bx );
     }
     else{
       ftreeEvent->SetBranchAddress("Event", &event_, &b_eventLumi);
     }
     fChain-> AddFriend(ftreeEvent);     
   }

   if (dol1uGT){
     std::cout<<"Setting branch addresses for L1uGT tree..."<<std::endl;
     l1uGT_        = new GlobalAlgBlk();
     
     fl1uGT->SetBranchAddress("L1uGT", &l1uGT_ );
     fChain-> AddFriend(fl1uGT);     
   }

   if (dol1unpackuGT){
     l1unpackuGT_        = new GlobalAlgBlk();
     
     fl1unpackuGT->SetBranchAddress("L1uGT", &l1unpackuGT_ );
     fChain-> AddFriend(fl1unpackuGT, "UnpackuGT_ZB");     
   }

   if (doRecoJet)
   {
     std::cout<<"Setting branch addresses for reco Jets...   "<<std::endl;
     recoJet_ = new L1Analysis::L1AnalysisRecoJetDataFormat();
     recoSum_ = new L1Analysis::L1AnalysisRecoMetDataFormat();
     ftreeRecoJet->SetBranchAddress("Jet",&recoJet_);
     ftreeRecoJet->SetBranchAddress("Sums",&recoSum_);
   }



   if (doRecoMet)
   {
     //std::cout<<"Setting branch addresses for reco Jets...   "<<std::endl;
     //recoJet_ = new L1Analysis::L1AnalysisRecoJetDataFormat();
     //recoSum_ = new L1Analysis::L1AnalysisRecoMetDataFormat();
     //ftreeRecoJet->SetBranchAddress("Jet",&recoJet_);
     //ftreeRecoJet->SetBranchAddress("Sums",&recoSum_);
   }



   if (doRecoEle)
   {
     std::cout<<"Setting branch addresses for reco Eles...   "<<std::endl;
     recoEle_ = new L1Analysis::L1AnalysisRecoElectronDataFormat();
     ftreeRecoEle->SetBranchAddress("Electron",&recoEle_);
   }

   if (doRecoMuon)
   {
     std::cout<<"Setting branch addresses for reco Muons...   "<<std::endl;
     recoMuon_ = new L1Analysis::L1AnalysisRecoMuon2DataFormat();
     ftreeRecoMuon->SetBranchAddress("Muon",&recoMuon_);
   }

   if (doRecoTau)
   {
     std::cout<<"Setting branch addresses for reco Taus...   "<<std::endl;
     recoTau_ = new L1Analysis::L1AnalysisRecoTauDataFormat();
     ftreeRecoTau->SetBranchAddress("Tau",&recoTau_);
   }

   if (doRecoFilter)
   {
     std::cout<<"Setting branch addresses for reco Filters...   "<<std::endl;
     recoFilter_ = new L1Analysis::L1AnalysisRecoMetFilterDataFormat();
     ftreeRecoFilter->SetBranchAddress("MetFilters",&recoFilter_);
   }

   if (doRecoVtx)
   {
     std::cout<<"Setting branch addresses for reco Vertexes...   "<<std::endl;
     recoVtx_ = new L1Analysis::L1AnalysisRecoVertexDataFormat();
     ftreeRecoVtx->SetBranchAddress("Vertex",&recoVtx_);
   }

   if (dol1CaloTower)
   {
     std::cout<<"Setting branch addresses for L1CaloTower...   "<<std::endl;
     l1CaloTower_ = new L1Analysis::L1AnalysisL1CaloTowerDataFormat();
     ftreeCaloTower->SetBranchAddress("L1CaloTower",&l1CaloTower_);
   }

   // if (fChain->GetBranch("Simulation"))
   //   fChain->SetBranchAddress("Simulation", &simulation_ );
   // else
   //   std::cout<<"Simulation Branch not added..."<<std::endl;
   // 
   // fChain->SetBranchAddress("GCT",   &gct_   );
   // fChain->SetBranchAddress("GMT",   &gmt_   );
   // fChain->SetBranchAddress("GT",    &gt_    );
   // fChain->SetBranchAddress("RCT",   &rct_   );
   // fChain->SetBranchAddress("CSCTF", &csctf_ );
   // fChain->SetBranchAddress("DTTF",  &dttf_  );



     //if (doreco)
     //  {
     // std::cout<<"Setting branch addresses for reco..."<<std::endl;
     //
     // recoMet_          = new L1Analysis::L1AnalysisRecoMetDataFormat();
     // recoJet_          = new L1Analysis::L1AnalysisRecoJetDataFormat();
     // recoBasicCluster_ = new L1Analysis::L1AnalysisRecoClusterDataFormat();
     // recoSuperCluster_ = new L1Analysis::L1AnalysisRecoClusterDataFormat();
     // recoVertex_       = new L1Analysis::L1AnalysisRecoVertexDataFormat();
     // recoTrack_        = new L1Analysis::L1AnalysisRecoTrackDataFormat();
     //
     // ftreereco->SetBranchAddress("Jet", &recoJet_);
     // ftreereco->SetBranchAddress("BasicClusters", &recoBasicCluster_);
     // ftreereco->SetBranchAddress("SuperClusters", &recoSuperCluster_);
     // ftreereco->SetBranchAddress("Met", &recoMet_);
     // ftreereco->SetBranchAddress("Tracks", &recoTrack_);
     // ftreereco->SetBranchAddress("Vertices", &recoVertex_);
     // fChain->AddFriend(ftreereco);
     // std::cout << "RecoTree "  << ftreereco->GetEntries() << endl;
     //  }
     //
     //if (domuonreco)
     //  {
     //  std::cout<<"Setting branch addresses for muons...   "<<std::endl;
     //  recoMuon_     = new L1Analysis::L1AnalysisRecoMuonDataFormat() ;
     //  recoRpcHit_   = new L1Analysis::L1AnalysisRecoRpcHitDataFormat();
     //  ftreemuon->SetBranchAddress("Muon",&recoMuon_);
     //  ftreemuon->SetBranchAddress("RpcHit",&recoRpcHit_);
     //  }
     //
     //if (dol1extra)
     //  {
     //  std::cout<<"Setting branch addresses for L1Extra... "<<std::endl;
     //  l1extra_ = new L1Analysis::L1AnalysisL1ExtraDataFormat();
     //  ftreeExtra->SetBranchAddress("L1Extra",&l1extra_);
     //  fChain->AddFriend(ftreeExtra);
     //  std::cout  << "L1ExtraTree: "<< ftreeExtra->GetEntries() << std::endl;
     //  }
     //if ( dol1emuextra){
     //      std::cout<<"Setting branch addresses for L1EmuExtra... "<<std::endl;
     //  l1emuextra_ = new L1Analysis::L1AnalysisL1ExtraDataFormat();
     //  ftreeEmuExtra->SetBranchAddress("L1Extra",&l1emuextra_);
     //  fChain->AddFriend(ftreeEmuExtra);
     //  std::cout  << "L1EmuExtraTree: "<< ftreeEmuExtra->GetEntries() << std::endl;
     //}
     //
     //if (dol1menu)
     //  {
     //  std::cout<<"Setting branch addresses for L1Menu... "<<std::endl;
     //  l1menu_ = new L1Analysis::L1AnalysisL1MenuDataFormat();
     //  ftreeMenu->SetBranchAddress("L1Menu",&l1menu_);
     //  }

}

void L1Ntuple::Test()
{ 

  if (fChain == 0)  return;
 
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  unsigned int nevents =0;

  std::cout << nentries << " events to process"<<std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    //fChain->GetEvent(jentry);
  
    nevents++;
    if (nevents<9)  //eight first events
      { 
    std::cout << "--------------------- Event "<<jentry<<" ---------------------"<<std::endl;

    //event_
    std::cout << "L1Tree         : event_->run = "<<event_->run<<std::endl;

    }
  }
   
}

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::PrintRecoTree
//  Description:  
// ===========================================================================
bool L1Ntuple::PrintRecoTree() const
{
  if (recoJet_ != NULL)
  {
    for (int i = 0; i < recoJet_->nJets; ++i)
    {
      std::cout << recoJet_->eta.at(i) << " ";
    }
    std::cout << "." << std::endl;
  }
  return true;
}       // -----  end of function L1Ntuple::PrintRecoTree  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::GetuGTAlias
//  Description:  /* cursor */
// ===========================================================================
std::map<std::string, std::string> L1Ntuple::GetuGTAlias(TChain* fl1uGT)
{
  std::map<std::string, std::string> SeedAlias;
  std::vector<std::string> names;
  if (fl1uGT == NULL)  return SeedAlias;

  fl1uGT->GetEntry(1);
  TList * aliases = fl1uGT->GetTree()->GetListOfAliases();
  TIter iter(aliases);
  std::for_each(iter.Begin(), TIter::End(), [&](TObject* alias){ names.push_back(alias->GetName()); } );
  for (auto const & name: names) {
    SeedAlias[name] = fl1uGT->GetAlias(name.c_str());
  }
  return SeedAlias;
}       // -----  end of function L1Ntuple::GetuGTAlias  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::SelectTree
//  Description:  
// ===========================================================================
bool L1Ntuple::SelectTree(bool UseUnpack)
{
  if(!doNano){
    if (!UseUnpack) return false;
    MainTreePath = "l1UpgradeTree/L1UpgradeTree";
    CaloTreePath = "l1CaloTowerTree/L1CaloTowerTree";
    uGTTreePath  = "l1uGTTree/L1uGTTree";
    EventTreePath = "l1EventTree/L1EventTree";
    return true;
  }
  else {
    MainTreePath = "Events";
    EventTreePath = "Events";
    CaloTreePath = "Events";
    uGTTreePath  = "Events";
    return true;
  }
}       // -----  end of function L1Ntuple::SelectTree  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::SetNanoBranchAddresses
//  Description:  
// ===========================================================================
bool L1Ntuple::SetNanoBranchAddresses()
{
  // Set maximum buffer size to avoid memory errors with nano branch loading
  int bufferEG = 64;
  int bufferTau = 64;
  int bufferJet = 40;
  int bufferMu = 12;
  int bufferSums = 85; // Always 85 for nanoAOD: 17 sum types across 5 bx [-2, -1, 0, 1, 2]
                       // NOTE: for L1Ntuple this is cut off at 60 - bx = 2 sums are missing and only 9 sums are done for bx = 1
     
  // EG objects
  upgrade_->egEt.resize(bufferEG);
  upgrade_->egEta.resize(bufferEG);
  upgrade_->egPhi.resize(bufferEG);
  upgrade_->egIEt.resize(bufferEG);
  upgrade_->egIEta.resize(bufferEG);
  upgrade_->egIPhi.resize(bufferEG);
  upgrade_->egIso.resize(bufferEG);
  upgrade_->egBx.resize(bufferEG);
  upgrade_->egTowerIPhi.resize(bufferEG);
  upgrade_->egTowerIEta.resize(bufferEG);
  upgrade_->egRawEt.resize(bufferEG);
  upgrade_->egIsoEt.resize(bufferEG);
  upgrade_->egFootprintEt.resize(bufferEG);
  upgrade_->egNTT.resize(bufferEG);
  upgrade_->egShape.resize(bufferEG);
  upgrade_->egTowerHoE.resize(bufferEG);
  upgrade_->egHwQual.resize(bufferEG);

  fChain->SetBranchAddress("nL1EG", &upgrade_->nEGs, &b_nEGs);
  fChain->SetBranchAddress("L1EG_pt", upgrade_->egEt.data());// pt == et
  fChain->SetBranchAddress("L1EG_eta", upgrade_->egEta.data());
  fChain->SetBranchAddress("L1EG_phi", upgrade_->egPhi.data());
  fChain->SetBranchAddress("L1EG_hwPt", upgrade_->egIEt.data());// IEt = hwPt (etc)
  fChain->SetBranchAddress("L1EG_hwEta", upgrade_->egIEta.data());
  fChain->SetBranchAddress("L1EG_hwPhi", upgrade_->egIPhi.data());
  fChain->SetBranchAddress("L1EG_hwIso", upgrade_->egIso.data());// Iso = hwIso
  fChain->SetBranchAddress("L1EG_bx", upgrade_->egBx.data());
  fChain->SetBranchAddress("L1EG_towerIPhi", upgrade_->egTowerIPhi.data());
  fChain->SetBranchAddress("L1EG_towerIEta", upgrade_->egTowerIEta.data());
  fChain->SetBranchAddress("L1EG_rawEt", upgrade_->egRawEt.data());
  fChain->SetBranchAddress("L1EG_isoEt", upgrade_->egIsoEt.data());
  fChain->SetBranchAddress("L1EG_footprintEt", upgrade_->egFootprintEt.data());
  fChain->SetBranchAddress("L1EG_nTT", upgrade_->egNTT.data());
  fChain->SetBranchAddress("L1EG_shape", upgrade_->egShape.data());
  fChain->SetBranchAddress("L1EG_towerHoE", upgrade_->egTowerHoE.data());
  fChain->SetBranchAddress("L1EG_hwQual", upgrade_->egHwQual.data());

  // Tau objects
  upgrade_->tauEt.resize(bufferTau);
  upgrade_->tauEta.resize(bufferTau);
  upgrade_->tauPhi.resize(bufferTau);
  upgrade_->tauIEt.resize(bufferTau);
  upgrade_->tauIEta.resize(bufferTau);
  upgrade_->tauIPhi.resize(bufferTau);
  upgrade_->tauIso.resize(bufferTau);
  upgrade_->tauBx.resize(bufferTau);
  upgrade_->tauTowerIPhi.resize(bufferTau);
  upgrade_->tauTowerIEta.resize(bufferTau);
  upgrade_->tauRawEt.resize(bufferTau);
  upgrade_->tauIsoEt.resize(bufferTau);
  upgrade_->tauNTT.resize(bufferTau);
  upgrade_->tauHasEM.resize(bufferTau);
  upgrade_->tauIsMerged.resize(bufferTau);
  upgrade_->tauHwQual.resize(bufferTau);

  fChain->SetBranchAddress("nL1Tau", &upgrade_->nTaus, &b_nTaus);
  fChain->SetBranchAddress("L1Tau_pt", upgrade_->tauEt.data());
  fChain->SetBranchAddress("L1Tau_eta", upgrade_->tauEta.data());
  fChain->SetBranchAddress("L1Tau_phi", upgrade_->tauPhi.data());
  fChain->SetBranchAddress("L1Tau_hwPt", upgrade_->tauIEt.data());
  fChain->SetBranchAddress("L1Tau_hwEta", upgrade_->tauIEta.data());
  fChain->SetBranchAddress("L1Tau_hwPhi", upgrade_->tauIPhi.data());
  fChain->SetBranchAddress("L1Tau_hwIso", upgrade_->tauIso.data());
  fChain->SetBranchAddress("L1Tau_bx", upgrade_->tauBx.data());
  fChain->SetBranchAddress("L1Tau_towerIPhi", upgrade_->tauTowerIPhi.data());
  fChain->SetBranchAddress("L1Tau_towerIEta", upgrade_->tauTowerIEta.data());
  fChain->SetBranchAddress("L1Tau_rawEt", upgrade_->tauRawEt.data());
  fChain->SetBranchAddress("L1Tau_isoEt", upgrade_->tauIsoEt.data());
  fChain->SetBranchAddress("L1Tau_nTT", upgrade_->tauNTT.data());
  fChain->SetBranchAddress("L1Tau_hasEM", upgrade_->tauHasEM.data());
  fChain->SetBranchAddress("L1Tau_isMerged", upgrade_->tauIsMerged.data());
  fChain->SetBranchAddress("L1Tau_hwQual", upgrade_->tauHwQual.data());

  // Jet objects
  upgrade_->jetEt.resize(bufferJet);
  upgrade_->jetEta.resize(bufferJet);
  upgrade_->jetPhi.resize(bufferJet);
  upgrade_->jetIEt.resize(bufferJet);
  upgrade_->jetIEta.resize(bufferJet);
  upgrade_->jetIPhi.resize(bufferJet);
  upgrade_->jetHwQual.resize(bufferJet);
  upgrade_->jetBx.resize(bufferJet);
  upgrade_->jetTowerIPhi.resize(bufferJet);
  upgrade_->jetTowerIEta.resize(bufferJet);
  upgrade_->jetRawEt.resize(bufferJet);
  upgrade_->jetSeedEt.resize(bufferJet);
  upgrade_->jetPUEt.resize(bufferJet);
  upgrade_->jetPUDonutEt0.resize(bufferJet);
  upgrade_->jetPUDonutEt1.resize(bufferJet);
  upgrade_->jetPUDonutEt2.resize(bufferJet);
  upgrade_->jetPUDonutEt3.resize(bufferJet);

  fChain->SetBranchAddress("nL1Jet", &upgrade_->nJets, &b_nJets);
  fChain->SetBranchAddress("L1Jet_pt", upgrade_->jetEt.data());
  fChain->SetBranchAddress("L1Jet_eta", upgrade_->jetEta.data());
  fChain->SetBranchAddress("L1Jet_phi", upgrade_->jetPhi.data());
  fChain->SetBranchAddress("L1Jet_hwPt", upgrade_->jetIEt.data());
  fChain->SetBranchAddress("L1Jet_hwEta", upgrade_->jetIEta.data());
  fChain->SetBranchAddress("L1Jet_hwPhi", upgrade_->jetIPhi.data());
  fChain->SetBranchAddress("L1Jet_hwQual", upgrade_->jetHwQual.data());
  fChain->SetBranchAddress("L1Jet_bx", upgrade_->jetBx.data());
  fChain->SetBranchAddress("L1Jet_towerIPhi", upgrade_->jetTowerIPhi.data());
  fChain->SetBranchAddress("L1Jet_towerIEta", upgrade_->jetTowerIEta.data());
  fChain->SetBranchAddress("L1Jet_rawEt", upgrade_->jetRawEt.data());
  fChain->SetBranchAddress("L1Jet_seedEt", upgrade_->jetSeedEt.data());
  fChain->SetBranchAddress("L1Jet_puEt", upgrade_->jetPUEt.data());
  fChain->SetBranchAddress("L1Jet_puDonutEt0", upgrade_->jetPUDonutEt0.data());
  fChain->SetBranchAddress("L1Jet_puDonutEt1", upgrade_->jetPUDonutEt1.data());
  fChain->SetBranchAddress("L1Jet_puDonutEt2", upgrade_->jetPUDonutEt2.data());
  fChain->SetBranchAddress("L1Jet_puDonutEt3", upgrade_->jetPUDonutEt3.data());

  // Muon objects
  upgrade_->muonEt.resize(bufferMu);
  upgrade_->muonEtUnconstrained.resize(bufferMu);
  upgrade_->muonEta.resize(bufferMu);
  upgrade_->muonPhi.resize(bufferMu);
  upgrade_->muonEtaAtVtx.resize(bufferMu);
  upgrade_->muonPhiAtVtx.resize(bufferMu);
  upgrade_->muonIEt.resize(bufferMu);
  upgrade_->muonIEtUnconstrained.resize(bufferMu);
  upgrade_->muonIEta.resize(bufferMu);
  upgrade_->muonIPhi.resize(bufferMu);
  upgrade_->muonIEtaAtVtx.resize(bufferMu);
  upgrade_->muonIPhiAtVtx.resize(bufferMu);
  upgrade_->muonIDEta.resize(bufferMu);
  upgrade_->muonIDPhi.resize(bufferMu);
  upgrade_->muonChg.resize(bufferMu);
  upgrade_->muonIso.resize(bufferMu);
  upgrade_->muonQual.resize(bufferMu);
  upgrade_->muonDxy.resize(bufferMu);
  upgrade_->muonTfMuonIdx.resize(bufferMu);
  upgrade_->muonBx.resize(bufferMu);
  muonChgValid.resize(bufferMu);

  fChain->SetBranchAddress("nL1Mu", &upgrade_->nMuons, &b_nMuons);
  fChain->SetBranchAddress("L1Mu_pt", upgrade_->muonEt.data());
  fChain->SetBranchAddress("L1Mu_ptUnconstrained", upgrade_->muonEtUnconstrained.data());
  fChain->SetBranchAddress("L1Mu_eta", upgrade_->muonEta.data());
  fChain->SetBranchAddress("L1Mu_phi", upgrade_->muonPhi.data());
  fChain->SetBranchAddress("L1Mu_etaAtVtx", upgrade_->muonEtaAtVtx.data());
  fChain->SetBranchAddress("L1Mu_phiAtVtx", upgrade_->muonPhiAtVtx.data());
  fChain->SetBranchAddress("L1Mu_hwPt", upgrade_->muonIEt.data());
  fChain->SetBranchAddress("L1Mu_hwPtUnconstrained", upgrade_->muonIEtUnconstrained.data());
  fChain->SetBranchAddress("L1Mu_hwEta", upgrade_->muonIEta.data());
  fChain->SetBranchAddress("L1Mu_hwPhi", upgrade_->muonIPhi.data());
  fChain->SetBranchAddress("L1Mu_hwEtaAtVtx", upgrade_->muonIEtaAtVtx.data());
  fChain->SetBranchAddress("L1Mu_hwPhiAtVtx", upgrade_->muonIPhiAtVtx.data());
  fChain->SetBranchAddress("L1Mu_hwDEtaExtra", upgrade_->muonIDEta.data());
  fChain->SetBranchAddress("L1Mu_hwDPhiExtra", upgrade_->muonIDPhi.data());
  fChain->SetBranchAddress("L1Mu_hwCharge", upgrade_->muonChg.data());
  fChain->SetBranchAddress("L1Mu_hwChargeValid", muonChgValid.data());
  fChain->SetBranchAddress("L1Mu_hwIso", upgrade_->muonIso.data());
  fChain->SetBranchAddress("L1Mu_hwQual", upgrade_->muonQual.data());
  fChain->SetBranchAddress("L1Mu_hwDXY", upgrade_->muonDxy.data());
  fChain->SetBranchAddress("L1Mu_tfMuonIndex", upgrade_->muonTfMuonIdx.data());
  fChain->SetBranchAddress("L1Mu_bx", upgrade_->muonBx.data());

  // Sums
  upgrade_->sumType.resize(bufferSums);
  upgrade_->sumEt.resize(bufferSums);
  upgrade_->sumPhi.resize(bufferSums);
  upgrade_->sumIEt.resize(bufferSums);
  upgrade_->sumIPhi.resize(bufferSums);
  upgrade_->sumBx.resize(bufferSums);
  sumBxTmp.resize(bufferSums);
  sumTypeTmp.resize(bufferSums);
  sumIEtTmp.resize(bufferSums);
  sumIPhiTmp.resize(bufferSums);

  fChain->SetBranchAddress("nL1EtSum", &upgrade_->nSums, &b_nSums); 
  fChain->SetBranchAddress("L1EtSum_etSumType", sumTypeTmp.data()); // use temp int vector for initial loading
  fChain->SetBranchAddress("L1EtSum_pt", upgrade_->sumEt.data());
  fChain->SetBranchAddress("L1EtSum_phi", upgrade_->sumPhi.data());
  fChain->SetBranchAddress("L1EtSum_hwPt", sumIEtTmp.data()); // use temp int vector for initial loading
  fChain->SetBranchAddress("L1EtSum_hwPhi", sumIPhiTmp.data()); // use temp int vector for initial loading
  fChain->SetBranchAddress("L1EtSum_bx", sumBxTmp.data()); // use temp short int vector for initial loading

  // Muon Shower placeholders - not yet in nano
  // fChain->SetBranchAddress("nL1MuShower", &upgrade_->nMuonShowers, &b_nMuonShowers); 
  // fChain->SetBranchAddress("L1MuShower_isOneNominal", upgrade_->muonShowerOneNominal.data()); 
  // fChain->SetBranchAddress("L1MuShower_isOneTight", upgrade_->muonShowerOneTight.data());
  // fChain->SetBranchAddress("L1MuShower_isTwoLoose", upgrade_->muonShowerTwoLoose.data());
  // fChain->SetBranchAddress("L1MuShower_isTwoLooseDiffSectors", upgrade_->muonShowerTwoLooseDiffSectors.data()); 
  // fChain->SetBranchAddress("L1MuShower_bx", sumBxTmp.data()); // use temp short int vector for initial loading

  // Sums ZDC placeholders - used for heavy ions; no plans yet to include in nano
  // unsigned short int nSumsZDC;
  // std::vector<short int> sumZDCType;
  // std::vector<float> sumZDCEt;
  // std::vector<float> sumZDCPhi;
  // std::vector<short int> sumZDCIEt;
  // std::vector<short int> sumZDCIPhi;
  // std::vector<float> sumZDCBx;

  return true;
} // -----  end of function L1Ntuple::SetNanoBranchAddresses  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::ResizeNanoVectors
//  Description:  
// ===========================================================================
bool L1Ntuple::ResizeNanoVectors(Long64_t entry)
{
  // Load only the numbers of each object type in the event
  b_nEGs->GetEntry(entry);
  b_nTaus->GetEntry(entry);
  b_nJets->GetEntry(entry);
  b_nMuons->GetEntry(entry);
  b_nSums->GetEntry(entry);

  // Resize vectors ready for full event loading
  // NOTE: These vector sizes are used directly in menulib
  upgrade_->egBx.resize(upgrade_->nEGs);
  upgrade_->tauBx.resize(upgrade_->nTaus);
  upgrade_->jetBx.resize(upgrade_->nJets);
  upgrade_->muonBx.resize(upgrade_->nMuons);
  upgrade_->sumBx.resize(upgrade_->nSums);
  upgrade_->sumType.resize(upgrade_->nSums);

  // Remainder of vectors
  upgrade_->egEt.resize(upgrade_->nEGs);
  upgrade_->egEta.resize(upgrade_->nEGs);
  upgrade_->egPhi.resize(upgrade_->nEGs);
  upgrade_->egIEt.resize(upgrade_->nEGs);
  upgrade_->egIEta.resize(upgrade_->nEGs);
  upgrade_->egIPhi.resize(upgrade_->nEGs);
  upgrade_->egIso.resize(upgrade_->nEGs);
  upgrade_->egTowerIPhi.resize(upgrade_->nEGs);
  upgrade_->egTowerIEta.resize(upgrade_->nEGs);
  upgrade_->egRawEt.resize(upgrade_->nEGs);
  upgrade_->egIsoEt.resize(upgrade_->nEGs);
  upgrade_->egFootprintEt.resize(upgrade_->nEGs);
  upgrade_->egNTT.resize(upgrade_->nEGs);
  upgrade_->egShape.resize(upgrade_->nEGs);
  upgrade_->egTowerHoE.resize(upgrade_->nEGs);
  upgrade_->egHwQual.resize(upgrade_->nEGs);

  upgrade_->tauEt.resize(upgrade_->nTaus);
  upgrade_->tauEta.resize(upgrade_->nTaus);
  upgrade_->tauPhi.resize(upgrade_->nTaus);
  upgrade_->tauIEt.resize(upgrade_->nTaus);
  upgrade_->tauIEta.resize(upgrade_->nTaus);
  upgrade_->tauIPhi.resize(upgrade_->nTaus);
  upgrade_->tauIso.resize(upgrade_->nTaus);
  upgrade_->tauTowerIPhi.resize(upgrade_->nTaus);
  upgrade_->tauTowerIEta.resize(upgrade_->nTaus);
  upgrade_->tauRawEt.resize(upgrade_->nTaus);
  upgrade_->tauIsoEt.resize(upgrade_->nTaus);
  upgrade_->tauNTT.resize(upgrade_->nTaus);
  upgrade_->tauHasEM.resize(upgrade_->nTaus);
  upgrade_->tauIsMerged.resize(upgrade_->nTaus);
  upgrade_->tauHwQual.resize(upgrade_->nTaus);

  upgrade_->jetEt.resize(upgrade_->nJets);
  upgrade_->jetEta.resize(upgrade_->nJets);
  upgrade_->jetPhi.resize(upgrade_->nJets);
  upgrade_->jetIEt.resize(upgrade_->nJets);
  upgrade_->jetIEta.resize(upgrade_->nJets);
  upgrade_->jetIPhi.resize(upgrade_->nJets);
  upgrade_->jetHwQual.resize(upgrade_->nJets);
  upgrade_->jetTowerIPhi.resize(upgrade_->nJets);
  upgrade_->jetTowerIEta.resize(upgrade_->nJets);
  upgrade_->jetRawEt.resize(upgrade_->nJets);
  upgrade_->jetSeedEt.resize(upgrade_->nJets);
  upgrade_->jetPUEt.resize(upgrade_->nJets);
  upgrade_->jetPUDonutEt0.resize(upgrade_->nJets);
  upgrade_->jetPUDonutEt1.resize(upgrade_->nJets);
  upgrade_->jetPUDonutEt2.resize(upgrade_->nJets);
  upgrade_->jetPUDonutEt3.resize(upgrade_->nJets);

  upgrade_->muonEt.resize(upgrade_->nMuons);
  upgrade_->muonEtUnconstrained.resize(upgrade_->nMuons);
  upgrade_->muonEta.resize(upgrade_->nMuons);
  upgrade_->muonPhi.resize(upgrade_->nMuons);
  upgrade_->muonEtaAtVtx.resize(upgrade_->nMuons);
  upgrade_->muonPhiAtVtx.resize(upgrade_->nMuons);
  upgrade_->muonIEt.resize(upgrade_->nMuons);
  upgrade_->muonIEtUnconstrained.resize(upgrade_->nMuons);
  upgrade_->muonIEta.resize(upgrade_->nMuons);
  upgrade_->muonIPhi.resize(upgrade_->nMuons);
  upgrade_->muonIEtaAtVtx.resize(upgrade_->nMuons);
  upgrade_->muonIPhiAtVtx.resize(upgrade_->nMuons);
  upgrade_->muonIDEta.resize(upgrade_->nMuons);
  upgrade_->muonIDPhi.resize(upgrade_->nMuons);
  upgrade_->muonChg.resize(upgrade_->nMuons);
  upgrade_->muonIso.resize(upgrade_->nMuons);
  upgrade_->muonQual.resize(upgrade_->nMuons);
  upgrade_->muonDxy.resize(upgrade_->nMuons);
  upgrade_->muonTfMuonIdx.resize(upgrade_->nMuons);

  upgrade_->sumEt.resize(upgrade_->nSums);
  upgrade_->sumPhi.resize(upgrade_->nSums);
  upgrade_->sumIEt.resize(upgrade_->nSums);
  upgrade_->sumIPhi.resize(upgrade_->nSums);

  return true;
} // -----  end of function L1Ntuple::ResizeNanoVectors  -----

// ===  FUNCTION  ============================================================
//         Name:  L1Ntuple::LoadNanoVariables
//  Description:  
// ===========================================================================
bool L1Ntuple::LoadNanoVariables()
{
  // Nano loads charges based on hardware boolean - replicate logic here
  for(int imu=0; imu < upgrade_->nMuons; imu++) {
    if(muonChgValid.at(imu) == 1){ // check charge measurement is valid
      if(upgrade_->muonChg.at(imu) == 1){upgrade_->muonChg.at(imu) = -1;}
      else if(upgrade_->muonChg.at(imu) == 0){upgrade_->muonChg.at(imu) = 1;}
    }
    else { // set charge to 0 if measurement is invalid
      upgrade_->muonChg.at(imu) = 0;
    }
  }
  
  // Cast values loaded into temporary vectors with nanoAOD branch types into types used by L1Ntuple
  for(int isum=0; isum < upgrade_->nSums; isum++) { 
    upgrade_->sumBx.at(isum) = static_cast<float>(sumBxTmp.at(isum));
    upgrade_->sumType.at(isum) = static_cast<short int>(sumTypeTmp.at(isum));
    upgrade_->sumIEt.at(isum) = static_cast<short int>(sumIEtTmp.at(isum));
    upgrade_->sumIPhi.at(isum) = static_cast<short int>(sumIPhiTmp.at(isum));
  }
  
  return true;
} // -----  end of function L1Ntuple::LoadNanoVariables  -----
