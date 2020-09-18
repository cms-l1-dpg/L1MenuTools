// ===========================================================================
// 
//       Filename:  L1TnP.C
// 
//    Description:  A class for tag and probe approach for efficiency
//    measurement for EG and Muon.
// 
//        Version:  1.0
//        Created:  04/07/2016 07:15:19 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================



#include "L1TnP.h"


//----------------------------------------------------------------------------
//       Class:  L1TnP
//      Method:  L1TnP
// Description:  constructor
//----------------------------------------------------------------------------
L1TnP::L1TnP (
    TFile* outrootfile_,
    L1Analysis::L1AnalysisEventDataFormat         *event__,
    L1Analysis::L1AnalysisL1UpgradeDataFormat     *upgrade__,
    L1Analysis::L1AnalysisRecoJetDataFormat       *recoJet__,
    L1Analysis::L1AnalysisRecoMetDataFormat       *recoSum__,
    L1Analysis::L1AnalysisRecoElectronDataFormat  *recoEle__,
    L1Analysis::L1AnalysisRecoMuon2DataFormat     *recoMuon__,
    L1Analysis::L1AnalysisRecoTauDataFormat       *recoTau__,
    L1Analysis::L1AnalysisRecoMetFilterDataFormat *recoFilter__,
    L1Analysis::L1AnalysisL1CaloTowerDataFormat   *l1CaloTower__,
    L1Analysis::L1AnalysisRecoVertexDataFormat    *recoVtx__,
    GlobalAlgBlk *l1uGT__        
    ):
  L1Plot( outrootfile_, event__,  upgrade__, recoJet__, recoSum__, recoEle__, 
	  recoMuon__, recoTau__,recoFilter__, l1CaloTower__, recoVtx__,l1uGT__ ),
  doMuonTnP(false)
{
}  // -----  end of method L1TnP::L1TnP  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1TnP
//      Method:  ~L1TnP
// Description:  destructor
//----------------------------------------------------------------------------
L1TnP::~L1TnP ()
{
}  // -----  end of method L1TnP::-L1TnP  (destructor)  -----


// ===  FUNCTION  ============================================================
//         Name:  L1TnP::RunTnP
//  Description:  
// ===========================================================================
bool L1TnP::RunTnP()
{
  if (doMuonTnP)
    RunMuonTnP();

  return true;
}       // -----  end of function L1TnP::RunTnP  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::PostRun
//  Description:  
// ===========================================================================
bool L1TnP::PostRun()
{
  if (doMuonTnP) WriteMuonHistogram();
  return true;
}       // -----  end of function L1TnP::PostRun  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::DoMuonTnP
//  Description:  
// ===========================================================================
bool L1TnP::DoMuonTnP()
{
  doMuonTnP = true;
  BookMuonHistogram();
  return true;
}       // -----  end of function L1TnP::DoMuonTnP  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::BookMuonHistogram
//  Description:  
// ===========================================================================
bool L1TnP::BookMuonHistogram()
{
  hMuon1F["nRecoMuons"]  = new TH1F("nRecoMuons", "nRecoMuons;No. of Reco Muons;Events", 10, 0, 10);
  hMuon1F["RecoZMass"]  = new TH1F("RecoZMass", "RecoZMass;Reco Z mass;Events", 120, 60, 120);
  hMuon1F["RecoZPt"]  = new TH1F("RecoZPt", "RecoZPt;Reco Z Pt;Events", 120, 0, 120);
  hMuon1F["AllZMass"]  = new TH1F("AllZMass", "AllZMass;All Z mass;Events", 120, 60, 120);
  hMuon1F["AllZPt"]  = new TH1F("AllZPt", "AllZPt;All Z Pt;Events", 120, 0, 120);

  hMuon2F["RecL1dR"]  = new TH2F("RecL1dR", "RecL1dR;Reco Muon Pt; dR(L1Mu, recoMu)", 200, 0, 200, 10, 0, 0.1);
  hMuon2F["RecL1dPt"]  = new TH2F("RecL1dPt", "RecL1dPt;Reco Muon Pt; dPt(L1Mu, recoMu)", 200, 0, 200, 100, -100, 100);
  hMuon2F["RecL1Eta"]  = new TH2F("RecL1Eta", "RecL1Eta;Reco Muon Eta; L1 Muon Eta", 50, -2.5, 2.5, 50, -2.5, 2.5);
  hMuon2F["RecL1Pt"]  = new TH2F("RecL1Pt", "RecL1Pt;Reco Muon Pt; L1 Muon Pt", 100, 0, 200, 100, 0, 200);
  BookMuonEffHist("Reco");
  return true;
}       // -----  end of function L1TnP::BookMuonHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::BookMuonEffHist
//  Description:  
// ===========================================================================
bool L1TnP::BookMuonEffHist(const std::string prefix) 
{
  std::string name = prefix + "_EffPt";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 100, 0, 100 );
  name = prefix + "_EffPt_BMTF";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 100, 0, 100 );
  name = prefix + "_EffPt_OMTF";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 100, 0, 100 );
  name = prefix + "_EffPt_EMTF";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 100, 0, 100 );
  name = prefix + "_EffEta";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 50, -2.5, 2.5 );
  name = prefix + "_EffEta_Pt16";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 50, -2.5, 2.5 );
  name = prefix + "_EffEta_Pt20";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 50, -2.5, 2.5 );
  name = prefix + "_EffEta_Pt30";
  hMuonEff[name] = new TEfficiency(name.c_str(), name.c_str(), 50, -2.5, 2.5 );

  return true;
}       // -----  end of function L1TnP::BookMuonEffHist  -----


