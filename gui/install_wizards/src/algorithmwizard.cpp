#include "algorithmwizard.h"
#include <qwidget.h>
#include <gui/widgets/include/qmywizard.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include "gui/parameter_widgets/include/algorithmdisplay.h"
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qspinbox.h>
#if QT_VERSION < 0x040000
#include <qheader.h>
#include <qlistview.h>
#include <qlistbox.h>
#include <qpopupmenu.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#endif
#include <iostream>
#include <fstream>
#include "utilities.h"
#include "tofAnalysis_t.h"
extern bool existing(string filename);
#if QT_VERSION < 0x040000
AlgorithmWizard::AlgorithmWizard( QWidget* parent, const char* name, bool modal , WFlags fl )
:QMyWizard(parent,name,modal,fl)
{
  if(!name)setName("AlgorithmWizard");
#else
AlgorithmWizard::AlgorithmWizard( QWidget* parent, Qt::WindowFlags f )
:QMyWizard(parent,f)
{
  setObjectName("AlgorithmWizard");
#endif
  vector<TMyClass> tmpC;
  TMyClass::fileContent(1,string(getenv("KTOFPACK"))+"/analyser/include/tofAnalysis_t.h",tmpC);
  for(unsigned int i=0;i<tmpC.size();i++)
    if(tmpC[i].Name()=="tofAnalysis")
      anaClass=tmpC[i];
  wizardPages=new QWidget*[5];
  pageLayouts=new QGridLayout*[5];
    //first page
  labels=new QLabel*[15];
  for(int i=0;i<5;i++)
    {
      wizardPages[i]=new QWidget(this);
      pageLayouts[i]=new QGridLayout(wizardPages[i]);
    }
  labels[0]=new QLabel(wizardPages[0]);
  nameInput=new QLineEdit(wizardPages[0]);
  copyInput=new QCheckBox(wizardPages[0]);
  catBox=new QComboBox(wizardPages[0]);

  //installLog box
  QGroupBox* installLogBox=new QGroupBox(wizardPages[0]);
#if QT_VERSION < 0x040000
  installLogBox->addSpace(5);
#endif
  readButton=new QPushButton(installLogBox);
  createButton=new QPushButton(installLogBox);
  logLabel=new QLabel(installLogBox);
  logLabel->setText(tr("install-log file"));
  installLogBox->setTitle("Install log");

  labels[1]=new QLabel(wizardPages[0]);

  //IO box
  QGroupBox* ioLogBox=new QGroupBox(wizardPages[0]);
  ioLogBox->setTitle("IO-algorithm");
  ioCheckBox=new QCheckBox(ioLogBox);
  ioLabel=new QLabel(ioLogBox);
  ioSourceLabel=new QLabel(ioLogBox);
  ioBrowseButton=new QPushButton(ioLogBox);
  ioClassBox=new QComboBox(ioLogBox);
  ioSelectBox=new QComboBox(ioLogBox);
  ioLabel->setText(tr("io-header-file"));
  ioSourceLabel->setText(tr("io-source-file"));
#if QT_VERSION < 0x040000
  ioLogBox->setColumnLayout(1,Qt::Horizontal);
#else
  QGridLayout* ioLogLayout=new QGridLayout(ioLogBox);
  ioLogLayout->addWidget(ioBrowseButton,1,1);
  ioLogLayout->addWidget(ioSelectBox,1,0);
  ioLogLayout->addWidget(ioCheckBox,0,0,1,2);
  ioLogLayout->addWidget(ioClassBox,3,0,1,2);
  ioLogLayout->addWidget(ioLabel,2,0,1,2);
  ioLogLayout->addWidget(ioSourceLabel,4,0,1,2);
  ioLogBox->setLayout(ioLogLayout);
#endif

  labels[2]=new QLabel(wizardPages[0]);
  pageLayouts[0]->addWidget(nameInput,0,1);
  pageLayouts[0]->addWidget(copyInput,0,2);
  pageLayouts[0]->addWidget(labels[1],3,2);
  pageLayouts[0]->addWidget(catBox,4,2);
  pageLayouts[0]->addWidget(ioLogBox,5,2);

#if QT_VERSION < 0x040000
  definedBox=new QListBox(wizardPages[0]);
  pageLayouts[0]->addMultiCellWidget(definedBox,2,5,0,1);
  installLogBox->setColumnLayout(2,Qt::Horizontal);
  pageLayouts[0]->addMultiCellWidget(installLogBox,1,2,2,2);
  pageLayouts[0]->addMultiCellWidget(labels[2],1,1,0,1);
  pageLayouts[0]->addMultiCellWidget(labels[0],0,0,0,0);
  pageLayouts[0]->setColStretch(0,1);
  pageLayouts[0]->setColStretch(1,1);
  pageLayouts[0]->setColStretch(2,4);
#else
  definedBox=new QListWidget(wizardPages[0]);
  pageLayouts[0]->addWidget(installLogBox,1,2,2,1);
  QGridLayout *installLogLayout=new QGridLayout(installLogBox);
  installLogLayout->addWidget(readButton,1,0);
  installLogLayout->addWidget(createButton,1,1);
  installLogLayout->addWidget(logLabel,0,0,1,2);
  pageLayouts[0]->addWidget(labels[0],0,0,1,1);
  pageLayouts[0]->addWidget(definedBox,2,0,4,2);
  pageLayouts[0]->addWidget(labels[2],1,0,1,2);
  pageLayouts[0]->setColumnStretch(0,1);
  pageLayouts[0]->setColumnStretch(1,1);
  pageLayouts[0]->setColumnStretch(2,4);
#endif

  //second page
  adisplay=new AlgorithmDisplay(this);
  //third page
  headerInput=new QLineEdit(wizardPages[1]);
  headerBrowse=new QPushButton(wizardPages[1]);
  classesBox=new QComboBox(wizardPages[1]);
  copyLinkInput=new QComboBox(wizardPages[1]);
  classDefinitionShow=new QTextEdit(wizardPages[1]);
  for(int i=0;i<4;i++) labels[i+3]=new QLabel(wizardPages[1]);
  pageLayouts[1]->addWidget(labels[3],0,0);
  pageLayouts[1]->addWidget(labels[4],0,2);
  pageLayouts[1]->addWidget(labels[5],2,0);
  pageLayouts[1]->addWidget(copyLinkInput,4,2);
#if QT_VERSION < 0x040000
  sourceList=new QListBox(wizardPages[1]);
  pageLayouts[1]->addMultiCellWidget(classDefinitionShow,2,3,2,2);
  pageLayouts[1]->addMultiCellWidget(sourceList,3,3,0,1);
  pageLayouts[1]->addMultiCellWidget(headerInput,1,1,0,1);
  pageLayouts[1]->addMultiCellWidget(labels[6],4,4,0,1);
#else
  sourceList=new QListWidget(wizardPages[1]);
  pageLayouts[1]->addWidget(headerInput,1,0,1,2);
  pageLayouts[1]->addWidget(classDefinitionShow,2,2,2,1);
  pageLayouts[1]->addWidget(sourceList,3,0,1,2);
  pageLayouts[1]->addWidget(labels[6],4,0,1,2);
#endif
  pageLayouts[1]->addWidget(headerBrowse,0,1);
  pageLayouts[1]->addWidget(classesBox,1,2);

  //fourth page
  frequencyGroup=new QButtonGroup(wizardPages[2]);
  commentInput=new QLineEdit(wizardPages[2]);
  labels[14]=new QLabel(wizardPages[2]);
#if QT_VERSION > 0x040000
  QGroupBox *frequencyGBox=new QGroupBox(wizardPages[2]);
  QGridLayout* frequencyLayout=new QGridLayout(frequencyGBox);
  frequencyGBox->setTitle(tr("call frequency"));
#else
  frequencyGroup->setTitle(tr("call frequency"));
  frequencyGroup->setColumnLayout(2,Qt::Horizontal);
#endif
  frequencyGroup->setExclusive(true);
#if QT_VERSION < 0x040000
  QRadioButton *buttons[4];
  for(int i=0;i<3;i++)
  {  
    buttons[i]=new QRadioButton(frequencyGroup);
    frequencyGroup->addSpace(5);
  }
  buttons[3]=new QRadioButton(frequencyGroup);
  for(int i=0;i<4;i++)
    frequencyGroup->insert(buttons[i],i);
  frequencyBox=new QComboBox(frequencyGroup);
#else
  QRadioButton *buttons[4]={new QRadioButton(frequencyGBox),new QRadioButton(frequencyGBox),new QRadioButton(frequencyGBox),new QRadioButton(frequencyGBox)};
  for(int i=0;i<4;i++) {
    frequencyLayout->addWidget(buttons[i],i,0);
    frequencyGroup->addButton(buttons[i],i);}
  frequencyBox=new QComboBox(frequencyGBox);
  frequencyLayout->addWidget(frequencyBox,3,1);
#endif
  buttons[0]->setText(tr("once per Event"));
  buttons[1]->setText(tr("once per Track"));
  buttons[2]->setText(tr("once per Detector"));
  buttons[3]->setText(tr("once per Entry in"));
  fitBox=new QSpinBox(wizardPages[2]);
  for(int i=0;i<5;i++) labels[7+i]=new QLabel(wizardPages[2]);
#if QT_VERSION < 0x040000
  signalList=new QListBox(wizardPages[2]);
  slotList=new QListBox(wizardPages[2]);
  connectionList=new QListView(wizardPages[2]);
#else
  signalList=new QListWidget(wizardPages[2]);
  slotList=new QListWidget(wizardPages[2]);
  connectionList=new QTreeWidget(wizardPages[2]);
#endif
  pageLayouts[2]->addWidget(fitBox,3,1);
  pageLayouts[2]->addWidget(labels[7],3,0);
  pageLayouts[2]->addWidget(labels[8],3,2);
  pageLayouts[2]->addWidget(labels[10],1,3);
  pageLayouts[2]->addWidget(labels[11],1,4);
  pageLayouts[2]->addWidget(signalList,2,3);
  pageLayouts[2]->addWidget(slotList,2,4);
#if QT_VERSION < 0x040000
  pageLayouts[2]->addMultiCellWidget(labels[9],0,0,3,4);
  pageLayouts[2]->addMultiCellWidget(connectionList,3,6,3,4);
  pageLayouts[2]->addMultiCellWidget(frequencyGroup,0,2,0,2);
  pageLayouts[2]->addMultiCellWidget(commentInput,5,5,0,2);
  pageLayouts[2]->addMultiCellWidget(labels[14],4,4,0,2);
  connectionList->addColumn(tr("Sender"));
  connectionList->addColumn(tr("Signal"));
  connectionList->addColumn(tr("Receiver"));
  connectionList->addColumn(tr("Slot"));
#else
  pageLayouts[2]->addWidget(labels[9],0,3,1,2);
  pageLayouts[2]->addWidget(connectionList,3,3,4,2);
  pageLayouts[2]->addWidget(frequencyGBox,0,0,3,3);
  pageLayouts[2]->addWidget(commentInput,5,0,1,3);
  pageLayouts[2]->addWidget(labels[14],4,0,1,3);
  connectionList->setHeaderLabels(QStringList()<<tr("Sender")<<tr("Signal")<<tr("Receiver")<<tr("Slot"));
#endif

//     //fifth/last page

  constrInput=new QLineEdit(wizardPages[3]);
  takeButton=new QPushButton(wizardPages[3]);
  tpLabel=new QLabel(wizardPages[3]);
  tpLabel->setText(tr("int"));
  labels[12]=new QLabel(wizardPages[3]);
  constructorEdit=new QTextEdit(wizardPages[3]);
  constructorEdit->setReadOnly(true);
#if QT_VERSION < 0x040000
  variablesList=new QListView(wizardPages[3]);
  parameterList=new QListView(wizardPages[3]);
  parameterList->setSorting(-1);
  pageLayouts[3]->addMultiCellWidget(constructorEdit,0,0,0,3);
  pageLayouts[3]->addMultiCellWidget(parameterList,1,2,1,3);
  pageLayouts[3]->addMultiCellWidget(constrInput,4,4,0,1);
  parameterList->addColumn(tr("Parameter"));
  parameterList->addColumn(tr("type"));
  parameterList->addColumn(tr("Variable"));
  parameterList->addColumn(tr("type"));
  variablesList->addColumn(tr("Variable"));
  variablesList->addColumn(tr("type"));
  pageLayouts[3]->setColStretch(1,2);
  pageLayouts[3]->setColStretch(0,2);
#else
  variablesList=new QTreeWidget(wizardPages[3]);
  parameterList=new QTreeWidget(wizardPages[3]);
  pageLayouts[3]->addWidget(constructorEdit,0,0,1,4);
  pageLayouts[3]->addWidget(parameterList,1,1,2,3);
  pageLayouts[3]->addWidget(constrInput,4,0,1,2);
  parameterList->setHeaderLabels(QStringList()<<tr("Parameter")<<tr("type")<<tr("Variable")<<tr("type"));
  variablesList->setHeaderLabels(QStringList()<<tr("Variable")<<tr("type"));
  pageLayouts[3]->setColumnStretch(1,1);
#endif
  pageLayouts[3]->setRowStretch(2,2);
  pageLayouts[3]->addWidget(variablesList,2,0);
  pageLayouts[3]->addWidget(takeButton,4,3);
  pageLayouts[3]->addWidget(labels[12],1,0);
  pageLayouts[3]->addWidget(tpLabel,4,2);
  //finish page
  labels[13]=new QLabel(wizardPages[4]);
#if QT_VERSION < 0x040000
  pageLayouts[4]->expand(3,3);
#endif
  pageLayouts[4]->addWidget(labels[13],1,1);
  //add pages to wizard
    addPage(wizardPages[0],"General");
    addPage(adisplay,"Parameter");
    addPage(wizardPages[1],"Files");
    addPage(wizardPages[2],"Connections");
    addPage(wizardPages[3],"Constructor");
    addPage(wizardPages[4],"Finish");
    for(int i=0;i<4;i++)
      {
	pageLayouts[i]->setMargin(5);
	pageLayouts[i]->setSpacing(5);
      }
    showPage(wizardPages[0]);
    /****connections**/

    connect(readButton,SIGNAL(clicked()),this,SLOT(installLogRead()));
    connect(createButton,SIGNAL(clicked()),this,SLOT(installLogWrite()));
    connect(ioCheckBox,SIGNAL(clicked()),this,SLOT(ioClick()));
    connect(copyInput,SIGNAL(clicked()),this,SLOT(copyClick()));
    connect(adisplay,SIGNAL(changed()),this,SLOT(paramChanged()));
    connect(headerBrowse,SIGNAL(clicked()),this,SLOT(browseHeader()));
    connect(ioBrowseButton,SIGNAL(clicked()),this,SLOT(browseIOHeader()));
    connect(classesBox,SIGNAL(activated(int)),this,SLOT(displayClass(int)));
    connect(this,SIGNAL(selected(const QString &)),this,SLOT(pageSelected(const QString &)));
    connect(this,SIGNAL(helpClicked()),this,SLOT(requestHelp()));
#if QT_VERSION < 0x040000
    connect(variablesList, SIGNAL(clicked(QListViewItem *)),this,SLOT(variableClick(QListViewItem *)));
    connect(parameterList, SIGNAL(clicked(QListViewItem *)),this,SLOT(parameterDoubleClick(QListViewItem *)));
    connect(connectionList, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint &, int)),this,SLOT(connectionsContextMenu(QListViewItem *,const QPoint &)));
    connect(sourceList, SIGNAL(contextMenuRequested(QListBoxItem *, const QPoint &)),this,SLOT(sourcesContextMenu(QListBoxItem *,const QPoint &)));
