// ===========================================================================
// 
//       Filename:  PreColumn.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/24/2017 03:21:19 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#include "PreColumn.h"


//----------------------------------------------------------------------------
//       Class:  PreColumn
//      Method:  PreColumn
// Description:  constructor
//----------------------------------------------------------------------------
PreColumn::PreColumn (int ColIdx_, std::map<std::string, L1Seed> mL1Seed_, int prescale_precision_, nlohmann::json* customPUweights_):
  ColIdx(ColIdx_), mL1Seed(mL1Seed_), prescale_precision(prescale_precision_), customPUweights(customPUweights_)
{
  bybit = false;
  nFireevents = 0;  
  //nFireevents_w = 0;
}  // -----  end of method PreColumn::PreColumn  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  PreColumn
//      Method:  ~PreColumn
// Description:  destructor
//----------------------------------------------------------------------------
PreColumn::~PreColumn ()
{
}  // -----  end of method PreColumn::-PreColumn  (destructor)  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::PassRelation
//  Description:  
// ===========================================================================
bool PreColumn::PassRelation( std::vector<std::string> vL1Seed_,
    std::map<int, std::string> BitMap_,
    std::map<std::string, std::vector<int> > POGMap_,
    std::map<std::string, std::vector<int> > PAGMap_)
{
  vL1Seed =vL1Seed_;
  BitMap  =BitMap_;
  POGMap  =POGMap_;
  PAGMap  =PAGMap_;
  return true;
}       // -----  end of function PreColumn::PassRelation  -----

bool PreColumn::InsertInMenu(std::string L1name, bool value) 
{
  bool post_prescale = false;

  if ( mL1Seed.find(L1name) == mL1Seed.end() ) {
    std::cout << "This shouldn't happen!" << std::endl;
    return false;
  }

  mL1Seed[L1name].eventfire = false;
  if (value)
    mL1Seed[L1name].ncounts += prescale_precision;

  if (mL1Seed[L1name].prescale == 0)
    return false;

  if (mL1Seed[L1name].ncounts >= mL1Seed[L1name].prescale_discret)
  {
    post_prescale = value; 
    mL1Seed[L1name].ncounts -= mL1Seed[L1name].prescale_discret;
  }

  mL1Seed[L1name].eventfire = post_prescale;
  if (post_prescale)
  {
    mL1Seed[L1name].firecounts++;
    mL1Seed[L1name].uwfirecounts++;
    FireSeed.insert(L1name);
  }

  return true;
}  // -----  end of function PreColumn::InsertInMenu  -----

// Reweighting procedure: information about the weight corresponding to a given pileup needed for the counting
bool PreColumn::InsertInMenu(std::string L1name, bool value, float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  bool post_prescale = false;
  float ev_puweight = -1;
  ev_puweight = ExtractPileUpWeight(pu, reweight_2018, reweight_Run3, custom_weights);

  if ( mL1Seed.find(L1name) == mL1Seed.end() ) {
    std::cout << "This shouldn't happen!" << std::endl;
    return false;
  }

  mL1Seed[L1name].eventfire = false;
  if (value)
    mL1Seed[L1name].ncounts += ev_puweight * prescale_precision;

  if (mL1Seed[L1name].prescale == 0)
    return false;

  if ((mL1Seed[L1name].ncounts >= mL1Seed[L1name].prescale_discret) && value)
  {
    post_prescale = value; 
    mL1Seed[L1name].ncounts -= mL1Seed[L1name].prescale_discret;
  }

  mL1Seed[L1name].eventfire = post_prescale;
  if (post_prescale)
  {
    //mL1Seed[L1name].firecounts_w+=ev_puweight;
    mL1Seed[L1name].firecounts+=ev_puweight;
    mL1Seed[L1name].uwfirecounts++;
    FireSeed.insert(L1name);
  }

  return true;
} // -----  end of function PreColumn::InsertInMenu(std::string L1name, bool value, float pu)  -----


// ===  FUNCTION  ============================================================
//         Name:  PreColumn::EventReset
//  Description:  
// ===========================================================================
bool PreColumn::EventReset()
{
  FireSeed.clear();
  FiredPhy.clear();
  return true;
}       // -----  end of function PreColumn::EventReset  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::CheckCorrelation
//  Description:  
// ===========================================================================
bool PreColumn::CheckCorrelation()
{
  if (FireSeed.size() > 0)
    nFireevents++;

  CheckPhysFire();
  CheckPureFire();
  return true;
}       // -----  end of function PreColumn::CheckCorrelation  -----

