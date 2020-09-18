#ifndef  __L1ALGOFACTORY_INC__
#define  __L1ALGOFACTORY_INC__

#include <cmath>
#include "L1Ntuple.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include <iostream>
#include <cassert>
#include "TLorentzVector.h"
#include "L1Struct.h"

// From  https://root.cern.ch/doc/master/TVector2_8cxx_source.html#l00102/
// Returns phi angle in the interval [-PI,PI)
inline float Phi_mpi_pi(double x)
{
  while(x >= 3.14) x -= 2 * 3.14;
  while(x < -3.14) x += 2 * 3.14;
  return fabs(x);
}

class L1AlgoFactory: public L1Ntuple{
 public:
  L1AlgoFactory():jetCentFwd(3.0015),muonER(2.1),eleER(2.1315),tauER(2.1315),
  MuJetCordPhi(0.4), MuJetCordEta(0.4), MuOpenJetCordPhi(0.4), MuMudPhi(1.0), Onia2015ER(1.6), SelBx(0), EGIsobit(2),
  UseUpgradeLyr1(false), UseL1CaloTower(false)
  {};
  //L1AlgoFactory(TTree *tree);

  void SingleMuPt(Float_t& ptcut, Bool_t isER, Int_t qualmin=2);
  void DoubleMuPt(Float_t& cut1, Float_t& cut2, int Qual = 1, Bool_t isER = false);
  void DoubleMu_ETMPt(Float_t &cut1, Float_t &cut2, Float_t& ETMcut, Bool_t isER=false);
  void TripleMuPt(Float_t& mu1pt, Float_t& mu2pt, Float_t& mu3pt, Int_t qualmin = 4);
  void QuadMuPt(Float_t& mu1pt, Float_t& mu2pt, Float_t& mu3pt, Float_t& mu4pt, Int_t qualmin = 4);
  void ComplexSingleMuPt(Float_t& ptcut, Bool_t isER, Int_t qualmin, int muonType=0, int muonBX=0);

  void SingleEGPt(Float_t& ptcut, Bool_t isIsolated, Bool_t isER);
  void DoubleEGPt(Float_t& ele1pt, Float_t& ele2pt, Bool_t isIsolated = false, Bool_t isER = false);
  void TripleEGPt(Float_t& ele1pt, Float_t& ele2pt, Float_t& ele3pt);

