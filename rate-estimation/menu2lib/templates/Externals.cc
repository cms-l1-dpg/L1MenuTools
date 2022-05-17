{#
 # @author: Takashi MATSUSHITA
 #}
{% block Externals scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // Before the start of Run 3, the decision was always set FALSE;
  // This is set to be always TRUE in the emulator. This is important to do for Heavy Ion studies (as requested by the Heavy Ions team).
  //  bool pass = true;
  
  // However, this affects the rate quite significantly for pp collisions studies, and therefore has to be set to FALSE for pp studies.
  bool pass = false;
  
  return pass;
}
{% endblock Externals %}
{# eof #}
