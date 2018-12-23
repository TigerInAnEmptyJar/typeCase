#include "qalgorithmdefinewidget.h"
#include <qpushbutton.h>
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif

#if QT_VERSION < 0x040000
QAlgorithmDefineWidget::QAlgorithmDefineWidget( QWidget* parent, const char* name, WFlags fl): QWidget( parent, name, fl ), ID(0)
#else
QAlgorithmDefineWidget::QAlgorithmDefineWidget( QWidget* parent,Qt::WindowFlags f): QWidget( parent, f )
#endif
{
  insertButton=new QPushButton(this);
  insertButton->setText("insert");
  insertButton->resize(80,30);
  connect(insertButton,SIGNAL(clicked()),this,SLOT(OnInsertButtonClick()));
  ap.setCategory(10);
}

QAlgorithmDefineWidget::~QAlgorithmDefineWidget()
{
  disconnect(insertButton);
  delete insertButton;
}

void QAlgorithmDefineWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  insertButton->setGeometry((w-80)/2,h-21,80,20);
}

void QAlgorithmDefineWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}

void QAlgorithmDefineWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAlgorithmDefineWidget::setRuns(vector<run_parameter*> *selectedRuns)
{
}
void QAlgorithmDefineWidget::OnInsertButtonClick()
{
  algorithm_parameter *a=new algorithm_parameter(ap);
  emit insertClick(a, isInput);
}
int QAlgorithmDefineWidget::getID()const
{
  return ID;
}
void QAlgorithmDefineWidget::setID(int v){ID=v;ap.setID(ID);}
