#include <qmessagebox.h>
#include "utilities.h"
void DataBasisWidget::init()
{
  mode=-1;
  ReadOnly=false;
#if QT_VERSION < 0x040000
  listView->setSortColumn(-1);
#else
  listView->setSortingEnabled(false);
#endif
}
void DataBasisWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void DataBasisWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void DataBasisWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  listView->setGeometry(1,1,w-5,h-2);
}
void DataBasisWidget::setDataBasis(vector<beamTime_parameter*> &algoIn, vector<run_parameter*> &runsIn )
{
  mode=1;
  bp2=&algoIn;
  rp2=&runsIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[bp2->size()];
#else
  QTreeWidgetItem *items[bp2->size()];
#endif
  for(unsigned int i=0;i<bp2->size();i++)
    {
      items[i]=0;
      if(bp2->at(i)==NULL)continue;
#if QT_VERSION < 0x040000
      items[i]=new QListViewItem(listView,bp2->at(i)->getName().data());
#else
      items[i]=new QTreeWidgetItem(listView,QStringList()<<bp2->at(i)->getName().data());
#endif
    }
  int which;
  for(unsigned int i=0;i<rp2->size();i++)
    {
      if(rp2->at(i)==NULL)continue;
      which=rp2->at(i)->getParentNumber();
      if(which<0||which>(int)bp2->size())
	continue;
      if(items[which]==NULL)continue;
#if QT_VERSION < 0x040000
      new QCheckListItem(items[which], rp2->at(i)->getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(items[which],QStringList()<< rp2->at(i)->getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
    }
}
void DataBasisWidget::setDataBasis( vector<beamTime_parameter> &algoIn, vector<run_parameter> &runsIn)
{
  mode=0;
  bp1=&algoIn;
  rp1=&runsIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[bp1->size()];
#else
  QTreeWidgetItem *items[bp1->size()];
#endif
  for(unsigned int i=0;i<bp1->size();i++)
    {
      items[i]=0;
#if QT_VERSION < 0x040000
      items[i]=new QListViewItem(listView,bp1->at(i).getName().data());
#else
      items[i]=new QTreeWidgetItem(listView,QStringList()<<bp1->at(i).getName().data());
#endif
    }
  int which;
  for(unsigned int i=0;i<rp1->size();i++)
    {
      which=rp1->at(i).getParentNumber();
      if(which<0||which>(int)bp1->size())
	continue;
      if(items[which]==NULL)continue;
#if QT_VERSION < 0x040000
      new QCheckListItem(items[which], rp1->at(i).getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(items[which], QStringList()<<rp1->at(i).getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
    }
}
void DataBasisWidget::setDataBasis(vector<beamTime_parameter*> &algoIn, vector<vector<run_parameter> > &runsIn )
{
  mode=3;
  bp2=&algoIn;
  rp3=&runsIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[bp2->size()];
#else
  QTreeWidgetItem *items[bp2->size()];
#endif
  for(unsigned int i=0;i<bp2->size();i++)
    {
      items[i]=0;
      if(bp2->at(i)==NULL)continue;
#if QT_VERSION < 0x040000
      items[i]=new QListViewItem(listView,bp2->at(i)->getName().data());
#else
      items[i]=new QTreeWidgetItem(listView,QStringList()<<bp2->at(i)->getName().data());
#endif
    }
  int which;
  for(unsigned int j=0;j<rp3->size();j++)
    {
      which=j;
      for(unsigned int i=0;i<rp3->at(j).size();i++)
	{
	  if(items[which]==NULL)continue;
#if QT_VERSION < 0x040000
      new QCheckListItem(items[which], rp3->at(j).at(i).getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(items[which], QStringList()<<rp3->at(j).at(i).getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
	}
    }
}
void DataBasisWidget::setDataBasis( vector<beamTime_parameter> &algoIn, vector<vector<run_parameter> > &runsIn)
{
  mode=2;
  bp1=&algoIn;
  rp3=&runsIn;
  listView->clear();
#if QT_VERSION < 0x040000
  QListViewItem *items[bp1->size()];
#else
  QTreeWidgetItem *items[bp1->size()];
#endif
  for(unsigned int i=0;i<bp1->size();i++)
    {
      items[i]=0;
#if QT_VERSION < 0x040000
      items[i]=new QListViewItem(listView,bp1->at(i).getName().data());
#else
      items[i]=new QTreeWidgetItem(listView,QStringList()<<bp1->at(i).getName().data());
#endif
    }
  int which;
  for(unsigned int j=0;j<rp3->size();j++)
    {
      which=j;
      for(unsigned int i=0;i<rp3->at(j).size();i++)
	{
	  if(items[which]==NULL)continue;
#if QT_VERSION < 0x040000
      new QCheckListItem(items[which], rp3->at(j).at(i).getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(items[which], QStringList()<<rp3->at(j).at(i).getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
	}
    }
}
bool DataBasisWidget::readOnly()
{
    return ReadOnly;
}
void DataBasisWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
}
/* void DataBasisWidget::onDatabasisSelect(QListViewItem* it) */
/* { */
/*   if(it==NULL)return; */
/*   if(mode==0) */
/*     { */
/*       for(unsigned int i=0;i<ap1->size();i++) */
/* 	if(it->text(0)==ap1->at(i).getName().data()) */
/* 	  pdisplay->setAlgorithm(&(ap1->at(i))); */
/*     } */
/*   else if(mode==1) */
/*     { */
/*        for(unsigned int i=0;i<ap2->size();i++) */
/* 	if(it->text(0)==ap2->at(i)->getName().data()) */
/* 	  pdisplay->setAlgorithm(ap2->at(i)); */
/*     } */
/* } */
#if QT_VERSION < 0x040000
beamTime_parameter* DataBasisWidget::getParameterFromItem(QListViewItem*it, int &s)
#else
beamTime_parameter* DataBasisWidget::getParameterFromItem(QTreeWidgetItem*it, int &s)
#endif
{
  if(it==NULL)return NULL;
  if(it->parent()==NULL)
    {
      beamTime_parameter *aps=0;
      if((mode%2)==0)
	{
	  for(unsigned int i=0;i<bp1->size();i++)
	    if(it->text(0)==bp1->at(i).getName().data())
	      aps=&(bp1->at(i));
	}
      else
	for(unsigned int i=0;i<bp2->size();i++)
	  if(it->text(0)==bp2->at(i)->getName().data())
	    aps=bp2->at(i);

      return aps;
   }
  else
    {
      run_parameter *aps=0;
      if(mode==0)
	{
	  for(unsigned int i=0;i<rp1->size();i++)
	    if(it->text(0)==rp1->at(i).getName().data())
	      aps=&(rp1->at(i));
	}
      else if(mode==1)
	{
	  for(unsigned int i=0;i<rp2->size();i++)
	    if(it->text(0)==rp2->at(i)->getName().data())
	      aps=rp2->at(i);
	}
      else
	{
	  for(unsigned int j=0;j<rp3->size();j++)
	    for(unsigned int i=0;i<rp3->at(j).size();i++)
	      if(it->text(0)==rp3->at(j).at(i).getName().data())
		aps=&(rp3->at(j).at(i));
	}
      return aps;
    }
}
#if QT_VERSION < 0x040000
QListViewItem* DataBasisWidget::getItemFromParameter(beamTime_parameter*al)
#else
QTreeWidgetItem* DataBasisWidget::getItemFromParameter(beamTime_parameter*al)
#endif
{
  if(al==NULL)return NULL;
#if QT_VERSION < 0x040000
  QListViewItem *ac=listView->firstChild();
  while(ac!=NULL)
    {
      if(ac->text(0)==al->getName().data())
	return ac;
      ac=ac->nextSibling();
    }
#else
  for(int i=0;i<listView->topLevelItemCount();i++)
    if(listView->topLevelItem(i)->text(0)==al->getName().data())
      return listView->topLevelItem(i);
#endif
  return NULL;
}
#if QT_VERSION < 0x040000
QListViewItem* DataBasisWidget::getItemFromParameter(run_parameter*al)
#else
QTreeWidgetItem* DataBasisWidget::getItemFromParameter(run_parameter*al)
#endif
{
  if(al==NULL)return NULL;
#if QT_VERSION < 0x040000
  QListViewItem *ac=listView->firstChild(),*ac1=0;
  while(ac!=NULL)
    {
      ac1=ac->firstChild();
      while(ac1!=NULL)
	{
	  if(ac1->text(0)==al->getName().data())
	    return ac1;
	  ac1=ac1->nextSibling();
	}
       ac=ac->nextSibling();
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
#if QT_VERSION < 0x040000
void DataBasisWidget::dataBasisClicked(int b,QListViewItem* it,const QPoint& p, int c)
#else
void DataBasisWidget::dataBasisClicked(QTreeWidgetItem* it)
#endif
{
  if(it==NULL)return;
  if(it->parent()==NULL)return;
  dataBasisChecked(it);
}
#if QT_VERSION < 0x040000
void DataBasisWidget::dataBasisChecked(QListViewItem* it)
#else
void DataBasisWidget::dataBasisChecked(QTreeWidgetItem* it)
#endif
{
  int n;
  beamTime_parameter *aps=getParameterFromItem(it,n);
  if(aps==NULL){
    QMessageBox::warning(this,"Beam-time/run not found", QString("Beam-time/run \"")+it->text(0)+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }
  if(it->parent()!=NULL)
#if QT_VERSION < 0x040000
    emit runSelected((run_parameter*)aps,((QCheckListItem*)it)->isOn());
#else
    emit runSelected((run_parameter*)aps,((QTreeWidgetItem*)it)->checkState(0)==Qt::Checked);
#endif
}
void DataBasisWidget::runSelect(const string & runName, bool selected)
{
  switch(mode)
    {
    case 0:
      {
	for(unsigned int i=0;i<rp1->size();i++)
	  if(runName==rp1->at(i).getName())
	    {
	      emit runSelected(&rp1->at(i),selected);
	      dataBasisSelect(rp1->at(i),selected);
	    }
	break;
      }
    case 1:
      {
	for(unsigned int i=0;i<rp2->size();i++)
	  if(runName==rp2->at(i)->getName())
	    {
	      emit runSelected((rp2->at(i)),selected);
	      dataBasisSelect(*(rp2->at(i)),selected);
	    }
	break;
      }
    case 2:
      {
	for(unsigned int j=0;j<rp3->size();j++)
	  for(unsigned int i=0;i<rp3->at(j).size();i++)
	    if(runName==rp3->at(j).at(i).getName())
	      {
		emit runSelected(&rp3->at(j).at(i),selected);
		dataBasisSelect(rp3->at(j).at(i),selected);
	      }
	break;
      }
    }
}
void DataBasisWidget::dataBasisSelect(run_parameter &algo, bool value)
{
#if QT_VERSION < 0x040000
  QListViewItem *it=getItemFromParameter(&algo);
#else
  QTreeWidgetItem *it=getItemFromParameter(&algo);
#endif
  if(it==NULL){
    QMessageBox::warning(this,"run not found", QString("run \"")+algo.getName().data()+"\" not found!", QMessageBox::Ok, QMessageBox::NoButton);
    return;
  }
#if QT_VERSION < 0x040000
  ((QCheckListItem*)it)->setOn(value);
#else
  ((QTreeWidgetItem*)it)->setCheckState(0,value?Qt::Checked:Qt::Unchecked);
#endif
}
#if QT_VERSION < 0x040000
void DataBasisWidget::dataBasisRightClicked(QListViewItem* it,const QPoint& p)
{
#else
#include <QHeaderView>
void DataBasisWidget::dataBasisRightClicked(const QPoint& p)
{
  QTreeWidgetItem* it=listView->itemAt(p);
#endif
  if(ReadOnly)return;
  currentItem=it;
#if QT_VERSION < 0x040000
  QPopupMenu pm(this);
  if(it!=NULL)
    {
      if(it->parent()!=NULL)
	pm.insertItem("Inspect",this,SLOT(showRun()));
      else
	pm.insertItem("Inspect",this,SLOT(showBeamTime()));
    }
  pm.insertItem("new Beam-Time",this,SLOT(addBeamTime()));
  if(it!=NULL)
    {
      pm.insertItem("delete Beam-Time",this,SLOT(deleteBeamTime()));
      pm.insertItem("new Run",this,SLOT(addRun()));
      if(it->parent()!=NULL)
	pm.insertItem("delete Run",this,SLOT(deleteRun()));
    }
  pm.exec(p);
#else
  QMenu pm(this);
  if(it!=NULL)
    {
      if(it->parent()!=NULL)
	pm.addAction("Inspect",this,SLOT(showRun()));
      else
	pm.addAction("Inspect",this,SLOT(showBeamTime()));
    }
  pm.addAction("new Beam-Time",this,SLOT(addBeamTime()));
  if(it!=NULL)
    {
      pm.addAction("delete Beam-Time",this,SLOT(deleteBeamTime()));
      pm.addAction("new Run",this,SLOT(addRun()));
      if(it->parent()!=NULL)
	pm.addAction("delete Run",this,SLOT(deleteRun()));
    }
  pm.exec(listView->viewport()->mapToGlobal(p));
#endif
}
void DataBasisWidget::deleteBeamTime()
{
  int num=-1;
  beamTime_parameter *aps=getParameterFromItem(currentItem,num);
  if(num<0||aps==0||currentItem==NULL){currentItem=0;return;}
  int ret=QMessageBox::warning(this,"Kill runs",QString("All runs belonging to beamtime\n")+aps->getName().data()+" will be deleted.\nContinue?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
  if(ret==QMessageBox::No){currentItem=0;return;}
  if((mode%2)==0)
    {
      if(num>(int)bp1->size()-1){currentItem=0;return;}
      vector<beamTime_parameter> tmp;
      while((int)bp1->size()>num+1)
	{
	  tmp.push_back(bp1->back());
	  bp1->pop_back();
	}
      bp1->pop_back();
      while(!tmp.empty())
	{
	  bp1->push_back(tmp.back());
	  tmp.pop_back();
	}
    }
  else
    {
      if(num>(int)bp2->size()-1){currentItem=0;return;}
      vector<beamTime_parameter*> tmp;
      while((int)bp2->size()>num+1)
	{
	  tmp.push_back(bp2->back());
	  bp2->pop_back();
	}
      bp2->pop_back();
      while(!tmp.empty())
	{
	  bp2->push_back(tmp.back());
	  tmp.pop_back();
	}
    }
  switch(mode)
    {
    case 0:
      {
	vector<run_parameter> tmp;
	while(!rp1->empty())
	  {
	    if(rp1->back().getParent()!=aps||rp1->back().getParentNumber()!=num)
	      tmp.push_back(rp1->back());
	    rp1->pop_back();
	  }
	while(!tmp.empty())
	  {
	    rp1->push_back(tmp.back());
	    tmp.pop_back();
	  }
	break;
      }
    case 1:
      {
	vector<run_parameter*> tmp;
	while(!rp2->empty())
	  {
	    if(rp2->back()->getParent()!=aps||rp2->back()->getParentNumber()!=num)
	      tmp.push_back(rp2->back());
	    rp2->pop_back();
	  }
	while(!tmp.empty())
	  {
	    rp2->push_back(tmp.back());
	    tmp.pop_back();
	  }
      }
    case 2:
    case 3:
      {
 	vector<vector<run_parameter> > tmp;
	while((int)rp3->size()>num+1)
	  {
	    tmp.push_back(rp3->back());
	    rp3->pop_back();
	  }
	while(!tmp.empty())
	  {
	    rp3->push_back(tmp.back());
	    tmp.pop_back();
	  }
	break;
      }
    }
  emit changed();
#if QT_VERSION < 0x040000
  listView->takeItem(currentItem);
#else
  listView->takeTopLevelItem(listView->indexOfTopLevelItem(currentItem));
#endif
  delete currentItem;
  currentItem=NULL;
}
#include "qsinglewidgetdialog.h"
#include "beamtimedisplay.h"
#include "rundisplay.h"
void DataBasisWidget::addBeamTime()
{
  QStringInputDialog fenst(this);
#if QT_VERSION < 0x040000
  fenst.setCaption("New Beam-Time");
#else
  fenst.setWindowTitle("New Beam-Time");
#endif
  fenst.setStart("new beamtime");
  fenst.exec();
  if(fenst.result()!=QDialog::Accepted){currentItem=0;return;}
  QSingleWidgetDialog fen2(this);
  BeamTimeDisplay *wid=new BeamTimeDisplay(&fen2);
  wid->setReadOnly(false);
  beamTime_parameter tmp(QS2s(fenst.line()));
  wid->setBeamTime(tmp);
  fen2.setWidget(wid);
  fen2.exec();
  if(fen2.result()==QDialog::Accepted)
    {
      if((mode%2)==0)
	bp1->push_back(beamTime_parameter(tmp));
      else
	bp2->push_back(new beamTime_parameter(tmp));
#if QT_VERSION < 0x040000
      new QListViewItem(listView,tmp.getName().data());
#else
      new QTreeWidgetItem(listView,QStringList()<<tmp.getName().data());
#endif
      emit changed();
    }
  delete wid;
}
void DataBasisWidget::showBeamTime()
{
  int n;
  beamTime_parameter *aps=getParameterFromItem(currentItem,n);
  if(aps==NULL){currentItem=0;return;}
  QSingleWidgetDialog fen2(this);
  BeamTimeDisplay *wid=new BeamTimeDisplay(&fen2);
  beamTime_parameter tmp(*aps);
  wid->setReadOnly(false);
  wid->setBeamTime(tmp);
  fen2.setWidget(wid);
  hasChanged=false;
  fen2.resize(600,400);
  connect(wid,SIGNAL(changed()),this,SLOT(changes()));
  fen2.exec();
  if(fen2.result()==QDialog::Accepted)
    {
      if(hasChanged)emit changed();
      (*aps)=tmp;
      currentItem->setText(0,tmp.getName().data());
    }
  currentItem=NULL;
  disconnect(wid);
  delete wid;
}
void DataBasisWidget::deleteRun()
{
  if(currentItem==NULL)return;
  if(currentItem->parent()==NULL){currentItem=0;return;}
  int n=-1;
  run_parameter *aps=(run_parameter*)getParameterFromItem(currentItem,n);
  int m=-1;
  getParameterFromItem(currentItem->parent(),m);
  if(n<0||aps==NULL){currentItem=0;return;}
  int ret=QMessageBox::warning(this,"Kill run",QString("Delete run\n")+aps->getName().data()+"\nContinue?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
  if(ret==QMessageBox::No){currentItem=0;return;}
  switch(mode)
    {
    case 0:
      {
	vector<run_parameter> tmp;
	while((int)rp1->size()>n+1)
	  {
	    tmp.push_back(rp1->back());
	    rp1->pop_back();
	  }
	rp1->pop_back();
	while(!tmp.empty())
	  {
	    rp1->push_back(tmp.back());
	    tmp.pop_back();
	  }
	break;
      }
    case 1:
      {
	vector<run_parameter*> tmp;
	while((int)rp2->size()>n+1)
	  {
	    tmp.push_back(rp2->back());
	    rp2->pop_back();
	  }
	rp2->pop_back();
	while(!tmp.empty())
	  {
	    rp2->push_back(tmp.back());
	    tmp.pop_back();
	  }
	break;
      }
    case 2:
    case 3:
      {
	vector<run_parameter> tmp;
	while((int)rp3->at(m).size()>n+1)
	  {
	    tmp.push_back(rp3->at(m).back());
	    rp3->at(m).pop_back();
	  }
	rp3->at(m).pop_back();
	while(!tmp.empty())
	  {
	    rp3->at(m).push_back(tmp.back());
	    tmp.pop_back();
	  }
	break;
      }
    }
  emit changed();
#if QT_VERSION < 0x040000
  listView->takeItem(currentItem);
#else
  currentItem->parent()->takeChild(currentItem->parent()->indexOfChild(currentItem));
#endif
  delete currentItem;
  currentItem=NULL;
}
void DataBasisWidget::addRun()
{
  if(currentItem==NULL)return;
  QStringInputDialog fenst(this);
#if QT_VERSION < 0x040000
  fenst.setCaption("New Run");
#else
  fenst.setWindowTitle("New Run");
#endif
  fenst.setStart("new run");
  fenst.exec();
  if(fenst.result()!=QDialog::Accepted){currentItem=0;return;}
  vector<string> nms;
  if((mode%2)==0)
    for(unsigned int i=0;i<bp1->size();i++)
      nms.push_back(bp1->at(i).getName());
  else
    for(unsigned int i=0;i<bp2->size();i++)
      nms.push_back(bp2->at(i)->getName());
  int n;
  beamTime_parameter *par=getParameterFromItem(currentItem->parent()==NULL?currentItem:currentItem->parent(),n);
  run_parameter tmp(*par,n);
  tmp.setName(QS2s(fenst.line()));
  tmp.setParentNumber(n);
  QSingleWidgetDialog fen2(this);
  RunDisplay *wid=new RunDisplay(&fen2);
  wid->setBeamtimes(nms);
  wid->setRunTypes(runTypes);
  wid->setReadOnly(false);
  wid->setRun(tmp);
  fen2.setWidget(wid);
  fen2.exec();
  if(fen2.result()==QDialog::Accepted)
    {
      if(tmp.getParentNumber()!=n)
	tmp.setParent(((mode%2)==0?&bp1->at(tmp.getParentNumber()):bp2->at(tmp.getParentNumber())));
      switch(mode)
	{
	case 0:
	  rp1->push_back(run_parameter(tmp));
	  break;
	case 1:
	  rp2->push_back(new run_parameter(tmp));
	  break;
	case 2:
	case 3:
	  rp3->at(tmp.getParentNumber()).push_back(run_parameter(tmp));
	  break;
	}
#if QT_VERSION < 0x040000
      QListViewItem *it=getItemFromParameter(tmp.getParent());
      if(it!=NULL)
	new QCheckListItem(it,tmp.getName().data(),QCheckListItem::CheckBox);
      else
	new QCheckListItem(listView->firstChild(),tmp.getName().data(),QCheckListItem::CheckBox);
#else
      QTreeWidgetItem *it=getItemFromParameter(tmp.getParent());
      if(it!=NULL)
	(new QTreeWidgetItem(it,QStringList()<<tmp.getName().data(),0))->setCheckState(0,Qt::Unchecked);
      else
	(new QTreeWidgetItem(listView->topLevelItem(0),QStringList()<<tmp.getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
      emit changed();
    }
  currentItem=NULL;
  delete wid;
}
void DataBasisWidget::showRun()
{
  int n;
  if(currentItem==NULL)return;
  if(currentItem->parent()==NULL){currentItem=NULL;return;}
  run_parameter *aps=(run_parameter*)getParameterFromItem(currentItem,n);
  if(aps==NULL){currentItem=0;return;}
  vector<string> nms;
  if((mode%2)==0)
    for(unsigned int i=0;i<bp1->size();i++)
      nms.push_back(bp1->at(i).getName());
  else
    for(unsigned int i=0;i<bp2->size();i++)
      nms.push_back(bp2->at(i)->getName());
  QSingleWidgetDialog fen2(this);
  RunDisplay *wid=new RunDisplay(&fen2);
  run_parameter tmp(*aps);
  wid->setBeamtimes(nms);
  wid->setRunTypes(runTypes);
  wid->setReadOnly(false);
  wid->setRun(tmp);
  fen2.setWidget(wid);
  fen2.resize(600,400);
  hasChanged=false;
  connect(wid,SIGNAL(changed()),this,SLOT(changes()));
  fen2.exec();
  if(fen2.result()==QDialog::Accepted)
    {
      if(aps->getParentNumber()!=tmp.getParentNumber())
	{
#if QT_VERSION < 0x040000
	  currentItem->parent()->takeItem(currentItem);
	  QListViewItem *it=getItemFromParameter((mode%2)==0?&bp1->at(tmp.getParentNumber()):bp2->at(tmp.getParentNumber()));
	  it->insertItem(currentItem);
#else
	  currentItem->parent()->takeChild(currentItem->parent()->indexOfChild(currentItem));
	  QTreeWidgetItem *it=getItemFromParameter((mode%2)==0?&bp1->at(tmp.getParentNumber()):bp2->at(tmp.getParentNumber()));
	  it->addChild(currentItem);
#endif
	}
      (*aps)=tmp;
      currentItem->setText(0,tmp.getName().data());
      if(hasChanged)
	emit changed();
    }
  disconnect(wid);
  delete wid;
  currentItem=NULL;
}
void DataBasisWidget::setRunTypes(const vector<string> & nmsIn)
{
  while(!runTypes.empty())runTypes.pop_back();
  for(unsigned int i=0;i<nmsIn.size();i++)
    runTypes.push_back(nmsIn[i]);
}
void DataBasisWidget::changes()
{hasChanged=true;}
