#include "agenerateoutputwidget.h"
#include <qcheckbox.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#endif
#include <qlabel.h>
#include <qlineedit.h>
#include <qwidget.h>
#include "utilities.h"
#if QT_VERSION < 0x040000
QAGenerateOutputWidget::QAGenerateOutputWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget(parent,name,fl)
{
#else
QAGenerateOutputWidget::QAGenerateOutputWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("Generate output widget");
#endif
  ID=34;
  isInput=false;
  ap.setName("Write to Simple Tree");
  ap.setID(ID);
  ap.setUse(true);
  ap.setCategory(10);
  ap.addParam<bool>(single_parameter<bool>("use mc file",false));
  ap.addParam<bool>(single_parameter<bool>("write track-data",true));
  ap.addParam<bool>(single_parameter<bool>("write cluster-data",false));
  ap.addParam<bool>(single_parameter<bool>("write pixel-data",false));
  ap.addParam<bool>(single_parameter<bool>("write hit-data",false));
  ap.addParam<bool>(single_parameter<bool>("write reactions",true));
  ap.addParam<bool>(single_parameter<bool>("add file to run",true));
  ap.addParam<bool>(single_parameter<bool>("exchange file in run",false));
  ap.addParam<bool>(single_parameter<bool>("use local directory",false));
  ap.addParam<string>(single_parameter<string>("MC Track file",""));
  ap.addParam<string>(single_parameter<string>("Output file",""));
  ap.addParam<string>(single_parameter<string>("local directory",""));
  vector<string>tmp;
  tmp.push_back("ppelastic");
  tmp.push_back("dpi+");
  tmp.push_back("pp-pkl-pkppi");
//   tmp.push_back("pp-pk0S+-ppipippi");
//   tmp.push_back("pp-pk0S+-ppipinpi");
  ap.addParam<vector<string> >(single_parameter<vector<string> >("reactions",tmp));
  doHits=new QCheckBox(this);
  doHits->setText("write Hits");
  doHits->setChecked(false);
  connect(doHits,SIGNAL(clicked()),this,SLOT(OnDoHitsClick()));
  doPixels=new QCheckBox(this);
  doPixels->setText("write Pixel");
  doPixels->setChecked(false);
  connect(doPixels,SIGNAL(clicked()),this,SLOT(OnDoPixelsClick()));
  doClusters=new QCheckBox(this);
  doClusters->setText("write Cluster");
  doClusters->setChecked(false);
  connect(doClusters,SIGNAL(clicked()),this,SLOT(OnDoClustersClick()));
  doTracks=new QCheckBox(this);
  doTracks->setText("write Tracks");
  doTracks->setChecked(true);
  connect(doTracks,SIGNAL(clicked()),this,SLOT(OnDoTracksClick()));
  doReactions=new QCheckBox(this);
  doReactions->setText("write Reaction");
  doReactions->setChecked(true);
  connect(doReactions,SIGNAL(clicked()),this,SLOT(OnDoReactionsClick()));
#if QT_VERSION < 0x040000
  reactNames=new QListBox(this);
  for(unsigned int i=0;i<tmp.size();i++)reactNames->insertItem(tmp[i].data());
  connect(reactNames,SIGNAL(mouseButtonClicked(int,QListBoxItem*,const QPoint &)),this,SLOT(OnReactNamesClick(int,QListBoxItem*)));
#else
  reactNames=new QListWidget(this);
  for(unsigned int i=0;i<tmp.size();i++)reactNames->addItem(tmp[i].data());
  reactNames->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(reactNames,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnReactNamesClick(const QPoint&)));
  connect(reactNames,SIGNAL(itemClicked(QListWidgetItem*)),
	  this,SLOT(OnReactNamesClick(QListWidgetItem*)));
