{#
 # @author: Takashi MATSUSHITA
 #}
{% block EsumTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects()[0] %}

{% set prefix = object | getPrefix  %}
{% if object.getType() == tmEventSetup.ETT -%}
  {% set type = 'L1Analysis::kTotalEt' %}
{% elif object.getType() == tmEventSetup.HTT -%}
  {% set type = 'L1Analysis::kTotalHt' %}
{% elif object.getType() == tmEventSetup.ETM -%}
  {% set type = 'L1Analysis::kMissingEt' %}
{% elif object.getType() == tmEventSetup.HTM -%}
  {% set type = 'L1Analysis::kMissingHt' %}
{% elif object.getType() == tmEventSetup.MBT0HFM -%}
  {% set type = 'L1Analysis::kMinBiasHFM0' %}
{% elif object.getType() == tmEventSetup.MBT0HFP -%}
  {% set type = 'L1Analysis::kMinBiasHFP0' %}
{% elif object.getType() == tmEventSetup.MBT1HFM -%}
  {% set type = 'L1Analysis::kMinBiasHFM1' %}
{% elif object.getType() == tmEventSetup.MBT1HFP -%}
  {% set type = 'L1Analysis::kMinBiasHFP1' %}
{% elif object.getType() == tmEventSetup.ETMHF -%}
  {% set type = 'L1Analysis::kMissingEtHF' %}
{% elif object.getType() == tmEventSetup.ETTEM -%}
  {% set type = 'L1Analysis::kTotalEtEm' %}
{% elif object.getType() == tmEventSetup.TOWERCOUNT -%}
  {% set type = 'L1Analysis::kTowerCount' %}
{% endif -%}


bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;

  {{ objects | hasEtaPhiCuts }}
  for (size_t ii = 0; ii < {{prefix}}Bx.size(); ii++)
  {
    if (not ({{prefix}}Type.at(ii) == {{ type }})) continue;
    if (not ({{prefix}}Bx.at(ii) == {{ object.getBxOffset() }})) continue;
    {{ macros.getEsumCuts(prefix, 'ii', object, tmEventSetup) }}

    pass = true;
    break;
  }

  return pass;
}
{% endblock EsumTemplate %}
{# eof #}
