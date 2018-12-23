#include "amultipletreeoutputwidget.h"
#include <qspinbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtooltip.h>
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "utilities.h"
#if QT_VERSION < 0x040000
QAMultipleTreeOutputWidget::QAMultipleTreeOutputWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget(parent,name,fl)
{
#else
QAMultipleTreeOutputWidget::QAMultipleTreeOutputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAMultipleTreeOutput");
#endif
  ID=54;
  isInput=false;
  ap.setName("Write to Simple Multiple Tree");
  ap.setID(ID);
  ap.setUse(true);
  ap.setCategory(10);
  ap.addParam<bool>(single_parameter<bool>("begin new file at end of run",true));
  ap.addParam<bool>(single_parameter<bool>("use local directory",false));
  ap.addParam<int>(single_parameter<int>("Number of tracks per tree",5));
  ap.addParam<string>(single_parameter<string>("File-pattern",""));
  ap.addParam<string>(single_parameter<string>("local directory",""));
  beginNew=new QCheckBox(this);
  beginNew->setText("begin new file for each run");
  beginNew->setChecked(false);
  connect(beginNew,SIGNAL(clicked()),this,SLOT(OnBeginNewClick()));
  filename=new QLineEdit(this);
  connect(filename,SIGNAL(textChanged ( const QString & )),this,SLOT(OnFileNameChange()));
  label1=new QLabel(this);
  label1->setText("output name scheme");
  label2=new QLabel(this);
  label2->setText("number of tracks per tree");
  nTracksPer=new QSpinBox(this);
  nTracksPer->setRange(1,20);
  nTracksPer->setValue(5);
  connect(nTracksPer,SIGNAL(valueChanged()),this,SLOT(OnNTracksChange()));
  UseLocal=new QCheckBox(this);
  UseLocal->setText("Use local directory");
  UseLocal->setChecked(false);
  connect(UseLocal,SIGNAL(clicked()),this,SLOT(OnUseLocalClick()));
  LocalDir=new QLineEdit(this);
  LocalDir->setEnabled(false);
  connect(LocalDir,SIGNAL(textChanged(const QString &)),this,SLOT(OnLocalDirChange()));
#if QT_VERSION < 0x040000
  QToolTip::add(filename,"File-path-pattern: algorithm will add \"T.root\" \n(or \"runNameT.root\", if begin-new-file option is activated)");
#else
  filename->setToolTip("File-path-pattern: algorithm will add \"T.root\" \n(or \"runNameT.root\", if begin-new-file option is activated)");
#endif
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}
QAMultipleTreeOutputWidget::~QAMultipleTreeOutputWidget()
{
#if QT_VERSION < 0x040000
  QToolTip::remove(filename);
#endif
  disconnect(beginNew);
  disconnect(filename);
  disconnect(UseLocal);
  disconnect(LocalDir);
  disconnect(nTracksPer);
  delete beginNew;
  delete filename;
  delete label1;
  delete label2;
  delete UseLocal;
  delete LocalDir;
  delete nTracksPer;
}
void QAMultipleTreeOutputWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAMultipleTreeOutputWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAMultipleTreeOutputWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int ch=25,lh=20,abf=5, hh=h-50,ww=w-10;
  int minheight=ch*5+abf*6;
  if(hh>minheight)
    abf=abf+(hh-minheight)/6;
  beginNew->setGeometry(5,abf,ww-10,ch);
  label1->setGeometry(5,abf*2+ch,ww-135,lh);
  filename->setGeometry(ww-125,abf*2+ch,120,ch);
  label2->setGeometry(5,abf*3+ch*2,ww-135,lh);
  nTracksPer->setGeometry(ww-125,abf*3+ch*2,120,ch);
  UseLocal->setGeometry(5,abf*4+ch*3,ww-10,ch);
  LocalDir->setGeometry(10,abf*5+ch*4,ww-20,ch);
}
void QAMultipleTreeOutputWidget::OnBeginNewClick()
{
  ap.changeParam<bool>("begin new file at end of run",beginNew->isChecked());
}
void QAMultipleTreeOutputWidget::OnNTracksChange()
{
  ap.changeParam<int>("Number of tracks per tree",nTracksPer->value());
}
void QAMultipleTreeOutputWidget::OnFileNameChange()
{
  ap.changeParam<string>("File-pattern",QS2s(filename->text()));
}
void QAMultipleTreeOutputWidget::OnUseLocalClick()
{
  ap.changeParam<bool>("use local directory",UseLocal->isChecked());
  LocalDir->setEnabled(UseLocal->isChecked());
}
void QAMultipleTreeOutputWidget::OnLocalDirChange()
{
  ap.changeParam<string>("local directory",QS2s(LocalDir->text()));
}
