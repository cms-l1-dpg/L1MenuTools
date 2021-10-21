{#
 # @author: Takashi MATSUSHITA
 #}
{% block SingleObjectOvRmTemplate scoped %}

{% import 'macros.jinja2' as macros %}

{% set objects = cond.getObjects() %}
{% if overlap_removal %}
  {% set reference = objects[1] %}
  {% set etaScaleRef = scaleMap | getScale(reference, tmGrammar.ETA) %}
  {% set nEtaBits = etaScaleRef.getNbits() %}  
{% endif %}
{% set nObjects = 1 %}

{% set prefix0 = objects[0] | getPrefix %}

{% set etaScale0 = scaleMap | getScale(objects[0], tmGrammar.ETA) %}
{% set nEtaBits0 = etaScale0.getNbits() %}
{% set phiScale0 = scaleMap | getScale(objects[0], tmGrammar.PHI) %}

{# assume phiScale is the same for calo objects #}

{% set iPi = (0.5*(phiScale0.getMaximum() - phiScale0.getMinimum())/phiScale0.getStep()) | int -%}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  {% if overlap_removal %}
    {{ macros.getReference(reference, tmEventSetup, nEtaBits) }}
  {% endif %}
  size_t nobj0 = 0;
  std::vector<int> candidates;
  for (size_t ii = 0; ii < data->{{prefix0}}Bx.size(); ii++)
  {
    if (not (data->{{prefix0}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj0++;
    {% if prefix0 | isTau %}
    if (nobj0 > {{macros.getMaxTaus()}}) break;
    {% endif %}
{{ macros.checkObjectIndex(objects[0], 'nobj0') }} {# same indexing for all objects #}
    {% if overlap_removal %}
      {{ cond | hasCorrelationCuts() }}
      {{ macros.removeOverlap(cond, objects[0], 'ii', reference, tmEventSetup, scaleMap, iPi) }}
    {% endif %}
    candidates.emplace_back(ii);
  }

  bool pass = false;
  if (candidates.size() < {{nObjects}}) return pass;

  const auto& combination = CombinationFactory::get(candidates.size(), {{nObjects}});
  const auto& permutation = PermutationFactory::get({{nObjects}});

  for (const auto& set: combination)
  {
    for (const auto& indicies: permutation)
    {
	int idx = -1;
{% for kk in range(nObjects) %}
      idx = candidates.at(set.at(indicies.at({{kk}})));
{{ macros.getObjectCuts(prefix0, 'idx', objects[kk], tmEventSetup, nEtaBits0) }}
{% endfor %}
      {{ cond.getCuts() | chkChgCor(prefix0, nObjects) }}
      pass = true;
      break;
    }

    if (pass) break;
  }

  return pass;
}

{% endblock SingleObjectOvRmTemplate %}
{# eof #}
