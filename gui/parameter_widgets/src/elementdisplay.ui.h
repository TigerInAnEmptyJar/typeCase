#include "string_utilities.h"
void ElementDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void ElementDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void ElementDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
  int bh=23;
  int abt=1;
  if(h>bh*8)
    {
      abt=(h-bh*8)/9;
      nameLabel->setGeometry(1,abt,w,bh);
      massLabel->setGeometry(1,bh*2+abt*3,w,bh);
      chargeLabel->setGeometry(1,bh*4+abt*5,w,bh);
      weightLabel->setGeometry(1,bh*6+abt*7,w,bh);
      if(nameInput!=NULL)nameInput->setGeometry(1,bh*1+abt*2,w,bh);
      if(massInput!=NULL)massInput->setGeometry(1,bh*3+abt*4,w,bh);
      if(chargeInput!=NULL)chargeInput->setGeometry(1,bh*5+abt*6,w,bh);
      if(weightInput!=NULL)weightInput->setGeometry(1,bh*7+abt*8,w,bh);
    }
  else if(h>bh*4&&w>100)
    {
      abt=(h-bh*4)/5;
      nameLabel->setGeometry(1,abt,(w-3)/2,bh);
      massLabel->setGeometry(1,bh*1+abt*2,(w-3)/2,bh);
      chargeLabel->setGeometry(1,bh*2+abt*3,(w-3)/2,bh);
      weightLabel->setGeometry(1,bh*3+abt*4,(w-3)/2,bh);
      if(nameInput!=NULL)nameInput->setGeometry((w-3)/2+2,bh*0+abt*1,(w-3)/2,bh);
      if(massInput!=NULL)massInput->setGeometry((w-3)/2+2,bh*1+abt*2,(w-3)/2,bh);
      if(chargeInput!=NULL)chargeInput->setGeometry((w-3)/2+2,bh*2+abt*3,(w-3)/2,bh);
      if(weightInput!=NULL)weightInput->setGeometry((w-3)/2+2,bh*3+abt*4,(w-3)/2,bh);
    }
  else
    {
      abt=1;
      nameLabel->setGeometry(1,abt,w,bh);
      massLabel->setGeometry(1,bh*2+abt*3,w,bh);
      chargeLabel->setGeometry(1,bh*4+abt*5,w,bh);
      weightLabel->setGeometry(1,bh*6+abt*7,w,bh);
      if(nameInput!=NULL)nameInput->setGeometry(1,bh*1+abt*2,w,bh);
      if(massInput!=NULL)massInput->setGeometry(1,bh*3+abt*4,w,bh);
      if(chargeInput!=NULL)chargeInput->setGeometry(1,bh*5+abt*6,w,bh);
      if(weightInput!=NULL)weightInput->setGeometry(1,bh*7+abt*8,w,bh);
    }
}
void ElementDisplay::setElement( element_parameter & elementIn )
{
  sp=&elementIn;
  copy.setName(sp->getName());
  copy.setMass(sp->getMass());
  copy.setCharge(sp->getCharge());
  copy.setWeight(sp->getWeight());
  copy=elementIn;
  ((QLineEdit*)nameInput)->setText(copy.getName().data());
  ((QLineEdit*)massInput)->setText(string_number(copy.getMass()).data());
  ((QLineEdit*)chargeInput)->setText(string_number(copy.getCharge()).data());
  ((QLineEdit*)weightInput)->setText(string_number(copy.getWeight()).data());
}

void ElementDisplay::setReadOnly( bool value )
{
  if(ReadOnly==value)return;
  ReadOnly=value;
  nameInput->setReadOnly(value);
  massInput->setReadOnly(value);
  chargeInput->setReadOnly(value);
  weightInput->setReadOnly(value);
}

void ElementDisplay::init()
{
  ReadOnly=false;
}
void ElementDisplay::destroy()
{
}
void ElementDisplay::onPropertyChange()
{
  if(nameInput==focusWidget())
    {
      copy.setName(string(((QLineEdit*)nameInput)->text().
#if QT_VERSION < 0x040000
		   ascii()));
#else
      toAscii()));
#endif
    }
  else if(weightInput==focusWidget())
    {
      copy.setWeight(((QLineEdit*)weightInput)->text().toFloat());
    }
  else if(chargeInput==focusWidget())
    {
      copy.setCharge(((QLineEdit*)chargeInput)->text().toFloat());
    }
  else if(massInput==focusWidget())
    {
      copy.setMass(((QLineEdit*)massInput)->text().toFloat());
    }
  if(!ReadOnly)
    {
      sp->setName(copy.getName());
      sp->setMass(copy.getMass());
      sp->setCharge(copy.getCharge());
      sp->setWeight(copy.getWeight());
    }
  emit changed();
}
bool ElementDisplay::readOnly()
{return ReadOnly;}