// ===  FUNCTION  ============================================================
//         Name:  L1TnP::WriteMuonHistogram
//  Description:  
// ===========================================================================
bool L1TnP::WriteMuonHistogram() const
{
  if (!doMuonTnP) return false;
  outfile->mkdir("TnPMuon");
  outfile->cd("TnPMuon");
  TCanvas c1("Eff", "Eff", 600, 500);
  for(auto f : hMuon2F)
  {
    f.second->Write();
  }
  for(auto f : hMuon1F)
  {
    f.second->Write();
  }
  for(auto f : hMuonEff)
  {
    f.second->GetPassedHistogram()->Write();
    f.second->GetTotalHistogram()->Write();
    c1.cd();
    f.second->Paint("AP");
    f.second->GetPaintedGraph()->Write( f.first.c_str());
  }
  outfile->cd();
  return true;
}       // -----  end of function L1TnP::WriteMuonHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::RunMuonTnP
//  Description:  How to do Tag and Probe for muon?
//  Use L1Muon, tag a barrel Muon and probe the forward, within Z mass?
//  Use RecoMuon, found the Z, tag the recoMuon from Z?
// ===========================================================================
bool L1TnP::RunMuonTnP()
{
  // Veto bad event
  bool passFilter = true && recoFilter_->goodVerticesFilter
                   && recoFilter_->cscTightHalo2015Filter
                   && recoFilter_->eeBadScFilter
                   && recoFilter_->ecalDeadCellTPFilter
                   && recoFilter_->hbheNoiseIsoFilter
                   && recoFilter_->hbheNoiseFilter
                   && recoFilter_->chHadTrackResFilter
                   && recoFilter_->muonBadTrackFilter;
  if (!passFilter) return passFilter; // If event is skip aleady

  // Get medium PFMuons
  std::vector<TLorentzVector> RecoMuons = GetRecoMuon(2.5, 0.15, 2); 
  hMuon1F["nRecoMuons"] -> Fill(RecoMuons.size());

  // Check whether these are Z, now RecoMuons are the two muon from Z
  //if (!Mu_PassRecoZ(RecoMuons)) return false;

  // Mapping recoMuon to L1Muon by index
  std::unordered_map<int, int> MuRecL1 = Mu_MapRecoL1(RecoMuons, 2, 0);


  for(unsigned int i=0; i < RecoMuons.size(); ++i)
  {
    TLorentzVector rMu = RecoMuons.at(i);
    if (MuRecL1[i]!=-1)
    {
      hMuon2F["RecL1Eta"] ->Fill(rMu.Eta(), upgrade_->muonEta.at(MuRecL1[i]) );
      hMuon2F["RecL1Pt"] ->Fill(rMu.Pt(), upgrade_->muonEt.at(MuRecL1[i]) );
    }

    hMuonEff["Reco_EffEta"]->Fill(MuRecL1[i]!=-1, rMu.Eta());
    if (rMu.Pt() > 16)
      hMuonEff["Reco_EffEta_Pt16"]->Fill(MuRecL1[i]!=-1, rMu.Eta());
    if (rMu.Pt() > 20)
      hMuonEff["Reco_EffEta_Pt20"]->Fill(MuRecL1[i]!=-1, rMu.Eta());
    if (rMu.Pt() > 30)
      hMuonEff["Reco_EffEta_Pt30"]->Fill(MuRecL1[i]!=-1, rMu.Eta());
  }
  return true;
}       // -----  end of function L1TnP::RunMuonTnP  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::Mu_PassRecoZ
//  Description:  No charge in recoMuon stored. Form Z with 20GeV window for
//  stat.
// ===========================================================================
bool L1TnP::Mu_PassRecoZ(std::vector<TLorentzVector> &rMuons )
{
  const float zMass = 91;
  const float zMassMin = 71;
  const float zMassMax = 111;

  double zMassCurrent = 1.0e300;
  TLorentzVector bestRecoZ(0, 0, 0, 0);
  int sumCharge = 0;
  std::pair<unsigned int, unsigned int> mupair;
  for(unsigned int i = 0; i < rMuons.size(); ++i)
  {
    for(unsigned int j = 0; j < i && j < rMuons.size(); ++j)
    {
      double zm = (rMuons.at(i) + rMuons.at(j)).M();
      //if(zm > zMassMin && zm < zMassMax && fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      if(fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      {
        bestRecoZ = rMuons.at(i) + rMuons.at(j);
        mupair = std::make_pair(i, j);
        zMassCurrent = zm;
        sumCharge = 0;
        // No charge stored in the recoMuon!!
        //sumCharge = cutMuCharge->at(i) + cutMuCharge->at(j);
      }
    }
  }

  hMuon1F["AllZMass"] ->Fill(bestRecoZ.M());
  hMuon1F["AllZPt"] ->Fill(bestRecoZ.Pt());
  if (bestRecoZ.Pt() != 0 && sumCharge == 0 && (bestRecoZ.M() > zMassMin) && (bestRecoZ.M() < zMassMax))
  {
    hMuon1F["RecoZMass"] ->Fill(bestRecoZ.M());
    hMuon1F["RecoZPt"] ->Fill(bestRecoZ.Pt());

    std::vector<TLorentzVector> MuFromZ;
    MuFromZ.push_back(rMuons.at(mupair.first));
    MuFromZ.push_back(rMuons.at(mupair.second));
    rMuons = MuFromZ;
    return true;
  } else{
    rMuons.clear();
    return false;
  }

  return false;
}       // -----  end of function L1TnP::Mu_PassRecoZ  -----


