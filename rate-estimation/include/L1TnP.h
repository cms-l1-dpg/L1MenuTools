// ===========================================================================
// 
//       Filename:  L1TnP.h
// 
//    Description:  A class for tag and probe approach for efficiency
//    measurement for EG and Muon.
// 
//        Version:  1.0
//        Created:  04/07/2016 07:15:53 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_L1TNP_INC
#define  MY_L1TNP_INC

#include <map>
#include <iostream>
#include <unordered_map>

#include "TH1F.h"
#include "TH2F.h"
#include "TVector2.h"
#include "TFile.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"

#include "L1Plot.h"

// ===========================================================================
//        Class:  L1TnP
//  Description:  
// ===========================================================================
class L1TnP : public L1Plot
{
  public:
    // ====================  LIFECYCLE     ===============================
    L1TnP (
        TFile* outrootfile_,
        L1Analysis::L1AnalysisEventDataFormat         *event__      = nullptr,
        L1Analysis::L1AnalysisL1UpgradeDataFormat     *upgrade__    = nullptr,
        L1Analysis::L1AnalysisRecoJetDataFormat       *recoJet__    = nullptr,
        L1Analysis::L1AnalysisRecoMetDataFormat       *recoSum__    = nullptr,
        L1Analysis::L1AnalysisRecoElectronDataFormat  *recoEle__    = nullptr,
        L1Analysis::L1AnalysisRecoMuon2DataFormat     *recoMuon__   = nullptr,
        L1Analysis::L1AnalysisRecoTauDataFormat       *recoTau__    = nullptr,
        L1Analysis::L1AnalysisRecoMetFilterDataFormat *recoFilter__ = nullptr,
        L1Analysis::L1AnalysisL1CaloTowerDataFormat   *l1CaloTower_ = nullptr,
        L1Analysis::L1AnalysisRecoVertexDataFormat    *recoVtx_     = nullptr,
	GlobalAlgBlk                                  *l1uGT_       = nullptr		
        );

    ~L1TnP ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool DoMuonTnP();
    bool RunTnP();
    bool PostRun();


    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================


    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    // ====================  DATA MEMBERS  ===============================
    std::map<std::string,TH1F*> hMuon1F;
    std::map<std::string,TH2F*> hMuon2F;
    std::map<std::string,TEfficiency*> hMuonEff;

  private:
    // ====================  METHODS       ===============================
    bool BookMuonHistogram();
    bool RunMuonTnP();
    bool WriteMuonHistogram() const;
    bool BookMuonEffHist(const std::string prefix);

    unsigned int Mu_PickOverlapL1Mu(TLorentzVector &rMu, unsigned int imu1, unsigned int imu2, unsigned int opt);
    bool Mu_PassRecoZ(std::vector<TLorentzVector> &rMuons );
    std::unordered_map<int, int> Mu_MapRecoL1(std::vector<TLorentzVector> &rMuons, int l1qualmin, int l1muonBX);

    // ====================  DATA MEMBERS  ===============================
    bool doMuonTnP;

}; // -----  end of class L1TnP  -----

#endif   // ----- #ifndef MY_L1TNP_INC  -----
