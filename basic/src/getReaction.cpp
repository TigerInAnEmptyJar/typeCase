#include "reactions.h"
#include "geometry.h"
#include "Eparticles.h"
#include "algorithmparameter.h"
RbaseReaction *getReactionFromName(const string &name)
{
  if(name=="pp-elastic")
    return new Rppelastic();
  if(name=="ppelastic")
    return new Rppelastic();
  if(lower(name)=="pp-pkl-pkppi")
    return new Rpp_pkl_pkppi();
  if(lower(name)=="dpi+"||lower(name)=="pp-dpi+"||
     lower(name)=="dpiplus"||lower(name)=="pp-dpiplus")
    return new Rpp_dpiPlus();
//   if(name.lower()=="pp-pks-ppipippi")
//     return new Rpp_pks_ppipippi();
  return NULL;
}
RbaseReaction *getReactionFromName(const string &name,momentum4D m1, momentum4D m2)
{
  if(name=="pp-elastic"||name=="ppelastic"||name=="ppel")
    return new Rppelastic(NULL,m1,m2);
  if(lower(name)=="pp-pkl-pkppi")
    return new Rpp_pkl_pkppi(NULL,m1,m2);
  if(lower(name)=="dpi+"||lower(name)=="pp-dpi+"||
     lower(name)=="dpiplus"||lower(name)=="pp-dpiplus")
    return new Rpp_dpiPlus(NULL,m1,m2);
//   if(name.lower()=="pp-pks-ppipippi")
//     return new Rpp_pks_ppipippi();
  return NULL;
}
RbaseReaction *getReaction(const algorithm_parameter &param, momentum4D m1, momentum4D m2)
{
  RbaseReaction *ret=NULL;
  if(!param.IsUsed())return NULL;
  switch(param.getID())
    {
    case 0://pp-elastic
      {
	//	anaLog<<"test pp-elastic "<<endli;
	ret=new Rppelastic(NULL,m1,m2);
	ret->setChiLimit(param.getParam<float>(0).getData());
	if(param.getNumberOfParam<float>()>=3)
	  {
	    ((Rppelastic*)ret)->setCoplanarityDifference(param.getParam<float>(1).getData());
	    ((Rppelastic*)ret)->setGammaDifference(param.getParam<float>(2).getData());
	  }
	break;
      }
    case 1://d-pi+
      {
	//	anaLog<<"test pp-dpi+ "<<endli;
	ret=new Rpp_dpiPlus(NULL,m1,m2);
	ret->setChiLimit(param.getParam<float>(0).getData());
	if(param.getNumberOfParam<float>()>=4)
	  {
	    ((Rpp_dpiPlus*)ret)->setCoplanarityDifference(param.getParam<float>(1).getData());
	    ((Rpp_dpiPlus*)ret)->setAngleLimit(param.getParam<float>(2).getData());
	    ((Rpp_dpiPlus*)ret)->setCrossoverMomentum(param.getParam<float>(3).getData());
	  }
	break;
      }
    case 10://PK\\Lambda
      {
	//	anaLog<<"test pp- p-Kaon-Lambda "<<endli;
	ret=new Rpp_pkl_pkppi(NULL,m1,m2);
	ret->setChiLimit(param.getParam<float>(3).getData());
	((Rpp_pkl_pkppi*)ret)->setMissingMassLimit(param.getParam<float>(0).getData());
	((Rpp_pkl_pkppi*)ret)->setAngleLimit(param.getParam<float>(1).getData());
	((Rpp_pkl_pkppi*)ret)->setExact(param.getParam<bool>(0).getData());
	((Rpp_pkl_pkppi*)ret)->setMultiLineFit(param.getParam<bool>(1).getData());
	((Rpp_pkl_pkppi*)ret)->setUseKinfit(param.getParam<bool>(2).getData());
	((Rpp_pkl_pkppi*)ret)->setMomentumLimit(param.getParam<float>(2).getData());
	((Rpp_pkl_pkppi*)ret)->setAngleRangesPK(param.getParam<vector<float> >(0).getData().at(0),
						param.getParam<vector<float> >(0).getData().at(1));
	((Rpp_pkl_pkppi*)ret)->setAngleRangesDecay(param.getParam<vector<float> >(0).getData().at(2),
						   param.getParam<vector<float> >(0).getData().at(3));
	((Rpp_pkl_pkppi*)ret)->setMaximumProtonAngle(param.getParam<float>(4).getData());
	((Rpp_pkl_pkppi*)ret)->setUseDecay(param.getParam<bool>(3).getData());
	((Rpp_pkl_pkppi*)ret)->setTOFResolution(param.getParam<vector<float> >(1).getData().at(0),
						param.getParam<vector<float> >(1).getData().at(1),
						param.getParam<vector<float> >(1).getData().at(2),
						param.getParam<vector<float> >(1).getData().at(3));
	((Rpp_pkl_pkppi*)ret)->setRepresentation(param.getParam<vector<int> >(0).getData().at(0),
						 param.getParam<vector<int> >(0).getData().at(1),
						 param.getParam<vector<int> >(0).getData().at(2),
						 param.getParam<vector<int> >(0).getData().at(3));
	break;
      }
    }
  return ret;
}