// ===  FUNCTION  ============================================================
//         Name:  L1TnP::Mu_MapRecoL1
//  Description:  
// ===========================================================================
std::unordered_map<int, int> L1TnP::Mu_MapRecoL1(std::vector<TLorentzVector> &rMuons, int l1qualmin, int l1muonBX)
{
  const float dRMax = 0.3;
  std::unordered_map<int, int> RecL1Mu;
  for(unsigned irMu = 0; irMu < rMuons.size(); irMu++)
  {
    RecL1Mu[irMu] = -1;
  }

  for(UInt_t imu=0; imu < upgrade_->nMuons; imu++) {
    // Select muonBX
    if(upgrade_->muonBx.at(imu) != l1muonBX) continue;
    if (l1qualmin == 0 && ! (upgrade_->muonQual.at(imu) >= 4))  continue;
    if (l1qualmin == 1 && ! (upgrade_->muonQual.at(imu) >= 8))  continue;
    if (l1qualmin == 2 && ! (upgrade_->muonQual.at(imu) >= 12)) continue;
    TLorentzVector l1Mu(0, 0,0,0);
    l1Mu.SetPtEtaPhiM(upgrade_->muonEt.at(imu), upgrade_->muonEta.at(imu),
        upgrade_->muonPhi.at(imu), 0);
    for(unsigned irMu = 0; irMu < rMuons.size(); irMu++)
    {
      TLorentzVector rMu = rMuons.at(irMu);
      if (l1Mu.DeltaR(rMu) < dRMax) 
      {
        if (RecL1Mu[irMu] == -1)
        {
          RecL1Mu[irMu] = imu;
        } else{ // Multiple L1Mu map to RecoMuon
          RecL1Mu[irMu] = Mu_PickOverlapL1Mu(rMu, RecL1Mu[irMu], imu, 0);
        }
      }
    }
  }

  return RecL1Mu;
}       // -----  end of function L1TnP::Mu_MapRecoL1  -----

