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
  // It has been switched to be always TRUE as it is done in the emulator (according to the request from the Heavy Ion group).
  // NOTE: in general, the decision could be checked in the UGT data...
  bool pass = true;
  return pass;
}
{% endblock Externals %}
{# eof #}
