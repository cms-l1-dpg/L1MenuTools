// ===========================================================================
// 
//       Filename:  L1AlgoFactory.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/07/2016 01:41:46 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "L1AlgoFactory.h"

Bool_t L1AlgoFactory::SingleMu(Float_t ptcut, Bool_t isER, Int_t qualmin) {
  Float_t tmp_cut = -10.;
  SingleMuPt(tmp_cut,isER, qualmin);
  if(tmp_cut >= ptcut) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleMu(Float_t mu1pt, Float_t mu2pt, int muQual, Bool_t isER) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleMuPt(tmp_cut1,tmp_cut2,muQual,isER);
  if(tmp_cut1 >= mu1pt && tmp_cut2 >= mu2pt) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleMu_ETM(Float_t mu1pt, Float_t mu2pt, Float_t ETMcut, Bool_t isER) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_etm  = -10.;
  DoubleMu_ETMPt(tmp_cut1,tmp_cut2,tmp_etm,isER);
  if(tmp_cut1 >= mu1pt && tmp_cut2 >= mu2pt && tmp_etm >= ETMcut) return true;
  return false;
}

Bool_t L1AlgoFactory::Onia2015(Float_t mu1pt, Float_t mu2pt, Bool_t isER, Bool_t isOS, Int_t delta, Float_t MuEta) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Onia2015Pt(tmp_cut1,tmp_cut2,isER,isOS,delta, MuEta);
  if(tmp_cut1 >= mu1pt && tmp_cut2 >= mu2pt) return true;
  return false;
}

Bool_t L1AlgoFactory::TripleMu(Float_t mu1pt, Float_t mu2pt, Float_t mu3pt, Int_t qualmin) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_cut3 = -10.;
  TripleMuPt(tmp_cut1,tmp_cut2,tmp_cut3,qualmin);
  if(tmp_cut1 >= mu1pt && tmp_cut2 >= mu2pt &&  tmp_cut3 >= mu3pt) return true;
  return false;
}

Bool_t L1AlgoFactory::QuadMu(Float_t mu1pt, Float_t mu2pt, Float_t mu3pt, Float_t mu4pt, Int_t qualmin) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_cut3 = -10.;
  Float_t tmp_cut4 = -10.;
  QuadMuPt(tmp_cut1,tmp_cut2,tmp_cut3,tmp_cut4,qualmin);
  if(tmp_cut1 >= mu1pt && tmp_cut2 >= mu2pt &&  tmp_cut3 >= mu3pt && tmp_cut4 >= mu4pt) return true;
  return false;
}

Bool_t L1AlgoFactory::SingleEG(Float_t ptcut, Bool_t isIsolated, Bool_t isER) {
  Float_t tmp_cut1 = -10.;
  SingleEGPt(tmp_cut1,isIsolated,isER);
  if(tmp_cut1 >= ptcut) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleEG(Float_t ptcut1, Float_t ptcut2, Bool_t isIsolated) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleEGPt(tmp_cut1,tmp_cut2,isIsolated);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2) return true;
  return false;
}

Bool_t L1AlgoFactory::TripleEG(Float_t ptcut1, Float_t ptcut2, Float_t ptcut3) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_cut3 = -10.;
  TripleEGPt(tmp_cut1,tmp_cut2,tmp_cut3);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2 && tmp_cut3 >= ptcut3) return true;
  return false;
}

Bool_t L1AlgoFactory::SingleJet(Float_t ptcut, Bool_t isCentral) {
  Float_t tmp_cut1 = -10.;
  SingleJetPt(tmp_cut1,isCentral);
  if(tmp_cut1 >= ptcut) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleJet(Float_t ptcut1, Float_t ptcut2, Bool_t isCentral) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleJetPt(tmp_cut1,tmp_cut2,isCentral);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleJet_EtaRes_deltaEta(Float_t ptcut1, Float_t ptcut2, Float_t EtaRestrict, Float_t dEtaMax) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleJet_EtaRes_deltaEtaPt(tmp_cut1,tmp_cut2, EtaRestrict, dEtaMax);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleTauJetEta2p17(Float_t ptcut1, Float_t ptcut2, Bool_t isIsolated) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleTauJetEta2p17Pt(tmp_cut1,tmp_cut2,isIsolated);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2) return true;
  return false;
}

Bool_t L1AlgoFactory::TripleJet(Float_t ptcut1, Float_t ptcut2, Float_t ptcut3, Bool_t isCentral) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_cut3 = -10.;
  TripleJetPt(tmp_cut1,tmp_cut2,tmp_cut3,isCentral);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2 && tmp_cut3 >= ptcut3) return true;
  return false;
}

Bool_t L1AlgoFactory::QuadJet(Float_t ptcut1, Float_t ptcut2, Float_t ptcut3, Float_t ptcut4, Bool_t isCentral) {
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  Float_t tmp_cut3 = -10.;
  Float_t tmp_cut4 = -10.;
  QuadJetPt(tmp_cut1,tmp_cut2,tmp_cut3,tmp_cut4,isCentral);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2 && tmp_cut3 >= ptcut3 && tmp_cut4 >= ptcut4) return true;
  return false;
}

Bool_t L1AlgoFactory::Mu_EG(Float_t mucut, Float_t EGcut, Bool_t isIsolated, Int_t qualmin) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_elecut = -10.;
  Mu_EGPt(tmp_mucut,tmp_elecut,isIsolated,qualmin);
  if(tmp_mucut >= mucut && tmp_elecut >= EGcut) return true;
  return false;
}

Bool_t L1AlgoFactory::DoubleMu_EG(Float_t mucut, Float_t EGcut, int muonQual, bool isOS) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_elecut = -10.;
  DoubleMu_EGPt(tmp_mucut,tmp_elecut, muonQual, isOS);
  if(tmp_mucut >= mucut && tmp_elecut >= EGcut) return true;
  return false;
}

Bool_t L1AlgoFactory::Mu_DoubleEG(Float_t mucut, Float_t EGcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_elecut = -10.;
  Mu_DoubleEGPt(tmp_mucut,tmp_elecut);
  if(tmp_mucut >= mucut && tmp_elecut >= EGcut) return true;
  return false;
}

Bool_t L1AlgoFactory::Muer_JetCentral(Float_t mucut, Float_t jetcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_jetcut = -10.;
  Muer_JetCentralPt(tmp_mucut,tmp_jetcut);
  if(tmp_mucut >= mucut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::Mu_JetCentral_delta(Float_t mucut, Float_t jetcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_jetcut = -10.;
  Mu_JetCentral_deltaPt(tmp_mucut,tmp_jetcut);
  if(tmp_mucut >= mucut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::Mu_DoubleJetCentral(Float_t mucut, Float_t jetcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_jetcut = -10.;
  Mu_DoubleJetCentralPt(tmp_mucut,tmp_jetcut);
  if(tmp_mucut >= mucut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::EG_Jet(Float_t egcut, Float_t jetcut, Float_t dRMin, bool isEGER, bool isEGIso, bool isJetCen) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_jetcut = -10.;
  EG_JetPt(tmp_egcut,tmp_jetcut, dRMin, isEGER, isEGIso, isJetCen);
  if(tmp_egcut >= egcut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::EG_DoubleJetCentral(Float_t egcut, Float_t jetcut) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_jetcut = -10.;
  EG_DoubleJetCentralPt(tmp_egcut,tmp_jetcut);
  if(tmp_egcut >= egcut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::EGer_TripleJetCentral(Float_t egcut, Float_t jetcut, bool isEGER, bool isEGIso) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_jetcut = -10.;
  EGer_TripleJetCentralPt(tmp_egcut,tmp_jetcut, isEGER, isEGIso);
  if(tmp_egcut >= egcut && tmp_jetcut >= jetcut) return true;
  return false;
}

Bool_t L1AlgoFactory::IsoEGer_TauJetEta2p17(Float_t egcut, Float_t taucut, bool IsIsoTau){
  Float_t tmp_egcut  = -10.;
  Float_t tmp_taucut = -10.;
  IsoEGer_TauJetEta2p17Pt(tmp_egcut, tmp_taucut, IsIsoTau);
  if(tmp_egcut >= egcut && tmp_taucut >= taucut) return true;
  return false;
}

Bool_t L1AlgoFactory::QuadJetCentral_TauJet(Float_t jetcut, Float_t taucut, Bool_t isTauER, Bool_t isTauIso){
  Float_t tmp_jetcut = -10.;
  Float_t tmp_taucut = -10.;
  QuadJetCentral_TauJetPt(tmp_jetcut,tmp_taucut, isTauER, isTauIso);
  if(tmp_jetcut >= jetcut && tmp_taucut >= taucut) return true;
  return false;
}

inline Bool_t L1AlgoFactory::correlateInEta(Float_t mueta, Float_t jeteta, Float_t delta) {
  return fabs(mueta-jeteta) <= delta;
}

inline Bool_t L1AlgoFactory::correlatedRMax(Float_t eta1, Float_t eta2, Float_t phi1, Float_t phi2, Float_t dRbound)
{
  Float_t dEtaSq = pow(eta1 - eta2, 2);
  Float_t dPhiSq = pow(Phi_mpi_pi(phi1 - phi2), 2);
  if ( dEtaSq + dPhiSq > pow(dRbound, 2) ) return true;
  return false;
}

inline Bool_t L1AlgoFactory::correlatedRMin(Float_t eta1, Float_t eta2, Float_t phi1, Float_t phi2, Float_t dRbound)
{
  Float_t dEtaSq = pow(eta1 - eta2, 2);
  Float_t dPhiSq = pow(Phi_mpi_pi(phi1 - phi2), 2);
  if ( dEtaSq + dPhiSq < pow(dRbound, 2) ) return true;
  return false;
}

inline Bool_t L1AlgoFactory::correlateMT(Float_t pt1, Float_t pt2, Float_t phi1, Float_t phi2, Float_t MTcut)
{
  Float_t dPhi = Phi_mpi_pi(phi1 - phi2);
  Float_t MT2 = 2 * pt1 * pt2 * (1- cos(dPhi));
  if ( MT2 < pow(MTcut, 2) ) return true;
  return false;
}

inline Double_t L1AlgoFactory::degree(Double_t radian) {
  if (radian<0.)
    return 360.+(radian/acos(-1.)*180.);
  else
    return radian/acos(-1.)*180.;
}

Bool_t L1AlgoFactory::ETM(Float_t ETMcut) {
  Float_t tmp_cut = -10.;
  ETMVal(tmp_cut);
  if(tmp_cut >= ETMcut) return true;
  return false;
}

Bool_t L1AlgoFactory::HTT(Float_t HTTcut) {
  Float_t tmp_cut = -10.;
  HTTVal(tmp_cut);
  if(tmp_cut >= HTTcut) return true;
  return false;
}

Bool_t L1AlgoFactory::HTM(Float_t HTMcut) {
  Float_t tmp_cut = -10.;
  HTMVal(tmp_cut);
  if(tmp_cut >= HTMcut) return true;
  return false;
}

Bool_t L1AlgoFactory::ETT(Float_t ETTcut) {
  Float_t tmp_cut = -10.;
  ETTVal(tmp_cut);
  if(tmp_cut >= ETTcut) return true;
  return false;
}


void L1AlgoFactory::SingleMuPt(Float_t& ptcut, Bool_t isER, Int_t qualmin) {

  if(upgrade_->nMuons < 1) return;

  Float_t ptmax = -10.;

  for(UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    if(!PassMuonQual(imu, qualmin)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muonER && isER) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       

    if(pt >= ptmax) ptmax = pt;
  }

  ptcut = ptmax;
  return;
}

Bool_t L1AlgoFactory::ComplexSingleMu(Float_t& ptcut, Bool_t isER, Int_t qualmin, int muonType, int muonBX) {
  Float_t tmp_cut = -10.;
  ComplexSingleMuPt(tmp_cut,isER, qualmin, muonType, muonBX);
  if(tmp_cut >= ptcut) return true;
  return false;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::ComplexSingleMuPt
//  Description:  
// ===========================================================================
void L1AlgoFactory::ComplexSingleMuPt(Float_t& ptcut, Bool_t isER, Int_t qualmin, int muonType, int muonBX)
{
  if(upgrade_->nMuons < 1) return;

  Float_t ptmax = -10.;

  for(UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    // Select muonBX
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != muonBX) continue;

#ifndef  L1T_PRE_TSGV3 //muonTfMuonIdx doesn't exist at and before TSGv3
    // Select muonType 
    // 1:BMTF -> > 35 && < 72
    // 2:OMTF -> ( >= 18 && <=35 ) || (>=72 && <=89 )
    // 3:EMTF -> < 18 || > 89
    Int_t TfIdx = upgrade_->muonTfMuonIdx.at(imu);
    if (muonType == 1 && ! (TfIdx > 35 && TfIdx < 72))
      continue;
    if (muonType == 2 && ! ( ( TfIdx >= 18 && TfIdx <= 35) || (TfIdx>= 72 && TfIdx <= 89)))
      continue;
    if (muonType == 3 && ! (TfIdx< 18 || TfIdx < 89))
      continue;
#endif     // -----  not L1T_POST_TSGV3  -----

    if(!PassMuonQual(imu, qualmin)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muonER && isER) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       

    if(pt >= ptmax) ptmax = pt;
  }

  ptcut = ptmax;
}       // -----  end of function L1AlgoFactory::ComplexSingleMuPt  -----

void L1AlgoFactory::DoubleMuPt(Float_t& cut1, Float_t& cut2, int Qual, Bool_t isER) {

  Float_t mu1ptmax = -10.;
  Float_t mu2ptmax = -10.;

  if(upgrade_->nMuons < 2) return;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(isER && fabs(eta) > muonER) continue;
    if(!PassMuonQual(imu, Qual)) continue;

    if(pt >= mu1ptmax)
    {
      mu2ptmax = mu1ptmax;
      mu1ptmax = pt;
    }
    else if(pt >= mu2ptmax) mu2ptmax = pt;
  }

  if(mu2ptmax >= 0.){
    cut1 = mu1ptmax;
    cut2 = mu2ptmax;
  }

  return;
}

void L1AlgoFactory::TripleMuPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Int_t qualmin) {

  Float_t mu1ptmax = -10.;
  Float_t mu2ptmax = -10.;
  Float_t mu3ptmax = -10.;

  if(upgrade_->nMuons < 3) return;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, qualmin)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			

    if(pt >= mu1ptmax)
    {
      mu3ptmax = mu2ptmax;
      mu2ptmax = mu1ptmax;
      mu1ptmax = pt;
    }
    else if(pt >= mu2ptmax){
      mu3ptmax = mu2ptmax;
      mu2ptmax = pt;
    }
    else if(pt >= mu3ptmax) mu3ptmax = pt;
  }

  if(mu3ptmax >= 0.){
    cut1 = mu1ptmax;
    cut2 = mu2ptmax;
    cut3 = mu3ptmax;
  }

  return;
}

void L1AlgoFactory::QuadMuPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Float_t& cut4, Int_t qualmin) {

  Float_t mu1ptmax = -10.;
  Float_t mu2ptmax = -10.;
  Float_t mu3ptmax = -10.;
  Float_t mu4ptmax = -10.;

  if(upgrade_->nMuons < 4) return;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, qualmin)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			

    if(pt >= mu1ptmax)
    {
      mu4ptmax = mu3ptmax;
      mu3ptmax = mu2ptmax;
      mu2ptmax = mu1ptmax;
      mu1ptmax = pt;
    }
    else if(pt >= mu2ptmax){
      mu4ptmax = mu3ptmax;
      mu3ptmax = mu2ptmax;
      mu2ptmax = pt;
    }
    else if(pt >= mu3ptmax){
      mu4ptmax = mu3ptmax;
      mu3ptmax = pt;
    }
    else if(pt >= mu4ptmax) mu4ptmax = pt;
  }

  if(mu4ptmax >= 0.){
    cut1 = mu1ptmax;
    cut2 = mu2ptmax;
    cut3 = mu3ptmax;
    cut4 = mu4ptmax;
  }

  return;
}

