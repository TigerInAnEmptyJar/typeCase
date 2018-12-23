#include <qmessagebox.h>
#include "utilities.h"
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
void MaterialWidget::init()
{
  mode=-1;
  ReadOnly=false;
  pdisplay->setReadOnly(ReadOnly);
  fraction=0.2;
#if QT_VERSION < 0x040000
  listView->setSortColumn(-1);
#else
  listView->setSortingEnabled(false);
#endif
}
void MaterialWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void MaterialWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void MaterialWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  listView->setGeometry(1,1,(int)(w*fraction)-5,h-2);
  pdisplay->setGeometry((int)(w*fraction)+5,1, (int)(w*(1-fraction))-15,h-2);
  line->setGeometry((int)(w*fraction)-4,0,8,h);
}
void MaterialWidget::setMaterials(vector<material_parameter*> &algoIn)
{
  mode=1;
  ap2=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap2->size();i++)
    {
      if(ap2->at(i)==NULL)continue;
#if QT_VERSION < 0x040000
      new QListViewItem(listView, ap2->at(i)->getName().data());
#else
      new QTreeWidgetItem(listView, QStringList()<<ap2->at(i)->getName().data());
#endif
    }
}
void MaterialWidget::setMaterials(vector<material_parameter> &algoIn)
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
bool MaterialWidget::readOnly()
{
    return ReadOnly;
}
void MaterialWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    pdisplay->setReadOnly(ReadOnly);
}
void MaterialWidget::mouseMoveEvent ( QMouseEvent * e )
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
void MaterialWidget::onMaterialSelect(QListViewItem* it)
#else
void MaterialWidget::onMaterialSelect(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  material_parameter *aps=NULL;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  {selected=i;
	  aps=&(ap1->at(i));}
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  {selected=i;
	  aps=ap2->at(i);}
    }
  if(aps==NULL)return;
  pdisplay->setMaterial(*aps);
}
#if QT_VERSION < 0x040000
material_parameter* MaterialWidget::getParameterFromItem(QListViewItem*it)
#else
material_parameter* MaterialWidget::getParameterFromItem(QTreeWidgetItem*it)
#endif
{
  if(it==NULL)return NULL;
  material_parameter *aps=NULL;
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
QListViewItem* MaterialWidget::getItemFromParameter(material_parameter*al)
{
  if(al==NULL)return NULL;
  QListViewItem *it=0,*ac;
  ac=listView->firstChild();
  while(ac!=NULL)
    {
      if(ac->text(0)==al->getName().data())it=ac;
      ac=ac->nextSibling();
    }
  return it;
}
#else
QTreeWidgetItem* MaterialWidget::getItemFromParameter(material_parameter*al)
{
  if(al==NULL)return NULL;
  for(int i=0;i<listView->topLevelItemCount();i++)
    if(listView->topLevelItem(i)->text(0)==al->getName().data())
      return listView->topLevelItem(i);
  return NULL;
}
#endif
void MaterialWidget::materialChanged()
{
#if QT_VERSION < 0x040000
  QListViewItem *it=listView->selectedItem();
  if(it==NULL)return;
  if(selected<0)return;
#else
  if(listView->selectedItems().count()==0)return;
  QTreeWidgetItem *it=listView->selectedItems().first();
  if(selected<0)return;
#endif
  if(mode==0)
    it->setText(0,ap1->at(selected).getName().data());
  else
    it->setText(0,ap2->at(selected)->getName().data());
  emit changed();
}
#if QT_VERSION < 0x040000
void MaterialWidget::materialRightClicked(QListViewItem* it,const QPoint& p) 
{ 
  if(ReadOnly)return;
  QPopupMenu pm(this);
  pm.insertItem("Add Material",this,SLOT(addMaterial()));
  if(it!=0)
    pm.insertItem("Delete Material",this,SLOT(deleteMaterial()));
  pm.exec(p);
#else
void MaterialWidget::materialRightClicked(const QPoint& p) 
{ 
  QTreeWidgetItem *it=listView->itemAt(p);
  if(ReadOnly)return;
  QMenu pm(this);
  pm.addAction("Add Material",this,SLOT(addMaterial()));
  if(it!=0)
    pm.addAction("Delete Material",this,SLOT(deleteMaterial()));
  pm.exec(listView->viewport()->mapToGlobal(p));
#endif
 } 
void MaterialWidget::addMaterial()
{
  if(ReadOnly)return;
  QStringInputDialog fenst(this);
#if QT_VERSION < 0x040000
  fenst.setCaption("New Material");
#else
  fenst.setWindowTitle("New Material");
#endif
  fenst.setStart("new material");
  fenst.exec();
  if(fenst.result()!=QDialog::Accepted)return;
#if QT_VERSION < 0x040000
  new QListViewItem(listView,fenst.line());
#else
  new QTreeWidgetItem(listView,QStringList()<<fenst.line());
#endif
  if(mode==0)
    {
      ap1->push_back(material_parameter());
      ap1->back().setName(QS2s(fenst.line()));
      pdisplay->setMaterial(ap1->back());
    }
  else
    {
      ap2->push_back(new material_parameter());
      ap2->back()->setName(QS2s(fenst.line()));
      pdisplay->setMaterial(*ap2->back());
    }
}
void MaterialWidget::deleteMaterial()
{
  if(ReadOnly)return;
  if(selected<0)return;
  if(mode==0)
    {
      if(selected>=(int)ap1->size())return;
#if QT_VERSION < 0x040000
      QListViewItem *it
#else
      QTreeWidgetItem *it
#endif
	=getItemFromParameter(&ap1->at(selected));
      if(it==NULL)
	{
	  QMessageBox::warning(this,"Material not Found in ListView",QString("Material \"")+ap1->at(selected).getName().data()+"\" not found in List", QMessageBox::Ok,QMessageBox::NoButton);
	  return;
	}
      vector<material_parameter> tmp;
      while((int)ap1->size()>selected+1)
	{
	  tmp.push_back(ap1->back());
	  ap1->pop_back();
	}
      ap1->pop_back();
      while(!tmp.empty())
	{
	  ap1->push_back(tmp.back());
	  tmp.pop_back();
	}
#if QT_VERSION < 0x040000
      listView->takeItem(it);
#else
      listView->takeTopLevelItem(listView->indexOfTopLevelItem(it));
#endif
      delete it;
    }
  else
    {
#if QT_VERSION < 0x040000
      QListViewItem *it
#else
      QTreeWidgetItem *it
#endif
	=getItemFromParameter(&ap1->at(selected));
      if(selected>=(int)ap2->size())return;
      if(it==NULL)
	{
	  QMessageBox::warning(this,"Material not Found in ListView",QString("Material \"")+ap2->at(selected)->getName().data()+"\" not found in List", QMessageBox::Ok,QMessageBox::NoButton);
	  return;
	}
      vector<material_parameter*> tmp;
      while((int)ap2->size()>selected+1)
	{
	  tmp.push_back(ap2->back());
	  ap2->pop_back();
	}
      ap2->pop_back();
      while(!tmp.empty())
	{
	  ap2->push_back(tmp.back());
	  tmp.pop_back();
	}
#if QT_VERSION < 0x040000
      listView->takeItem(it);
#else
      listView->takeTopLevelItem(listView->indexOfTopLevelItem(it));
#endif
      delete it;
    }
}
/* void MaterialWidget::materialChecked(QListViewItem* it) */
/* { */
/*   if(it==NULL)return; */
/*   material_parameter *aps=getParameterFromItem(it); */
/*   if(ReadOnly){ */
/*     if(aps!=NULL) */
/*       ((QCheckListItem*)it)->setOn(aps->IsUsed());return;} */
/*   if(aps==NULL){ */
/*     QMessageBox::warning(this,"Material not found", QString("Material \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton); */
/*     return; */
/*   } */
/*   if(aps->IsUsed()==((QCheckListItem*)it)->isOn())return; */
/*   aps->setUse(((QCheckListItem*)it)->isOn()); */
/*   emit materialSelected(*aps,((QCheckListItem*)it)->isOn()); */
/* } */
/* void MaterialWidget::materialSelect(material_parameter &algo, bool value) */
/* { */
/*   QListViewItem *it=getItemFromParameter(&algo); */
/*   if(it==NULL){ */
/*     QMessageBox::warning(this,"Material not found", QString("Material \"")+algo.getName().data()+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton); */
/*     return; */
/*   } */
/*   ((QCheckListItem*)it)->setOn(algo.IsUsed()); */
/* } */
