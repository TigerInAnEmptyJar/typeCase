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
string reactionRecognitionFile;
#include "string_utilities.h"
#include "parameterManager.h"
#include "qsinglewidgetdialog.h"
#include "rundisplay.h"
#include "shapedisplay.h"
#include "detectordisplay.h"
#include "algorithmdisplay.h"
#include "algorithmwriter.h"
#include "algorithmwizard.h"
#include "shapewriter.h"
#include "shapewizard.h"
extern bool existing(string filename);
void typeCaseMW::init()
{
  parameterManager::init();
  Setup=new SetupWidget(NULL);
  Dataw=new DataBasisWidget(NULL);
  Analysis=new AnalysisWidget(NULL);
  Algorithm=new AlgorithmWidget(NULL);
  Help=new HelpWidget(NULL);

  //    View=new ViewWidget(NULL);

  connect(Algorithm,SIGNAL(algorithmSelected(algorithm_parameter&,bool)),Analysis,SLOT(algorithmSelected(algorithm_parameter&,bool))); 
  connect(this,SIGNAL(selectAlgorithm(algorithm_parameter&,bool)),Analysis,SLOT(algorithmSelected(algorithm_parameter&,bool))); 
  connect(this,SIGNAL(selectAlgorithm(algorithm_parameter&,bool)),Algorithm,SLOT(algorithmSelect(algorithm_parameter&,bool))); 
  connect(Analysis,SIGNAL(algorithmSelect(algorithm_parameter&, bool)),Algorithm,SLOT(algorithmSelect(algorithm_parameter&, bool)));
  connect(Dataw,SIGNAL(runSelected(run_parameter*, bool)),Analysis,SLOT(runSelected(run_parameter*,bool)));
  connect(Analysis,SIGNAL(analysisParameterWritten(const string &)),this,SLOT(onAnalysisParameterWritten(const string &)));
  connect(Analysis,SIGNAL(requestRun(const string&,bool)),Dataw,SLOT(runSelect(const string &, bool)));
/*   connect(Setup,SIGNAL(helpAsked(const string&)),Help,SLOT(setHelp(const string&)));  */
/*   connect(Dataw,SIGNAL(helpAsked(const string&)),Help,SLOT(setHelp(const string&))); */
/*   connect(Algorithm,SIGNAL(helpAsked(const string&)),Help,SLOT(setHelp(const string&))); */
  connect(Analysis,SIGNAL(helpRequested(const string&)),Help,SLOT(setHelp(const string&))); 
    QDir cur=QDir::current();
    if(!existing(QS2s(typeCaseHome.path())))
    {
      prepareFirstRun(typeCaseHome.path());
      writeInitFile();
    }
    ifstream input;
    input.open(QS2s(typeCaseHome.path()+"/initFile.data").data());
    char li[1000];
    vector<string> tt;
    for(int i=0;i<7;i++)
    {
	input.getline(li,1000);
	tt.push_back(string(li));
    }
    input.getline(li,1000);
    calibrationDir=QDir(QString(li));
    input.getline(li,1000);
    dataDir=QDir(QString(li));
    input.getline(li,1000);
    tt.push_back(string(li));
    input.getline(li,1000);
    tt.push_back(string(li));
    input.close();
    string ddir=QS2s(dataDir.path());
    string ktdir=QS2s(ktofDir.path());
    for(unsigned int i=0;i<tt.size();i++)
    {	
      if(tt[i].find("$(DATA)")<tt[i].npos)
	filenames.push_back(replace(tt[i],"$(DATA)",ddir));
      else if(tt[i].find("$(KTOFPACK)")<tt[i].npos)
	filenames.push_back(replace(tt[i],"$(KTOFPACK)",ktdir));
      else
	filenames.push_back(tt[i]);
    }
    reactionRecognitionFile=filenames[6];
    cout<<"reading: data ... "<<flush;
    parameterManager::readDataBasis_ascii(filenames[0],beamtimes,runs);
    //  cout<<"analysisParameter ... "<<flush;
    cout<<"shapes ... "<<flush;
    parameterManager::readShapeParameter_ascii(filenames[2],default_shapes);
    cout<<"detectors ... "<<flush; 
    parameterManager::readDetectorParameter_ascii(filenames[3],detectors,experiment);
    parameterManager::readMaterialParameter_ascii(filenames[7],materials);
    cout<<"algorithms ... "<<flush;
    parameterManager::readAlgorithmParameter_ascii(filenames[4],algorithms);
    setRunTypes(filenames[8]);
    cout<<"done "<<endl;
    Setup->setSetup(detectors,materials,default_shapes,experiment);
    Dataw->setDataBasis(beamtimes,runs);
    Algorithm->setAlgorithms(algorithms);
    for(unsigned int i=0;i<algorithms.size();i++)
      if(algorithms[i].IsUsed())
	emit selectAlgorithm(algorithms[i],true);
    Analysis->readAnalysisParameter(filenames[1]); 
}

