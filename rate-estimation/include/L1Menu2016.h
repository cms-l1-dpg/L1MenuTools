// ===========================================================================
// 
//       Filename:  L1Menu2016.h
// 
//    Description:
// 
//        Version:  1.0
//        Created:  01/13/2016 18:39:36
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_L1MENU2016_INC
#define  MY_L1MENU2016_INC

#ifdef UTM_MENULIB
#include "../menulib.hh"
#endif /* UTM_MENULIB */

#include <map>
#include <regex>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <functional>
#include <ctype.h>
#include <stdlib.h>

// ROOT
#include "TH1F.h"
#include "TH2F.h"

// boost
#include "boost/bind.hpp"
#include "boost/tokenizer.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"

// Local
#include "L1AlgoFactory.h"
#include "L1Plot.h"
#include "L1TnP.h"
#include "L1uGT.h"
#include "L1Struct.h"
#include "PreColumn.h"

#define INFTY 262139
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;


// ===  FUNCTION  ============================================================
//         Name:  SingleObjPt
//  Description:  /* cursor */
// ===========================================================================
inline bool SingleObjPt(float* obj, double pt)
{
  return *obj >=pt ;
}       // -----  end of function SingleObjPt  -----
// ===========================================================================
//        Class:  L1Menu2016
//  Description:  A class to handle the L1Menu
// ===========================================================================
class L1Menu2016 : public L1AlgoFactory 
{
  public:

    // ====================  LIFECYCLE     ===============================
    L1Menu2016 (std::string MenuName, std::string filelist); // constructor
    ~L1Menu2016 ();                                          // destructor

    // ====================  ACCESSORS     ===============================
    bool InitConfig();
    bool PrintRates(std::ostream &out);
    bool OpenWithList(std::string filelist);
    bool ParseConfig(const std::string line);
    bool ParseRanges(const std::string Config, std::vector<std::pair<unsigned int, unsigned int> >& Container);
    bool PrintConfig() const;
    bool PostLoop();
    bool Loop();
    bool CheckL1Seed(const std::string L1Seed);
    bool L1SeedFunc();
    bool PreLoop(std::map<std::string, float> &config, std::map<std::string, std::string> &configstr);
    bool BookHistogram();
    bool ReadMenu();
    bool ReadMenuCSV(std::ifstream &menufile);
    bool ReadMenuTXT(std::ifstream &menufile);
    bool ReadDataPU();
    bool BuildRelation();
    bool PrintPUCSV();
    bool WriteHistogram();
    bool GetRunConfig(std::map<std::string, float> &config, std::map<std::string, std::string> &configstr);
    bool InitOutput();
    bool CheckLS(unsigned int currentLumi) const;
    bool CheckBX(unsigned int currentBX) const;
    bool Found_BX_in_48b(int targetBX, int currentBX);
    bool Found_BX_in_12b(int targetBX, int currentBX);
    bool BXReweight_is_1_to_6_47_48(int currentBX);
    bool BXReweight_is128(int currentBX);
    bool BXReweight_is34567(int currentBX);
    bool BXReweight_is_1_to_6_11_12(int currentBX);
    bool BXReweight_is_5_to_10(int currentBX);


    bool ConfigOutput(bool writetext_, bool writecsv_, bool writeplot_, 
        std::string outputdir_, std::string outputname_);
    std::string SetOutputName() const;
    // ====================  MUTATORS      ===============================

    bool ParseL1Seed(const std::string SeedName);
    bool ParseSingleObject(const std::string SeedName);
    bool ParseTripleJetVBF(const std::string& SeedName);
    bool ParseDoubleTau(const std::string& SeedName);
    bool ParseDoubleJet(const std::string& SeedName);
    bool ParseQuadJet(const std::string& SeedName);
    bool ParseDoubleEG(const std::string& SeedName);
    bool ParseTripleEG(const std::string& SeedName);
    bool ParseEGSum(const std::string& SeedName);
    bool ParseComplexSingleMu(const std::string& SeedName);
    bool ParseEGStrategy(const std::string & SeedName);
    bool ParseETMJetdPhi(const std::string & SeedName);
    bool ParseCrossMu(const std::string& SeedName);
    std::function<bool()> ParseBptx(const std::string Seedtoken);
    bool ParseMultiEGMass(const std::string& SeedName);
    bool ParseMuEG(const std::string& SeedName);
    bool ParseMuerTauer(const std::string& SeedName);
    bool ParseMuSum(const std::string& SeedName);
    // ====================  OPERATORS     ===============================

    L1Menu2016& operator = ( const L1Menu2016 &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================
    bool writefiles;
    bool writecsv;
    bool writeplots;
    std::string  outputdir;
    std::string  outputname;
    std::fstream *outfile;
    std::fstream *outcsv;
    TFile        *outrootfile;


  protected:
    // ====================  METHODS       ===============================

    bool GetL1Event();
    void CorrectScale(TH1* h, Float_t scal);
    bool InsertInMenu(std::string L1name, bool value);
    bool FillLumiSection(int currentLumi);
    bool FillPileUpSec();
    bool PrintCSV(std::ostream &out);
    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    double CalScale(int nEvents_ = 0, int nBunches_ = 0, bool print=false);
    bool RunMenu();
    bool Fill2DCorrelations(const std::string &histname, std::set<std::string> &event) const;
    void CalLocalHT(float &HTTcut, bool withHF);
    void CalLocalHTM(float &HTMcut);
    void CalLocalETM(float &ETMcut, bool withHF);
    bool FormPrescaleColumns();
    std::vector<std::string> TokenGroups(std::string instring) const;

    // ====================  DATA MEMBERS  ===============================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Configurations ~~~~~
    std::string outfilename;
    std::string outfiledir;
    std::string menufilename;
    std::string tuplefilename;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Rate variables ~~~~~
    double scale;
    unsigned int nZeroBiasevents;
    std::set<unsigned int> nLumi;
    std::vector<std::pair<unsigned int, unsigned int> > pLS;
    std::vector<std::pair<unsigned int, unsigned int> > pBX;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ L1Seeds ~~~~~
    StructL1Event L1Event;
    L1Plot *l1Plot;
    L1TnP *l1TnP;
    L1uGT *l1uGT;
    L1uGT *l1unpackuGT;

    std::map<std::string, float*> L1ObjectMap;
    std::map<std::string, float> L1Config;
    std::map<std::string, std::string> L1ConfigStr;
    std::map<std::string, L1Seed> mL1Seed;
	std::map<std::string, std::function<bool()>> L1SeedFun;
    std::map<int, std::string> BitMap;
    std::map<int, PreColumn*> ColumnMap;
    std::vector<std::string> vL1Seed;

    //Relationship
    std::map<std::string, std::map<int, int> > L1LSCount; // counting lumi section

    // Seed, PU, count
    std::map<unsigned, std::map<unsigned, double> > DataLSPU; // mapping of PU for data
    std::map<unsigned, unsigned> DataFillNO; // mapping of run and fill for data. Can't find fill info in L1Ntuple

    std::map<std::string, std::vector<int> > POGMap;
    std::map<std::string, std::vector<int> > PAGMap;


}; // -----  end of class L1Menu2016  -----

#endif   // ----- #ifndef MY_L1MENU2016_INC  -----
