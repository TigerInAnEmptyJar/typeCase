#include "agenerateeventswithdecaywidget.h"
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#include <qlistview.h>
#else
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#endif
#include <qcheckbox.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <iostream>
#include <qspinbox.h>
#include "Eparticles.h"
#include "utilities.h"
#if QT_VERSION < 0x040000
QAGenerateEventsWithDecayWidget::QAGenerateEventsWithDecayWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAGenerateEventsWithDecayWidget");
#else
QAGenerateEventsWithDecayWidget::QAGenerateEventsWithDecayWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAGenerateEventsWithDecayWidget");
#endif
  ID=48;
  isInput=true;
  ap.setName("Generate Events with decay");
  ap.setID(ID);
  ap.setUse(true);
  vector<string> des;
  des.push_back("This algorithm generates events with the ROOT");
  des.push_back("Phase-Space-Generator. You specify the generated ");
  des.push_back("particles by their GEANT-ID and which of these");
  des.push_back("particles decayed off primary ones. A decay parameter");
  des.push_back("of -1 means a primary particle, otherwise it decayed");
  des.push_back("off the particle with the specified number. For each");
  des.push_back("decay a modification from phase-space can be given.");
  des.push_back("There can be given some trigger conditions for some");
  des.push_back("tracks (charged (+) or charged+uncharged(-)) to ");
  des.push_back("simulate acceptance.");
  ap.setDescription(des);
  vector<int> tmp;
  vector<string> tmps;
  ap.addParam<vector<int> >(single_parameter<vector<int> >("particles",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("decayed off",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("required Hits in detector",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("required Hits in OR- detector",tmp));
  ap.addParam<vector<string> >(single_parameter<vector<string> >("functions for decay",tmps));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("parameters for functions",tmp));
  ap.addParam<bool>(single_parameter<bool>("use Phase-Space",true));
  ap.addParam<bool>(single_parameter<bool>("simulate acceptance",false));
  useFunction=new QCheckBox(this);
  connect(useFunction,SIGNAL(clicked()),this,SLOT(OnUseFunctionClick()));
#if QT_VERSION < 0x040000
  partIDs=new QListView(this);
  partIDs->addColumn("Particle");
  partIDs->addColumn("ID");
  partIDs->addColumn("number");
  partIDs->addColumn("decay modification function");
  new QListViewItem(partIDs,"initial Reaction");
  params=new QListBox(this);
  accAnd=new QListBox(this);
  accOr=new QListBox(this);
  connect(partIDs,SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint &, int)),
	  this,SLOT(OnPartIDsClick(int,QListViewItem*, const QPoint&, int)));
  connect(params,SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint &)),
	  this,SLOT(OnParamsClick(int,QListBoxItem*)));
  connect(accAnd,SIGNAL(mouseButtonClicked(int,QListBoxItem *,const QPoint &)),this,SLOT(OnAccClick(int,QListBoxItem*))); 
  connect(accOr,SIGNAL(mouseButtonClicked(int,QListBoxItem *,const QPoint &)),this,SLOT(OnAccClick(int,QListBoxItem*))); 
#else
  partIDs=new QTreeWidget(this);
  partIDs->setHeaderLabels(QStringList()<<"Particle"<<"ID"<<"number"<<"decay modification function");
  new QTreeWidgetItem(partIDs,QStringList()<<"initial Reaction");
  params=new QListWidget(this);
  accAnd=new QListWidget(this);
  accOr=new QListWidget(this);
  partIDs->setContextMenuPolicy(Qt::CustomContextMenu);
  params->setContextMenuPolicy(Qt::CustomContextMenu);
  accAnd->setContextMenuPolicy(Qt::CustomContextMenu);
  accOr->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(partIDs,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnPartIDsClick(const QPoint&)));
  connect(params,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnParamsClick(const QPoint&)));
  connect(partIDs,SIGNAL(itemClicked(QTreeWidgetItem *,int)),
	  this,SLOT(OnPartIDsClick(QTreeWidgetItem*,int)));
  connect(params,SIGNAL(itemClicked(QListWidgetItem *)),
	  this,SLOT(OnParamsClick(QListWidgetItem*)));
  connect(accAnd,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnAccClick(const QPoint &)));
  connect(accOr,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnAccClick(const QPoint &)));
  connect(accAnd,SIGNAL(itemClicked(QListWidgetItem *)),
	  this,SLOT(OnAccClick(QListWidgetItem *)));
  connect(accOr,SIGNAL(itemClicked(QListWidgetItem *)),
	  this,SLOT(OnAccClick(QListWidgetItem *)));
