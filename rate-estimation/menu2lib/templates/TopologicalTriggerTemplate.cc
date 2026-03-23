{% block TopologicalTriggerTemplate scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Topo trigger 
  bool pass = false;

  return pass;
}
{% endblock TopologicalTriggerTemplate %}
{# eof #}
