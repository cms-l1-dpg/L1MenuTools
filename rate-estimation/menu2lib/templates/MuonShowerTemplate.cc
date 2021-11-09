{#
    # @author: Sven Dildick (Rice University)
 #}
{# Template for muon showers #}
{# See in: L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h #}

{% block MuonShowerTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects()[0] %}
{% set prefix = object | getPrefix  %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;

  {# Is there at least one shower object? #}
  if (data->nMuonShowers >= 1) {

    {# Are signals in same bx? #}
    if (data->{{ prefix }}Bx.at(0) == {{ object.getBxOffset() }})
      {
        {% if cond.getType() == tmEventSetup.MuonShower0 -%}
        if (data->{{ prefix }}OneNominal.at(0))
          {
            pass = true;
          }
        {% elif cond.getType() == tmEventSetup.MuonShower1 -%}
        if (data->{{ prefix }}OneTight.at(0))
          {
            pass = true;
          }
        {% elif cond.getType() == tmEventSetup.MuonShowerOutOfTime0 -%}
        {# not implemented #}
        {% elif cond.getType() == tmEventSetup.MuonShowerOutOfTime1 -%}
        {# not implemented #}
        {% endif -%}
      }
  }
  return pass;
}
{% endblock MuonShowerTemplate %}
{# eof #}