#endif
  //////////
  acceptance=new QCheckBox(this);
  acceptance->setChecked(false);
  accAnd->setEnabled(false);
  accOr->setEnabled(false);
  connect(acceptance,SIGNAL(clicked()),this,SLOT(OnAcceptance()));
  useFunction->setText("use Phase-Space");
  useFunction->setChecked(true);
  params->setEnabled(!useFunction->isChecked());
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}
QAGenerateEventsWithDecayWidget::~QAGenerateEventsWithDecayWidget()
{
  disconnect(useFunction);
  disconnect(partIDs);
  disconnect(params);
  disconnect(accAnd);
  disconnect(accOr);
  disconnect(acceptance);
  delete useFunction;
  delete params;
  delete partIDs;
  delete accAnd;
  delete accOr;
  delete acceptance;
}
void QAGenerateEventsWithDecayWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int cbH=20;
  int leH=25;
  int abf=5;
  if(w<=40-2*abf)return;
  if(h-60-cbH*3-abf*5-leH<=0)return;
  partIDs->setGeometry(5,abf,w/2-10-abf,(h-abf*3-25)/2);
  params->setGeometry(5,abf*2+(h-abf*3-25)/2,w/2-10-abf,(h-abf*3-25)/2);
  useFunction->setGeometry(w/2-5,5,w/2-10-abf,cbH);
  acceptance->setGeometry(w/2-5,5+cbH+abf,w/2-10-abf,cbH);
  accAnd->setGeometry(w/2-5,5+cbH*2+abf*2,w/2-10-abf,(h-5-cbH*2-abf*4)/2);
  accOr->setGeometry(w/2-5,5+cbH*2+abf*3+(h-5-cbH*2-abf*4)/2,w/2-10-abf,(h-5-cbH*2-abf*4)/2);
}
void QAGenerateEventsWithDecayWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAGenerateEventsWithDecayWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAGenerateEventsWithDecayWidget::OnUseFunctionClick()
{
  //  function->setEnabled(useFunction->isChecked());
  ap.changeParam<bool>("use Phase-Space",useFunction->isChecked());
  params->setEnabled(!useFunction->isChecked());
}
// void QAGenerateEventsWithDecayWidget::OnFunctionChange()
// {
//   //  ap.changeParam<string>("Function_to_modify_weight",function->text());
// }
#include "qintegerinputdialog.h"
#include "qstringinputdialog.h"
#if QT_VERSION < 0x040000
void QAGenerateEventsWithDecayWidget::OnPartIDsClick(int button, QListViewItem *it, const QPoint &p, int column)
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  switch(button)
    {
    case 1://left
      {
	if(column==0)
	  {
	    int n=QIntegerInputDialog::getLine(this,"new particle ID",14);
	    if(n>0)
	      {
		if(it!=NULL)
		  new QListViewItem(it,Eparticles::getNameG(n).data(),QString::number(n),"");
		else
		  new QListViewItem(partIDs->firstChild(),Eparticles::getNameG(n),QString::number(n),"");
	      }
	  }
	else
	  {
	    if(it==NULL)return;
	    switch(column)
	      {
	      case 0:
	      case 1:
		{
		  int n=QIntegerInputDialog::getLine(this,"change particle ID",it->text(1).toInt());
		  if(n>0)
		    {
		      it->setText(0,Eparticles::getNameG(n).data());
		      it->setText(1,QString::number(n));
		    }
		  break;
		}
	      case 3:
		{
		  if(!useFunction->isChecked())return;
		  if(it->childCount()<=0)return;
		  QString val=QStringInputDialog::getLine(this,"change decay function",it->text(3));
		  if(val!="")
		    it->setText(3,val);
		  break;
		}
	      }
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	partIDs->takeItem(it);
	delete it;
	break;
      }
    }
  getParticlesFromView();
}
#else
void QAGenerateEventsWithDecayWidget::OnPartIDsClick(QTreeWidgetItem *it, int column)
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  if(column==0)
    {
      int n=QIntegerInputDialog::getLine(this,"new particle ID",14);
      if(n>0)
	{
	  if(it!=NULL)
	    new QTreeWidgetItem(it,QStringList()<<Eparticles::getNameG(n).data()<<QString::number(n)<<"");
	  else
	    new QTreeWidgetItem(partIDs->topLevelItem(0),QStringList()<<Eparticles::getNameG(n).data()<<QString::number(n)<<"");
	}
    }
  else
    {
      if(it==NULL)return;
      switch(column)
	{
	case 0:
	case 1:
	  {
	    int n=QIntegerInputDialog::getLine(this,"change particle ID",it->text(1).toInt());
	    if(n>0)
	      {
		it->setText(0,Eparticles::getNameG(n).data());
		it->setText(1,QString::number(n));
	      }
	    break;
	  }
	case 3:
	  {
	    if(!useFunction->isChecked())return;
	    if(it->childCount()<=0)return;
	    QString val=QStringInputDialog::getLine(this,"change decay function",it->text(3));
	    if(val!="")
	      it->setText(3,val);
	    break;
	  }
	}
    }
  getParticlesFromView();
}
void QAGenerateEventsWithDecayWidget::OnPartIDsClick(const QPoint &p)
{
  QTreeWidgetItem *it=partIDs->itemAt(p);
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  if(it==NULL)return;
  partIDs->takeTopLevelItem(partIDs->indexOfTopLevelItem(it));
  delete it;
  getParticlesFromView();
}
#endif
#if QT_VERSION < 0x040000
void QAGenerateEventsWithDecayWidget::OnParamsClick(int button, QListBoxItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  switch(button)
    {
    case 1://left
      {
	int n=QIntegerInputDialog::getLine(this,"Parameter");
	if(n!=-10000)
	  {
	    tmp.push_back(n);
	    params->insertItem(QString::number(n));
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	int num=params->index(it);
	params->removeItem(num);
	delete it;
	takeItemFromVector<int>(tmp,tmp[num]);
	break;
      }
    }
  ap.changeParam<vector<int> >("parameters for functions",tmp);
}
#else
void QAGenerateEventsWithDecayWidget::OnParamsClick(QListWidgetItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  int n=QIntegerInputDialog::getLine(this,"Parameter");
  if(n!=-10000)
    {
      tmp.push_back(n);
      params->addItem(QString::number(n));
    }
  ap.changeParam<vector<int> >("parameters for functions",tmp);
}
void QAGenerateEventsWithDecayWidget::OnParamsClick(const QPoint &p)
{
  QListWidgetItem *it=params->itemAt(p);
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  if(it==NULL)return;
  int num=params->row(it);
  params->takeItem(num);
  delete it;
  takeItemFromVector<int>(tmp,tmp[num]);
  ap.changeParam<vector<int> >("parameters for functions",tmp);
}
#endif
void QAGenerateEventsWithDecayWidget::OnAcceptance()
{
  ap.changeParam<bool>("simulate acceptance",acceptance->isChecked());
  accAnd->setEnabled(acceptance->isChecked());
  accOr->setEnabled(acceptance->isChecked());
}
//  connect(acceptance,SIGNAL(clicked()),this,SLOT(OnAcceptance()));
#if QT_VERSION < 0x040000
void QAGenerateEventsWithDecayWidget::OnAccClick(int button,QListBoxItem* it)
{
  if(it==NULL)return;
  QListBox *lw=it->listBox();
  int num=lw->index(it);
  if(button==1)//left
    {
	string cap;
	if(lw==accAnd)
	  cap=("Add \"and\"-requirement");
	else
	  cap=("Add \"or\"-requirement");
	int n=QIntegerInputDialog::getLine(this,cap.data());
	if(n!=-10000)
	  {
	    if(lw->count()<=1)
	      lw->changeItem(QString::number(n),it->listBox()->index(it));
	    else
	      lw->insertItem(QString::number(n));
	    if(lw==accAnd)
	      {
		vector<int> tmpV=ap.getParam<vector<int> >(2).getData();//"required Hits in detector");
		tmpV.push_back(n);
		ap.changeParam<vector<int> >("required Hits in detector",tmpV);
	      }
	    else
	      {
		vector<int> tmpV=ap.getParam<vector<int> >(3).getData();//"required Hits in OR- detector");
		tmpV.push_back(n);
		ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
	      }
	  }
    }
  else //right
    {
      if(lw==accAnd)
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(2).getData();//"required Hits in detector");
	  takeItemFromVector<int>(tmpV,tmpV[num]);
	  ap.changeParam<vector<int> >("required Hits in detector",tmpV);
	}
      else
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(3).getData();//"required Hits in OR- detector");
	  takeItemFromVector<int>(tmpV,tmpV[num]);
	  ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
	}
      if(lw->count()<=1)
	lw->changeItem("new Item",0);
      else
	lw->removeItem(lw->index(it));
    }
}
#else
void QAGenerateEventsWithDecayWidget::OnAccClick(QListWidgetItem* it)
{
  if(it==NULL)return;
  QListWidget* lw=it->listWidget();
//   int num=lw->row(it);
  string cap;
  if(lw==accAnd)
    cap=("Add \"and\"-requirement");
  else
    cap=("Add \"or\"-requirement");
  int n=QIntegerInputDialog::getLine(this,cap.data());
  if(n!=-10000)
    {
      if(lw->count()<=1)
	lw->item(lw->row(it))->setText(QString::number(n));
      else
	lw->addItem(QString::number(n));
      if(lw==accAnd)
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(2).getData();//"required Hits in detector");
	  tmpV.push_back(n);
	  ap.changeParam<vector<int> >("required Hits in detector",tmpV);
	}
      else
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(3).getData();//"required Hits in OR- detector");
	  tmpV.push_back(n);
	  ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
	}
    }
}
void QAGenerateEventsWithDecayWidget::OnAccClick(const QPoint &p)
{
  QListWidgetItem* it=accAnd->itemAt(p);
  if(it==NULL)it=accOr->itemAt(p);
  if(it==NULL)return;
  QListWidget* lw=it->listWidget();
  int num=lw->row(it);
  if(lw==accAnd)
    {
      vector<int> tmpV=ap.getParam<vector<int> >(2).getData();//"required Hits in detector");
      takeItemFromVector<int>(tmpV,tmpV[num]);
      ap.changeParam<vector<int> >("required Hits in detector",tmpV);
    }
  else
    {
      vector<int> tmpV=ap.getParam<vector<int> >(3).getData();//"required Hits in OR- detector");
      takeItemFromVector<int>(tmpV,tmpV[num]);
      ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
    }
  if(lw->count()<=1)
    lw->item(0)->setText("new Item");
  else
    delete lw->takeItem(num);
}
#endif
void QAGenerateEventsWithDecayWidget::getParticlesFromView()
{
  vector<int> pid;
  vector<string>fu;
  vector<int> df;
#if QT_VERSION < 0x040000
  vector<QListViewItem*>allp;
  QListViewItem *it=partIDs->firstChild();
  it=it->firstChild();
  while(it!=NULL)
    {
      allp.push_back(it);
      it=it->nextSibling();
    }
#else
  vector<QTreeWidgetItem*>allp;
  for(int i=0;i<partIDs->topLevelItemCount();i++)
    allp.push_back(partIDs->topLevelItem(i));
#endif
  int n=0;
  while(n<(int)allp.size())
    {
      if(allp[n]->childCount()>0)
	{
#if QT_VERSION < 0x040000
	  it=allp[n]->firstChild();
	  while(it!=NULL)
	    {
	      allp.push_back(it);
	      it=it->nextSibling();
	    }
#else
	  for(int i=0;i<allp[n]->childCount();i++)
	    allp.push_back(allp[n]->child(i));
#endif
	}
      n++;
    }
  for(unsigned int i=0;i<allp.size();i++)
    {
      allp[i]->setText(2,QString::number(pid.size()));
      pid.push_back(allp[i]->text(1).toInt());
      n=-1;
      for(unsigned int j=0;j<i;j++)
	if(allp[i]->parent()==allp[j])
	  n=j;
      df.push_back(n);
      if(n<0)fu.push_back(QS2s(allp[i]->text(3)));
    }
  ap.changeParam<vector<int> >("particles",pid);
  ap.changeParam<vector<int> >("decayed off",df);
  ap.changeParam<vector<string> >("functions for decay",fu);
}