// Reweighting procedure: information about the weight corresponding to a given pileup needed for the counting
bool PreColumn::CheckCorrelation(float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  float ev_puweight = -1;
  ev_puweight = ExtractPileUpWeight(pu, reweight_2018, reweight_Run3, custom_weights);

  if (FireSeed.size() > 0)
    {
      //nFireevents_w += ev_puweight;
      nFireevents += ev_puweight;
    }

  CheckPhysFire(pu, reweight_2018, reweight_Run3, custom_weights);
  CheckPureFire(pu, reweight_2018, reweight_Run3, custom_weights);
  return true;
}       // -----  end of function PreColumn::CheckCorrelation(float pu)  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::CheckPureFire
//  Description:  
// ===========================================================================
bool PreColumn::CheckPureFire() 
{
//**************************************************************************//
//                              Check Pure Rate                             //
//**************************************************************************//
  // Trigger path pure rate
  if (FireSeed.size() == 1) 
    mL1Seed[*(FireSeed.begin())].purecounts++;

  // POG pure rate
  std::set<std::string> POGset;
  for(auto fireit : FireSeed)
  {
    for(auto &pog : mL1Seed[fireit].POG)
    {
      POGset.insert(pog);
    }
  }
  if (POGset.size() == 1)
  {
    PhyPureCounts[*(POGset.begin())]++;
  }

  std::set<std::string> PAGset;
  for(auto fireit : FireSeed)
  {
    for(auto &pag : mL1Seed[fireit].PAG)
    {
      PAGset.insert(pag);
    }
  }
  if (PAGset.size() == 1)
  {
    PhyPureCounts[*(PAGset.begin())]++;
  }


//**************************************************************************//
//                          Check Proportional Rate                         //
//**************************************************************************//
  // Get Proportional counts;
  for(auto i : FireSeed)
    mL1Seed[i].propcounts += 1.0 / FireSeed.size();

  for(auto i : POGset)
    PhyPropCounts[i] += 1.0 / POGset.size();

  for(auto i : PAGset)
    PhyPropCounts[i] += 1.0/ PAGset.size();
    
  return true;
}       // -----  end of function PreColumn::CheckPureFire  -----

// Reweighting procedure: information about the weight corresponding to a given pileup needed for the counting
bool PreColumn::CheckPureFire(float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  float ev_puweight = -1;
  ev_puweight = ExtractPileUpWeight(pu, reweight_2018, reweight_Run3, custom_weights);

//**************************************************************************//
//                              Check Pure Rate                             //
//**************************************************************************//
  // Trigger path pure rate
  if (FireSeed.size() == 1) 
    mL1Seed[*(FireSeed.begin())].purecounts+=ev_puweight;

  // POG pure rate
  std::set<std::string> POGset;
  for(auto fireit : FireSeed)
  {
    for(auto &pog : mL1Seed[fireit].POG)
    {
      POGset.insert(pog);
    }
  }
  if (POGset.size() == 1)
  {
    PhyPureCounts[*(POGset.begin())]+=ev_puweight;
  }

  std::set<std::string> PAGset;
  for(auto fireit : FireSeed)
  {
    for(auto &pag : mL1Seed[fireit].PAG)
    {
      PAGset.insert(pag);
    }
  }
  if (PAGset.size() == 1)
  {
    PhyPureCounts[*(PAGset.begin())]+=ev_puweight;
  }


//**************************************************************************//
//                          Check Proportional Rate                         //
//**************************************************************************//
  // Get Proportional counts;
  for(auto i : FireSeed)
    mL1Seed[i].propcounts += ev_puweight / FireSeed.size();

  for(auto i : POGset)
    PhyPropCounts[i] += ev_puweight / POGset.size();

  for(auto i : PAGset)
    PhyPropCounts[i] += ev_puweight / PAGset.size();
    
  return true;
}       // -----  end of function PreColumn::CheckPureFire  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::CheckPhysFire
//  Description:  
// ===========================================================================
bool PreColumn::CheckPhysFire()
{
  std::set<std::string> eventPOG;
  std::set<std::string> eventPAG;

  for(auto fired : FireSeed)
  {
    L1Seed &seed = mL1Seed[fired];
    //if (writefiles)
      //*outfile <<  event_->run <<","<<event_->lumi<<"," <<event_->event<<","<<seed.name << std::endl;
    for(auto pog : seed.POG)
    {
      if (FiredPhy.insert(pog).second) 
      {
        eventPOG.insert(pog);
        PhyCounts[pog]++;
      }
    }
    for(auto pag : seed.PAG)
    {
      if (FiredPhy.insert(pag).second) 
      {
        eventPAG.insert(pag);
        PhyCounts[pag]++;
      }
    }
  }

  Fill2DCorrelations("cor_Seeds", FireSeed);
  Fill2DCorrelations("cor_Block", eventPOG);
  Fill2DCorrelations("cor_PAGS", eventPAG);
  return true;
}       // -----  end of function PreColumn::CheckPhysFire  -----

