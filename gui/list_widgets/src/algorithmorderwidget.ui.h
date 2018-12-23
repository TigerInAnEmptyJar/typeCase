#include <qmessagebox.h>
void AlgorithmOrderWidget::init()
{
  ap1=NULL;
  ap2=NULL;
  mode=-1;
  pdisplay->setReadOnly(false);
  fraction=0.5;
  //  listView->setFocusPolicy(QListBox::NoFocus);
}
void AlgorithmOrderWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void AlgorithmOrderWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void AlgorithmOrderWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  listView->setGeometry(1,1,(int)(w*fraction)-5,h-2);
  pdisplay->setGeometry((int)(w*fraction)+5,1, (int)(w*(1-fraction))-5,h-2);
  line->setGeometry((int)(w*fraction),0,8,h);
}
void AlgorithmOrderWidget::setAlgorithms(vector<algorithm_parameter*> &algoIn)
{
  mode=1;
  ap2=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap2->size();i++)
    {
      if(ap2->at(i)==NULL)continue;
#if QT_VERSION < 0x040000
      listView->insertItem(ap2->at(i)->getName().data());
#else
      listView->addItem(ap2->at(i)->getName().data());
#endif
    }
}
void AlgorithmOrderWidget::setAlgorithms(vector<algorithm_parameter> &algoIn)
{
  mode=0;
  ap1=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap1->size();i++)
#if QT_VERSION < 0x040000
    listView->insertItem(ap1->at(i).getName().data());
#else
    listView->addItem(ap1->at(i).getName().data());
