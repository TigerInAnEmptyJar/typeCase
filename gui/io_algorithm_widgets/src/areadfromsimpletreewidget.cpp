#include "areadfromsimpletreewidget.h"
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include "utilities.h"
#if QT_VERSION < 0x040000
#include <qlistview.h>
#else
#include <QResizeEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#endif
#include <qspinbox.h>
#if QT_VERSION < 0x040000
QAReadFromSimpleTreeWidget::QAReadFromSimpleTreeWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAReadFromSimpleTreeWidget");
#else
QAReadFromSimpleTreeWidget::QAReadFromSimpleTreeWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAReadFromSimpleTreeWidget");
#endif

  ID=40;
  isInput=true;
  ap.setName("Read from simple Tree");
  ap.setID(40);
  ap.setUse(true);
  ap.addParam<bool>(single_parameter<bool>("read Tracks",false));
  ap.addParam<bool>(single_parameter<bool>("read Cluster",false));
  ap.addParam<bool>(single_parameter<bool>("read Pixel",false));
  ap.addParam<bool>(single_parameter<bool>("read Hits",true));
  ap.addParam<bool>(single_parameter<bool>("reset unread data",false));
  ap.addParam<bool>(single_parameter<bool>("shift start event",false));
  ap.addParam<bool>(single_parameter<bool>("copy data to local directory",false));
  ap.addParam<int>(single_parameter<int>("starting event",1));
  ap.addParam<string>(single_parameter<string>("Filename",""));
  ap.addParam<string>(single_parameter<string>("Treename",""));
  ap.addParam<string>(single_parameter<string>("Filename for Tracks",""));
  ap.addParam<string>(single_parameter<string>("Filename for Clusters",""));
  ap.addParam<string>(single_parameter<string>("Filename for Pixels",""));
  ap.addParam<string>(single_parameter<string>("Filename for Hits",""));
  ap.addParam<string>(single_parameter<string>("local directory","/local/scratch0/pion/ehrhardt/tmpData"));
  runBox=new QComboBox(this);
  connect(runBox,SIGNAL(highlighted(int)),this,SLOT(OnRunSelect(int)));
//   fileBox=new QListView(this);
//   fileBox->addColumn("File");
//   fileBox->addColumn("Type");
//   connect(fileBox,SIGNAL(clicked(QListViewItem*)),this,SLOT(OnFileSelect(QListViewItem*)));
  trackFile=new QLineEdit(this);
  clusterFile=new QLineEdit(this);
  pixelFile=new QLineEdit(this);
  hitFile=new QLineEdit(this);
  localDir=new QLineEdit(this);
  doCluster=new QCheckBox(this);
  connect(doCluster,SIGNAL(clicked()),this,SLOT(OnDoClusterClick()));
  UseLocal=new QCheckBox(this);
  UseLocal->setChecked(false);
  UseLocal->setText("use local directory");
  connect(UseLocal,SIGNAL(clicked()),this,SLOT(OnUseLocalClick()));
  doTrack=new QCheckBox(this);
  connect(doTrack,SIGNAL(clicked()),this,SLOT(OnDoTrackClick()));
  doPixel=new QCheckBox(this);
  connect(doPixel,SIGNAL(clicked()),this,SLOT(OnDoPixelClick()));
  doHits=new QCheckBox(this);
  connect(doHits,SIGNAL(clicked()),this,SLOT(OnDoHitsClick()));
  doHits->setChecked(true);
  doShift=new QCheckBox(this);
  connect(doShift,SIGNAL(clicked()),this,SLOT(OnDoShiftClick()));
  shiftNumber=new QSpinBox(this);
  shiftNumber->setRange(0,1000000000);
  connect(shiftNumber,SIGNAL(valueChanged(int)),this,SLOT(OnShiftValueChanged(int)));
  trackFile->setReadOnly(true);
  hitFile->setReadOnly(true);
  pixelFile->setReadOnly(true);
  clusterFile->setReadOnly(true);
  shiftNumber->setEnabled(doShift->isChecked());
  localDir->setEnabled(false);
  label1=new QLabel(this);
  Treename=new QLineEdit(this);
  resetUn=new QCheckBox(this);
  connect(resetUn,SIGNAL(clicked()),this,SLOT(OnResetClick()));
  connect(hitFile,SIGNAL(textChanged(const QString &)),this,SLOT(OnHitFileChange()));
  connect(pixelFile,SIGNAL(textChanged(const QString &)),this,SLOT(OnPixelFileChange()));
  connect(clusterFile,SIGNAL(textChanged(const QString &)),this,SLOT(OnClusterFileChange()));
  connect(trackFile,SIGNAL(textChanged(const QString &)),this,SLOT(OnTrackFileChange()));
  connect(Treename,SIGNAL(textChanged(const QString &)),this,SLOT(OnTreeNameChange()));
  connect(localDir,SIGNAL(textChanged(const QString &)),this,SLOT(OnLocalDirectoryChange()));
  acceptedFileTypes.push_back(4+2);
  acceptedFileTypes.push_back(8+2);
  acceptedFileTypes.push_back(16+2);
  acceptedFileTypes.push_back(32+2);
  doTrack->setText("read Tracks");
  doCluster->setText("read Clusters");
  doPixel->setText("read Pixels");
  doHits->setText("read Hits");
  doShift->setText("shift starting \nEvent");
  resetUn->setText("reset unread Data");
  label1->setText("Name for Tree");
  resize(width(),height());
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

