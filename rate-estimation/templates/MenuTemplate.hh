#ifndef menulib_hh
#define menulib_hh
{#
 # @author: Takashi MATSUSHITA
 #}
/* automatically generated from {{ menu.getName() }} with menu2lib.py */
/* https://gitlab.cern.ch/cms-l1t-utm/scripts */

#include <string>
#include <vector>
#include <functional>
#include <map>

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1CaloTowerDataFormat.h"

//
// common functions for algorithm implementations
//
/** re-calculate total-Ht with the given parameters
 *
 * @param upgrade [in] uGt inputs
 * @param max_eta [in] maximum abs(tower-IEta) to take into account for the sum
 * @param threshold [in] minimum tower energy to take into account for the sum
 * @return total-Ht
 */
double
get_total_ht(L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
             const int max_eta=28,
             const double threshold=30.);


/** compute transverse mass
 *
 * @param upgrade [in] uGt inputs
 * @param threshold_eg [in] minimum ET requirement for the leading-EG object
 * @param threshold [in] minimum missing-ET requirement
 * @return transverse mass. If Mt cannot be computed, -1.
 */
double
get_transverse_mass(L1Analysis::L1AnalysisL1UpgradeDataFormat* upgrade,
                    const double threshold_eg=30.,
                    const double threshold_met=30.);


// utility methods
void
getCombination(int N,
               int K,
               std::vector<std::vector<int> >& combination);

void
getPermutation(int N,
               std::vector<std::vector<int> >& permutation);

const long long POW10[] =
{
                      1, 
                     10,
                    100,
                   1000,
                  10000,
                 100000,
                1000000,
               10000000,
              100000000,
             1000000000,
            10000000000,
           100000000000,
          1000000000000,
         10000000000000,
        100000000000000,
       1000000000000000,
      10000000000000000,
     100000000000000000
};


// auxiliary functions
std::string getNameFromId(const int index);

int getIdFromName(const std::string& name);

typedef bool (*AlgorithmFunction)(L1Analysis::L1AnalysisL1UpgradeDataFormat*, L1Analysis::L1AnalysisL1CaloTowerDataFormat*);

AlgorithmFunction getFuncFromId(const int index);

AlgorithmFunction getFuncFromName(const std::string& name);

bool addFuncFromName(std::map<std::string, std::function<bool()>> &L1SeedFun,
                     L1Analysis::L1AnalysisL1UpgradeDataFormat* ntuple,
                     L1Analysis::L1AnalysisL1CaloTowerDataFormat* calo_tower);

// algorithms
{% for name, algo in menu.getAlgorithmMapPtr().iteritems() %}
bool
{{ name }}(L1Analysis::L1AnalysisL1UpgradeDataFormat*, L1Analysis::L1AnalysisL1CaloTowerDataFormat*);
{% endfor %}


{#
 # jinja templates
 #}
{% set scaleMap = menu.getScaleMapPtr() %}

// generate scales
{% include 'Scales.jinja2' %}

// generate LUTs
{% include 'CaloCaloLUT.jinja2' %}
{% include 'CaloEsumLUT.jinja2' %}
{% include 'CaloMuonLUT.jinja2' %}
{% include 'MuonEsumLUT.jinja2' %}
{% include 'MuonMuonLUT.jinja2' %}

#endif // menulib_hh
// eof
