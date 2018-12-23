#include "shapedisplay.h"
#include "qspinbox.h"
#include "geometry.h"
#include "baseShape.h"
extern base_shape* getShape(const shape_parameter &sh);
void ShapeDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void ShapeDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void ShapeDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
  nameLabel->setGeometry(1,1,50,20);
  if(nameInput!=NULL)nameInput->setGeometry(55,1,(int)(w/(DrawShape?2:1))-55,20);
  float wfrac=0.5;
  QRect geo2;
  if(DrawShape)
    {
      descLabel->setGeometry((int)(w*wfrac)+2,(int)((1-fraction)*h)+2,(int)(w*(1-wfrac))-2,20);
      descriptionEdit->setGeometry((int)(w*wfrac)+2,(int)((1-fraction)*h)+4+20,(int)(w*(1-wfrac))-2,(int)((1-fraction)*h)-26);
      shapeShow->setGeometry((int)(w*wfrac)+2,1,(int)(w*(1-wfrac))-2,(int)(fraction*h)-2);
      geo2=QRect(1,44,(int)(w*wfrac)-2,h-45);
    }
  else
    {
      descLabel->setGeometry(-1,-1,0,0);
      descriptionEdit->setGeometry(-1,-1,0,0);
      shapeShow->setGeometry(-1,-1,0,0);
      geo2=QRect(1,44,w-2,h-45);
    }
#if QT_VERSION > 0x040000
    helpGroup->setGeometry(geo2);  
#else
    propertyGroup->setGeometry(geo2);
#endif
}
void ShapeDisplay::setShape( shape_parameter & shapeIn )
{
  sp=&shapeIn;
  copy=shapeIn;
  while(!properties.empty())
    {
      QWidget* tmp=properties.back();
      properties.pop_back();
      disconnect(tmp);
      delete tmp;
    }
  if(ReadOnly||IsConstant)
    ((QLabel*)nameInput)->setText(copy.getName().data());
  else
    ((QLineEdit*)nameInput)->setText(copy.getName().data());
#if QT_VERSION < 0x040000
  propertyGroup->setColumnLayout(2,Qt::Horizontal);
#endif
  for(int i=0;i<copy.NumberOfParams<int>();i++)
    {
      addItemToLayout(0, copy.getParamName<int>(i), "intP");
      ((QSpinBox*)properties.back())->setValue(copy.getParam<int>(i));
      connect(properties.back(),SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    }
  for(int i=0;i<copy.NumberOfParams<float>();i++)
    {
      addItemToLayout(1, copy.getParamName<float>(i), "floatP");
      ((QLineEdit*)properties.back())->setText(string_number(copy.getParam<float>(i)).data());
      if(ReadOnly)
	((QLineEdit*)properties.back())->setReadOnly(true);
      else
	connect(properties.back(),SIGNAL(textChanged(const QString &)),this,SLOT(onPropertyChange()));
    }
  for(int i=0;i<copy.NumberOfParams<point3D>();i++)
    {
      addItemToLayout(2, copy.getParamName<point3D>(i), "pointP");
      ((Q3DInput*)properties.back())->setValues(copy.getParam<point3D>(i).X(),copy.getParam<point3D>(i).Y(),copy.getParam<point3D>(i).Z());
      connect(properties.back(),SIGNAL(valueChanged(float,float,float)),this,SLOT(onPropertyChange()));
    }
  for(int i=0;i<copy.NumberOfParams<vector3D>();i++)
    {
      addItemToLayout(3, copy.getParamName<vector3D>(i), "vectorP");
      ((Q3DInput*)properties.back())->setValues(copy.getParam<vector3D>(i).Theta()*180./M_PI,copy.getParam<vector3D>(i).Phi()*180./M_PI,copy.getParam<vector3D>(i).R());
      connect(properties.back(),SIGNAL(valueChanged(float,float,float)),this,SLOT(onPropertyChange()));
    }
  int nLines=0;
  for(unsigned int i=0;i<properties.size();i++)
    {
      nLines++;
      properties[i]->setEnabled(true);
      properties[i]->show();
    }
  nLines=nLines/2+2;
  setMinimumHeight(nLines*23
#if QT_VERSION < 0x040000
		   +2*(nLines-2)
#endif
);
  resize(width(),height());
}
void ShapeDisplay::OnShapeDraw()
{
  if(drawer!=NULL)delete drawer;
  drawer=getShape(copy);
  if(drawer==NULL)return;
  if(drawer->getName()=="none")
    return;
  shapeShow->GetCanvas()->cd()->Clear();
  vector4D bb;
  drawer->Draw(point3D(0,0,0),plane3D(point3D(0,0,1),vector3D(0,0,1)),&bb);
  float dx,dy,mx,my;
  dx=bb[2]-bb[0];
  dy=bb[3]-bb[1];
  mx=bb[0]+dx/2.;
  my=bb[1]+dy/2.;
  shapeShow->GetCanvas()->Range(mx-dx*0.6,my-dy*0.6,mx+dx*.6,my+dy*.6);
  shapeShow->Refresh();
}
void ShapeDisplay::setReadOnly( bool value )
{
  if(ReadOnly==value)return;
  ReadOnly=value;
  if(nameInput!=NULL)
    {
      delete nameInput;
      nameInput=NULL;
    }
  if(ReadOnly||IsConstant)
    {
#if QT_VERSION < 0x040000
      nameInput=new QLabel(this,"nameInput");
#else
      nameInput=new QLabel(this);nameInput->setObjectName("nameInput");
#endif
      ((QLabel*)nameInput)->setText(copy.getName().data());
    }
  else
    {
#if QT_VERSION < 0x040000
      nameInput=new QLineEdit(this,"nameInput");
#else
      nameInput=new QLineEdit(this);nameInput->setObjectName("nameInput");
#endif
      ((QLineEdit*)nameInput)->setText(copy.getName().data());
      connect(nameInput,SIGNAL(textChanged(const QString &)),this,SLOT(onPropertyChange()));
    }
  resize(width(),height());
  nameInput->setEnabled(true);
  nameInput->show();
}
void ShapeDisplay::setConstant( bool value )
{
  if(IsConstant==value)return;
  IsConstant=value;
}
void ShapeDisplay::init()
{
  ReadOnly=true;
  IsConstant=false;
  DrawShape=true;
  fraction=0.5;
  drawer=NULL;
#if QT_VERSION < 0x040000
  propertyGroup->setColumnLayout(2,Qt::Horizontal);
      nameInput=new QLabel(this,"nameInput");
#else
      nameInput=new QLabel(this);nameInput->setObjectName("nameInput");
#endif
  nameInput->show();
}
void ShapeDisplay::destroy()
{
  if(drawer!=NULL)delete drawer;
  while(!properties.empty())
    {
      QWidget* tmp=properties.back();
      properties.pop_back();
      delete tmp;
    }
}
void ShapeDisplay::onPropertyChange()
{
  QWidget* focusWidget=NULL;
  if(nameInput!=NULL)
    {
      if(nameInput->hasFocus()&&!ReadOnly)
	{
	  copy.setName(string(((QLineEdit*)nameInput)->text()
#if QT_VERSION < 0x040000
		       .ascii()));
#else
	  .toAscii()));
#endif
      emit changed();
	  return;
	}
    }
  int n=-1;
  for(unsigned int i=0;i<properties.size();i++)
    {
#if QT_VERSION < 0x040000
      string s=string(properties[i]->name());
#else
      string s=string(properties[i]->objectName().toAscii());
#endif
      if(s=="pointP"||s=="vectorP")
	{
	if(((Q3DInput*)properties[i])->hasFocus())
	  {
	    n=i;
	    focusWidget=properties[i];
	  }
	}
      else
	{
	  if(properties[i]->hasFocus())
	    {
	      n=i;
	      focusWidget=properties[i];
	    }
	}
    }
  if(n<1||focusWidget==NULL)return;
