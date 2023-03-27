/* automatically generated from {{ menu.getName() }} with menu2lib.py */
/* https://github.com/cms-l1-dpg/L1MenuTools */

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
  static const int64_t cos_coeff[72] = {1023, 1019, 1007, 988, 961, 927, 886, 838, 784, 723, 658, 587, 512, 432, 350, 265, 178, 89, 0, -89, -178, -265, -350, -432, -512, -587, -658, -723, -784, -838, -886, -927, -961, -988, -1007, -1019, -1023, -1019, -1007, -988, -961, -927, -886, -838, -784, -723, -658, -587, -512, -432, -350, -265, -178, -89, 0, 89, 178, 265, 350, 432, 511, 587, 658, 723, 784, 838, 886, 927, 961, 988, 1007, 1019};
  static const int64_t sin_coeff[72] = {0, 89, 178, 265, 350, 432, 512, 587, 658, 723, 784, 838, 886, 927, 961, 988, 1007, 1019, 1023, 1019, 1007, 988, 961, 927, 886, 838, 784, 723, 658, 587, 512, 432, 350, 265, 178, 89, 0, -89, -178, -265, -350, -432, -512, -587, -658, -723, -784, -838, -886, -927, -961, -988, -1007, -1019, -1023, -1019, -1007, -988, -961, -927, -886, -838, -784, -723, -658, -587, -512, -432, -350, -265, -178, -89};

  if (not calo_tower) return {-1., -9999.};

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
        ex += (et * cos_coeff[index] / 1024.);
        ey += (et * sin_coeff[index] / 1024.);
      }
    }
  }

  return {sqrt(ex*ex + ey*ey), atan2(-ey, -ex)};
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

  mt = sqrt(2. * eg_et * met_et * (1. - cos(delta_phi)));
  return mt;
}


// utility factories

const CombinationFactory::data_t& CombinationFactory::get(const size_t n, const size_t k)
{
  const auto rc = cache_.find(std::make_pair(n, k));
  if (rc != cache_.end())
    return rc->second;
  return insert(n, k);
}


void CombinationFactory::clear()
{
  cache_.clear();
}


const CombinationFactory::data_t& CombinationFactory::insert(const size_t n, const size_t k)
{
  data_t v;

  std::string bitmask(k, 1);
  bitmask.resize(n, 0);

  do
  {
    std::vector<size_t> set;
    set.reserve(n);
    for (size_t ii = 0; ii < n; ++ii)
    {
      if (bitmask[ii])
      {
        set.emplace_back(ii);
      }
    }
    v.emplace_back(set);
  }
  while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  const auto key = std::make_pair(n, k);
  cache_.emplace(key, v);
  return cache_.at(key);
}


CombinationFactory::cache_t CombinationFactory::cache_ = {};


const PermutationFactory::data_t& PermutationFactory::get(const size_t n)
{
  const auto rc = cache_.find(n);
  if (rc != cache_.end())
    return rc->second;
  return insert(n);
}


void PermutationFactory::clear()
{
  cache_.clear();
}


const PermutationFactory::data_t& PermutationFactory::insert(const size_t n)
{
  data_t v;

  std::vector<size_t> indicies(n);
  for (size_t ii = 0; ii < n; ++ii)
  {
    indicies[ii] = ii;
  }

  do
  {
    std::vector<size_t> set;
    set.reserve(n);
    for (size_t ii = 0; ii < n; ++ii)
    {
      set.emplace_back(indicies.at(ii));
    }
    v.emplace_back(set);
  }
  while (std::next_permutation(indicies.begin(), indicies.end()));

  cache_.emplace(n, v);
  return cache_.at(n);
}


PermutationFactory::cache_t PermutationFactory::cache_ = {};


