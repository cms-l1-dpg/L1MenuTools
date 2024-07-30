{% block CicadaTriggerTemplate scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Cicada trigger 
  bool pass = false;

  return pass;
}
{% endblock CicadaTriggerTemplate %}
{# eof #}
