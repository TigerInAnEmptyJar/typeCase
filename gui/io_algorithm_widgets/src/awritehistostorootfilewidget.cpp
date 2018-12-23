#include "awritehistostorootfilewidget.h"
#include <qlabel.h>
#include <qlineedit.h>
#include <qwidget.h>
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "utilities.h"
#if QT_VERSION < 0x040000
QAWriteHistosToRootFileWidget::QAWriteHistosToRootFileWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget(parent,name,fl)
{
#else
QAWriteHistosToRootFileWidget::QAWriteHistosToRootFileWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAWriteHistosToRootFileWidget");
#endif
  ID=25;
  isInput=false;
  ap.setName("advanced Histogramming");
  ap.setID(ID);
  ap.setUse(true);
  ap.setCategory(10);
  ap.addParam<string>(single_parameter<string>("output file","testFile.root"));
  ap.addParam<string>(single_parameter<string>("Directory","elastics"));
  ap.addParam<string>(single_parameter<string>("histo pattern","__pi__0_1_2_3_4_5___tr__1_2_3_v_k___re__ppel_pp-pkl-pkppi_"));
  filename=new QLineEdit(this);
  directory=new QLineEdit(this);
  pattern=new QLineEdit(this);
  connect(pattern,SIGNAL(textChanged ( const QString & )),this,SLOT(OnPatternChange()));
  connect(directory,SIGNAL(textChanged ( const QString & )),this,SLOT(OnDirectoryChange()));
  connect(filename,SIGNAL(textChanged ( const QString & )),this,SLOT(OnFileNameChange()));
  label1=new QLabel(this);
  label2=new QLabel(this);
  label3=new QLabel(this);
  filename->setText("testFile.root");
  directory->setText("elastics");
  pattern->setText("__pi__0_1_2_3_4_5___tr__1_2_3_v_k___re__ppel_pp-pkl-pkppi_");
  label1->setText("Output file name");
  label2->setText("Directory");
  label3->setText("Histogram pattern");
  resize(width(),height());
}
QAWriteHistosToRootFileWidget::~QAWriteHistosToRootFileWidget()
{
  disconnect(filename);
  disconnect(directory);
  disconnect(pattern);
  delete label1;
  delete label2;
  delete label3;
  delete filename;
  delete directory;
  delete pattern;
}
void QAWriteHistosToRootFileWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int ww=w-30;
  int hh=h-50;
  int lh=30;
  int th=25;
  int abf=5;
  int minheight=lh*3+abf*2+5;
  if(hh>minheight)
    abf=5+(hh-minheight)/12;
  label1->setGeometry(5,5,ww/2-10,th);
  label2->setGeometry(5,5+lh+abf,ww/2-10,th);
  label3->setGeometry(5,5+lh*2+abf*2,ww/2-10,th);
  filename->setGeometry(ww/2+10,5,ww/2-10,lh);
  directory->setGeometry(ww/2+10,5+lh+abf,ww/2-10,lh);
  pattern->setGeometry(ww/2+10,5+lh*2+abf*2,ww/2-10,lh);
}
void QAWriteHistosToRootFileWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAWriteHistosToRootFileWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAWriteHistosToRootFileWidget::OnFileNameChange()
{
  ap.changeParam<string>("output file",QS2s(filename->text()));
}
void QAWriteHistosToRootFileWidget::OnDirectoryChange()
{
  ap.changeParam<string>("Directory",QS2s(directory->text()));
}
void QAWriteHistosToRootFileWidget::OnPatternChange()
{
  ap.changeParam<string>("histo pattern",QS2s(pattern->text()));
}
