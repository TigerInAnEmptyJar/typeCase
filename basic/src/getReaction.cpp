#include "Eparticles.h"
#include "algorithmparameter.h"
#include "geometry.h"
#include "reactions.h"
RbaseReaction* getReactionFromName(const string& name)
{
  if (name == "pp-elastic")
    return new Rppelastic();
  if (name == "ppelastic")
    return new Rppelastic();
  if (lower(name) == "pp-pkl-pkppi")
    return new Rpp_pkl_pkppi();
  if (lower(name) == "dpi+" || lower(name) == "pp-dpi+" || lower(name) == "dpiplus" ||
      lower(name) == "pp-dpiplus")
    return new Rpp_dpiPlus();
  //   if(name.lower()=="pp-pks-ppipippi")
  //     return new Rpp_pks_ppipippi();
  return NULL;
}
RbaseReaction* getReactionFromName(const string& name, momentum4D m1, momentum4D m2)
{
  if (name == "pp-elastic" || name == "ppelastic" || name == "ppel")
    return new Rppelastic(NULL, m1, m2);
  if (lower(name) == "pp-pkl-pkppi")
    return new Rpp_pkl_pkppi(NULL, m1, m2);
  if (lower(name) == "dpi+" || lower(name) == "pp-dpi+" || lower(name) == "dpiplus" ||
      lower(name) == "pp-dpiplus")
    return new Rpp_dpiPlus(NULL, m1, m2);
  //   if(name.lower()=="pp-pks-ppipippi")
  //     return new Rpp_pks_ppipippi();
  return NULL;
}
RbaseReaction* getReaction(const algorithm_parameter& param, momentum4D m1, momentum4D m2)
{
  RbaseReaction* ret = NULL;
  if (!param.IsUsed())
    return NULL;
  switch (param.getID()) {
  case 0: // pp-elastic
  {
    //	anaLog<<"test pp-elastic "<<endli;
    ret = new Rppelastic(NULL, m1, m2);
    ret->setChiLimit(param.value(0).value<float>());
    if (param.numberOfValues() >= 3) {
      ((Rppelastic*)ret)->setCoplanarityDifference(param.value(1).value<float>());
      ((Rppelastic*)ret)->setGammaDifference(param.value(2).value<float>());
    }
    break;
  }
  case 1: // d-pi+
  {
    //	anaLog<<"test pp-dpi+ "<<endli;
    ret = new Rpp_dpiPlus(NULL, m1, m2);
    ret->setChiLimit(param.value(0).value<float>());
    if (param.numberOfValues() >= 4) {
      ((Rpp_dpiPlus*)ret)->setCoplanarityDifference(param.value(1).value<float>());
      ((Rpp_dpiPlus*)ret)->setAngleLimit(param.value(2).value<float>());
      ((Rpp_dpiPlus*)ret)->setCrossoverMomentum(param.value(3).value<float>());
    }
    break;
  }
  case 10: // PK\\Lambda
  {
    //	anaLog<<"test pp- p-Kaon-Lambda "<<endli;
    ret = new Rpp_pkl_pkppi(NULL, m1, m2);
    ((Rpp_pkl_pkppi*)ret)->setExact(param.value(0).value<bool>());
    ((Rpp_pkl_pkppi*)ret)->setMultiLineFit(param.value(1).value<bool>());
    ((Rpp_pkl_pkppi*)ret)->setUseKinfit(param.value(2).value<bool>());
    ((Rpp_pkl_pkppi*)ret)->setUseDecay(param.value(3).value<bool>());
    ret->setChiLimit(param.value(7).value<float>());
    ((Rpp_pkl_pkppi*)ret)->setMissingMassLimit(param.value(4).value<float>());
    ((Rpp_pkl_pkppi*)ret)->setAngleLimit(param.value(5).value<float>());
    ((Rpp_pkl_pkppi*)ret)->setMomentumLimit(param.value(6).value<float>());
    ((Rpp_pkl_pkppi*)ret)->setMaximumProtonAngle(param.value(8).value<float>());
    auto p = param.value(10).value<std::vector<float>>();
    ((Rpp_pkl_pkppi*)ret)->setAngleRangesPK(p.at(0), p.at(1));
    ((Rpp_pkl_pkppi*)ret)->setAngleRangesDecay(p.at(2), p.at(3));
    p = param.value(11).value<std::vector<float>>();
    ((Rpp_pkl_pkppi*)ret)->setTOFResolution(p.at(0), p.at(1), p.at(2), p.at(3));
    auto v = param.value(0).value<std::vector<int>>();
    ((Rpp_pkl_pkppi*)ret)->setRepresentation(v.at(0), v.at(1), v.at(2), v.at(3));
    break;
  }
  }
  return ret;
}