// Reweighting procedure: information about the weight corresponding to a given pileup needed for the counting
bool PreColumn::CheckPhysFire(float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  float ev_puweight = -1;
  ev_puweight = ExtractPileUpWeight(pu, reweight_2018, reweight_Run3, custom_weights);
  std::set<std::string> eventPOG;
  std::set<std::string> eventPAG;

  for(auto fired : FireSeed)
  {
    L1Seed &seed = mL1Seed[fired];
    //if (writefiles)
      //*outfile <<  event_->run <<","<<event_->lumi<<"," <<event_->event<<","<<seed.name << std::endl;
    for(auto pog : seed.POG)
    {
      if (FiredPhy.insert(pog).second) 
      {
        eventPOG.insert(pog);
        PhyCounts[pog]+=ev_puweight;
      }
    }
    for(auto pag : seed.PAG)
    {
      if (FiredPhy.insert(pag).second) 
      {
        eventPAG.insert(pag);
        PhyCounts[pag]+=ev_puweight;
      }
    }
  }

  Fill2DCorrelations("cor_Seeds", FireSeed);
  Fill2DCorrelations("cor_Block", eventPOG);
  Fill2DCorrelations("cor_PAGS", eventPAG);
  return true;
}       // -----  end of function PreColumn::CheckPhysFire(float pu)  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::BookHistogram
//  Description:  
// ===========================================================================
bool PreColumn::BookHistogram()
{
  Int_t Nbin_max = 50;
  const Int_t NPAGS = 7;
  const Int_t NTRIGPHYS = 6;
  const Int_t N128 = 128;			// could be > 128 for "test seeds"

  HistMap["Cross"]           = new TH1F("h_Cross","h_Cross",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["MultiCross"]      = new TH1F("h_MultiCross","h_MultiCross",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["Sums"]            = new TH1F("h_Sums","h_Sums",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["Jets"]            = new TH1F("h_Jets","h_Jets",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["MultiJets"]       = new TH1F("h_MultiJets","h_MultiJets",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["EG"]             = new TH1F("h_Egamma","h_Egamma",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["MultiEG"]     = new TH1F("h_MultiEgamma","h_MultiEgamma",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["Muons"]           = new TH1F("h_Muons","h_Muons",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["MultiMuons"]      = new TH1F("h_MultiMuons","h_MultiMuons",Nbin_max,-0.5,(float)Nbin_max-0.5);
  HistMap["Technical"]       = new TH1F("h_Technical","h_Technical",Nbin_max,-0.5,(float)Nbin_max-0.5);

  HistMap["Block"]           = new TH1F("h_Block","h_Block",11,-0.5,10.5);

  HistMap["PAGS_pure"]       = new TH1F("h_PAGS_pure","h_PAGS_pure",NPAGS,-0.5,(float)NPAGS-0.5);
  HistMap["PAGS_shared"]     = new TH1F("h_PAGS_shared","h_PAGS_shared",NPAGS,-0.5,(float)NPAGS-0.5);

  HistMap["TRIGPHYS_pure"]   = new TH1F("h_TRIGPHYS_pure","h_TRIGPHYS_pure",NTRIGPHYS,-0.5,(float)NTRIGPHYS-0.5);
  HistMap["TRIGPHYS_shared"] = new TH1F("h_TRIGPHYS_shared","h_TRIGPHYS_shared",NTRIGPHYS,-0.5,(float)NTRIGPHYS-0.5);

  HistMap["All"]             = new TH1F("h_All","h_All",N128,-0.5,N128-0.5);
  HistMap["Pure"]           = new TH1F("h_Pure","h_Pure",N128,-0.5,N128-0.5);

  // Correlation among L1Seeds
  Hist2D["cor_Seeds"]       = new TH2F("cor_Seeds","cor_Seeds", vL1Seed.size(), 0, vL1Seed.size(), vL1Seed.size(), 0, vL1Seed.size());
  for (unsigned int i = 0; i < vL1Seed.size(); ++i)
  {
    int bin = Hist2D["cor_Seeds"]->GetXaxis()->FindBin(i);
    Hist2D["cor_Seeds"]->GetXaxis()->SetBinLabel(bin, vL1Seed.at(i).c_str());
    Hist2D["cor_Seeds"]->GetYaxis()->SetBinLabel(bin, vL1Seed.at(i).c_str());
  }

  // Correlation among L1Seed type/Block/POG
  Hist2D["cor_Block"]       = new TH2F("cor_Block","cor_Block", POGMap.size(), 0, POGMap.size(), POGMap.size(), 0, POGMap.size());
  for (unsigned int i = 0; i < POGMap.size(); ++i)
  {
    std::map<std::string, std::vector<int> >::iterator mapit = POGMap.begin();
    std::advance(mapit, i);
    std::string key = mapit-> first;
    int bin = Hist2D["cor_Block"]->GetXaxis()->FindBin(i);
    Hist2D["cor_Block"]->GetXaxis()->SetBinLabel(bin, key.c_str());
    Hist2D["cor_Block"]->GetYaxis()->SetBinLabel(bin, key.c_str());
  }

  // Correlation among L1Seeds PAGs
  Hist2D["cor_PAGS"]        = new TH2F("cor_PAGS","cor_PAGS", PAGMap.size(), 0, PAGMap.size(), PAGMap.size(), 0, PAGMap.size());
  for (unsigned int i = 0; i < PAGMap.size(); ++i)
  {
    std::map<std::string, std::vector<int> >::iterator mapit = PAGMap.begin();
    std::advance(mapit, i);
    std::string key = mapit-> first;
    int bin = Hist2D["cor_PAGS"]->GetXaxis()->FindBin(i);
    Hist2D["cor_PAGS"]->GetXaxis()->SetBinLabel(bin, key.c_str());
    Hist2D["cor_PAGS"]->GetYaxis()->SetBinLabel(bin, key.c_str());
  }
  return true;
}       // -----  end of function PreColumn::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::WriteHistogram
//  Description:  
// ===========================================================================
bool PreColumn::WriteHistogram(TFile *outrootfile) 
{
  outrootfile->cd();

  std::string foldername = "Col" + std::to_string(ColIdx);
  outrootfile->mkdir(foldername.c_str());
  outrootfile->cd(foldername.c_str());
  for(auto h : HistMap)
  {
    h.second->Write();
  }
  for(auto h : Hist2D)
  {
    h.second->Write();
  }
  outrootfile->cd();

  return true;
}       // -----  end of function PreColumn::WriteHistogram  -----

// ===============================================================================                                                                   
//         Name:  PreColumn::ExtractPileUpWeight                                                                                                                     
//  Description:  Extract weights for reweighting of the pileup distribution in MC                                                                                  
// ===============================================================================                                                                         
// Reweighting procedure: information about the weight corresponding to a given pileup needed for the counting
float PreColumn::ExtractPileUpWeight(float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  double weight = -1;
  // WEIGHTS obtained as the ratio between the 2018 pileup profile and the Run 3 MC nPV_True distribution:               
  // see here -> https://elfontan.web.cern.ch/elfontan/Run3_MENU/PileupReweighting/weights_nPV_True.png.                           
  // A weight equal to 0 is set in bins where the number of events in data is less than 100. 

  // 2018 weights                                                                                                                                                                  
  if (reweight_2018)
    {
      h_PUweights = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.7805557154345164, 1.8060490539875227, 1.8429444310772023, 1.8847969284653239, 1.9023685309193448, 1.9237248769457562, 1.9052231983237566, 1.9323838791307304, 1.9085059563417344, 1.8603791701197574, 1.7603108386627149, 1.6756647663900748, 1.5370617605401784, 1.3999688210755514, 1.259781632746541, 1.113578483437248, 0.9458259616433159, 0.8004237934065365, 0.67859967863879, 0.5416839609316458, 0.4312760960624466, 0.3410534695481281, 0.2670644366287844, 0.20005146208632094, 0.15028893320205652, 0.10953206021410954, 0.08057347625153384, 0.05889674651099118, 0.04287951951844378, 0.03136484854119283, 0.022045797907650135, 0.015338620880659437, 0.011042294804776033, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
  // Run 3 LumiPOG weights                                                                                                                                  
  else if (reweight_Run3)
    {
      h_PUweights = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.11768066610316973, 0.15659943608123963, 0.2062485504625545, 0.268258220098052, 0.33984423319498697, 0.42640469375359685, 0.5188836993090095, 0.6415574459968488, 0.7679292883090271, 0.9038989080521145, 1.0312039036982403, 1.184185989240607, 1.3135745255235602, 1.452683767877649, 1.595818724793107, 1.7332340493078218, 1.8220240159036707, 1.9232725225809801, 2.0500456898039943, 2.0735607093466237, 2.107312361581553, 2.1410569727220117, 2.1657353540686484, 2.1041608280359783, 2.055486270892838, 1.9497829996617755, 1.865648229655712, 1.770365292692509, 1.6681387392525582, 1.573366538275714, 1.4204671249736036, 1.2648609312008994, 1.1619530797268465, 1.032957960295197, 0.9106977649201128, 0.8043353997543611, 0.7051914455399683, 0.5998090800948392, 0.522684322280154, 0.43992380960221283, 0.36786673008123166, 0.31065622561951234, 0.2568458813937691, 0.20984708112111544, 0.1707654623586224, 0.13792878579949355, 0.11263581096823445, 0.08921848542040954, 0.07125932229597162, 0.05617010906326074, 0.04314936365485068, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
  else if (custom_weights)
  {
    if (pu < customPUweights->size())
    {
      return (*customPUweights)[pu];
    }
    else
    {
      return 0;
    }
  }

  weight = h_PUweights.at(pu);  
  return weight;
}
// -----  end of function PreColumn::ExtractPileUpWeight  -----   

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::FillPileUpSec
//  Description:  
// ===========================================================================
bool PreColumn::FillPileUpSec(float pu, bool reweight_2018, bool reweight_Run3, bool custom_weights)
{
  bool eFired = false;
  bool reweight = false;
  float ev_puweight = -1;

  // Reweighting procedure: info about the pileup of the event and the corresponding weight needed for the counting;
  // reweight label set to true to allow different event counting 
  if (reweight_2018 || reweight_Run3 || custom_weights)
    {
      reweight = true;
      ev_puweight = ExtractPileUpWeight(pu, reweight_2018, reweight_Run3, custom_weights);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ L1Seed ~~~~~
  for(auto l1 : mL1Seed)
  {
    if(L1PUCount[l1.first].find(pu) == L1PUCount[l1.first].end())
    {
      L1PUCount[l1.first][pu] = 0;
    }
    if (l1.second.eventfire)
    {
      eFired= true;
      // Reweighting procedure
      if (reweight) 
	L1PUCount[l1.first][pu] += ev_puweight;
      else
	L1PUCount[l1.first][pu]++;
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ POG ~~~~~
  std::set<std::string> POGset;
  for(auto& pog : POGMap)
  {
    std::string l1pog = "L1T_"+pog.first;
    std::string l1pogpure = "L1T_Pure"+pog.first;
    if(L1PUCount[l1pog].find(pu) == L1PUCount[l1pog].end())
    {
      L1PUCount[l1pog][pu] = 0;
    }
    if(L1PUCount[l1pogpure].find(pu) == L1PUCount[l1pogpure].end())
    {
      L1PUCount[l1pogpure][pu] = 0;
    }

    if (FiredPhy.find(pog.first) != FiredPhy.end())
    {
      // Reweighting procedure
      if (reweight) 
	L1PUCount[l1pog][pu] += ev_puweight; 
      else
	L1PUCount[l1pog][pu] ++;
      POGset.insert(pog.first);
    }
  }
  if (POGset.size() == 1)
  {
    std::string l1pogpure = "L1T_Pure"+*(POGset.begin());
    if (reweight) 
      L1PUCount[l1pogpure][pu] += ev_puweight; // Reweighting procedure 
    else
      L1PUCount[l1pogpure][pu]++;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PAG ~~~~~
  std::set<std::string> PAGset;
  for(auto& pag : PAGMap)
  {
    std::string l1pag = "L1A_"+pag.first;
    std::string l1pagpure = "L1A_Pure"+pag.first;
    if(L1PUCount[l1pag].find(pu) == L1PUCount[l1pag].end())
    {
      L1PUCount[l1pag][pu] = 0;
    }
    if(L1PUCount[l1pagpure].find(pu) == L1PUCount[l1pagpure].end())
    {
      L1PUCount[l1pagpure][pu] = 0;
    }
    if (FiredPhy.find(pag.first) != FiredPhy.end())
    {
      // Reweighting procedure
      if (reweight) 
	L1PUCount[l1pag][pu] += ev_puweight; 
      else
	L1PUCount[l1pag][pu] ++;
      PAGset.insert(pag.first);
    }
  }
  if (PAGset.size() == 1)
  {
    std::string l1pagpure = "L1A_Pure"+*(PAGset.begin());
    // Reweighting procedure
    if (reweight) 
      L1PUCount[l1pagpure][pu] += ev_puweight; 
    else
      L1PUCount[l1pagpure][pu]++;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Total ~~~~~
  // Reweighting procedure
  if (reweight) 
    L1PUCount["Count"][pu] += ev_puweight; 
  else
    L1PUCount["Count"][pu]++;

  if (eFired)
  {
    // Reweighting procedure
    if (reweight) 
      L1PUCount["L1APhysics"][pu] += ev_puweight; 
    else
      L1PUCount["L1APhysics"][pu]++;
  }

  return true;
}       // -----  end of function PreColumn::FillPileUpSec  -----


// ===  FUNCTION  ============================================================
//         Name:  PreColumn::PrintCSV
//  Description:  
// ===========================================================================
bool PreColumn::PrintCSV(std::vector<std::string> &out, double scale)
{
  std::vector<std::string>::iterator csvout = out.begin();
  float totalrate = 0.;
  float totalpurerate = 0.;
  float totalproprate = 0.;

  std::stringstream ss;
  ss << "pre-scale"<<ColIdx
    << "," << "rate"<<ColIdx
    << "," << "error_rate"<<ColIdx
    << "," << "pure"<<ColIdx
    << "," << "propotional"<<ColIdx<<",";
    //<< "," << "Comments"<<ColIdx<<",";
  csvout++->append(ss.str());

  // L1Seeds
  for(auto sed : vL1Seed)
  {
    auto seed = mL1Seed[sed];
    ss.str("");
    ss<< seed.prescale
      << "," << seed.firerate
      << "," << seed.firerateerror
      << "," << seed.purerate     
      << "," << seed.proprate      <<",";
      //<< ",\""<<seed.second.comment<<"\""<<",";
    csvout++->append(ss.str());
    totalrate += seed.firerate;
    totalpurerate += seed.purerate;
    totalproprate += seed.proprate;
  }
  
  // POG
  csvout++;
  for(auto pog : POGMap)
  {
    ss.str("");
    ss <<"1,"<< PhyCounts[pog.first]           *scale / 1000.
      <<","<< sqrt(PhyCounts[pog.first])     *scale / 1000.
      <<","<< PhyPureCounts[pog.first]       *scale / 1000.
      <<","<< PhyPropCounts[pog.first]       *scale / 1000.
      //<<","<< sqrt(PhyPureCounts[pog.first]) *scale / 1000.
      <<",";
    csvout++->append(ss.str());
  }

  // PAG
  csvout++;
  for(auto pag : PAGMap)
  {
    ss.str("");
    ss <<"1,"<< PhyCounts[pag.first]           *scale / 1000.
      <<","<< sqrt(PhyCounts[pag.first])     *scale / 1000.
      <<","<< PhyPureCounts[pag.first]       *scale / 1000.
      <<","<< PhyPropCounts[pag.first]       *scale / 1000.
      //<<","<< sqrt(PhyPureCounts[pog.first]) *scale / 1000.
      <<",";
    csvout++->append(ss.str());
  }

  // Total
  csvout++;
  ss.str("");
  //ss <<"1,"<< nFireevents_w / 1000 * scale 
  ss <<"1,"<< nFireevents / 1000 * scale 
    <<","<<sqrt(nFireevents) * scale / 1000 
    <<","<<totalpurerate / 1000 
    <<","<<totalproprate / 1000 <<",";
  csvout++->append(ss.str());

  return true;
}       // -----  end of function PreColumn::PrintCSV  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::PrintPUCSV
//  Description:  
// ===========================================================================
bool PreColumn::PrintPUCSV( std::vector<std::string> &csvout)
{
  bool isFirst= csvout.empty();
  std::stringstream ss;

  if (isFirst)
  {
    ss.str("");
    ss << "L1Seed"<< ",PileUp,Total,Prescale"<<ColIdx <<",Fired"<<ColIdx<<",";
    csvout.push_back(ss.str());

    for(auto &name_ : L1PUCount)
    {
      auto name = name_.first;
      auto l1seed = name_.second;
      for(auto pu : l1seed)
      {
        int prescale = mL1Seed.find(name) != mL1Seed.end() ?  mL1Seed.at(name).prescale : 1;
        ss.str("");
        ss<<name<<","<< pu.first <<","
          <<L1PUCount["Count"][pu.first]<<","
          <<prescale<<","
          <<pu.second<<",";
        csvout.push_back(ss.str());
      }
    }
  }
  else
  {
    std::vector<std::string>::iterator ctit = csvout.begin();

    ss.str("");
    ss << "Prescale"<<ColIdx <<",Fired"<<ColIdx<<",";
    ctit++->append(ss.str());

    for(auto &name_ : L1PUCount)
    {
      auto name = name_.first;
      auto l1seed = name_.second;
      for(auto pu : l1seed)
      {
        int prescale = mL1Seed.find(name) != mL1Seed.end() ?  mL1Seed.at(name).prescale : 1;
        ss.str("");
        ss<<prescale<<"," <<pu.second<<",";
        ctit++->append(ss.str());
      }
    }
  }

  return true;
}       // -----  end of function PreColumn::PrintPUCSV  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::PrintRates
//  Description:  
// ===========================================================================
bool PreColumn::PrintRates(std::ostream &out, double scale)
{
  float totalrate = 0.;
  float totalpurerate = 0.;
  float totalproprate = 0.;
  std::size_t L1NameLength = 0;
  for(auto k : mL1Seed)
  {
    L1NameLength = k.first.size() > L1NameLength ? k.first.size() : L1NameLength;
  }

  out << std::endl;
  out << "========================== Column " << ColIdx << " ========================== "<<std::endl;
  out << std::endl;

  out << std::left
      << std::setw(10)             << "L1Bit"
      << std::setw(L1NameLength+2) << "L1SeedName"
      << std::setw(10)             << "pre-scale"
      << std::setw(10)             << "firecounts"
      << std::setw(10)             << "rate@13TeV"       << " +/- "
      << std::setw(20)             << "error_rate@13TeV"
      << std::setw(15)             << "pure@13TeV"       
      << std::setw(15)             << "prop@13TeV"       
      << std::setw(15)             << "passevts@13TeV"
      << "Comments"
      << std::endl;

  if (bybit)
  {
    for(auto i : BitMap)
    {
      auto seed = mL1Seed[i.second];
      out << std::left
          << std::setw(10)             << seed.bit
          << std::setw(L1NameLength+2) << seed.name
          << std::setw(10)             << seed.prescale
          << std::setw(10)             << seed.firecounts
          << std::setw(10)             << seed.firerate      << " +/- "
          << std::setw(20)             << seed.firerateerror
          << std::setw(15)             << seed.purerate      
          << std::setw(15)             << seed.proprate
          << std::setw(15)             << seed.uwfirecounts
          << seed.comment
          << std::endl;
      totalrate +=seed.firerate;
      totalpurerate +=seed.purerate;
      totalproprate +=seed.proprate;
    }
  }
  else{
    for(auto name : vL1Seed)
    {
      auto seed = mL1Seed[name];
      out << std::left
          << std::setw(10)             << seed.bit
          << std::setw(L1NameLength+2) << seed.name
          << std::setw(10)             << seed.prescale
          << std::setw(10)             << seed.firecounts
          << std::setw(10)             << seed.firerate      << " +/- "
          << std::setw(20)             << seed.firerateerror
          << std::setw(15)             << seed.purerate      
          << std::setw(15)             << seed.proprate
          << std::setw(15)             << seed.uwfirecounts
          << seed.comment
          << std::endl;
      totalrate +=seed.firerate;
      totalpurerate +=seed.purerate;
      totalproprate +=seed.proprate;
    }

  }


  //out << std::endl << "Total rate  = " << nFireevents_w / 1000 * scale 
  out << std::endl << "Total rate  = " << nFireevents / 1000 * scale 
    <<" +/- " << sqrt(nFireevents) * scale / 1000 << " (kHz)" << std::endl;
  out << std::endl << "Total rate (with overlaps) = " << totalrate / 1000 << " (kHz)" << std::endl;
  out << std::endl << "Total pure rate  = " << totalpurerate / 1000 <<" (kHz)" << std::endl;
  out << std::endl << "Total prop rate  = " << totalproprate / 1000 <<" (kHz)" << std::endl;
  return true;
}       // -----  end of function PreColumn::PrintRates  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::FillDefHist1D
//  Description:  Fill in the redantant Histogram, for comparison with the old
//  code
// ===========================================================================
bool PreColumn::FillDefHist1D(double &scale)
{
  for(auto pog : POGMap)
  {
    if (HistMap.find(pog.first) == HistMap.end())
      continue;
    //Sort by bit
    std::sort( pog.second.begin(), pog.second.end());
    int binidx =1;
    for(auto l1bit : pog.second)
    {
      std::string l1name = BitMap[l1bit];
      std::cout << l1bit << " " << l1name<< std::endl;
      HistMap[pog.first]->GetXaxis()->SetBinLabel(binidx, l1name.c_str());
      //HistMap[pog.first]->SetBinContent(binidx, mL1Seed[l1name].firecounts_w);
      HistMap[pog.first]->SetBinContent(binidx, mL1Seed[l1name].firecounts);
      binidx++;
    }
    //assert(binidx == pog.second.size());
    CorrectScale(HistMap[pog.first], scale);
  }

  return true;
}       // -----  end of function PreColumn::FillDefHist1D  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::FillDefHist2D
//  Description:  
// ===========================================================================
bool PreColumn::FillDefHist2D(double &scale)
{
  for(auto h2d : Hist2D)
  {
    CorrectScale(h2d.second, scale);
  }
  return true;
}       // -----  end of function PreColumn::FillDefHist2D  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::Fill2DCorrelations
//  Description:  
// ===========================================================================
bool PreColumn::Fill2DCorrelations(const std::string &histname, std::set<std::string> &event) const
{

  for(auto fire1 : event)
  {
    for(auto fire2 : event)
    {
      Hist2D.at(histname)->Fill(fire1.c_str(), fire2.c_str(), 1);
    }
  }
  return true;
}       // -----  end of function PreColumn::Fill2DCorrelations  -----


void PreColumn::CorrectScale(TH1* h, Float_t scal) {

  Int_t nbins = h -> GetNbinsX();

  for (Int_t i=1; i<= nbins; i++)  {
    Float_t val = h -> GetBinContent(i);
    Float_t er = sqrt(val);
    val = val * scal;
    er = er * scal;
    h -> SetBinContent(i,val);
    h -> SetBinError(i,er);
  }
}

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::CalRate
//  Description:  
// ===========================================================================
bool PreColumn::CalRate(double scale)
{
  for(auto &seed : mL1Seed)
  {
    seed.second.firerate = seed.second.firecounts *scale;
    seed.second.firerateerror = sqrt(seed.second.firecounts)*scale;
    seed.second.purerate = seed.second.purecounts *scale;
    seed.second.proprate = seed.second.propcounts *scale;
  }
  return true;
}       // -----  end of function PreColumn::CalRate  -----

// ===  FUNCTION  ============================================================
//         Name:  PreColumn::PrintMenuRate
//  Description:  
// ===========================================================================
bool PreColumn::PrintMenuRate(double scale) const
{
  std::cout << "Col" << ColIdx << ": Menu rate  = " << nFireevents / 1000 * scale 
    <<" +/- " << sqrt(nFireevents) * scale / 1000 << " (kHz)" << std::endl;
  return true;
}       // -----  end of function PreColumn::PrintMenuRate  -----
