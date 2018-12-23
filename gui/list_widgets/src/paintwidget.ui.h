#include <qmessagebox.h>
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
#include "utilities.h"
void PaintWidget::init()
{
  mode=-1;
  ReadOnly=false;
  pdisplay->setReadOnly(ReadOnly);
  fraction=0.5;
#if QT_VERSION < 0x040000
  listView->setSortColumn(-1);
#else
  listView->setSortingEnabled(false);
  current=NULL;
#endif
}
void PaintWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void PaintWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void PaintWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  listView->setGeometry(1,1,(int)(w*fraction)-5,h-2);
  pdisplay->setGeometry((int)(w*fraction)+5,1, (int)(w*(1-fraction))-5,h-2);
  line->setGeometry((int)(w*fraction),0,8,h);
}
void PaintWidget::setPaints(vector<paint_parameter> &algoInT, vector<paint_parameter> &algoInP, vector<paint_parameter> &algoInH)
{
  mode=1;
  while(!ap2.empty())ap2.pop_back();
  ap2.push_back(&algoInH);
  ap2.push_back(&algoInP);
  ap2.push_back(&algoInT);
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[3]={new QListViewItem(listView,"Hits"),
			   new QListViewItem(listView,"Pixel"),
			   new QListViewItem(listView,"Tracks")};
#else
  QTreeWidgetItem *items[3]={new QTreeWidgetItem(listView,QStringList()<<"Hits"),
			   new QTreeWidgetItem(listView,QStringList()<<"Pixel"),
			   new QTreeWidgetItem(listView,QStringList()<<"Tracks")};
#endif
  for(unsigned int i=0;i<ap2.size();i++)
    for(unsigned int j=0;j<ap2[i]->size();j++)
#if QT_VERSION < 0x040000
      new QListViewItem(items[i], ap2[i]->at(j).getName().data());
#else
      new QTreeWidgetItem(items[i], QStringList()<<ap2[i]->at(j).getName().data());
#endif
}
void PaintWidget::setPaints(vector<vector<paint_parameter> > &algoIn )
{
  mode=0;
  ap1=&algoIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[3]={new QListViewItem(listView,"Hits"),
			   new QListViewItem(listView,"Pixel"),
			   new QListViewItem(listView,"Tracks")};
#else
  QTreeWidgetItem *items[3]={new QTreeWidgetItem(listView,QStringList()<<"Hits"),
			   new QTreeWidgetItem(listView,QStringList()<<"Pixel"),
			   new QTreeWidgetItem(listView,QStringList()<<"Tracks")};
#endif
  for(unsigned int i=0;i<ap1->size();i++)
    for(unsigned int j=0;j<ap1->at(i).size();j++)
#if QT_VERSION < 0x040000
      new QListViewItem(items[i], ap1->at(i).at(j).getName().data());
#else
      new QTreeWidgetItem(items[i], QStringList()<<ap1->at(i).at(j).getName().data());
#endif
}
bool PaintWidget::readOnly()
{
    return ReadOnly;
}
void PaintWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    pdisplay->setReadOnly(ReadOnly);
}
void PaintWidget::mouseMoveEvent ( QMouseEvent * e )
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
void PaintWidget::onPaintSelect(QListViewItem* it)
#else
void PaintWidget::onPaintSelect(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
#if QT_VERSION > 0x040000
  current=it;
#endif
  paint_parameter *aps=getParameterFromItem(it,selected,selectedT);
  if(aps==NULL)return;
  pdisplay->setPaint(*aps);
}
#if QT_VERSION < 0x040000
paint_parameter* PaintWidget::getParameterFromItem(QListViewItem*it, int &n, int &m)
#else
paint_parameter* PaintWidget::getParameterFromItem(QTreeWidgetItem*it, int &n, int &m)
#endif
{
  if(it==NULL)return NULL;
  paint_parameter *aps=NULL;
  if(mode==0)
    {
      for(unsigned int j=0;j<ap1->size();j++)
	for(unsigned int i=0;i<ap1->at(j).size();i++)
	  if(it->text(0)==ap1->at(j).at(i).getName().data())
	    {n=i;
	    m=j;
	    aps=&(ap1->at(j).at(i));}
    }
  else if(mode==1)
    {
      for(unsigned int j=0;j<ap2.size();j++)
	for(unsigned int i=0;i<ap2[j]->size();i++)
	  if(it->text(0)==ap2[j]->at(i).getName().data())
	    {n=i;
	    m=j;
	    aps=&(ap2[j]->at(i));}
    }
  return aps;
}
#if QT_VERSION < 0x040000
QListViewItem* PaintWidget::getItemFromParameter(paint_parameter*al)
#else
QTreeWidgetItem* PaintWidget::getItemFromParameter(paint_parameter*al)
#endif
{
  if(al==NULL)return NULL;
#if QT_VERSION < 0x040000
  QListViewItem *ac,*ac1;
  ac1=listView->firstChild();
  while(ac1!=NULL)
    {
      ac=ac1->firstChild();
      while(ac!=NULL)
	{
	  if(ac->text(0)==al->getName().data())
	    return ac;
	  ac=ac->nextSibling();
	}
      ac1=ac1->nextSibling();
    }
#else
  QTreeWidgetItem *ac;
  for(int i=0;i<listView->topLevelItemCount();i++)
    {
      ac=listView->topLevelItem(i);
      for(int j=0;j<ac->childCount();j++)
	if(ac->child(j)->text(0)==al->getName().data())
	  return ac->child(j);
    }
#endif
  return NULL;
}
void PaintWidget::paintChanged()
{
#if QT_VERSION < 0x040000
  QListViewItem *it=listView->selectedItem();
#else
  QTreeWidgetItem *it=current;//listView->selectedItems().first();
#endif
  if(it==NULL)return;
  if(it->parent()==NULL)return;
  if(selected<0)return;
  if(selectedT<0||selectedT>=3)return;
  string s=(mode==0?ap1->at(selectedT).at(selected).getName():ap2[selectedT]->at(selected).getName());
  it->setText(0,s.data());
  emit changed();
}
#if QT_VERSION < 0x040000
void PaintWidget::paintRightClicked(QListViewItem* it,const QPoint& p) 
{ 
#else
void PaintWidget::paintRightClicked(const QPoint& p) 
{ 
  QTreeWidgetItem* it=listView->itemAt(p);
  current=it;
#endif
  if(ReadOnly||it==NULL)return;
  int n=-1,m=-1;
  int tmp=-1;
  if(it!=NULL)
    {
      getParameterFromItem(it,n,tmp);
      string s=QS2s((it->parent()!=NULL?it->parent():it)->text(0));
      if(s=="Hits")m=0;
      else if(s=="Pixel")m=1;
      else if(s=="Tracks")m=2;
    }
#if QT_VERSION < 0x040000
  QPopupMenu pm(this);
  pm.insertItem("Add Paint-Parameter",this,SLOT(addPaint(int)),m,m);
  if(it!=0)
    if(it->parent()!=NULL)
      pm.insertItem("Delete Paint-Parameter",this,SLOT(deletePaint(int)),n+1000*tmp,n+1000*tmp);
  pm.exec(p);
#else
  QMenu pm(this);
  pm.addAction("Add Paint-Parameter")/*,this,SLOT(addPaint(QAction*)))*/->setStatusTip(QString::number(m));
  if(it!=0)
    if(it->parent()!=NULL)
      pm.addAction("Delete Paint-Parameter")/*,this,SLOT(deletePaint(QAction*)))*/->setStatusTip(QString::number(n+1000*tmp));
  connect(&pm,SIGNAL(triggered(QAction*)),this,SLOT(onPaint(QAction*)));
  pm.exec(listView->viewport()->mapToGlobal(p));
  disconnect(&pm,SIGNAL(triggered(QAction*)),this,SLOT(onPaint(QAction*)));
#endif
 } 
#if QT_VERSION > 0x040000
void PaintWidget::onPaint(QAction* ac)
{
  if(ac==NULL)return;
  int n=ac->statusTip().toInt();
  if(n<0)return;
  if(ac->text()=="Add Paint-Parameter")
    addPaint(n);
  else if(ac->text()=="Delete Paint-Parameter")
    deletePaint(n);
}
#endif
void PaintWidget::addPaint(int num)
{
  if(ReadOnly)return;
  QString s=QStringInputDialog::getLine(this,"New Paint","new paint");
  if(s=="")return;
#if QT_VERSION < 0x040000
  QListViewItem *it=listView->firstChild();
  while(it!=NULL)
    {
      if(num==0&&it->text(0)=="Hits")break;
      if(num==1&&it->text(0)=="Pixel")break;
      if(num==2&&it->text(0)=="Tracks")break;
      it=it->nextSibling();
    }
  new QListViewItem(it,s);
  if(it==NULL)return;
#else
  QTreeWidgetItem *it=listView->topLevelItem(num);
  if(it==NULL)return;
  new QTreeWidgetItem(it,QStringList()<<s);
#endif
  if(mode==0)
    {
      ap1->at(num).push_back(paint_parameter());
      ap1->at(num).back().setName(QS2s(s));
      pdisplay->setPaint(ap1->at(num).back());
    }
  else
    {
      ap2[num]->push_back(paint_parameter());
      ap2[num]->back().setName(QS2s(s));
      pdisplay->setPaint(ap2[num]->back());
    }
}
void PaintWidget::deletePaint(int num)
{
  if(ReadOnly)return;
  if(num<0)return;
  if(selectedT<0)return;
  string s=(mode==0?ap1->at(selectedT).at(num).getName():
	    ap2[selectedT]->at(num).getName());
#if QT_VERSION < 0x040000
  QListViewItem 
#else
  QTreeWidgetItem 
#endif
    *it=getItemFromParameter((mode==0?
			      &ap1->at(selectedT).at(num):
			      &(ap2[selectedT]->at(num))));
  if(it==NULL)
    {
      QMessageBox::warning(this,"Paint not Found in ListView",QString("Paint \"")+s.data()+"\" not found in List", QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  if(mode==0)
    {
      if(selectedT>=(int)ap1->size())return;
      if(num>=(int)ap1->at(selectedT).size())return;
      vector<paint_parameter> tmp;
      while((int)ap1->at(selectedT).size()>num+1)
	{
	  tmp.push_back(ap1->at(selectedT).back());
	  ap1->at(selectedT).pop_back();
	}
      ap1->at(selectedT).pop_back();
      while(!tmp.empty())
	{
	  ap1->at(selectedT).push_back(tmp.back());
	  tmp.pop_back();
	}
    }
  else
    {
      if(num>=(int)ap2[selectedT]->size())return;
      vector<paint_parameter> tmp;
      while((int)ap2[selectedT]->size()>num+1)
	{
	  tmp.push_back(ap2[selectedT]->back());
	  ap2[selectedT]->pop_back();
	}
      ap2[selectedT]->pop_back();
      while(!tmp.empty())
	{
	  ap2[selectedT]->push_back(tmp.back());
	  tmp.pop_back();
	}
    }
#if QT_VERSION < 0x040000
  listView->takeItem(it);
#else
  it->parent()->takeChild(it->parent()->indexOfChild(it));
#endif
  delete it;
}

