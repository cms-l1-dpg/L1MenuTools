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
PreColumn::PreColumn (int ColIdx_, std::map<std::string, L1Seed> mL1Seed_):
  ColIdx(ColIdx_), mL1Seed(mL1Seed_)
{
  bybit = false;
  nFireevents = 0;
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

bool PreColumn::InsertInMenu(std::string L1name, bool value) {

  bool post_prescale = false;

  if ( mL1Seed.find(L1name) == mL1Seed.end() ) {
    std::cout << "This shouldn't happen!" << std::endl;
    return false;
  }

  mL1Seed[L1name].eventfire = false;
  if (value)
    mL1Seed[L1name].ncounts++;

  if (mL1Seed[L1name].prescale == 0)
    return false;

  if ( mL1Seed[L1name].ncounts % mL1Seed[L1name].prescale == 0) 
    post_prescale = value; 

  mL1Seed[L1name].eventfire = post_prescale;
  if (post_prescale)
  {
    mL1Seed[L1name].firecounts++;
    FireSeed.insert(L1name);
  }

  return true;
}


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
    nFireevents ++;

  CheckPhysFire();
  CheckPureFire();
  return true;
}       // -----  end of function PreColumn::CheckCorrelation  -----

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


// ===  FUNCTION  ============================================================
//         Name:  PreColumn::FillPileUpSec
//  Description:  
// ===========================================================================
bool PreColumn::FillPileUpSec(float pu)
{
  bool eFired = false;
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
      L1PUCount[l1pog][pu] ++;
      POGset.insert(pog.first);
    }
  }
  if (POGset.size() == 1)
  {
    std::string l1pogpure = "L1T_Pure"+*(POGset.begin());
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
      L1PUCount[l1pag][pu] ++;
      PAGset.insert(pag.first);
    }
  }
  if (PAGset.size() == 1)
  {
    std::string l1pagpure = "L1A_Pure"+*(PAGset.begin());
    L1PUCount[l1pagpure][pu]++;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Total ~~~~~
  L1PUCount["Count"][pu]++;
  if (eFired)
  {
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
    totalrate +=seed.firerate;
    totalpurerate +=seed.purerate;
    totalproprate +=seed.proprate;
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
      << std::setw(10)             << "rate@13TeV"       << " +/- "
      << std::setw(20)             << "error_rate@13TeV"
      << std::setw(15)             << "pure@13TeV"       
      << std::setw(15)             << "prop@13TeV"       
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
          << std::setw(10)             << seed.firerate      << " +/- "
          << std::setw(20)             << seed.firerateerror
          << std::setw(15)             << seed.purerate      
          << std::setw(15)             << seed.proprate
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
          << std::setw(10)             << seed.firerate      << " +/- "
          << std::setw(20)             << seed.firerateerror
          << std::setw(15)             << seed.purerate      
          << std::setw(15)             << seed.proprate
          << seed.comment
          << std::endl;
      totalrate +=seed.firerate;
      totalpurerate +=seed.purerate;
      totalproprate +=seed.proprate;
    }

  }


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
