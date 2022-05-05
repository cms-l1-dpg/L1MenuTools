{# Template for centrality triggers for the Heavy Ion Menu #}
{# See in: L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h #}

{% block CentralityTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects()[0] %}
{% set prefix = object | getPrefix  %}
{% set bitmask = cond.getType() | getCentralityBitmask %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;
  {# Is signal in same bx? #}
  if (data->{{ prefix }}Bx.at(18) == {{ object.getBxOffset() }})
  {
    {# Is signal bit set? #}
    if ((data->{{ prefix }}IEt.at(18) & {{ bitmask }})) 
    {
      pass = true;
    }
  }
  return pass;
}
{% endblock CentralityTemplate %}
