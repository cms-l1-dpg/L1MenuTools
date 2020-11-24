"""L1 trigger menu to C++ library for rate estimations."""

import argparse
import logging
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
  tmEventSetup.Muon: 'muon',
  tmEventSetup.Egamma: 'eg',
  tmEventSetup.Tau: 'tau',
  tmEventSetup.Jet: 'jet',
  tmEventSetup.ETT: 'sum',
  tmEventSetup.HTT: 'sum',
  tmEventSetup.ETTEM: 'sum',
  tmEventSetup.ETM: 'sum',
  tmEventSetup.HTM: 'sum',
  tmEventSetup.ETMHF: 'sum',
  tmEventSetup.MBT0HFM: 'sum',
  tmEventSetup.MBT0HFP: 'sum',
  tmEventSetup.MBT1HFM: 'sum',
  tmEventSetup.MBT1HFP: 'sum',
  tmEventSetup.TOWERCOUNT: 'sum',
}


#
# helpers
#
def getObjectName(key):
  return {
    tmEventSetup.Muon: tmGrammar.MU,
    tmEventSetup.Egamma: tmGrammar.EG,
    tmEventSetup.Jet: tmGrammar.JET,
    tmEventSetup.Tau: tmGrammar.TAU,
    tmEventSetup.ETM: tmGrammar.ETM,
    tmEventSetup.HTM: tmGrammar.HTM,
    tmEventSetup.ETMHF: tmGrammar.ETMHF,
  }[key.getType()]


def isVectorSum(key):
  return key.getType() in (tmEventSetup.ETM, tmEventSetup.HTM, tmEventSetup.ETMHF)


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
def toDecimal(value, bits):
  signed = 1 << (bits - 1);
  return (~signed & value) - signed if (signed & value) else value


def toCharge(charge):
  """Return encoded charge."""
  return {'positive': 1, 'negative': -1}.get(charge, 0)


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
        if ((data->%(prefix)sChg.at(idx0) == 0) or (data->%(prefix)sChg.at(idx1) == 0))
        {
          invalid = true;
          break;
        }
        if (data->%(prefix)sChg.at(idx0) != data->%(prefix)sChg.at(idx1))
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
        if (data->%(prefix)sChg.at(idx) > 0) positives++;
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


def getPrefix(key):
  return PREFIX[key.getType()]


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
    if cut.getCutType() == tmEventSetup.Slice:
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


def render(menu, template):
  module_dir = os.path.dirname(os.path.abspath(__file__))
  templates_dir = os.path.join(module_dir, 'templates')
  j2_env = Environment(loader=FileSystemLoader(templates_dir), trim_blocks=True)

  j2_env.add_extension('jinja2.ext.loopcontrols')
  j2_env.filters['toDecimal'] = toDecimal
  j2_env.filters['toCharge'] = toCharge
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

  data = {
    "tmGrammar": tmGrammar,
    "tmEventSetup": tmEventSetup,
    "menu": menu
  }

  return j2_env.get_template(template).render(data)

def parse_args():
  parser = argparse.ArgumentParser()
  parser.add_argument("--menu", required=True, help="path to the level1 trigger menu xml file")
  parser.add_argument("--output", default='menulib.cc', help="output c++ file name")
  return parser.parse_args()

def main():
  args = parse_args()

  logger = logging.getLogger().setLevel(logging.INFO)

  logging.info("loading... %s", args.menu)
  menu = tmEventSetup.getTriggerMenu(args.menu)

  dirname = os.path.dirname(args.output)

  content = render(menu, 'MenuTemplate.cc')
  filename = args.output
  logging.info("writing... %s", filename)
  with open(filename, 'w') as fp:
    fp.write(content)

  content = render(menu, 'MenuTemplate.hh')
  filename = os.path.join(dirname, 'menulib.hh')
  logging.info("writing... %s", filename)
  with open(filename, 'w') as fp:
    fp.write(content)

  content = render(menu, 'menu.txt')
  filename = os.path.join(dirname, 'menu.txt')
  logging.info("writing... %s", filename)
  with open(filename, 'w') as fp:
    fp.write(content)

  logging.info("done.")

if __name__ == '__main__':
    main()
