{#
 # @author: Takashi MATSUSHITA
 #}
{% block MuonMuonMuonCorrelationTemplate scoped %}

{% import 'macros.jinja2' as macros %}

{% set objects = cond.getObjects() %}
{% if overlap_removal %}
  {% set reference = objects[objects|length -1] %}
{% endif %}
{% set nObjects = 3 %}

{% set etaScale = scaleMap | getScale(objects[0], tmGrammar.ETA) %}
{% set nEtaBits = etaScale.getNbits() %}
{% set phiScale = scaleMap | getScale(objects[0], tmGrammar.PHI) %}

{% set prefix = objects[0] | getPrefix %}

{% set LUTSAB = scaleMap | getLookUpTable(objects[0], objects[1]) %}
{% set LUTSAC = scaleMap | getLookUpTable(objects[0], objects[2]) %}
{% set LUTSBC = scaleMap | getLookUpTable(objects[1], objects[2]) %}

{% set iPi = (0.5*(phiScale.getMaximum() - phiScale.getMinimum())/phiScale.getStep()) | int -%}


{% if objects[0].getBxOffset() == objects[1].getBxOffset() and objects[2].getBxOffset() == objects[1].getBxOffset() %}
bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
{% if overlap_removal %}
    {{ macros.getReference(reference, tmEventSetup, nEtaBits) }}
{% endif %}
  size_t nobj = 0;
  std::vector<int> candidates;
  for (size_t ii = 0; ii < data->{{prefix}}Bx.size(); ii++)
  {
    if (not (data->{{prefix}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj++;
{{ macros.checkObjectIndex(objects[0], 'nobj') }} {# same indexing for all objects #}
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
      const int idx0 = candidates.at(set.at(indicies.at(0)));
      const int idx1 = candidates.at(set.at(indicies.at(1)));
      const int idx2 = candidates.at(set.at(indicies.at(2)));
{{ macros.getObjectCuts(prefix, 'idx0', objects[0], tmEventSetup, nEtaBits) }}
{{ macros.getObjectCuts(prefix, 'idx1', objects[1], tmEventSetup, nEtaBits) }}
{{ macros.getObjectCuts(prefix, 'idx2', objects[2], tmEventSetup, nEtaBits) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx0', 'idx1', cond, tmEventSetup, LUTSAB, iPi) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx0', 'idx2', cond, tmEventSetup, LUTSAC, iPi) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx1', 'idx2', cond, tmEventSetup, LUTSBC, iPi) }}
      pass = true;
      break;
    }

    if (pass) break;
  }

  return pass;
}

{% else %}
bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  {% if overlap_removal %}
    {{ macros.getReference(reference, tmEventSetup, nEtaBits) }}
  {% endif %}
  bool pass = false;
  size_t nobj0 = 0;
  for (size_t ii = 0; ii < data->{{prefix}}Bx.size(); ii++)
  {
    if (not (data->{{prefix}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj0++;
    {{ macros.checkObjectIndex(objects[0], 'nobj0') }}
    {% if overlap_removal %}
      {{ cond | hasCorrelationCuts() }}
      {{ macros.removeOverlap(cond, objects[0], 'ii', reference, tmEventSetup, scaleMap, iPi) }}
    {% endif %}
    const int idx0 = ii;
{{ macros.getObjectCuts(prefix, 'idx0', objects[0], tmEventSetup, nEtaBits) }}

    size_t nobj1 = 0;
    for (size_t jj = 0; jj < data->{{prefix}}Bx.size(); jj++)
    {
      if (not (data->{{prefix}}Bx.at(jj) == {{ objects[1].getBxOffset() }})) continue;
      nobj1++;
      {{ macros.checkObjectIndex(objects[1], 'nobj1') }}
      {% if overlap_removal %}
        {{ cond | hasCorrelationCuts() }}
        {{ macros.removeOverlap(cond, objects[1], 'jj', reference, tmEventSetup, scaleMap, iPi) }}
      {% endif %}
      const int idx1 = jj;
{{ macros.getObjectCuts(prefix, 'idx1', objects[1], tmEventSetup, nEtaBits) }}

      size_t nobj2 = 0;
      for (size_t kk = 0; kk < data->{{prefix}}Bx.size(); kk++)
      {
        if (not (data->{{prefix}}Bx.at(kk) == {{ objects[2].getBxOffset() }})) continue;
        nobj2++;
        {{ macros.checkObjectIndex(objects[2], 'nobj2') }}
        {% if overlap_removal %}
          {{ cond | hasCorrelationCuts() }}
          {{ macros.removeOverlap(cond, objects[2], 'kk', reference, tmEventSetup, scaleMap, iPi) }}
        {% endif %}
        const int idx2 = kk;
{{ macros.getObjectCuts(prefix, 'idx2', objects[2], tmEventSetup, nEtaBits) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx0', 'idx1', cond, tmEventSetup, LUTSAB, iPi) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx0', 'idx2', cond, tmEventSetup, LUTSAC, iPi) }}
{{ macros.getSameTypeCorrelationCuts(prefix, 'idx1', 'idx2', cond, tmEventSetup, LUTSBC, iPi) }}
        pass = true;
        break;
      }
      if (pass) break;
    }
  }
  return pass;
}
{% endif %}

{% endblock MuonMuonMuonCorrelationTemplate %}
{# eof #}