void L1AlgoFactory::SingleEGPt(Float_t& cut, Bool_t isIsolated , Bool_t isER) {

  if(upgrade_->nEGs < 1) return;

  Float_t ptmax = -10.;

  for(UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    if(isIsolated && (upgrade_->egIso.at(ue) & 1 ) == 0) continue;
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER && isER) continue;  // eta = 5 - 16

    Float_t pt = upgrade_->egEt.at(ue);    // the rank of the electron
    if(pt >= ptmax) ptmax = pt;
  }

  cut = ptmax;

  return;
}

void L1AlgoFactory::DoubleEGPt(Float_t& cut1, Float_t& cut2, Bool_t isIsolated, Bool_t isER ) {

  if(upgrade_->nEGs < 2) return;

  Float_t ele1ptmax = -10.;
  Float_t ele2ptmax = -10.;

  Float_t ele1Phimax = -1000.;
  Float_t ele1Etamax = -1000.;

  Bool_t EG1_ER = false;
  Bool_t EG2_ER = false;

  Int_t EG1_isol = 0;
  Int_t EG2_isol = 0;

  for(UInt_t ue=0; ue < upgrade_->nEGs; ue++) {               
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    Float_t phi = upgrade_->egPhi.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);

    if(fabs(pt-ele1ptmax) < 0.001 && fabs(phi-ele1Phimax) < 0.001 && fabs(eta-ele1Etamax) < 0.001) continue; //to avoid double counting in noniso/relaxiso lists

    if(pt >= ele1ptmax)
    {
      ele2ptmax = ele1ptmax;
      EG2_ER = EG1_ER;
      EG2_isol = EG1_isol;
      ele1ptmax = pt;
      ele1Phimax = phi;
      ele1Etamax = eta;
      if(fabs(eta) < eleER) EG1_ER = true;
      else EG1_ER = false;
      EG1_isol = upgrade_->egIso.at(ue);
    }
    else if(pt >= ele2ptmax){
      ele2ptmax = pt;
      if(fabs(eta) < eleER) EG2_ER = true;
      else EG2_ER = false;
      EG1_isol = upgrade_->egIso.at(ue);
    }
  }

  if(isER && (!EG1_ER || !EG2_ER)) return;
  if(isIsolated && ( (EG1_isol  & EGIsobit ) == 0 || (EG2_isol & EGIsobit ) == 0)) return;

  if(ele2ptmax >= 0.){
    cut1 = ele1ptmax;
    cut2 = ele2ptmax;
  }

  return;
}

void L1AlgoFactory::TripleEGPt(Float_t& cut1, Float_t& cut2, Float_t& cut3 ) {

  if(upgrade_->nEGs < 2) return;

  Float_t ele1ptmax = -10.;
  Float_t ele2ptmax = -10.;
  Float_t ele3ptmax = -10.;

  Float_t ele1Phimax = -1000.;
  Float_t ele1Etamax = -1000.;

  Float_t ele2Phimax = -1000.;
  Float_t ele2Etamax = -1000.;

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    Float_t phi = upgrade_->egPhi.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);

    if(fabs(pt-ele1ptmax) < 0.001 && fabs(phi-ele1Phimax) < 0.001 && fabs(eta-ele1Etamax) < 0.001) continue; //to avoid double counting in noniso/relaxiso lists
    if(fabs(pt-ele2ptmax) < 0.001 && fabs(phi-ele2Phimax) < 0.001 && fabs(eta-ele2Etamax) < 0.001) continue; //to avoid double counting in noniso/relaxiso lists

    if(pt >= ele1ptmax)
    {
      ele3ptmax = ele2ptmax;
      ele2ptmax = ele1ptmax;
      ele1ptmax = pt;
      ele1Phimax = phi;
      ele1Etamax = eta;
    }
    else if(pt >= ele2ptmax){
      ele3ptmax = ele2ptmax;
      ele2ptmax = pt;
      ele2Phimax = phi;
      ele2Etamax = eta;
    }
    else if(pt >= ele3ptmax) ele3ptmax = pt;
  }

  if(ele3ptmax >= 0.){
    cut1 = ele1ptmax;
    cut2 = ele2ptmax;
    cut3 = ele3ptmax;
  }

  return;
}

void L1AlgoFactory::SingleTauPt(Float_t& cut, Bool_t isER, Bool_t isIsolated) {

  Float_t ptmax = -10.;
  for(UInt_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);        		
    if(bx != 0) continue;
    if(isER && fabs(upgrade_->tauEta.at(ue)) > tauER) continue;
    if(isIsolated && !upgrade_->tauIso.at(ue)) continue;

    Float_t pt = upgrade_->tauEt.at(ue);
    if(pt >= ptmax) ptmax = pt;
  }

  cut = ptmax;
  return;
}


void L1AlgoFactory::SingleJetPt(Float_t& cut, Bool_t isCentral) {

  Float_t ptmax = -10.;
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isCentral && isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= ptmax) ptmax = pt;
  }

  cut = ptmax;
  return;
}

void L1AlgoFactory::DoubleJetPt(Float_t& cut1, Float_t& cut2, Bool_t isCentral ) {

  Float_t maxpt1 = -10.;
  Float_t maxpt2 = -10.;

  if(upgrade_->nJets < 2) return;

  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isCentral && isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= maxpt1)
    {
      maxpt2 = maxpt1;
      maxpt1 = pt;
    }
    else if(pt >= maxpt2) maxpt2 = pt;
  }

  if(maxpt2 >= 0.){
    cut1 = maxpt1;
    cut2 = maxpt2;
  }

  return;
}

void L1AlgoFactory::DoubleJet_EtaRes_deltaEtaPt(Float_t& cut1, Float_t& cut2, Float_t &EtaRestrict, Float_t &dEtaMax ) {

  if(upgrade_->nJets < 2) return;

  Float_t maxpt1 = -10.;
  Float_t maxpt2 = -10.;
  Bool_t corr = false;
  std::vector<std::pair<Float_t,Float_t> > jetPairs;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    Float_t eta1 = upgrade_->jetEta.at(ue);
    if (fabs(eta1) > EtaRestrict) continue;  // eta = 6 - 16

    for(UInt_t ve=ue+1; ve < upgrade_->nJets; ve++) {
      if(ve == ue) continue;
      Int_t bx2 = upgrade_->jetBx.at(ve);        		
      if(bx2 != 0) continue;
      Float_t pt2 = upgrade_->jetEt.at(ve);
      Float_t eta2 = upgrade_->jetEta.at(ve);
      if (fabs(eta2) > EtaRestrict) continue;  // eta = 6 - 16

      if(correlateInEta(eta1, eta2, dEtaMax)){
        corr = true;
        Float_t tmp_pt1 = pt >= pt2 ? pt : pt2;
        Float_t tmp_pt2 = pt >= pt2 ? pt2 : pt;
        jetPairs.push_back(std::pair<Float_t,Float_t>(tmp_pt1,tmp_pt2));
      }
    }
  }

  if(corr){
    std::vector<std::pair<Float_t,Float_t> >::const_iterator jetPairIt  = jetPairs.begin();
    std::vector<std::pair<Float_t,Float_t> >::const_iterator jetPairEnd = jetPairs.end();
    for (; jetPairIt != jetPairEnd; ++jetPairIt) {
      Float_t pt1 = jetPairIt->first;
      Float_t pt2 = jetPairIt->second;

      if ( pt1 > maxpt1 || (fabs(maxpt1-pt1)<10E-2 && pt2>maxpt2) ) 
      {
        maxpt1 = pt1;
        maxpt2 = pt2;
      }
    }
  }

  cut1 = maxpt1;
  cut2 = maxpt2;

  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJet_ForwardBackward
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJet_ForwardBackward(Float_t ptcut1, Float_t ptcut2) 
{
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleJet_ForwardBackwardPt(tmp_cut1,tmp_cut2);
  if(tmp_cut1 >= ptcut1 && tmp_cut2 >= ptcut2) return true;

  return false;
}       // -----  end of function L1AlgoFactory::DoubleJet_ForwardBackward  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJet_ForwardBackwardPt
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJet_ForwardBackwardPt(Float_t& cut1, Float_t& cut2 )
{
  if(upgrade_->nJets < 2) return false;
  Float_t Fwdpt = -10.;
  Float_t Bwdpt = -10.;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Forward ~~~~~
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    if (!(upgrade_->jetEta.at(ue) > 3 &&  upgrade_->jetEta.at(ue) < 5))
      continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= Fwdpt) Fwdpt = pt;
  }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Backward ~~~~~
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    if (!(upgrade_->jetEta.at(ue) > -5 &&  upgrade_->jetEta.at(ue) < -3))
      continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= Bwdpt) Bwdpt = pt;
  }

  cut1 = Fwdpt;
  cut2 = Bwdpt;

  return true;
}       // -----  end of function L1AlgoFactory::DoubleJet_ForwardBackwardPt  -----

void L1AlgoFactory::DoubleTauJetEta2p17Pt(Float_t& cut1, Float_t& cut2, Bool_t isIsolated) {

  Float_t maxpt1 = -10.;
  Float_t maxpt2 = -10.;

  if(upgrade_->nTaus < 2) return;

  for(UInt_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);
    if(bx != 0) continue; 
    if(isIsolated && !upgrade_->tauIso.at(ue)) continue;
    Float_t pt = upgrade_->tauEt.at(ue);
    Float_t eta = upgrade_->tauEta.at(ue);
    if(fabs(eta) > tauER) continue;  // eta = 5 - 16

    if(pt >= maxpt1)
    {
      maxpt2 = maxpt1;
      maxpt1 = pt;
    }
    else if(pt >= maxpt2) maxpt2 = pt;
  }

  if(maxpt2 >= 0.){
    cut1 = maxpt1;
    cut2 = maxpt2;
  }

  return;
}

void L1AlgoFactory::TripleJetPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Bool_t isCentral) {

  Float_t jet1ptmax = -10.;
  Float_t jet2ptmax = -10.;
  Float_t jet3ptmax = -10.;

  if(upgrade_->nJets < 3) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > 3. ? true : false;
    if(isCentral && isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jet1ptmax)
    {
      jet3ptmax = jet2ptmax;
      jet2ptmax = jet1ptmax;
      jet1ptmax = pt;
    }
    else if(pt >= jet2ptmax){
      jet3ptmax = jet2ptmax;
      jet2ptmax = pt;
    }
    else if(pt >= jet3ptmax) jet3ptmax = pt;
  }

  if(jet3ptmax >= 0.){
    cut1 = jet1ptmax;
    cut2 = jet2ptmax;
    cut3 = jet3ptmax;
  }

  return;
}

//For now, only usable in Menu mode
Bool_t L1AlgoFactory::TripleJet_VBF(Float_t jet1, Float_t jet2, Float_t jet3, Int_t jetclass ) {

  Bool_t jet=false;
  Bool_t jetf=false;

  Bool_t jetc1=false;
  Bool_t jetc2=false;
  Bool_t jetc3=false;

  Bool_t jetf1=false;           
  Bool_t jetf2=false;
  Bool_t jetf3=false;

  Int_t n1=0;
  Int_t n2=0;
  Int_t n3=0;

  Int_t f1=0;
  Int_t f2=0;
  Int_t f3=0;

  if(upgrade_->nJets < 3) return false;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    Float_t pt = upgrade_->jetEt.at(ue);

    if (isFwdJet) {
      if(pt >= jet1) f1++;
      if(pt >= jet2) f2++;
      if(pt >= jet3) f3++;              
    } 
    else {
      if(pt >= jet1) n1++;
      if(pt >= jet2) n2++;
      if(pt >= jet3) n3++;
    }    
  }

  jet   = ( n1 >= 1 && n2 >= 2 && n3 >= 3 );
  jetf  = ( f1 >= 1 && f2 >= 2 && f3 >= 3 );

  jetc1 = ( n1 >= 1 && f2 >= 1 && f3 >= 2 );
  jetc2 = ( f1 >= 1 && n2 >= 1 && f3 >= 2 );
  jetc3 = ( f1 >= 1 && f2 >= 1 && n3 >= 2 );

  jetf1 = ( f1 >= 1 && n2 >= 1 && n3 >= 2 );
  jetf2 = ( n1 >= 1 && f2 >= 1 && n3 >= 2 );  
  jetf3 = ( n1 >= 1 && n2 >= 2 && f3 >= 1 );  

  if(jetclass == 1) return jet;
  else if(jetclass == 2) return jetf1;
  else if(jetclass == 3) return jetf2;
  else if(jetclass == 4) return jetf3;
  else if(jetclass == 5) return jetc1;
  else if(jetclass == 6) return jetc2;
  else if(jetclass == 7) return jetc3;
  else if(jetclass == 8) return jetf;

  return ( jet || jetf1 || jetf2 || jetf3 );
  //return ( jet || jetf1 || jetf2 || jetf3 || jetc1 || jetc2 || jetc3);
}

