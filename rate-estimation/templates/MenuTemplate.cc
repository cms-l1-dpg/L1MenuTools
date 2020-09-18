{#
 # @author: Takashi MATSUSHITA
 #}
/* automatically generated from {{ menu.getName() }} with menu2lib.py */
/* https://gitlab.cern.ch/cms-l1t-utm/scripts */

#include <algorithm>
#include <map>
#include <string>
#include <sstream>
#include <cmath>

#include "menulib.hh"

//
// common functions for algorithm implementations
//
std::pair<double, double>
get_missing_et(L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower,
               const int max_eta,
               const double threshold)
{
  // https://github.com/cms-sw/cmssw/blob/CMSSW_9_0_X/L1Trigger/L1TCalorimeter/src/CaloTools.cc#L13=L15
  const int64_t cos_coeff[72] = {1023, 1019, 1007, 988, 961, 927, 886, 838, 784, 723, 658, 587, 512, 432, 350, 265, 178, 89, 0, -89, -178, -265, -350, -432, -512, -587, -658, -723, -784, -838, -886, -927, -961, -988, -1007, -1019, -1023, -1019, -1007, -988, -961, -927, -886, -838, -784, -723, -658, -587, -512, -432, -350, -265, -178, -89, 0, 89, 178, 265, 350, 432, 511, 587, 658, 723, 784, 838, 886, 927, 961, 988, 1007, 1019};

  const int64_t sin_coeff[72] = {0, 89, 178, 265, 350, 432, 512, 587, 658, 723, 784, 838, 886, 927, 961, 988, 1007, 1019, 1023, 1019, 1007, 988, 961, 927, 886, 838, 784, 723, 658, 587, 512, 432, 350, 265, 178, 89, 0, -89, -178, -265, -350, -432, -512, -587, -658, -723, -784, -838, -886, -927, -961, -988, -1007, -1019, -1023, -1019, -1007, -988, -961, -927, -886, -838, -784, -723, -658, -587, -512, -432, -350, -265, -178, -89};

  if (not calo_tower) return std::make_pair(-1., -9999.);

  double ex = 0.;
  double ey = 0.;

  for (int ii = 0; ii < calo_tower->nTower; ii++)
  {
    if (abs(calo_tower->ieta.at(ii)) <= max_eta)
    {
      const double et = calo_tower->iet.at(ii) * 0.5;
      if (et > threshold)
      {
        const int index = calo_tower->iphi.at(ii) - 1;
        ex += (et*cos_coeff[index]/1024.);
        ey += (et*sin_coeff[index]/1024.);
      }
    }
  }

  return std::make_pair(sqrt(ex*ex + ey*ey), atan2(-ey, -ex));
}


double
get_total_ht(L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
             const int max_eta,
             const double threshold)
{
  double sum = 0.;

  for (int ii = 0; ii < upgrade->nJets; ii++)
  {
    if (upgrade->jetBx.at(ii) != 0) continue;

    if (abs(upgrade->jetIEta.at(ii)) <= 2*max_eta)
    {
      const double et = upgrade->jetEt.at(ii);
      if (et > threshold)
      {
        sum += et;
      }
    }
  }

  return sum;
} 


double
get_transverse_mass(L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
                    const double threshold_eg,
                    const double threshold_met)
{
  double mt = -1.;

  if (upgrade->nEGs == 0) return mt;

  // leading-eg
  int id_leading_eg = -1;
  for (int ii = 0; ii < upgrade->nEGs; ii++)
  {
    if (upgrade->egBx.at(ii) != 0) continue;
    if (id_leading_eg < 0)
    {
      id_leading_eg = ii;
      break;
    }
  }

  if (id_leading_eg < 0) return mt;

  const double eg_et = upgrade->egEt.at(id_leading_eg);
  const double eg_phi = upgrade->egPhi.at(id_leading_eg);

  if (eg_et < threshold_eg) return mt;


  // missing-Et
  int id_missing_et = -1;
  for (int ii = 0; ii < upgrade->nSums; ii++)
  {
    if (upgrade->sumBx.at(ii) != 0) continue;
    if (upgrade->sumType.at(ii) == L1Analysis::kMissingEt)
    {
      id_missing_et = ii;
      break;
    }
  }

  if (id_missing_et < 0) return mt;

  const double met_et = upgrade->sumEt.at(id_missing_et);
  const double met_phi = upgrade->sumPhi.at(id_missing_et);

  if (met_et < threshold_met) return mt;


  // mt
  double delta_phi = eg_phi - met_phi;
  while (delta_phi >= M_PI) delta_phi -= 2.*M_PI;
  while (delta_phi < -M_PI) delta_phi += 2.*M_PI;

  mt = sqrt(2.*eg_et*met_et*(1. - cos(delta_phi)));
  return mt;
}


// utility methods
void
getCombination(int N,
               int K,
               std::vector<std::vector<int> >& combination)
{
  std::string bitmask(K, 1);
  bitmask.resize(N, 0);

  do
  {
    std::vector<int> set;
    for (int ii = 0; ii < N; ++ii)
    {
      if (bitmask[ii]) set.push_back(ii);
    }
    combination.push_back(set);
  }
  while (std::prev_permutation(bitmask.begin(), bitmask.end()));
}


void
getPermutation(int N,
               std::vector<std::vector<int> >& permutation)
{
  std::vector<int> indicies(N);
  for (int ii = 0; ii < N; ii++) indicies.at(ii) = ii;

  do
  {
    std::vector<int> set;
    for (int ii = 0; ii < N; ++ii)
    {
      set.push_back(indicies.at(ii));
    }
    permutation.push_back(set);
  }
  while (std::next_permutation(indicies.begin(), indicies.end()));
}


{#
 # jinja templates
 #}
{% set scaleMap = menu.getScaleMapPtr() %}


//
// NB: tmEventSetup.XxxWithOverlapRemoval was removed between utm-overlapRemoval-xsd330 and utm_0.6.5
//
// generate conditions
{% for name, cond in menu.getConditionMapPtr().iteritems() %}
  {%- set overlap_removal = 0 -%}
  {%- if cond.getType() in (tmEventSetup.MuonMuonCorrelationWithOverlapRemoval,
                           tmEventSetup.MuonEsumCorrelationWithOverlapRemoval,
                           tmEventSetup.CaloEsumCorrelationWithOverlapRemoval,
                           tmEventSetup.CaloMuonCorrelationWithOverlapRemoval,
                           tmEventSetup.CaloCaloCorrelationOvRm,
                           tmEventSetup.InvariantMassOvRm) %}
    {% set overlap_removal = 1 %}
  {% endif -%}

  {%- if cond.getType() in (tmEventSetup.SingleMuon, tmEventSetup.DoubleMuon, tmEventSetup.TripleMuon, tmEventSetup.QuadMuon,
                           tmEventSetup.SingleEgamma, tmEventSetup.DoubleEgamma, tmEventSetup.TripleEgamma, tmEventSetup.QuadEgamma,
                           tmEventSetup.SingleTau, tmEventSetup.DoubleTau, tmEventSetup.TripleTau, tmEventSetup.QuadTau,
                           tmEventSetup.SingleJet, tmEventSetup.DoubleJet, tmEventSetup.TripleJet, tmEventSetup.QuadJet) %}
    {% include 'ObjectTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.TotalEt, tmEventSetup.TotalHt, tmEventSetup.MissingEt, tmEventSetup.MissingHt,
                             tmEventSetup.TotalEtEM, tmEventSetup.MissingEtHF,
                             tmEventSetup.MinBiasHFP0, tmEventSetup.MinBiasHFP1,
                             tmEventSetup.MinBiasHFM0, tmEventSetup.MinBiasHFM1,
                             tmEventSetup.TowerCount) %}
    {% include 'EsumTemplate.cc' %}

  {% elif cond.getType() == tmEventSetup.Externals %}
    {% include 'Externals.cc' %}

  {% elif cond.getType() in (tmEventSetup.MuonMuonCorrelation, tmEventSetup.MuonMuonCorrelationWithOverlapRemoval) %}
    {% include 'MuonMuonCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.MuonEsumCorrelation, tmEventSetup.CaloEsumCorrelation, tmEventSetup.MuonEsumCorrelationWithOverlapRemoval, tmEventSetup.CaloEsumCorrelationWithOverlapRemoval) %}
    {% include 'EsumCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.CaloMuonCorrelation, tmEventSetup.CaloMuonCorrelationWithOverlapRemoval) %}
    {% include 'CaloMuonCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.CaloCaloCorrelation, tmEventSetup.CaloCaloCorrelationOvRm) %}
    {% include 'CaloCaloCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.InvariantMass, tmEventSetup.InvariantMassOvRm) %}
    {% set objects = cond.getObjects() %}
    {% set combination = tmEventSetup.getObjectCombination(objects[0].getType(), objects[1].getType()) %}
    {% if combination == tmEventSetup.MuonMuonCombination %}
      {% include 'MuonMuonCorrelationTemplate.cc' %}
    {% elif combination == tmEventSetup.CaloMuonCombination %}
      {% include 'CaloMuonCorrelationTemplate.cc' %}
    {% elif combination == tmEventSetup.CaloCaloCombination %}
      {% include 'CaloCaloCorrelationTemplate.cc' %}
    {% endif %}

  {% elif cond.getType() in (tmEventSetup.TransverseMass, tmEventSetup.TransverseMassOvRm) %}
    {% set objects = cond.getObjects() %}
    {% set combination = tmEventSetup.getObjectCombination(objects[0].getType(), objects[1].getType()) %}
    {% if combination == tmEventSetup.CaloEsumCombination %}
      {% include 'EsumMtTemplate.cc' %}
    {% else %}
      err> unsupported object combination for mT
    {% endif %}

  {% endif -%}
{% endfor %}


// generate algorithms
{% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
bool
{{ name }}(L1Analysis::L1AnalysisL1UpgradeDataFormat* data, L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower)
{
  return {{ algo.getExpressionInCondition() | toCpp }};
}
{% endfor %}


std::string getNameFromId(const int index)
{
  static const std::pair<int, std::string> id2name[] = {
    {% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
      std::make_pair({{ algo.getIndex() }}, "{{ name }}"){% if not loop.last %},{% endif %}
    {% endfor %}
  };

  static const std::map<int, std::string> Id2Name(id2name, id2name + sizeof(id2name) / sizeof(id2name[0]));
  const std::map<int, std::string>::const_iterator rc = Id2Name.find(index);
  std::string name;
  if (rc != Id2Name.end()) name = rc->second;
  return name;
}


int getIdFromName(const std::string& name)
{
  static const std::pair<std::string, int> name2id[] = {
    {% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
      std::make_pair("{{ name }}", {{ algo.getIndex() }}){% if not loop.last %},{% endif %}
    {% endfor %}
  };

  static const std::map<std::string, int> Name2Id(name2id, name2id + sizeof(name2id) / sizeof(name2id[0]));
  const std::map<std::string, int>::const_iterator rc = Name2Id.find(name);
  int id = -1;
  if (rc != Name2Id.end()) id = rc->second;
  return id;
}


AlgorithmFunction getFuncFromId(const int index)
{
  static const std::pair<int, AlgorithmFunction> id2func[] = {
    {% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
      std::make_pair({{ algo.getIndex() }}, &{{ name }}){% if not loop.last %},{% endif %}
    {% endfor %}
  };

  static const std::map<int, AlgorithmFunction> Id2Func(id2func, id2func + sizeof(id2func) / sizeof(id2func[0]));
  const std::map<int, AlgorithmFunction>::const_iterator rc = Id2Func.find(index);
  AlgorithmFunction fp = 0;
  if (rc != Id2Func.end()) fp = rc->second;
  return fp;
}


AlgorithmFunction getFuncFromName(const std::string& name)
{
  static const std::pair<std::string, AlgorithmFunction> name2func[] = {
    {% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
      std::make_pair("{{ name }}", &{{ name }}){% if not loop.last %},{% endif %}
    {% endfor %}
  };

  static const std::map<std::string, AlgorithmFunction> Name2Func(name2func, name2func + sizeof(name2func) / sizeof(name2func[0]));
  const std::map<std::string, AlgorithmFunction>::const_iterator rc = Name2Func.find(name);
  AlgorithmFunction fp = 0;
  if (rc != Name2Func.end()) fp = rc->second;
  if (fp == 0)
  {
    std::stringstream ss;
    ss << "fat> algorithm '" << name << "' is not defined in {{ menu.getName() }}\n";
    throw std::runtime_error(ss.str());
  }
  return fp;
}


bool addFuncFromName(std::map<std::string, std::function<bool()>> &L1SeedFun,
                     L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
                     L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower)
{
  static const std::pair<std::string, AlgorithmFunction> name2func[] = {
    {% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
      std::make_pair("{{ name }}", &{{ name }}){% if not loop.last %},{% endif %}
    {% endfor %}
  };

  for (auto pair : name2func)
  {
    L1SeedFun[pair.first] = std::bind(pair.second, upgrade, calo_tower);
  }

  return true;
}
// eof
