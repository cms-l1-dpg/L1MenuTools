// ===========================================================================
// 
//       Filename:  L1Plot.h
// 
//    Description: G
// 
//        Version:  1.0
//        Created:  01/31/2016 03:55:43 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_L1PLOT_INC
#define  MY_L1PLOT_INC

#include <regex>
#include <map>
#include <iostream>
#include <functional>
#include <cassert>

#include "TH1F.h"
#include "TH2F.h"
#include "TVector2.h"
#include "TFile.h"
#include "TEfficiency.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TProfile.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoJetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMetFilterDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoElectronDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMuon2DataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoTauDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1CaloTowerDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoVertexDataFormat.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"

#include "L1Struct.h"

// ===========================================================================
//        Class:  L1Plot
//  Description:  
// ===========================================================================
class L1Plot
{
  public:

    // ====================  LIFECYCLE     ===============================
    L1Plot (
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
        L1Analysis::L1AnalysisRecoVertexDataFormat    *recoVtx_ = nullptr,
	GlobalAlgBlk                                  *l1uGT_       = nullptr	
        );

    L1Plot ( const L1Plot &other );   // copy constructor
    ~L1Plot ();                            // destructor

    // ====================  ACCESSORS     ===============================

    // ====================  MUTATORS      ===============================
    bool PreRun( StructL1Event *L1Event_, std::map<std::string, L1Seed> *mL1Seed_, std::map<std::string, std::string> muGtAlias_);
    bool RunPlot();
    bool PostRun(double scale);
    void SetTodo ( std::map<std::string, float> &L1Config);

    bool GetRecoEvent();
    bool PlotRatePerLS( const std::map<std::string, std::map<int, int> > &L1LSCount, const float &nBunches);
    // ====================  OPERATORS     ===============================

    L1Plot& operator = ( const L1Plot &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool GetRecoFilter() const;
    bool GoodRecoJet(int ijet) const;
    std::vector<TLorentzVector> GetRecoTau(bool isER=false, int Iso =0) const;
    std::vector<TLorentzVector> GetRecoMuon(float MuERcut=999, float IsoCut=0, int qual=0) const;
    std::vector<TLorentzVector> GetRecoEle(bool isER=false, float IsoCut=0, int qual=0) const;
    std::vector<TLorentzVector> GetRecoJet(bool isCent=false) const;

    // ====================  DATA MEMBERS  ===============================
    TFile        *outfile;
    L1Analysis::L1AnalysisEventDataFormat         *event_;
    L1Analysis::L1AnalysisL1UpgradeDataFormat     *upgrade_;
    L1Analysis::L1AnalysisRecoJetDataFormat       *recoJet_;
    L1Analysis::L1AnalysisRecoMetDataFormat       *recoSum_;
    L1Analysis::L1AnalysisRecoElectronDataFormat  *recoEle_;
    L1Analysis::L1AnalysisRecoMuon2DataFormat     *recoMuon_;
    L1Analysis::L1AnalysisRecoTauDataFormat       *recoTau_;
    L1Analysis::L1AnalysisRecoMetFilterDataFormat *recoFilter_;
    L1Analysis::L1AnalysisL1CaloTowerDataFormat   *l1CaloTower_;
    L1Analysis::L1AnalysisRecoVertexDataFormat    *recoVtx_;
    GlobalAlgBlk                                  *l1uGT_;        

  private:
    // ====================  METHODS       ===============================
    std::vector<TLorentzVector> GetRecoSum(std::string type ) const;
    std::vector<TLorentzVector> GetRecoHTLocal() const;
    std::vector<TLorentzVector> GetRecoHTMLocal() const;
    TVector2 GetRecoMETNoMu() const;
    TVector2 GetL1METCalo();
    inline bool SortVTLVs(std::vector<TLorentzVector> &reTLVs) const;
    double FunLeadingPt(std::string obj);

    bool BookRateHistogram();
    bool FillRateHistogram();
    bool WriteRateHistogram(double scale) const;
    float SingleMuEta(float ptCut, unsigned int qualmin=2) const;
    float FillRateDoubleMu(float pt2Cut, unsigned int qualmin=1, float pt1Cut=0);
    float SingleJetEta(float ptCut) const;
    float SingleEGEta(float ptCut, bool doIso=false) const;

    bool BookEffHistogram();
    bool FillEffHistogram();
    bool WriteEffHistogram();

    bool BookuGtHistogram();
    bool FilluGtHistogram();
    bool WriteuGtHistogram();
    
    bool BookLSHistogram( const std::map<std::string, std::map<int, int> > &L1LSCount, const float &nBunches);
    bool ScaleLSHistogram( const std::map<std::string, std::map<int, int> > &L1LSCount, const float &nBunches);
    bool WriteLSHistogram( const std::map<std::string, std::map<int, int> > &L1LSCount);

    bool WriteTestHistogram() const;
    bool BookTestHistogram();
    bool TestMETActivity();
    float TestRecoAct(float eta) const;
    bool TestCaloTower();
    bool TestMuon();
    // ====================  DATA MEMBERS  ===============================
    bool doPlotRate;
    bool doPlotEff;
    bool doPlotTest;
    bool doPlotuGt;
    bool UseL1CaloTower;
    bool UsePFMETNoMuon;

    StructL1Event *L1Event;
    std::map<std::string, L1Seed> *mL1Seed;
    std::map<std::string, std::string> muGtAlias;
    std::map<std::string, int> muGtAlgoMap;    
    std::map<std::string, std::vector<TLorentzVector> > recoEvent;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Plots ~~~~~
    std::map<std::string,TH1F*> hRate1F;
    std::map<std::string,TH2F*> hRate2F;
    std::map<std::string,TH1F*> huGt1F;
    std::map<std::string,TH2F*> huGt2F;        
    std::map<std::string,TEfficiency*> hEff;
    std::map<std::string,TH1F*> hTest1F;
    std::map<std::string,TH2F*> hTest2F;
    std::map<std::string,TProfile*> hTestPro;
	std::map<std::string, std::function<double()> > hEffFun;
}; // -----  end of class L1Plot  -----


#endif   // ----- #ifndef MY_L1PLOT_INC  -----
