// ===========================================================================
// 
//       Filename:  L1Struct.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/2016 03:49:15 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_L1STRUCT_INC
#define  MY_L1STRUCT_INC
 
// Some global config tag for dataformat changes
//#define L1T_PRE_TSGV3


#include "DataFormats/L1Trigger/interface/EtSum.h"

enum EtSumType { 
  ETT   = l1t::EtSum::EtSumType::kTotalEt,
  HTT   = l1t::EtSum::EtSumType::kTotalHt,
  ETM   = l1t::EtSum::EtSumType::kMissingEt,
  HTM   = l1t::EtSum::EtSumType::kMissingHt,
  ETMHF = l1t::EtSum::EtSumType::kMissingEtHF,
  ETTHF = l1t::EtSum::EtSumType::kTotalEtHF,
  ETTEM = l1t::EtSum::EtSumType::kTotalEtEm,
  HTTHF = l1t::EtSum::EtSumType::kTotalHtHF,
  HTMHF = l1t::EtSum::EtSumType::kMissingHtHF,
}; // Base on "DataFormats/L1Trigger/interface/EtSum.h"

struct L1Seed
{
  std::string name;
  int bit;
  int prescale;
  std::vector<std::string> POG;
  std::vector<std::string> PAG;
  unsigned int ncounts;
  bool eventfire;
  unsigned int firecounts;
  unsigned int purecounts;
  float propcounts; // proportional counts;
  double firerate;
  double firerateerror;
  double purerate;
  double proprate;
  std::string comment;
  std::string singleObj;

  L1Seed()
  {
    name          = "";
    bit           = -1;
    prescale      = 0;
    ncounts       = 0;
    eventfire     = false;
    firecounts    = 0;
    purecounts    = 0;
    propcounts    = 0.0;
    firerate      = 0.0;
    firerateerror = 0.0;
    purerate      = 0.0;
    proprate      = 0.0;
    comment       = "";
    singleObj     = "";
    POG.clear();
    PAG.clear();
  }
};

struct StructL1Event
{
  float JetPt;
  float JetCenPt;
  float TauPt;
  float TauerPt;
  float IsoTauPt;
  float IsoTauerPt;
  float EGPt;
  float EGerPt;
  float IsoEGPt;
  float IsoEGerPt;
  float MuPt;
  float MuOpenPt;
  float MuerPt;
  float HTT;
  float HTM;
  float ETM;
  float ETT;


  float oniaMuPt1   = 0.;
  float oniaMuPt2   = 0.;
  float doubleMuPt1 = -10.;
  float doubleMuPt2 = -10.;
  float dijetPt1    = -10.;
  float dijetPt2    = -10.;
  float diCenjetPt1 = -10.;
  float diCenjetPt2 = -10.;
  float ditauPt     = -10.;
  float diIsotauPt  = -10.;
  float quadjetPt   = -10.;
  float quadjetCPt  = -10.;
  float diEG1       = -10.;
  float diEG2       = -10.;
  float diIsolEG1   = -10.;
  float diIsolEG2   = -10.;
  float ETMHF       = -10.;
  float ETTHF       = -10.;
  float ETTEM       = -10.;
  float HTTHF       = -10.;
  float HTMHF       = -10.;



  StructL1Event()
  {
    JetPt       = -10;
    JetCenPt    = -10;
    TauPt       = -10;
    TauerPt     = -10;
    IsoTauPt    = -10;
    IsoTauerPt  = -10;
    EGPt        = -10;
    EGerPt      = -10;
    IsoEGPt     = -10;
    IsoEGerPt   = -10;
    MuPt        = -10;
    MuOpenPt    = -10;
    MuerPt      = -10;
    HTT         = -10;
    HTM         = -10;
    ETM         = -10;
    ETT         = -10;
    oniaMuPt1   = -10;
    oniaMuPt2   = -10;
    doubleMuPt1 = -10.;
    doubleMuPt2 = -10.;
    dijetPt1    = -10.;
    dijetPt2    = -10.;
    diCenjetPt1 = -10.;
    diCenjetPt2 = -10.;
    ditauPt     = -10.;
    diIsotauPt  = -10.;
    quadjetPt   = -10.;
    quadjetCPt  = -10.;
    diEG1       = -10.;
    diEG2       = -10.;
    diIsolEG1   = -10.;
    diIsolEG2   = -10.;
    ETMHF       = -10.;
    ETTHF       = -10.;
    ETTEM       = -10.;
    HTTHF       = -10.;
    HTMHF       = -10.;
  }
};


#endif   // ----- #ifndef MY_L1STRUCT_INC  -----
