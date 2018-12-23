#include "ahittreeoutputwidget.h"
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
QAHitTreeOutputWidget::QAHitTreeOutputWidget( QWidget* parent, const char* name, WFlags fl ):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAHitTreeOutputWidget");
#else
QAHitTreeOutputWidget::QAHitTreeOutputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAHitTreeOutputWidget");
#endif
  ID=60;
  isInput=false;
  ap.setName("Read hits from hit tree");
  ap.setID(ID);
  ap.setUse(true);
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

QAHitTreeOutputWidget::~QAHitTreeOutputWidget()
{
  disconnect(minimum);
  delete minimum;
}
void QAHitTreeOutputWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
}
void QAHitTreeOutputWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAHitTreeOutputWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}