QAReadFromSimpleTreeWidget::~QAReadFromSimpleTreeWidget()
{
  disconnect(runBox);
  //  disconnect(fileBox);
  disconnect(trackFile);
  disconnect(clusterFile);
  disconnect(pixelFile);
  disconnect(hitFile);
  disconnect(doTrack);
  disconnect(doCluster);
  disconnect(doPixel);
  disconnect(doHits);
  disconnect(doShift);
  disconnect(shiftNumber);
  disconnect(label1);
  disconnect(Treename);
  disconnect(resetUn);
  disconnect(UseLocal);
  disconnect(localDir);
  delete runBox;
  //  delete fileBox;
  delete trackFile;
  delete clusterFile;
  delete pixelFile;
  delete hitFile;
  delete doTrack;
  delete doCluster;
  delete doPixel;
  delete doHits;
  delete doShift;
  delete shiftNumber;
  delete label1;
  delete Treename;
  delete resetUn;
  delete localDir;
  delete UseLocal;
}
void QAReadFromSimpleTreeWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  int cbHeight=25;
  int leHeight=25;
  int cbwidth=120;
  int abt=2;
  int minHeight=50+cbHeight*9+abt*10;
  if(h>minHeight)
    abt=2+(h-minHeight)/10;
  runBox->setGeometry(     10, abt,w-20,leHeight);
//   fileBox->setGeometry(    w/2+10, 10+leHeight+abt,w/2-30,h-abt-leHeight-60);
  doHits->setGeometry(     10,     abt*2+cbHeight,cbwidth,cbHeight);
  hitFile->setGeometry(    20+cbwidth,  abt*2+cbHeight,w-30-cbwidth,leHeight);
  doPixel->setGeometry(    10,     abt*3+cbHeight*2,cbwidth,cbHeight);
  pixelFile->setGeometry(  20+cbwidth,abt*3+cbHeight*2,w-30-cbwidth,leHeight);
  doCluster->setGeometry(  10,     abt*4+cbHeight*3,cbwidth,cbHeight);
  clusterFile->setGeometry(20+cbwidth,abt*4+cbHeight*3,w-30-cbwidth,leHeight);
  doTrack->setGeometry(    10,     abt*5+cbHeight*4,cbwidth,cbHeight);
  trackFile->setGeometry(  20+cbwidth,abt*5+cbHeight*4,w-30-cbwidth,leHeight);
  doShift->setGeometry(    10,     abt*6+cbHeight*5,100,leHeight);
  shiftNumber->setGeometry(115,    abt*6+cbHeight*5,w-30-cbwidth,leHeight);
  resetUn->setGeometry(    10,     abt*7+cbHeight*6,cbwidth,cbHeight);
  label1->setGeometry(     10,     abt*8+cbHeight*7,cbwidth,cbHeight);
  Treename->setGeometry(   20+cbwidth, abt*8+cbHeight*7,w-30-cbwidth,leHeight);
  UseLocal->setGeometry(   10,     abt*9+cbHeight*8,cbwidth,cbHeight);
  localDir->setGeometry(   20+cbwidth,abt*9+cbHeight*8,w-30-cbwidth,cbHeight);
}
void QAReadFromSimpleTreeWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}
void QAReadFromSimpleTreeWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}
void QAReadFromSimpleTreeWidget::setRuns(vector<run_parameter*> *selectedRuns)
{
  selectedRun=selectedRuns;
  runBox->clear();
  for(unsigned int i=0;i<selectedRuns->size();i++)
    {
#if QT_VERSION < 0x040000
      runBox->insertItem((*selectedRuns)[i]->getName().data());
#else
      runBox->addItem((*selectedRuns)[i]->getName().data());
#endif
      vector<string>st;
      vector<int>it;
      for(int j=0;j<(*selectedRuns)[i]->getNumberOfFiles();j++)
	{
	  for(unsigned int k=0;k<acceptedFileTypes.size();k++)
	    if((*selectedRuns)[i]->getFileType(j)==acceptedFileTypes[k])
	      {
		st.push_back((*selectedRuns)[i]->getFile(j));
		it.push_back((*selectedRuns)[i]->getFileType(j));
	      }
	}
      fileNames.push_back(st);
      fileTypes.push_back(it);
    }
  if(fileNames.size()==0)return;
  setComboBoxCurrentItem(runBox,0);
  OnRunSelect(0);
}

