#include "utilities.h"
bool DetectorDisplay::readOnly()
{return ReadOnly;}
bool DetectorDisplay::doDraw()
{return DrawShape;}
void DetectorDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void DetectorDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void DetectorDisplay::setFraction(float f)
{fraction=f;}
void DetectorDisplay::setDraw(bool value)
{DrawShape=value;resize(width(),height());}
int getShapeHeight(const shape_parameter &sh)
{
  int np=0;
  np=np+sh.NumberOfParams<int>();
  np=np+sh.NumberOfParams<float>();
  np=np+sh.NumberOfParams<point3D>();
  np=np+sh.NumberOfParams<vector3D>();
  return (np+1)*20+45;
}
void DetectorDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
  int bh=23;
  int mh=6*bh;
  float wfrac=0.4;
  int ew=(int)(w*wfrac);
  int sh=getShapeHeight(copyshape);
  int abt=1;
  int lw=60;
  if(abs(sh-mh)<mh/3)
    {
    }
  /* propertyGroup */shap->setGeometry(ew+1,1,w-ew-2,h-10); 
  //  shap->setGeometry(0,0,ew-2,h-bh-10); 
  shapeShow->setGeometry(1,bh*6+abt*7,ew-2,h-1-bh*6-abt*7); 
  nameLabel->setGeometry(1,abt,lw,bh);
  if(nameInput!=NULL)nameInput->setGeometry(lw+5,abt,ew-(lw+6),bh);
  idLabel->setGeometry(1,bh+abt*2,lw,bh);
  idBox->setGeometry(lw+5,bh+abt*2,ew-(lw+6),bh);
  NElLabel->setGeometry(1,bh*2+abt*3,lw,bh);
  elBox->setGeometry(lw+5,bh*2+abt*3,ew-(lw+6),bh);
  stackLabel->setGeometry(1,bh*3+abt*4,lw,bh);
  stackBox->setGeometry(lw+5,bh*3+abt*4,ew-(lw+6),bh);
  matLabel->setGeometry(1,bh*4+abt*5,lw,bh);
  matBox->setGeometry(lw+5,bh*4+abt*5,ew-(lw+6),bh);
  circularBox->setGeometry(1,bh*5+abt*6,ew-6,bh);
}
void DetectorDisplay::setDetector( detector_parameter & detectorIn )
{
  sp=&detectorIn;
  copy=detectorIn;
  copy.setID(detectorIn.getID());
  copyshape=copy.getShape();
  if(nameInput!=NULL)
    {
      if(ReadOnly)
	((QLabel*)nameInput)->setText(copy.getName().data());
      else
	((QLineEdit*)nameInput)->setText(copy.getName().data());
    }
  int tmpI=copy.getNumberOfElements();
  elBox->setValue(tmpI);
  idBox->setValue(copy.getID());
  stackBox->setValue(copy.getStackType());
  setComboBoxCurrentItem(matBox,copy.getMaterial());
  circularBox->setChecked(copy.isCircular());
  currentShapeType=-1;
  for(unsigned int i=0;i<defined_shapes.size();i++)
    {
      modShapes[i]=defined_shapes[i];
      if(copyshape.getName()==defined_shapes[i].getName())
	currentShapeType=i;
    }
  if(currentShapeType<0)
    {
      defined_shapes.push_back(copyshape);
      modShapes.push_back(copyshape);
      currentShapeType=modShapes.size()-1;
    }
  modShapes[currentShapeType]=copyshape;
  shap->setShape(modShapes[currentShapeType]);
  setMinimumHeight((shap->minimumHeight()<5*23+7?5*23+7:shap->minimumHeight()));
}
extern base_shape* getShape(const shape_parameter& param);
#include "volumeShape.h"
void DetectorDisplay::OnDetectorDraw()
{
  if(single!=NULL)
    { 
      delete single;
      single=NULL;
    }
  if(drawer!=NULL)
    {
      delete drawer;
      drawer=NULL;
    }
  single=getShape(copyshape);
  if(single==NULL)return;
  if(single->getName()=="none")return;
  drawer=((volumeShape*)single)->getEnvelope(copy.getNumberOfElements(),copy.getStackType());
  if(drawer==NULL)return;
  vector4D bb;
  shapeShow->GetCanvas()->cd()->Clear();
  single->Draw(point3D(0,0,0),plane3D(point3D(0,0,1),vector3D(0,0,1)),&bb);
  drawer->Draw(point3D(0,0,0),plane3D(point3D(0,0,1),vector3D(0,0,1)),&bb);
  single->Draw(point3D(0,0,0),plane3D(point3D(0,0,1),vector3D(0,0,1)),NULL,NULL,1,2,1001);
  float dx,dy,mx,my;
  dx=bb[2]-bb[0];
  dy=bb[3]-bb[1];
  mx=bb[0]+dx/2.;
  my=bb[1]+dy/2.;
  shapeShow->GetCanvas()->Range(mx-dx*0.6,my-dy*0.6,mx+dx*.6,my+dy*.6);
  shapeShow->Refresh();
}
void DetectorDisplay::setReadOnly( bool value )
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  shap->setReadOnly(value);
  if(nameInput!=NULL)delete nameInput;
  if(ReadOnly)
    {
      nameInput=new QLabel(this);
      ((QLabel*)nameInput)->setText(copy.getName().data());
    }
  else
    {
      nameInput=new QLineEdit(this);
      ((QLineEdit*)nameInput)->setText(copy.getName().data());
    }