void L1AlgoFactory::QuadJetPt(Float_t& cut1, Float_t& cut2, Float_t& cut3, Float_t& cut4, Bool_t isCentral){

  Float_t jet1ptmax = -10.;
  Float_t jet2ptmax = -10.;
  Float_t jet3ptmax = -10.;
  Float_t jet4ptmax = -10.;

  if(upgrade_->nJets < 4) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isCentral && isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jet1ptmax)
    {
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = jet1ptmax;
      jet1ptmax = pt;
    }
    else if(pt >= jet2ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = pt;
    }
    else if(pt >= jet3ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = pt;
    }
    else if(pt >= jet4ptmax) jet4ptmax = pt;
  }

  if(jet4ptmax >= 0.){
    cut1 = jet1ptmax;
    cut2 = jet2ptmax;
    cut3 = jet3ptmax;
    cut4 = jet4ptmax;
  }

  return;
}

void L1AlgoFactory::Mu_EGPt(Float_t& mucut, Float_t& EGcut, Bool_t isIsolated, Int_t qualmin) {

  Float_t muptmax = -10.;

  for(UInt_t imu=0; imu < upgrade_->nMuons; imu++) {   
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, qualmin)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(pt >= muptmax) muptmax = pt;
  }

  Float_t eleptmax = -10.;

  for(UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    if(isIsolated && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);    // the rank of the electron
    if(pt >= eleptmax) eleptmax = pt;
  }

  if(muptmax >= 0. && eleptmax >= 0.){
    mucut = muptmax;
    EGcut = eleptmax;
  }

  return;
}


void L1AlgoFactory::DoubleMu_EGPt(Float_t& mucut, Float_t& EGcut, int muonQual, bool isOS){

  //Float_t muptmax = -10.;
  Float_t second_muptmax = -10.;
  Float_t EGmax = -10.;

  if(upgrade_->nMuons < 2) return;
  std::vector<std::pair<unsigned, unsigned> > muonPairs;

  for (UInt_t imu1=0; imu1 < upgrade_->nMuons; imu1++) {
    Int_t bx1 = upgrade_->muonBx.at(imu1);
    if(bx1 != 0) continue;
    Int_t charge1 = upgrade_->muonChg.at(imu1);
    if(!PassMuonQual(imu1, muonQual)) continue;


    for (UInt_t imu2=imu1+1; imu2 < upgrade_->nMuons; imu2++) {
      Int_t bx2 = upgrade_->muonBx.at(imu2);
      if(bx2 != 0) continue;
      Int_t charge2 = upgrade_->muonChg.at(imu2);
      if(!PassMuonQual(imu2, muonQual)) continue;

      if(isOS && charge1*charge2 > 0) continue;
      muonPairs.push_back(std::make_pair(imu1, imu2));
    }
  }


  for(auto mp : muonPairs)
  {
    //Float_t pt1 = max(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));
    Float_t pt2 = min(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));
    if(pt2 >= second_muptmax) second_muptmax = pt2;
  }

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    if (pt >= EGmax) EGmax = pt;
  }  // end loop over EM objects

  if(second_muptmax >= 0.){
    mucut = second_muptmax;
    EGcut = EGmax;
  }

  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleMu_ETMPt
//  Description:  /* cursor */
// ===========================================================================
void L1AlgoFactory::DoubleMu_ETMPt(Float_t &cut1, Float_t &cut2, Float_t& ETMcut, Bool_t isER)
{
  Float_t muptmax = -10.;
  Float_t second_muptmax = -10.;
  Float_t TheETM = -10.;

  if(upgrade_->nMuons < 2) return;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 1)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(isER && fabs(eta) > muonER) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(pt >= muptmax){
      second_muptmax = muptmax;
      muptmax = pt;
    }
    else if(pt >= second_muptmax) second_muptmax = pt;
  }

  ETMVal(TheETM);


  if(second_muptmax >= 0.){
    cut1 = muptmax;
    cut2 = second_muptmax;
    ETMcut = TheETM;
  }

  return;
}       // -----  end of function L1AlgoFactory::DoubleMu_ETMPt  -----


void L1AlgoFactory::Mu_DoubleEGPt(Float_t& mucut, Float_t& EGcut ) {

  Float_t muptmax    = -10.;
  Float_t eleptmax1  = -10.;
  Float_t eleptmax2  = -10.;
  Float_t ele1Phimax = -1000.;
  Float_t ele1Etamax = -1000.;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(!PassMuonQual(imu, 2)) continue;
    if(pt >= muptmax) muptmax = pt; 
  }

  if(upgrade_->nEGs < 2) return;

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt  = upgrade_->egEt.at(ue);
    Float_t phi = upgrade_->egPhi.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);

    if(fabs(pt-eleptmax1) < 0.001 && fabs(phi-ele1Phimax) < 0.001 && fabs(eta-ele1Etamax) < 0.001) continue; //to avoid double counting in noniso/relaxiso lists

    if(pt >= eleptmax1){
      eleptmax2 = eleptmax1;
      eleptmax1 = pt;
      ele1Phimax = phi;
      ele1Etamax = eta;
    }
    else if(pt >= eleptmax2) eleptmax2 = pt;
  }

  if(muptmax >= 0. && eleptmax2 >= 0.){
    mucut = muptmax;
    EGcut = eleptmax2;
  }

  return;
}

void L1AlgoFactory::Mu_DoubleJetCentralPt(Float_t& mucut, Float_t& jetcut) {

  Float_t muptmax = -10.;
  Float_t jetptmax1 = -10.;
  Float_t jetptmax2 = -10.;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 2)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if (pt >= muptmax) muptmax = pt;
  }

  if(upgrade_->nJets < 2) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > 3. ? true : false;
    if(isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= jetptmax1){
      jetptmax2 = jetptmax1;
      jetptmax1 = pt;
    }
    else if(pt >= jetptmax2) jetptmax2 = pt;
  }

  if(muptmax >= 0. && jetptmax2 >= 0.){
    mucut = muptmax;
    jetcut = jetptmax2;
  }

  return;
}

void L1AlgoFactory::Muer_JetCentralPt(Float_t& mucut, Float_t& jetcut) {

  Float_t muptmax = -10.;
  Float_t jetptmax = -10.;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 2)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > 2.1) continue;

    if (pt >= muptmax) muptmax = pt;
  }

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > 3. ? true : false;
    if(isFwdJet) continue;

    Float_t pt = upgrade_->jetEt.at(ue);
    if (pt >= jetptmax) jetptmax = pt;
  }

  if(muptmax >= 0. && jetptmax >= 0.){
    mucut = muptmax;
    jetcut = jetptmax;
  }

  return;
}

void L1AlgoFactory::Mu_JetCentral_deltaPt(Float_t& mucut, Float_t& jetcut) {

  Float_t muptmax = -10.;
  Float_t jetptmax = -10.;
  Bool_t correlate = false;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 2)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu); 
    Float_t phi = upgrade_->muonPhiAtVtx.at(imu); 
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu); 
    Float_t phi = upgrade_->muonPhi.at(imu); 
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----

    for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
      Int_t bxj = upgrade_->jetBx.at(ue);        		
      if(bxj != 0) continue;
      Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
      if(isFwdJet) continue;

      Float_t ptj = upgrade_->jetEt.at(ue);
      Float_t phijet = upgrade_->jetPhi.at(ue);
      Float_t etajet = upgrade_->jetEta.at(ue);


      //MuJetCordPhi,  MuJetCordEta
      if(Phi_mpi_pi(phi-phijet) <= MuJetCordPhi && fabs(eta-etajet) <= MuJetCordEta){
        correlate = true;
        if(pt >= muptmax) muptmax = pt;
        if(ptj >= jetptmax) jetptmax = ptj;
      }
    }
  }

  if(correlate){
    mucut = muptmax;
    jetcut = jetptmax;
  }

  return;
}

void L1AlgoFactory::EG_JetPt(Float_t& EGcut, Float_t& Jetcut, Float_t dRMin, bool isEGER, bool isEGIso, bool isJetCen) {

  Float_t eleptmax = -10.;
  Float_t jetptmax = -10.;

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t egbx = upgrade_->egBx.at(ue);        		
    if(egbx != 0) continue;
    if(isEGIso && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t EGeta = upgrade_->egEta.at(ue);
    if(fabs(EGeta) > eleER && isEGER) continue;  // eta = 5 - 16
    Float_t EGphi = upgrade_->egPhi.at(ue);


    for (UInt_t uj=0; uj < upgrade_->nJets; uj++) {        
      Int_t jetbx = upgrade_->jetBx.at(uj);        		
      if(jetbx != 0) continue;
      Float_t jeteta = upgrade_->jetEta.at(uj);
      Float_t jetphi = upgrade_->jetPhi.at(uj);
      Bool_t isFwdJet = fabs(jeteta) > jetCentFwd ? true : false;
      if(isJetCen && isFwdJet) continue;

      if (correlatedRMin (EGeta, jeteta, EGphi, jetphi,dRMin)) continue;

      Float_t jetpt = upgrade_->jetEt.at(uj);
      Float_t EGpt  = upgrade_->egEt.at(ue);
      if (jetpt >= jetptmax &&  EGpt >= eleptmax)
      {
        jetptmax = jetpt;
        eleptmax = EGpt;
      }
    }
  }

  if(eleptmax >= 0. && jetptmax >= 0.){
    EGcut = eleptmax;
    Jetcut = jetptmax;
  }

  return;
}

void L1AlgoFactory::EG_DoubleJetCentralPt(Float_t& EGcut, Float_t& jetcut) {

  Float_t eleptmax = -10.;
  Float_t jetptmax1 = -10.;
  Float_t jetptmax2 = -10.;

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt  = upgrade_->egEt.at(ue);
    if(pt >= eleptmax) eleptmax = pt; 
  }

  if(upgrade_->nJets < 2) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > 3. ? true : false;
    if(isFwdJet) continue;
    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jetptmax1){
      jetptmax2 = jetptmax1;
      jetptmax1 = pt;
    }
    else if(pt >= jetptmax2) jetptmax2 = pt;
  }

  if(eleptmax >= 0. && jetptmax2 >= 0.){
    EGcut = eleptmax;
    jetcut = jetptmax2;
  }

  return;
}

void L1AlgoFactory::EGer_TripleJetCentralPt(Float_t& EGcut, Float_t& jetcut, bool &isEGER, bool &isEGIso) {

  Float_t eleptmax = -10.;
  Float_t elemaxeta = -10.;
  Float_t jetptmax1 = -10.;
  Float_t jetptmax2 = -10.;
  Float_t jetptmax3 = -10.;

  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++){
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    Float_t pt  = upgrade_->egEt.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);
    if(isEGER && fabs(eta) > eleER) continue;  // eta = 5 - 16
    if(isEGIso && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    if(pt >= eleptmax){
      eleptmax = pt; 
      elemaxeta = upgrade_->egEta.at(ue);
    }
  }  // end loop over EM objects

  if(upgrade_->nJets < 3) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > 3. ? true : false;
    if(isFwdJet) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    Float_t jeteta = upgrade_->jetEta.at(ue);

    if(jeteta == elemaxeta) continue;   //both are binned with the same binning

    if(pt >= jetptmax1){
      jetptmax3 = jetptmax2;
      jetptmax2 = jetptmax1;
      jetptmax1 = pt;
    }
    else if(pt >= jetptmax2){
      jetptmax3 = jetptmax2;
      jetptmax2 = pt;
    }
    else if(pt >= jetptmax3) jetptmax3 = pt;

  }

  if(eleptmax >= 0. && jetptmax3 >= 0.){
    EGcut = eleptmax;
    jetcut = jetptmax3;
  }

  return;
}

void L1AlgoFactory::IsoEGer_TauJetEta2p17Pt(Float_t& egcut, Float_t& taucut, bool& IsIsoTau) {

  Float_t eleptmax  = -10.;
  Float_t eleetamax = -999.;
  Float_t maxpttau  = -10.;

  if(upgrade_->nEGs < 1) return;
  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);        		
    if(bx != 0) continue;
    if((upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t pt  = upgrade_->egEt.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER) continue;  // eta = 5 - 16
    if(pt >= eleptmax){
      eleptmax = pt;
      eleetamax = eta;
    }
  }

  for(UInt_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);        		
    if(bx != 0) continue; 
    Float_t pt = upgrade_->tauEt.at(ue);
    Float_t eta = upgrade_->tauEta.at(ue);
    if(IsIsoTau && !upgrade_->tauIso.at(ue)) continue;

    if(fabs(eta) > tauER) continue;  // eta = 5 - 16

    if(fabs(eta-eleetamax) < 0.2) continue;

    if(pt >= maxpttau) maxpttau = pt;
  }

  if(eleptmax >= 0.){
    egcut  = eleptmax;
    taucut = maxpttau;
  }

  return;
}

void L1AlgoFactory::QuadJetCentral_TauJetPt(Float_t& jetcut, Float_t& taucut, Bool_t isTauER, Bool_t isTauIso){

  Float_t jet1ptmax = -10.;
  Float_t jet2ptmax = -10.;
  Float_t jet3ptmax = -10.;
  Float_t jet4ptmax = -10.;
  Float_t maxpttau  = -10.;

  if(upgrade_->nJets < 4) return;
  if(upgrade_->nTaus < 1) return;

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isFwdJet) continue;
    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jet1ptmax)
    {
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = jet1ptmax;
      jet1ptmax = pt;
    }
    else if(pt >= jet2ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = pt;
    }
    else if(pt >= jet3ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = pt;
    }
    else if(pt >= jet4ptmax) jet4ptmax = pt;
  }

  for(UInt_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);        		
    if(bx != 0) continue; 
    if(isTauER && fabs(upgrade_->tauEta.at(ue)) > tauER) continue;
    if(isTauIso && !upgrade_->tauIso.at(ue)) continue;
    Float_t pt = upgrade_->tauEt.at(ue);
    if(pt >= maxpttau) maxpttau = pt;
  }

  if(jet4ptmax >= 0. && maxpttau >= 0.){
    jetcut = jet4ptmax;
    taucut = maxpttau;
  }

  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::GetSumEtIdx
//  Description:  /* cursor */
// ===========================================================================
int L1AlgoFactory::GetSumEtIdx(EtSumType type)
{
  for (unsigned int i = 0; i < upgrade_->sumType.size(); ++i )
  {
    if (upgrade_->sumType.at(i) == type && upgrade_->sumBx.at(i) == SelBx)
      return i;
  }
  return -1;
}       // -----  end of function L1AlgoFactory::GetSumEtIdx  -----

