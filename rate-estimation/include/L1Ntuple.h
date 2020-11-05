//////////////////////////////////////
// Example root macro for l1 ntuples
//////////////////////////////////////

#ifndef L1Ntuple_h
#define L1Ntuple_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TFriendElement.h>
#include <TList.h>
#include <TMatrix.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TCanvas.h>

#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoJetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoElectronDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMuon2DataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetFilterDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoTauDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoVertexDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1CaloTowerDataFormat.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"

class L1Ntuple {
public:
  TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  TChain          *fEvent;
  TChain          *ftreeUpgradeLayer1;
  TChain          *ftreeEvent;
  TChain          *ftreemuon;
  TChain          *ftreeExtra;
  TChain          *ftreeMenu;
  TChain          *ftreeEmuExtra;
  TChain          *ftreeRecoJet;
  TChain          *ftreeRecoMet;
  TChain          *ftreeRecoEle;
  TChain          *ftreeRecoMuon;
  TChain          *ftreeRecoTau;
  TChain          *ftreeRecoFilter;
  TChain          *ftreeRecoVtx;
  TChain          *ftreeCaloTower;
  TChain          *fl1uGT;
  TChain          *fl1unpackuGT;
  Int_t            fCurrent; //!current Tree number in a TChain

  bool doEvent;
  bool domuonreco;
  bool dol1extra;
  bool dol1emuextra;
  bool dol1menu;  
  bool doRecoJet;
  bool doRecoMet;
  bool doRecoEle;
  bool doRecoMuon;
  bool doRecoTau;
  bool doRecoFilter;
  bool doRecoVtx;
  bool doBitWiseLayer1;
  bool dol1CaloTower;
  bool dol1uGT;
  bool dol1unpackuGT;

  L1Analysis::L1AnalysisEventDataFormat         *event_;
  L1Analysis::L1AnalysisL1UpgradeDataFormat     *upgrade_;
  L1Analysis::L1AnalysisL1UpgradeDataFormat     *upgrade_lyr1_;
  L1Analysis::L1AnalysisRecoVertexDataFormat    *recoVtx_;
  L1Analysis::L1AnalysisRecoJetDataFormat       *recoJet_;
  L1Analysis::L1AnalysisRecoMetDataFormat       *recoSum_;
  L1Analysis::L1AnalysisRecoElectronDataFormat  *recoEle_;
  L1Analysis::L1AnalysisRecoMuon2DataFormat     *recoMuon_;
  L1Analysis::L1AnalysisRecoTauDataFormat       *recoTau_;
  L1Analysis::L1AnalysisRecoMetFilterDataFormat *recoFilter_;
  L1Analysis::L1AnalysisL1CaloTowerDataFormat   *l1CaloTower_;
  GlobalAlgBlk                                  *l1uGT_;
  GlobalAlgBlk                                  *l1unpackuGT_;
  

  L1Ntuple();
  L1Ntuple(const std::string & fname);

  virtual ~L1Ntuple();

  bool Open(const std::string & fname);
  bool OpenWithList(const std::string & fname);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init();
  //virtual void     Loop();
  void     Test();
  void     Test2();
  bool PrintRecoTree() const;
  Long64_t GetEntries();
  std::map<std::string, std::string> GetuGTAlias(TChain* fl1uGT);

protected:
  bool CheckFirstFile();
  bool OpenWithoutInit();
  bool SelectTree(bool UseUnpack);
  bool OpenNtupleList(const std::string & fname);

  std::vector<std::string> listNtuples;
private :
  Long64_t nentries_;
  TFile* rf;
  std::string MainTreePath;
  std::string CaloTreePath;
  std::string uGTTreePath;
};

#endif
