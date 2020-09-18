// ===========================================================================
// 
//       Filename:  PreColumn.h
// 
//    Description:  A prescale column, which is a simpler class of the
//    L1Menu2016. Only calculate the rate of the menu
// 
//        Version:  1.0
//        Created:  01/24/2017 03:06:58 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_PRECOLUMN_INC
#define  MY_PRECOLUMN_INC

#include <map>
#include "L1Struct.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

// ===========================================================================
//        Class:  PreColumn
//  Description:  
// ===========================================================================
class PreColumn
{
  public:

    // ====================  LIFECYCLE     ===============================
    PreColumn (int ColIdx, std::map<std::string, L1Seed> mL1Seed); // constructor
    ~PreColumn ();
    bool PassRelation( std::vector<std::string> vL1Seed_, 
        std::map<int, std::string> BitMap_, 
        std::map<std::string, std::vector<int> > POGMap_, 
        std::map<std::string, std::vector<int> > PAGMap_);

    // ====================  ACCESSORS     ===============================
    bool CheckCorrelation();
    bool EventReset();
    bool InsertInMenu(std::string L1name, bool value);
    bool PrintCSV(std::vector<std::string> &out, double scale);
    bool WriteHistogram(TFile *outrootfile);
    bool PrintRates(std::ostream &out, double scale);
    bool PrintPUCSV( std::vector<std::string> &csvout);
    bool BookHistogram();
    bool FillPileUpSec(float pu);
    bool FillDefHist2D(double &scale);
    bool FillDefHist1D(double &scale);
    bool Fill2DCorrelations(const std::string &histname, std::set<std::string> &event) const;
    void CorrectScale(TH1* h, Float_t scal);
    bool CalRate(double scale);
    bool PrintMenuRate(double scale) const;

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool CheckPureFire();
    bool CheckPhysFire();

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    int ColIdx;
    bool bybit;
    std::map<std::string, L1Seed> mL1Seed;
    double nFireevents;

    std::set<std::string> FireSeed;
    std::set<std::string> FiredPhy;
    std::map<std::string, int > PhyCounts;
    std::map<std::string, int > PhyPureCounts;
    std::map<std::string, float > PhyPropCounts;
    std::map<std::string, std::map<float, int> > L1PUCount; // counting lumi section

    std::map<std::string, TH1F*> HistMap;
    std::map<std::string, TH2F*> Hist2D;

    std::vector<std::string> vL1Seed;
    std::map<int, std::string> BitMap;
    std::map<std::string, std::vector<int> > POGMap;
    std::map<std::string, std::vector<int> > PAGMap;
    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class PreColumn  -----


#endif   // ----- #ifndef MY_PRECOLUMN_INC  -----