  void SingleJetPt(Float_t& ptcut, Bool_t isCentral = false);
  void DoubleJetPt(Float_t& cut1, Float_t& cut2, Bool_t isCentral = false);
  void DoubleJet_EtaRes_deltaEtaPt(Float_t& cut1, Float_t& cut2, Float_t &EtaRestrict, Float_t &dEtaMax );
  void DoubleTauJetEta2p17Pt(Float_t& cut1, Float_t& cut2, Bool_t isIsolated = false);
  void TripleJetPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Bool_t isCentral = false);
  Bool_t TripleJet_VBF(Float_t jet1, Float_t jet2, Float_t jet3, Int_t jetclass = 0);
  void QuadJetPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Float_t& cut4, Bool_t isCentral = false);
  void SingleTauPt(Float_t& cut, Bool_t isER, Bool_t isIsolated);
  bool DoubleJet_ForwardBackwardPt(Float_t& cut1, Float_t& cut2 );
  bool DoubleJet_ForwardBackward(Float_t ptcut1, Float_t ptcut2);

  void Mu_EGPt(Float_t& mucut, Float_t& EGcut, Bool_t isIsolated = false, Int_t qualmin=4);
  void DoubleMu_EGPt(Float_t& mucut, Float_t& EGcut, int muonQual=1, bool isOS=false);
  void Mu_DoubleEGPt(Float_t& mucut, Float_t& EGcut );

  void Muer_JetCentralPt(Float_t& mucut, Float_t& jetcut);
  void Mu_JetCentral_deltaPt(Float_t& mucut, Float_t& jetcut);
  void Mu_DoubleJetCentralPt(Float_t& mucut, Float_t& jetcut);

  void EG_JetPt(Float_t& EGcut, Float_t& Jetcut, Float_t dRMin, bool isEGER, bool isEGIso, bool isJetCen);
  void EG_DoubleJetCentralPt(Float_t& EGcut, Float_t& jetcut);
  void EGer_TripleJetCentralPt(Float_t& EGcut, Float_t& jetcut, bool &isEGER, bool &isEGIso);
  void IsoEGer_TauJetEta2p17Pt(Float_t& egcut, Float_t& taucut, bool& IsIsoTau);

  void QuadJetCentral_TauJetPt(Float_t& jetcut, Float_t& taucut, Bool_t isTauER, Bool_t isTauIso);

  int GetSumEtIdx(EtSumType type);
  void ETMVal(Float_t& ETMcut);
  void HTTVal(Float_t& HTTcut);
  void HTMVal(Float_t& HTMcut);
  void ETTVal(Float_t& ETTcut);
  void ETMHFVal(Float_t& ETMcut );
  void HTTHFVal(Float_t& HTTcut);


  Bool_t SingleMu(Float_t ptcut, Bool_t isER, Int_t qualmin=2);
  Bool_t ComplexSingleMu(Float_t& ptcut, Bool_t isER, Int_t qualmin=2, Int_t muonType=0, Int_t muonBX=0);
  Bool_t DoubleMu(Float_t mu1pt, Float_t mu2pt, int Qual=1, Bool_t isER=false);
  Bool_t DoubleMu_ETM(Float_t mu1pt, Float_t mu2pt, Float_t ETMcut, Bool_t isER=false);
  Bool_t TripleMu(Float_t mu1pt, Float_t mu2pt, Float_t mu3pt, Int_t qualmin);
  Bool_t QuadMu(Float_t mu1pt, Float_t mu2pt, Float_t mu3pt, Float_t mu4pt, Int_t qualmin);

  Bool_t SingleEG(Float_t ptcut, Bool_t isIsolated, Bool_t isER);
  Bool_t DoubleEG(Float_t ptcut1, Float_t ptcut2, Bool_t isIsolated = false);
  Bool_t TripleEG(Float_t ptcut1, Float_t ptcut2, Float_t ptcut3);

  Bool_t SingleJet(Float_t ptcut, Bool_t isCentral = false);
  Bool_t DoubleJet(Float_t cut1, Float_t cut2, Bool_t isCentral = false);
  Bool_t DoubleJet_EtaRes_deltaEta(Float_t ptcut1, Float_t ptcut2, Float_t EtaRestrict, Float_t dEtaMax);
  Bool_t DoubleTauJetEta2p17(Float_t cut1, Float_t cut2, Bool_t isIsolated = false);
  Bool_t TripleJet(Float_t cut1, Float_t cut2, Float_t cut3, Bool_t isCentral = false);
  Bool_t QuadJet(Float_t cut1, Float_t cut2, Float_t cut3, Float_t cut4, Bool_t isCentral);

  Bool_t Mu_EG(Float_t mucut, Float_t EGcut, Bool_t isIsolated = false, Int_t qualmin=4);
  Bool_t DoubleMu_EG(Float_t mucut, Float_t EGcut, int muonQual=1, bool isOS=false);
  Bool_t Mu_DoubleEG(Float_t mucut, Float_t EGcut);

  Bool_t Muer_JetCentral(Float_t mucut, Float_t jetcut);
  Bool_t Mu_JetCentral_delta(Float_t mucut, Float_t jetcut);
  Bool_t Mu_DoubleJetCentral(Float_t mucut, Float_t jetcut);

  Bool_t EG_Jet(Float_t egcut, Float_t jetcut, Float_t dRMin, bool isEGER, bool isEGIso, bool isJetCen);
  Bool_t EG_DoubleJetCentral(Float_t EGcut, Float_t jetcut);
  Bool_t EGer_TripleJetCentral(Float_t egcut, Float_t jetcut, bool isEGER, bool isEGIso);
  Bool_t IsoEGer_TauJetEta2p17(Float_t egcut, Float_t taucut, bool IsIsoTau=false);

  Bool_t QuadJetCentral_TauJet(Float_t jetcut, Float_t taucut, Bool_t isTauER, Bool_t isTauIso);


  Bool_t ETM(Float_t ETMcut);
  Bool_t HTT(Float_t HTTcut);
  Bool_t HTM(Float_t HTMcut);
  Bool_t ETT(Float_t ETTcut);

  inline Bool_t correlateInPhi(Int_t jetphi, Int_t muphi, Int_t delta=1);
  inline Bool_t correlateInEta(Float_t mueta, Float_t jeteta, Float_t delta);
  inline Bool_t correlatedRMax(Float_t eta1, Float_t eta2, Float_t phi1, Float_t phi2, Float_t dRbound);
  inline Bool_t correlatedRMin(Float_t eta1, Float_t eta2, Float_t phi1, Float_t phi2, Float_t dRbound);
  Int_t etaMuIdx(Double_t eta);
  Int_t phiINjetCoord(Double_t phi);
  Int_t etaINjetCoord(Double_t eta);
  inline Double_t degree(Double_t radian);
  inline Bool_t correlateMT(Float_t pt1, Float_t pt2, Float_t phi1, Float_t phi2, Float_t MTcut);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MultiMuon ~~~~~
  Bool_t DoubleMuXOpen(Float_t mu1pt);
  void DoubleMuXOpenPt(Float_t& cut);
  Bool_t DoubleMuOpen(Float_t mu1pt);
  void DoubleMuOpenPt(Float_t& cut);
  Bool_t Onia2015(Float_t mu1pt, Float_t mu2pt, Bool_t isER, Bool_t isOS, Int_t delta, Float_t MuEta=1.6);
  void Onia2015Pt(Float_t& ptcut1, Float_t& ptcut2, Bool_t isER, Bool_t isOS, Int_t delta, float Onia2015Pt_= -1);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Cross ~~~~~
  Bool_t Mu_HTT(Float_t mucut, Float_t HTcut);
  void Mu_HTTPt(Float_t& mucut, Float_t& HTcut );
  Bool_t Muer_ETM(Float_t mucut, Float_t ETMcut);
  void Muer_ETMPt(Float_t& mucut, Float_t& ETMcut );
  Bool_t SingleEG_Eta2p1_HTT(Float_t egcut, Float_t HTTcut, Bool_t isIsolated = false);
  void SingleEG_Eta2p1_HTTPt(Float_t& egcut, Float_t& HTTcut, Bool_t isIsolated = false);
  Bool_t Muer_TauJetEta2p17(Float_t mucut, Float_t taucut, Bool_t isIsolated = false);
  void Muer_TauJetEta2p17Pt(Float_t& mucut, Float_t& taucut, Bool_t isIsolated = false);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MultiCross ~~~~~
  Bool_t DoubleJetCentral_ETM(Float_t jetcut1, Float_t jetcut2, Float_t ETMcut);
  void DoubleJetCentral_ETMPt(Float_t& jetcut1, Float_t& jetcut2, Float_t& ETMcut);
  Bool_t DoubleEG_HT(Float_t EGcut, Float_t HTcut);
  void DoubleEG_HTPt(Float_t& EGcut, Float_t& HTcut);
  Bool_t Jet_MuOpen_EG_dPhiMuEG1(Float_t jetcut, Float_t egcut, int MuQual=0);
  Bool_t Jet_MuOpen_Mu_dPhiMuMu1(Float_t jetcut, Float_t mucut, int MuQual=0);
  void Jet_MuOpen_Mu_dPhiMuMu1Pt(Float_t& jetcut, Float_t& mucut, int MuQual=0);
  void Jet_MuOpen_EG_dPhiMuEG1Pt(Float_t& jetcut, Float_t& egcut, int MuQual=0);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Mass ~~~~~
  bool MultiEGMass(int pt1, int pt2, int pt3, int pt4, int Mcut, bool isIsolated, bool isER) const;
  Float_t DiJetMass(Float_t Mj1, Float_t Mj2, Bool_t isCentral) const;
  bool DoubleJetMass(Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
      Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut);
  bool DoubleJetMass_Mu( Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
      Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut, 
      Float_t Muptcut, Bool_t isMuER, Int_t Muqualmin);
  bool DoubleJetMass_EG( Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
      Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut, 
      Float_t EGptcut, Bool_t isEGIsolated, Bool_t isEGER);

  bool ETM_Jet(float ETMcut, float jetcut, bool isCent);
  void ETM_JetPt(float& ETMcut, float& jetcut, const bool& isCent);
  void HTM_HTTPt(float &HTMcut, float &HTTcut);
  bool HTM_HTT(float HTMcut, float HTTcut);
  bool Mu_Jet(float mucut, float jetcut, bool isMuER, float JetCent);
  void Mu_JetPt(float &mucut, float &jetcut, const bool isMuER, const float JetCent);
  bool ETM_HTT(float ETMcut, float HTTcut, bool isETMHF);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2017 Proposals ~~~~~
  Bool_t EG_HTT(Float_t egcut, Float_t HTcut, bool isEGer, bool isEGIso);
  void EG_HTTPt(Float_t& egcut, Float_t& HTcut, bool &isEGER, bool &isEGIso);
  bool HTT_QuadJet(Float_t HTTcut, Float_t Jet1cut, Float_t Jet2cut, Float_t Jet3cut, Float_t Jet4cut, Float_t JetEtaRestr);
  void HTT_QuadJetPt(Float_t &HTTcut, Float_t &Jet1cut, Float_t &Jet2cut, Float_t &Jet3cut, Float_t &Jet4cut, Float_t &JetEtaRestr);
  bool Mu_DoubleJet_Cor(Float_t mupt, Float_t diJetpt, Float_t EtaRestrict, Float_t MuJetdRMax, Float_t diJetdEtaMax);
  Bool_t Tau_ETM(Float_t taucut, Float_t ETMcut, Bool_t isTauER, Bool_t isTauIso, Bool_t isETMHF);
  void Tau_ETMPt(Float_t& taucut, Float_t& ETMcut, Bool_t isTauER, Bool_t isTauIso, Bool_t isETMHF);
  bool DoubleMuMass(float mu1cut, float mu2cut, float muER, int Qual, bool isOS, float Mlow, float Mhigh);

  bool DoubleMu_ETMHF_Jets( const float mu1pt, const float mu2pt, const int muqual, const bool ismuER, 
      const float etmcut, const bool isETMHF, const float singleJetpt, const float diJetpt);
  bool DoubleMu_HTT( Float_t cut1, Float_t cut2, int muqual, Bool_t isER, Float_t HTTcut);
  bool ETM_JetsComb( const float etmcut, const bool isETMHF, const float singleJetpt, const float diJetpt, const float triJetpt, const bool skipJetEta);
  bool DoubleMudRMax(float mu1cut, float mu2cut, float muER, int Qual, bool isOS, float dRMax);
  bool TripleEGIso(Float_t cut1, Float_t cut2, Float_t cut3, bool isIso1, bool isIso2, bool isIso3);
  bool TripleMu_DoubleMuMass( const float triMupt1, const float triMupt2, const float triMupt3, 
      const int triMuQual, const float diMupt1, const float diMupt2, const bool diMuOS, 
      const float diMuMmin, const float diMuMmax);
  bool TripleMuOS( const float triMupt1, const float triMupt2, const float triMupt3, const int triMuQual, 
      const float diMupt1, const float diMupt2, const bool diMuOS);

  bool DoubleEGIsoPer(Float_t cut1, Float_t cut2, Bool_t Iso1, Bool_t Iso2, Bool_t isER);
  bool TripleEGIsoPer(Float_t cut1, Float_t cut2, Float_t cut3, bool isIso1, bool isIso2, bool isIso3);
  bool SingleEGMT(Float_t EGcut, Float_t MTcut, Float_t ETMcut, Bool_t isIso, Bool_t isER);
  bool MuonCDC_dPhi(float mupt, int muQual, float muER, float dPhiMin, float dPhiMax);
  bool DoubleJet_dEtaMass(float Jet1Pt, float Jet2Pt, float EtaRestrict, bool isleading, float dEtaMax, float MassMin);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setting variables ~~~~~
  void SetMuonER(float newER);
  void SetUseUpgradeLyr1(bool option);
  void SetUseL1CaloTower(bool option);

 protected:
  bool PassMuonQual(int imu, int MuQual=1) const;

 private:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Hard coded cut value ~~~~~
 float jetCentFwd;
 float muonER;
 float eleER;
 float tauER;
 float MuJetCordPhi;
 float MuJetCordEta;
 float MuOpenJetCordPhi;
 float MuMudPhi;
 float Onia2015ER;
 int SelBx;
 int EGIsobit;
 
 bool UseUpgradeLyr1;
 bool UseL1CaloTower;
};

#endif   // ----- #ifndef __L1ALGOFACTORY_INC__  -----
