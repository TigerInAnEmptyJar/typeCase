#include "agenerateeventswidget.h"
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#endif
#include <qcheckbox.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <iostream>
#include "utilities.h"
#if QT_VERSION < 0x040000
QAGenerateEventsWidget::QAGenerateEventsWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAGenerateEventsWidget");
#else
QAGenerateEventsWidget::QAGenerateEventsWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAGenerateEventsWidget");
#endif
  ID=39;
  isInput=true;
  ap.setName("Generate Events");
  ap.setID(ID);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("use SAID function",false));
  ap.addParam<string>(single_parameter<string>("Function_to_modify_weight",""));
  vector<int> tmp;
  tmp.push_back(14);
  tmp.push_back(14);
  ap.addParam<vector<int> >(single_parameter<vector<int> >("Particle_IDs",tmp));
  ap.addParam<vector<int> >(single_parameter<vector<int> >("parameter_of_function",vector<int>()));
  useFunction=new QCheckBox(this);
  connect(useFunction,SIGNAL(clicked()),this,SLOT(OnUseFunctionClick()));
#if QT_VERSION < 0x040000
  partIDs=new QListBox(this);
  params=new QListBox(this);
  partIDs->insertItem(QString::number(14));
  partIDs->insertItem(QString::number(14));
  connect(partIDs,SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint &)),
	  this,SLOT(OnPartIDsClick(int,QListBoxItem*)));
  connect(params,SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint &)),
	  this,SLOT(OnParamsClick(int,QListBoxItem*)));
#else
  partIDs=new QListWidget(this);
  params=new QListWidget(this);
  partIDs->addItem(QString::number(14));
  partIDs->addItem(QString::number(14));
  partIDs->setContextMenuPolicy(Qt::CustomContextMenu);
  params->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(partIDs,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnPartIDsClick(const QPoint&)));
  connect(params,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnParamsClick(const QPoint&)));
  connect(partIDs,SIGNAL(itemClicked(QListWidgetItem *)),
	  this,SLOT(OnPartIDsClick(QListWidgetItem*)));
  connect(params,SIGNAL(itemClicked(QListWidgetItem *)),
	  this,SLOT(OnParamsClick(QListWidgetItem*)));
#endif
  function=new QLineEdit(this);
  connect(function,SIGNAL(textChanged ( const QString & )),this,SLOT(OnFunctionChange()));
  label1=new QLabel(this);
  label2=new QLabel(this);
  label3=new QLabel(this);
  label1->setText("Particle IDs");
  label2->setText("Parameter of Function");
  label3->setText("Function");
  useFunction->setText("use SAID function to modify weight");
  function->setEnabled(useFunction->isChecked());
  params->setEnabled(useFunction->isChecked());
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}
QAGenerateEventsWidget::~QAGenerateEventsWidget()
{
  disconnect(useFunction);
  disconnect(partIDs);
  disconnect(params);
  disconnect(function);
  delete useFunction;
  delete params;
  delete partIDs;
  delete function;
  delete label1;
  delete label2;
  delete label3;
}
void QAGenerateEventsWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int cbH=20;
  int leH=25;
  int abf=5;
  if(w<=40-2*abf)return;
  if(h-60-cbH*3-abf*5-leH<=0)return;
  label1->setGeometry(10,10,w/2-10,cbH);
  partIDs->setGeometry(10,10+cbH+abf,w/2-10-abf,h-cbH-60-abf);
  useFunction->setGeometry(w/2+5,10,w/2-20-abf,cbH);
  label3->setGeometry(w/2+5,10+cbH+abf,w/2-20-abf,cbH);
  function->setGeometry(w/2+5,10+cbH*2+abf*2,w/2-20-abf,leH);
  label2->setGeometry(w/2+5,10+cbH*2+abf*3+leH,w/2-20-abf,cbH);
  params->setGeometry(w/2+5,10+cbH*3+abf*4+leH,w/2-20-abf,h-60-cbH*3-abf*5-leH);
}
void QAGenerateEventsWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAGenerateEventsWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAGenerateEventsWidget::OnUseFunctionClick()
{
  function->setEnabled(useFunction->isChecked());
  ap.changeParam<bool>("use SAID function",useFunction->isChecked());
  params->setEnabled(useFunction->isChecked());
}
void QAGenerateEventsWidget::OnFunctionChange()
{
  ap.changeParam<string>("Function_to_modify_weight",QS2s(function->text()));
}
#include "qintegerinputdialog.h"
#if QT_VERSION < 0x040000
void QAGenerateEventsWidget::OnPartIDsClick(int button, QListBoxItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  switch(button)
    {
    case 1://left
      {
	int n=QIntegerInputDialog::getLine(this,"new particle ID",14);
	if(n>0)
	  {
	    tmp.push_back(n);
	    partIDs->insertItem(QString::number(tmp.back()));
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	int num=partIDs->index(it);
	takeItemFromVector<int>(tmp,tmp[num]);
	partIDs->removeItem(partIDs->index(it));
	break;
      }
    }
  ap.changeParam<vector<int> >("Particle_IDs",tmp);
}
#else
void QAGenerateEventsWidget::OnPartIDsClick(QListWidgetItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  int n=QIntegerInputDialog::getLine(this,"new particle ID",14);
  if(n>0)
    {
      tmp.push_back(n);
      partIDs->addItem(QString::number(tmp.back()));
    }
  ap.changeParam<vector<int> >("Particle_IDs",tmp);
}
void QAGenerateEventsWidget::OnPartIDsClick(const QPoint & p)
{
  QListWidgetItem *it=partIDs->itemAt(p);
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  if(it==NULL)return;
  int num=partIDs->row(it);
  takeItemFromVector<int>(tmp,tmp[num]);
  delete partIDs->takeItem(partIDs->row(it));
  ap.changeParam<vector<int> >("Particle_IDs",tmp);
}
#endif
#if QT_VERSION < 0x040000
void QAGenerateEventsWidget::OnParamsClick(int button, QListBoxItem *it)
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
	int num=partIDs->index(it);
	takeItemFromVector<int>(tmp,tmp[num]);
	break;
      }
    }
  ap.changeParam<vector<int> >("parameter_of_function",tmp);
}
#else
void QAGenerateEventsWidget::OnParamsClick(QListWidgetItem *it)
{
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  int n=QIntegerInputDialog::getLine(this,"Parameter");
  if(n!=-10000)
    {
      tmp.push_back(n);
      params->addItem(QString::number(n));
    }
  ap.changeParam<vector<int> >("parameter_of_function",tmp);
}
void QAGenerateEventsWidget::OnParamsClick(const QPoint &p)
{
  vector<int> tmp=ap.getParam<vector<int> >(1).getData();
  QListWidgetItem *it=params->itemAt(p);
  if(it==NULL)return;
  int num=params->row(it);
  delete params->takeItem(num);
  takeItemFromVector<int>(tmp,tmp[num]);
  ap.changeParam<vector<int> >("parameter_of_function",tmp);
}
#endif
