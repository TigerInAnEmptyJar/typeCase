#include "agenerateeventswithdecaywidget.h"
#include <qlistbox.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <iostream>
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
  QStringList des;
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
  ap.setDiscription(des);
  vector<int> tmp;
  vector<QString> tmps;
  ap.addParam<vector<int> >(single_parameter<vector<int> >("particles",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("decayed off",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("required Hits in detector",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("required Hits in OR- detector",tmp));
  ap.addParam<vector<QString> >(single_parameter<vector<QString> >("functions for decay",tmps));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("parameters for functions",tmp));
  ap.addParam<bool>(single_parameter<bool>("use Phase-Space",true));
  ap.addParam<bool>(single_parameter<bool>("simulate acceptance",false));
  useFunction=new QCheckBox(this);
  connect(useFunction,SIGNAL(clicked()),this,SLOT(OnUseFunctionClick()));
  partIDs=new QListView(this);
  partIDs->addColumn("Particle");
  partIDs->addColumn("ID");
  partIDs->addColumn("number");
  partIDs->addColumn("decay modification function");
  new QListViewItem(partIDs,"initial Reaction");
  connect(partIDs,SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint &, int)),
	  this,SLOT(OnPartIDsClick(int,QListViewItem*, const QPoint&, int)));
  params=new QListBox(this);
  connect(params,SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint &)),
	  this,SLOT(OnParamsClick(int,QListBoxItem*)));
  //////////
  acceptance=new QCheckBox(this);
  acceptance->setChecked(false);
  accAnd=new QListBox(this);
  accOr=new QListBox(this);
  accAnd->setEnabled(false);
  accOr->setEnabled(false);
  connect(acceptance,SIGNAL(clicked()),this,SLOT(OnAcceptance()));
#if QT_VERSION < 0x040000
  connect(accAnd,SIGNAL(mouseButtonClicked(int,QListBoxItem *,const QPoint &)),this,SLOT(OnAccClick(int,QListBoxItem*))); 
  connect(accOr,SIGNAL(mouseButtonClicked(int,QListBoxItem *,const QPoint &)),this,SLOT(OnAccClick(int,QListBoxItem*))); 
#else
  accAnd->setContextMenuPolicy(Qt::CustomContextMenu);
  accOr->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(accAnd,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(OnAccClick(QListWidgetItem*))); 
  connect(accOr,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(OnAccClick(QListWidgetItem*))); 
  connect(accAnd,SIGNAL(customContextMenuRequested ( const QPoint & )),this,SLOT(OnAccClick(const QPoint&))); 
  connect(accOr,SIGNAL(customContextMenuRequested ( const QPoint & )),this,SLOT(OnAccClick(const QPoint&))); 