void L1AlgoFactory::ETMHFVal(Float_t& ETMcut ) 
{

  Float_t TheETM = -10;
  int idx = GetSumEtIdx(EtSumType::ETMHF);
  if (idx == -1) return;
  assert(upgrade_->sumType.at(idx) == EtSumType::ETMHF);
  if(upgrade_->sumBx.at(idx)==SelBx) TheETM =upgrade_->sumEt.at(idx);
  ETMcut = TheETM;

  return;
}

void L1AlgoFactory::ETMVal(Float_t& ETMcut ) {

  Float_t TheETM = -10;
  if (UseUpgradeLyr1 && upgrade_lyr1_)
  {
    for (unsigned int i = 0; i < upgrade_lyr1_->sumType.size(); ++i )
    {
      if (upgrade_lyr1_->sumType.at(i) == EtSumType::ETM && upgrade_lyr1_->sumBx.at(i) == 0)
        TheETM = upgrade_lyr1_->sumEt.at(i);
    }
  } 

  if (UseL1CaloTower && l1CaloTower_) {
    Double_t metX = 0.0;
    Double_t metY = 0.0;
    for(int jTower=0; jTower< l1CaloTower_ ->nTower; ++jTower){
      Int_t ieta = l1CaloTower_->ieta[jTower];
      Int_t iphi = l1CaloTower_->iphi[jTower];
      Int_t iet = l1CaloTower_->iet[jTower];
      if( abs(ieta) < 29){
        Double_t phi = (Double_t)iphi * TMath::Pi()/36.;
        Double_t et = 0.5 * (Double_t)iet;
        metX += et * TMath::Cos(phi);
        metY += et * TMath::Sin(phi);
      }
    }
    TheETM = TMath::Sqrt(metX*metX + metY*metY);
  } 

  if (!UseL1CaloTower && !UseUpgradeLyr1)
  {
    int idx = GetSumEtIdx(EtSumType::ETM);
    if (idx == -1) return;
    assert(upgrade_->sumType.at(idx) == EtSumType::ETM);
    if(upgrade_->sumBx.at(idx)==SelBx) TheETM =upgrade_->sumEt.at(idx);
  }


  ETMcut = TheETM;

  return;
}

void L1AlgoFactory::HTTVal(Float_t& HTTcut) {

  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTT);
  if (idx == -1) return;
  assert(upgrade_->sumType.at(idx) == EtSumType::HTT);
  if(upgrade_->sumBx.at(idx)==SelBx) TheHTT =upgrade_->sumEt.at(idx);
  HTTcut = TheHTT;
  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::HTTHFVal
//  Description:  
// ===========================================================================
void L1AlgoFactory::HTTHFVal(Float_t& HTTcut)
{
  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTTHF);
  if (idx == -1) return;
  assert(upgrade_->sumType.at(idx) == EtSumType::HTTHF);
  if(upgrade_->sumBx.at(idx)==SelBx) TheHTT =upgrade_->sumEt.at(idx);
  HTTcut = TheHTT;
  return;
}       // -----  end of function L1AlgoFactory::HTTHFVal  -----

void L1AlgoFactory::HTMVal(Float_t& HTMcut) {

  Float_t TheHTM = -10;
  int idx= GetSumEtIdx(EtSumType::HTM);
  if (idx == -1) return;
  assert(upgrade_->sumType.at(idx) == EtSumType::HTM);
  if(upgrade_->sumBx.at(idx)==SelBx) TheHTM =upgrade_->sumEt.at(idx);
  HTMcut = TheHTM;
  return;
}

void L1AlgoFactory::ETTVal(Float_t& ETTcut) {

  Float_t TheETT = -10;
  if (UseUpgradeLyr1 && upgrade_lyr1_)
  {
    for (unsigned int i = 0; i < upgrade_lyr1_->sumType.size(); ++i )
    {
      if (upgrade_lyr1_->sumType.at(i) == EtSumType::ETT && upgrade_lyr1_->sumBx.at(i) == 0)
        TheETT = upgrade_lyr1_->sumEt.at(i);
    }
  }

  if (UseL1CaloTower && l1CaloTower_) {
    double temp = 0.0;
    for(int jTower=0; jTower< l1CaloTower_ ->nTower; ++jTower){
      Int_t ieta = l1CaloTower_->ieta[jTower];
      Int_t iet = l1CaloTower_->iet[jTower];
      if( abs(ieta) < 29){
        Double_t et = 0.5 * (Double_t)iet;
        temp+= et;
      }
    }
    TheETT= temp;
  } 

  if (!UseL1CaloTower && !UseUpgradeLyr1)
  {
    int idx= GetSumEtIdx(EtSumType::ETT);
    if (idx == -1) return;
    assert(upgrade_->sumType.at(idx) == EtSumType::ETT);
    if(upgrade_->sumBx.at(idx)==SelBx) TheETT =upgrade_->sumEt.at(idx);
  }


  ETTcut = TheETT;
  return;
}

//**************************************************************************//
//                                   Cross                                  //
//**************************************************************************//
// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::PassMuonQual
//  Description:  
// ===========================================================================
bool L1AlgoFactory::PassMuonQual(int imu, int MuQual) const
{
  if (MuQual == 0)
    return upgrade_->muonQual.at(imu) >= 4; // l1t-tsg-v4 for SingleMuOpen
  if (MuQual == 1)
    return upgrade_->muonQual.at(imu) >= 8; // l1t-tsg-v4 for Double/TripleMu
  if (MuQual == 2)
    return upgrade_->muonQual.at(imu) == 12; // l1t-tsg-v4 for SingleMu

  return false; //No good
}       // -----  end of function L1AlgoFactory::PassMuonQual  -----

Bool_t L1AlgoFactory::Mu_HTT(Float_t mucut, Float_t HTcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_HTcut = -10.;
  Mu_HTTPt(tmp_mucut,tmp_HTcut);
  if(tmp_mucut >= mucut && tmp_HTcut >= HTcut) return true;
  return false;
}

void L1AlgoFactory::Mu_HTTPt(Float_t& mucut, Float_t& HTcut ) {

  Float_t muptmax = -10.;

  Int_t Nmu = upgrade_->nMuons;
  for (Int_t imu=0; imu < Nmu; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       
    if(!PassMuonQual(imu, 2)) continue;
    if(pt >= muptmax) muptmax = pt;
  }

  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTT);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
    TheHTT =upgrade_->sumEt.at(idx);

  if(muptmax >= 0.){
    mucut = muptmax;
    HTcut = TheHTT;
  }

  return;
}

Bool_t L1AlgoFactory::Muer_ETM(Float_t mucut, Float_t ETMcut) {
  Float_t tmp_mucut = -10.;
  Float_t tmp_ETMcut = -10.;
  Muer_ETMPt(tmp_mucut,tmp_ETMcut);
  if(tmp_mucut >= mucut && tmp_ETMcut >= ETMcut) return true;
  return false;
}

void L1AlgoFactory::Muer_ETMPt(Float_t& mucut, Float_t& ETMcut ) {

  Float_t muptmax = -10.;

  Int_t Nmu = upgrade_->nMuons;
  for (Int_t imu=0; imu < Nmu; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       
    if(!PassMuonQual(imu, 2)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muonER) continue;
    if(pt >= muptmax) muptmax = pt;
  }

  Float_t TheETM = -10;
  int idx = GetSumEtIdx(EtSumType::ETM);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
    TheETM =upgrade_->sumEt.at(idx);

  if(muptmax >= 0.){
    mucut = muptmax;
    ETMcut = TheETM;
  }

  return;
}

Bool_t L1AlgoFactory::SingleEG_Eta2p1_HTT(Float_t egcut, Float_t HTTcut, Bool_t isIsolated) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_HTTcut = -10.;
  SingleEG_Eta2p1_HTTPt(tmp_egcut,tmp_HTTcut,isIsolated);
  if(tmp_egcut >= egcut && tmp_HTTcut >= HTTcut) return true;
  return false;
}

void L1AlgoFactory::SingleEG_Eta2p1_HTTPt(Float_t& egcut, Float_t& HTTcut, Bool_t isIsolated) {

  Float_t eleptmax = -10.;

  Int_t Nele = upgrade_->nEGs;
  for (Int_t ue=0; ue < Nele; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    if(isIsolated && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER ) continue;  
    Float_t pt = upgrade_->egEt.at(ue);   
    if(pt >= eleptmax) eleptmax = pt;
  }

  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTT);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
    TheHTT =upgrade_->sumEt.at(idx);

  if(eleptmax >= 0.){
    egcut = eleptmax;
    HTTcut = TheHTT;
  }

  return;
}

Bool_t L1AlgoFactory::Muer_TauJetEta2p17(Float_t mucut, Float_t taucut, Bool_t isIsolated){
  Float_t tmp_mucut  = -10.;
  Float_t tmp_taucut = -10.;
  Muer_TauJetEta2p17Pt(tmp_mucut, tmp_taucut,isIsolated);
  if(tmp_mucut >= mucut && tmp_taucut >= taucut) return true;
  return false;
}

void L1AlgoFactory::Muer_TauJetEta2p17Pt(Float_t& mucut, Float_t& taucut, Bool_t isIsolated) {

  Float_t maxptmu  = -10.;
  Float_t maxpttau = -10.;

  Int_t Nmu = upgrade_->nMuons;
  if(Nmu < 1) return;
  for (Int_t imu=0; imu < Nmu; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       
    if(!PassMuonQual(imu, 2)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muonER ) continue;
    if(pt >= maxptmu) maxptmu = pt;
  }

  for(Int_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);        		
    if(bx != 0) continue; 
    if(isIsolated && !upgrade_->tauIso.at(ue)) continue;

    Float_t pt = upgrade_->tauEt.at(ue);
    Float_t eta = upgrade_->tauEta.at(ue);
    if(fabs(eta) > tauER) continue;  // eta = 5 - 16

    if(pt >= maxpttau) maxpttau = pt;
  }

  if(maxptmu >= 0.){
    mucut  = maxptmu;
    taucut = maxpttau;
  }

  return;

}

Bool_t L1AlgoFactory::DoubleJetCentral_ETM(Float_t jetcut1, Float_t jetcut2, Float_t ETMcut) {
  Float_t tmp_jetcut1 = -10.;
  Float_t tmp_jetcut2 = -10.;
  Float_t tmp_ETMcut  = -10.;
  DoubleJetCentral_ETMPt(tmp_jetcut1,tmp_jetcut2,tmp_ETMcut);
  if(tmp_jetcut1 >= jetcut1 && tmp_jetcut2 >= jetcut2 && tmp_ETMcut >= ETMcut) return true;
  return false;
}

void L1AlgoFactory::DoubleJetCentral_ETMPt(Float_t& jetcut1, Float_t& jetcut2, Float_t& ETMcut){

  Float_t jetptmax1 = -10.;
  Float_t jetptmax2 = -10.;

  Float_t TheETM = -10;
  int idx = GetSumEtIdx(EtSumType::ETM);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
    TheETM =upgrade_->sumEt.at(idx);

  if(upgrade_->nJets < 2) return;

  for (Int_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isFwdJet) continue;
    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jetptmax1){
      jetptmax2 = jetptmax1;
      jetptmax1 = pt;
    }
    else if(pt >= jetptmax2) jetptmax2 = pt;
  }       

  if(jetptmax2 >= 0.){
    jetcut1 = jetptmax1;
    jetcut2 = jetptmax2;
    ETMcut = TheETM;
  }

  return;
}

Bool_t L1AlgoFactory::DoubleEG_HT(Float_t egcut, Float_t HTcut) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_HTcut = -10.;
  DoubleEG_HTPt(tmp_egcut,tmp_HTcut);
  if(tmp_egcut >= egcut && tmp_HTcut >= HTcut) return true;
  return false;
}

void L1AlgoFactory::DoubleEG_HTPt(Float_t& EGcut, Float_t& HTcut) {

  Float_t eleptmax1  = -10.;
  Float_t eleptmax2  = -10.;
  Float_t ele1Phimax = -1000.;
  Float_t ele1Etamax = -1000.;

  if(upgrade_->nEGs < 2) return;

  for(Int_t ue=0; ue < upgrade_->nEGs ; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    Float_t phi = upgrade_->egPhi.at(ue);
    Float_t eta = upgrade_->egEta.at(ue);

    if(fabs(pt-eleptmax1) < 0.001 && fabs(phi-ele1Phimax) < 0.001 && fabs(eta-ele1Etamax) < 0.001) continue; //to avoid double counting in noniso/relaxiso lists

    if(pt >= eleptmax1){
      eleptmax2 = eleptmax1;
      eleptmax1 = pt;
      ele1Phimax = phi;
      ele1Etamax = eta;
    }
    else if(pt >= eleptmax2) eleptmax2 = pt;
  }

  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTT);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
    TheHTT =upgrade_->sumEt.at(idx);

  if(eleptmax2 >= 0.){
    EGcut = eleptmax2;
    HTcut = TheHTT;
  }

  return;
}
Bool_t L1AlgoFactory::Jet_MuOpen_Mu_dPhiMuMu1(Float_t jetcut, Float_t mucut, int MuQual) {
  Float_t tmp_jetcut = -10.;
  Float_t tmp_mucut = -10.;
  Jet_MuOpen_Mu_dPhiMuMu1Pt(tmp_jetcut,tmp_mucut, MuQual);
  if(tmp_jetcut >= jetcut && tmp_mucut >= mucut) return true;
  return false;
}