{#
 # jinja templates
 #}
{% set scaleMap = menu.getScaleMapPtr() %}


//
// NB: tmEventSetup.XxxWithOverlapRemoval was removed between utm-overlapRemoval-xsd330 and utm_0.6.5
//
/////////////////////////
// Generate conditions //
/////////////////////////

{% set OverlapRemovalConditionTypes = (
  tmEventSetup.CaloCaloCorrelationOvRm,
  tmEventSetup.DoubleJetOvRm,
  tmEventSetup.DoubleTauOvRm,
  tmEventSetup.InvariantMassOvRm,
) %}

{% set ObjectConditionTypes = (
  tmEventSetup.SingleMuon,
  tmEventSetup.DoubleMuon,
  tmEventSetup.TripleMuon,
  tmEventSetup.QuadMuon,
  tmEventSetup.SingleEgamma,
  tmEventSetup.DoubleEgamma,
  tmEventSetup.TripleEgamma,
  tmEventSetup.QuadEgamma,
  tmEventSetup.SingleTau,
  tmEventSetup.DoubleTau,
  tmEventSetup.TripleTau,
  tmEventSetup.QuadTau,
  tmEventSetup.SingleJet,
  tmEventSetup.DoubleJet,
  tmEventSetup.TripleJet,
  tmEventSetup.QuadJet,
) %}

{% set EsumConditionTypes = (
  tmEventSetup.TotalEt,
  tmEventSetup.TotalHt,
  tmEventSetup.MissingEt,
  tmEventSetup.MissingHt,
  tmEventSetup.TotalEtEM,
  tmEventSetup.MissingEtHF,
  tmEventSetup.MinBiasHFP0,
  tmEventSetup.MinBiasHFP1,
  tmEventSetup.MinBiasHFM0,
  tmEventSetup.MinBiasHFM1,
  tmEventSetup.TowerCount,
) %}

{% set AsymConditionTypes = (
  tmEventSetup.AsymmetryEt,
  tmEventSetup.AsymmetryHt,
  tmEventSetup.AsymmetryEtHF,
  tmEventSetup.AsymmetryHtHF,
) %}

{% set ExternalConditionTypes = (
  tmEventSetup.Externals,
) %}

{% set EsumMtConditionTypes = (
  tmEventSetup.TransverseMass,
  tmEventSetup.TransverseMassOvRm,
) %}

{% set Muon3CorrelationConditionTypes = (
  tmEventSetup.InvariantMass3,
) %}

{% set MuonShowerConditionTypes = (
  tmEventSetup.MuonShower0,
  tmEventSetup.MuonShower1,
  tmEventSetup.MuonShower2,
  tmEventSetup.MuonShowerOutOfTime0,
  tmEventSetup.MuonShowerOutOfTime1,
) %}

{% set CentralityConditionTypes = (
  tmEventSetup.Centrality0,
  tmEventSetup.Centrality1,
  tmEventSetup.Centrality2,
  tmEventSetup.Centrality3,
  tmEventSetup.Centrality4,
  tmEventSetup.Centrality5,
  tmEventSetup.Centrality6,
  tmEventSetup.Centrality7,
) %}

{% set ZDCConditionTypes = (
  tmEventSetup.ZDCPlus,
  tmEventSetup.ZDCMinus,
) %}

{% set AnomalyDetectionTriggerConditionTypes = (
  tmEventSetup.AnomalyDetectionTrigger,
) %}

{% for name, cond in menu.getConditionMapPtr().items() %}
  {%- if cond.getType() in OverlapRemovalConditionTypes %}
    {% set overlap_removal = 1 %}
  {% else -%}
    {% set overlap_removal = 0 %}
  {% endif -%}

  {%- if cond.getType() in ObjectConditionTypes %}
    {% include 'ObjectTemplate.cc' %}

  {% elif cond.getType() in EsumConditionTypes %}
    {% include 'EsumTemplate.cc' %}

  {% elif cond.getType() in AsymConditionTypes  %}
    {% include 'AsymTemplate.cc' %}

  {% elif cond.getType() in ExternalConditionTypes %}
    {% include 'Externals.cc' %}

  {% elif cond.getType() in (tmEventSetup.MuonMuonCorrelation, ) %}
    {% include 'MuonMuonCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.MuonEsumCorrelation, tmEventSetup.CaloEsumCorrelation) %}
    {% include 'EsumCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.CaloMuonCorrelation, ) %}
    {% include 'CaloMuonCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.CaloCaloCorrelation, tmEventSetup.CaloCaloCorrelationOvRm) %}
    {% include 'CaloCaloCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.DoubleJetOvRm, tmEventSetup.DoubleTauOvRm) %}
    {% include 'CaloThreeObjectCorrelationTemplate.cc' %}

  {% elif cond.getType() in (tmEventSetup.InvariantMass, tmEventSetup.InvariantMassOvRm, tmEventSetup.InvariantMassUpt) %}
    {% set objects = cond.getObjects() %}
    {% set combination = tmEventSetup.getObjectCombination(objects[0].getType(), objects[1].getType()) %}
    {% if combination == tmEventSetup.MuonMuonCombination %}
      {% include 'MuonMuonCorrelationTemplate.cc' %}
    {% elif combination == tmEventSetup.CaloMuonCombination %}
      {% include 'CaloMuonCorrelationTemplate.cc' %}
    {% elif combination == tmEventSetup.CaloCaloCombination %}
      {% if cond.getType() == tmEventSetup.InvariantMass %}
        {% include 'CaloCaloCorrelationTemplate.cc' %}
      {% elif cond.getType() == tmEventSetup.InvariantMassOvRm %}
        {% include 'CaloThreeObjectCorrelationTemplate.cc' %}
      {% endif %}
    {% else %}
      err> unsupported object combination for correlation
    {% endif %}

  {% elif cond.getType() in EsumMtConditionTypes %}
    {% set objects = cond.getObjects() %}
    {% set combination = tmEventSetup.getObjectCombination(objects[0].getType(), objects[1].getType()) %}
    {% if combination == tmEventSetup.CaloEsumCombination %}
      {% include 'EsumMtTemplate.cc' %}
    {% else %}
      err> unsupported object combination for mT
    {% endif %}

  {% elif cond.getType() in Muon3CorrelationConditionTypes %}
    {% set objects = cond.getObjects() %}
    {% set combination01 = tmEventSetup.getObjectCombination(objects[0].getType(), objects[1].getType()) %}
    {% set combination02 = tmEventSetup.getObjectCombination(objects[0].getType(), objects[2].getType()) %}
    {% if combination01 == tmEventSetup.MuonMuonCombination and combination02 == tmEventSetup.MuonMuonCombination %}
      {% include 'Muon3CorrelationTemplate.cc' %}
    {% endif %}

  {% elif cond.getType() in MuonShowerConditionTypes %}
    {% include 'MuonShowerTemplate.cc' %}

  {% elif cond.getType() in CentralityConditionTypes %}
    {% include 'CentralityTemplate.cc' %}

  {% elif cond.getType() in ZDCConditionTypes %}
    {% include 'ZDCTemplate.cc' %}

  {% elif cond.getType() in AnomalyDetectionTriggerConditionTypes %}
    {% include 'AnomalyDetectionTriggerTemplate.cc' %}

  {% endif -%}
{% endfor %}


// generate algorithms
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
bool
{{ name }}(L1Analysis::L1AnalysisL1UpgradeDataFormat* data, L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower)
{
  return {{ algo.getExpressionInCondition() | toCpp }};
}
{% endfor %}


std::string getNameFromId(const size_t index)
{
  static const std::map<size_t, std::string> id2name = {
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
    {{ '{' }}{{ algo.getIndex() }}, "{{ name }}"{{ '}' }}{% if not loop.last %},{% endif %}

{% endfor %}
  };

  const auto rc = id2name.find(index);
  if (rc == id2name.end())
  {
    std::ostringstream oss;
    oss << "no such algorithm index: " << index << ", in menu: {{ menu.getName() }}\n";
    throw std::runtime_error(oss.str());
  }
  return rc->second;
}


int getIdFromName(const std::string& name)
{
  static const std::map<std::string, int> name2id = {
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
  {{ '{' }}"{{ name }}", {{ algo.getIndex() }}{{ '}' }}{% if not loop.last %},{% endif %}

{% endfor %}
  };

  const auto rc = name2id.find(name);
  if (rc == name2id.end())
  {
    std::ostringstream oss;
    oss << "no such algorithm name: \"" << name << "\", in menu: {{ menu.getName() }}\n";
    throw std::runtime_error(oss.str());
  }
  return rc->second;
}


AlgorithmFunction getFuncFromId(const size_t index)
{
  static const std::map<size_t, AlgorithmFunction> id2func = {
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
    {{ '{' }}{{ algo.getIndex() }}, &{{ name }}{{ '}' }}{% if not loop.last %},{% endif %}

{% endfor %}
  };

  const auto rc = id2func.find(index);
  if (rc == id2func.end())
  {
    std::ostringstream oss;
    oss << "no such algorithm index: " << index << ", in menu: {{ menu.getName() }}\n";
    throw std::runtime_error(oss.str());
  }
  return rc->second;
}


AlgorithmFunction getFuncFromName(const std::string& name)
{
  static const std::map<std::string, AlgorithmFunction> name2func = {
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
    {{ '{' }}"{{ name }}", &{{ name }}{{ '}' }}{% if not loop.last %},{% endif %}

{% endfor %}
  };

  const auto rc = name2func.find(name);
  if (rc == name2func.end())
  {
    std::ostringstream oss;
    oss << "no such algorithm name: \"" << name << "\", in menu: {{ menu.getName() }}\n";
    throw std::runtime_error(oss.str());
  }
  return rc->second;
}


bool addFuncFromName(std::map<std::string, std::function<bool()>> &L1SeedFun,
                     L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
                     L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower)
{
  static const std::map<std::string, AlgorithmFunction> name2func = {
{% for name, algo in menu.getAlgorithmMapPtr().items() %}
    {{ '{' }}"{{ name }}", &{{ name }}{{ '}' }}{% if not loop.last %},{% endif %}

{% endfor %}
  };

  for (const auto& pair : name2func)
  {
    L1SeedFun[pair.first] = std::bind(pair.second, upgrade, calo_tower);
  }

  return true;
}
// eof