#endif
  reactNames->setEnabled(doReactions->isChecked());
  filename=new QLineEdit(this);
  connect(filename,SIGNAL(textChanged ( const QString & )),this,SLOT(OnFileNameChange()));
  label1=new QLabel(this);
  label1->setText("output name scheme");
  UseLocal=new QCheckBox(this);
  UseLocal->setText("Use local directory");
  UseLocal->setChecked(false);
  connect(UseLocal,SIGNAL(clicked()),this,SLOT(OnUseLocalClick()));
  LocalDir=new QLineEdit(this);
  LocalDir->setEnabled(false);
  connect(LocalDir,SIGNAL(textChanged(const QString &)),this,SLOT(OnLocalDirChange()));
}
QAGenerateOutputWidget::~QAGenerateOutputWidget()
{
  disconnect(doHits);
  disconnect(doPixels);
  disconnect(doClusters);
  disconnect(doTracks);
  disconnect(doReactions);
  disconnect(reactNames);
  disconnect(filename);
  disconnect(label1);
  disconnect(UseLocal);
  disconnect(LocalDir);
  delete doHits;
  delete doPixels;
  delete doClusters;
  delete doTracks;
  delete doReactions;
  delete reactNames;
  delete filename;
  delete label1;
  delete UseLocal;
  delete LocalDir;
}
void QAGenerateOutputWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int ch=25;
  int lh=30;
  int abf=5;
  int ww=w-10;
  int hh=h-50;
  int minheight=5+ch*5+abf*5+lh;
  int cbw=125;
  if(hh>minheight)
    abf=5+(hh-minheight)/12;
  doHits->setGeometry(5,5,cbw,ch);
  doPixels->setGeometry(5,5+ch+abf,cbw,ch);
  doClusters->setGeometry(5,5+ch*2+abf*2,cbw,ch);
  doTracks->setGeometry(5,5+ch*3+abf*3,cbw,ch);
  doReactions->setGeometry(ww/2+5,5,cbw,ch);
  reactNames->setGeometry(ww/2+5,5+ch+abf,ww/2-5-abf,hh-ch-5-abf);
  label1->setGeometry(5,5+ch*4+abf*4,cbw,ch);
  filename->setGeometry(5+abf+cbw,5+ch*4+abf*4,ww/2-5-abf*2-cbw,lh);
  UseLocal->setGeometry(5,5+ch*5+abf*5,cbw,ch);
  LocalDir->setGeometry(5+cbw+abf,5+ch*5+abf*5,ww/2-5-abf*2-cbw,lh);
}
void QAGenerateOutputWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAGenerateOutputWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAGenerateOutputWidget::OnDoHitsClick()
{
  ap.changeParam<bool>("write hit-data",doHits->isChecked());
}
void QAGenerateOutputWidget::OnDoPixelsClick()
{
  ap.changeParam<bool>("write pixel-data",doPixels->isChecked());
}
void QAGenerateOutputWidget::OnDoClustersClick()
{
  ap.changeParam<bool>("write cluster-data",doClusters->isChecked());
}
void QAGenerateOutputWidget::OnDoTracksClick()
{
  ap.changeParam<bool>("write track-data",doTracks->isChecked());
}
void QAGenerateOutputWidget::OnDoReactionsClick()
{
  ap.changeParam<bool>("write reactions",doReactions->isChecked());
  reactNames->setEnabled(doReactions->isChecked());
}
#include "qstringinputdialog.h"
#if QT_VERSION < 0x040000
void QAGenerateOutputWidget::OnReactNamesClick(int button, QListBoxItem *it)
{
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  switch(button)
    {
    case 1://left
      {
	QString val=QStringInputDialog::getLine(this,"Reaction");
	if(val!="")
	  {
	    tmp.push_back(QS2s(val));
	    reactNames->insertItem(val);
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	int num=reactNames->index(it);
	reactNames->removeItem(num);
	takeItemFromVector<string>(tmp,tmp[num]);
	break;
      }
    }
  ap.changeParam<vector<string> >("reactions",tmp);
}
#else
void QAGenerateOutputWidget::OnReactNamesClick(const QPoint & p)
{
  QListWidgetItem *it=reactNames->itemAt(p);
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  if(it==NULL)return;
  int num=reactNames->row(it);
  delete reactNames->takeItem(num);
  takeItemFromVector<string>(tmp,tmp[num]);
  ap.changeParam<vector<string> >("reactions",tmp);
}
void QAGenerateOutputWidget::OnReactNamesClick(QListWidgetItem *it)
{
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  QString val=QStringInputDialog::getLine(this,"Reaction");
  if(val!="")
    {
      tmp.push_back(QS2s(val));
      reactNames->addItem(val);
    }
  ap.changeParam<vector<string> >("reactions",tmp);
}
#endif
void QAGenerateOutputWidget::OnFileNameChange()
{
  ap.changeParam<string>("Output file",QS2s(filename->text()));
}
void QAGenerateOutputWidget::OnUseLocalClick()
{
  ap.changeParam<bool>("use local directory",UseLocal->isChecked());
  LocalDir->setEnabled(UseLocal->isChecked());
}
void QAGenerateOutputWidget::OnLocalDirChange()
{
  ap.changeParam<string>("local directory",QS2s(LocalDir->text()));
}