#if QT_VERSION < 0x040000
      nameInput->setName("nameInput");
#else
      nameInput->setObjectName("nameInput");
#endif
  nameInput->setGeometry(60+5,1,width()/2-(60+6),23);
  nameInput->show();
  idBox->setEnabled(!ReadOnly);
  elBox->setEnabled(!ReadOnly);
  stackBox->setEnabled(!ReadOnly);
  matBox->setEnabled(!ReadOnly);
  circularBox->setEnabled(!ReadOnly);
}
void DetectorDisplay::onPropertyChange()
{
  QObject *tmp=focusWidget();
  if(tmp==NULL)return;
  if(tmp->parent()==NULL)return;
  while(tmp->parent()!=NULL&&tmp->parent()!=this)tmp=tmp->parent();
  if(tmp->parent()==NULL)return;
  if(tmp!=NULL)
    {
      if(tmp==idBox)
	{
	  if(ReadOnly)
	      idBox->setValue(copy.getID());
	  else
	    copy.setID(idBox->value());
	}
      else if(tmp==elBox)
	{
	  if(ReadOnly)
	    elBox->setValue(copy.getNumberOfElements());
	  else
	    copy.setNumberOfElements(elBox->value());
	}
      else if(tmp==stackBox)
	{
	  if(ReadOnly)
	    stackBox->setValue(copy.getStackType());
	  else
	    copy.setStackType(stackBox->value());
	}
      else if(tmp==matBox)
	{
	  if(ReadOnly)
	    setComboBoxCurrentItem(matBox,copy.getMaterial());
	  else
	    copy.setMaterial(comboBoxCurrentItem(matBox));
	}
      else if(tmp==circularBox)
	{
	  if(ReadOnly)
	    circularBox->setChecked(copy.isCircular());
	  else
	    copy.setCircular(circularBox->isChecked());
	}
      else if(tmp==nameInput)
	{
#if QT_VERSION < 0x040000
	  copy.setName(((QLineEdit*)nameInput)->text().ascii());
#else
	  copy.setName(string(((QLineEdit*)nameInput)->text().toAscii()));
#endif
	}
    }
  if(!ReadOnly)
    {
      copy.setShape(copyshape);
      (*sp)=copy;
      sp->setID(copy.getID());
      emit changed();
    }
}
void DetectorDisplay::init()
{
  ReadOnly=false;
  shap->setReadOnly(false); 
  single=NULL;
  drawer=NULL;
  nameInput=new QLineEdit(this);
  nameInput->show();
  shap->show();
}
void DetectorDisplay::destroy()
{
  if(single!=NULL)
    { 
      delete single;
      single=NULL;
    }
  if(drawer!=NULL)
    {
      delete drawer;
      drawer=NULL;
    }
}
void DetectorDisplay::setMaterials(const vector<string>& mats)
{
  while(!materials.empty())materials.pop_back();
  matBox->clear();
  for(unsigned int i=0;i<mats.size();i++)
    {
      materials.push_back(mats[i]);
      add2ComboBox(matBox,mats[i].data());
    }
  setComboBoxCurrentItem(matBox,copy.getMaterial());
}
point2D DetectorDisplay::pointOnCanvas(const QPoint &p)
{
    double x1,x2,y1,y2;
    shapeShow->GetCanvas()->GetRange(x1,y1,x2,y2);
    int w=shapeShow->geometry().width(),h=shapeShow->geometry().height();
    double xp=x1+((double)(p.x()))* ((double)(x2-x1))/ ((double)w);
    double yp=y2+((double)(p.y()))* ((double)(y1-y2))/ ((double)h);
    return point2D(xp,yp);
}
void DetectorDisplay::mouseOnCanvas(const QPoint &p)
{
    point2D pp=pointOnCanvas(p);
    QString ret="";
    if(drawings.size()!=2)return;
    TCutG cut("cut1",((TPolyLine*)drawings[0])->GetN(),
	      ((TPolyLine*)drawings[0])->GetX(),
	      ((TPolyLine*)drawings[0])->GetY());
    if(cut.IsInside(pp.x(),pp.y()))
      ret=ret+"first element\n";
    cut=TCutG("cut1",((TPolyLine*)drawings[1])->GetN(),
	 ((TPolyLine*)drawings[1])->GetX(),
	 ((TPolyLine*)drawings[1])->GetY());
    if(cut.IsInside(pp.x(),pp.y()))
      ret=ret+"whole detector";
#if QT_VERSION < 0x040000
    QToolTip::remove(shapeShow);
    QToolTip::add(shapeShow,ret);
#else
    shapeShow->setToolTip(ret);
#endif
}
void DetectorDisplay::setDefinedShapes(vector<shape_parameter> shp)
{
  while(!defined_shapes.empty()){defined_shapes.pop_back();modShapes.pop_back();}

  for(unsigned int i=0;i<shp.size();i++)
    {
      defined_shapes.push_back(shp[i]);
      modShapes.push_back(shp[i]);
    }
}
#if QT_VERSION > 0x040000
#include <QMenu>
void DetectorDisplay::mouseReleaseEvent ( QMouseEvent * e )
{
  if(ReadOnly)return;
  QWidget *ch=childAt ( e->x(), e->y());
  if(!defined_shapes.empty()&&e->button()==Qt::RightButton)
    {
      if(ch==shap->nameLabel)
	{
	  QMenu pm(this);
	  for(unsigned int i=0;i<defined_shapes.size();i++)
	    pm.addAction(defined_shapes[i].getName().data())->setStatusTip(QString::number(i));//,this,SLOT(onshapeChanged()));
	  connect(&pm,SIGNAL(triggered(QAction*)),this,SLOT(onshapeChanged(QAction*)));
	  pm.exec(mapToGlobal(QPoint(e->x(),e->y())));
	  e->accept();
	  return;
	}
    }
  QWidget::mouseReleaseEvent(e);
}
void DetectorDisplay::onshapeChanged(QAction *trac)
{
  if(trac==NULL)return;
  int num=trac->statusTip().toInt();
  shapeChanged(num);
}
#else
#include <qpopupmenu.h>
void DetectorDisplay::mouseReleaseEvent ( QMouseEvent * e )
{
  if(ReadOnly)return;
  QWidget *ch=childAt ( e->x(), e->y());
  if(!defined_shapes.empty()&&e->button()==Qt::RightButton)
    {
      if(ch==shap->nameLabel)
	{
	  QPopupMenu pm(this);
	  for(unsigned int i=0;i<defined_shapes.size();i++)
	    pm.insertItem(defined_shapes[i].getName().data(),this,SLOT(shapeChanged(int)),i,i);
	  pm.exec(mapToGlobal(QPoint(e->x(),e->y())));
	  e->accept();
	  return;
	}
    }
  QWidget::mouseReleaseEvent(e);
}
#endif

void DetectorDisplay::shapeChanged(int num)
{
  if(num<0||num>=(int)defined_shapes.size())return;
  shap->setShape(modShapes[num]);
  currentShapeType=num;
}
void DetectorDisplay::shapeChanged()
{
  if(ReadOnly)return;
  copyshape=modShapes[currentShapeType];
  copy.setShape(copyshape);
  sp->setShape(copyshape);
}
