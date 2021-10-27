{# Template for signal type conditions like centrality or muon showers #}

// Muon shower algorithms:
// L1_MuShower_OneNominal_InTime, L1_MuShower_OneTight_InTime, L1_MuShower_TwoLoose_InTime

{% block SignalsTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects() %}
{% set prefix = objects[0] | getPrefix  %}
{% set analysis_type = objects[0] | getAnalysisType  %}

// EF
// MuonShowerType from L1TNtuples => L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h:
// kInvalid, kOneNominal,kOneTight, kTwoLoose
//{% if object.getType() == tmEventSetup.MUS0 -%}
//  {% set type = 'L1Analysis::kOneNominal' %}
//{% elif object.getType() == tmEventSetup.MUS1 -%}
//  {% set type = 'L1Analysis::kOneTight' %}
//{% endif -%}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;
  // Is signals in same bx?
  if (data->{{ prefix }}Bx.at(0) == {{ objects[0].getBxOffset() }})  
    {
      // Is signal set?
      if (data->{{ prefix }}Type.at(0) == L1Analysis::{{ analysis_type }})
      // EF: if (data->{{ prefix }}Type.at(0) == {{ type }})
	{
	  pass = true;
	}
    }
  return pass;
}
{% endblock SignalsTemplate %}
