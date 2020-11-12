#!/bin/env python
#
# @author: Takashi MATSUSHITA
#

import argparse
import math
import os
import re
import string
from jinja2 import Environment, FileSystemLoader

import tmGrammar
import tmEventSetup


#
# constants
#
PREFIX = {
  tmEventSetup.Muon: "data->muon",
  tmEventSetup.Egamma: "data->eg",
  tmEventSetup.Tau: "data->tau",
  tmEventSetup.Jet: "data->jet",
  tmEventSetup.ETT: "data->sum",
  tmEventSetup.HTT: "data->sum",
  tmEventSetup.ETTEM: "data->sum",
  tmEventSetup.ETM: "data->sum",
  tmEventSetup.HTM: "data->sum",
  tmEventSetup.ETMHF: "data->sum",
  tmEventSetup.MBT0HFM: "data->sum",
  tmEventSetup.MBT0HFP: "data->sum",
  tmEventSetup.MBT1HFM: "data->sum",
  tmEventSetup.MBT1HFP: "data->sum",
  tmEventSetup.TOWERCOUNT: "data->sum",
}


#
# helpers
#
def getObjectName(x):
  enum_to_name = {
    tmEventSetup.Muon: tmGrammar.MU,
    tmEventSetup.Egamma: tmGrammar.EG,
    tmEventSetup.Jet: tmGrammar.JET,
    tmEventSetup.Tau: tmGrammar.TAU,
    tmEventSetup.ETM: tmGrammar.ETM,
    tmEventSetup.HTM: tmGrammar.HTM,
    tmEventSetup.ETMHF: tmGrammar.ETMHF,
  }
  return enum_to_name[x.getType()]


def isVectorSum(x):
  if x.getType() in (tmEventSetup.ETM, tmEventSetup.HTM, tmEventSetup.ETMHF):
    return True
  return False


def getPrecision(scaleMap, obj1, obj2, kind):
  key = '-'.join([tmEventSetup.PRECISION, getObjectName(obj1), getObjectName(obj2), kind])
  return scaleMap[key].getNbits()


def getPrecisionByName(scaleMap, obj1, obj2, kind):
  key = '-'.join([tmEventSetup.PRECISION, obj1, obj2, kind])
  return scaleMap[key].getNbits()


def getScaleByName(scaleMap, obj, cut):
  key = '-'.join([obj, cut])
  return scaleMap[key]


#
# filters
#
def toDecimal(value, nBit):
  signed = 1 << (nBit - 1);
  return (~signed & value) - signed if (signed & value) else value


def toCharge(charge):
  rc = 0
  if charge == "positive": rc = 1
  elif charge == "negative": rc = -1
  return rc


def chkChgCor(cuts, prefix, nobjects):
  data = {}
  data['prefix'] = prefix;
  data['nobjects'] = nobjects;
  text = ""
  check = """
      // charge correlation
      bool equal = true;
      bool invalid = false;
      for (size_t mm = 0; mm < %(nobjects)s -1; mm++)
      {
        int idx0 = candidates.at(set.at(indicies.at(mm)));
        int idx1 = candidates.at(set.at(indicies.at(mm+1)));
        if ((%(prefix)sChg.at(idx0) == 0) or (%(prefix)sChg.at(idx1) == 0))
        {
          invalid = true;
          break;
        }
        if (%(prefix)sChg.at(idx0) != %(prefix)sChg.at(idx1))
        {
          equal = false;
          break;
        }
      }
      if (invalid) continue;
""" % data

  for cut in cuts:
    if cut.getCutType() == tmEventSetup.ChargeCorrelation:
      if cut.getData() == "os":
        if nobjects == 4:
          text = """
      size_t positives = 0;
      for (size_t mm = 0; mm < %(nobjects)s; mm++)
      {
        int idx = candidates.at(set.at(indicies.at(mm)));
        if (%(prefix)sChg.at(idx) > 0) positives++;
      }
      // charge correlation: "os"
      if (positives != 2) continue;  // as in emulator
""" % data
        else:
          text = check + """
      // charge correlation: "os"
      if (equal) continue;
"""
      elif cut.getData() == "ls":
        text = check + """
      // charge correlation: "ls"
      if (not equal) continue;
"""
  return text


