{% block AnomalyDetectionTriggerTemplate scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Anomaly detection trigger (ADT) is still experimental.
  bool pass = false;

  return pass;
}
{% endblock AnomalyDetectionTriggerTemplate %}
{# eof #}
