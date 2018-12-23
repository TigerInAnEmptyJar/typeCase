#include <qmessagebox.h>
void ShapeWidget::init()
{
  mode=-1;
  ReadOnly=false;
  pdisplay->setReadOnly(ReadOnly);
  fraction=0.5;
#if QT_VERSION < 0x040000
  listView->setSortColumn(-1);
#else
  listView->setSortingEnabled(false);
#endif
}
void ShapeWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void ShapeWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void ShapeWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  listView->setGeometry(1,1,((int)(w*fraction))-5,h-2);
  pdisplay->setGeometry(((int)(w*fraction))+10,1, (int)(w*(1-fraction))-12,h-2);
  line->setGeometry((int)(w*fraction),0,8,h);
}
void ShapeWidget::setShapes(vector<shape_parameter*> &algoIn)
{
  mode=1;
  ap2=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap2->size();i++)
#if QT_VERSION < 0x040000
    new QListViewItem(listView, ap2->at(i)->getName().data());
#else
    new QTreeWidgetItem(listView, QStringList()<<ap2->at(i)->getName().data());
#endif
}
void ShapeWidget::setShapes(vector<shape_parameter> &algoIn)
{
  mode=0;
  ap1=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap1->size();i++)
#if QT_VERSION < 0x040000
    new QListViewItem(listView, ap1->at(i).getName().data());
#else
    new QTreeWidgetItem(listView, QStringList()<<ap1->at(i).getName().data());
#endif
}
bool ShapeWidget::readOnly()
{
    return ReadOnly;
}
void ShapeWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    pdisplay->setReadOnly(ReadOnly);
}
void ShapeWidget::mouseMoveEvent ( QMouseEvent * e )
{
    QWidget *ch=childAt ( e->x(), e->y());
    if(ch==line)
    {
	if(e->pos().y()<height()-30&&e->pos().y()>30)
	{
	    fraction=((float)e->pos().x()-2)/ ((float)width());
	}
	e->accept();
	resize(width(),height());
	return;
    }
}
#if QT_VERSION < 0x040000
void ShapeWidget::onShapeSelect(QListViewItem* it)
#else
void ShapeWidget::onShapeSelect(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  pdisplay->setShape((ap1->at(i)));
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  pdisplay->setShape(*ap2->at(i));
    }
}
#if QT_VERSION < 0x040000
shape_parameter* ShapeWidget::getParameterFromItem(QListViewItem*it)
#else
shape_parameter* ShapeWidget::getParameterFromItem(QTreeWidgetItem*it)
#endif
{
  if(it==NULL)return NULL;
  if(it->parent()==NULL)return NULL;
  shape_parameter *aps=NULL;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  aps=&(ap1->at(i));
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  aps=ap2->at(i);
    }
  return aps;
}
#if QT_VERSION < 0x040000
QListViewItem* ShapeWidget::getItemFromParameter(shape_parameter*al)
{
  if(al==NULL)return NULL;
  QListViewItem *items;
  items=listView->firstChild();
  QListViewItem *it=0,*ac;
  ac=items->firstChild();
  while(ac!=NULL)
    {
      if(ac->text(0)==al->getName().data())it=ac;
      ac=ac->nextSibling();
    }
  return it;
}
#else
QTreeWidgetItem* ShapeWidget::getItemFromParameter(shape_parameter*al)
{
  if(al==NULL)return NULL;
  QTreeWidgetItem *it=0;
  for(int i=0;i<listView->topLevelItemCount();i++)
    if(listView->topLevelItem(i)->text(0)==al->getName().data())
      it=listView->topLevelItem(i);
  return it;
}
#endif
#include "qcanvaswidget.h"
point2D ShapeWidget::pointOnCanvas(const QPoint &p)
{
    double x1,x2,y1,y2;
    pdisplay->shapeShow->GetCanvas()->GetRange(x1,y1,x2,y2);
    int w=pdisplay->shapeShow->geometry().width(),h=pdisplay->shapeShow->geometry().height();
    double xp=x1+((double)(p.x()))* ((double)(x2-x1))/ ((double)w);
    double yp=y2+((double)(p.y()))* ((double)(y1-y2))/ ((double)h);
    return point2D(xp,yp);
}