#if QT_VERSION < 0x040000
string s=focusWidget->name();
#else
string s=string(focusWidget->objectName().toAscii());
#endif
  if(s=="intP")
    {
      for(int i=0;i<copy.NumberOfParams<int>();i++)
	{
	  if(((QLabel*)properties[n-1])->text()==copy.getParamName<int>(i).data())
	  {
	    if(ReadOnly||IsConstant)
	      ((QSpinBox*)focusWidget)->setValue(copy.getParam<int>(i));
	    else
	      {
		copy.setParam<int>(i,((QSpinBox*)focusWidget)->value());
		emit changed();
	      }
	  }
	}
    }
  else if(s=="floatP")
    {
      for(int i=0;i<copy.NumberOfParams<float>();i++)
	if(((QLabel*)properties[n-1])->text()==copy.getParamName<float>(i).data())
	  if(!(ReadOnly||IsConstant))
	    {
	      copy.setParam<float>(i,((QLineEdit*)focusWidget)->text().toFloat());
	      emit changed();
	    }
    }
  else if(s=="pointP")
    {
      for(int i=0;i<copy.NumberOfParams<point3D>();i++)
	if(((QLabel*)properties[n-1])->text()==copy.getParamName<point3D>(i).data())
	  {
	    if(ReadOnly||IsConstant)
	      ((Q3DInput*)focusWidget)->setValues(copy.getParam<point3D>(i).X(),
						  copy.getParam<point3D>(i).Y(),
						  copy.getParam<point3D>(i).Z());
	    else
	      {
		copy.setParam<point3D>(i,point3D(((Q3DInput*)focusWidget)->getX(),
					       ((Q3DInput*)focusWidget)->getY(),
					       ((Q3DInput*)focusWidget)->getZ()));
		emit changed();
	      }

	  }
    }
  else if(s=="vectorP")
    {
      for(int i=0;i<copy.NumberOfParams<vector3D>();i++)
	if(((QLabel*)properties[n-1])->text()==copy.getParamName<vector3D>(i).data())
	  {
	    if(ReadOnly||IsConstant)
	      ((Q3DInput*)focusWidget)->setValues(copy.getParam<vector3D>(i).Theta()*180./M_PI,
						  copy.getParam<vector3D>(i).Phi()*180./M_PI,
						  copy.getParam<vector3D>(i).R());
	    else
	      {
		float t=((Q3DInput*)focusWidget)->getX()/180.*M_PI,
		  p=((Q3DInput*)focusWidget)->getY()/180.*M_PI,
		  r=((Q3DInput*)focusWidget)->getZ();
		copy.setParam<vector3D>(i,r*vector3D(sin(t)*cos(p),sin(t)*sin(p),cos(t)));
		emit changed();
	      }
	  }
    }
  if(!(ReadOnly||IsConstant))
    (*sp)=copy;
}
bool ShapeDisplay::readOnly()
{return ReadOnly;}
bool ShapeDisplay::isConstant()
{return IsConstant;}
bool ShapeDisplay::doDraw()
{return DrawShape;}
void ShapeDisplay::setDraw(bool value)
{
  if(DrawShape==value)return;
  DrawShape=value;
  resize(width(),height());
}
void ShapeDisplay::setFraction(float f)
{fraction=f;resize(width(),height());}
point2D ShapeDisplay::pointOnCanvas(const QPoint &p)
{
    double x1,x2,y1,y2;
    shapeShow->GetCanvas()->GetRange(x1,y1,x2,y2);
    int w=shapeShow->geometry().width(),h=shapeShow->geometry().height();
    double xp=x1+((double)(p.x()))* ((double)(x2-x1))/ ((double)w);
    double yp=y2+((double)(p.y()))* ((double)(y1-y2))/ ((double)h);
    return point2D(xp,yp);
}
void ShapeDisplay::mouseOnCanvas(const QPoint &p)
{
/*     point2D pp=pointOnCanvas(p); */
/*     QString ret=""; */
/*     for(unsigned int i=0;i<elementDrawings.size();i++) */
/*     { */
/* 	if(elementDrawings[i]==NULL)continue; */
/* 	if(elementDrawings[i]->IsA()==TPolyLine::Class()) */
/* 	{ */
/* 	    TCutG cut("cut1",((TPolyLine*)elementDrawings[i])->GetN(), */
/* 		      ((TPolyLine*)elementDrawings[i])->GetX(), */
/* 		      ((TPolyLine*)elementDrawings[i])->GetY()); */
/* 	    if(cut.IsInside(pp.x(),pp.y())) */
/* 		ret=ret+"element:"+QString::number(i)+"\n"; */
/* 	} */
/*     } */
/*     QToolTip::remove(shapeShow); */
/*     QToolTip::add(shapeShow,ret); */
}
void ShapeDisplay::addItemToLayout(int tp, string name, string widgetName)
{
  QWidget *theWidget=NULL;
  switch(tp)
    {
    case 0:
#if QT_VERSION < 0x040000 
      theWidget=new QSpinBox(propertyGroup,widgetName.data());
#else 
      theWidget=new QSpinBox(helpGroup); 
      theWidget->setObjectName(widgetName.data());
#endif
	break;
    case 1:
#if QT_VERSION < 0x040000 
      theWidget=new QLineEdit(propertyGroup,widgetName.data());
#else 
      theWidget=new QLineEdit(helpGroup); 
      theWidget->setObjectName(widgetName.data());
#endif
	break;
    case 2:
    case 3:
#if QT_VERSION < 0x040000 
      theWidget=new Q3DInput(propertyGroup,widgetName.data());
#else 
      theWidget=new Q3DInput(helpGroup); 
      theWidget->setObjectName(widgetName.data());
#endif
	break;
    }
  if(theWidget==NULL)return;
#if QT_VERSION < 0x040000
  properties.push_back(new QLabel(name.data(),propertyGroup));
/*   propertyGroup->addSpace(5); */
/*   propertyGroup->addSpace(5); */
#else
  properties.push_back(new QLabel(name.data(),helpGroup));
  propertyGroup->addWidget(properties.back(),(properties.size()+1)/2,0,1,1,Qt::AlignLeft|Qt::AlignVCenter);
  propertyGroup->addWidget(theWidget,(properties.size()+1)/2,1,1,1,Qt::AlignJustify|Qt::AlignVCenter);
  theWidget->show();
#endif
  properties.push_back(theWidget);
}
