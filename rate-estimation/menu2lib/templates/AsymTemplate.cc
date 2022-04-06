{#
 # @author: Takashi MATSUSHITA
 #}
{% block AsymTemplate scoped %}
{% import 'macros.jinja2' as macros %}
{% set object = cond.getObjects()[0] %}

{% set prefix = object | getPrefix  %}
{% if object.getType() == tmEventSetup.ASYMET -%}
  {% set type = 'L1Analysis::kAsymEt' %}
{% elif object.getType() == tmEventSetup.ASYMHT -%}
  {% set type = 'L1Analysis::kAsymHt' %}
{% elif object.getType() == tmEventSetup.ASYMETHF -%}
  {% set type = 'L1Analysis::kAsymEtHF' %}
{% elif object.getType() == tmEventSetup.ASYMHTHF -%}
  {% set type = 'L1Analysis::kAsymHtHF' %}
{% endif -%}


bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  bool pass = false;

  for (size_t ii = 0; ii < data->{{prefix}}Bx.size(); ii++)
  {
    if (not (data->{{prefix}}Type.at(ii) == {{ type }})) continue;
    if (not (data->{{prefix}}Bx.at(ii) == {{ object.getBxOffset() }})) continue;
    {{ macros.getEsumCuts(prefix, 'ii', object, tmEventSetup) }}

    pass = true;
    break;
  }

  return pass;
}
{% endblock AsymTemplate %}
{# eof #}
