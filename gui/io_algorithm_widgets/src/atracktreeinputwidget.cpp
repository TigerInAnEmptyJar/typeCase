#include "atracktreeinputwidget.h"
#include "qstringinputdialog.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include <qspinbox.h>
#include <qlineedit.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#endif
#include <qlayout.h>
#include "utilities.h"

#if QT_VERSION < 0x040000
QATrackTreeInputWidget::QATrackTreeInputWidget( QWidget* parent, const char* name, WFlags fl ):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QATrackTreeInputWidget");
#else
QATrackTreeInputWidget::QATrackTreeInputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QATrackTreeInputWidget");
#endif
  ID=62;
  isInput=true;
  ap.setName("Read tracks from track tree");
  ap.setID(ID);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("search for event",false));
  ap.addParam<bool>(single_parameter<bool>("Use as event input list",false));
  ap.addParam<bool>(single_parameter<bool>("read event pattern",false));
  ap.addParam<bool>(single_parameter<bool>("Use local directory",false));
  ap.addParam<string>(single_parameter<string>("local directory",""));
  vector<int> tmp;tmp.push_back(0);tmp.push_back(0);tmp.push_back(0);
  ap.addParam<vector<int> >(single_parameter<vector<int> >("event pattern",tmp));
  searchForEvent=new QCheckBox(this);
  useAsEventList=new QCheckBox(this);
  useLocalDir=new QCheckBox(this);
  localDir=new QLineEdit(this);
  useEventPattern=new QCheckBox(this);
  prptt=new QSpinBox(this);
  kiptt=new QSpinBox(this);
  veptt=new QSpinBox(this);
  useLocalDir->setChecked(false);
  localDir->setEnabled(useLocalDir->isChecked());
  QGridLayout *layout=new QGridLayout(this);
  label1=new QLabel(this);
  layout->addWidget(label1,3,0);
  layout->addWidget(prptt,3,1);
  layout->addWidget(kiptt,3,2);
  layout->addWidget(veptt,3,3);
#if QT_VERSION < 0x040000
  layout->addMultiCellWidget(searchForEvent,0,0,0,3);
  layout->addMultiCellWidget(useAsEventList,1,1,0,3);
  layout->addMultiCellWidget(useEventPattern,2,2,0,3);
  layout->addMultiCellWidget(useLocalDir,4,4,0,3);
  layout->addMultiCellWidget(localDir,5,5,0,3);
#else
  layout->addWidget(searchForEvent,0,0,1,4);
  layout->addWidget(useAsEventList,1,0,1,4);
  layout->addWidget(useEventPattern,2,0,1,4);
  layout->addWidget(useLocalDir,4,0,1,4);
  layout->addWidget(localDir,5,0,1,4);
#endif
  connect(useAsEventList,SIGNAL(clicked()),this,SLOT(onUseEventListChange()));
  connect(useEventPattern,SIGNAL(clicked()),this,SLOT(onPatternChange()));
  connect(useLocalDir,SIGNAL(clicked()),this,SLOT(onUseLocalChange()));
  connect(searchForEvent,SIGNAL(clicked()),this,SLOT(onSearchChange()));
  connect(localDir,SIGNAL(textChanged(const QString&)),this,SLOT(onLocalChange()));
  connect(prptt,SIGNAL(valueChanged(int)),this,SLOT(onPatternPChange()));
  connect(kiptt,SIGNAL(valueChanged(int)),this,SLOT(onPatternKChange()));
  connect(veptt,SIGNAL(valueChanged(int)),this,SLOT(onPatternVChange()));
  resize(width(),height());
  languageChange();
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

void QATrackTreeInputWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption(tr("ATrackTreeInput"));
#else
    setWindowTitle(tr("ATrackTreeInput"));
#endif
  searchForEvent->setText(tr("search for given event"));
  useAsEventList->setText(tr("use entries as event input list"));
  useLocalDir->setText(tr("use local directory"));
  useEventPattern->setText(tr("use event pattern"));
  label1->setText(tr("Input pattern"));
}
QATrackTreeInputWidget::~QATrackTreeInputWidget()
{
}
void QATrackTreeInputWidget::onUseLocalChange()
{
  localDir->setEnabled(useLocalDir->isChecked());
  ap.changeParam<bool>("Use local directory",useLocalDir->isChecked());
}
void QATrackTreeInputWidget::onLocalChange()
{
  ap.changeParam<string>("local directory",QS2s(localDir->text()));
}
void QATrackTreeInputWidget::onUseEventListChange()
{
  ap.changeParam<bool>("Use as event input list",useAsEventList->isChecked());
}
void QATrackTreeInputWidget::onSearchChange()
{
  ap.changeParam<bool>("search for event",searchForEvent->isChecked());
}
void QATrackTreeInputWidget::onUsePatternChange()
{
  ap.changeParam<bool>("search for event",searchForEvent->isChecked());
}
void QATrackTreeInputWidget::onPatternPChange()
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  tmp[0]=prptt->value();
  ap.changeParam<vector<int> >("event pattern",tmp);
}
void QATrackTreeInputWidget::onPatternKChange()
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  tmp[1]=kiptt->value();
  ap.changeParam<vector<int> >("event pattern",tmp);
}
void QATrackTreeInputWidget::onPatternVChange()
{
  vector<int> tmp=ap.getParam<vector<int> >(0).getData();
  tmp[2]=veptt->value();
  ap.changeParam<vector<int> >("event pattern",tmp);
}
