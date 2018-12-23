/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <iostream>
#include <qmessagebox.h> 
#if QT_VERSION < 0x040000
#else
#include <QTextBlock>
#endif
#include "doc_classes.h"
#include "utilities.h"
#include "string_utilities.h"
extern bool existing(string filename);
extern void initKnownClasses(const string & filename);
extern void initKnownTypes(const string &filename);
extern void init_Q_and_T_object();
extern TMyClass* getKnownClass(const string &name);
extern void writeKnownTypes(const string &filename);
extern void addToKnownTypes(const string &classIn,const string &url);
void addToKnownClasses(const TMyClass &classIn);
extern void initKnownFunctions(const string &filename);
extern void writeKnownFunctions(const string &filename);
extern vector<functionNameTypeDoc> knownFunctions;
//bool operator==(const string&s1,const QString&s2){return (s2==s1.data());}
bool operator==(const QString&s1,const string&s2){return (s1==s2.data());}
void documentation_mw::init()
{
#if QT_VERSION < 0x040000
    listView1->setSortColumn(-1);
    statusBar()->message("load classes",1000);
#else
    statusBar()->showMessage("load classes",1000);
#endif
    initKnownClasses("classes.data");
#if QT_VERSION < 0x040000
    statusBar()->message("load lypes",1000);
#else
    statusBar()->showMessage("load lypes",1000);
#endif
    initKnownTypes("types.data");
#if QT_VERSION < 0x040000
    statusBar()->message("init functions",1000);
#else
    statusBar()->showMessage("init functions",1000);
#endif
    init_Q_and_T_object();
#if QT_VERSION < 0x040000
    statusBar()->message("init done",1000);
#else
    statusBar()->showMessage("init done",1000);
#endif
    OnDependencies();
    knowncls=new my_doc_classes();
#if QT_VERSION < 0x040000
    textEdit1->setTextFormat(Qt::PlainText);
#endif
    initKnownFunctions("functions.data");
}
void documentation_mw::destroy()
{
    writeKnownTypes("types.data");
    writeKnownFunctions("functions.data");
    for(unsigned int i=0;i<displayers.size();i++)
    {
	QWidget *tmp=displayers.back();
	displayers.pop_back();
	delete tmp;
    }
    delete knowncls;
}
void documentation_mw::OnFileOpen()
{
    string s=getOpenFileName(this,"Open file",(string(getenv("KTOFPACK"))+"/doc/").data(),"Class description files (*.data)");
    if(s=="")return;
    if(!existing(s))return;
    ifstream input;
    input.open(s.data());
    int ncl=classes.size();
    bool found;
    while(!input.eof()&&input.good())
    {
	TMyClass tmp;
	input>>tmp;
	if(tmp.Name()!="")
	{
	    found=classNumByName(tmp.Name())>=0;
	    if(!found)
	    {
		classes.push_back(tmp);
		files.push_back(s);
		add2ComboBox(comboBox2,tmp.Name().data());
	    }
	}
    }
    input.close();
    OnClassClick();
    s=string("file: ")+s+" opened, "+string_number((long)classes.size()-ncl)+"classes read";
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),2000);
#else
    statusBar()->showMessage(s.data(),2000);
#endif
    OnDependencies();
}
void documentation_mw::OnFileImport()
{
    string s=getOpenFileName(this,"Open file",(string(getenv("KTOFPACK"))+"/include").data(),"C++ Header  files (*.h)");
    if(s=="")return;
    if(!existing(s))return;
    //    cout<<"open file :\"" <<s.data()<<"\""<<endl;
    vector<TMyClass> ret;
    TMyClass::fileContent(1, s, ret);
    for(unsigned int i=0;i<ret.size();i++)
    {
	ret[i].setHeaderFile(s);
	classes.push_back(ret[i]);
	files.push_back("");
	add2ComboBox(comboBox2,ret[i].Name().data());
    }
    OnClassClick();
    s=string("file: ")+s+" imported, "+string_number((int)ret.size())+"classes read";
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),2000);
#else
    statusBar()->showMessage(s.data(),2000);
#endif
    OnDependencies();
}
void documentation_mw::OnFileSaveAs()
{ 
   int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    string s=getSaveFileName (this, "save class description", (string(getenv("KTOFPACK"))+"/doc/").data(), "Class description files (*.data)");
    if(s=="")return;
    if(existing(s))
    {
	if(QMessageBox::warning(this, "file exists", "This file exists already.\nOverwrite?", QMessageBox::Yes, QMessageBox::No ) ==QMessageBox::No)
	    return;
    }
    files[n]=s;
    ofstream out;
    out.open(s.data());
    out<<classes[n];
    out.close();
    s=string("class ")+classes[n].Name()+" saved in file: "+s;
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),2000);
#else
    statusBar()->showMessage(s.data(),2000);
#endif
}
void documentation_mw::OnFileSave()
{
  int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    ofstream out;
    if(files[n]=="")
    {
	OnFileSaveAs();
	return;
    }
    out.open(files[n].data());
    for(unsigned int i=0;i<classes.size();i++)
	if(files[n]==files[i])
	    out<<classes[i];
    out.close();
    string s=string("class ")+classes[n].Name()+" saved in file: "+files[n];
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),2000);
#else
    statusBar()->showMessage(s.data(),2000);
