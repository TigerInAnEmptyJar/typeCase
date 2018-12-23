#include "cluster.h"
TCluster::TCluster(int maxElements):TBase("TCluster"),numElem(maxElements)
{
  accElem=0;
  cpoint=point3D(0,0,0);
  pix=new TPixel*[numElem];
  defined=false;
}

void TCluster::addPixel(TPixel &p)
{
  defined=true;
  if(accElem>=numElem-1) return;
  pix[accElem]=&p;//.Add(&p);
  accElem++;
}

TPixel &TCluster::getPixelr(int num)
{
  defined=true;
  if((num<0)||(num>accElem)) 
    {
      return *pix[0];
    }
  return *(pix[num]);//.GetObject());
}
TCluster::~TCluster()
{
  delete [] pix;
}

int TCluster::getNumberOfPixels() const
{
  return accElem;
}

int TCluster::getMaxNumberOfPixels() const
{
  return numElem;
}

point3D TCluster::getCenter() const
{
  return cpoint;
}

TPixel TCluster::getPixel(int num) const
{
  if((num<0)||(num>accElem)) return TPixel();
  return *(pix[num]);//.GetObject());
}

void TCluster::setCenter(point3D p)
{
  defined=true;
  cpoint=p;
}

void TCluster::setID(int IDin)
{
  id=IDin;
}
int TCluster::getID()const
{
  return id;
}
int TCluster::ID()const
{
  return id;
}

void TCluster::resetPixel()
{
  accElem=0;
  //  pix.Clear();
}

point3D &TCluster::getCenterr()
{
  defined=true;
  return cpoint;
}
vector3D TCluster::getClusterRadius()const
{ 
  if(accElem<=0)return vector3D();
  return radius*pix[0]->getShape()->getNormal();
}
void TCluster::setClusterRadius(float value)
{ 
  radius=value;
}
void TCluster::calculateClusterRadius()
{
  radius=0;
  vector3D dist;
  for(int i=0;i<accElem;i++)
    {
      dist=(pix[i])->getShape()->getCenter()-cpoint;
      if(dist.R()>radius)radius=dist.R();
    }
  if(accElem>0)
    radius=radius+(pix[0])->getShape()->circumscribedRadius();
}
void TCluster::reset()
{
  resetPixel();
  radius=0;
  cpoint=point3D();
}
bool TCluster::isDefined()
{
    return defined;
}
