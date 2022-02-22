{#
 # @author: Takashi MATSUSHITA
 #}
{% block CaloThreeObjectCorrelationTemplate scoped %}

{% import 'macros.jinja2' as macros %}

{% set objects = cond.getObjects() %}
{% set reference = objects[objects|length -1] %}
{% set etaScaleRef = scaleMap | getScale(reference, tmGrammar.ETA) %}
{% set nEtaBits = etaScaleRef.getNbits() %}  
{% set nObjects = 2 %}
{% set objects = objects[0] | sortObjects(objects[1]) %}
{% set prefix0 = objects[0] | getPrefix %}
{% set prefix1 = objects[1] | getPrefix %}
{% set ref = reference | getPrefix %}

{% set etaScale0 = scaleMap | getScale(objects[0], tmGrammar.ETA) %}
{% set nEtaBits0 = etaScale0.getNbits() %}
{% set phiScale0 = scaleMap | getScale(objects[0], tmGrammar.PHI) %}

{% set etaScale1 = scaleMap | getScale(objects[1], tmGrammar.ETA) %}
{% set nEtaBits1 = etaScale1.getNbits() %}

{# assume phiScale is the same for calo objects #}

{% set LUTS = scaleMap | getLookUpTable(objects[0], objects[1]) %}

{% set iPi = (0.5*(phiScale0.getMaximum() - phiScale0.getMinimum())/phiScale0.getStep()) | int -%}

bool
{{ cond.getName() }}
//test
(L1Analysis::L1AnalysisL1UpgradeDataFormat* data)
{
{{ macros.getReference(reference, tmEventSetup, nEtaBits) }}
  bool pass = false;
  size_t nobj0 = 0;
  for (size_t ii = 0; ii < data->{{prefix0}}Bx.size(); ii++)
  {
    if (not (data->{{prefix0}}Bx.at(ii) == {{ objects[0].getBxOffset() }})) continue;
    nobj0++;
{% if prefix0 | isTau %}
      if (nobj0 > {{macros.getMaxTaus()}}) break;
{% endif %}
{{ macros.checkObjectIndex(objects[0], 'nobj0') }}
{{ macros.getObjectCuts(prefix0, 'ii', objects[0], tmEventSetup, nEtaBits0) }}

    size_t nobj1 = 0;
    for (size_t jj = 0; jj < data->{{prefix1}}Bx.size(); jj++)
    {
      if (not (data->{{prefix1}}Bx.at(jj) == {{ objects[1].getBxOffset() }})) continue;
      nobj1++;
      {% if prefix1 | isTau %}
        if (nobj1 > {{macros.getMaxTaus()}}) break;
      {% endif %}
      {{ macros.checkObjectIndex(objects[1], 'nobj1') }}

{{ macros.getObjectCuts(prefix1, 'jj', objects[1], tmEventSetup, nEtaBits1) }}
{{ macros.getSameTypeCorrelationCuts(prefix0, 'ii', 'jj', cond, tmEventSetup, LUTS, iPi) }}

  {% for cut in cond.getCuts() %} {# extract cut values #}
    {% if cut.getCutType() == tmEventSetup.OvRmDeltaR %}
     // {{cut.getMinimum().value}} <= DeltaEta <= {{cut.getMaximum().value}}
     const long long cutDeltaEtaMin = {{(cut.getMinimum().value * 10**LUTS.PREC_DELTA) | int}}; // {{cut.getMinimum().value}} * 10^{{LUTS.PREC_DELTA}}
     const long long cutDeltaEtaMax = {{(cut.getMaximum().value * 10**LUTS.PREC_DELTA) | int}}; // {{cut.getMaximum().value}} * 10^{{LUTS.PREC_DELTA}}
  
     {% elif cut.getCutType() == tmEventSetup.OvRmDeltaPhi %}
      // {{cut.getMinimum().value}} <= DeltaPhi <= {{cut.getMaximum().value}}
      const long long cutDeltaPhiMin = {{(cut.getMinimum().value * 10**LUTS.PREC_DELTA) | int}}; // {{cut.getMinimum().value}} * 10^{{LUTS.PREC_DELTA}}
      const long long cutDeltaPhiMax = {{(cut.getMaximum().value * 10**LUTS.PREC_DELTA) | int}}; // {{cut.getMaximum().value}} * 10^{{LUTS.PREC_DELTA}}
  
     {% elif cut.getCutType() == tmEventSetup.OvRmDeltaR %}
      // {{cut.getMinimum().value | toDeltaR}} <= DeltaR <= {{cut.getMaximum().value|toDeltaR}}
      const long long cutDeltaR2Min = {{(cut.getMinimum().value * 10**LUTS.PREC_DR) | int}}; // {{cut.getMinimum().value}} * 10^{{LUTS.PREC_DR}}
      const long long cutDeltaR2Max = {{(cut.getMaximum().value * 10**LUTS.PREC_DR) | int}}; // {{cut.getMaximum().value}} * 10^{{LUTS.PREC_DR}}
     {% endif %}
  {% endfor %}

	for (size_t kk = 0; kk < reference.size(); kk++)
	  {
	    const int index = reference.at(kk);
	    {% for cut in cond.getCuts() %}
	    {% if cut.getCutType() == tmEventSetup.OvRmDeltaEta %}
		{
	      {{ macros.setDeltaEta(prefix0, ref, 'ii', 'index', LUTS) }}
		  if ((cutDeltaEtaMin <= deltaEta) and (deltaEta <= cutDeltaEtaMax)) continue;
		}

		{
	      {{ macros.setDeltaEta(prefix1, ref, 'jj', 'index', LUTS) }}
		  if ((cutDeltaEtaMin <= deltaEta) and (deltaEta <= cutDeltaEtaMax)) continue;
		}
	
	    {% elif cut.getCutType() == tmEventSetup.OvRmDeltaPhi %}
		{
	      {{ macros.setDeltaPhi(prefix0, ref, 'ii', 'index', LUTS, iPi) }}
		  if ((cutDeltaPhiMin <= minDeltaPhi) and (minDeltaPhi <= cutDeltaPhiMax)) continue;

		}

		{
	      {{ macros.setDeltaPhi(prefix1, ref, 'jj', 'index', LUTS, iPi) }}
		  if ((cutDeltaPhiMin <= minDeltaPhi) and (minDeltaPhi <= cutDeltaPhiMax)) continue;
		}
	
	    {% elif cut.getCutType() == tmEventSetup.OvRmDeltaR %}
		{
	      {{ macros.setDeltaR2(prefix0, ref, 'ii', 'index', LUTS, iPi) }}
	      if ((cutDeltaR2Min <= deltaR2) and (deltaR2 <= cutDeltaR2Max)) continue;
		}

		{
	      {{ macros.setDeltaR2(prefix1, ref, 'jj', 'index', LUTS, iPi) }}
	      if ((cutDeltaR2Min <= deltaR2) and (deltaR2 <= cutDeltaR2Max)) continue;
		}

	    {% endif %}

	      pass = true;
	      break;

	    {% endfor %}
	  }

      if (pass) break;
    }
    if (pass) break;
  }

  return pass;

}

{% endblock CaloThreeObjectCorrelationTemplate %}
{# eof #}
