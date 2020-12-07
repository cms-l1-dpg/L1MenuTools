{#
 # @author: Takashi MATSUSHITA
 #}
{% block ObjectTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set objects = cond.getObjects() %}
{% set nObjects = objects | length %}

{% set etaScale = scaleMap | getScale(objects[0], tmGrammar.ETA) %}
{% set nEtaBits = etaScale.getNbits() %}
{% set prefix = objects[0] | getPrefix  %}


bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // collect candidates of same BX offset
  size_t nobj = 0;
  std::vector<int> candidates;
  for (size_t ii = 0; ii < data->{{prefix}}Bx.size(); ii++)
  {
    if (not (data->{{prefix}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj++;
{% if prefix | isTau %}
    if (nobj > {{macros.getMaxTaus()}}) break;
{% endif %}
{# same indexing for all objects #}
{{ macros.checkObjectIndex(objects[0], 'nobj') }}
    candidates.emplace_back(ii);
  }

  bool pass = false;

  // sufficient candidates found?
  if (candidates.size() < {{nObjects}}) return pass;

  const auto& combination = CombinationFactory::get(candidates.size(), {{nObjects}});
  const auto& permutation = PermutationFactory::get({{nObjects}});

  // match combinations
  for (const auto& set: combination)
  {
    for (const auto& indicies: permutation)
    {
      int idx = -1;
{% for kk in range(nObjects) %}
      idx = candidates.at(set.at(indicies.at({{kk}})));
{{ macros.getObjectCuts(prefix, 'idx', objects[kk], tmEventSetup, nEtaBits) }}
{% endfor %}
      {{ cond.getCuts() | chkChgCor(prefix, nObjects) }}
      pass = true;
      break;
    }

    if (pass) break;
  }

  return pass;
}
{% endblock ObjectTemplate %}
{# eof #}