def hasEtaPhiCuts(objects):
  nEtaCuts = 0
  nPhiCuts = 0

  for obj in objects:
    nEta = 0
    nPhi = 0
    for cut in obj.getCuts():
      if cut.getCutType() == tmEventSetup.Eta:
        nEta += 1
      elif cut.getCutType() == tmEventSetup.Phi:
        nPhi += 1
    nEtaCuts = max(nEtaCuts, nEta)
    nPhiCuts = max(nPhiCuts, nPhi)

  declaration = ""
  if nEtaCuts == 1:
    declaration += "bool etaWindow1;"
  elif nEtaCuts == 2:
    declaration += "bool etaWindow1, etaWindow2;"

  if nPhiCuts == 1:
    declaration += "bool phiWindow1;"
  elif nPhiCuts == 2:
    declaration += "bool phiWindow1, phiWindow2;"

  return declaration


def hasCorrelationCuts(condition):
  declaration = ""
  requireDeltaEta = False

  for cut in condition.getCuts():
    if cut.getCutType() in (tmEventSetup.DeltaEta, tmEventSetup.OvRmDeltaR, tmEventSetup.Mass,
                            tmEventSetup.OvRmDeltaEta, tmEventSetup.DeltaR):
      requireDeltaEta = True

  if requireDeltaEta:
    declaration += "int iEta = -9999999; unsigned int deltaIEta = 9999999;"

  return declaration


def sortObjects(obj1, obj2):
  if obj1.getType() == obj2.getType(): return obj1, obj2

  if obj1.getType() == tmEventSetup.Muon:
    if obj2.getType() not in (tmEventSetup.ETM, tmEventSetup.HTM, tmEventSetup.ETMHF):
      return obj2, obj1

  elif obj1.getType() == tmEventSetup.Jet:
    if obj2.getType() not in (tmEventSetup.Tau, tmEventSetup.ETM, tmEventSetup.HTM,
                              tmEventSetup.ETMHF, tmEventSetup.Muon):
      return obj2, obj1

  elif obj1.getType() == tmEventSetup.Tau:
    if obj2.getType() not in (tmEventSetup.ETM, tmEventSetup.HTM, tmEventSetup.ETMHF,
                              tmEventSetup.Muon):
      return obj2, obj1
  elif obj1.getType() in (tmEventSetup.ETM, tmEventSetup.HTM, tmEventSetup.ETMHF):
    return obj2, obj1

  return obj1, obj2


def toCpp(value):
  tokens = value.split()
  array = []

  pattern = "(\W*[a-zA-Z0-9]+_i[0-9]+)(\W*)"
  prog = re.compile(pattern)

  for token in tokens:
    result = prog.match(token)
    if result:
      array.append(result.group(1) + '(data)' + result.group(2))
    elif token == tmGrammar.AND:
      array.append(tmGrammar.AND.lower())
    elif token == tmGrammar.OR:
      array.append(tmGrammar.OR.lower())
    else:
      if tmGrammar.NOT in token:
        token = token.replace(tmGrammar.NOT, tmGrammar.NOT.lower())
      array.append(token)
  return ' '.join(array)


def getLut(scale, precision):
  lut = tmEventSetup.LlongVec()
  tmEventSetup.getLut(lut, scale, precision.getNbits())
  return lut


def getCaloMuonConversionLut(scaleMap, calo, cuts):
  rc = {}
  for cut in cuts:
    scale_calo = getScaleByName(scaleMap, calo, cut)
    scale_muon = getScaleByName(scaleMap, tmGrammar.MU, cut)
    lut = tmEventSetup.LlongVec()

    if cut == tmGrammar.ETA:
      tmEventSetup.getCaloMuonEtaConversionLut(lut, scale_calo, scale_muon)
      rc[tmGrammar.ETA] = lut

    elif cut == tmGrammar.PHI:
      tmEventSetup.getCaloMuonPhiConversionLut(lut, scale_calo, scale_muon)
      rc[tmGrammar.PHI] = lut

  return rc


