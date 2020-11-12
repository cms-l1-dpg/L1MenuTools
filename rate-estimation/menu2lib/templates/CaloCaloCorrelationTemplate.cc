{#
 # @author: Takashi MATSUSHITA
 #}
{% block CaloCaloCorrelationTemplate scoped %}

{% import 'macros.jinja2' as macros %}

{% set objects = cond.getObjects() %}
{% if overlap_removal %}
  {% set reference = objects[objects|length -1] %}
{% endif %}
{% set nObjects = 2 %}
{% set objects = objects[0] | sortObjects(objects[1]) %}
{% set prefix0 = objects[0] | getPrefix %}
{% set prefix1 = objects[1] | getPrefix %}

{% set etaScale0 = scaleMap | getScale(objects[0], tmGrammar.ETA) %}
{% set nEtaBits0 = etaScale0.getNbits() %}
{% set phiScale0 = scaleMap | getScale(objects[0], tmGrammar.PHI) %}

{% set etaScale1 = scaleMap | getScale(objects[1], tmGrammar.ETA) %}
{% set nEtaBits1 = etaScale1.getNbits() %}
{# assume phiScale is the same for calo objects #}

{% set LUTS = scaleMap | getLookUpTable(objects[0], objects[1]) %}

{% set iPi = (0.5*(phiScale0.getMaximum() - phiScale0.getMinimum())/phiScale0.getStep()) | int -%}


{% if ((objects[0].getType() == objects[1].getType()) and (objects[0].getBxOffset() == objects[1].getBxOffset())) %}
bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  {% if overlap_removal %}
    {{ macros.getReference(reference, tmEventSetup, nEtaBits) }}
  {% endif %}
  size_t nobj0 = 0;
  std::vector<int> candidates;
  for (size_t ii = 0; ii < {{prefix0}}Bx.size(); ii++)
  {
    if (not ({{prefix0}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj0++;
    {% if prefix0 | isTau %}
    if (nobj0 > {{macros.getMaxTaus()}}) break;
    {% endif %}
    {{ macros.checkObjectIndex(objects[0], 'nobj0') }} {# same indexing for all objects #}
    {% if overlap_removal %}
      {{ cond | hasCorrelationCuts() }}
      {{ macros.removeOverlap(cond, objects[0], 'ii', reference, tmEventSetup, scaleMap, iPi) }}
    {% endif %}
    candidates.push_back(ii);
  }

  bool pass = false;
  if (candidates.size() < {{nObjects}}) return pass;

  std::vector<std::vector<int> > combination;
  getCombination(candidates.size(), {{nObjects}}, combination);
  std::vector<std::vector<int> > permutation;
  getPermutation({{nObjects}}, permutation);

  for (size_t ii = 0; ii < combination.size(); ii++)
  {
    const std::vector<int>& set = combination.at(ii);
    for (size_t jj = 0; jj < permutation.size(); jj++)
    {
      const std::vector<int>& indicies = permutation.at(jj);
      {{ objects | hasEtaPhiCuts }}
      const int idx0 = candidates.at(set.at(indicies.at(0)));
      const int idx1 = candidates.at(set.at(indicies.at(1)));
      {{ macros.getObjectCuts(prefix0, 'idx0', objects[0], tmEventSetup, nEtaBits0) }}
      {{ macros.getObjectCuts(prefix0, 'idx1', objects[1], tmEventSetup, nEtaBits0) }}
      {{ macros.getSameTypeCorrelationCuts(prefix0, 'idx0', 'idx1', cond, tmEventSetup, LUTS, iPi) }}
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
  for (size_t ii = 0; ii < {{prefix0}}Bx.size(); ii++)
  {
    if (not ({{prefix0}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj0++;
    {% if prefix0 | isTau %}
      if (nobj0 > {{macros.getMaxTaus()}}) break;
    {% endif %}
    {{ macros.checkObjectIndex(objects[0], 'nobj0') }}
    {% if overlap_removal %}
      {{ cond | hasCorrelationCuts() }}
      {{ macros.removeOverlap(cond, objects[0], 'ii', reference, tmEventSetup, scaleMap, iPi) }}
    {% endif %}
    {{ objects | hasEtaPhiCuts }}
    {{ macros.getObjectCuts(prefix0, 'ii', objects[0], tmEventSetup, nEtaBits0) }}

    size_t nobj1 = 0;
    for (size_t jj = 0; jj < {{prefix1}}Bx.size(); jj++)
    {
      if (not ({{prefix1}}Bx.at(jj) == {{ objects[1].getBxOffset() }})) continue;
      nobj1++;
      {% if prefix1 | isTau %}
        if (nobj1 > {{macros.getMaxTaus()}}) break;
      {% endif %}
      {{ macros.checkObjectIndex(objects[1], 'nobj1') }}
      {% if overlap_removal %}
        {{ cond | hasCorrelationCuts() }}
        {{ macros.removeOverlap(cond, objects[1], 'jj', reference, tmEventSetup, scaleMap, iPi) }}
      {% endif %}
      {{ macros.getObjectCuts(prefix1, 'jj', objects[1], tmEventSetup, nEtaBits1) }}
      {{ macros.getDifferentTypeCorrelationCuts(prefix0, prefix1, 'ii', 'jj', cond, tmEventSetup, LUTS, iPi) }}
      pass = true;
      break;
    }
    if (pass) break;
  }

  return pass;
}
{% endif %}

{% endblock CaloCaloCorrelationTemplate %}
{# eof #}