void L1AlgoFactory::Jet_MuOpen_Mu_dPhiMuMu1Pt(Float_t& jetcut, Float_t& mucut, int MuQual) {

  //Find the highest pt jet with deltaphi condition
  Float_t jetptmax = -10.;
  Int_t Nj = upgrade_->nJets;
  Int_t Nmu = upgrade_->nMuons;

  for(Int_t ue=0; ue < Nj; ue++){
    Int_t bxjet = upgrade_->jetBx.at(ue);        		
    if(bxjet != 0) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt < jetptmax) continue;
    Float_t phijet = upgrade_->jetPhi.at(ue);

    Bool_t corr = false;

    for(Int_t imu=0; imu < Nmu; imu++){
      Int_t bx = upgrade_->muonBx.at(imu);
      if (bx != 0) continue;
      if(!PassMuonQual(imu, MuQual)) continue;
      Float_t muonpt = upgrade_->muonEt.at(imu);			
      if(muonpt < 0.) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t muphi = upgrade_->muonPhiAtVtx.at(imu);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t muphi = upgrade_->muonPhi.at(imu);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      if(fabs(muphi-phijet) < MuOpenJetCordPhi) corr = true;
    }

    if(corr) jetptmax = pt;
  }

  //Loop over the muons list twice and save all pairs that satisfy the deltaphi veto
  Bool_t corr = false;
  Float_t maxpt1 = -10.;
  Float_t maxpt2 = -10.;

  std::vector<std::pair<Float_t,Float_t> > muonPairs;
  for (Int_t imu=0; imu < Nmu; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if (bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(!PassMuonQual(imu, MuQual)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t phi1 = upgrade_->muonPhiAtVtx.at(imu);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t phi1 = upgrade_->muonPhi.at(imu);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----

    for (Int_t imu2=imu+1; imu2 < Nmu; imu2++) {
      if (imu2 == imu) continue;
      Int_t bx2 = upgrade_->muonBx.at(imu2);
      if (bx2 != 0) continue;
      Float_t pt2 = upgrade_->muonEt.at(imu2);			
      if(!PassMuonQual(imu, MuQual)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t phi2 = upgrade_->muonPhiAtVtx.at(imu2);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t phi2 = upgrade_->muonPhi.at(imu2);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----

      Float_t dphi = phi1 - phi2; //Should get the binning, but for GMT is quite fine
      if(fabs(dphi) > MuMudPhi){
        corr = true;
        muonPairs.push_back(std::pair<Float_t,Float_t>(pt,pt2));
      }
    }
  }

  //Select the muon pair in which one of the two muons is the highest pt muon satisfying the deltaphi veto
  if(corr){
    std::vector<std::pair<Float_t,Float_t> >::const_iterator muonPairIt  = muonPairs.begin();
    std::vector<std::pair<Float_t,Float_t> >::const_iterator muonPairEnd = muonPairs.end();
    for(; muonPairIt != muonPairEnd; ++muonPairIt){
      Float_t pt1 = muonPairIt->first;
      Float_t pt2 = muonPairIt->second;

      if(pt1 > maxpt1 || (fabs(maxpt1-pt1)<10E-2 && pt2>maxpt2) ) {
        maxpt1 = pt1;
        maxpt2 = pt2;
      }
    }
  }

  Float_t maxptmu = maxpt1 > maxpt2 ? maxpt1 : maxpt2; //only the highest pt muon counts for the correlation, the second muon is Open

  if(jetptmax > 0. && maxptmu > 0.){
    jetcut = jetptmax;
    mucut = maxptmu;
  }

  return;
}

Bool_t L1AlgoFactory::DoubleMuOpen(Float_t mu1pt) {
  Float_t tmp_cut = -10.;
  DoubleMuOpenPt(tmp_cut);
  if(tmp_cut >= mu1pt) return true;
  return false;
}

void L1AlgoFactory::DoubleMuOpenPt(Float_t& cut) {

  Int_t n2=0;
  Float_t ptmax = -10.;

  for(Int_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(!PassMuonQual(imu, 0)) continue;
    if( pt >= ptmax ){
      ptmax = pt;
    }
    if(pt >= 0.) n2++;
  }

  if(n2>=2) cut = ptmax;
  else cut = -10.;

  return;
}

Bool_t L1AlgoFactory::DoubleMuXOpen(Float_t mu1pt) {
  Float_t tmp_cut = -10.;
  DoubleMuXOpenPt(tmp_cut);
  if(tmp_cut >= mu1pt) return true;
  return false;
}

void L1AlgoFactory::DoubleMuXOpenPt(Float_t& cut) {

  Int_t n2=0;
  Float_t ptmax = -10.;

  for(Int_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if(bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(!PassMuonQual(imu, 0)) continue;
    if( pt >= ptmax ){
      ptmax = pt;
    }
    if(pt >= 0.) n2++;
  }

  if(n2>=2) {
    Float_t tmp_cut = -10.;
    SingleMuPt(tmp_cut,false, 1);
    cut = tmp_cut;
  }
  else cut = -10.;

  return;
}

void L1AlgoFactory::Onia2015Pt(Float_t& ptcut1, Float_t& ptcut2, Bool_t isER, Bool_t isOS, Int_t delta, float Onia2015ER_) {

  Int_t Nmu = upgrade_->nMuons;
  if(Nmu < 2) return;
  if (Onia2015ER_== -1) Onia2015ER_ = Onia2015ER;

  Float_t maxpt1 = -10.;
  Float_t maxpt2 = -10.;
  Float_t corr = false;

  std::vector<std::pair<Float_t,Float_t> > muonPairs;

  for (Int_t imu=0; imu < Nmu; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);
    if (bx != 0) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(!PassMuonQual(imu, 1)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(isER && fabs(eta) > Onia2015ER_) continue;
    Int_t charge1 = upgrade_->muonChg.at(imu);

    for (Int_t imu2=imu+1; imu2 < Nmu; imu2++) {
      if (imu2 == imu) continue;
      Int_t bx2 = upgrade_->muonBx.at(imu2);
      if (bx2 != 0) continue;
      Float_t pt2 = upgrade_->muonEt.at(imu2);			
      if(!PassMuonQual(imu2,1)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t eta2 = upgrade_->muonEtaAtVtx.at(imu2);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t eta2 = upgrade_->muonEta.at(imu2);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      if(isER && fabs(eta2) > Onia2015ER_) continue;
      Int_t charge2 = upgrade_->muonChg.at(imu2);

      if(isOS && charge1*charge2 > 0) continue;

      Float_t deta = eta - eta2; 
      //Assuming ieta corresponds to 0.1 eta, which is true for barrel, but not for endcap
      if(fabs(deta) <= static_cast<float>(delta)/10.){  
        corr = true;
        if (pt >= pt2)
          muonPairs.push_back(std::pair<Float_t,Float_t>(pt,pt2));
        else
          muonPairs.push_back(std::pair<Float_t,Float_t>(pt2,pt));
      }
    }
  }

  if(corr){
    std::vector<std::pair<Float_t,Float_t> >::const_iterator muonPairIt  = muonPairs.begin();
    std::vector<std::pair<Float_t,Float_t> >::const_iterator muonPairEnd = muonPairs.end();
    for(; muonPairIt != muonPairEnd; ++muonPairIt){
      Float_t pt1 = muonPairIt->first;
      Float_t pt2 = muonPairIt->second;

      if(pt1 > maxpt1 || (fabs(maxpt1-pt1)<10E-2 && pt2>maxpt2) ) 
      {
        maxpt1 = pt1;
        maxpt2 = pt2;
      }
    }
  }

  if(corr && maxpt2 >= 0.){
    ptcut1 = maxpt1;
    ptcut2 = maxpt2;
  }

  return;
}

Bool_t L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1(Float_t jetcut, Float_t egcut, int MuQual) {
  Float_t tmp_jetcut = -10.;
  Float_t tmp_egcut = -10.;
  Jet_MuOpen_EG_dPhiMuEG1Pt(tmp_jetcut,tmp_egcut, MuQual);
  if(tmp_jetcut >= jetcut && tmp_egcut >= egcut) return true;
  return false;
}

void L1AlgoFactory::Jet_MuOpen_EG_dPhiMuEG1Pt(Float_t& jetcut, Float_t& egcut, int MuQual){

  //Find the highest pt jet with deltaphi condition with MuOpen
  Float_t jetptmax = -10.;
  Int_t Nj = upgrade_->nJets;
  Int_t Nmu = upgrade_->nMuons;

  for(Int_t ue=0; ue < Nj; ue++){
    Int_t bxjet = upgrade_->jetBx.at(ue);        		
    if(bxjet != 0) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt < jetptmax) continue;
    Float_t phijet = upgrade_->jetPhi.at(ue);

    Bool_t corr = false;

    //Loop over all muons to check if a MuOpen is within 2 calo phi bins
    for(Int_t imu=0; imu < Nmu; imu++){
      Int_t bx = upgrade_->muonBx.at(imu);
      if (bx != 0) continue;
      if(!PassMuonQual(imu, MuQual)) continue;
      Float_t muonpt = upgrade_->muonEt.at(imu);			
      if(muonpt < 0.) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t muphi = upgrade_->muonPhiAtVtx.at(imu);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t muphi = upgrade_->muonPhi.at(imu);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      if(fabs(muphi-phijet) < MuOpenJetCordPhi) corr = true;
    }

    if(corr) jetptmax = pt;
  }

  //Loop over electrons, and save all the electrons which satisfy the deltaphi veto with a MuOpen
  Int_t Nele = upgrade_->nEGs;
  Float_t maxptEG = -10.;
  for(Int_t ue=0; ue < Nele; ue++){     
    Int_t bxele = upgrade_->egBx.at(ue);  
    if(bxele != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);   
    Float_t EGphi = upgrade_->egPhi.at(ue);   
    if(pt < maxptEG) continue;

    //Check the deltaphi veto with any muon
    Bool_t corr = false;
    for(Int_t imu=0; imu < Nmu; imu++) {
      Int_t bxmu = upgrade_->muonBx.at(imu);
      if(bxmu != 0) continue;
      if(!PassMuonQual(imu, MuQual)) continue;
      Float_t ptmu = upgrade_->muonEt.at(imu);			
      if(ptmu < 0.) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t muphi = upgrade_->muonPhiAtVtx.at(imu);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t muphi = upgrade_->muonPhi.at(imu);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----

      if(fabs(muphi-EGphi) > MuMudPhi) corr = true;
    }

    if(corr) maxptEG = pt;
  }

  if(jetptmax > 0. && maxptEG > 0.){
    jetcut = jetptmax;
    egcut = maxptEG;
  }

  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::MultiEGMass
//  Description:  
// ===========================================================================
bool L1AlgoFactory::MultiEGMass(int pt1, int pt2, int pt3, int pt4, int Mcut, bool isIsolated, bool isER) const
{
  
  std::vector<TLorentzVector> vEG;
  const float EMass = 0.510998 / 1000; // MeV / 1000 to GeV

  for(UInt_t ue=0; ue < upgrade_->nEGs; ue++) {               
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    if(isIsolated && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER && isER) continue;  // eta = 5 - 16

    Float_t pt = upgrade_->egEt.at(ue);
    if (pt < pt4) continue;
    TLorentzVector temp(0,0,0,0);
    temp.SetPtEtaPhiM(pt,eta, upgrade_->egPhi.at(ue), EMass);
    vEG.push_back(temp);
  }

  std::sort(vEG.begin(), vEG.end(), 
      [](const TLorentzVector& a, const TLorentzVector& b ) -> bool
      { return a.Pt() > b.Pt(); }
      );

  //for(auto aj : vEG)
  //{
    //std::cout << aj.Pt() <<" : ";
  //}
  //std::cout << "" << std::endl;

  // Check each pt cut
  if (pt1 > 0 && vEG.size() > 0)
  {
    if (vEG.at(0).Pt() < pt1)
      return false;
  }
  if (pt2 > 0 && vEG.size() > 1)
  {
    if (vEG.at(1).Pt() < pt2)
      return false;
  }
  if (pt3 > 0 && vEG.size() > 2)
  {
    if (vEG.at(2).Pt() < pt3)
      return false;
  }
  if (pt4 > 0 && vEG.size() > 3)
  {
    if (vEG.at(3).Pt() < pt4)
      return false;
  }

  std::vector<int> Ptcuts = {pt1, pt2, pt3, pt4};
  unsigned int nEG = 0;
  nEG = std::count_if(Ptcuts.begin(), Ptcuts.end(), [](int i){return i > 0;}); 
  //nEG = vEG.size();
  if (nEG > vEG.size()) return false;

  for(unsigned int i=0; i < nEG; ++i)
  {
    TLorentzVector eg1 = vEG.at(i);
    for(unsigned int j=i+1; j < nEG; ++j)
    {
      TLorentzVector eg2 = vEG.at(j);
      TLorentzVector sum = eg1 + eg2;
      if (sum.M() >= Mcut)
      {
        return true;
      }

    }
  }
  return false;
}       // -----  end of function L1AlgoFactory::MultiEGMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::SetMuonER
//  Description:  
// ===========================================================================
void L1AlgoFactory::SetMuonER(float newER)
{
  muonER  = newER;
}       // -----  end of function L1AlgoFactory::SetMuonER  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::SetUserUpgradeLyr1
//  Description:  /* cursor */
// ===========================================================================
void L1AlgoFactory::SetUseUpgradeLyr1(bool option)
{
  UseUpgradeLyr1 = option;
}       // -----  end of function L1AlgoFactory::SetUserUpgradeLyr1  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::SetUseL1CaloTower
//  Description:  /* cursor */
// ===========================================================================
void L1AlgoFactory::SetUseL1CaloTower(bool option) 
{
  UseL1CaloTower = option;
}       // -----  end of function L1AlgoFactory::SetUseL1CaloTower  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::ETM_Jet
//  Description:  
// ===========================================================================
bool L1AlgoFactory::ETM_Jet(float ETMcut, float jetcut, bool isCent)
{
  Float_t tmp_ETMcut = -10.;
  Float_t tmp_jetcut = -10.;
  ETM_JetPt(tmp_ETMcut,tmp_jetcut, isCent);

  if(tmp_jetcut >= jetcut && tmp_ETMcut >= ETMcut) return true;

  return false;
}       // -----  end of function L1AlgoFactory::ETM_Jet  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::ETM_JetPt
//  Description:  
// ===========================================================================
void L1AlgoFactory::ETM_JetPt(float& ETMcut, float& jetcut, const bool& isCent) 
{
  Float_t TheETM = -10;
  Float_t ETMPhi = -10;
  int idx = GetSumEtIdx(EtSumType::ETM);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==0) 
  {
    TheETM =upgrade_->sumEt.at(idx);
    ETMPhi =upgrade_->sumPhi.at(idx);
  }

  Float_t ptmax = -10.;
  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;

    Bool_t isFwdJet = fabs(upgrade_->jetEta.at(ue)) > jetCentFwd ? true : false;
    if(isCent && isFwdJet) continue;

    float phi  =upgrade_->jetPhi.at(ue);
    if (Phi_mpi_pi(phi - ETMPhi) < 0.4) continue;

    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= ptmax) ptmax = pt;
  }

  ETMcut = TheETM;
  jetcut = ptmax;
}       // -----  end of function L1AlgoFactory::ETM_JetPt  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::HTM_HTT
//  Description:  
// ===========================================================================
bool L1AlgoFactory::HTM_HTT(float HTMcut, float HTTcut)
{
  Float_t tmp_HTMcut = -10.;
  Float_t tmp_HTTcut = -10.;
  HTM_HTTPt(tmp_HTMcut,tmp_HTTcut);
  if(tmp_HTMcut >= HTMcut && tmp_HTTcut >= HTTcut) return true;
  return false;
}       // -----  end of function L1AlgoFactory::HTM_HTT  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::HTM_HTTPt
//  Description:  
// ===========================================================================
void L1AlgoFactory::HTM_HTTPt(float &HTMcut, float &HTTcut)
{
  Float_t TheHTT = -10;
  int idx= GetSumEtIdx(EtSumType::HTT);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==SelBx) 
    TheHTT =upgrade_->sumEt.at(idx);



  Float_t TheHTM = -10;
  idx= GetSumEtIdx(EtSumType::HTM);
  if (idx == -1) return;
  if(upgrade_->sumBx.at(idx)==SelBx) 
    TheHTM =upgrade_->sumEt.at(idx);

  HTMcut = TheHTM;
  HTTcut = TheHTT;

}       // -----  end of function L1AlgoFactory::HTM_HTTPt  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::Mu_Jet
//  Description:  
// ===========================================================================
bool L1AlgoFactory::Mu_Jet(float mucut, float jetcut, bool isMuER, float JetCent)
{
  Float_t tmp_mucut = -10.;
  Float_t tmp_jetcut = -10.;
  Mu_JetPt(tmp_mucut,tmp_jetcut, isMuER, JetCent);
  if(tmp_mucut >= mucut && tmp_jetcut >= jetcut) return true;
  return false;
}       // -----  end of function L1AlgoFactory::Mu_Jet  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::Mu_JetPt
//  Description:  
// ===========================================================================
void L1AlgoFactory::Mu_JetPt(float &mucut, float &jetcut, const bool isMuER, const float JetCent)
{
  Float_t muptmax = -10.;
  Float_t jetptmax = -10.;
  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 2)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muonER && isMuER) continue;
    Float_t pt = upgrade_->muonEt.at(imu);
    if(pt >= muptmax) muptmax = pt;
  }

  for(UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    if(JetCent != 999  && fabs(upgrade_->jetEta.at(ue)) > JetCent) continue;
    Float_t pt = upgrade_->jetEt.at(ue);
    if(pt >= jetptmax) jetptmax = pt;
  }
  mucut = muptmax;
  jetcut = jetptmax;

}       // -----  end of function L1AlgoFactory::Mu_JetPt  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DiJetMass
//  Description:  
// ===========================================================================
Float_t L1AlgoFactory::DiJetMass(Float_t Mj1, Float_t Mj2, Bool_t isCentral) const
{
  TLorentzVector jet1(0,0,0,0);
  TLorentzVector jet2(0,0,0,0);
  float maxMass = 0;

  if(upgrade_->nJets < 2) return maxMass;


  for(UInt_t ue1=0; ue1 < upgrade_->nJets; ue1++) {
    Int_t bx1 = upgrade_->jetBx.at(ue1);        		
    if(bx1 != 0) continue;
    Bool_t isFwdJet1 = fabs(upgrade_->jetEta.at(ue1)) > jetCentFwd ? true : false;
    if(isCentral && isFwdJet1) continue;
    Float_t pt1 = upgrade_->jetEt.at(ue1);
    if (pt1 < Mj1) continue;
    jet1.SetPtEtaPhiM(pt1,upgrade_->jetEta.at(ue1), upgrade_->jetPhi.at(ue1), 0);

    for(UInt_t ue2=ue1+1; ue2 < upgrade_->nJets; ue2++) {
      Int_t bx2 = upgrade_->jetBx.at(ue2);        		
      if(bx2 != 0) continue;
      Bool_t isFwdJet2 = fabs(upgrade_->jetEta.at(ue2)) > jetCentFwd ? true : false;
      if(isCentral && isFwdJet2) continue;
      Float_t pt2 = upgrade_->jetEt.at(ue2);
      if (pt2 < Mj2) continue;
      jet2.SetPtEtaPhiM(pt2,upgrade_->jetEta.at(ue2), upgrade_->jetPhi.at(ue2), 0);

      TLorentzVector dijMass = jet1 + jet2;
      if (dijMass.M() > maxMass)
      {
        maxMass = dijMass.M();
      }
    }
  }

  return maxMass;
}       // -----  end of function L1AlgoFactory::DiJetMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJetMass
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJetMass(Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
    Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut)
{
  bool passDiJetCut = false;
  bool passDiJetMass = false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting DoubleJetPt ~~~~~
  Float_t tmp_cut1 = -10.;
  Float_t tmp_cut2 = -10.;
  DoubleJetPt(tmp_cut1,tmp_cut2,iscutCentral);

  if(tmp_cut1 >= cut1 && tmp_cut2 >= cut2) 
    passDiJetCut = true;
  else
    return false;
  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting DiJetMass ~~~~~
  float diJetMass = DiJetMass(Mj1, Mj2, isMjCentral);

  if (diJetMass >= Masscut)
    passDiJetMass = true;
  else
    return false;

  return passDiJetCut && passDiJetMass;
}       // -----  end of function L1AlgoFactory::DoubleJetMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJetMass_Mu
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJetMass_Mu( Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
    Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut, 
    Float_t Muptcut, Bool_t isMuER, Int_t Muqualmin)
{

  bool passDiJet =  false;
  bool passSingleMu = false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting DiJet ~~~~~
  passDiJet = DoubleJetMass(cut1, cut2, iscutCentral, Mj1, Mj2, isMjCentral, Masscut);
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting SingleMu ~~~~~
  Float_t tmp_cut = -10.;
  SingleMuPt(tmp_cut,isMuER, Muqualmin);
  if(tmp_cut >= Muptcut) passSingleMu = true;

  return passDiJet && passSingleMu;
}       // -----  end of function L1AlgoFactory::DoubleJetMass_Mu  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJetMass_EG
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJetMass_EG( Float_t cut1, Float_t cut2, Bool_t iscutCentral, 
    Float_t Mj1, Float_t Mj2, Bool_t isMjCentral, Float_t Masscut, 
    Float_t EGptcut, Bool_t isEGIsolated, Bool_t isEGER)
{
  
  bool passDiJet =  false;
  bool passSingleEG = false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting DiJet ~~~~~
  passDiJet = DoubleJetMass(cut1, cut2, iscutCentral, Mj1, Mj2, isMjCentral, Masscut);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting SingleEG ~~~~~
  Float_t tmp_cut1 = -10.;
  SingleEGPt(tmp_cut1,isEGIsolated,isEGER);
  if(tmp_cut1 >= EGptcut) passSingleEG = true;

  return passDiJet && passSingleEG;
}       // -----  end of function L1AlgoFactory::DoubleJetMass_EG  -----