def getDeltaLut(scaleMap, obj1, obj2, cuts=None):
  precision = getPrecisionByName(scaleMap, obj1, obj2, 'Delta')

  rc = {}
  for cut in cuts:
    scale1 = getScaleByName(scaleMap, obj1, cut)
    scale2 = getScaleByName(scaleMap, obj2, cut)
    vec = tmEventSetup.DoubleVec()
    lut = tmEventSetup.LlongVec()
    n = tmEventSetup.getDeltaVector(vec, scale1, scale2)
    tmEventSetup.setLut(lut, vec, precision)
    rc[cut] = lut
  return rc


def getEtLut(scaleMap, obj, precision):
  lut = tmEventSetup.LlongVec()
  scale = getScaleByName(scaleMap, obj, 'ET')
  tmEventSetup.getLut(lut, scale, precision)
  return lut


def getMathLut(scaleMap, obj1, obj2, math=None):
  precision_math = getPrecisionByName(scaleMap, obj1, obj2, 'Math')

  rc = {}
  for cut, func in list(math.items()):
    scale1 = getScaleByName(scaleMap, obj1, cut)
    scale2 = getScaleByName(scaleMap, obj2, cut)
    vec = tmEventSetup.DoubleVec()
    lut = tmEventSetup.LlongVec()
    n = tmEventSetup.getDeltaVector(vec, scale1, scale2)
    func(vec, n)
    tmEventSetup.setLut(lut, vec, precision_math)
    rc[func.__name__] = lut

  precision_pt = getPrecisionByName(scaleMap, obj1, obj2, 'MassPt')
  if obj1 == obj2:
    key = '-'.join([obj1, 'ET'])
    rc['Key'] = key
    rc[key] = getEtLut(scaleMap, obj1, precision_pt)

  return rc


def getPrefix(x):
  return PREFIX[x.getType()]


def isTau(prefix):
  return prefix == PREFIX[tmEventSetup.Tau]


def isMuon(prefix):
  return prefix == PREFIX[tmEventSetup.Muon]


def hasIndexCut(cuts):
  for cut in cuts:
    if cut.getCutType() == tmEventSetup.Slice:
      return True
  return False


def getIndexCut(cuts):
  for cut in cuts:
    if cut.getCutType() == tmEventSetup.Index:
      minimum_index = int(cut.getMinimum().value);
      maximum_index = int(cut.getMaximum().value);
      return minimum_index, maximum_index
  return 0, 11  # maximum object index range [0, 11]


def getScale(scaleMap, obj, cut):
  key = '-'.join([getObjectName(obj), cut])
  return scaleMap[key]


def getLookUpTable(scaleMap, obj1, obj2):
  precisionDelta = getPrecision(scaleMap, obj1, obj2, 'Delta')
  precisionMath = getPrecision(scaleMap, obj1, obj2, 'Math')
  precisionPt = getPrecision(scaleMap, obj1, obj2, 'MassPt')

  has_deta = not (isVectorSum(obj1) or isVectorSum(obj2))
  convert = ((obj1.getType() != obj2.getType())
             and
             ((obj1.getType() == tmEventSetup.Muon) or (obj2.getType() == tmEventSetup.Muon)))

  rc = {}
  rc['DPHI'] = "LUT_DPHI_%s_%s" % (getObjectName(obj1), getObjectName(obj2))
  rc['ET0'] = "LUT_%s_ET" % getObjectName(obj1)
  rc['ET1'] = "LUT_%s_ET" % getObjectName(obj2)
  rc['PREC_DELTA'] = precisionDelta
  rc['PREC_MATH'] = precisionMath
  rc['PREC_MASS'] = 2*precisionPt + precisionMath
  rc['COS_DPHI'] = "LUT_COS_DPHI_%s_%s" % (getObjectName(obj1), getObjectName(obj2))

  if has_deta:
    rc['DETA'] = "LUT_DETA_%s_%s" % (getObjectName(obj1), getObjectName(obj2))
    rc['COSH_DETA'] = "LUT_COSH_DETA_%s_%s" % (getObjectName(obj1), getObjectName(obj2))
    rc['PREC_DR'] = 2*precisionDelta

  if convert:
    if obj1.getType() == tmEventSetup.Muon:
      rc['CONV_PHI'] = 'LUT_PHI_%s2%s' % (getObjectName(obj2), getObjectName(obj1))
      if has_deta:
        rc['CONV_ETA'] = 'LUT_ETA_%s2%s' % (getObjectName(obj2), getObjectName(obj1))
        rc['ETA_OFFSET'] = 2**getScale(scaleMap, obj2, tmGrammar.ETA).getNbits()
    else:
      rc['CONV_PHI'] = 'LUT_PHI_%s2%s' % (getObjectName(obj1), getObjectName(obj2))
      if has_deta:
        rc['CONV_ETA'] = 'LUT_ETA_%s2%s' % (getObjectName(obj1), getObjectName(obj2))
        rc['ETA_OFFSET'] = 2**getScale(scaleMap, obj1, tmGrammar.ETA).getNbits()
  return rc


