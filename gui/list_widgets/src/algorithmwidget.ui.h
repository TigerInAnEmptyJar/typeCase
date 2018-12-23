#include <qmessagebox.h>
void AlgorithmWidget::init()
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
void AlgorithmWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void AlgorithmWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void AlgorithmWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
    listView->setGeometry(1,1,(int)(w*fraction)-5,h-2);
    pdisplay->setGeometry((int)(w*fraction)+5,1, (int)(w*(1-fraction))-5,h-2);
    line->setGeometry((int)(w*fraction),0,8,h);
}
void AlgorithmWidget::setAlgorithms(vector<algorithm_parameter*> &algoIn)
{
  mode=1;
  ap2=&algoIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[5]={
    new QListViewItem(listView,"Calibration"),
    new QListViewItem(listView,"Pixel/Tracking"),
    new QListViewItem(listView,"Post Tracking"),
    new QListViewItem(listView,"Histogramming"),
    new QListViewItem(listView,"Else")};
  for(int i=0;i<5;i++)items[i]->setOpen(true);
#else
  QTreeWidgetItem *items[5]={
    new QTreeWidgetItem(listView,QStringList()<<"Calibration"),
    new QTreeWidgetItem(listView,QStringList()<<"Pixel/Tracking"),
    new QTreeWidgetItem(listView,QStringList()<<"Post Tracking"),
    new QTreeWidgetItem(listView,QStringList()<<"Histogramming"),
    new QTreeWidgetItem(listView,QStringList()<<"Else")};
  for(int i=0;i<5;i++)items[i]->setExpanded(true);
#endif
  int which;
  for(unsigned int i=0;i<ap2->size();i++)
    {
      if(ap2->at(i)==NULL)continue;
      which=ap2->at(i)->getCategory();
      if(which<0||which>4)which=4;
#if QT_VERSION < 0x040000
      (new QCheckListItem(items[which], ap2->at(i)->getName().data(), QCheckListItem::CheckBox))
	->setOn(ap2->at(i)->IsUsed()); 
#else
      (new QTreeWidgetItem(items[which],QStringList()<<ap2->at(i)->getName().data(),0))->setCheckState(0,ap2->at(i)->IsUsed()?Qt::Checked:Qt::Unchecked);
#endif
    }
}
void AlgorithmWidget::setAlgorithms(vector<algorithm_parameter> &algoIn)
{
  mode=0;
  ap1=&algoIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[5]={
    new QListViewItem(listView,"Calibration"),
    new QListViewItem(listView,"Pixel/Tracking"),
    new QListViewItem(listView,"Post Tracking"),
    new QListViewItem(listView,"Histogramming"),
    new QListViewItem(listView,"Else")};
  for(int i=0;i<5;i++)items[i]->setOpen(true);
#else
  QTreeWidgetItem *items[5]={
    new QTreeWidgetItem(listView,QStringList()<<"Calibration"),
    new QTreeWidgetItem(listView,QStringList()<<"Pixel/Tracking"),
    new QTreeWidgetItem(listView,QStringList()<<"Post Tracking"),
    new QTreeWidgetItem(listView,QStringList()<<"Histogramming"),
    new QTreeWidgetItem(listView,QStringList()<<"Else")};
  for(int i=0;i<5;i++)items[i]->setExpanded(true);
#endif
  int which;
  for(unsigned int i=0;i<ap1->size();i++)
    {
      which=ap1->at(i).getCategory();
      if(which<0||which>4)which=4;
#if QT_VERSION < 0x040000
      (new QCheckListItem(items[which], ap1->at(i).getName().data(), QCheckListItem::CheckBox))
	->setOn(ap1->at(i).IsUsed()); 
#else
      (new QTreeWidgetItem(items[which],QStringList()<<ap1->at(i).getName().data(),0))->setCheckState(0,ap1->at(i).IsUsed()?Qt::Checked:Qt::Unchecked);
#endif
    }
}
bool AlgorithmWidget::readOnly()
{
    return ReadOnly;
}
void AlgorithmWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    pdisplay->setReadOnly(ReadOnly);
}
void AlgorithmWidget::mouseMoveEvent ( QMouseEvent * e )
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
void AlgorithmWidget::onAlgorithmSelect(QListViewItem* it)
#else
void AlgorithmWidget::onAlgorithmSelect(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  pdisplay->setAlgorithm(&(ap1->at(i)));
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  pdisplay->setAlgorithm(ap2->at(i));
    }
}
#if QT_VERSION < 0x040000
algorithm_parameter* AlgorithmWidget::getParameterFromItem(QListViewItem*it)
#else
algorithm_parameter* AlgorithmWidget::getParameterFromItem(QTreeWidgetItem*it)
#endif
{
  if(it==NULL)return NULL;
  if(it->parent()==NULL)return NULL;
  algorithm_parameter *aps=NULL;
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
QListViewItem* AlgorithmWidget::getItemFromParameter(algorithm_parameter*al)
#else
QTreeWidgetItem* AlgorithmWidget::getItemFromParameter(algorithm_parameter*al)
#endif
{
  if(al==NULL)return NULL;
#if QT_VERSION < 0x040000
  QListViewItem *items[5];
  QListViewItem *it=0,*ac;
  items[0]=listView->firstChild();
  for(int i=0;i<4;i++)items[i+1]=items[i]->nextSibling();
  for(int i=0;i<5;i++)
    {
      ac=items[i]->firstChild();
      while(ac!=NULL)
	{
	  if(ac->text(0)==al->getName().data())it=ac;
	  ac=ac->nextSibling();
	}
    }
#else
  QTreeWidgetItem *it=0,*ac;
  for(int i=0;i<listView->topLevelItemCount();i++)
    {
      ac=listView->topLevelItem(i);
      for(int j=0;j<ac->childCount();j++)
	if(ac->child(j)->text(0)==al->getName().data())
	  it=ac->child(j);
    }
#endif
  return it;
}
#if QT_VERSION < 0x040000
void AlgorithmWidget::algorithmClicked(int b,QListViewItem* it,const QPoint& p, int c)
#else
void AlgorithmWidget::algorithmClicked(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  algorithmChecked(it);
  return;
  algorithm_parameter *aps=getParameterFromItem(it);
  if(ReadOnly){
    if(aps!=NULL)
#if QT_VERSION < 0x040000
      ((QCheckListItem*)it)->setOn(aps->IsUsed());return;}
#else
      it->setCheckState(0,aps->IsUsed()?Qt::Checked:Qt::Unchecked);return;}
#endif
  if(aps==NULL){
    QMessageBox::warning(this,"Algorithm not found", QString("Algorithm \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }
#if QT_VERSION < 0x040000
  if(aps->IsUsed()==((QCheckListItem*)it)->isOn())return;
  aps->setUse(((QCheckListItem*)it)->isOn());
  emit algorithmSelected(*aps,((QCheckListItem*)it)->isOn());
#else
  if(aps->IsUsed()==(it->checkState(0)==Qt::Checked))return;
  aps->setUse(it->checkState(0)==Qt::Checked);
  emit algorithmSelected(*aps,it->checkState(0)==Qt::Checked);
#endif
}
#if QT_VERSION < 0x040000
void AlgorithmWidget::algorithmChecked(QListViewItem* it)
#else
void AlgorithmWidget::algorithmChecked(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  algorithm_parameter *aps=getParameterFromItem(it);
  if(ReadOnly){
    if(aps!=NULL)
#if QT_VERSION < 0x040000
      ((QCheckListItem*)it)->setOn(aps->IsUsed());
#else
      it->setCheckState(0,aps->IsUsed()?Qt::Checked:Qt::Unchecked);
#endif
      return;}
  if(aps==NULL){
    QMessageBox::warning(this,"Algorithm not found", QString("Algorithm \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }
#if QT_VERSION < 0x040000
  if(aps->IsUsed()==((QCheckListItem*)it)->isOn())return;
  aps->setUse(((QCheckListItem*)it)->isOn());
  emit algorithmSelected(*aps,((QCheckListItem*)it)->isOn());
#else
  if(aps->IsUsed()==(it->checkState(0)==Qt::Checked))return;
  aps->setUse(it->checkState(0)==Qt::Checked);
  emit algorithmSelected(*aps,it->checkState(0)==Qt::Checked);
#endif
}
void AlgorithmWidget::algorithmSelect(algorithm_parameter &algo, bool value)
{
#if QT_VERSION < 0x040000
  QListViewItem *it=getItemFromParameter(&algo);
#else
  QTreeWidgetItem *it=getItemFromParameter(&algo);
#endif
  if(it==NULL){
    QMessageBox::warning(this,"Algorithm not found", QString("Algorithm \"")+algo.getName().data()+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }
#if QT_VERSION < 0x040000
  ((QCheckListItem*)it)->setOn(algo.IsUsed());
#else
  it->setCheckState(0,algo.IsUsed()?Qt::Checked:Qt::Unchecked);
#endif
}
