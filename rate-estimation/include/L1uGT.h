// ===========================================================================
// 
//       Filename:  L1uGT.h
// 
//    Description:  A class to compare with the L1uGTTree decision
// 
//        Version:  1.0
//        Created:  04/20/2016 02:05:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_L1UGT_INC
#define  MY_L1UGT_INC

#include <regex>
#include <map>

#include "TFile.h"

#include "L1Struct.h"

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// ===========================================================================
//        Class:  L1uGT
//  Description:  
// ===========================================================================
class L1uGT
{
  public:

    // ====================  LIFECYCLE     ===============================
    L1uGT ( TFile* outrootfile_, 
        L1Analysis::L1AnalysisEventDataFormat *event__, 
        GlobalAlgBlk *l1uGT__, StructL1Event *L1Event_, 
        std::map<std::string, L1Seed> *mL1Seed_ );
    L1uGT ( const L1uGT &other );   // copy constructor
    ~L1uGT ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool CompEvents();
    bool GetTreeAlias(std::map<std::string, std::string> SeedAlias);
    bool GetuGTDecision(const std::string &seed, bool IsInit=true);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    L1uGT& operator = ( const L1uGT &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    unsigned int ParseAlias(std::string alias) const;

    // ====================  DATA MEMBERS  ===============================
    TFile                                 *outfile;
    L1Analysis::L1AnalysisEventDataFormat *event_;

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    GlobalAlgBlk *l1uGT_;
    StructL1Event *L1Event;
    std::map<std::string, L1Seed> *mL1Seed;

    std::map<std::string, unsigned int> SeedBit;
    std::map<std::string, std::string> XMLConv;
    

}; // -----  end of class L1uGT  -----

#endif   // ----- #ifndef MY_L1UGT_INC  -----