// ===  FUNCTION  ============================================================
//         Name:  L1TnP::Mu_PickOverlapL1Mu
//  Description:  In case two L1Mus and RecoMu within certain dR, how to pick?
//  option 0: Use the minimum dR
// ===========================================================================
unsigned int L1TnP::Mu_PickOverlapL1Mu(TLorentzVector &rMu, unsigned int imu1, unsigned int imu2, unsigned int opt)
{

  TLorentzVector l1Mu1(0, 0,0,0);
  TLorentzVector l1Mu2(0, 0,0,0);
  l1Mu1.SetPtEtaPhiM(upgrade_->muonEt.at(imu1), upgrade_->muonEta.at(imu1),
      upgrade_->muonPhi.at(imu1), 0);
  l1Mu2.SetPtEtaPhiM(upgrade_->muonEt.at(imu2), upgrade_->muonEta.at(imu2),
      upgrade_->muonPhi.at(imu2), 0);
  hMuon2F["RecL1dR"] ->Fill(rMu.Pt(), rMu.DeltaR(l1Mu1));
  hMuon2F["RecL1dR"] ->Fill(rMu.Pt(), rMu.DeltaR(l1Mu2));
  hMuon2F["RecL1dPt"] ->Fill(rMu.Pt(), rMu.Pt()-l1Mu1.Pt()); 
  hMuon2F["RecL1dPt"] ->Fill(rMu.Pt(), rMu.Pt()-l1Mu2.Pt()); 

  if (opt == 0)
  {
    if (rMu.DeltaR(l1Mu1) < rMu.DeltaR(l1Mu2))
      return imu1;
    else
      return imu2;
  }

  if (opt == 1)
  {
    if (fabs(rMu.Pt()- l1Mu1.Pt()) < fabs(rMu.Pt()- l1Mu2.Pt()))
      return imu1;
    else
      return imu2;
  }

  return -1;
}       // -----  end of function L1TnP::Mu_PickOverlapL1Mu  -----


//// ===  FUNCTION  ============================================================
////         Name:  L1TnP::Mu_GetTagProbeLegs
////  Description:  
//// ===========================================================================
  //std::vector<std::pair<TLorentzVector, int> > 
//L1TnP::Mu_GetTagProbeLegs( std::vector<TLorentzVector> &rMuons, std::unordered_map<int, int> MuRecL1 )
//{
  //return true;
//}       // -----  end of function L1TnP::Mu_GetTagProbeLegs  -----