//**************************************************************************//
//                          2017 L1 Menu Proposals                          //
//**************************************************************************//
Bool_t L1AlgoFactory::EG_HTT(Float_t egcut, Float_t HTcut, bool isEGER, bool isEGIso) {
  Float_t tmp_egcut = -10.;
  Float_t tmp_HTcut = -10.;
  EG_HTTPt(tmp_egcut,tmp_HTcut,isEGER, isEGIso );
  if(tmp_egcut >= egcut && tmp_HTcut >= HTcut) return true;
  return false;
}

void L1AlgoFactory::EG_HTTPt(Float_t& egcut, Float_t& HTcut, bool &isEGER, bool &isEGIso)  {

  Float_t egptmax = -10.;

  for(UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    if(isEGIso && (upgrade_->egIso.at(ue) & EGIsobit ) == 0) continue;
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER && isEGER) continue;  // eta = 5 - 16

    Float_t pt = upgrade_->egEt.at(ue);    // the rank of the electron
    if(pt >= egptmax) egptmax = pt;
  }

  Float_t TheHTT = -10;
  HTTVal(TheHTT);

  if(egptmax >= 0.){
    egcut = egptmax;
    HTcut = TheHTT;
  }

  return;
}


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::HTT_QuadJet
//  Description:  
// ===========================================================================
bool L1AlgoFactory::HTT_QuadJet(Float_t HTTcut, Float_t Jet1cut, Float_t Jet2cut, Float_t Jet3cut, Float_t Jet4cut, Float_t JetEtaRestr)
{
  Float_t tmp_HTTcut = -10.;
  Float_t tmp_Jet1cut = -10.;
  Float_t tmp_Jet2cut = -10.;
  Float_t tmp_Jet3cut = -10.;
  Float_t tmp_Jet4cut = -10.;

  HTT_QuadJetPt(tmp_HTTcut, tmp_Jet1cut, tmp_Jet2cut, tmp_Jet3cut, tmp_Jet4cut, JetEtaRestr);

  if ( tmp_HTTcut >= HTTcut &&
      tmp_Jet1cut >= Jet1cut &&
      tmp_Jet2cut >= Jet2cut &&
      tmp_Jet3cut >= Jet3cut &&
      tmp_Jet4cut >= Jet4cut )
    return true;
  return false;
}       // -----  end of function L1AlgoFactory::HTT_QuadJet  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::HTT_QuadJetPt
//  Description:  
// ===========================================================================
void L1AlgoFactory::HTT_QuadJetPt(Float_t &HTTcut, Float_t &Jet1cut, Float_t &Jet2cut, Float_t &Jet3cut, Float_t &Jet4cut, Float_t &JetEtaRestr)
{
  Float_t TheHTT = -10;
  Float_t jet1ptmax = -10.;
  Float_t jet2ptmax = -10.;
  Float_t jet3ptmax = -10.;
  Float_t jet4ptmax = -10.;


  if(upgrade_->nJets < 4) return;
  HTTVal(TheHTT);

  for (UInt_t ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Float_t eta = upgrade_->jetEta.at(ue);
    if (fabs(eta) > JetEtaRestr) continue;
    Float_t pt = upgrade_->jetEt.at(ue);

    if(pt >= jet1ptmax)
    {
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = jet1ptmax;
      jet1ptmax = pt;
    }
    else if(pt >= jet2ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = jet2ptmax;
      jet2ptmax = pt;
    }
    else if(pt >= jet3ptmax){
      jet4ptmax = jet3ptmax;
      jet3ptmax = pt;
    }
    else if(pt >= jet4ptmax) jet4ptmax = pt;
  }

  if(jet4ptmax >= 0.){
    Jet1cut = jet1ptmax;
    Jet2cut = jet2ptmax;
    Jet3cut = jet3ptmax;
    Jet4cut = jet4ptmax;
    HTTcut = TheHTT;
  }

  return;
}       // -----  end of function L1AlgoFactory::HTT_QuadJetPt  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::Mu_DoubleJet_Cor
//  Description:  /* cursor */
// ===========================================================================
bool L1AlgoFactory::Mu_DoubleJet_Cor(Float_t mupt, Float_t diJetpt, Float_t EtaRestrict, Float_t MuJetdRMax, Float_t diJetdEtaMax)
{
  if(upgrade_->nJets < 2) return false;
  std::vector<unsigned> Muidx;
  std::vector<std::pair<unsigned, unsigned> > JetPairs;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Get list of muon ~~~~~
  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, 2)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if (EtaRestrict != 0 && fabs(eta) > EtaRestrict) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if (pt >= mupt) 
    {
      Muidx.push_back(imu);
    }
  }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Get Jet pairs ~~~~~
  for (UInt_t ui=0; ui < upgrade_->nJets; ui++) {
    Int_t bx1 = upgrade_->jetBx.at(ui);        		
    if(bx1 != 0) continue;
    float  eta1 = upgrade_->jetEta.at(ui);
    if (EtaRestrict != 0 && fabs(eta1) > EtaRestrict) continue;
    Float_t pt1 = upgrade_->jetEt.at(ui);
    if(pt1 < diJetpt ) continue;

    for (UInt_t uj=ui+1; uj < upgrade_->nJets; uj++) {
      Int_t bx2 = upgrade_->jetBx.at(uj);        		
      if(bx2 != 0) continue;
      float  eta2 = upgrade_->jetEta.at(uj);
      if (EtaRestrict != 0 && fabs(eta2) > EtaRestrict) continue;
      Float_t pt2 = upgrade_->jetEt.at(uj);
      if(pt2 < diJetpt ) continue;

      if(correlateInEta(eta1, eta2, diJetdEtaMax)){
        JetPairs.push_back(std::make_pair(ui,uj));
      }
    }

  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Muon and Jet ~~~~~
  for(auto m : Muidx)
  {
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t Mueta = upgrade_->muonEtaAtVtx.at(m);			
    Float_t Muphi = upgrade_->muonPhiAtVtx.at(m);			
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t Mueta = upgrade_->muonEta.at(m);			
    Float_t Muphi = upgrade_->muonPhi.at(m);			
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    for(auto jets : JetPairs)
    {
      Float_t Jeteta = upgrade_->jetEta.at(jets.first);
      Float_t Jetphi = upgrade_->jetPhi.at(jets.first);
      if (!correlatedRMax(Mueta, Jeteta, Muphi, Jetphi, MuJetdRMax))
        return true;
      Jeteta = upgrade_->jetEta.at(jets.second);
      Jetphi = upgrade_->jetPhi.at(jets.second);
      if (!correlatedRMax(Mueta, Jeteta, Muphi, Jetphi, MuJetdRMax))
        return true;
    }
  }

  return false;
}       // -----  end of function L1AlgoFactory::Mu_DoubleJet_Cor  -----



Bool_t L1AlgoFactory::Tau_ETM(Float_t taucut, Float_t ETMcut, Bool_t isTauER, Bool_t isTauIso, Bool_t isETMHF) {
  Float_t tmp_taucut = -10.;
  Float_t tmp_ETMcut = -10.;
  Tau_ETMPt(tmp_taucut,tmp_ETMcut, isTauER, isTauIso, isETMHF);
  if(tmp_taucut >= taucut && tmp_ETMcut >= ETMcut) return true;
  return false;
}

