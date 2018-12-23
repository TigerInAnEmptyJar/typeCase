#include "amultipletreeinputwidget.h"
#include "qstringinputdialog.h"
#include "qcheckbox.h"
#include "qlabel.h"
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#endif

#if QT_VERSION < 0x040000
QAMultipleTreeInputWidget::QAMultipleTreeInputWidget( QWidget* parent, const char* name, WFlags fl ):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAReadFromSimpleTreeWidget");
#else
QAMultipleTreeInputWidget::QAMultipleTreeInputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAReadFromSimpleTreeWidget");
#endif

  ID=55;
  isInput=true;
  ap.setName("Read Tracks from multiple trees");
  ap.setID(ID);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("Search for read event",false));
  minimum=new QCheckBox(this);
  connect(minimum,SIGNAL(clicked()),this,SLOT(OnMinimumClicked()));
  minimum->setText("Search for read event");
  //  label1->setText("Name for Tree");
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

QAMultipleTreeInputWidget::~QAMultipleTreeInputWidget()
{
  disconnect(minimum);
  delete minimum;
}
void QAMultipleTreeInputWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  minimum->setGeometry(10,10,w-20,25);
}
void QAMultipleTreeInputWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAMultipleTreeInputWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAMultipleTreeInputWidget::OnMinimumClicked()
{
  ap.changeParam<bool>("Search for read event",minimum->isChecked());
}
