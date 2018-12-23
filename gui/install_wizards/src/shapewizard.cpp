#include "shapewizard.h"
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
#include <qwidget.h>
#include "gui/parameter_widgets/include/shapedisplay.h"
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
#include "gui/widgets/include/qsinglewidgetdialog.h"
#include "utilities.h"
extern bool existing(string filename);
#if QT_VERSION < 0x040000
ShapeWizard::ShapeWizard( QWidget* parent, const char* name, bool modal , WFlags fl )
:QMyWizard(parent,name,modal,fl)
{
  if(!name)setName("ShapeWizard");
#else
ShapeWizard::ShapeWizard( QWidget* parent, Qt::WindowFlags f )
:QMyWizard(parent,f)
{
  setObjectName("ShapeWizard");
#endif
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

  //  labels[1]=new QLabel(wizardPages[0]);

  labels[2]=new QLabel(wizardPages[0]);
  pageLayouts[0]->addWidget(nameInput,0,1);
  //  pageLayouts[0]->addWidget(labels[1],3,2);

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

  //second page
  adisplay=new ShapeDisplay(wizardPages[2]);
  addButton=new QPushButton(wizardPages[2]);
  removeButton=new QPushButton(wizardPages[2]);
  adisplay->setDraw(false);
  adisplay->setReadOnly(false);
#if QT_VERSION < 0x040000
  pageLayouts[2]->addMultiCellWidget(adisplay,0,0,0,1);
#else
  pageLayouts[2]->addWidget(adisplay,0,0,1,2);
#endif
  pageLayouts[2]->addWidget(addButton,1,0);
  pageLayouts[2]->addWidget(removeButton,1,1);

  //fourth page

//     //fifth/last page

  constrInput=new QLineEdit(wizardPages[3]);
  takeButton=new QPushButton(wizardPages[3]);
  tpLabel=new QLabel(wizardPages[3]);
  tpLabel->setText(tr("int"));
  labels[12]=new QLabel(wizardPages[3]);
  constructorEdit=new QTextEdit(wizardPages[3]);
  constructorEdit->setReadOnly(true);
  constructorSelectBox=new QComboBox(wizardPages[3]);
#if QT_VERSION < 0x040000
  variablesList=new QListView(wizardPages[3]);
  parameterList=new QListView(wizardPages[3]);
  parameterList->setSorting(-1);
  pageLayouts[3]->addMultiCellWidget(constructorEdit,1,1,0,3);
  pageLayouts[3]->addMultiCellWidget(parameterList,2,3,1,3);
  pageLayouts[3]->addMultiCellWidget(constrInput,5,5,0,1);
  parameterList->addColumn(tr("Parameter"));
  parameterList->addColumn(tr("type"));
  parameterList->addColumn(tr("Variable"));
  parameterList->addColumn(tr("type"));
  variablesList->addColumn(tr("Variable"));
  variablesList->addColumn(tr("type"));
  new QListViewItem(variablesList,"sp","shape_parameter");
  pageLayouts[3]->addMultiCellWidget(constructorSelectBox,0,0,0,3);
  pageLayouts[3]->setColStretch(1,2);
  pageLayouts[3]->setColStretch(0,2);
#else
  variablesList=new QTreeWidget(wizardPages[3]);
  parameterList=new QTreeWidget(wizardPages[3]);
  pageLayouts[3]->addWidget(constructorEdit,1,0,1,4);
  pageLayouts[3]->addWidget(parameterList,2,1,2,3);
  pageLayouts[3]->addWidget(constrInput,5,0,1,2);
  pageLayouts[3]->addWidget(constructorSelectBox,0,0,1,4);
  parameterList->setHeaderLabels(QStringList()<<tr("Parameter")<<tr("type")<<tr("Variable")<<tr("type"));
  variablesList->setHeaderLabels(QStringList()<<tr("Variable")<<tr("type"));
  new QTreeWidgetItem(variablesList,QStringList()<<"sp"<<"shape_parameter");
  pageLayouts[3]->setColumnStretch(1,1);
#endif
  pageLayouts[3]->setRowStretch(2,2);
  pageLayouts[3]->addWidget(variablesList,3,0);
  pageLayouts[3]->addWidget(takeButton,5,3);
  pageLayouts[3]->addWidget(labels[12],2,0);
  pageLayouts[3]->addWidget(tpLabel,5,2);
  //finish page
  labels[13]=new QLabel(wizardPages[4]);
#if QT_VERSION < 0x040000
  pageLayouts[4]->expand(3,3);
#endif
  pageLayouts[4]->addWidget(labels[13],1,1);
  //add pages to wizard
    addPage(wizardPages[0],"General");
    addPage(wizardPages[1],"Files");
    addPage(wizardPages[2],"Parameter");
    addPage(wizardPages[3],"Constructor");
    addPage(wizardPages[4],"Finish");
    for(int i=0;i<5;i++)
      {
	pageLayouts[i]->setMargin(5);
	pageLayouts[i]->setSpacing(5);
      }
    showPage(wizardPages[0]);
    /****connections**/

    connect(readButton,SIGNAL(clicked()),this,SLOT(installLogRead()));
    connect(createButton,SIGNAL(clicked()),this,SLOT(installLogWrite()));
    connect(adisplay,SIGNAL(changed()),this,SLOT(paramChanged()));
    connect(headerBrowse,SIGNAL(clicked()),this,SLOT(browseHeader()));
    connect(addButton,SIGNAL(clicked()),this,SLOT(addParameterClick()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(removeParameterClick()));
    connect(classesBox,SIGNAL(activated(int)),this,SLOT(displayClass(int)));
    connect(this,SIGNAL(selected(const QString &)),this,SLOT(pageSelected(const QString &)));
    connect(this,SIGNAL(helpClicked()),this,SLOT(requestHelp()));
    connect(constructorSelectBox,SIGNAL(activated(int)),this,SLOT(constructorSelected(int)));
#if QT_VERSION < 0x040000
    connect(variablesList, SIGNAL(clicked(QListViewItem *)),this,SLOT(variableClick(QListViewItem *)));
    connect(parameterList, SIGNAL(clicked(QListViewItem *)),this,SLOT(parameterDoubleClick(QListViewItem *)));
    //    connect(connectionList, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint &, int)),this,SLOT(connectionsContextMenu(QListViewItem *,const QPoint &)));
    connect(sourceList, SIGNAL(contextMenuRequested(QListBoxItem *, const QPoint &)),this,SLOT(sourcesContextMenu(QListBoxItem *,const QPoint &)));
#else
    sourceList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(variablesList, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(variableClick(QTreeWidgetItem *)));
    connect(parameterList, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(parameterDoubleClick(QTreeWidgetItem *)));
    connect(sourceList, SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(sourcesContextMenu(const QPoint &)));
#endif
    // connect(takeButton,SIGNAL(clicked()),this,SLOT());
 
  languageChange();
  resize( QSize(584, 381).expandedTo(minimumSizeHint()) );
  //  setDefault();
#if QT_VERSION < 0x040000
  clearWState( WState_Polished );
#else
  setWindowState( Qt::WindowNoState);
#endif

}
ShapeWizard::~ShapeWizard()
{
  delete []wizardPages;
  delete []pageLayouts;
  delete []labels;
}

void ShapeWizard::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Check Cuts" ) );
#else
    setWindowTitle( tr( "Check Cuts" ) );
#endif
    readButton->setText(tr("read"));
    createButton->setText(tr("create"));
    headerBrowse->setText(tr("browse"));
    copyLinkInput->clear();
    add2ComboBox(copyLinkInput, tr("copy"));
    add2ComboBox(copyLinkInput, tr("link"));
    add2ComboBox(copyLinkInput, tr("stay"));
  labels[0]->setText(tr("Name:"));
  labels[2]->setText(tr("Defined shapes"));
  labels[3]->setText(tr("Header-file"));
  labels[4]->setText(tr("Classes"));
  labels[5]->setText(tr("Source-files"));
  labels[6]->setText(tr("transfer file to $KTOFPACK/shapes/"));
  labels[12]->setText(tr("Available Variables"));
  labels[13]->setText(tr("Finish"));
  takeButton->setText(tr("take"));
  addButton->setText(tr("add"));
  removeButton->setText(tr("remove"));
}

bool ShapeWizard::checkFinish()
{
  if(nameInput->text()==""){
    showPage(wizardPages[0]);
    QMessageBox::information(this,"Missing name","Please specify an shape name",QMessageBox::Ok);
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
	QMessageBox::information(this,"Bad name","Please specify an UNUSED shape name",QMessageBox::Ok);
	return false;
      }
    
  if(headerInput->text()==""){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Missing header","Please specify an shape header file",QMessageBox::Ok);
	return false;
  }
  if(!existing(QS2s(headerInput->text()))){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Missing header","Please specify an EXISTING shape header file",QMessageBox::Ok);
	return false;
  }
  if(!classesInHeader[comboBoxCurrentItem(classesBox)].inherritsFrom("volumeShape")&&!classesInHeader[comboBoxCurrentItem(classesBox)].inherritsFrom("planeShape")){
	showPage(wizardPages[1]);
	QMessageBox::information(this,"Bad selected class","Please select a class that is derived from volumeShape or planeShape.",QMessageBox::Ok);
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
	QMessageBox::information(this,"Missing source","Please specify shape source file(s)",QMessageBox::Ok);
	return false;
  }
#if QT_VERSION < 0x040000
  QListViewItem *it=parameterList->firstChild();
  while(it!=NULL)
    {
      if(it->text(2)==""){
	showPage(wizardPages[2]);
	QMessageBox::information(this,"Missing parameter","Please specify values for all parameters in constructor.",QMessageBox::Ok);
	return false;
      }
      it=it->nextSibling();
    }
#else
  for(int i=0;i<parameterList->topLevelItemCount();i++)
    {
      if(parameterList->topLevelItem(i)->text(2)==""){
	showPage(wizardPages[2]);
	QMessageBox::information(this,"Missing parameter","Please specify values for all parameters in constructor.",QMessageBox::Ok);
	return false;
      }

    }
#endif
  return true;
}
void ShapeWizard::pageSelected(const QString & pagename)
{
  if(pagename!="Finish")return;
  setFinishEnabled(wizardPages[4],checkFinish());
}

void ShapeWizard::installLogRead()
{
  string s=getOpenFileName(this,"Read install Log",getenv("KTOFPACK"),"install logs (*.data) ;; any file (*)");
  if(s==""||!existing(s))return;
  installLogIO(s,false);
}
void ShapeWizard::installLogWrite()
{
  string s=getSaveFileName(this,"Write install Log",getenv("KTOFPACK"),"install logs (*.data) ;; any file (*)",true);
  if(s=="")return;
  readParameters();
  installLogIO(s,true);
}
void ShapeWizard::installLogIO(const string &filename, bool write)
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
	  else if(existing(string(getenv("KTOFPACK"))+"/shapes/include/"+algoWriter.headerFile()))
	    algoWriter.setHeaderFile(string(getenv("KTOFPACK"))+"/shapes/include/"+algoWriter.headerFile());
	}
      for(int i=0;i<algoWriter.nSourceFiles();i++)
	if(!existing(algoWriter.sourceFiles(i)))
	{
	  if(existing(s+"/"+algoWriter.sourceFiles(i)))
	    algoWriter.setSourceFiles(i,s+"/"+algoWriter.sourceFiles(i));
	  else if(existing(string(getenv("KTOFPACK"))+"/shapes/src/"+algoWriter.sourceFiles(i)))
	    algoWriter.setSourceFiles(i,string(getenv("KTOFPACK"))+"/shapes/src/"+algoWriter.sourceFiles(i));
	}
      //      readHeader(algoWriter.headerFile());
