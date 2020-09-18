// ===========================================================================
// 
//       Filename:  L1uGT.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/20/2016 02:05:39 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "L1uGT.h"



//----------------------------------------------------------------------------
//       Class:  L1uGT
//      Method:  L1uGT
// Description:  constructor
//----------------------------------------------------------------------------
L1uGT::L1uGT (
    TFile* outrootfile_,
    L1Analysis::L1AnalysisEventDataFormat         *event__,
    GlobalAlgBlk *l1uGT__,
    StructL1Event *L1Event_, 
    std::map<std::string, L1Seed> *mL1Seed_
    ): 
  outfile(outrootfile_),
  event_(event__),
  l1uGT_(l1uGT__),
  L1Event(L1Event_),
  mL1Seed(mL1Seed_)
{
  XMLConv["L1_MU20_EG15"] = "L1_Mu20_EG15";
}  // -----  end of method L1uGT::L1uGT  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1uGT
//      Method:  L1uGT
// Description:  copy constructor
//----------------------------------------------------------------------------
L1uGT::L1uGT ( const L1uGT &other )
{
}  // -----  end of method L1uGT::L1uGT  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1uGT
//      Method:  ~L1uGT
// Description:  destructor
//----------------------------------------------------------------------------
L1uGT::~L1uGT ()
{
}  // -----  end of method L1uGT::-L1uGT  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  L1uGT
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  L1uGT&
L1uGT::operator = ( const L1uGT &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method L1uGT::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  L1uGT::GetTreeAlias
//  Description:  
// ===========================================================================
bool L1uGT::GetTreeAlias(std::map<std::string, std::string> SeedAlias)
{
  for (auto const & name: SeedAlias) {
    if (XMLConv.find(name.first) != XMLConv.end())
      SeedBit[XMLConv[name.first]] = ParseAlias(name.second);
    else
      SeedBit[name.first] = ParseAlias(name.second);
  }
  return true;
}       // -----  end of function L1uGT::GetTreeAlias  -----

// ===  FUNCTION  ============================================================
//         Name:  L1uGT::ParseAlias
//  Description:  
// ===========================================================================
unsigned int L1uGT::ParseAlias(std::string alias) const
{
  std::smatch base_match;
  std::regex integer("L1uGT\\.m_algoDecisionInitial\\[([0-9]+)\\]");
  unsigned int nbit = 0;

  if (std::regex_match(alias, base_match, integer))
  {
    nbit = std::stoi(base_match[1].str(), nullptr);
  }

  return nbit;
}       // -----  end of function L1uGT::ParseAlias  -----

// ===  FUNCTION  ============================================================
//         Name:  L1uGT::CompEvents
//  Description:  /* cursor */
// ===========================================================================
bool L1uGT::CompEvents()
{
  std::vector<std::string> IgnoreSeeds;
  IgnoreSeeds.push_back("L1_ZeroBias");
  // No in the XML yet
  IgnoreSeeds.push_back("L1_Mu3_Jet35C");
  // OMTF has 0 charge muon
  IgnoreSeeds.push_back("L1_DoubleMu0er1p6_dEta_Max1p8_OS");
  // Random fireing 
  IgnoreSeeds.push_back("L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4");
  IgnoreSeeds.push_back("L1_Mu3_JetC60_dEta_Max0p4_dPhi_Max0p4");
  IgnoreSeeds.push_back("L1_Mu3_JetC120_dEta_Max0p4_dPhi_Max0p4");
  // Not checking for now
  IgnoreSeeds.push_back("L1_Jet32_DoubleMuOpen_Mu10_dPhi_Jet_Mu0_Max0p4_dPhi_Mu_Mu_Min1p0");
  IgnoreSeeds.push_back("L1_Jet32_MuOpen_EG10_dPhi_Jet_Mu_Max0p4_dPhi_Mu_EG_Min1p0");

  for(const auto &seed : *mL1Seed)
  {
    bool uGT  = l1uGT_->getAlgoDecisionInitial(SeedBit[seed.first]);
    bool uGTFinal = l1uGT_->getAlgoDecisionFinal(SeedBit[seed.first]);
    assert(uGT == uGTFinal);
    if (seed.second.eventfire != uGT)
    {
      if (std::find(IgnoreSeeds.begin(), IgnoreSeeds.end(), seed.first) != IgnoreSeeds.end())
        continue;
      std::cout << event_->run << ":"  << event_->event <<" " << seed.first 
        <<" from Menu " << seed.second.eventfire <<"; from uGT "<< uGT 
        <<" GTFinal " << uGTFinal  << std::endl;
    }

  }
  return true;
}       // -----  end of function L1uGT::CompEvents  -----


// ===  FUNCTION  ============================================================
//         Name:  L1uGT::GetuGTDecision
//  Description:  
// ===========================================================================
bool L1uGT::GetuGTDecision(const std::string &seed, bool IsInit)
{
  if (SeedBit.find(seed) == SeedBit.end())
    return false;
  if (IsInit)
    return l1uGT_->getAlgoDecisionInitial(SeedBit[seed]);
  else
    return l1uGT_->getAlgoDecisionFinal(SeedBit[seed]);
}       // -----  end of function L1uGT::GetuGTDecision  -----
