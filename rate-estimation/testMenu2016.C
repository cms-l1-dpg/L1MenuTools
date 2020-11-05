// ===========================================================================
// 
//       Filename:  testMenu2016.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/14/2016 10:04:16
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================
//
#include "include/L1Menu2016.h"

#include <cstdlib>
#include <iostream>  
#include <string>
#include <vector>

#include "TError.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// ===  FUNCTION  ============================================================
//         Name:  main
//  Description:  
// ===========================================================================
int main ( int argc, char *argv[] )
{
  namespace po = boost::program_options;
  // Declare the supported options.
  boost::program_options::options_description desc("Allowed options");
  const std::string defaultMenu = "menu/Slim2E34.txt";
  const std::string defaultntuple = "ntuple/Train_v87p3_PU55.list";
  const std::string defaultLumi = "menu/TrainPLTZ.csv";

  desc.add_options()
    ("help,h", "produce help message")
    ("menufile,m",     po::value<std::string>()->default_value(defaultMenu),   "set the input menu")
    ("filelist,l",     po::value<std::string>()->default_value(defaultntuple), "set the input ntuple list")
    ("Lumilist,u",     po::value<std::string>()->default_value(defaultLumi),   "set the input lumi list")
    ("outfilename,o",  po::value<std::string>()->default_value("Auto"),        "set output file name")
    ("outputdir,d",    po::value<std::string>()->default_value("results"),     "set output directory")
    ("writetext,t",    po::value<bool>()->default_value(true),                 "write rate to output")
    ("writecsv,c",     po::value<bool>()->default_value(true),                 "write rate to output in CSV format")
    ("writeplot,p",    po::value<bool>()->default_value(true),                 "write plot to output")
    ("doPlotRate",     po::bool_switch()->default_value(false),                "save rate plot to output")
    ("doPlotEff",      po::bool_switch()->default_value(false),                "save efficiency plot to output")
    ("doPlotTest",     po::bool_switch()->default_value(false),                "save testing plot to output")
    ("doPlotuGt",      po::bool_switch()->default_value(false),                "save uGT histograms to output")        
    ("doPlotLS",       po::bool_switch()->default_value(false),                "save count per LS plot to output")
    ("doTnPMuon",      po::bool_switch()->default_value(false),                "use tag & probe for muon efficiency")
    ("doPrintPU",      po::bool_switch()->default_value(false),                "print out rate per PU to file")
    ("doPrintBX",      po::bool_switch()->default_value(false),                "print out BX")
    ("doCompuGT",      po::bool_switch()->default_value(false),                "Compare emulation with uGT tree")
    ("doScanLS",       po::bool_switch()->default_value(false),                "Quickly scan files for selected LS")
    ("Select_BX_in_48b",     po::value<float>()->default_value(-1),            "select target bx in 48b")
    ("Select_BX_in_12b",     po::value<float>()->default_value(-1),            "select target bx in 12b")
    ("doBXReweight_1_to_6_47_48",   po::bool_switch()->default_value(false),   "select bx 1-6&47,48 in 48b")
    ("doBXReweight128",   po::bool_switch()->default_value(false),             "select bx 1,2&8 in 8b4e")
    ("doBXReweight34567",   po::bool_switch()->default_value(false),           "select bx 3-7 in 8b4e")
    ("doBXReweight_1_to_6_11_12",   po::bool_switch()->default_value(false),   "select bx 1-6&11,12 in 12b")
    ("doBXReweight_5_to_10",   po::bool_switch()->default_value(false),        "select bx 5-10 in 12b")
    ("maxEvent,n",     po::value<int>()->default_value(-1),                    "run number of events; -1 for all")
    ("nBunches,b",     po::value<float>(),                                     "set number of bunches")
    ("SumJetET",       po::value<int>(),                                       "PT threshold of Jet for HT")
    ("SumJetEta",      po::value<float>(),                                     "Eta threshold of Jet for HT")
    ("SetMuonER",      po::value<float>(),                                     "Set the ER in eta for Muon")
    ("SetL1AcceptPS",  po::value<int>()->default_value(0),                     "Set the L1Accept Prescale at HLT for this run")
    ("SetNoPrescale",  po::bool_switch()->default_value(false),                "Set all prescales to 1")
    ("IgnorePrescale", po::bool_switch()->default_value(false),                "Set all prescales of prescalsed Seeds to 0")
    ("UseUpgradeLyr1", po::bool_switch()->default_value(false),                "Use Upgrade Layer1 Tree")
    ("UseL1CaloTower", po::bool_switch()->default_value(false),                "Use Layer1 CaloTower Tree")
    ("UsePFMETNoMuon", po::bool_switch()->default_value(false),                "Use PFMET no Muon in SingleMu sample")
    ("UseuGTDecision", po::bool_switch()->default_value(false),                "Trigger seeds fired by uGT")
    ("UseUnpackTree",  po::bool_switch()->default_value(false),                "Use unpacked tree in Ntuple")
    ("SelectFill",     po::value<int>()->default_value(-1),                    "Select specific fill based on lumi list")
    ("SelectRun",      po::value<int>()->default_value(-1),                    "Select specific run")
    ("SelectEvent",    po::value<int>()->default_value(-1),                    "Select specific event")
    ("SelectLS",       po::value<std::string>()->default_value(""),            "Select specific LS ranges")
    ("SelectBX",       po::value<std::string>()->default_value(""),            "Select specific BX ranges")
    ("SelectCol",      po::value<std::string>()->default_value(""),            "Select prescale column from input csv menu")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Create output directory ~~~~~
  if (!boost::filesystem::is_directory(vm["outputdir"].as<std::string>() ))
  {
    boost::filesystem::create_directory(vm["outputdir"].as<std::string>());
  }

  // We need same histname for later on comparison
  gErrorIgnoreLevel = kFatal;

  L1Menu2016 men(vm["menufile"].as<std::string>(), vm["filelist"].as<std::string>());

  men.ConfigOutput(vm["writetext"].as<bool>(), 
      vm["writecsv"].as<bool>(), 
      vm["writeplot"].as<bool>(), 
      vm["outputdir"].as<std::string>(),
      vm["outfilename"].as<std::string>());


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Get the running time config ~~~~~
  std::map<std::string, float> L1Config;
  std::map<std::string, std::string> L1ConfigStr;
  for (const auto& it : vm) {
    if (!vm.count(it.first)) continue;
    auto& value = it.second.value();
    if (auto v = boost::any_cast<bool>(&value))
      L1Config[it.first] = *v;
    else if (auto v = boost::any_cast<int>(&value))
      L1Config[it.first] = *v;
    else if (auto v = boost::any_cast<float>(&value))
      L1Config[it.first] = *v;
    else if (auto v = boost::any_cast<std::string>(&value))
      L1ConfigStr[it.first] = *v;
  }
  men.PreLoop(L1Config, L1ConfigStr);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Start to Run ~~~~~
  men.Loop();
  men.PostLoop();

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