#endif
  useFunction->setText("use Phase-Space");
  useFunction->setChecked(true);
  params->setEnabled(!useFunction->isChecked());
  resize(width(),height());
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
//   //  ap.changeParam<QString>("Function_to_modify_weight",function->text());
// }
#include "qintegerinputdialog.h"
#include "qstringinputdialog.h"
void QAGenerateEventsWithDecayWidget::OnPartIDsClick(int button, QListViewItem *it, const QPoint &p, int column)
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  switch(button)
    {
    case 1://left
      {
	if(column==0)
	  {
	    QIntegerInputDialog f(this);
	    f.setCaption("new particle ID");
	    f.exec();
	    if(f.result()==QDialog::Accepted)
	      {
		if(it!=NULL)
		  new QListViewItem(it,Eparticles::getNameG(f.line()),QString::number(f.line()),"");
		else
		  new QListViewItem(partIDs->firstChild(),Eparticles::getNameG(f.line()),QString::number(f.line()),"");
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
		  QIntegerInputDialog f(this);
		  f.setCaption("change particle ID");
		  f.spinBox5->setValue(it->text(1).toInt());
		  f.exec();
		  if(f.result()==QDialog::Accepted)
		    {
		      it->setText(0,Eparticles::getNameG(f.line()));
		      it->setText(1,QString::number(f.line()));
		    }
		  break;
		}
	      case 3:
		{
		  if(!useFunction->isChecked())return;
		  if(it->childCount()<=0)return;
		  QStringInputDialog f(this);
		  f.setCaption("change decay function");
		  f.lineEdit->setText(it->text(3));
		  f.exec();
		  if(f.result()==QDialog::Accepted)
		    {
		      it->setText(3,f.line());
		    }
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
void QAGenerateEventsWithDecayWidget::OnParamsClick(int button, QListBoxItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  switch(button)
    {
    case 1://left
      {
	QIntegerInputDialog f(this);
	f.exec();
	if(f.result()==QDialog::Accepted)
	  {
	    tmp.push_back(f.line());
	    params->insertItem(QString::number(f.line()));
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	int num=params->index(it);
	takeItemFromVector<int>(tmp,tmp[num]);
	params->removeItem(num);
	break;
      }
    }
  ap.changeParam<vector<int> >("parameters for functions",tmp);
}
void QAGenerateEventsWithDecayWidget::OnAcceptance()
{
  ap.changeParam<bool>("simulate acceptance",acceptance->isChecked());
  accAnd->setEnabled(acceptance->isChecked());
  accOr->setEnabled(acceptance->isChecked());
}
//  connect(acceptance,SIGNAL(clicked()),this,SLOT(OnAcceptance()));
void QAGenerateEventsWithDecayWidget::OnAccClick(int button,QListBoxItem* it)
{
  if(it==NULL)return;
  if(button==1)//left
    {
	QIntegerInputDialog f(this);
	if(it->listBox()==accAnd)
	  f.setCaption("Add \"and\"-requirement");
	else
	  f.setCaption("Add \"or\"-requirement");
	f.exec();
	if(f.result()==QDialog::Accepted)
	  {
	    if(it->listBox()->count()<=1)
	      it->setText(QString::number(f.line()));
	    else
	      it->listBox()->insertItem(QString::number(f.line()));
	    if(it->listBox()==accAnd)
	      {
		vector<int> tmpV=ap.getParam<vector<int> >(2);//"required Hits in detector");
		tmpV.push_back(f.line());
		ap.changeParam<vector<int> >("required Hits in detector",tmpV);
	      }
	    else
	      {
		vector<int> tmpV=ap.getParam<vector<int> >(3);//"required Hits in OR- detector");
		tmpV.push_back(f.line());
		ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
	      }
	  }
    }
  else //right
    {
      QListBox *tmpL=it->listBox();
      if(tmpL==accAnd)
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(2);//"required Hits in detector");
	  int num=tmpL->index(it);
	  takeItemFromVector<int>(tmpV,tmpV[num]);
	  ap.changeParam<vector<int> >("required Hits in detector",tmpV);
	}
      else
	{
	  vector<int> tmpV=ap.getParam<vector<int> >(3);//"required Hits in OR- detector");
	  int num=tmpL->index(it);
	  takeItemFromVector<int>(tmpV,tmpV[num]);
	  ap.changeParam<vector<int> >("required Hits in OR- detector",tmpV);
	}
      if(tmpL->count()<=1)
	it->setText("new Item");
      else
	tmpL->removeItem(tmpL->index(it));
    }
}
void QAGenerateEventsWithDecayWidget::getParticlesFromView()
{
  vector<int> pid;
  vector<QString>fu;
  vector<int> df;
  vector<QListViewItem*>allp;
  QListViewItem *it=partIDs->firstChild();
  it=it->firstChild();
  while(it!=NULL)
    {
      allp.push_back(it);
      it=it->nextSibling();
    }
  int n=0;
  while(n<allp.size())
    {
      if(allp[n]->childCount()>0)
	{
	  it=allp[n]->firstChild();
	  while(it!=NULL)
	    {
	      allp.push_back(it);
	      it=it->nextSibling();
	    }
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
      if(n<0)fu.push_back(allp[i]->text(3));
    }
  ap.changeParam<vector<int> >("particles",pid);
  ap.changeParam<vector<int> >("decayed off",df);
  ap.changeParam<vector<int> >("functions for decay",fu);
}
