{% block Axol1tlTriggerTemplate scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Axol1tl trigger 
  bool pass = false;

  return pass;
}
{% endblock Axol1tlTriggerTemplate %}
{# eof #}