void typeCaseMW::destroy()
{
  delete Setup; 
  delete Dataw;
  delete Analysis; 
  delete Algorithm;
  delete Help; 
/*    delete View; */
}
void typeCaseMW::closeEvent(QCloseEvent *e)
{
  QWidget::closeEvent(e);
  Setup->close(); 
  Dataw->close();
  Analysis->close(); 
  Algorithm->close();
  Help->close(); 
/*     View->close(); */
  close();
}
void typeCaseMW::prepareFirstRun(QString typecaseHome)
{
    QDir::home().mkdir(".typeCase");
    vector<string> filenam;
    filenam.push_back("beamTimes.data");
    filenam.push_back("paintParams.data");
    filenam.push_back("shapes.data");
    filenam.push_back("detectors.data");
    filenam.push_back("algorithms.data");
    filenam.push_back("stacktypes.data");
    filenam.push_back("demo.data");
    filenam.push_back("materials.data");
    filenam.push_back("runTypes.data");
    for(unsigned int i=0;i<filenam.size();i++)
      filenames.push_back(QS2s(typeCaseHome.path())+"/files/"+filenam[i]);
    calibrationDir=typeCaseHome.path()+"/files/Data";
    dataDir=typecaseHome+"/files";
    writeInitFile();
    typeCaseHome.mkdir("files");
    QDir fd=typeCaseHome.path()+"/files";
    fd.mkdir("Data");
    QDir dd=fd.path()+"/Data";
    string pathfrom=QS2s(typeCaseBase.path());
    string pathto=QS2s(typeCaseHome.path());
    for(unsigned int i=0;i<filenames.size();i++)
    {
      if(!existing(pathfrom+"/files/Data/"+filenam[i]))
	cout<<"notFound!!! \""<<(pathfrom+"/files/Data/"+filenam[i]+"\" to \""
			  +pathto+"/files/"+filenam[i]).data()<<"\""<<endl;
      int ret;
      ret=system((string("cp ")+pathfrom+"/files/Data/"+filenam[i]+" "
	      +pathto+"/files/"+filenam[i]).data());
/* #if QT_VERSION < 0x040000 */
/*       QProcess copier(QString("cp")); */
/* 	copier.addArgument(typeCaseBase.path()+"/files/Data/"+filenam[i].data()); */
/* 	copier.addArgument(typeCaseHome.path()+"/files/"+filenam[i].data()); */
/* 	copier.start(); */
/* 	while(copier.isRunning()){} */
/* 	copier.clearArguments(); */
/* #else */
/* 	QProcess copier; */
/* 	copier.start(QString("cp ") */
/* 		     +typeCaseBase.path()+"/files/Data/"+filenam[i].data()+" " */
/* 		     +typeCaseHome.path()+"/files/"+filenam[i].data()); */
/* 	while(copier.waitForFinished()){} */
/* #endif */
    }
}
void typeCaseMW::OnDataBasisShow()
{
    Dataw->show();
}
void typeCaseMW::OnSetupShow()
{
    Setup->resize(600,400);
    Setup->show(); 
}
void typeCaseMW::OnAnalysisShow()
{
  Analysis->show(); 
}
void typeCaseMW::OnAlgorithmsShow()
{
    Algorithm->show();
}
void typeCaseMW::OnViewShow()
{
/*     View->show(); */
}
void typeCaseMW::OnHelpShow()
{
  Help->setHelp((string(getenv("HOME"))+"/WWW/KTOF/index.html").data());
  Help->show();
}
extern bool existing(string filename);
void typeCaseMW::OnLoadData()
{
  string p=filenames[0];
  p=p.substr(0,p.rfind("/"));
  string s=getOpenFileName(this,"Load data-basis file",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  while(!beamtimes.empty())beamtimes.pop_back();
  while(!runs.empty())runs.pop_back();
  filenames[0]=s;
  parameterManager::readDataBasis_ascii(s,beamtimes,runs);
  writeInitFile();
}
void typeCaseMW::OnLoadGeometry()
{
  string p=(filenames[3]);
  p=p.substr(0,p.rfind("/"));
  string s=getOpenFileName(this,"Load geometry file",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  filenames[3]=s;
  while(!detectors.empty())detectors.pop_back();
  parameterManager::readDetectorParameter_ascii(s,detectors,experiment);
  Setup->setSetup(detectors,materials,default_shapes,experiment);
  writeInitFile();
}
void typeCaseMW::OnLoadShapes()
{
  string p=filenames[2];
  p=p.substr(0,p.rfind("/"));
  string s=getOpenFileName(this,"Load shapes file",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  filenames[2]=s;
  while(!default_shapes.empty())default_shapes.pop_back();
  parameterManager::readShapeParameter_ascii(s,default_shapes);
  Setup->setSetup(detectors,materials,default_shapes,experiment);
  writeInitFile();
}
void typeCaseMW::OnLoadAlgorithms()
{
  string p=filenames[4];
  p=p.substr(0,p.rfind("/"));
  string s=getOpenFileName(this,"Load algorithm file",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  for(unsigned int i=0;i<algorithms.size();i++)
    if(algorithms[i].IsUsed())emit selectAlgorithm(algorithms[i],false);
  filenames[4]=s;
  while(!algorithms.empty())algorithms.pop_back();
  parameterManager::readAlgorithmParameter_ascii(s,algorithms);
  for(unsigned int i=0;i<algorithms.size();i++)
    if(algorithms[i].IsUsed())
      emit selectAlgorithm(algorithms[i],false);
  writeInitFile();
}
void typeCaseMW::OnLoadAnalysis()
{
  string p=filenames[1];
  p=p.substr(0,p.rfind("/"));
  string s=getOpenFileName(this,"Load anamlysis-pattern file",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  if(!existing(s))return;
  filenames[1]=s;
  Analysis->readAnalysisParameter(s);
  writeInitFile();
}
void typeCaseMW::OnSaveData()
{
  parameterManager::writeDataBasis_ascii(filenames[0],beamtimes,runs);
}
void typeCaseMW::OnSaveGeometry()
{
  parameterManager::writeDetectorParameter_ascii(filenames[3],detectors,experiment); 
}
void typeCaseMW::OnSaveShapes()
{
  parameterManager::writeShapeParameter_ascii(filenames[2],default_shapes); 
}
void typeCaseMW::OnSaveAlgorithms()
{
  parameterManager::writeAlgorithmParameter_ascii(filenames[4],algorithms);
}
void typeCaseMW::OnSaveAnalysis()
{
  Analysis->writeAnalysisParameter(filenames[1]);
}
void typeCaseMW::OnSaveAsData()
{
  string p=filenames[0];
  p=p.substr(0,p.rfind("/"));
  string s=getSaveFileName(this,"Save data-basis",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  vector<string> fn;
  for(unsigned int i=0;i<beamtimes.size();i++)
    {
      string s1=getSaveFileName(this,string("Save beam-time \"")+beamtimes[i].getName(),p,"data files (*.data) ;; any files (*)");
      if(s1=="")return;
      fn.push_back(s1);
    }
  filenames[0]=s;
  for(unsigned int i=0;i<beamtimes.size();i++)
    beamtimes[i].setFileName(fn[i]);
  parameterManager::writeDataBasis_ascii(filenames[0],beamtimes,runs);
  writeInitFile();
}
void typeCaseMW::OnSaveAsGeometry()
{
  string p=filenames[3];
  p=p.substr(0,p.rfind("/"));
  string s=getSaveFileName(this,"Save geometry",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  filenames[2]=s;
  parameterManager::writeDetectorParameter_ascii(s,detectors,experiment); 
  writeInitFile();
}
void typeCaseMW::OnSaveAsShapes()
{
  string p=filenames[2];
  p=p.substr(0,p.rfind("/"));
  string s=getSaveFileName(this,"Save shapes",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  filenames[2]=s;
  parameterManager::writeShapeParameter_ascii(s,default_shapes);
  writeInitFile();
}
void typeCaseMW::OnSaveAsAlgorithms()
{
  string p=filenames[4];
  p=p.substr(0,p.rfind("/"));
  string s=getSaveFileName(this,"Save algorithms",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  filenames[4]=s;
  parameterManager::writeAlgorithmParameter_ascii(s,algorithms);
  writeInitFile();
}
void typeCaseMW::OnSaveAsAnalysis()
{
  string p=filenames[1];
  p=p.substr(0,p.rfind("/"));
  string s=getSaveFileName(this,"Save analysis-pattern",p,"data files (*.data) ;; any files (*)");
  if(s=="")return;
  filenames[1]=s;
  Analysis->writeAnalysisParameter(s);
  writeInitFile();
}
void typeCaseMW::onAnalysisParameterWritten(const string & fn)
{
  filenames[1]=fn;
}
void typeCaseMW::writeInitFile()
{
//    QDir homeDir(QDir::home().path()+"/.typeCase");
    ofstream out;
    out.open(QS2s(typeCaseHome.path()+"/initFile.data").data());
    string s;
    string p=QS2s(dataDir.path());
    for(int i=0;i<7;i++)
    {
	s=filenames[i];
	if(hasA(s,p))
	    s=replace(s,p,"$(DATA)");
	out<<s.data()<<endl;
    }
    out<<QS2s(calibrationDir.path()).data()<<endl;
    out<<QS2s(dataDir.path()).data()<<endl;
    for(int i=7;i<9;i++)
    {
	s=filenames[i];
	if(hasA(s,p))
	    s=replace(s,p,"$(DATA)");
	out<<s.data()<<endl;
    }
    out.close();
}

void typeCaseMW::OnChangeCalibrationDirectory()
{
  string s=getOpenDirectory(this,"Set Calibration Directory",QS2s(calibrationDir.path()),"");
  if(s=="")return;
  calibrationDir=QDir(QString(s.data()));
  writeInitFile();
}
void typeCaseMW::OnChangeDataDirectory()
{
  string s=getOpenDirectory(this,"Set default Data Directory",QS2s(dataDir.path()),"");
  if(s=="")return;
  dataDir=QDir(s.data());
  writeInitFile();
}
void typeCaseMW::OnCopyToDataDirectory()
{
    for(int i=0;i<6;i++)
    {
      filenames[i]=QS2s(dataDir.path())+filenames[i].substr(filenames[i].rfind("/"),filenames[i].length()-filenames[i].rfind("/"));
    }
    OnSaveData();
    OnSaveGeometry();
    OnSaveShapes();
    OnSaveAlgorithms();
    OnSaveAnalysis();
    writeInitFile();
}
void typeCaseMW::OnAlgorithmInstall()
{
  AlgorithmWizard fenst(this);
  connect(&fenst,SIGNAL(helpRequested(const string&)),Help,SLOT(setHelp(const string&))); 
  fenst.setAlgorithms(algorithms);
  fenst.exec();
  disconnect(&fenst,SIGNAL(helpRequested(const string&)),Help,SLOT(setHelp(const string&))); 
  if(fenst.result()==QDialog::Accepted)
    {
      algorithms.push_back(fenst.getAlgorithm());
      Algorithm->setAlgorithms(algorithms);
    }
}
void typeCaseMW::readInstallLog()
{
  string s=getOpenFileName(this,"Load installLog Algorithm","","data files (*.data)");
  if(s=="")return;
  AlgorithmWizard fenst(this);
  fenst.setAlgorithms(algorithms);
  fenst.installLogIO(s, false);
  fenst.accept();
  if(fenst.result()==QDialog::Accepted)
    {
      algorithms.push_back(fenst.getAlgorithm());
      Algorithm->setAlgorithms(algorithms);
    }
}
void typeCaseMW::OnShapeInstall()
{
  //  QMessageBox::information(this,"Shape-install","Unfortunately this part of the program is not available yet.",QMessageBox::Ok);
  ShapeWizard fenst(this);
  connect(&fenst,SIGNAL(helpRequested(const string&)),Help,SLOT(setHelp(const string&))); 
  fenst.setShapes(default_shapes);
  fenst.exec();
  disconnect(&fenst,SIGNAL(helpRequested(const string&)),Help,SLOT(setHelp(const string&))); 
  if(fenst.result()==QDialog::Accepted)
    {
      default_shapes.push_back(fenst.getShape());
      Setup->setShapes(default_shapes);
    }
/*     Setup->InstallShape(); */
}
void typeCaseMW::OnShapeInstallLog()
{
  string s=getOpenFileName(this,"Load installLog Shape","","data files (*.data)");
  if(s=="")return;
  ShapeWizard fenst(this);
  fenst.setShapes(default_shapes);
  fenst.installLogIO(s, false);
  fenst.accept();
  if(fenst.result()==QDialog::Accepted)
    {
      default_shapes.push_back(fenst.getShape());
      Setup->setShapes(default_shapes);
    }
  //QMessageBox::information(this,"Load Shape install-log","Unfortunately this part of the program is not available yet.",QMessageBox::Ok);
/*     Setup->InstallShapeLog(); */
}
void typeCaseMW::OnAlgorithmNew()
{
  QMessageBox::information(this,"New Algorithm","Unfortunately this part of the program is not available yet.\nYou will have to program the algorithm the hard way, \nbut you can use the install-feature to add it to the analyser and typeCase.",QMessageBox::Ok);
}
void typeCaseMW::OnShapeNew()
{
  QMessageBox::information(this,"New Shape","Unfortunately this part of the program is not available yet.\nYou will have to program the shape the hard way, \nbut you can use the install-feature to add it to the analyser and typeCase.",QMessageBox::Ok);
}
void typeCaseMW::OnAddAlgorithm()
{
  string s=getOpenFileName(this,"Load single Algorithm",filenames[4].substr(0,filenames[4].rfind("/")),"data files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  vector<algorithm_parameter> newalgorithm;
  parameterManager::readAlgorithmParameter_ascii(s,newalgorithm);
  QMessageBox::information(this,"Add Algorithm",(string_number((int)newalgorithm.size())+" algorithms found in file \n\""+s+"\".").data(),QMessageBox::Ok);
  QSingleWidgetDialog fenst(this);
  AlgorithmDisplay wid(&fenst);
  for(unsigned int i=0;i<newalgorithm.size();i++)
    {
      wid.setAlgorithm(&(newalgorithm[i]));
      fenst.exec();
      if(fenst.result()==QDialog::Accepted)
	algorithms.push_back(newalgorithm[i]);
    }
      Algorithm->setAlgorithms(algorithms);
}
void typeCaseMW::OnRunAdd()
{
  string s=getOpenFileName(this,"Load single Run",filenames[0].substr(0,filenames[0].rfind("/")),"data files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  vector<run_parameter> newrun;
  parameterManager::readRunParameter_ascii(s,newrun);
  QMessageBox::information(this,"Add run",(string_number((int)newrun.size())+" runs found in file\""+s+"\"").data(),QMessageBox::Ok);
  QSingleWidgetDialog fenst(this);
  RunDisplay wid(&fenst);
  fenst.setWidget(&wid);
  wid.setRunTypes(readRunTypes(filenames[8]));
  vector<string> tmp; for(unsigned int i=0;i<beamtimes.size();i++)tmp.push_back(beamtimes[i].getName());
  wid.setBeamtimes(tmp);
  for(unsigned int i=0;i<newrun.size();i++)
    {
      wid.setRun(newrun[i]);
      fenst.exec();
      if(fenst.result()==QDialog::Accepted&&newrun[i].getParentNumber()>=0&&newrun[i].getParentNumber()<(int)beamtimes.size())
	{
	  runs.push_back(newrun[i]);
	  runs.back().setParent(&beamtimes[newrun[i].getParentNumber()]);
	}
    }
  Dataw->setDataBasis(beamtimes,runs);
}
void typeCaseMW::OnBTAdd()
{
  string s=getOpenFileName(this,"Load single BeamTime",filenames[0].substr(0,filenames[0].rfind("/")),"data files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  vector<run_parameter> newrun;
  vector<beamTime_parameter> newBT;
  parameterManager::readDataBasis_ascii(s,newBT,newrun);
  QMessageBox::information(this,"Add Beam-time",(string_number((int)newBT.size())+" beam-times and "+string_number((int)newrun.size())+" runs found in file \n\""+s+"\".").data(),QMessageBox::Ok);
  for(unsigned int i=0;i<newBT.size();i++)
    beamtimes.push_back(newBT[i]);
  for(unsigned int i=0;i<newrun.size();i++)
    runs.push_back(newrun[i]);
  Dataw->setDataBasis(beamtimes,runs);
}
void typeCaseMW::OnShapeAdd()
{
  string s=getOpenFileName(this,"Load single shape",filenames[2].substr(0,filenames[2].rfind("/")),"data files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  vector<shape_parameter> newshape;
  parameterManager::readShapeParameter_ascii(s,newshape);
  QMessageBox::information(this,"Add Shape",(string_number((int)newshape.size())+" shapes found in file \n\""+s+"\".").data(),QMessageBox::Ok);
  QSingleWidgetDialog fenst(this);
  ShapeDisplay wid(&fenst);
  for(unsigned int i=0;i<newshape.size();i++)
    {
      wid.setShape(newshape[i]);
      fenst.exec();
      if(fenst.result()==QDialog::Accepted)
	default_shapes.push_back(newshape[i]);
    }
  Setup->setSetup(detectors,materials,default_shapes,experiment);
}
void typeCaseMW::OnDetectorAdd()
{
  string s=getOpenFileName(this,"Load single detector",filenames[3].substr(0,filenames[3].rfind("/")),"data files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  vector<detector_parameter> newdet;
  parameterManager::readDetectorParameter_ascii(s,newdet);
  QMessageBox::information(this,"Add Detector",(string_number((int)newdet.size())+" detectors found in file \n\""+s+"\".").data(),QMessageBox::Ok);
  QSingleWidgetDialog fenst(this);
  DetectorDisplay wid(&fenst);
  vector<string> tmp;for(unsigned int i=0;i<materials.size();i++)tmp.push_back(materials[i].getName());
  wid.setMaterials(tmp);
  wid.setDefinedShapes(default_shapes);
  for(unsigned int i=0;i<newdet.size();i++)
    {
      wid.setDetector(newdet[i]);
      fenst.exec();
      if(fenst.result()==QDialog::Accepted)
	detectors.push_back(newdet[i]);
    }
  Setup->setSetup(detectors,materials,default_shapes,experiment);
}
void typeCaseMW::setRunTypes(const string &filename)
{
  Dataw->setRunTypes(readRunTypes(filename));
}
vector<string> typeCaseMW::readRunTypes(const string &filename)
{
  vector<string> names;
  if(!existing(filename))return vector<string>();
  ifstream input;
  input.open(filename.data());
  char c;
  string s;
  while(!input.eof())
    {
      input.get(c);
      s="";
      while(c!='\n'&&!input.eof())
	{s=s+c;input.get(c);}
      if(s.length()>0)
	names.push_back(s);
    }
  input.close();
  return names;
}
