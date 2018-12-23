#include "pixel.h"
TPixel::TPixel(int numDetIn):TBase("TPixel"),numDet(numDetIn)
{
  elements=new TCalibHit*[numDet];//TRef[numDet];
  accNum=0;
  defined=false;
  planeShape *tmp=0;//new planeShape("none");
  p_shape=(tmp);
  emergencySave=NULL;
  valid=false;
}

TCalibHit &TPixel::getElementr(int num)
{
  defined=true;
  if((num<0)||(num>numDet)) 
    {
      return *(elements[0]);
    }
  return *(elements[num]);
}


void TPixel::addElement(TCalibHit &hit)
{
  defined=true;
  if(accNum>numDet-1)return;
  elements[accNum]=(&hit);//[accNum]=&hit;//.SetObject(&hit);
  accNum++;
}

TPixel::~TPixel()
{
  delete [] elements;
  //  delete p_shape;
}

planeShape *TPixel::getShape() 
{
  if(p_shape!=emergencySave)
    return emergencySave;
  return p_shape;//((planeShape*)p_shape.GetObject());
}

TCalibHit TPixel::getElement(int num) const
{
  if((num<0)||(num>accNum)) return TCalibHit();
  return *(elements[(num)]);//((TCalibHit*)elements[num].GetObject());
}

int TPixel::getNumberOfElements() const
{
  return numDet;
}

int TPixel::getNumberOfActualElements() const
{
  return accNum;
}

void TPixel::setShape(planeShape *sh)
{
  defined=true;
  //  planeShape *tmp=p_shape;//((planeShape*)p_shape.GetObject());
  p_shape=(sh);
  emergencySave=sh;
  if(p_shape!=sh)
    {
      p_shape=(sh);
      if(p_shape!=sh)
	{
	  //  cout<<ID<<" "<<sh<<" "<<p_shape.GetObject()<<endl;
	  p_shape=(NULL);
	  setValid(false);
	}
    }
  //  delete tmp;
  //  if(!p_shape.GetObject()) p_shape=TRef(new planeShape("none"));
}
void TPixel::resetElements()
{
  accNum=0;
  //  elements.Clear();
}

bool TPixel::isDefined()
{
  return defined;
}

int TPixel::getID()const
{
  return ID;
}

void TPixel::setID(int IDIn)
{
  ID=IDIn;
}

float TPixel::getMaxChiSquared()const
{
  return maxChi;
}

void TPixel::setMaxChiSquared(float value)
{
  maxChi=value;
}
void TPixel::reset()
{
  valid=false;
//   planeShape *ps=new planeShape("none");
//   planeShape *tmp=getShape();
//   setShape(ps);
//   if(!tmp)
//     delete tmp;
  resetElements();
  //  emergencySave=NULL;
  maxChi=1000000;
}

bool TPixel::operator ==(TPixel &p)
{
  if(accNum!=p.getNumberOfActualElements()) return false;
  if(ID!=p.getID()) return false;
  bool tmp[accNum];
  for(int i=0;i<accNum;i++)
    {
      tmp[i]=false;
      for(int j=0;j<accNum;j++)
	{
	  if((elements[(i)])/*.GetObject()*/==&p.getElementr(j)) tmp[i]=true;
	}
    }
  for(int i=0;i<accNum;i++)
    if(!(tmp[i])) return false;
  return true;
}

bool TPixel::isValid()const
{
  return valid;
}

void TPixel::setValid(bool v)
{
  valid=v;
}
