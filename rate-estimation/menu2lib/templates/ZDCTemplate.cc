{% block ZDCTemplate scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Zero degree calorimeter (ZDC) data currently not available.
  bool pass = false;

  return pass;
}
{% endblock ZDCTemplate %}
{# eof #}