void QAReadFromSimpleTreeWidget::OnRunSelect(int num)
{
  if(num<0)return;
//   fileBox->clear();
//   QListViewItem *it;
//   for(unsigned int i=0;i<fileNames[num].size();i++)
//     it=new QListViewItem(fileBox,fileNames[num][i],QString::number(fileTypes[num][i]));
  for(unsigned int i=0;i<fileNames[num].size();i++)
    {
      switch(fileTypes[num][i])
	{
	case 6:
	  hitFile->setText(fileNames[num][i].data());
	  break;
	case 10:
	  pixelFile->setText(fileNames[num][i].data());
	  break;
	case 18:
	  clusterFile->setText(fileNames[num][i].data());
	  break;
	case 34:
	  trackFile->setText(fileNames[num][i].data());
	  break;
	}
    }
  OnHitFileChange();
  OnPixelFileChange();
  OnTrackFileChange();
  OnClusterFileChange();
}
// #if QT_VERSION < 0x040000
// void QAReadFromSimpleTreeWidget::OnFileSelect(QListViewItem *it)
// #else
// void QAReadFromSimpleTreeWidget::OnFileSelect(QTreeWidgetItem *it)
// #endif
// {
//   if(it==NULL)return;
//   bool cond;
//   QLineEdit *tm=NULL;
//   QString n;
//   for(unsigned int i=0;i<acceptedFileTypes.size();i++)
//     {
//       cond=false;
//       switch(i)
// 	{
// 	case 0:cond=doHits->isChecked();tm=hitFile;n="Filename for Hits";break;
// 	case 1:cond=doPixel->isChecked();tm=pixelFile;n="Filename for Pixels";break;
// 	case 2:cond=doCluster->isChecked();tm=clusterFile;n="Filename for Clusters";break;
// 	case 3:cond=doTrack->isChecked();tm=trackFile;n="Filename for Tracks";break;
// 	}
//       if(!cond)continue;
//       if(it->text(1).toInt()==acceptedFileTypes[i])
// 	{
// 	  tm->setText(it->text(0));
// 	  ap.changeParam<string>(n,it->text(0).ascii());
// 	  return;
// 	}
//     }
// }
void QAReadFromSimpleTreeWidget::OnDoHitsClick()
{
  ap.changeParam<bool>("read Hits",doHits->isChecked());
  //  hitFile->setEnabled(doHits->isChecked());
}
void QAReadFromSimpleTreeWidget::OnDoPixelClick()
{
  ap.changeParam<bool>("read Pixel",doPixel->isChecked());
  //  pixelFile->setEnabled(doPixel->isChecked());
}
void QAReadFromSimpleTreeWidget::OnDoClusterClick()
{
  ap.changeParam<bool>("read Cluster",doCluster->isChecked());
  //  clusterFile->setEnabled(doCluster->isChecked());
}
void QAReadFromSimpleTreeWidget::OnDoTrackClick()
{
  ap.changeParam<bool>("read Tracks",doTrack->isChecked());
  //  trackFile->setEnabled(doTrack->isChecked());
}
void QAReadFromSimpleTreeWidget::OnDoShiftClick()
{
  ap.changeParam<bool>("shift starting event",doShift->isChecked());
  shiftNumber->setEnabled(doShift->isChecked());
}
void QAReadFromSimpleTreeWidget::OnShiftValueChanged(int num)
{
  ap.changeParam<int>("starting event",shiftNumber->value());
}
void QAReadFromSimpleTreeWidget::OnResetClick()
{
  ap.changeParam<bool>("reset unread data",resetUn->isChecked());
}
void QAReadFromSimpleTreeWidget::OnHitFileChange()
{
  ap.changeParam<string>("Filename for Hits",QS2s(hitFile->text()),5);
}
void QAReadFromSimpleTreeWidget::OnPixelFileChange()
{
  ap.changeParam<string>("Filename for Pixels",QS2s(pixelFile->text()),4);
}
void QAReadFromSimpleTreeWidget::OnTrackFileChange()
{
  ap.changeParam<string>("Filename for Tracks",QS2s(trackFile->text()),2);
}
void QAReadFromSimpleTreeWidget::OnClusterFileChange()
{
  ap.changeParam<string>("Filename for Clusters",QS2s(clusterFile->text()),3);
}
void QAReadFromSimpleTreeWidget::OnTreeNameChange()
{
  ap.changeParam<string>("Treename",QS2s(Treename->text()));
}
void QAReadFromSimpleTreeWidget::OnLocalDirectoryChange()
{
  ap.changeParam<string>("local directory",QS2s(localDir->text()));
}
void QAReadFromSimpleTreeWidget::OnUseLocalClick()
{
  ap.changeParam<bool>("copy data to local directory",UseLocal->isChecked());
  localDir->setEnabled(UseLocal->isChecked());
}