#else
    sourceList->setContextMenuPolicy(Qt::CustomContextMenu);
    connectionList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(variablesList, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(variableClick(QTreeWidgetItem *)));
    connect(parameterList, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(parameterDoubleClick(QTreeWidgetItem *)));
    connect(connectionList, SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(connectionsContextMenu(const QPoint &)));
    connect(sourceList, SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(sourcesContextMenu(const QPoint &)));
#endif
    // connect(takeButton,SIGNAL(clicked()),this,SLOT());
    //fill variables list
    vector<string> vars=tofAnalysis::getVariables();
    for(unsigned int i=0;i<vars.size()/3;i++)
      {
#if QT_VERSION < 0x040000
	new QListViewItem(variablesList,vars[i*3].data(),vars[i*3+1].data());
#else
	new QTreeWidgetItem(variablesList,QStringList()<<vars[i*3].data()<<vars[i*3+1].data());
#endif
      }
    // 
  languageChange();
  resize( QSize(584, 381).expandedTo(minimumSizeHint()) );
  setDefault();
#if QT_VERSION < 0x040000
  clearWState( WState_Polished );
#else
  setWindowState( Qt::WindowNoState);
#endif
}
AlgorithmWizard::~AlgorithmWizard()
{
  delete []wizardPages;
  delete []pageLayouts;
  delete []labels;
}

