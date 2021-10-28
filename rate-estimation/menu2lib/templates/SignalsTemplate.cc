{# Template for signal type conditions like centrality or muon showers #}

{#
// Muon shower algorithms:
// L1_MuShower_OneNominal_InTime, L1_MuShower_OneTight_InTime, L1_MuShower_TwoLoose_InTime
#}

{% block SignalsTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects() %}
{% set prefix = object | getPrefix  %}
{% set analysis_type = object | getAnalysisType  %}

{#
// EF
// MuonShowerType from L1TNtuples => L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h:
// kInvalid, kOneNominal,kOneTight, kTwoLoose
//{% if object.getType() == tmEventSetup.MuonShower0 -%}
//  {% set type = 'L1Analysis::kOneNominal' %}
//{% elif object.getType() == tmEventSetup.MuonShower1 -%}
//  {% set type = 'L1Analysis::kOneTight' %}
//{% if object.getType() == tmEventSetup.MuonShowerOutOfTime0 -%}
//{% if object.getType() == tmEventSetup.MuonShowerOutOfTime1 -%}
//{% endif -%}
#}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;
  // Is signals in same bx?
  if (data->{{ prefix }}Bx.at(0) == {{ object.getBxOffset() }})
    {
      // Is signal set?
      if (data->{{ prefix }}Type.at(0) == L1Analysis::{{ analysis_type }})
      {# EF: if (data->{{ prefix }}Type.at(0) == {{ type }}) #}
	{
	  pass = true;
	}
    }
  return pass;
}
{% endblock SignalsTemplate %}
