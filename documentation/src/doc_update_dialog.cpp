#include "doc_update_dialog.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include "tmyclass.h"
#include <qlayout.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#include <qlistview.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#endif

#if QT_VERSION < 0x040000
doc_update_dialog::doc_update_dialog(QWidget* parent, bool modal,WFlags fl):QDialog(parent,"update dialog",modal,fl)
{
  setName("update dialog");
#else
doc_update_dialog::doc_update_dialog(QWidget* parent, Qt::WindowFlags fl):QDialog(parent,fl)
{
  setObjectName("update dialog");
#endif
  oldClass=NULL;
  newClass=NULL;
  QWidget *privateWidget=new QWidget(this);
  QHBoxLayout *privateLayout=new QHBoxLayout(privateWidget);
  helpButton=new QPushButton(privateWidget);
  okButton=new QPushButton(privateWidget);
  cancelButton=new QPushButton(privateWidget);
  privateLayout->addWidget(helpButton);
  privateLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
  privateLayout->addWidget(okButton);
  privateLayout->addWidget(cancelButton);
  privateLayout->setSpacing(5);
  assignButton=new QPushButton(this);
  oldLabel=new QLabel(this);
  newLabel=new QLabel(this);
#if QT_VERSION < 0x040000
  oldBox=new QListBox(this);
  newBox=new QListBox(this);
  togetherView=new QListView(this);
  togetherView->addColumn("old");
  togetherView->addColumn("new");
#else
  oldBox=new QListWidget(this);
  newBox=new QListWidget(this);
  togetherView=new QTreeWidget(this);
#endif
  flayout=new QGridLayout(this);
  flayout->addWidget(oldLabel,0,0);
  flayout->addWidget(newLabel,0,1);
  flayout->addWidget(assignButton,2,0);
  flayout->addWidget(oldBox,1,0);
  flayout->addWidget(newBox,1,1);
#if QT_VERSION < 0x040000
  flayout->addMultiCellWidget(togetherView,3,3,0,1);
  flayout->addMultiCellWidget(privateWidget,4,4,0,1);
#else
  flayout->addWidget(togetherView,3,0,1,2);
  flayout->addWidget(privateWidget,4,0,1,2);
#endif
  flayout->setMargin(5);flayout->setSpacing(5);
  helpButton->setText("Help");
  okButton->setText("Ok");
  cancelButton->setText("Cancel");
  assignButton->setText("Assign");
  oldLabel->setText("old Class");
  newLabel->setText("new Class");
  resize( QSize(1011, 423).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
  setCaption("Update Class");
  clearWState( WState_Polished );
  connect(togetherView,SIGNAL(rightButtonClicked(QListViewItem *, const QPoint &, int)),this,SLOT(onTogeterClick(QListViewItem*)));
#else
  setWindowTitle("Update Class");
  setWindowState( Qt::WindowNoState);
  togetherView->setContextMenuPolicy( Qt::CustomContextMenu);
  connect(togetherView,SIGNAL( customContextMenuRequested ( const QPoint &)),this,SLOT(onTogeterClick(const QPoint&)));
#endif
  connect(okButton,SIGNAL(clicked()),this, SLOT(accept()));
  connect(cancelButton,SIGNAL(clicked()),this, SLOT(reject()));
  connect(assignButton,SIGNAL(clicked()),this,SLOT(onAssignClick()));
}
doc_update_dialog::~doc_update_dialog()
{
}
#if QT_VERSION < 0x040000
void doc_update_dialog::addProps2Box(QListBox* lb, TMyClass *cls)
#else
void doc_update_dialog::addProps2Box(QListWidget* lb, TMyClass *cls)
#endif
{
  if(cls==NULL)return;
  lb->clear();
  for(int j=0;j<cls->nConstructors();j++)
    lb->insertItem(cls->Constructor(j).toString().data());
  for(int i=0;i<3;i++)
    {
      for(int j=0;j<cls->nSlots(i);j++)
	lb->insertItem(cls->Slot(j,i).toString().data());
      for(int j=0;j<cls->nMethods(i);j++)
	lb->insertItem(cls->Method(j,i).toString().data());
      for(int j=0;j<cls->nVariables(i);j++)
	lb->insertItem(cls->Variable(j,i).toString().data());
    }
  for(int j=0;j<cls->nSignals();j++)
    lb->insertItem(cls->Signal(j).toString().data());
}
void doc_update_dialog::setOldClass(TMyClass* old)
{
  if(old==NULL)return;
  oldClass=old;
  oldLabel->setText((string("old class:")+oldClass->Name()).data());
  addProps2Box(oldBox,oldClass);
  checkBoxes();
}
void doc_update_dialog::setNewClass(TMyClass* nCl)
{
  if(nCl==NULL)return;
  newClass=nCl;
  newLabel->setText((string("new class:")+newClass->Name()).data());
  addProps2Box(newBox,newClass);
  checkBoxes();
}
void doc_update_dialog::checkBoxes()
{
  if(oldClass==NULL||newClass==NULL)return;
  togetherView->clear();
  for(unsigned int i=0;i<oldBox->count();i++)
    {
      int m=-1;
      for(unsigned int j=0;j<newBox->count();j++)
	if(newBox->text(j)==oldBox->text(i))m=j;
      if(m>=0)
	{
	  new QListViewItem(togetherView,oldBox->text(i),newBox->text(m));
	  oldBox->removeItem(i);
	  newBox->removeItem(m);
	  i--;
	  if(i==oldBox->count()-1)break;
	}
    }
}
nameTypeDoc* doc_update_dialog::getDoc(int tp,const QString &nme)
{
  TMyClass *cls=(tp==0?oldClass:newClass);
  if(cls==NULL)return NULL;
  for(int j=0;j<cls->nConstructors();j++)
    if(nme==cls->Constructor(j).toString().data())return &cls->Constructor(j);
  for(int i=0;i<3;i++)
    {
      for(int j=0;j<cls->nSlots(i);j++)
	if(nme==cls->Slot(j,i).toString().data())return &cls->Slot(j,i);
      for(int j=0;j<cls->nMethods(i);j++)
	if(nme==cls->Method(j,i).toString().data())return &cls->Method(j,i);
      for(int j=0;j<cls->nVariables(i);j++)
	if(nme==cls->Variable(j,i).toString().data())return &cls->Variable(j,i);
    }
  for(int j=0;j<cls->nSignals();j++)
    if(nme==cls->Signal(j).toString().data())return &cls->Signal(j);
  return NULL;
}
void doc_update_dialog::accept()
{
  QListViewItem *it=togetherView->firstChild();
  nameTypeDoc *ndoc,*odoc;
  int num=0;
  while(it!=NULL)
    {
      ndoc=getDoc(1,it->text(1));
      odoc=getDoc(0,it->text(0));
      if(ndoc!=NULL&&odoc!=NULL)
	{
	  num++;
	  //cout<<"assigning "<<*odoc<<"to"<<*ndoc<<endl;
	  ndoc->clearDescription();
	  for(int i=0;i<odoc->DescriptionLines();i++)ndoc->addDescriptionLine(odoc->Description(i));
	}
      it=it->nextSibling();
    }
  QMessageBox::information(this,"Property assignment",QString::number(num)+" properties assigned",QMessageBox::Ok);
  if(newBox->count()>0)QMessageBox::information(this,"Property assignment",QString::number(newBox->count())+" new properties unassigned",QMessageBox::Ok);
  if(oldBox->count()>0)QMessageBox::information(this,"Property assignment",QString::number(oldBox->count())+" old properties unassigned",QMessageBox::Ok);
  for(int i=0;i<oldClass->DescriptionLines();i++)
    newClass->addDescriptionLine(oldClass->Description(i));
  QDialog::accept();
}
void doc_update_dialog::onAssignClick()
{
  int n=oldBox->currentItem(),m=newBox->currentItem();
  if(n<0||m<0)return;
  new QListViewItem(togetherView,oldBox->text(n),newBox->text(m));
  oldBox->removeItem(n);
  newBox->removeItem(m);
}
#if QT_VERSION < 0x040000
void doc_update_dialog::onOldClick(QListBoxItem *it)
#else
void doc_update_dialog::onOldClick(QListWidgetItem *it)
#endif
{
}
#if QT_VERSION < 0x040000
void doc_update_dialog::onNewClick(QListBoxItem *it)
#else
void doc_update_dialog::onNewClick(QListWidgetItem *it)
#endif
{
}
#if QT_VERSION < 0x040000
void doc_update_dialog::onTogeterClick(QListViewItem* it)
{
  if(it==NULL)return;
  oldBox->insertItem(it->text(0));
  newBox->insertItem(it->text(1));
  togetherView->takeItem(it);
  delete it;
}
#else
void doc_update_dialog::onTogeterClick(const QPoint &pos)
{
}
#endif
bool doc_update_dialog::classesMatch()
{
  if(oldClass==NULL||newClass==NULL)return false;
  if(oldBox->count()!=0||newBox->count()!=0)return false;
  accept();
  return true;
}
