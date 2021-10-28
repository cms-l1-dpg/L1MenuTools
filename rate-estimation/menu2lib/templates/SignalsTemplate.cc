{# Template for signal type conditions like centrality or muon showers #}
{# MuonShowerType from L1TNtuples: kInvalid, kOneNominal,kOneTight, kTwoLoose #}
{# See in: L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h #}

{% block SignalsTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects()[0] %}
{% set prefix = object | getPrefix  %}
{% set analysis_type = object | getAnalysisType  %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;
  {# Is signals in same bx? #}
  if (data->{{ prefix }}Bx.at(0) == {{ object.getBxOffset() }})
  {
    {# Is signal set? #}
    if (data->{{ prefix }}Type.at(0) == L1Analysis::{{ analysis_type }})
    {
      pass = true;
    }
  }
  return pass;
}
{% endblock SignalsTemplate %}
