#include "ahittreeinputwidget.h"
#include "qstringinputdialog.h"
#include "qcheckbox.h"
#include "qlabel.h"
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
QAHitTreeInputWidget::QAHitTreeInputWidget( QWidget* parent, const char* name, WFlags fl ):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAHitTreeInputWidget");
#else
QAHitTreeInputWidget::QAHitTreeInputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAHitTreeInputWidget");
#endif
  ID=60;
  isInput=true;
  ap.setName("Read hits from hit tree");
  ap.setID(ID);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("search for event",false));
  ap.addParam<bool>(single_parameter<bool>("Use as event input list",false));
  ap.addParam<bool>(single_parameter<bool>("Use local directory",false));
  ap.addParam<string>(single_parameter<string>("local directory",""));
  searchForEvent=new QCheckBox(this);
  useAsEventList=new QCheckBox(this);
  useLocalDir=new QCheckBox(this);
  localDir=new QLineEdit(this);
  useLocalDir->setChecked(false);
  localDir->setEnabled(useLocalDir->isChecked());
  QGridLayout *layout=new QGridLayout(this);
  layout->addWidget(searchForEvent,0,0);
  layout->addWidget(useAsEventList,1,0);
  layout->addWidget(useLocalDir,2,0);
  layout->addWidget(localDir,3,0);
  connect(useAsEventList,SIGNAL(clicked()),this,SLOT(onUseEventListChange()));
  connect(useLocalDir,SIGNAL(clicked()),this,SLOT(onUseLocalChange()));
  connect(searchForEvent,SIGNAL(clicked()),this,SLOT(onSearchChange()));
  connect(localDir,SIGNAL(textChanged(const QString&)),this,SLOT(onLocalChange()));
  resize(width(),height());
  languageChange();
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

void QAHitTreeInputWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption(tr("AHitTreeInput"));
#else
    setWindowTitle(tr("AHitTreeInput"));
#endif
  searchForEvent->setText(tr("search for given event"));
  useAsEventList->setText(tr("use entries as event input list"));
  useLocalDir->setText(tr("use local directory"));
}
QAHitTreeInputWidget::~QAHitTreeInputWidget()
{
}
void QAHitTreeInputWidget::onUseLocalChange()
{
  localDir->setEnabled(useLocalDir->isChecked());
  ap.changeParam<bool>("Use local directory",useLocalDir->isChecked());
}
void QAHitTreeInputWidget::onLocalChange()
{
  ap.changeParam<string>("local directory",QS2s(localDir->text()));
}
void QAHitTreeInputWidget::onUseEventListChange()
{
  ap.changeParam<bool>("Use as event input list",useAsEventList->isChecked());
}
void QAHitTreeInputWidget::onSearchChange()
{
  ap.changeParam<bool>("search for event",searchForEvent->isChecked());
}
