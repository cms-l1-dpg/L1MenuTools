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
  size_t nobj = 0;
  std::vector<int> candidates;
  for (size_t ii = 0; ii < {{prefix}}Bx.size(); ii++)
  {
    if (not ({{prefix}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj++;
    {% if prefix | isTau %}
      if (nobj > {{macros.getMaxTaus()}}) break;
    {% endif %}
    {{ macros.checkObjectIndex(objects[0], 'nobj') }} {# same indexing for all objects #}
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
      int idx = -1;
      {{ objects | hasEtaPhiCuts }}
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