#endif
}
void documentation_mw::OnFileExit()
{
    close();
}
void documentation_mw::OnFileClose()
{
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    if(!existing(files[n])||files[n]=="")
    {
	OnFileSaveAs();	
    }
    else
    {
	int res=QMessageBox::question(this,"save file","Do you want to save this class before closing it?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No,QMessageBox::Cancel|QMessageBox::Escape);
	if(res==QMessageBox::Cancel)
	    return;
	if(res==QMessageBox::Yes)
	    OnFileSave();
    }
    string na=classes[n].Name();
    vector<string> tmpf;
    vector<TMyClass> tmpc;
    while((int)classes.size()>n+1)
    {
	tmpf.push_back(files.back());
	files.pop_back();
	tmpc.push_back(classes.back());
	classes.pop_back();
    }
    classes.pop_back();
    files.pop_back();
    while(!tmpc.empty())
    {
	files.push_back(tmpf.back());
	classes.push_back(tmpc.back());
	tmpf.pop_back();
	tmpc.pop_back();
    }
    comboBox2->removeItem(n);
#if QT_VERSION < 0x040000
    statusBar()->message(QString("class: ")+na.data()+" closed ",2000);
#else
    statusBar()->showMessage(QString("class: ")+na.data()+" closed ",2000);
#endif
    if(classes.size()==0)
    {
	listView1->clear();
	listView2->clear();
	textEdit1->clear();
    }
    else
    {
	setComboBoxCurrentItem(comboBox2,n==0?0:n-1);
	OnClassClick();
    }
 }
void documentation_mw::OnFileOpenDirectory()
{
  string s=getOpenDirectory(this, "get directory to open", "", "");
  if(s=="")return;
  QDir dir(s.data());
  if(!dir.exists())return;
  int nF=0;
  int ncl=0;
  for(int i=0;i<(int)dir.count();i++)
    {
      if((int)QS2s(dir[i]).rfind(".data")!=(int)dir[i].length()-5)continue;
      nF++;
      ifstream input;
      input.open((QS2s(dir.path()+"/"+dir[i])).data());
      bool found;
      while(!input.eof()&&input.good())
	{
	  TMyClass tmp;
	  input>>tmp;
	  if(tmp.Name()!="")
	    {
	      found=classNumByName(tmp.Name())>=0;
	      if(!found)
		{
		  ncl++;
		  classes.push_back(tmp);
		  files.push_back(QS2s(dir.path()+"/"+dir[i]));
		  add2ComboBox(comboBox2,tmp.Name().data());
		}
	    }
	}
      input.close();
    }
  s=string("directory: ")+s+" opened, "+string_number(nF)+" files read, "+string_number(ncl)+"classes read";
#if QT_VERSION < 0x040000
  statusBar()->message(s.data(),5000);
#else
  statusBar()->showMessage(s.data(),5000);
#endif
}
void documentation_mw::OnClassClick()
{
    if(checkBox8->isChecked())return;
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    classShow->clear();
    vector<string> tmp=classes[n].toHeader();
    for(unsigned int i=0;i<tmp.size();i++)classShow->append(tmp[i].data());
    listView1->clear();
#if QT_VERSION < 0x040000
    QListViewItem *ac;
    QListViewItem *it;    
    it=new QListViewItem(listView1,"Signals",(string_number(classes[n].nSignals())).data());
    for(int j=0;j< classes[n].nSignals();j++){
	ac=new QListViewItem(it,classes[n].Signal(j).Name(),classes[n].Signal(j).Type(),"public");
	addParameterToListView(ac,classes[n].Signal(j));
    }
    it=new QListViewItem(listView1,"Slots",(string_number(classes[n].nSlots(0)+classes[n].nSlots(1)+classes[n].nSlots(2))).data());
    for(int i=0;i<3;i++){
      for(int j=0;j< classes[n].nSlots(i);j++){
	ac=new QListViewItem(it,classes[n].Slot(j,i).Name(),classes[n].Slot(j,i).Type(),(i==0?"public":(i==1?"protected":"private")));
	addParameterToListView(ac,classes[n].Slot(j,i));
      }
    }
    it=new QListViewItem(listView1,"Functions",(string_number(classes[n].nMethods(0)+classes[n].nMethods(1)+classes[n].nMethods(2))).data());
    for(int i=0;i<3;i++){
	for(int j=0;j< classes[n].nMethods(i);j++){
	    ac=new QListViewItem(it,classes[n].Method(j,i).Name(),classes[n].Method(j,i).Type(),(i==0?"public":(i==1?"protected":"private")));
	    addParameterToListView(ac,classes[n].Method(j,i));
	}
    }
    it=new QListViewItem(listView1,"Constructors",(string_number(classes[n].nConstructors())).data());
    for(int j=0;j< classes[n].nConstructors();j++){
	ac=new QListViewItem(it,classes[n].Constructor(j).Name(),classes[n].Constructor(j).Type(),"public");
	addParameterToListView(ac,classes[n].Constructor(j));
    }
    it=new QListViewItem(listView1,"Variables",(string_number(classes[n].nVariables(0)+classes[n].nVariables(1)+classes[n].nVariables(2))).data());
    for(int i=0;i<3;i++)
	for(int j=0;j< classes[n].nVariables(i);j++)
	    new QListViewItem(it,classes[n].Variable(j,i).Name(),classes[n].Variable(j,i).Type(),(i==0?"public":(i==1?"protected":"private")),classes[n].Variable(j,i).Default());
    listView2->clear();
    for(int i=0;i<classes[n].nSuperClasses();i++)
    {
	new QListViewItem(listView2,classes[n].SuperClass(i),classes[n].SuperClassDoc(i));
    }
#else
    QStringList visi;visi<<"public"<<"protected"<<"private";
    QTreeWidgetItem *it, *ac;
    it=new QTreeWidgetItem(listView1,QStringList()<<"Signals"<<(string_number(classes[n].nSignals())).data());
    for(int j=0;j< classes[n].nSignals();j++){
      ac=new QTreeWidgetItem(it,QStringList()<<classes[n].Signal(j).Name().data()<<classes[n].Signal(j).Type().data()<<visi[0]);
      addParameterToListView(ac,classes[n].Signal(j));
    }
    it=new QTreeWidgetItem(listView1,QStringList()<<"Slots"<<(string_number(classes[n].nSlots(0)+classes[n].nSlots(1)+classes[n].nSlots(2))).data());
    for(int i=0;i<3;i++){
	for(int j=0;j< classes[n].nSlots(i);j++){
	    ac=new QTreeWidgetItem(it,QStringList()<<classes[n].Slot(j,i).Name().data()<<classes[n].Slot(j,i).Type().data()<<visi[i]);
	    addParameterToListView(ac,classes[n].Slot(j,i));
	}
    }
    it=new QTreeWidgetItem(listView1,QStringList()<<"Functions"<<(string_number(classes[n].nMethods(0)+classes[n].nMethods(1)+classes[n].nMethods(2))).data());
    for(int i=0;i<3;i++){
	for(int j=0;j< classes[n].nMethods(i);j++){
	    ac=new QTreeWidgetItem(it,QStringList()<<classes[n].Method(j,i).Name().data()<<classes[n].Method(j,i).Type().data()<<visi[i]);
	    addParameterToListView(ac,classes[n].Method(j,i));
	}
    }
    it=new QTreeWidgetItem(listView1,QStringList()<<"Constructors"<<(string_number(classes[n].nConstructors())).data());
    for(int j=0;j< classes[n].nConstructors();j++){
	ac=new QTreeWidgetItem(it,QStringList()<<classes[n].Constructor(j).Name().data()<<classes[n].Constructor(j).Type().data()<<visi[0]);
	addParameterToListView(ac,classes[n].Constructor(j));
    }
    it=new QTreeWidgetItem(listView1,QStringList()<<"Variables"<<(string_number(classes[n].nVariables(0)+classes[n].nVariables(1)+classes[n].nVariables(2))).data());
    for(int i=0;i<3;i++)
	for(int j=0;j< classes[n].nVariables(i);j++)
	    new QTreeWidgetItem(it,QStringList()<<classes[n].Variable(j,i).Name().data()<<classes[n].Variable(j,i).Type().data()<<visi[i]<<classes[n].Variable(j,i).Default().data());
    listView2->clear();
    for(int i=0;i<classes[n].nSuperClasses();i++)
    {
	new QTreeWidgetItem(listView2,QStringList()<<classes[n].SuperClass(i).data()<<classes[n].SuperClassDoc(i).data());
    }

#endif
    if(checkBox1->isChecked())
    {
	textEdit1->clear();
	for(int i=0;i<classes[n].DescriptionLines();i++)
	    textEdit1->append(classes[n].Description(i).data());
    }
 }
#if QT_VERSION < 0x040000
void documentation_mw::addParameterToListView(QListViewItem *it, functionNameTypeDoc &var)
{
    for(int k=var.nParameters()-1;k>=0;k--)
	new QListViewItem(it,var.Parameter(k).Name(),var.Parameter(k).Type(),"", var.Parameter(k).Default());
}
#else
void documentation_mw::addParameterToListView(QTreeWidgetItem *it, functionNameTypeDoc &var)
{
  for(int k=var.nParameters()-1;k>=0;k--)
    new QTreeWidgetItem(it,QStringList()<<var.Parameter(k).Name().data()<<var.Parameter(k).Type().data()<<""<< var.Parameter(k).Default().data());
}
#endif
void documentation_mw::OnEditUpdate()
{    
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    QMessageBox::information(this,"Check header",(string("Check header-file \"")+classes[n].HeaderFile()).data(),QMessageBox::Ok);
    vector<TMyClass> ret;
    TMyClass::fileContent(1, classes[n].HeaderFile(), ret);
    int m=-1;
    for(unsigned int i=0;i<ret.size();i++)
      if(classes[n].Name()==ret[i].Name())m=i;
    if(m<0)
      {
	QMessageBox::critical(this,"Class update","Update not possible. Class not found in header.", QMessageBox::Abort, 0);
	return;
      }
    doc_update_dialog fenst(this);
    fenst.setOldClass(&classes[n]);
    fenst.setNewClass(&ret[m]);
    if(!fenst.classesMatch())
      fenst.exec();
    if(fenst.result()==QDialog::Accepted)
      {
	ret[m].setHeaderFile(classes[n].HeaderFile());
	classes[n]=ret[m];
	OnClassClick();
      }
}
void documentation_mw::OnRemoveItem()
{
#if QT_VERSION < 0x040000
	QListViewItem *it=listView1->selectedItem();
#else
	QTreeWidgetItem *it=listView1->currentItem();
#endif
    if(!checkBox8->isChecked())
    {
	if(it==NULL)return;
	int n=classNumByName(comboBox2->currentText());
	if(n==-1)return;
	nameTypeDoc *property=getProperty(it);
	if(property==NULL)return;
	bool removeIt=false;
	QString tp;
	int whicht=-1;
	    int m=-1,l=-1;
	if(hasA(property->Doc(),"#f")||hasA(property->Doc(),"#l")||hasA(property->Doc(),"#s"))
	{
	  whicht=0;
	    for(int i=0;i<7;i++)
		for(int j=0;j<classes[n].nMethods(i);j++)
		    if(classes[n].Method(j,i)==*((functionNameTypeDoc*)property))
		    {m=j;l=i;}
	    if(m==-1||l==-1)return;
	    if(l==0||l==1||l==2)tp="method";
	    else if(l==3||l==4||l==5)tp="slot";
	    else tp="signal";
	}
	else if(hasA(property->Doc(),"#c"))
	{
	  whicht=1;
	    for(int j=0;j<classes[n].nConstructors();j++)
		if(classes[n].Constructor(j)==*((functionNameTypeDoc*)property))
		{m=j;}
	    tp="constructor";
	    if(m==-1)return;
	}
	else if(hasA(property->Doc(),"#v"))
	{
	  whicht=2;
	    for(int i=0;i<7;i++)
		for(int j=0;j<classes[n].nVariables(i);j++)
		    if(classes[n].Variable(j,i)==*((functionNameTypeDoc*)property))
		    {m=j;l=i;}
	    if(m==-1||l==-1)return;
	    tp="varable";
	}
	if(whicht>=0)
	  {
	    int ret=QMessageBox::question(this, QString("Remove ")+tp, QString("Remove ")+tp+"\""+property->toString().data()+"\"?", QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	    if(ret==QMessageBox::Yes)
	      {
		switch(whicht)
		  {
		  case 0:classes[n].removeMethod(l,m);break;
		  case 1:classes[n].removeConstructor(m);break;
		  case 2:classes[n].removeVariable(l,m);break;
		  }    
		removeIt=true;
#if QT_VERSION < 0x040000
		listView1->takeItem(it);
		delete it;
#else
		delete it->parent()->takeChild(it->parent()->indexOfChild(it));
#endif
	      }
	  }
    }
    else
    {
	nameTypeDoc *property=getProperty(it);
	if(property==NULL)return;
	vector<functionNameTypeDoc> tmpV;
	while(!knownFunctions.empty())
	{
	    if(!(knownFunctions.back()==*((functionNameTypeDoc*)property)))
		tmpV.push_back(knownFunctions.back());
	    knownFunctions.pop_back();
	}
	while(!tmpV.empty())
	{
	    knownFunctions.push_back(tmpV.back());
	    tmpV.pop_back();
	}
	if(it->parent())
	  {
#if QT_VERSION < 0x040000
	    it->parent()->takeItem(it);
	    delete it;
#else
	    delete it->parent()->takeChild(it->parent()->indexOfChild(it));
#endif
	  }
    }
}
void documentation_mw::OnEditHTML()
{
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    string s=getSaveFileName (this, "Write html", (string(getenv("KTOFPACK"))+"/doc/").data(), "Web files (*.html)");
    if(s=="")return;
    classes[n].setDoc(s.substr(s.rfind("/")+1,s.length()-s.rfind("/")-1));
    ofstream out;
    out.open(s.data());
    vector<string> ret=classes[n].toHTML(associatedFunctions(classes[n].Name()));
    for(unsigned int i=0;i<ret.size();i++)
	out<<ret[i].data()<<endl;
    addToKnownTypes(classes[n].Name(),classes[n].Doc());
    out.close();
    int num=-1;
    for(unsigned int i=0;i<displayers.size();i++)
	if(((my_doc_display*)displayers[i])->getClass()==&classes[n])
	    num=i;
    if(num==-1)
    {
	displayers.push_back(new my_doc_display());
	((my_doc_display*)displayers.back())->showText(ret,&classes[n]);
	num=displayers.size()-1;
    }
    else
    {
	((my_doc_display*)displayers[num])->showText(ret,&classes[n]);
    }
    displayers[num]->show();
    s=string("HTML document generated for class ")+classes[n].Name()+" in file: "+s;
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),2000);
#else
    statusBar()->showMessage(s.data(),2000);
#endif
}
void documentation_mw::OnEditHTMLAll()
{
    vector<string> ucls;
    vector<int>index;
    doc_selectClassesDialog fenst(this);
    fenst.setClasses(classes);
    fenst.exec();
    if(fenst.result()==QDialog::Accepted)
    {
	fenst.getClasses(ucls);
	for(unsigned int j=0;j<ucls.size();j++)
	{
	    for(unsigned int i=0;i<classes.size();i++)
		if(classes[i].Name()==ucls[j])
		{
		    index.push_back(i);
		    break;
		}
	}
    }
    else return;
    vector<string> htmlFiles;
    for(unsigned int i=0;i<index.size();i++)
    {
	string s=getSaveFileName ( this,string("Write html for class ")+classes[index[i]].Name(),string(getenv("KTOFPACK"))+"/html/", "Web files (*.html)");
	if(s!="")
	    htmlFiles.push_back(s);
	else
	    htmlFiles.push_back(classes[index[i]].Doc());
	classes[index[i]].setDoc(s.substr(s.rfind("/")+1,s.length()-s.rfind("/")-1));
    }
    OnDependencies();
    for(unsigned int i=0;i<index.size();i++)
    {
	ofstream out;
	out.open(htmlFiles[i].data());
	vector<string> ret=classes[index[i]].toHTML(associatedFunctions(classes[index[i]].Name()));
	for(unsigned int j=0;j<ret.size();j++)
	    out<<ret[j].data()<<endl;
	addToKnownTypes(classes[index[i]].Name(),classes[index[i]].Doc());
	out.close();
	int num=-1;
	for(unsigned int j=0;j<displayers.size();j++)
	    if(((my_doc_display*)displayers[j])->getClass()==&classes[index[i]])
		num=j;
	if(num==-1)
	{
	    displayers.push_back(new my_doc_display());
	    ((my_doc_display*)displayers.back())->showText(ret,&classes[index[i]]);
	    num=displayers.size()-1;
	}
	else
	{
	    ((my_doc_display*)displayers[num])->showText(ret,&classes[index[i]]);
	}
	displayers[num]->show();
    }
    string s=string("HTML document generated for ")+string_number((long)index.size())+(index.size()>1?" classes ":" class ");
#if QT_VERSION < 0x040000
    statusBar()->message(s.data(),3000);
#else
    statusBar()->showMessage(s.data(),3000);
#endif
}
vector<functionNameTypeDoc> documentation_mw::associatedFunctions(const string&classname)
{
    vector<functionNameTypeDoc> ret;
    string tmp;
    for(unsigned int i=0;i<knownFunctions.size();i++)
    {
	tmp=knownFunctions[i].Type();
	tmp=remove(tmp,'*');
	tmp=remove(tmp,'&');
	tmp=remove(tmp,'[');
	tmp=remove(tmp,']');
	if(classname==tmp.data())
	{
	    ret.push_back(knownFunctions[i]);
	}
	else
	{
	    for(int j=0;j<knownFunctions[i].nParameters();j++)
	    {
		tmp=knownFunctions[i].Parameter(j).Type();
		tmp=remove(remove(remove(remove(tmp,'['),']'),'&'),'*');
/* 		tmp.remove("&"); */
/* 		tmp.remove("["); */
/* 		tmp.remove("]"); */
		if(classname==tmp.data())
		{
		    ret.push_back(knownFunctions[i]);
		    break;
		}
	    }
	}
    }
    return ret;
}
void documentation_mw::OnDependencies()
{
    for(unsigned int i=0;i<classes.size();i++)
	addToKnownClasses(classes[i]);
    for(unsigned int i=0;i<classes.size();i++)
    {
	for(int j=0;j<classes[i].nSuperClasses();j++)
	{
	    if(classes[i].Super(j)==NULL&&getKnownClass(classes[i].SuperClass(j))!=NULL)
	    {
		int tmpi=classes[i].InherritanceType(j);
		string cn=classes[i].SuperClass(j);
		string cd=classes[i].SuperClassDoc(j);
		classes[i].removeSuperClass(cn);
		classes[i].addSuperClass(cn,cd,tmpi,getKnownClass(classes[i].SuperClass(j)));
	    }
	}
    }
    for(unsigned int i=0;i<classes.size();i++)
	addToKnownClasses(classes[i]);
#if QT_VERSION < 0x040000
    statusBar()->message("Super class dependencies checked",2000);
#else
    statusBar()->showMessage("Super class dependencies checked",2000);
#endif
}
void documentation_mw::OnEditNextItem()
{
#if QT_VERSION < 0x040000
    QListViewItem *it=listView1->selectedItem();
    if(it==NULL)return;
    if(it->parent()!=NULL||checkBox1->isChecked())
      OnApplyButtonClick();
    if(checkBox1->isChecked())
      checkBox1->setChecked(false);
    if(it->parent()==NULL)
    {
	while(it->firstChild()==NULL&&it->nextSibling()!=NULL)
	    it=it->nextSibling();
	if(it->firstChild()==NULL)return;
	setItemOn(it->firstChild());
    }
    else
    {
	while(it->parent()->parent()!=NULL)
	    it=it->parent();
	it->setOpen(false);
	if(it->nextSibling()!=NULL)
	  setItemOn(it->nextSibling());
	else
	{
	    it->setOpen(false);
	    it=it->parent();
	    if(it->nextSibling()==NULL)
		it=listView1->firstChild();
	    else
		it=it->nextSibling();
	    while(it->firstChild()==NULL&&it->nextSibling()!=NULL)
		it=it->nextSibling();
	    if(it->firstChild()==NULL)
	    {
		it=listView1->firstChild();
		while(it->firstChild()==NULL&&it->nextSibling()!=NULL)
		    it=it->nextSibling();
		if(it->firstChild()==NULL)
		    return;
	    }
	    setItemOn(it->firstChild());
	}
    }
#else
    QTreeWidgetItem *it=listView1->currentItem();
    if(it==NULL)return;
    int cathe=0;int num=0;
    int ind=listView1->indexOfTopLevelItem(it);
    if(ind>=0||checkBox1->isChecked())
      OnApplyButtonClick();
    if(checkBox1->isChecked())
      checkBox1->setChecked(false);
    if(ind>=0)
      {
	cathe=ind;
	num=0;
	int iter=0;
	while(listView1->topLevelItem(cathe)->childCount()==0&&iter<1)
	  {
	    while(listView1->topLevelItem(cathe)->childCount()==0&&cathe<listView1->topLevelItemCount()-1)
	      cathe++;
	    if(listView1->topLevelItem(cathe)->childCount()==0)cathe=0;
	  }
	if(listView1->topLevelItem(cathe)->childCount()==0)return;
	if(cathe!=ind) listView1->collapseItem(it);
      }
    else
      {
	while(listView1->indexOfTopLevelItem(it->parent())<0)it=it->parent();
	listView1->collapseItem(it);
	cathe=listView1->indexOfTopLevelItem(it->parent());
	num=it->parent()->indexOfChild(it);
	if(num<it->parent()->childCount())num++;
	else
	  {
	    num=0;
	    int iter=0;
	    while(listView1->topLevelItem(cathe)->childCount()==0&&iter<1)
	      {
		while(listView1->topLevelItem(cathe)->childCount()==0&&cathe<listView1->topLevelItemCount()-1)
		  cathe++;
		if(listView1->topLevelItem(cathe)->childCount()==0)cathe=0;
	      }
	    if(listView1->topLevelItem(cathe)->childCount()==0)return;
	  }
      }
    setItemOn(listView1->topLevelItem(cathe)->child(num));
#endif
}
#if QT_VERSION < 0x040000
void documentation_mw::setItemOn(QListViewItem *it)
{
  listView1->setSelected(it,true);
  it->setOpen(true);
  listView1->ensureItemVisible(it);
  if(it->firstChild()!=NULL)
    {
      it=it->firstChild();
      while(it->nextSibling()!=NULL)
	it=it->nextSibling();
      listView1->ensureItemVisible(it);
    }
#else
void documentation_mw::setItemOn(QTreeWidgetItem *it)
{
  listView1->setCurrentItem(it);
  listView1->expandItem(it);
#endif
  OnPropertyClick(it);
}
#if QT_VERSION < 0x040000
void documentation_mw::OnPropertyRightClick(QListViewItem *it)
#else
void documentation_mw::OnPropertyRightClick(QTreeWidgetItem *it)
#endif
{
     if(it==NULL)return;
    nameTypeDoc *tmp=getProperty(it);
    if(tmp==NULL)return;
    if(it->text(0)==tmp->Name().data())
    {
	string s;
	s=string("Name: ")+tmp->Name()+"\n";
	s+=string("Type: ")+tmp->Type()+"\n";
	if(tmp->Doc()!="")s+=string("Doc: ")+tmp->Doc()+"\n";
	if(tmp->TypeDoc()!="")s+=string("TypeDoc: ")+tmp->TypeDoc()+"\n";
	if(tmp->Specifier()!="")s+=string("Specifier: ")+tmp->Specifier()+"\n";
	if(tmp->Default()!="")s+=string("Default: ")+tmp->Default();
	QMessageBox::information(this,"Info",s.data(),QMessageBox::Ok);
    }
    else
    {
	int n=-1;
	for(int i=0;i<((functionNameTypeDoc*)tmp)->nParameters();i++)
	    if(it->text(0)==((functionNameTypeDoc*)tmp)->Parameter(i).Name().data())
		n=i;
	if(n==-1)
	    return;
	tmp=&(((functionNameTypeDoc*)tmp)->Parameter(n));
	string s;
	s=string("Name: ")+tmp->Name()+"\n";
	s+=string("Type: ")+tmp->Type()+"\n";
	if(tmp->Doc()!="")s+=string("Doc: ")+tmp->Doc()+"\n";
	if(tmp->TypeDoc()!="")s+=string("TypeDoc: ")+tmp->TypeDoc()+"\n";
	if(tmp->Specifier()!="")s+=string("Specifier: ")+tmp->Specifier()+"\n";
	if(tmp->Default()!="")s+=string("Default: ")+tmp->Default();
	QMessageBox::information(this,"Info",s.data(),QMessageBox::Ok);
    }
}
#if QT_VERSION < 0x040000
void documentation_mw::OnPropertyClick(QListViewItem *it)
#else
void documentation_mw::OnPropertyClick(QTreeWidgetItem *it)
#endif
{
    if(checkBox8->isChecked())
    {
	nameTypeDoc *tmp=getProperty(it);
	if(tmp==NULL)return;
	textEdit1->clear();
	for(int i=0;i<tmp->DescriptionLines();i++)
	    textEdit1->append(tmp->Description(i).data());
	return;
    }
    else
    {
	if(checkBox1->isChecked())return;
	if(it==NULL)return;
	nameTypeDoc *tmp=getProperty(it);
	if(tmp==NULL)return;
	textEdit1->clear();
	for(int i=0;i<tmp->DescriptionLines();i++)
	    textEdit1->append(tmp->Description(i).data());
	return;
    }
}
vector<string> getTextOfTextEdit(QTextEdit* textEdit)
{
  vector<string> ret;
  string tmp;
#if QT_VERSION < 0x040000
  for(int i=0;i<textEdit->paragraphs();i++)
    {
      tmp=QS2s(textEdit->text(i));
      ret.push_back(tmp);
    }
#else
  QTextDocument* doc=textEdit->document();
  if(doc==NULL)return ret;
  for(int i=0;i<doc->blockCount();i++)
    ret.push_back(QS2s(doc->findBlock(i).text()));
#endif
  return ret;
}
void documentation_mw::OnApplyButtonClick()
{
    if(checkBox8->isChecked())
    {
	nameTypeDoc *tmp1=getProperty(listView1->currentItem());
	if(tmp1==NULL)return;
	tmp1->clearDescription();
	vector<string> tmp=getTextOfTextEdit(textEdit1);
	for(unsigned int i=0;i<tmp.size();i++)
	  tmp1->addDescriptionLine(tmp[i].substr(0,tmp[i].length()-1));
    }
    else
    {
	if(checkBox1->isChecked())
	{
	    int n=classNumByName(comboBox2->currentText());
	    if(n==-1)return;
	    classes[n].clearDescription();
	    vector<string> tmp=getTextOfTextEdit(textEdit1);
	    for(unsigned int i=0;i<tmp.size();i++)
	      classes[n].addDescriptionLine(tmp[i].substr(0,tmp[i].length()-1));
	}
	else
	{
	    nameTypeDoc *tmp1=getProperty(listView1->currentItem());
	    if(tmp1==NULL)return;
	    tmp1->clearDescription();
	    vector<string> tmp=getTextOfTextEdit(textEdit1);
	    for(unsigned int i=0;i<tmp.size();i++)
	      tmp1->addDescriptionLine(tmp[i].substr(0,tmp[i].length()-1));
	}
    }
#if QT_VERSION < 0x040000
    statusBar()->message("Description added",2000);
#else
    statusBar()->showMessage("Description added",2000);
#endif
}
void documentation_mw::OnClassDescriptionCheck()
{
    if(checkBox8->isChecked())return;
    if(checkBox1->isChecked())
    {
	int n=classNumByName(comboBox2->currentText());
	if(n==-1)return;
	textEdit1->clear();
	for(int i=0;i<classes[n].DescriptionLines();i++)
	    textEdit1->append(classes[n].Description(i).data());
    }
    else
    {
	OnPropertyClick(listView1->currentItem());
    }
}
#if QT_VERSION < 0x040000
bool documentation_mw::checkWithProperty(QListViewItem *it, functionNameTypeDoc* prp)
#else
bool documentation_mw::checkWithProperty(QTreeWidgetItem *it, functionNameTypeDoc* prp)
#endif
{
  if(it==NULL||prp==NULL)return false;
  if(it->text(0)!=prp->Name().data()||prp->nParameters()!=it->childCount())return false;
#if QT_VERSION < 0x040000
  if(prp->nParameters()==0)return true;
  QListViewItem *ac=it->firstChild();
  if(ac->text(1)!=prp->Parameter(0).Type().data())return false;
  for(int k=1;k<prp->nParameters();k++)
    {
      ac=ac->nextSibling();
      if(ac==NULL)break;
      if(ac->text(1)!=prp->Parameter(k).Type().data())
	{ac=NULL;break;}
    }
  if(ac!=NULL)
    return true;
  return false;
#else
  for(int j=0;j<it->childCount();j++)
    if(it->child(j)->text(1)!=prp->Parameter(j).Type().data())
      {return false;}
  return true;
#endif
}

#if QT_VERSION < 0x040000
nameTypeDoc *documentation_mw::getProperty(QListViewItem *it)
#else
nameTypeDoc *documentation_mw::getProperty(QTreeWidgetItem *it)
#endif
{
    if(it==NULL)return NULL;
    if(checkBox8->isChecked())
    {
      if(it->parent()==NULL)return NULL;
      while(it->parent()->parent()!=NULL)it=it->parent();
      for(unsigned int i=0;i<knownFunctions.size();i++)
	if(checkWithProperty(it,&knownFunctions[i]))
	  return &knownFunctions[i];
      return NULL;
    }
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return NULL;
    if(it->parent()==NULL)return NULL;
    while(it->parent()->parent()!=NULL)it=it->parent();
    if(it->parent()->text(0)=="Variables")
    {
	for(int j=0;j<3;j++)
	    for(int i=0;i<classes[n].nVariables(j);i++)
		if(it->text(0)==classes[n].Variable(i,j).Name().data()&&it->text(1)==classes[n].Variable(i,j).Type().data())
		    return &classes[n].Variable(i,j);
    }
    else if(it->parent()->text(0)=="Functions")
    {
	for(int j=0;j<3;j++)
	{
	    for(int i=0;i<classes[n].nMethods(j);i++)
	    {
	      if(checkWithProperty(it,&classes[n].Method(i,j)))
		return &classes[n].Method(i,j);
	    }
	}
    }
    else if(it->parent()->text(0)=="Constructors")
    {
	for(int i=0;i<classes[n].nConstructors();i++)
	{
	  if(checkWithProperty(it,&classes[n].Constructor(i)))
	    return &classes[n].Constructor(i);
	}
    }
    else if(it->parent()->text(0)=="Signals")
    {
	for(int i=0;i<classes[n].nSignals();i++)
	{
	  if(checkWithProperty(it,&classes[n].Signal(i)))
	    return &classes[n].Signal(i);
	}
    }
    else if(it->parent()->text(0)=="Slots")
    {
	for(int j=0;j<3;j++)
	{
	    for(int i=0;i<classes[n].nSlots(j);i++)
	    {
	      if(checkWithProperty(it,&classes[n].Slot(i,j)))
		return &classes[n].Slot(i,j);
	    }
	}
    }
    return NULL;
}
#if QT_VERSION < 0x040000
void documentation_mw::OnSuperClassClick(QListViewItem *it)
#else
void documentation_mw::OnSuperClassClick(QTreeWidgetItem *it)
#endif
{
    if(checkBox8->isChecked())return;
    if(it==NULL)return;
    int n=classNumByName(comboBox2->currentText());
    if(n==-1)return;
    int m=-1;
    for(int i=0;i<classes[n].nSuperClasses();i++)
	if(it->text(0)==classes[n].SuperClass(i).data())
	    m=i;
    if(m==-1)return;
    string line="Superclass: ";
    line+=classes[n].SuperClass(m)+"\n\n";
    if(classes[n].SuperClassDoc(m)!="")
	line+=string("Documented in: ")+classes[n].SuperClassDoc(m)+"\n";
    if(classes[n].Super(m)!=NULL)
	line+="\tcurrently defined\n";
    else
	if(getKnownClass(classes[n].SuperClass(m))!=NULL)
	    line+="known but not assigned\nDo menu-Edit-Dependencies\n";
    QMessageBox::information(this,"super class info",line.data(),QMessageBox::Ok);
}
#if QT_VERSION < 0x040000
void documentation_mw::OnSuperClassRightClick(QListViewItem *it)
#else
void documentation_mw::OnSuperClassRightClick(QTreeWidgetItem *it)
#endif
{
    if(checkBox8->isChecked())return;
    if(it==NULL)return;
    string s=QStringInputDialog::getLine(this, (string("Class ")+QS2s(it->text(0))+" documented in").data());
    if(s!="")it->setText(1,s.data());
}
void documentation_mw::OnHelpKnownClasses()
{
    knowncls->showClasses();
    knowncls->show();
}
void documentation_mw::OnHTMLitalic()
{
    int p1,p2,i1,i2;
    textEdit1->getSelection (&p1, &i1, &p2, &i2);
    if(p1==p2&&p1==-1&&i1==i2&&i1==-1)
    {
	textEdit1->getCursorPosition ( &p1, &i1 );
	textEdit1->insertAt("<i></i>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+3);
    }
    else
    {
	textEdit1->insertAt("</i>",p2,i2);
	textEdit1->insertAt("<i>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+3);
    }
}
void documentation_mw::OnHTMLbold()
{
    int p1,p2,i1,i2;
    textEdit1->getSelection (&p1, &i1, &p2, &i2);
    if(p1==p2&&p1==-1&&i1==i2&&i1==-1)
    {
	textEdit1->getCursorPosition ( &p1, &i1 );
	textEdit1->insertAt("<b></b>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+3);
    }
    else
    {
	textEdit1->insertAt("</b>",p2,i2);
	textEdit1->insertAt("<b>",p1,i1);
	textEdit1->setCursorPosition(p2,i2+3);
    }
}
void documentation_mw::OnHTMLvar()
{
    int p1,p2,i1,i2;
    textEdit1->getSelection (&p1, &i1, &p2, &i2);
    if(p1==p2&&p1==-1&&i1==i2&&i1==-1)
    {
	textEdit1->getCursorPosition ( &p1, &i1 );
	textEdit1->insertAt("<var></var>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+5);
    }
    else
    {
	textEdit1->insertAt("</var>",p2,i2);
	textEdit1->insertAt("<var>",p1,i1);
	textEdit1->setCursorPosition(p2,i2+5);
    }
}
void documentation_mw::OnHTMLsuper()
{
    int p1,p2,i1,i2;
    textEdit1->getSelection (&p1, &i1, &p2, &i2);
    if(p1==p2&&p1==-1&&i1==i2&&i1==-1)
    {
	textEdit1->getCursorPosition ( &p1, &i1 );
	textEdit1->insertAt("<sup></sup>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+5);
    }
    else
    {
	textEdit1->insertAt("</sup>",p2,i2);
	textEdit1->insertAt("<sup>",p1,i1);
	textEdit1->setCursorPosition(p2,i2+5);
    }
}
void documentation_mw::OnHTMLsub()
{
    int p1,p2,i1,i2;
    textEdit1->getSelection (&p1, &i1, &p2, &i2);
    if(p1==p2&&p1==-1&&i1==i2&&i1==-1)
    {
	textEdit1->getCursorPosition ( &p1, &i1 );
	textEdit1->insertAt("<sub></sub>",p1,i1);
	textEdit1->setCursorPosition(p1,i1+5);
    }
    else
    {
	textEdit1->insertAt("</sub>",p2,i2);
	textEdit1->insertAt("<sub>",p1,i1);
	textEdit1->setCursorPosition(p2,i2+5);
    }
}
void documentation_mw::OnQt3Des()
{
  int p1,i1;
  textEdit1->getCursorPosition ( &p1, &i1 );
  textEdit1->insertAt("Qt3-mode; non-visible in Qt4-mode",p1,i1);
  textEdit1->setCursorPosition(p1,i1+33);
}
void documentation_mw::OnQt4Des()
{
  int p1,i1;
  textEdit1->getCursorPosition ( &p1, &i1 );
  textEdit1->insertAt("Qt4-mode; non-visible in Qt3-mode",p1,i1);
  textEdit1->setCursorPosition(p1,i1+33);
}
void documentation_mw::OnKnownFunctionsClick()
{
    if(checkBox8->isChecked())
    {
	textEdit1->clear();
	listView1->clear();
	QListViewItem *it=new QListViewItem(listView1,"non member functions");
	for(unsigned int i=0;i<knownFunctions.size();i++)
	{
	    addParameterToListView(new QListViewItem(it,knownFunctions[i].Name(),knownFunctions[i].Type(),"",knownFunctions[i].Default()),knownFunctions[i]);
	}
    }
    else
    {
	OnClassClick();
    }
}
int documentation_mw::classNumByName(const string &nme)const
{
  for(unsigned int i=0;i<classes.size();i++)
    if(classes[i].Name()==nme)
      return i;
  return -1;
}
int documentation_mw::classNumByName(const QString &nme)const
{
  for(unsigned int i=0;i<classes.size();i++)
    if(nme==classes[i].Name().data())
      return i;
  return -1;
}
void documentation_mw::OnClassShow()
{
  classShow->show();
}