void AlgorithmWizard::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Check Cuts" ) );
#else
    setWindowTitle( tr( "Check Cuts" ) );
#endif
    copyInput->setText(tr("copy algorithm"));
    readButton->setText(tr("read"));
    createButton->setText(tr("create"));
    ioCheckBox->setText(tr("IO-algorithm"));
    ioBrowseButton->setText(tr("browse"));
    catBox->clear();
    add2ComboBox(catBox,tr("Calibration"));
    add2ComboBox(catBox,tr("Tracking"));
    add2ComboBox(catBox,tr("Post-Tracking"));
    add2ComboBox(catBox,tr("else"));
    ioSelectBox->clear();
    add2ComboBox(ioSelectBox,tr("Input"));
    add2ComboBox(ioSelectBox,tr("Output"));
    headerBrowse->setText(tr("browse"));
    copyLinkInput->clear();
    add2ComboBox(copyLinkInput, tr("copy"));
    add2ComboBox(copyLinkInput, tr("link"));
    add2ComboBox(copyLinkInput, tr("stay"));
  labels[0]->setText(tr("Name:"));
  labels[1]->setText(tr("Cathegory"));
  labels[2]->setText(tr("Defined algorithms"));
  labels[3]->setText(tr("Header-file"));
  labels[4]->setText(tr("Classes"));
  labels[5]->setText(tr("Source-files"));
  labels[6]->setText(tr("transfer file to $KTOFPACK/algorithms/"));
  labels[7]->setText(tr("defines"));
  labels[8]->setText(tr("fit algorithms"));
  labels[9]->setText(tr("Connections"));
  labels[10]->setText(tr("Signals"));
  labels[11]->setText(tr("Slots"));
  labels[12]->setText(tr("Available Variables"));
  labels[13]->setText(tr("Finish"));
  labels[14]->setText(tr("Comment on init"));
  takeButton->setText(tr("take"));
}