#endif
}
void AlgorithmOrderWidget::mouseMoveEvent ( QMouseEvent * e )
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
void AlgorithmOrderWidget::keyPressEvent ( QKeyEvent * e )
{
  //  cout<<"keypressevent"<<endl;
  //  QWidget *ch=focusWidget();//childAt ( e->x(), e->y());
  if(e->text()=="")return;
  //  cout<<"\""<<e->text().ascii()<<"\" "<<ch<<endl;
  //  if(ch==listView&&e->text()!="")
    {
      if(e->text()=="u"||e->text()=="n"||e->text()=="h"||e->text()=="k"||e->text()=="d")
	{
	  e->accept();
	  int num=
#if QT_VERSION < 0x040000
	  listView->currentItem();
#else
	  listView->currentRow(); 
#endif
	  if(e->text()=="u")
	    onAlgorithmUp(num);
	  if(e->text()=="n")
	    onAlgorithmDown(num);
	  if(e->text()=="h")
	    onAlgorithmPos1(num);
	  if(e->text()=="k")
	    onAlgorithmEnd(num);
	  if(e->text()=="d")
	    onAlgorithmDelete(num);
	  return;
	}
    }
  QWidget::keyPressEvent(e);
}
#if QT_VERSION < 0x040000
void AlgorithmOrderWidget::onAlgorithmSelect(QListBoxItem* it)
#else
void AlgorithmOrderWidget::onAlgorithmSelect(QListWidgetItem* it)
#endif
{
  if(it==NULL)return;
  algorithm_parameter *aps=getParameterFromItem(it);
  if(aps!=NULL)
    pdisplay->setAlgorithm(aps);
}
#if QT_VERSION < 0x040000
algorithm_parameter* AlgorithmOrderWidget::getParameterFromItem(QListBoxItem*it)
#else
algorithm_parameter* AlgorithmOrderWidget::getParameterFromItem(QListWidgetItem*it)
#endif
{
  if(it==NULL||mode<0)return NULL;
  algorithm_parameter *aps=NULL;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text()==ap1->at(i).getName().data())
	  aps=&(ap1->at(i));
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text()==ap2->at(i)->getName().data())
	  aps=ap2->at(i);
    }
  if(aps==NULL)
    {
      for(unsigned int i=0;i<added.size();i++)
	if(it->text()==added[i]->getName().data())
	  aps=added[i];
    }
  return aps;
}
#if QT_VERSION < 0x040000
QListBoxItem* AlgorithmOrderWidget::getItemFromParameter(algorithm_parameter*al)
#else
QListWidgetItem* AlgorithmOrderWidget::getItemFromParameter(algorithm_parameter*al)
#endif
{
  if(al==NULL)return NULL;
#if QT_VERSION < 0x040000
  QListBoxItem *it=0;
#else
  QListWidgetItem *it=0;
#endif
  for(int i=0;i<(int)listView->count();i++)
    {
#if QT_VERSION < 0x040000
     if(listView->text(i)==al->getName().data())
#else
     if(listView->item(i)->text()==al->getName().data())
#endif
	return listView->item(i);
    }
  return it;
}
/* void AlgorithmOrderWidget::algorithmClicked(int b,QListViewItem* it,const QPoint& p, int c) */
/* { */
/*   if(it==NULL)return; */
/*   algorithmChecked(it); */
/*   return; */
/*   algorithm_parameter *aps=getParameterFromItem(it); */
/*   if(ReadOnly){ */
/*     if(aps!=NULL) */
/*       ((QCheckListItem*)it)->setOn(aps->IsUsed());return;} */
/*   if(aps==NULL){ */
/*     QMessageBox::warning(this,"Algorithm not found", QString("Algorithm \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton); */
/*     return; */
/*   } */
/*   if(aps->IsUsed()==((QCheckListItem*)it)->isOn())return; */
/*   aps->setUse(((QCheckListItem*)it)->isOn()); */
/*   emit algorithmSelected(*aps,((QCheckListItem*)it)->isOn()); */
/* } */
/* void AlgorithmOrderWidget::algorithmChecked(QListViewItem* it) */
/* { */
/*   if(it==NULL)return; */
/*   algorithm_parameter *aps=getParameterFromItem(it); */
/*   if(ReadOnly){ */
/*     if(aps!=NULL) */
/*       ((QCheckListItem*)it)->setOn(aps->IsUsed());return;} */
/*   if(aps==NULL){ */
/*     QMessageBox::warning(this,"Algorithm not found", QString("Algorithm \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton); */
/*     return; */
/*   } */
/*   if(aps->IsUsed()==((QCheckListItem*)it)->isOn())return; */
/*   aps->setUse(((QCheckListItem*)it)->isOn()); */
/*   emit algorithmSelected(*aps,((QCheckListItem*)it)->isOn()); */
/* } */
void AlgorithmOrderWidget::algorithmSelect(algorithm_parameter &algo, bool value)
{
  if(mode<0)return;
  int n=-1;
  if(mode==0&&ap1!=NULL)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if((ap1->at(i).getName())==algo.getName())n=i;
      if(value)
	{
	  if(n<0){
	    ap1->push_back(algo);
#if QT_VERSION < 0x040000
	    listView->insertItem(algo.getName().data());
#else
	    listView->addItem(algo.getName().data());
#endif
	  }}
      else
	{
	  if(n>=0)
	    {
#if QT_VERSION < 0x040000
	      listView->removeItem(n);
#else
	      delete listView->takeItem(n);
#endif
	      vector<algorithm_parameter> tmp;
	      while((int)ap1->size()>n+1)
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
	    }
	}
    }
  else if(mode==1&&ap2!=NULL)
    {
      for(unsigned int i=0;i<ap2->size();i++)
	if(ap2->at(i)->getName()==algo.getName())n=i;
      if(value)
	{
	  if(n<0){
	    ap2->push_back(&algo);
#if QT_VERSION < 0x040000
  	    listView->insertItem(algo.getName().data());
#else
  	    listView->addItem(algo.getName().data());
#endif
	  }}
      else
	{
	  if(n>=0)
	    {
#if QT_VERSION < 0x040000
	      listView->removeItem(n);
#else
	      delete listView->takeItem(n);
#endif
	      vector<algorithm_parameter*> tmp;
	      while((int)ap2->size()>n+1)
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
	    }
	}
    }
  else
    QMessageBox::warning(this,"AlgorithmOrderWidget", "AlgorithmOrderWidget not initialized!", QMessageBox::Ok, QMessageBox::NoButton);
  //  ((QCheckListItem*)it)->setOn(algo.IsUsed());
}
void AlgorithmOrderWidget::onAlgorithmDown(int n)
{
  if(n<0||n>=(int)listView->count())return;
  if(listView->count()==1)return;
  if((int)listView->count()==n+1)return;
#if QT_VERSION < 0x040000
  QListBoxItem *it=listView->item(n);
  listView->takeItem(it);
  listView->insertItem(it,listView->item(n));
  listView->setCurrentItem(it);
#else
  QListWidgetItem *it=listView->item(n);
  listView->takeItem(listView->row(it));
  listView->insertItem(n+1,it);
  listView->setCurrentItem(it);
#endif
}
void AlgorithmOrderWidget::onAlgorithmUp(int n)
{
  if(n<0||n>=(int)listView->count())return;
  if(n==0)return;
  if(listView->count()==1)return;
#if QT_VERSION < 0x040000
  QListBoxItem *it=listView->item(n);
  listView->takeItem(it);
  listView->insertItem(it,n-1);
#else
  QListWidgetItem *it=listView->takeItem(n);
  listView->insertItem(n-1,it);
#endif
  listView->setCurrentItem(it);
}
void AlgorithmOrderWidget::onAlgorithmPos1(int n)
{
  if(n<0||n>=(int)listView->count())return;
  if(n==0)return;
  if(listView->count()==1)return;
#if QT_VERSION < 0x040000
  QListBoxItem *it=listView->item(n);
  listView->takeItem(it);
  listView->insertItem(it,0);
#else
  QListWidgetItem *it=listView->takeItem(n);
  listView->insertItem(0,it);
#endif
  listView->setCurrentItem(it);
}
void AlgorithmOrderWidget::onAlgorithmEnd(int n)
{
  if(n<0||n>=(int)listView->count())return;
  if(n==(int)listView->count()-1)return;
  if(listView->count()==1)return;
#if QT_VERSION < 0x040000
  QListBoxItem *it=listView->item(n);
  listView->takeItem(it);
  listView->insertItem(it,listView->item(listView->count()-1));
#else
  QListWidgetItem *it=listView->takeItem(n);
  listView->insertItem(listView->count()-1,it);
#endif
  listView->setCurrentItem(it);
}
void AlgorithmOrderWidget::onAlgorithmDelete(int n)
{
  if(n<0||n>=(int)listView->count())return;
#if QT_VERSION < 0x040000
  QListBoxItem *it=listView->item(n);
  listView->takeItem(it);
  delete it;
  if((int)listView->count()>n)
    listView->setCurrentItem(n);
  else
    listView->setCurrentItem(listView->count()-1);
  listView->setSelected(listView->currentItem(),true);
#else
  delete listView->takeItem(n);
  if((int)listView->count()>n)
    listView->setCurrentRow(n);
  else
    listView->setCurrentRow(listView->count()-1);
  listView->currentItem()->setSelected(true);
#endif
}
void AlgorithmOrderWidget::algorithmAdd(algorithm_parameter *algo)
{
  bool has=false;
#if QT_VERSION < 0x040000
  QListBoxItem *it=getItemFromParameter(algo);
#else
  QListWidgetItem *it=getItemFromParameter(algo);
#endif
  for(unsigned int i=0;i<added.size();i++)
    if(added[i]->getName()==algo->getName())has=true;
  if(has||it!=NULL)
    {
      string aname=algo->getName();
      int n=0;
      while(has||it!=NULL)
	{
	  algo->setName(aname+"_"+string_number(n++));
	  has=false;
	  it=getItemFromParameter(algo);
	  for(unsigned int i=0;i<added.size();i++)
	    if(added[i]->getName()==algo->getName())has=true;
	}
    }
#if QT_VERSION < 0x040000
  listView->insertItem(algo->getName().data());
#else
  listView->addItem(algo->getName().data());
#endif
  added.push_back(algo);
  untaken.push_back(true);
}
vector<algorithm_parameter*> AlgorithmOrderWidget::getOrder()
{
  vector<algorithm_parameter*> ret;
  for(int i=0;i<(int)listView->count();i++)
    {
      algorithm_parameter *ap=getParameterFromItem(listView->item(i));
      if(ap==NULL)continue;
      ret.push_back(ap);
      for(unsigned int i=0;i<added.size();i++)
	if(ap->getName()==added[i]->getName())
	  untaken[i]=false;
    }
  return ret;
}