def toMass(value):
  return math.sqrt(2.*value)


def toDeltaR(value):
  return "%.2f" % math.sqrt(value)


def warning(message):
  print(message)
  return ''


THIS_DIR = os.path.dirname(os.path.abspath(__file__)) + "/templates"


def render(menu, template, name):
  j2_env = Environment(loader=FileSystemLoader(THIS_DIR), trim_blocks=True)

  j2_env.add_extension('jinja2.ext.loopcontrols')
  j2_env.filters['toDecimal'] = toDecimal
  j2_env.filters['toCharge'] = toCharge
  j2_env.filters['hasEtaPhiCuts'] = hasEtaPhiCuts
  j2_env.filters['hasCorrelationCuts'] = hasCorrelationCuts
  j2_env.filters['sortObjects'] = sortObjects
  j2_env.filters['toCpp'] = toCpp
  j2_env.filters['getDeltaLut'] = getDeltaLut
  j2_env.filters['getPrecisionByName'] = getPrecisionByName
  j2_env.filters['getEtLut'] = getEtLut
  j2_env.filters['getMathLut'] = getMathLut
  j2_env.filters['getCaloMuonConversionLut'] = getCaloMuonConversionLut
  j2_env.filters['warning'] = warning
  j2_env.filters['toMass'] = toMass
  j2_env.filters['toDeltaR'] = toDeltaR
  j2_env.filters['chkChgCor'] = chkChgCor
  j2_env.filters['getPrefix'] = getPrefix
  j2_env.filters['isMuon'] = isMuon
  j2_env.filters['isTau'] = isTau
  j2_env.filters['hasIndexCut'] = hasIndexCut
  j2_env.filters['getIndexCut'] = getIndexCut
  j2_env.filters['getScale'] = getScale
  j2_env.filters['getLookUpTable'] = getLookUpTable

  association = {
      "tmGrammar": tmGrammar,
      "tmEventSetup": tmEventSetup,
      "menu": menu
      }

  return j2_env.get_template(template).render(association)


if __name__ == '__main__':
  xml = 'L1Menu_Collisions2016_v5.xml'
  output = 'menulib.cc'

  parser = argparse.ArgumentParser()

  parser.add_argument("--menu", dest="xml", default=xml, type=str, action="store", required=True, help="path to the level1 trigger menu xml file")
  parser.add_argument("--output", dest="output", default=output, type=str, action="store", help="output c++ file name")

  options = parser.parse_args()

  menu = tmEventSetup.getTriggerMenu(options.xml)

  name = os.path.basename(output)
  name, ext = os.path.splitext(name)

  text = render(menu, 'MenuTemplate.cc', name)
  with open(options.output, "w") as fp:
    fp.write(text)

  text = render(menu, 'MenuTemplate.hh', name)
  path = os.path.dirname(options.output)
  path += "menulib.hh"
  with open(path, "w") as fp:
    fp.write(text)


  path = os.path.dirname(options.output)
  path += "menu.txt"
  fp = open(path, "w")

  header = '''#============================================================================#
#-------------------------------     Menu     -------------------------------#
#============================================================================#
# L1Seed                                                     Bit  Prescale POG     PAG
'''
  fp.write(header)
  algorithms = menu.getAlgorithmMapPtr()
  for name, algorithm in list(algorithms.items()):
    fp.write('{:60} {:>3}         1\n'.format(name, algorithm.getIndex()))
  fp.close()
# eof