bool AlgorithmWizard::checkFinish()
{
  if(nameInput->text()==""){
    showPage(wizardPages[0]);
    QMessageBox::information(this,"Missing name","Please specify an algorithm name",QMessageBox::Ok);
    return false;
  }
  for(int i=0;i<(int)definedBox->count();i++)
#if QT_VERSION < 0x040000
    if(nameInput->text()==definedBox->text(i))
#else
    if(nameInput->text()==definedBox->item(i)->text())
#endif
      {
	showPage(wizardPages[0]);
	QMessageBox::information(this,"Bad name","Please specify an UNUSED algorithm name",QMessageBox::Ok);
	return false;
      }
  if(copyInput->isChecked())return true;
  if(ioCheckBox->isChecked())
    {
      if(!existing(QS2s(ioLabel->text()))){
	showPage(wizardPages[0]);
	QMessageBox::information(this,"Missing IO-header","Please specify an IO algorithm header file",QMessageBox::Ok);
	return false;
      }
      if(!existing(QS2s(ioSourceLabel->text()))){
	showPage(wizardPages[0]);
	QMessageBox::information(this,"Missing IO-source","Please specify an IO algorithm source file",QMessageBox::Ok);
	return false;
      }
    }
  if(headerInput->text()==""){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Missing header","Please specify an algorithm header file",QMessageBox::Ok);
	return false;
  }
  if(!existing(QS2s(headerInput->text()))){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Missing header","Please specify an EXISTING algorithm header file",QMessageBox::Ok);
	return false;
  }
  if(!classesInHeader[comboBoxCurrentItem(classesBox)].inherritsFrom("AAlgorithm")){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Bad selected class","Please select a class that is derived from AAlgorithm.",QMessageBox::Ok);
	return false;
  }
  int n=0;
  for(int i=0;i<(int)sourceList->count();i++)
#if QT_VERSION < 0x040000
    if(existing(QS2s(sourceList->text(i))))n++;
#else
    if(existing(QS2s(sourceList->item(i)->text())))n++;
#endif
  if(n<=0){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Missing source","Please specify algorithm source file(s)",QMessageBox::Ok);
	return false;
  }
  if(commentInput->text()==""){
	showPage(wizardPages[2]);
	QMessageBox::information(this,"Missing comment","Please specify a comment put to log file on algorithm init",QMessageBox::Ok);
	return false;
  }
#if QT_VERSION < 0x040000
  QListViewItem *it=parameterList->firstChild();
  while(it!=NULL)
    {
      if(it->text(2)==""){
	showPage(wizardPages[3]);
	QMessageBox::information(this,"Missing parameter","Please specify values for all parameters in constructor.",QMessageBox::Ok);
	return false;
      }
      it=it->nextSibling();
    }
#else
  for(int i=0;i<parameterList->topLevelItemCount();i++)
    {
      if(parameterList->topLevelItem(i)->text(2)==""){
	showPage(wizardPages[3]);
	QMessageBox::information(this,"Missing parameter","Please specify values for all parameters in constructor.",QMessageBox::Ok);
	return false;
      }

    }
#endif
  return true;
}
void AlgorithmWizard::pageSelected(const QString & pagename)
{
  if(pagename!="Finish")return;
  setFinishEnabled(wizardPages[4],checkFinish());
}