void L1AlgoFactory::Tau_ETMPt(Float_t& taucut, Float_t& ETMcut, Bool_t isTauER, Bool_t isTauIso, Bool_t isETMHF) {

  Float_t tauptmax = -10.;

  for(UInt_t ue=0; ue < upgrade_->nTaus; ue++) {
    Int_t bx = upgrade_->tauBx.at(ue);        		
    if(bx != 0) continue;
    if(isTauER && fabs(upgrade_->tauEta.at(ue)) > tauER) continue;
    if(isTauIso && !upgrade_->tauIso.at(ue)) continue;
    Float_t pt = upgrade_->tauEt.at(ue);
    if(pt >= tauptmax) tauptmax = pt;
  }

  Float_t TheETM = -10;
  if (isETMHF)
    ETMHFVal(TheETM);
  else
    ETMVal(TheETM);

  if(tauptmax >= 0.){
    taucut = tauptmax;
    ETMcut = TheETM;
  }
  return;
}

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleMuMass
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleMuMass(float mu1cut, float mu2cut, float muER, int Qual, bool isOS, float Mlow, float Mhigh)
{

  std::vector<std::pair<unsigned, unsigned> > muonPairs;
  std::vector<std::pair<unsigned, unsigned> > muonMassPairs;

  if(upgrade_->nMuons < 2) return false;

  for (UInt_t imu1=0; imu1 < upgrade_->nMuons; imu1++) {
    Int_t bx1 = upgrade_->muonBx.at(imu1);
    if(bx1 != 0) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta1 = upgrade_->muonEtaAtVtx.at(imu1);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta1 = upgrade_->muonEta.at(imu1);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    Int_t charge1 = upgrade_->muonChg.at(imu1);
    if(muER != 999 && fabs(eta1) > muER) continue;
    if(!PassMuonQual(imu1, Qual)) continue;


    for (UInt_t imu2=imu1+1; imu2 < upgrade_->nMuons; imu2++) {
      Int_t bx2 = upgrade_->muonBx.at(imu2);
      if(bx2 != 0) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta2 = upgrade_->muonEtaAtVtx.at(imu2);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta2 = upgrade_->muonEta.at(imu2);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      Int_t charge2 = upgrade_->muonChg.at(imu2);
      if(muER != 999 && fabs(eta2) > muER) continue;
      if(!PassMuonQual(imu2, Qual)) continue;

      if(isOS && charge1*charge2 > 0) continue;
      muonPairs.push_back(std::make_pair(imu1, imu2));
    }
  }

  if (Mlow == 999 && Mhigh == 999)
  {
    muonMassPairs = muonPairs;
  }
  else{
    TLorentzVector mu1(0 , 0 , 0 , 0);
    TLorentzVector mu2(0 , 0 , 0 , 0);
    for(auto mp : muonPairs)
    {

#ifdef  L1NTUPLE_MUONCORATVTX
      mu1.SetPtEtaPhiM( upgrade_->muonEt.at(mp.first),
          upgrade_->muonEtaAtVtx.at(mp.first),
          upgrade_->muonPhiAtVtx.at(mp.first),0);
      mu2.SetPtEtaPhiM( upgrade_->muonEt.at(mp.second),
          upgrade_->muonEtaAtVtx.at(mp.second),
          upgrade_->muonPhiAtVtx.at(mp.second),0);
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      mu1.SetPtEtaPhiM( upgrade_->muonEt.at(mp.first),
          upgrade_->muonEta.at(mp.first),
          upgrade_->muonPhi.at(mp.first),0);
      mu2.SetPtEtaPhiM( upgrade_->muonEt.at(mp.second),
          upgrade_->muonEta.at(mp.second),
          upgrade_->muonPhi.at(mp.second),0);
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      TLorentzVector duMass = mu1 + mu2;
      if (Mlow != 999 && duMass.M() >= Mlow)
      {
        if (Mhigh == 999 || duMass.M() <= Mhigh )
        {
          muonMassPairs.push_back(mp);
        }
      }
    }
  }

  
  for(auto mp : muonMassPairs)
  {
    Float_t pt1 = max(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));
    Float_t pt2 = min(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));

    if (pt1 >= mu1cut && pt2 >= mu2cut)
    {
      return true;
    }
  }
  return false;
}       // -----  end of function L1AlgoFactory::DoubleMuMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::TripleEGIso
//  Description:  
// ===========================================================================
bool L1AlgoFactory::TripleEGIso(Float_t cut1, Float_t cut2, Float_t cut3, bool isIso1, bool isIso2, bool isIso3)
{
  if(upgrade_->nEGs < 2) return false;

  std::map<Float_t, int> EGPtIdx;
  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    EGPtIdx[pt] = ue;
  }

  if (EGPtIdx.size() < 3) return false;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ First EG ~~~~~
  std::map<Float_t, int>::reverse_iterator it=EGPtIdx.rbegin();
  if (upgrade_->egEt.at(it->second) < cut1) return false;
  if(isIso1 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) return false;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Second EG ~~~~~
  it++;
  if (upgrade_->egEt.at(it->second) < cut2) return false;
  if(isIso2 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) return false;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Third EG ~~~~~
  it++;
  if (upgrade_->egEt.at(it->second) < cut3) return false;
  if(isIso3 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) return false;

  return true;
}       // -----  end of function L1AlgoFactory::TripleEGIso  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleMu_ETMHF_Jets
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleMu_ETMHF_Jets(
    const float mu1pt, 
    const float mu2pt, 
    const int muqual, 
    const bool ismuER, 
    const float etmcut,
    const bool isETMHF,
    const float singleJetpt,
    const float diJetpt)
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gettting the doubleMuon ~~~~~
  float tmp_mu1pt = -10;
  float tmp_mu2pt = -10;
  DoubleMuPt(tmp_mu1pt, tmp_mu2pt, muqual, ismuER);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the ETM ~~~~~
  float tmp_ETMpt = -10;
  if (isETMHF)
    ETMHFVal(tmp_ETMpt);
  else
    ETMVal(tmp_ETMpt);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the jets ~~~~~
  float tmp_diJet1pt = -10;
  float tmp_diJet2pt = -10;
  DoubleJetPt(tmp_diJet1pt, tmp_diJet2pt, false);
  float tmp_SingleJetpt = -10;
  SingleJetPt(tmp_SingleJetpt, false);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the logic ~~~~~
  if (tmp_mu1pt >= mu1pt && tmp_mu2pt >= mu2pt)
  {
    if (tmp_ETMpt>= etmcut)
    {
      if (( singleJetpt <= 0 || tmp_SingleJetpt >= singleJetpt )
          || ( diJetpt <= 0 || (tmp_diJet1pt >= diJetpt && tmp_diJet2pt >= diJetpt ) ))
      {
        return true;
      }
    }
  }

  return false;
}       // -----  end of function L1AlgoFactory::DoubleMu_ETMHF_Jets  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleMu_HTT
//  Description:  /* cursor */
// ===========================================================================
bool L1AlgoFactory::DoubleMu_HTT( Float_t cut1, Float_t cut2, int muqual, Bool_t isER, Float_t HTTcut) 
{
  Float_t muptmax = -10.;
  Float_t second_muptmax = -10.;
  Float_t TheHTT = -10.;

  if(upgrade_->nMuons < 2) return false;

  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, muqual)) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta = upgrade_->muonEta.at(imu);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(isER && fabs(eta) > muonER) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    if(pt >= muptmax){
      second_muptmax = muptmax;
      muptmax = pt;
    }
    else if(pt >= second_muptmax) second_muptmax = pt;
  }

  HTTVal(TheHTT);

  if(second_muptmax >= 0.){
    if (muptmax >= cut1 && second_muptmax >= cut2 && TheHTT >= HTTcut)
    {
      return true;
    }
  }
  return false;
}       // -----  end of function L1AlgoFactory::DoubleMu_HTT  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::ETM_JetsComb
//  Description:  
// ===========================================================================
bool L1AlgoFactory::ETM_JetsComb(
    const float etmcut,
    const bool isETMHF,
    const float singleJetpt,
    const float diJetpt,
    const float triJetpt,
    const bool skipJetEta)
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the ETM ~~~~~
  float tmp_ETMpt = -10;
  if (isETMHF)
    ETMHFVal(tmp_ETMpt);
  else
    ETMVal(tmp_ETMpt);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting the jets ~~~~~
  float tmp_triJet1pt = -10;
  float tmp_triJet2pt = -10;
  float tmp_triJet3pt = -10;
  TripleJetPt(tmp_triJet1pt, tmp_triJet2pt, tmp_triJet3pt, false);
  float tmp_diJet1pt = -10;
  float tmp_diJet2pt = -10;
  DoubleJetPt(tmp_diJet1pt, tmp_diJet2pt, false);
  float tmp_SingleJetpt = -10;
  SingleJetPt(tmp_SingleJetpt, false);


  if (tmp_ETMpt >= etmcut)
  {
    bool fire = false;

    if (singleJetpt > 0 && (tmp_SingleJetpt >= singleJetpt ))
      fire = true;
    if ( diJetpt  > 0  && (tmp_diJet1pt >= diJetpt && tmp_diJet2pt >= diJetpt ) )
      fire = true;
    if (triJetpt > 0 && (tmp_triJet1pt >= triJetpt && tmp_triJet2pt >= triJetpt && tmp_triJet3pt >= triJetpt ) )
      fire = true;
    if (fire)
      return true;
  }

  return false;
}       // -----  end of function L1AlgoFactory::ETM_JetsComb  -----



// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleMudRMax
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleMudRMax(float mu1cut, float mu2cut, float muER, int Qual, bool isOS, float dRMax)
{

  std::vector<std::pair<unsigned, unsigned> > muonPairs;
  std::vector<std::pair<unsigned, unsigned> > muonMassPairs;

  if(upgrade_->nMuons < 2) return false;

  for (UInt_t imu1=0; imu1 < upgrade_->nMuons; imu1++) {
    Int_t bx1 = upgrade_->muonBx.at(imu1);
    if(bx1 != 0) continue;
    //Float_t pt1 = upgrade_->muonEt.at(imu1);			
#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t eta1 = upgrade_->muonEtaAtVtx.at(imu1);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t eta1 = upgrade_->muonEta.at(imu1);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    Int_t charge1 = upgrade_->muonChg.at(imu1);
    if(muER != 999 && fabs(eta1) > muER) continue;
    if(!PassMuonQual(imu1, Qual)) continue;


    for (UInt_t imu2=imu1+1; imu2 < upgrade_->nMuons; imu2++) {
      Int_t bx2 = upgrade_->muonBx.at(imu2);
      if(bx2 != 0) continue;
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t eta2 = upgrade_->muonEtaAtVtx.at(imu2);        
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t eta2 = upgrade_->muonEta.at(imu2);        
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      Int_t charge2 = upgrade_->muonChg.at(imu2);
      if(muER != 999 && fabs(eta2) > muER) continue;
      if(!PassMuonQual(imu2, Qual)) continue;

      if(isOS && charge1*charge2 > 0) continue;
      muonPairs.push_back(std::make_pair(imu1, imu2));
    }
  }

  if (dRMax == 0)
  {
    muonMassPairs = muonPairs;
  }
  else{
    for(auto mp : muonPairs)
    {
#ifdef  L1NTUPLE_MUONCORATVTX
      if (!correlatedRMax ( upgrade_->muonEtaAtVtx.at(mp.first),
            upgrade_->muonEtaAtVtx.at(mp.second),
            upgrade_->muonPhiAtVtx.at(mp.first),
            upgrade_->muonPhiAtVtx.at(mp.second),
            dRMax))
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      if (!correlatedRMax ( upgrade_->muonEta.at(mp.first),
            upgrade_->muonEta.at(mp.second),
            upgrade_->muonPhi.at(mp.first),
            upgrade_->muonPhi.at(mp.second),
            dRMax))
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      {
          muonMassPairs.push_back(mp);
      }
    }
  }

  
  for(auto mp : muonMassPairs)
  {
    Float_t pt1 = max(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));
    Float_t pt2 = min(upgrade_->muonEt.at(mp.first), upgrade_->muonEt.at(mp.second));

    if (pt1 >= mu1cut && pt2 >= mu2cut)
    {
      return true;
    }
  }
  return false;
}       // -----  end of function L1AlgoFactory::DoubleMudRMax  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::TripleMu_DoubleMuMass
//  Description:  // for L1_TripleMu_5_0_0_DoubleMu_5_0_OS_M0to17;
// ===========================================================================
bool L1AlgoFactory::TripleMu_DoubleMuMass(
    const float triMupt1,
    const float triMupt2,
    const float triMupt3,
    const int triMuQual,
    const float diMupt1,
    const float diMupt2,
    const bool diMuOS,
    const float diMuMmin,
    const float diMuMmax)
{

  if(upgrade_->nMuons < 3) return false;

  std::map<Float_t, int> MuPtIdx;
  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, triMuQual)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    MuPtIdx[pt] = imu;
  }
  if (MuPtIdx.size()< 3) return false;

  UInt_t mu1idx = -10;
  UInt_t mu2idx = -10;
  UInt_t mu3idx = -10;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ First Mu ~~~~~
  std::map<Float_t, int>::reverse_iterator it=MuPtIdx.rbegin();
  if (upgrade_->muonEt.at(it->second) < triMupt1) return false;
  mu1idx = it->second;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Second EG ~~~~~
  it++;
  if (upgrade_->muonEt.at(it->second) < triMupt2) return false;
  mu2idx = it->second;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Third EG ~~~~~
  it++;
  if (upgrade_->muonEt.at(it->second) < triMupt3) return false;
  mu3idx = it->second;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DiMuon Mass indepedant of the tripleMu ~~~~~
  return DoubleMuMass(diMupt1, diMupt2, 999, triMuQual, diMuOS, diMuMmin, diMuMmax);