//       algoWriter.setDefinition(classesInHeader[algoWriter.classInHeader()]);
      updateWidgets();
    }
  logLabel->setText(filename.data());
}

void ShapeWizard::updateWidgets()
{
  parameter=algoWriter.parameter();
  setComboBoxCurrentItem(constructorSelectBox,algoWriter.constructorInClass());
  paramChanged();
  adisplay->setShape(parameter);
  variablesList->clear();
#if QT_VERSION < 0x040000
  new QListViewItem(variablesList,"sp","shape_parameter");
  for(int i=0;i<parameter.NumberOfParams<point3D>();i++)new QListViewItem(variablesList,parameter.getParamName<point3D>(i).data(),"point3D");
  for(int i=0;i<parameter.NumberOfParams<vector3D>();i++)new QListViewItem(variablesList,parameter.getParamName<vector3D>(i).data(),"vector3D");
  for(int i=0;i<parameter.NumberOfParams<float>();i++)new QListViewItem(variablesList,parameter.getParamName<float>(i).data(),"float");
  for(int i=0;i<parameter.NumberOfParams<int>();i++)new QListViewItem(variablesList,parameter.getParamName<int>(i).data(),"int");
  for(int i=0;i<parameter.NumberOfParams<string>();i++)new QListViewItem(variablesList,parameter.getParamName<string>(i).data(),"string");
#else
  new QTreeWidgetItem(variablesList,QStringList()<<"sp"<<"shape_parameter");
  for(int i=0;i<parameter.NumberOfParams<point3D>();i++)new QTreeWidgetItem(variablesList,QStringList()<<parameter.getParamName<point3D>(i).data()<<"point3D");
  for(int i=0;i<parameter.NumberOfParams<vector3D>();i++)new QTreeWidgetItem(variablesList,QStringList()<<parameter.getParamName<vector3D>(i).data()<<"vector3D");
  for(int i=0;i<parameter.NumberOfParams<float>();i++)new QTreeWidgetItem(variablesList,QStringList()<<parameter.getParamName<float>(i).data()<<"float");
  for(int i=0;i<parameter.NumberOfParams<int>();i++)new QTreeWidgetItem(variablesList,QStringList()<<parameter.getParamName<int>(i).data()<<"int");
  for(int i=0;i<parameter.NumberOfParams<string>();i++)new QTreeWidgetItem(variablesList,QStringList()<<parameter.getParamName<string>(i).data()<<"string");
#endif
  readHeader(algoWriter.headerFile());
  displayClass(algoWriter.classInHeader());
  setComboBoxCurrentItem(copyLinkInput,algoWriter.copyOrLink());
  sourceList->clear();
  for(int i=0;i<algoWriter.nSourceFiles();i++)
#if QT_VERSION < 0x040000
    sourceList->insertItem(algoWriter.sourceFiles(i).data());
#else
  sourceList->addItem(algoWriter.sourceFiles(i).data());
#endif
  //  algoWriter
}
void ShapeWizard::paramChanged()
{
  algoWriter.setParameter(parameter);
  nameInput->setText(parameter.getName().data());
}
void ShapeWizard::browseHeader()
{
  string s=getOpenFileName(this,"Open C++ header file",getenv("KTOFPACK"),"C++ header files (*.h; *.H) ;; any files (*)");
  if(s==""||!existing(s))return;
  readHeader(s);
}
void ShapeWizard::readHeader(const string &filename)
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
void ShapeWizard::displayClass(int num)
{
  if(num<0||num>=(int)classesInHeader.size())return;
  classDefinitionShow->clear();
  vector<string> tmp=classesInHeader[num].toHeader();
  for(unsigned int i=0;i<tmp.size();i++)
    classDefinitionShow->append(tmp[i].data());
  classDefinitionShow->setReadOnly(true);
  constructorSelectBox->clear();
  for(int i=0;i<classesInHeader[num].nConstructors();i++)
    add2ComboBox(constructorSelectBox,classesInHeader[num].Constructor(i).toString().data());
  setComboBoxCurrentItem(constructorSelectBox,algoWriter.constructorInClass());
  constructorSelected(algoWriter.constructorInClass());
}
void ShapeWizard::setDefault()
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
int ShapeWizard::parameterFromName(string nam, string tp, int &tpID)
{
  int ret=-1;
  if(tp=="point3D")
    {
      tpID=0;
      for(int i=0;i<parameter.NumberOfParams<point3D>();i++)
	if(nam==parameter.getParamName<point3D>(i))
	  ret=i;
    }
  else if(tp=="vector3D")
    {
      tpID=1;
      for(int i=0;i<parameter.NumberOfParams<vector3D>();i++)
	if(nam==parameter.getParamName<vector3D>(i))
	  ret=i;
    }
  else if(tp=="float")
    {
      tpID=2;
      for(int i=0;i<parameter.NumberOfParams<float>();i++)
	if(nam==parameter.getParamName<float>(i))
	  ret=i;
    }
  else if(tp=="int")
    {
      tpID=3;
      for(int i=0;i<parameter.NumberOfParams<int>();i++)
	if(nam==parameter.getParamName<int>(i))
	  ret=i;
    }
  else if(tp=="string")
    {
      tpID=4;
      for(int i=0;i<parameter.NumberOfParams<string>();i++)
	if(nam==parameter.getParamName<string>(i))
	  ret=i;
    }
  return ret;
}
void ShapeWizard::readParameters()
{
  parameter.setName(QS2s(nameInput->text()));
  algoWriter.setParameter(parameter);
  algoWriter.setHeaderFile(QS2s(headerInput->text()));
  algoWriter.setClassInHeader(comboBoxCurrentItem(classesBox));
  algoWriter.setDefinition(classesInHeader[algoWriter.classInHeader()]);
  algoWriter.setCopyOrLink(comboBoxCurrentItem(copyLinkInput));
  algoWriter.clearSourceFiles();
  for(int i=0;i<(int)sourceList->count();i++)
#if QT_VERSION < 0x040000
    algoWriter.addSourceFiles(QS2s(sourceList->text(i)));
  QListViewItem *it=parameterList->firstChild();
  int id, n=0;
  while(it!=NULL)
    {
      n=parameterFromName(QS2s(it->text(2)),QS2s(it->text(3)),id);
      if(n>=0&&id>=0)
	algoWriter.setCallParameter(n,string("sp.getParam<")+QS2s(it->text(3))+">("+string_number(n)+")");
      else
	algoWriter.setCallParameter(n,QS2s(it->text(2)));
      n++;
      it=it->nextSibling();
    }
#else
  algoWriter.addSourceFiles(QS2s(sourceList->item(i)->text()));
  int id,n;
  for(int i=0;i<parameterList->topLevelItemCount();i++)
    {
      n=parameterFromName(QS2s(parameterList->topLevelItem(i)->text(2)),QS2s(parameterList->topLevelItem(i)->text(3)),id);
      if(n>=0&&id>=0)
	algoWriter.setCallParameter(i,string("sp.getParam<")+QS2s(parameterList->topLevelItem(i)->text(3))+">("+string_number(n)+")");
      else
	algoWriter.setCallParameter(i,QS2s(parameterList->topLevelItem(i)->text(2)));
    }
#endif

}
#if QT_VERSION < 0x040000
void ShapeWizard::sourcesContextMenu(QListBoxItem* it, const QPoint &p)
{
  QPopupMenu pm;
  pm.insertItem("add source file",this,SLOT(addSource()));
  if(it!=NULL)
    pm.insertItem("remove source file",this,SLOT(removeSource(int)),sourceList->index(it));
  pm.exec(p);
}
#else
void ShapeWizard::sourcesContextMenu( const QPoint &p)
{
  QListWidgetItem* it=sourceList->itemAt(p);
  QMenu pm;
  pm.addAction("add source file",this,SLOT(addSource()));
  if(it!=NULL)
    (pm.addAction("remove source file"))->setStatusTip(string_number(sourceList->row(it)).data());
  connect(&pm,SIGNAL(triggered(QAction *)),this,SLOT(onSourceContext(QAction*)));
  pm.exec(sourceList->viewport()->mapToGlobal(p));
}
void ShapeWizard::onSourceContext(QAction* action)
{
  if(action==NULL)return;
  if(action->text()=="add source file")return;
  if(action->text()=="remove source file")
      removeSource(action->statusTip().toInt());
}
#endif
void ShapeWizard::addSource()
{
  string s=getOpenFileName(this,"Open source file",string(getenv("KTOFPACK"))+"/shapes/src","C++ source files (*.cpp) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  bool definesD=false;
  for(int i=0;i<classesInHeader[comboBoxCurrentItem(classesBox)].nMethods(0);i++)
    if(classesInHeader[comboBoxCurrentItem(classesBox)].Method(i,0).Name()=="getDefinition")definesD=true;
  if(definesD)
    if(QMessageBox::question(this,"Description","Parse source file for definition of shape description?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape)==QMessageBox::Yes)
      {
	parseSourceForDescription(s);
      }
#if QT_VERSION < 0x040000
  sourceList->insertItem(s.data());
#else
  sourceList->addItem(s.data());
#endif
}
void ShapeWizard::removeSource(int num)
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
void ShapeWizard::variableClick(QListViewItem *it)
#else
void ShapeWizard::variableClick(QTreeWidgetItem *it)
#endif
{
  if(it==NULL)return;
  constrInput->setText(it->text(0));
  tpLabel->setText(it->text(1));
}
#if QT_VERSION < 0x040000
void ShapeWizard::parameterDoubleClick(QListViewItem *it)
#else
void ShapeWizard::parameterDoubleClick(QTreeWidgetItem *it)
#endif
{
  if(it==NULL)return;
  if(tpLabel->text()==it->text(1))
    {
      it->setText(2,constrInput->text());
      it->setText(3,tpLabel->text());
    }
}
void ShapeWizard::parseSourceForDescription(const string &filename)
{
  if(!existing(filename))return;
  return;
  /*
  vector<string> tmp;
  char c;
  string s;
  ifstream o;
  o.open(filename.data());
  s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}
  if(o.eof()){o.close();return;}
  while(!((hasA(s,"::getDescription()")&&hasA(s,"shape_parameter"))||o.eof()))
    {s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}}
  if(o.eof()){o.close();return;}
  int bocount=contains(s,"{");
  int bccount=contains(s,"}");
  while(bocount<1)
    {
      s="";o.get(c);while(!(c=='\n'||o.eof())){s=s+c;o.get(c);}
      bocount+=contains(s,"{");bccount+=contains(s,"}");
    }
  shape_parameter tmpa;
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
	}
      else if(hasA(s,"shape_parameter"))//constructor shape_parameter::shape_parameter(name,id,category)
	{
	  string n=s.substr(s.find("(\"")+2,s.length()-s.find("(\"")-2);
	  n=n.substr(0,n.find("\","));
	  tmpa.setName(n);
	}
      else if(hasA(s,"push_back(\""))//description
	{
	  string n=s.substr(s.find("push_back(\"")+11,s.length()-s.find("push_back(\"")-11);
	  n=n.substr(0,n.find("\","));
	  tmp.push_back(n);
	}
    }
  tmpa.setDescription(tmp);
  parameter=tmpa;
  paramChanged();
  o.close();
  */
}
void ShapeWizard::requestHelp()
{
  emit helpRequested(string(getenv("KTOFPACK"))+"/helps/installShape.html");
}
void ShapeWizard::accept()
{
  if(!checkFinish())
    {
      QMyWizard::reject();
      return;
    }
  readParameters();
  bool av[7]={algoWriter.headerAvail(),
	      algoWriter.makeAvail(),
	      algoWriter.shapeAvail(),
	      algoWriter.libAvail(),false,false,false};
  int ret[7];for(int i=0;i<7;i++)ret[i]=QMessageBox::No;
  if(!av[0])
    {
      ret[0]=QMessageBox::question(this,"Install shape","Install shape to shapes-header-file?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret[0]==QMessageBox::Yes)algoWriter.insertToHeader();
    }
  else QMessageBox::information(this,"Install shape","Shape already included in shapes-header-file.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
  if(!av[1])
    {
      ret[1]=QMessageBox::question(this,"Install shape","Install shape to shape-makefile?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret[1]==QMessageBox::Yes)algoWriter.insertToMakeFile();
    }
  else QMessageBox::information(this,"Install shape","Shape already included in makefile.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
  if(!av[2])
    {
      ret[2]=QMessageBox::question(this,"Install shape","Install shape to analyser?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret[2]==QMessageBox::Yes)algoWriter.insertToShapes();
    }
  else
    {
      QMessageBox::information(this,"Install shape","Shape already included in analyser.",QMessageBox::Ok|QMessageBox::Default,QMessageBox::NoButton);
    }
  if(!(ret[1]==QMessageBox::Yes||ret[2]==QMessageBox::Yes||ret[5]==QMessageBox::Yes||ret[6]==QMessageBox::Yes))
    {
      QMessageBox::information(this,"Installation progress","Shape is available for analysis. No restart required.",QMessageBox::Ok|QMessageBox::Default);
    }
  else
    {
      string mes="Code files changed. Please recompile the following libraries:";
      if(ret[1]==QMessageBox::Yes)
	mes+="\nshape-library";
      if(ret[2]==QMessageBox::Yes||ret[2]==QMessageBox::Yes)
	mes+="\nanalyser-library";
      if(ret[5]==QMessageBox::Yes||ret[5]==QMessageBox::Yes)
	mes+="\nio-widget-library";
      if(ret[6]==QMessageBox::Yes||ret[6]==QMessageBox::Yes)
	mes+="\ntypeCase-program";
      QMessageBox::information(this,"Installation progress",(mes+"\nand restart the program.").data(),QMessageBox::Ok|QMessageBox::Default);
    }
  QMyWizard::accept();
}
void ShapeWizard::setShapes(const vector<shape_parameter> & algos)
{ 
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    definedBox->insertItem(algos[i].getName().data());
#else
    definedBox->addItem(algos[i].getName().data());
#endif
}
void ShapeWizard::setShapes(const vector<shape_parameter*> & algos)
{
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    if(algos[i]!=NULL)definedBox->insertItem(algos[i]->getName().data());
#else
    if(algos[i]!=NULL)definedBox->addItem(algos[i]->getName().data());
#endif
}
void ShapeWizard::setShapes(const vector<string> & algos)
{
  definedBox->clear();
  for(unsigned int i=0;i<algos.size();i++)
#if QT_VERSION < 0x040000
    definedBox->insertItem(algos[i].data());
#else
    definedBox->addItem(algos[i].data());
#endif
}
shape_parameter ShapeWizard::getShape()const
{
  return parameter;
}
void ShapeWizard::addParameterClick()
{
  QSingleWidgetDialog fenst(this);
#if QT_VERSION < 0x040000
  fenst.setCaption("new parameter");
#else
  fenst.setWindowTitle("new parameter");
#endif
  QWidget tmpWidget(&fenst);
  fenst.setWidget(&tmpWidget);
  QLabel *tmpLabel1=new QLabel(&tmpWidget);
  QLabel *tmpLabel2=new QLabel(&tmpWidget);
  QLineEdit *tmpInput1=new QLineEdit(&tmpWidget);
  QComboBox *tmpInput2=new QComboBox(&tmpWidget);
  QGridLayout *tmpLayout=new QGridLayout(&tmpWidget);
  tmpLayout->addWidget(tmpLabel1,0,0);
  tmpLayout->addWidget(tmpLabel2,1,0);
  tmpLayout->addWidget(tmpInput1,0,1);
  tmpLayout->addWidget(tmpInput2,1,1);
  tmpLabel1->setText(tr("Name"));
  tmpLabel2->setText(tr("Type"));
  tmpInput1->setText("new parameter");
  add2ComboBox(tmpInput2,"point3D");
  add2ComboBox(tmpInput2,"vector3D");
  add2ComboBox(tmpInput2,"float");
  add2ComboBox(tmpInput2,"int");
  add2ComboBox(tmpInput2,"string");
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      string tp="";
      switch(comboBoxCurrentItem(tmpInput2))
	{
	case 0: tp="point3D";parameter.addParam<point3D>(point3D(),QS2s(tmpInput1->text()));break;
	case 1: tp="vector3D";parameter.addParam<vector3D>(vector3D(),QS2s(tmpInput1->text()));break;
	case 2: tp="float";parameter.addParam<float>(0,QS2s(tmpInput1->text()));break;
	case 3: tp="int";parameter.addParam<int>(0,QS2s(tmpInput1->text()));break;
	case 4: tp="string";parameter.addParam<string>("",QS2s(tmpInput1->text()));break;
	}
      adisplay->setShape(parameter);
      paramChanged();
#if QT_VERSION < 0x040000
      new QListViewItem(variablesList,tmpInput1->text(),tp.data());
#else
      new QTreeWidgetItem(variablesList,QStringList()<<tmpInput1->text()<<tp.data());
#endif
    }
}
void ShapeWizard::removeParameterClick()
{
  QSingleWidgetDialog fenst(this);
  vector<string> pnames;
  for(int i=0;i<parameter.NumberOfParams<point3D>();i++)pnames.push_back(parameter.getParamName<point3D>(i)+"<point3D>");
  for(int i=0;i<parameter.NumberOfParams<vector3D>();i++)pnames.push_back(parameter.getParamName<vector3D>(i)+"<vector3D>");
  for(int i=0;i<parameter.NumberOfParams<float>();i++)pnames.push_back(parameter.getParamName<float>(i)+"<float>");
  for(int i=0;i<parameter.NumberOfParams<int>();i++)pnames.push_back(parameter.getParamName<int>(i)+"<int");
  for(int i=0;i<parameter.NumberOfParams<string>();i++)pnames.push_back(parameter.getParamName<string>(i)+"<string>");
#if QT_VERSION < 0x040000
  fenst.setCaption("new parameter");
  QListBox tmpWidget(&fenst);
  for(unsigned int i=0;i<pnames.size();i++)
    tmpWidget.insertItem(pnames[i].data());
#else
  fenst.setWindowTitle("new parameter");
  QListWidget tmpWidget(&fenst);
  for(unsigned int i=0;i<pnames.size();i++)
    tmpWidget.addItem(pnames[i].data());
#endif
  fenst.setWidget(&tmpWidget);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      string s;
#if QT_VERSION < 0x040000
      s=QS2s(tmpWidget.currentText());
#else
      s=QS2s(tmpWidget.currentItem()->text());
#endif
      if(QMessageBox::warning(this,"Remove Parameter",(string("Parameter \"")+s+"\" will be removed.").data(),QMessageBox::Yes|QMessageBox::Default,QMessageBox::Cancel|QMessageBox::Escape)==QMessageBox::Yes)
	{
	  int tp=-1;
	  string s1=s.substr(s.find("<")+1,s.find(">")-s.find("<")-1);
	  s=s.substr(0,s.find("<"));
	  if(s1=="point3D")tp=0;
	  else if(s1=="vector3D")tp=1;
	  else if(s1=="float")tp=2;
	  else if(s1=="int")tp=3;
	  else if(s1=="string")tp=4;
	  if(tp==-1)return;
	  shape_parameter tmpShape;
	  tmpShape.setName(parameter.getName());
	  for(int i=0;i<parameter.NumberOfParams<point3D>();i++)
	    if(tp!=0||parameter.getParamName<point3D>(i)!=s)
	      tmpShape.addParam<point3D>(parameter.getParam<point3D>(i),parameter.getParamName<point3D>(i));;
	  for(int i=0;i<parameter.NumberOfParams<vector3D>();i++)
	    if(tp!=1||parameter.getParamName<vector3D>(i)!=s)
	      tmpShape.addParam<vector3D>(parameter.getParam<vector3D>(i),parameter.getParamName<vector3D>(i));;
	  for(int i=0;i<parameter.NumberOfParams<float>();i++)
	    if(tp!=2||parameter.getParamName<float>(i)!=s)
	      tmpShape.addParam<float>(parameter.getParam<float>(i),parameter.getParamName<float>(i));;
	  for(int i=0;i<parameter.NumberOfParams<int>();i++)
	    if(tp!=3||parameter.getParamName<int>(i)!=s)
	      tmpShape.addParam<int>(parameter.getParam<int>(i),parameter.getParamName<int>(i));;
	  for(int i=0;i<parameter.NumberOfParams<string>();i++)
	    if(tp!=4||parameter.getParamName<string>(i)!=s)
	      tmpShape.addParam<string>(parameter.getParam<string>(i),parameter.getParamName<string>(i));;
	  parameter=tmpShape;
	  adisplay->setShape(parameter);
	  paramChanged();
#if QT_VERSION < 0x040000
	  QListViewItem *it=variablesList->firstChild();
	  while(it!=NULL)
	    {
	      if(it->text(0)==s.data()&&it->text(1)==s1.data())break;
	      it=it->nextSibling();
	    }
	  if(it!=NULL)
	    {
	      variablesList->takeItem(it);
	      delete it;
	    }
	  it=parameterList->firstChild();
	  while(it!=NULL)
	    {
	      if(it->text(2)==s.data()&&it->text(3)==s1.data())
		{
		  it->setText(2,"");
		  it->setText(3,"");
		}
	      it=it->nextSibling();
	    }
#else
	  for(int i=0;i<parameterList->topLevelItemCount();i++)
	    if(parameterList->topLevelItem(i)->text(2)==s.data()&&
	       parameterList->topLevelItem(i)->text(3)==s1.data())
	      {
		  parameterList->topLevelItem(i)->setText(2,"");
		  parameterList->topLevelItem(i)->setText(3,"");
	      }
	  for(int i=variablesList->topLevelItemCount()-1;i>=0;i--)
	    {
	      if(variablesList->topLevelItem(i)->text(0)==s.data()&&
		 variablesList->topLevelItem(i)->text(1)==s1.data())
		{
		  delete variablesList->takeTopLevelItem(i);
		}
	    }
#endif
	}
    }
}
void ShapeWizard::constructorSelected(int num)
{
  if((int)classesInHeader.size()<=comboBoxCurrentItem(classesBox))
    return;
  if(classesInHeader[comboBoxCurrentItem(classesBox)].nConstructors()<=num)
    return;
  functionNameTypeDoc& constr=classesInHeader[comboBoxCurrentItem(classesBox)].Constructor(num);
  constructorEdit->clear();
  constructorEdit->append(constr.toString().data());
  parameterList->clear();
  bool sameClass=(classesInHeader[comboBoxCurrentItem(classesBox)].Name()==algoWriter.definition().Name());
  bool sameConstructor=(sameClass&&algoWriter.constructorInClass()==num);
  for(int i=constr.nParameters()-1;i>=0;i--)
    {
      string s1=constr.Parameter(i).Name();
      string s2=constr.Parameter(i).Type();
#if QT_VERSION < 0x040000
      QListViewItem *it=new QListViewItem(parameterList,QString(s1.data()),QString(s2.data()));
#else
      QTreeWidgetItem *it=new QTreeWidgetItem(parameterList,QStringList()<<s1.data()<<s2.data());
#endif
      if(sameConstructor)
	{
	  it->setText(2,algoWriter.callParameter(i).data());
	  it->setText(3,s2.data());
	}
    }
  algoWriter.setConstructorInClass(num);
}