void AlgorithmWizard::installLogRead()
{
  string s=getOpenFileName(this,"Read install Log",getenv("KTOFPACK"),"install logs (*.data) ;; any file (*)");
  if(s==""||!existing(s))return;
  installLogIO(s,false);
}
void AlgorithmWizard::installLogWrite()
{
  string s=getSaveFileName(this,"Write install Log",getenv("KTOFPACK"),"install logs (*.data) ;; any file (*)",true);
  if(s=="")return;
  readParameters();
  installLogIO(s,true);
}
void AlgorithmWizard::installLogIO(const string &filename, bool write)
{
  if(write)
    {
      ofstream out;
      out.open(filename.data());
      out<<algoWriter;
      out.close();
    }
  else
    {
      if(!existing(filename))return;
      ifstream out;
      out.open(filename.data());
      out>>algoWriter;
      out.close();
      string s=filename.substr(0,filename.rfind("/"));
      if(!existing(algoWriter.headerFile()))
	{
	  if(existing(s+"/"+algoWriter.headerFile()))
	    algoWriter.setHeaderFile(s+"/"+algoWriter.headerFile());
	  else if(existing(string(getenv("KTOFPACK"))+"/algorithms/include/"+algoWriter.headerFile()))
	    algoWriter.setHeaderFile(string(getenv("KTOFPACK"))+"/algorithms/include/"+algoWriter.headerFile());
	}
      for(int i=0;i<algoWriter.nSourceFiles();i++)
	if(!existing(algoWriter.sourceFiles(i)))
	{
	  if(existing(s+"/"+algoWriter.sourceFiles(i)))
	    algoWriter.setSourceFiles(i,s+"/"+algoWriter.sourceFiles(i));
	  else if(existing(string(getenv("KTOFPACK"))+"/algorithms/src/"+algoWriter.sourceFiles(i)))
	    algoWriter.setSourceFiles(i,string(getenv("KTOFPACK"))+"/algorithms/src/"+algoWriter.sourceFiles(i));
	}
      if(!existing(algoWriter.widgetHeader()))
	{
	  if(existing(s+"/"+algoWriter.widgetHeader()))
	    algoWriter.setWidgetHeader(s+"/"+algoWriter.widgetHeader());
	  else if(existing(string(getenv("KTOFPACK"))+"/gui/io_algorithm_widgets/include/"+algoWriter.widgetHeader()))
	    algoWriter.setWidgetHeader(string(getenv("KTOFPACK"))+"/gui/io_algorithm_widgets/include/"+algoWriter.widgetHeader());
	}
	if(!existing(algoWriter.widgetSource()))
	{
	  if(existing(s+"/"+algoWriter.widgetSource()))
	    algoWriter.setWidgetSource(s+"/"+algoWriter.widgetSource());
	  else if(existing(string(getenv("KTOFPACK"))+"/gui/io_algorithm_widgets/src/"+algoWriter.widgetSource()))
	    algoWriter.setWidgetSource(string(getenv("KTOFPACK"))+"/gui/io_algorithm_widgets/src/"+algoWriter.widgetSource());
	}
	cout<<algoWriter.widgetHeader().data()<<endl;
//       headerInput->setText(algoWriter.headerFile());
//       readHeader(algoWriter.headerFile());
//       algoWriter.setDefinition(classesInHeader[algoWriter.classInHeader()]);
      updateWidgets();
    }
  logLabel->setText(filename.data());
}
void AlgorithmWizard::copyClick()
{
  setAppropriate(adisplay,!copyInput->isChecked());
  setAppropriate(wizardPages[1],!copyInput->isChecked());
  setAppropriate(wizardPages[2],!copyInput->isChecked());
  setAppropriate(wizardPages[3],!copyInput->isChecked());
}
void AlgorithmWizard::ioClick()
{
  ioBrowseButton->setEnabled(ioCheckBox->isChecked());
  ioSelectBox->setEnabled(ioCheckBox->isChecked());
  ioClassBox->setEnabled(ioCheckBox->isChecked());
}
void AlgorithmWizard::updateWidgets()
{
  parameter=algoWriter.parameter();
  paramChanged();
  commentInput->setText(algoWriter.commentOnInit().data());
  ioCheckBox->setChecked(algoWriter.isIo());
  ioClick();
  if(algoWriter.isIo())
    {
      readIOFile(algoWriter.widgetHeader());
      ioLabel->setText(algoWriter.widgetHeader().data());
      ioSourceLabel->setText(algoWriter.widgetSource().data());
    }
  setComboBoxCurrentItem(ioSelectBox,algoWriter.inputAlgo());
  adisplay->setAlgorithm(&parameter);
  readHeader(algoWriter.headerFile());
  displayClass(algoWriter.classInHeader());
  setComboBoxCurrentItem(copyLinkInput,algoWriter.copyOrLink());
  sourceList->clear();
  for(int i=0;i<algoWriter.nSourceFiles();i++)
#if QT_VERSION < 0x040000
    sourceList->insertItem(algoWriter.sourceFiles(i).data());
  ((QRadioButton*)(frequencyGroup->find(algoWriter.frequency())))->setChecked(true);
#else
  sourceList->addItem(algoWriter.sourceFiles(i).data());
  ((QRadioButton*)(frequencyGroup->button(algoWriter.frequency())))->setChecked(true);
#endif
  if(algoWriter.frequency()==3)
    setComboBoxCurrentItem(frequencyBox,algoWriter.frequencyString());
  fitBox->setValue(algoWriter.nunCall());
  //  algoWriter
}
void AlgorithmWizard::paramChanged()
{
  algoWriter.setParameter(parameter);
  nameInput->setText(parameter.getName().data());
  setComboBoxCurrentItem(catBox,parameter.getCategory());
  adisplay->setAlgorithm(&parameter);
  frequencyBox->clear();
  for(int i=0;i<parameter.getNumberOfParam<vector<int> >();i++)
    add2ComboBox(frequencyBox,parameter.getParam<vector<int> >(i).getName().data());
  for(int i=0;i<parameter.getNumberOfParam<vector<float> >();i++)
    add2ComboBox(frequencyBox,parameter.getParam<vector<float> >(i).getName().data());
  for(int i=0;i<parameter.getNumberOfParam<vector<string> >();i++)
    add2ComboBox(frequencyBox,parameter.getParam<vector<string> >(i).getName().data());
}
void AlgorithmWizard::browseHeader()
{
  string s=getOpenFileName(this,"Open C++ header file",getenv("KTOFPACK"),"C++ header files (*.h; *.H) ;; any files (*)");
  if(s==""||!existing(s))return;
  readHeader(s);
}
void AlgorithmWizard::browseIOHeader()
{
  string sf;
  string s=getOpenFileName(this,"Open C++ widget-header/source file",getenv("KTOFPACK"),"C++ header files (*.h; *.H) ;; C++ source files (*.C; *.cpp; *.cxx) ;; any files (*)",&sf);
  if(s==""||!existing(s))return;
  if(sf=="")return;
  if(sf=="C++ header files (*.h; *.H)")
    readIOFile(s);
  else if(sf=="C++ source files (*.C; *.cpp; *.cxx)")
    ioSourceLabel->setText(s.data());
  else
    {
      string ext=s.substr(s.rfind(".")+1,s.length()-s.rfind(".")-1);
      if(ext=="h"||ext=="H")
	readIOFile(s);
      else if(ext=="C"||ext=="cpp"||"ext==CPP"||ext=="cxx"||ext=="CXX")
	ioSourceLabel->setText(s.data());
    }
}
void AlgorithmWizard::readHeader(const string &filename)
{
  if(!existing(filename))return;
  while(!classesInHeader.empty())classesInHeader.pop_back();
  TMyClass::fileContent(1,filename,classesInHeader);
  classesBox->clear();
  for(unsigned int i=0;i<classesInHeader.size();i++)
    add2ComboBox(classesBox,classesInHeader[i].Name().data());
  headerInput->setText(filename.data());
  algoWriter.setHeaderFile(filename);
  algoWriter.setDefinition(classesInHeader[algoWriter.classInHeader()]);
  displayClass(0);
}
void AlgorithmWizard::readIOFile(const string &filename)
{
  if(!existing(filename))return;
  while(!IOClasses.empty())IOClasses.pop_back();
  TMyClass::fileContent(1,filename,IOClasses);
  ioClassBox->clear();
  for(unsigned int i=0;i<IOClasses.size();i++)
    add2ComboBox(ioClassBox,IOClasses[i].Name().data());
  ioLabel->setText(filename.data());
  algoWriter.setWidgetHeader(filename);
}
void AlgorithmWizard::displayClass(int num)
{
  if(num<0||num>=(int)classesInHeader.size())return;
  classDefinitionShow->clear();
  vector<string> tmp=classesInHeader[num].toHeader();
  for(unsigned int i=0;i<tmp.size();i++)
    classDefinitionShow->append(tmp[i].data());
  classDefinitionShow->setReadOnly(true);
  constructorEdit->clear();
  if(classesInHeader[num].nConstructors()>0)
    {
      functionNameTypeDoc& constr=classesInHeader[num].Constructor(0);
      constructorEdit->append(constr.toString().data());
      parameterList->clear();
      bool sameClass=(classesInHeader[num].Name()==algoWriter.definition().Name());
      for(int i=constr.nParameters()-1;i>=0;i--)
	{
	  string s1=constr.Parameter(i).Name();
	  string s2=constr.Parameter(i).Type();
#if QT_VERSION < 0x040000
	  QListViewItem *it=new QListViewItem(parameterList,QString(s1.data()),QString(s2.data()));
#else
	  QTreeWidgetItem *it=new QTreeWidgetItem(parameterList,QStringList()<<s1.data()<<s2.data());
#endif
	  if(sameClass)
	    {
	      it->setText(2,algoWriter.callParameter(i).data());
	      it->setText(3,s2.data());
	    }
	}
    }
  signalList->clear();
  slotList->clear();
  for(int i=0;i<anaClass.nSignals();i++)
    signalList->
#if QT_VERSION < 0x040000
      insertItem
#else
      addItem
#endif
      ((string("analyser::")+anaClass.Signal(i).Name()+"("+anaClass.Signal(i).ParameterList(2)+")").data());
  for(int i=0;i<classesInHeader[num].nSignals();i++)
    signalList->
#if QT_VERSION < 0x040000
      insertItem
#else
      addItem
#endif
      ((string("algorithm::")+classesInHeader[num].Signal(i).Name()+"("+classesInHeader[num].Signal(i).ParameterList(2)+")").data());
  for(int i=0;i<anaClass.nSlots(0);i++)
    slotList->
#if QT_VERSION < 0x040000
      insertItem
#else
      addItem
#endif
      ((string("analyser::")+anaClass.Slot(i,0).Name()+"("+anaClass.Slot(i,0).ParameterList(2)+")").data());
  for(int i=0;i<classesInHeader[num].nSlots(0);i++)
    slotList->
#if QT_VERSION < 0x040000
      insertItem
#else
      addItem
#endif
      ((string("algorithm::")+classesInHeader[num].Slot(i,0).Name()+"("+classesInHeader[num].Slot(i,0).ParameterList(2)+")").data());
}
void AlgorithmWizard::setDefault()
{
  string fn="default_W.data";
  this->installLogIO(fn.data(),false);
  return;
  ifstream input;
  input.open("default.data");
  input>>parameter;
  input.close();
  algoWriter.setParameter(parameter);
  updateWidgets();
}
void AlgorithmWizard::readParameters()
{
  parameter.setName(QS2s(nameInput->text()));
  parameter.setCategory(comboBoxCurrentItem(catBox));
  algoWriter.setParameter(parameter);
  algoWriter.setIo(ioCheckBox->isChecked());
  algoWriter.setWidgetHeader(QS2s(ioLabel->text()));
  algoWriter.setInputAlgo(comboBoxCurrentItem(ioSelectBox)==0);
  algoWriter.setWidgetName(QS2s(ioClassBox->currentText()));
  algoWriter.setWidgetSource(QS2s(ioSourceLabel->text()));
  algoWriter.setHeaderFile(QS2s(headerInput->text()));
  algoWriter.setClassInHeader(comboBoxCurrentItem(classesBox));
  algoWriter.setDefinition(classesInHeader[algoWriter.classInHeader()]);
  algoWriter.setCopyOrLink(comboBoxCurrentItem(copyLinkInput));
  algoWriter.clearSourceFiles();
  for(int i=0;i<(int)sourceList->count();i++)
#if QT_VERSION < 0x040000
    algoWriter.addSourceFiles(QS2s(sourceList->text(i)));
  algoWriter.setFrequency(frequencyGroup->selectedId());
#else
    algoWriter.addSourceFiles(QS2s(sourceList->item(i)->text()));
  algoWriter.setFrequency(frequencyGroup->checkedId());
#endif
  algoWriter.setFrequencyString(comboBoxCurrentItem(frequencyBox));
  algoWriter.setNunCall(fitBox->value());
  algoWriter.setCommentOnInit(QS2s(commentInput->text()));
  algoWriter.clearConnections();
  int n=0;
  algoWriter.clearCallParameter();
#if QT_VERSION < 0x040000
  QListViewItem *it=connectionList->firstChild();
  while(it!=NULL)
    {
      algoWriter.addConnection(QS2s(it->text(0)),QS2s(it->text(1)),QS2s(it->text(2)),QS2s(it->text(3)));
      it=it->nextSibling();
    }
  it=parameterList->firstChild();
  while(it!=NULL)
    {
      algoWriter.setCallParameter(n++,QS2s(it->text(2)));
      it=it->nextSibling();
    }
#else
  for(int i=0;i<connectionList->topLevelItemCount();i++)
    algoWriter.addConnection(QS2s(connectionList->topLevelItem(i)->text(0)),QS2s(connectionList->topLevelItem(i)->text(1)),QS2s(connectionList->topLevelItem(i)->text(2)),QS2s(connectionList->topLevelItem(i)->text(3)));
  for(int i=0;i<parameterList->topLevelItemCount();i++)
    algoWriter.setCallParameter(i,QS2s(parameterList->topLevelItem(i)->text(2)));
#endif
  algoWriter.setNeedsMOC(classesInHeader[algoWriter.classInHeader()].nSignals()
			 +classesInHeader[algoWriter.classInHeader()].nSlots(0)
			 +classesInHeader[algoWriter.classInHeader()].nSlots(1)
			 +classesInHeader[algoWriter.classInHeader()].nSlots(2)>0);
  if(algoWriter.needsMOC())
    {
      vector<string> vdT;
      bool nvdh=false;
      for(int j=0;j<3;j++)
	for(int i=0;i<classesInHeader[algoWriter.classInHeader()].nSlots(j);i++)
	  for(int k=0;k<classesInHeader[algoWriter.classInHeader()].Slot(i,j).nParameters();k++)
	    for(unsigned int ii=0;ii<vdT.size();ii++)
	      if(hasA(classesInHeader[algoWriter.classInHeader()].Slot(i,j).Parameter(k).Type(),vdT[ii]))
		nvdh=true;
      for(int i=0;i<classesInHeader[algoWriter.classInHeader()].nSignals();i++)
	for(int k=0;k<classesInHeader[algoWriter.classInHeader()].Signal(i).nParameters();k++)
	  for(unsigned int ii=0;ii<vdT.size();ii++)
	    if(hasA(classesInHeader[algoWriter.classInHeader()].Signal(i).Parameter(k).Type(),vdT[ii]))
	      nvdh=true;
      algoWriter.setVersionDependentHeader(nvdh);
      nvdh=false;
      n=comboBoxCurrentItem(ioClassBox);
      for(int j=0;j<3;j++)
	for(int i=0;i<IOClasses[n].nSlots(j);i++)
	  for(int k=0;k<IOClasses[n].Slot(i,j).nParameters();k++)
	    for(unsigned int ii=0;ii<vdT.size();ii++)
	      if(hasA(IOClasses[n].Slot(i,j).Parameter(k).Type(),vdT[ii]))
		nvdh=true;
      for(int i=0;i<IOClasses[n].nSignals();i++)
	for(int k=0;k<IOClasses[n].Signal(i).nParameters();k++)
	  for(unsigned int ii=0;ii<vdT.size();ii++)
	    if(hasA(IOClasses[n].Signal(i).Parameter(k).Type(),vdT[ii]))
	      nvdh=true;
      algoWriter.setIoVersionDependentHeader(nvdh);
    }
}
#if QT_VERSION < 0x040000
void AlgorithmWizard::sourcesContextMenu(QListBoxItem* it, const QPoint &p)
{
  QPopupMenu pm;
  pm.insertItem("add source file",this,SLOT(addSource()));
  if(it!=NULL)
    pm.insertItem("remove source file",this,SLOT(removeSource(int)),sourceList->index(it));
  pm.exec(p);
}
#else
void AlgorithmWizard::sourcesContextMenu( const QPoint &p)
{
  QListWidgetItem* it=sourceList->itemAt(p);
  QMenu pm;
  pm.addAction("add source file",this,SLOT(addSource()));
  if(it!=NULL)
    (pm.addAction("remove source file"))->setStatusTip(string_number(sourceList->row(it)).data());
  connect(&pm,SIGNAL(triggered(QAction *)),this,SLOT(onSourceContext(QAction*)));
  pm.exec(sourceList->viewport()->mapToGlobal(p));
}
void AlgorithmWizard::onSourceContext(QAction* action)
{
  if(action==NULL)return;
  if(action->text()=="add source file")return;
  if(action->text()=="remove source file")
      removeSource(action->statusTip().toInt());
}
#endif
void AlgorithmWizard::addSource()
{
  string s=getOpenFileName(this,"Open source file",string(getenv("KTOFPACK"))+"/algorithms/src","C++ source files (*.cpp) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  bool definesD=false;
  for(int i=0;i<classesInHeader[comboBoxCurrentItem(classesBox)].nMethods(0);i++)
    if(classesInHeader[comboBoxCurrentItem(classesBox)].Method(i,0).Name()=="getDescription")definesD=true;
  if(definesD)
    if(QMessageBox::question(this,"Description","Parse source file for definition of algorithm description?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape)==QMessageBox::Yes)
      {
	parseSourceForDescription(s);
      }
#if QT_VERSION < 0x040000
  sourceList->insertItem(s.data());
#else
  sourceList->addItem(s.data());
#endif
}
void AlgorithmWizard::removeSource(int num)
{
  if(num<0||num>=(int)sourceList->count())return;
  if(QMessageBox::warning(this,"Removing file",QString("Remove file \"")+
#if QT_VERSION < 0x040000
			  sourceList->text(num)
#else
			  sourceList->item(num)->text()
#endif
			  +"\" from List?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape)==QMessageBox::Yes)
#if QT_VERSION < 0x040000
    sourceList->removeItem(num);
#else
    delete sourceList->takeItem(num);
#endif
}
#if QT_VERSION < 0x040000
void AlgorithmWizard::connectionsContextMenu(QListViewItem *it,const QPoint &p)
{
  currentConnection=it;
  QPopupMenu pm;
  if(signalList->currentItem()>=0&&slotList->currentItem()>=0)
    pm.insertItem("add connection",this,SLOT(addConnection()));
  if(it!=NULL)
    pm.insertItem("remove connection",this,SLOT(removeConnection()));
  pm.exec(p);
  currentConnection=NULL;
}
#else
void AlgorithmWizard::connectionsContextMenu(const QPoint &p)
{
  QTreeWidgetItem *it=connectionList->itemAt(p);
  currentConnection=it;
  QMenu pm;
  if(signalList->currentItem()>=0&&slotList->currentItem()>=0)
    pm.addAction("add connection",this,SLOT(addConnection()));
  if(it!=NULL)
    pm.addAction("remove connection",this,SLOT(removeConnection()));
  pm.exec(connectionList->viewport()->mapToGlobal(p));
  currentConnection=NULL;
}
#endif
void AlgorithmWizard::addConnection()
{
#if QT_VERSION < 0x040000
  string from=QS2s(signalList->currentText());
  string to=QS2s(slotList->currentText());
#else
  string from=QS2s(signalList->currentItem()->text());
  string to=QS2s(slotList->currentItem()->text());
#endif
  if(from.find("::")>=from.npos||to.find("::")>=to.npos)return;
  string sender,signal,receiver,slot;
  sender=from.substr(0,from.find("::"));
  signal=from.substr(from.find("::")+2,from.length()-2-from.find("::"));
  receiver=to.substr(0,to.find("::"));
  slot=to.substr(to.find("::")+2,to.length()-2-to.find("::"));
#if QT_VERSION < 0x040000
  new QListViewItem(connectionList,sender.data(),signal.data(),receiver.data(),slot.data());
#else
  new QTreeWidgetItem(connectionList,QStringList()<<sender.data()<<signal.data()<<receiver.data()<<slot.data());
#endif
}
void AlgorithmWizard::removeConnection()
{
  if(currentConnection==NULL)return;
#if QT_VERSION < 0x040000
  connectionList->takeItem(currentConnection);
#else
  connectionList->takeTopLevelItem(connectionList->indexOfTopLevelItem(currentConnection));
#endif
  delete currentConnection;
}
#if QT_VERSION < 0x040000
void AlgorithmWizard::variableClick(QListViewItem *it)
#else
void AlgorithmWizard::variableClick(QTreeWidgetItem *it)
#endif
{
  if(it==NULL)return;
  constrInput->setText(it->text(0));
  tpLabel->setText(it->text(1));
}
#if QT_VERSION < 0x040000
void AlgorithmWizard::parameterDoubleClick(QListViewItem *it)
#else
void AlgorithmWizard::parameterDoubleClick(QTreeWidgetItem *it)
#endif
{
  if(it==NULL)return;
  string s1=QS2s(tpLabel->text()),s2=QS2s(it->text(1));
  if(s1==s2)//tpLabel->text()==it->text(1))
    {
      it->setText(2,constrInput->text());
      it->setText(3,tpLabel->text());
      return;
    }
  s2=remove(s2,"&");
  s1=remove(s1,"&");
  if(s1==s2)
    {
      it->setText(2,constrInput->text());
      it->setText(3,tpLabel->text());
      return;
    }
  int ns1=contains(s1,"*");
  int ns2=contains(s2,"*");
  int ns3=contains(QS2s(constrInput->text()),"*");
  s1=remove(s1,"*");
  s2=remove(s2,"*");
  if(s1==s2)
    {
      if(ns1-ns3==ns2)
	{
	  it->setText(2,constrInput->text());
	  it->setText(3,it->text(1));
	}
    }
}
void AlgorithmWizard::parseSourceForDescription(const string &filename)
{
  if(!existing(filename))return;
  vector<string> tmp;
  char c;
  string s;
  ifstream o;
  o.open(filename.data());
  s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}
  if(o.eof()){o.close();return;}
  while(!((hasA(s,"::getDescription()")&&hasA(s,"algorithm_parameter"))||o.eof()))
    {s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}}
  if(o.eof()){o.close();return;}
  int bocount=contains(s,"{");
  int bccount=contains(s,"}");
  while(bocount<1)
    {
      s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}
      bocount+=contains(s,"{");bccount+=contains(s,"}");
    }
  algorithm_parameter tmpa;
  tmpa.setID(0);
  vector<string> vrtmp;
  while(bocount!=bccount)
    {
      s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}
      bocount+=contains(s,"{");bccount+=contains(s,"}");
      if(hasA(s,".addParam<"))//parameter of type X
	{
	  string n=s.substr(s.find("single_parameter<")+17,s.length()-s.find("single_parameter<")-17);
	  n=n.substr(n.find("(\"")+2,n.length()-2-n.find("(\""));
	  n=n.substr(0,n.find("\""));
	  string tp=s.substr(s.find(".addParam<")+10,s.find(">(")-s.find(".addParam<")-10);
	  if(tp=="int")
	    tmpa.addParam<int>(single_parameter<int>(n,-1));
	  else if(tp=="bool")
	    tmpa.addParam<bool>(single_parameter<bool>(n,false));
	  else if(tp=="float")
	    tmpa.addParam<float>(single_parameter<float>(n,-1));
	  else if(tp=="point3D")
	    tmpa.addParam<point3D>(single_parameter<point3D>(n,point3D()));
	  else if(tp=="vector3D")
	    tmpa.addParam<vector3D>(single_parameter<vector3D>(n,vector3D()));
	  else if(tp=="string")
	    tmpa.addParam<string>(single_parameter<string>(n,""));
	  else if(tp=="vector<int> ")
	    tmpa.addParam<vector<int> >(single_parameter<vector<int> >(n,vector<int>()));
	  else if(tp=="vector<float> ")
	    tmpa.addParam<vector<float> >(single_parameter<vector<float> >(n,vector<float>()));
	  else if(tp=="vector<string> ")
	    tmpa.addParam<vector<string> >(single_parameter<vector<string> >(n,vector<string>()));
	  vrtmp.push_back(n);vrtmp.push_back(tp);vrtmp.push_back("");
	}
      else if(hasA(s,"algorithm_parameter"))//constructor algorithm_parameter::algorithm_parameter(name,id,category)
	{
	  string n=s.substr(s.find("(\"")+2,s.length()-s.find("(\"")-2);
	  if(hasA(n,"\","))
	    n=n.substr(0,n.find("\","));
	  else
	    n=n.substr(0,n.find("\");"));
	  tmpa.setName(n);
	}
      else if(hasA(s,"push_back(\""))//description
	{
	  string n=s.substr(s.find("push_back(\"")+11,s.length()-s.find("push_back(\"")-14);
	  n=n.substr(0,n.find("\","));
	  tmp.push_back(n);
	}
    }
  tmpa.setDescription(tmp);
  parameter=tmpa;
  variablesList->clear();
  vector<string> vars=tofAnalysis::getVariables();
  for(unsigned int i=0;i<vars.size()/3;i++)
    {
#if QT_VERSION < 0x040000
      new QListViewItem(variablesList,vars[i*3].data(),vars[i*3+1].data());
#else
      new QTreeWidgetItem(variablesList,QStringList()<<vars[i*3].data()<<vars[i*3+1].data());
#endif
    }
  for(unsigned int i=0;i<vrtmp.size()/3;i++)
    {
#if QT_VERSION < 0x040000
      new QListViewItem(variablesList,vrtmp[i*3].data(),vrtmp[i*3+1].data());
#else
      new QTreeWidgetItem(variablesList,QStringList()<<vrtmp[i*3].data()<<vrtmp[i*3+1].data());
#endif
    }
  paramChanged();
  o.close();
}
void AlgorithmWizard::requestHelp()
{
  emit helpRequested(string(getenv("KTOFPACK"))+"/helps/installAlgorithm.html");
}
void AlgorithmWizard::accept()
{
  if(!checkFinish())
    {
      QMyWizard::reject();
      return;
    }
  readParameters();
  bool av[7]={algoWriter.headerAvail(),
	      algoWriter.makeAvail(),
	      algoWriter.analyserAvail(),
	      algoWriter.libAvail(),false,false,false};
  int ret[7];for(int i=0;i<7;i++)ret[i]=QMessageBox::No;
  if(!av[0])
    {
      ret[0]=QMessageBox::question(this,"Install algorithm","Install algorithm to algorithms-header-file?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret[0]==QMessageBox::Yes)algoWriter.insertToHeader();
    }
  else QMessageBox::information(this,"Install algorithm","Algorithm already included in algorithms-header-file.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
  if(!av[1])
    {
      ret[1]=QMessageBox::question(this,"Install algorithm","Install algorithm to algorithm-makefile?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret[1]==QMessageBox::Yes)algoWriter.insertToMakeFile();
    }
  else QMessageBox::information(this,"Install algorithm","Algorithm already included in makefile.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
  if(!av[2])
    {
      ret[2]=QMessageBox::question(this,"Install algorithm","Install algorithm to analyser?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      int id=algoWriter.getNextID();
      algoWriter.setID(id);
      parameter.setID(id);
      if(ret[2]==QMessageBox::Yes)algoWriter.insertToAnalyser();
    }
  else
    {
      int id=algoWriter.getInstalledID();
      QMessageBox::information(this,"Install algorithm",(string("Algorithm already included in analyser with ID ")+string_number(id)+".").data(),QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
      algoWriter.setID(id);
      parameter.setID(id);
    }
  if(algoWriter.isIo())
    {
      av[4]=algoWriter.ioHeaderAvail();
      av[5]=algoWriter.ioMakeAvail();
      av[6]=algoWriter.ioAvail();
      if(!av[4])
	{
	  ret[4]=QMessageBox::question(this,"Install algorithm widget","Install algorithm widget to algorithm-widgets-header-file?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
	  if(ret[4]==QMessageBox::Yes)
	    {
	      algoWriter.insertToIOHeader();
	    }
	}
      else QMessageBox::information(this,"Install algorithm widget","Algorithm widget already included in algorithm-widgets-header-file.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
      if(!av[5])
	{
	  ret[5]=QMessageBox::question(this,"Install algorithm widget","Install algorithm widget to algorithm-widget-makefile?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
	  if(ret[5]==QMessageBox::Yes)algoWriter.insertToIOMakeFile();
	}
      else QMessageBox::information(this,"Install algorithm widget","Algorithm widget already included in widget makefile.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
      if(!av[6])
	{
	  ret[6]=QMessageBox::question(this,"Install algorithm widget","Install algorithm widget to typeCase?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
	  if(ret[6]==QMessageBox::Yes)algoWriter.insertToIO();
	}
      else QMessageBox::information(this,"Install algorithm widget","Algorithm widget already included in typeCase.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
    }
  if(!(ret[1]==QMessageBox::Yes||ret[2]==QMessageBox::Yes||ret[5]==QMessageBox::Yes||ret[6]==QMessageBox::Yes))
    {
      QMessageBox::information(this,"Installation progress","Algorithm is available for analysis. No restart required.",QMessageBox::Ok|QMessageBox::Default);
    }
  else
    {
      string mes="Code files changed. Please recompile the following libraries:";
      if(ret[1]==QMessageBox::Yes)
	mes+="\nalgorithm-library";
      if(ret[2]==QMessageBox::Yes||ret[1]==QMessageBox::Yes)
	mes+="\nanalyser-library";
      if(ret[5]==QMessageBox::Yes||ret[5]==QMessageBox::Yes)
	mes+="\nio-widget-library";
      if(ret[6]==QMessageBox::Yes||ret[6]==QMessageBox::Yes)
	mes+="\ntypeCase-program";
      QMessageBox::information(this,"Installation progress",(mes+"\nand restart the program.").data(),QMessageBox::Ok|QMessageBox::Default);
    }
  QMyWizard::accept();
}
void AlgorithmWizard::setAlgorithms(const vector<algorithm_parameter> & algos)
{ 
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    definedBox->insertItem(algos[i].getName().data());
#else
    definedBox->addItem(algos[i].getName().data());
#endif
}
void AlgorithmWizard::setAlgorithms(const vector<algorithm_parameter*> & algos)
{
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    if(algos[i]!=NULL)definedBox->insertItem(algos[i]->getName().data());
#else
    if(algos[i]!=NULL)definedBox->addItem(algos[i]->getName().data());
#endif
}
void AlgorithmWizard::setAlgorithms(const vector<string> & algos)
{
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    definedBox->insertItem(algos[i].data());
#else
    definedBox->addItem(algos[i].data());
#endif
}
algorithm_parameter AlgorithmWizard::getAlgorithm()const
{
  return parameter;
}
