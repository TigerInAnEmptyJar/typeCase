//#include "algorithms.h"
#include "logger.h"
#include <qdatetime.h>
#include "tofAnalysis_t.h"
#include "shapes.h"
//#include "baseShape.h"

extern base_shape *getShape(const shape_parameter&sp);
volumeShape *tofAnalysis::getVShape(shape_parameter &sp)
{
  base_shape *sh=getShape(sp);
  if(sh==NULL)return new volumeShape("none");
  if(sh->getName()=="wedge"||sh->getName()=="spiral"||sh->getName()=="fiber"
     ||sh->getName()=="ring"||sh->getName()=="cylinder"||sh->getName()=="hexPrism"
     ||sh->getName()=="strawTube")
    return (volumeShape*)sh;

    volumeShape*shape;
	shape=new volumeShape("none");
    return shape;
}