//~~~~~~~~~~~~~~~~ This algo takes dimuon mass from the triple muon legs ~~~~~
  std::vector<std::pair<UInt_t, UInt_t> > muonPair;
  muonPair.push_back(std::make_pair(mu1idx, mu2idx));
  muonPair.push_back(std::make_pair(mu1idx, mu3idx));
  std::vector<Float_t> diMuMass;

  for(auto mp : muonPair)
  {
    Float_t pt1 = upgrade_->muonEt.at(mp.first);			
    Float_t pt2 = upgrade_->muonEt.at(mp.second);			
    if (pt1 < diMupt1 || pt2 < diMupt2) continue;

    Int_t charge1 = upgrade_->muonChg.at(mp.first);
    Int_t charge2 = upgrade_->muonChg.at(mp.second);
    if (diMuOS &&  charge1*charge2 > 0) continue;

    TLorentzVector mu1(0 , 0 , 0 , 0);
    TLorentzVector mu2(0 , 0 , 0 , 0);
#ifdef  L1NTUPLE_MUONCORATVTX
      mu1.SetPtEtaPhiM( upgrade_->muonEt.at(mp.first),
          upgrade_->muonEtaAtVtx.at(mp.first),
          upgrade_->muonPhiAtVtx.at(mp.first),0);
      mu2.SetPtEtaPhiM( upgrade_->muonEt.at(mp.second),
          upgrade_->muonEtaAtVtx.at(mp.second),
          upgrade_->muonPhiAtVtx.at(mp.second),0);
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      mu1.SetPtEtaPhiM( upgrade_->muonEt.at(mp.first),
          upgrade_->muonEta.at(mp.first),
          upgrade_->muonPhi.at(mp.first),0);
      mu2.SetPtEtaPhiM( upgrade_->muonEt.at(mp.second),
          upgrade_->muonEta.at(mp.second),
          upgrade_->muonPhi.at(mp.second),0);
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    diMuMass.push_back((mu1+mu2).M());
  }

  if (diMuMass.size() == 0 ) return false;
  
  for(auto m : diMuMass)
  {
    if (m >= diMuMmin && m <= diMuMmax ) return true;
  }

  return false;
}       // -----  end of function L1AlgoFactory::TripleMu_DoubleMuMass  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::MuonCDC_dPhi
//  Description:  
// ===========================================================================
bool L1AlgoFactory::MuonCDC_dPhi(float mupt, int muQual, float muER, float dPhiMin, float dPhiMax)
{
  std::vector<int> topmuidx;
  std::vector<int> botmuidx;

  for(UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    if(!PassMuonQual(imu, muQual)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);                       
    if (pt < mupt) continue;

#ifdef  L1NTUPLE_MUONCORATVTX
    Float_t phi = upgrade_->muonPhiAtVtx.at(imu);
    Float_t eta = upgrade_->muonEtaAtVtx.at(imu);
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
    Float_t phi = upgrade_->muonPhi.at(imu);
    Float_t eta = upgrade_->muonEta.at(imu);
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
    if(fabs(eta) > muER) continue;
    Int_t bx = upgrade_->muonBx.at(imu);
    if (bx == -1 && (phi >= 0.524 && phi <= 2.618))
      topmuidx.push_back(imu);
    if (bx == 0 &&  (phi >= 3.665 && phi <= 5.760))
      botmuidx.push_back(imu);
  }

  for(auto t : topmuidx)
  {
    for(auto b : botmuidx)
    {
#ifdef  L1NTUPLE_MUONCORATVTX
      Float_t topphi = upgrade_->muonPhiAtVtx.at(t);
      Float_t botphi = upgrade_->muonPhiAtVtx.at(b);
#else      // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t topphi = upgrade_->muonPhi.at(t);
      Float_t botphi = upgrade_->muonPhi.at(b);
#endif     // -----  not L1NTUPLE_MUONCORATVTX  -----
      Float_t dPhi = Phi_mpi_pi(topphi - botphi);
      if ( dPhi >= dPhiMin && dPhi <= dPhiMax)
        return true;
    }
  }

  return false;
}       // -----  end of function L1AlgoFactory::MuonCDC_dPhi  -----
//
// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleEGIsoPer
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleEGIsoPer(Float_t cut1, Float_t cut2, Bool_t Iso1, Bool_t Iso2, Bool_t isER)
{
  if(upgrade_->nEGs < 2) return false;
  std::vector<std::pair<UInt_t, UInt_t> > EGPair;

  for(UInt_t ue1=0; ue1 < upgrade_->nEGs; ue1++) {               
    Int_t bx1 = upgrade_->egBx.at(ue1);  
    if(bx1 != 0) continue;
    Float_t pt1 = upgrade_->egEt.at(ue1);
    if (pt1 < cut1 && pt1 < cut2) continue;
    if(fabs(upgrade_->egEta.at(ue1)) > eleER && isER) continue;  // eta = 5 - 16

    for(UInt_t ue2=ue1+1; ue2 < upgrade_->nEGs; ue2++) {               
      Int_t bx2 = upgrade_->egBx.at(ue2);  
      if(bx2 != 0) continue;
      if(fabs(upgrade_->egEta.at(ue2)) > eleER && isER) continue;  // eta = 5 - 16
      Float_t pt2 = upgrade_->egEt.at(ue2);
      if (pt2 < cut1 && pt2 < cut2) continue;
      if (pt1 >= pt2)
        EGPair.push_back(std::make_pair(ue1, ue2));
      else
        EGPair.push_back(std::make_pair(ue2, ue1));

    }
  }

  for(auto p : EGPair)
  {
    if (upgrade_->egEt.at(p.first) < cut1) continue;
    if(Iso1 && (upgrade_->egIso.at(p.first) & EGIsobit ) == 0) continue;
    if (upgrade_->egEt.at(p.second) < cut2) continue;
    if(Iso2 && (upgrade_->egIso.at(p.second) & EGIsobit ) == 0) continue;
    return true;
  }

  return false;
}       // -----  end of function L1AlgoFactory::DoubleEGIsoPer  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::TripleEGIsoPer
//  Description:  
// ===========================================================================
bool L1AlgoFactory::TripleEGIsoPer(Float_t cut1, Float_t cut2, Float_t cut3, bool isIso1, bool isIso2, bool isIso3)
{
  if(upgrade_->nEGs < 2) return false;

  std::vector<int> EGIdx;
  for (UInt_t ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    Float_t pt = upgrade_->egEt.at(ue);
    if (pt < cut3) continue;
    EGIdx.push_back(ue);
  }

  if (EGIdx.size() < 3) return false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ All the combination from Rosettacode ~~~~~
  std::string bitmask(3, 1);
  bitmask.resize(EGIdx.size(), 0);
  do {
    std::map<Float_t, int> EGPtIdx;
    for (unsigned int i = 0; i < EGIdx.size(); ++i) // [0..N-1] integers
    {
      if (bitmask[i]) 
        EGPtIdx[upgrade_->egEt.at(EGIdx.at(i))] = EGIdx.at(i);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ First EG ~~~~~
    std::map<Float_t, int>::reverse_iterator it=EGPtIdx.rbegin();
    if (upgrade_->egEt.at(it->second) < cut1) continue;
    if(isIso1 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) continue;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Second EG ~~~~~
    it++;
    if (upgrade_->egEt.at(it->second) < cut2) continue;
    if(isIso2 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) continue;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Third EG ~~~~~
    it++;
    if (upgrade_->egEt.at(it->second) < cut3) continue;
    if(isIso3 && (upgrade_->egIso.at(it->second) & EGIsobit ) == 0) continue;
    return true;

  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return false;
}       // -----  end of function L1AlgoFactory::TripleEGIsoPer  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::SingleEGMT
//  Description:  
// ===========================================================================
bool L1AlgoFactory::SingleEGMT(Float_t EGcut, Float_t MTcut, Float_t ETMcut, Bool_t isIso, Bool_t isER)
{
  if(upgrade_->nEGs < 1) return false;

  std::vector<UInt_t> EGIdx;
  Float_t ptmax = -10.;
  int leadidx = -10;
  int ETMidx = -10;

  for(int ue=0; ue < upgrade_->nEGs; ue++) {
    Int_t bx = upgrade_->egBx.at(ue);  
    if(bx != 0) continue;
    if(isIso && (upgrade_->egIso.at(ue) & 1 ) == 0) continue;
    Float_t eta = upgrade_->egEta.at(ue);
    if(fabs(eta) > eleER && isER) continue;  // eta = 5 - 16

    Float_t pt = upgrade_->egEt.at(ue);    // the rank of the electron
    //if(pt >= EGcut) 
      //EGIdx.push_back(ue);
    if (pt > ptmax)
    {
      ptmax = pt;
      leadidx = ue;
    }
  }

  ETMidx = GetSumEtIdx(EtSumType::ETM);

  // All the combination
  //for(auto i : EGIdx)
  //{
    //if (correlateMT( upgrade_->egEt.at(i), 
          //upgrade_->sumEt.at(ETMidx),
          //upgrade_->egPhi.at(i), 
          //upgrade_->sumPhi.at(ETMidx),
          //MTcut))
      //continue;
    //else
      //return true;
  //}
  if (ptmax < EGcut) return false;
  if (leadidx == -10 || ETMidx == -10 )
    return false;

  // Leading EG
  if (correlateMT( upgrade_->egEt.at(leadidx), 
        upgrade_->sumEt.at(ETMidx),
        upgrade_->egPhi.at(leadidx), 
        upgrade_->sumPhi.at(ETMidx),
        MTcut))
    return false;
  else
    return true;
}       // -----  end of function L1AlgoFactory::SingleEGMT  -----

// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::TripleMuOS
//  Description:  
// ===========================================================================
bool L1AlgoFactory::TripleMuOS(
    const float triMupt1,
    const float triMupt2,
    const float triMupt3,
    const int triMuQual,
    const float diMupt1,
    const float diMupt2,
    const bool diMuOS) 
{
  if(upgrade_->nMuons < 3) return false;

  std::map<Float_t, int> MuPtIdx;
  for (UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    Int_t bx = upgrade_->muonBx.at(imu);		
    if(bx != 0) continue;
    if(!PassMuonQual(imu, triMuQual)) continue;
    Float_t pt = upgrade_->muonEt.at(imu);			
    MuPtIdx[pt] = imu;
  }
  if (MuPtIdx.size()< 3) return false;

  UInt_t mu1idx = -10;
  UInt_t mu2idx = -10;
  UInt_t mu3idx = -10;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ First Mu ~~~~~
  std::map<Float_t, int>::reverse_iterator it=MuPtIdx.rbegin();
  if (upgrade_->muonEt.at(it->second) < triMupt1) return false;
  mu1idx = it->second;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Second EG ~~~~~
  it++;
  if (upgrade_->muonEt.at(it->second) < triMupt2) return false;
  mu2idx = it->second;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Third EG ~~~~~
  it++;
  if (upgrade_->muonEt.at(it->second) < triMupt3) return false;
  mu3idx = it->second;

//~~~~~~~~~~~~~~~~ This algo takes dimuon mass from the triple muon legs ~~~~~
  std::vector<std::pair<UInt_t, UInt_t> > muonPair;
  muonPair.push_back(std::make_pair(mu1idx, mu2idx));
  muonPair.push_back(std::make_pair(mu1idx, mu3idx));

  for(auto mp : muonPair)
  {
    Float_t pt1 = upgrade_->muonEt.at(mp.first);			
    Float_t pt2 = upgrade_->muonEt.at(mp.second);			
    if (pt1 < diMupt1 || pt2 < diMupt2) continue;

    Int_t charge1 = upgrade_->muonChg.at(mp.first);
    Int_t charge2 = upgrade_->muonChg.at(mp.second);
    if (diMuOS &&  charge1*charge2 > 0) continue;

    return true;
  }

  return false;
}       // -----  end of function L1AlgoFactory::TripleMuOS  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::ETM_HTT
//  Description:  
// ===========================================================================
bool L1AlgoFactory::ETM_HTT(float ETMcut, float HTTcut, bool isETMHF)
{
  
  Float_t TheHTT = -10;
  HTTVal(TheHTT);

  Float_t TheETM = -10;
  if (isETMHF)
    ETMHFVal(TheETM);
  else
    ETMVal(TheETM);

  if (TheETM >= ETMcut && TheHTT >= HTTcut)
    return true;
  return false;
}       // -----  end of function L1AlgoFactory::ETM_HTT  -----


// ===  FUNCTION  ============================================================
//         Name:  L1AlgoFactory::DoubleJet_dEtaMass
//  Description:  
// ===========================================================================
bool L1AlgoFactory::DoubleJet_dEtaMass(float Jet1Pt,  
    float Jet2Pt, float EtaRestrict, bool isleading, float dEtaMax, float MassMin)
{
  if(upgrade_->nJets < 2) return false;

  std::vector<std::pair<unsigned,unsigned> > jetPairs;

  for (unsigned ue=0; ue < upgrade_->nJets; ue++) {
    Int_t bx = upgrade_->jetBx.at(ue);        		
    if(bx != 0) continue;
    Float_t eta1 = upgrade_->jetEta.at(ue);
    if (EtaRestrict != 999 && fabs(eta1) > EtaRestrict) continue;  // eta = 6 - 16
    Float_t pt = upgrade_->jetEt.at(ue);


    for(unsigned ve=ue+1; ve < upgrade_->nJets; ve++) {
      if(ve == ue) continue;
      Int_t bx2 = upgrade_->jetBx.at(ve);        		
      if(bx2 != 0) continue;
      Float_t eta2 = upgrade_->jetEta.at(ve);
      if (EtaRestrict != 999 && fabs(eta2) > EtaRestrict) continue;  // eta = 6 - 16
      Float_t pt2 = upgrade_->jetEt.at(ve);

      unsigned tmp_pt1 = pt >= pt2 ? ue : ve;
      unsigned tmp_pt2 = pt >= pt2 ? ve : ue;
      jetPairs.push_back(std::pair<unsigned,unsigned>(tmp_pt1,tmp_pt2));
    }
  }

  std::vector<std::pair<unsigned,unsigned> > jetPairs2;
  if (isleading)
  {
    std::pair<unsigned, unsigned> max_pair = jetPairs.front();
    for(auto j : jetPairs)
    {
      //std::cout << j.first <<" : " << j.second <<"   " << upgrade_->jetEt.at(j.first) <<
        //":" << upgrade_->jetEt.at(j.second) << std::endl;
      if (upgrade_->jetEt.at(j.first)  >=  upgrade_->jetEt.at(max_pair.first) 
          && upgrade_->jetEt.at(j.second)  >=  upgrade_->jetEt.at(max_pair.second) )
      {
        max_pair = j;
      }
    }
    jetPairs2.push_back(max_pair);
  }
  else
    jetPairs2 = jetPairs;


  for(auto i : jetPairs2)
  {
    if (upgrade_->jetEt.at(i.first) < Jet1Pt) continue;
    if (upgrade_->jetEt.at(i.second) < Jet2Pt) continue;
    if(!correlateInEta(upgrade_->jetEta.at(i.first), upgrade_->jetEta.at(i.second), dEtaMax)) continue;
    TLorentzVector jet1(0,0,0,0);
    TLorentzVector jet2(0,0,0,0);
    jet1.SetPtEtaPhiM(upgrade_->jetEt.at(i.first),upgrade_->jetEta.at(i.first), upgrade_->jetPhi.at(i.first), 0);
    jet2.SetPtEtaPhiM(upgrade_->jetEt.at(i.second),upgrade_->jetEta.at(i.second), upgrade_->jetPhi.at(i.second), 0);
    TLorentzVector dijMass = jet1 + jet2;
    if (dijMass.M()  < MassMin )
      continue;
    return true;
  }

  return false;
}       // -----  end of function L1AlgoFactory::DoubleJet_dEtaMass  -----
