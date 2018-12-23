#include "areadfromtadewidget.h"
#include <qcombobox.h>
#if QT_VERSION < 0x040000
#include <qlistview.h>
#else
#include <QResizeEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#endif
#include "utilities.h"

#if QT_VERSION < 0x040000
QAReadFromTadeWidget::QAReadFromTadeWidget( QWidget* parent, const char* name, WFlags fl):QAlgorithmDefineWidget( parent, name, fl )
{
  if(name==NULL) setName("QAReadFromTadeWidget");
#else
QAReadFromTadeWidget::QAReadFromTadeWidget( QWidget* parent,Qt::WindowFlags f): QAlgorithmDefineWidget( parent, f)
{
  setObjectName("QAReadFromTadeWidget");
#endif
  ID=9;
  isInput=true;
  ap.setName("Read from Tade");
  ap.setID(9);
  ap.setUse(true);
  ap.addParam<string>(single_parameter<string>("filename",""));
  runBox=new QComboBox(this);
#if QT_VERSION < 0x040000
  fileBox=new QListView(this);
  fileBox->addColumn("File");
  fileBox->addColumn("Type");
  connect(fileBox,SIGNAL(selectionChanged(QListViewItem*)),this,SLOT(OnFileSelect(QListViewItem*)));
#else
  fileBox=new QTreeWidget(this);
  fileBox->setHeaderLabels(QStringList()<<"File"<<"Type");
  connect(fileBox,SIGNAL(itemSelectionChanged()),this,SLOT(OnFileSelect()));
#endif
  connect(runBox,SIGNAL(highlighted(int)),this,SLOT(OnRunSelect(int)));
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

QAReadFromTadeWidget::~QAReadFromTadeWidget()
{
  disconnect(runBox);
  disconnect(fileBox);
  delete runBox;
  delete fileBox;
}

void QAReadFromTadeWidget::resize(int w, int h)
{
  QAlgorithmDefineWidget::resize(w,h);
  fileBox->setGeometry(10,45,(int)((w-20)*0.8),(int)((h-90)*0.8));
  runBox->setGeometry(10,10,(int)((w-20)*0.8),30);
}

void QAReadFromTadeWidget::resize(const QSize &s)
{
  resize(s.width(),s.height());
}

void QAReadFromTadeWidget::resizeEvent(QResizeEvent *e)
{
  resize(e->size().width(),e->size().height());
}

void QAReadFromTadeWidget::setRuns(vector<run_parameter*> *selectedRuns)
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
	  if((*selectedRuns)[i]->getFileType(j)==0)
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
void QAReadFromTadeWidget::OnRunSelect(int num)
{
  if(num<0)return;
  fileBox->clear();
#if QT_VERSION < 0x040000
  QListViewItem *it;
#else
  QTreeWidgetItem *it;
#endif
  for(unsigned int i=0;i<fileNames[num].size();i++)
#if QT_VERSION < 0x040000
    it=new QListViewItem(fileBox,fileNames[num][i],QString::number(fileTypes[num][i]));
#else
  it=new QTreeWidgetItem(fileBox,QStringList()<<fileNames[num][i].data()<<QString::number(fileTypes[num][i]));
#endif
}
#if QT_VERSION < 0x040000
void QAReadFromTadeWidget::OnFileSelect(QListViewItem *it)
{
#else
void QAReadFromTadeWidget::OnFileSelect()
{
  QTreeWidgetItem* it=fileBox->selectedItems().first();
#endif
  if(it==NULL)return;
  ap.changeParam<string>("filename",QS2s(it->text(0)));
}
