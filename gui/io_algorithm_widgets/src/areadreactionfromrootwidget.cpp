#include "areadreactionfromrootwidget.h"
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
#include "utilities.h"

#if QT_VERSION < 0x040000
QAReadReactionFromRootWidget::QAReadReactionFromRootWidget( QWidget* parent, const char* name, WFlags fl ):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAReadReactionFromRootWidget");
#else
QAReadReactionFromRootWidget::QAReadReactionFromRootWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAReadReactionFromRootWidget");
#endif
  ID=46;
  isInput=true;
  ap.setName("Read Reaction from simple Tree");
  ap.setID(ID);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("read Hits",false));
  ap.addParam<bool>(single_parameter<bool>("read Pixel",false));
  ap.addParam<bool>(single_parameter<bool>("read Cluster",false));
  ap.addParam<bool>(single_parameter<bool>("read Tracks",true));
  ap.addParam<bool>(single_parameter<bool>("reconstruct hits",false));
  ap.addParam<bool>(single_parameter<bool>("read minimum",false));
  vector<string> tmp;tmp.push_back("ppelastic");
  ap.addParam<vector<string> >(single_parameter<vector<string> >("reaction names",tmp));
#if QT_VERSION < 0x040000
  reactionsBox=new QListBox(this);
  reactionsBox->insertItem("ppelastic");
  connect(reactionsBox,SIGNAL(mouseButtonClicked(int, QListBoxItem*, const QPoint&)),this,SLOT(OnReactionClick(int, QListBoxItem*)));
#else
  reactionsBox=new QListWidget(this);
  reactionsBox->addItem("ppelastic");
  reactionsBox->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(reactionsBox,SIGNAL(customContextMenuRequested (const QPoint &)),
	  this,SLOT(OnReactionClick(const QPoint &)));
  connect(reactionsBox,SIGNAL(itemClicked(QListWidgetItem*)),
	  this,SLOT(OnReactionClick(QListWidgetItem*)));
#endif
  doCluster=new QCheckBox(this);
  connect(doCluster,SIGNAL(clicked()),this,SLOT(OnDoClusterClick()));
  doTrack=new QCheckBox(this);
  connect(doTrack,SIGNAL(clicked()),this,SLOT(OnDoTrackClick()));
  doPixel=new QCheckBox(this);
  connect(doPixel,SIGNAL(clicked()),this,SLOT(OnDoPixelClick()));
  doHits=new QCheckBox(this);
  connect(doHits,SIGNAL(clicked()),this,SLOT(OnDoHitsClick()));
  doTrack->setChecked(true);
  reconstructHits=new QCheckBox(this);
  connect(reconstructHits,SIGNAL(clicked()),this,SLOT(OnReconstructClick()));
  reconstructHits->setEnabled(!doHits->isChecked());
  minimum=new QCheckBox(this);
  connect(minimum,SIGNAL(clicked()),this,SLOT(OnMinimumClicked()));
  label1=new QLabel(this);
  label1->setText("Reaction names");
  doTrack->setText("read Tracks");
  doCluster->setText("read Clusters");
  doPixel->setText("read Pixels");
  doHits->setText("read Hits");
  reconstructHits->setText("reconstruct hits\nfrom track data");
  minimum->setText("read only events where \nmaximal data is available");
  //  label1->setText("Name for Tree");
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

QAReadReactionFromRootWidget::~QAReadReactionFromRootWidget()
{
  disconnect(reactionsBox);
  disconnect(doCluster);
  disconnect(doTrack);
  disconnect(doPixel);
  disconnect(doHits);
  disconnect(reconstructHits);
  disconnect(minimum);
  delete reactionsBox;
  delete doCluster;
  delete doTrack;
  delete doPixel;
  delete doHits;
  delete reconstructHits;
  delete minimum;
  delete label1;
}
void QAReadReactionFromRootWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int cbHeight=20;
  int abt=5;
  int minHeight=60+cbHeight*8+abt*6;
  if(h>minHeight)
    abt=5+(h-minHeight)/6;

  label1->setGeometry(w/2+10,10,w/2-30,cbHeight);
  reactionsBox->setGeometry(w/2+10,10+cbHeight+abt,w/2-30,h-60-cbHeight-abt);
  doHits->setGeometry(10,10,w/2-20,cbHeight);
  doPixel->setGeometry(10,10+cbHeight+abt,w/2-20,cbHeight);
  doCluster->setGeometry(10,10+cbHeight*2+abt*2,w/2-20,cbHeight);
  doTrack->setGeometry(10,10+cbHeight*3+abt*3,w/2-20,cbHeight);
  reconstructHits->setGeometry(10,10+cbHeight*4+abt*4,w/2-20,cbHeight*2);
  minimum->setGeometry(10,10+cbHeight*6+abt*5,w/2-20,cbHeight*2);
}
void QAReadReactionFromRootWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAReadReactionFromRootWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
 // void setRuns(vector<run_parameter*> *selectedRuns);
 // void OnRunSelect(int num);
#if QT_VERSION < 0x040000
void QAReadReactionFromRootWidget::OnReactionClick(int button, QListBoxItem *it)
{
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  switch(button)
    {
    case 1://left
      {
	QString val=QStringInputDialog::getLine(this,"New reaction name","new reaction");
	if(val!="")
	  {
	    tmp.push_back(QS2s(val));
	    reactionsBox->insertItem(val);
	  }
	break;
      }
    case 2://right
      {
	if(it==NULL)return;
	int num=reactionsBox->index(it);
	reactionsBox->removeItem(num);
	takeItemFromVector<string>(tmp,tmp[num]);
	break;
      }
    }
  ap.changeParam<vector<string> >("reaction names",tmp);
}
#else
void QAReadReactionFromRootWidget::OnReactionClick(const QPoint &p)
{
  QListWidgetItem *it=reactionsBox->itemAt(p);
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  if(it==NULL)return;
  int num=reactionsBox->row(it);
  delete reactionsBox->takeItem(num);
  takeItemFromVector<string>(tmp,tmp[num]);
  ap.changeParam<vector<string> >("reaction names",tmp);
}
void QAReadReactionFromRootWidget::OnReactionClick(QListWidgetItem *it)
{
  vector<string> tmp=ap.getParam<vector<string> >(0).getData();
  QString val=QStringInputDialog::getLine(this,"New reaction name","new reaction");
  if(val!="")
    {
      tmp.push_back(QS2s(val));
      reactionsBox->addItem(val);
    }
  ap.changeParam<vector<string> >("reaction names",tmp);
}
#endif

void QAReadReactionFromRootWidget::OnDoHitsClick()
{
  ap.changeParam<bool>("read Hits",doHits->isChecked());
  reconstructHits->setEnabled(!doHits->isChecked());
}
void QAReadReactionFromRootWidget::OnDoPixelClick()
{
  ap.changeParam<bool>("read Pixel",doPixel->isChecked());
}
void QAReadReactionFromRootWidget::OnDoClusterClick()
{
  ap.changeParam<bool>("read Cluster",doCluster->isChecked());
}
void QAReadReactionFromRootWidget::OnDoTrackClick()
{
  ap.changeParam<bool>("read Tracks",doTrack->isChecked());
}
void QAReadReactionFromRootWidget::OnReconstructClick()
{
  ap.changeParam<bool>("reconstruct hits",reconstructHits->isChecked());
}
void QAReadReactionFromRootWidget::OnMinimumClicked()
{
  ap.changeParam<bool>("read minimum",minimum->isChecked());
}
