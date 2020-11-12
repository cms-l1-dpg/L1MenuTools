{#
 # @author: Takashi MATSUSHITA
 #}
{% block Externals scoped %}
{% import 'macros.jinja2' as macros %}

bool
{{ cond.getName() }}
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
  // for now return false always
  // could check decision available in ugt data
  bool pass = false;
  return pass;
}
{% endblock Externals %}
{# eof #}
