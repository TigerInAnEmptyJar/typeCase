#include "cutsmw.h"
#include "parameterManager.h"
#include "CommonCalibrationParser.h"
#include <TFile.h>
#include <TH2.h>
#include <TProfile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TSystem.h>
#include <TPostScript.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include "qcanvaswidget.h"
#include "qsinglewidgetdialog.h"
#include <qlineedit.h>
#include <qpushbutton.h>
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <qtable.h>
#include <qtabwidget.h>
#include <qframe.h>
#include <qdir.h>
#include <qmessagebox.h>
#include "checkwizard.h"
#if QT_VERSION < 0x040000
#include <qlayout.h>
#include <qlistview.h>
#else
#include <QGridLayout>
#include <QResizeEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#endif

#if QT_VERSION < 0x040000
cutsMW::cutsMW( QWidget* parent, const char* name, WFlags fl ):QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "CutsMW" );
#else
cutsMW::cutsMW( QWidget* parent, Qt::WindowFlags f):QMainWindow( parent, f )
{
    (void)statusBar();
    setObjectName( "CutsMW" );
#endif
    setCentralWidget( new QWidget( this ) );
    
    QCanvasWidget *tmp=new QCanvasWidget(this);
    delete tmp;

    control=new controlWidget(this);
    connect(control,SIGNAL(browseClicked()),this,SLOT(BrowseClicked()));
    connect(control,SIGNAL(fillClicked(int)),this,SLOT(fillClicked(int)));
    connect(control,SIGNAL(fitClicked(int)),this,SLOT(fitClicked(int)));
    connect(control,SIGNAL(checkClicked(int)),this,SLOT(checkClicked(int)));
    connect(control,SIGNAL(fillAllClicked()),this,SLOT(fill()));
    connect(control,SIGNAL(fitAllClicked()),this,SLOT(fit()));
    connect(control,SIGNAL(checkAllClicked()),this,SLOT(check()));
    connect(control,SIGNAL(write()),this,SLOT(writeCalibrationDB()));
    connect(control,SIGNAL(view()),this,SLOT(viewCalibrationDB()));
    hitTree=NULL;
    hitFile=NULL;
    TDChistograms=NULL;
    QDChistograms=NULL;
    isMC=false;
    languageChange();
    setMinimumSize(QSize(640,650));
    resize( QSize(684, 651).expandedTo(minimumSizeHint()) );

#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

cutsMW::~cutsMW()
{
  if(hitFile!=NULL)
    {
      hitFile->Close();
      delete hitFile;
    }
}
void cutsMW::resizeEvent(QResizeEvent *e){resize(e->size().width(),e->size().height());}
void cutsMW::resize(const QSize&s){resize(s.width(),s.height());};
void cutsMW::resize(int w,int h){
  QMainWindow::resize(w,h);
  control->resize(w,h-statusBar()->height());
}
void cutsMW::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "cuts" ) );
#else
    setWindowTitle( tr( "cuts" ) );
#endif
}
void cutsMW::closeEvent( QCloseEvent * e )
{
  QMainWindow::closeEvent(e);
}
extern bool existing(string filename);
void cutsMW::setCommandLineArguments(int argc, char** argv)
{
  bool donowfill=false;
  bool donowfit=false;
  bool doLogTDC=false;
  vector<int> fitDets;
  vector<int> showDets;
  vector<int> loadDets;
  vector<int> writeDets;
  string outFile="test.data";
  string direc="";
  bool nogui=false;
  for(int i=1;i<argc;i++)
    {
      string s=argv[i];
      if(s.substr(0,4)=="--d=")
	{
	  if(!existing(s.substr(4,s.length()-4)))continue;
	  clearObjects();
	  parameterManager::readDetectorParameter_ascii(s.substr(4,s.length()-4),setup);
	  control->setDetectors(setup);
	  initObjects();
	}
      else if(s.substr(0,4)=="--f=")
	{
	  cout<<"open file \""<<s.substr(4,s.length()-4).data()<<"\""<<endl;
	  readFile(s.substr(4,s.length()-4));
	}
      else if(s=="--fill")
	donowfill=true;
      else if(s=="--fit")
	donowfit=true;
      else if(s.substr(0,6)=="--fit=")
	fitDets.push_back(toInt(s.substr(6,s.length()-6)));
      else if(s.substr(0,7)=="--show=")
	showDets.push_back(toInt(s.substr(7,s.length()-7)));
      else if(s.substr(0,7)=="--load=")
	loadDets.push_back(toInt(s.substr(7,s.length()-7)));
      else if(s.substr(0,8)=="--write=")
	writeDets.push_back(toInt(s.substr(8,s.length()-8)));
      else if(s.substr(0,4)=="--o=")
	outFile=s.substr(4,s.length()-4);
      else if(s=="--exit")
	nogui=true;
      else if(s.substr(0,10)=="--log-TDC=")
	{
	  direc=s.substr(10,s.length()-10);
	  doLogTDC=existing(direc);
	}
    }
  if(!doLogTDC)
    {
      if(donowfill)
	fill();
      if(donowfit)
	fit();
      else
	for(unsigned int i=0;i<fitDets.size();i++)
	  if(fitDets[i]>=0)
	    for(unsigned int j=0;j<setup.size();j++)
	      if(setup[j].getID()==fitDets[i])
		{
		  cout<<"fit: "<<setup[j].getName().data()<<endl;
		  fitClicked(j);
		}
      if(loadDets.size()>0)
	{
	  vector<int> ld;
	  for(unsigned int i=0;i<loadDets.size();i++)
	    {
	      if(loadDets[i]>=0)
		for(unsigned int j=0;j<setup.size();j++)
		  if(setup[j].getID()==loadDets[i])
		    ld.push_back(j);
	    }
	  getLast(ld);
	}
//       TFile ffout("test.root","recreate");
//       ffout.cd();
//       TDChistograms[0][0]->Write();
//       TDChistograms[0][0]->SetDirectory(NULL);
//       ffout.Close();
      for(unsigned int i=0;i<showDets.size();i++)
	if(showDets[i]>=0)
	  for(unsigned int j=0;j<setup.size();j++)
	    if(setup[j].getID()==showDets[i])
	      {
		cout<<"show: "<<setup[j].getName().data()<<endl;
		checkClicked(j);
	      }
      if(writeDets.size()>0)
	{
	  vector<int> tmp;
	  for(unsigned int i=0;i<writeDets.size();i++)
	    for(unsigned int j=0;j<setup.size();j++)
	      if(setup[j].getID()==writeDets[i])
		tmp.push_back(j);
	  if(!hasA(outFile,".data")&&!hasA(outFile,".cal")&&hitFile!=NULL)
	    {
	      string s=hitFile->GetName();
	      if(s.find("/")<s.npos)s=s.substr(s.rfind("/")+1,s.length()-s.rfind("/")-1);
	      s=s.substr(3,4);
	      outFile=outFile+s+".data";
	    }
	  writeCalibrationDB(tmp,outFile);
	  writeLast();
	}
    }
  else
    {
      logTDC2D(direc,fitDets,outFile);
    }
  if(nogui)exit(1);
} 
void cutsMW::clearObjects()
{
  if(TDChistograms!=NULL)
    {
      for(unsigned int i=0;i<setup.size();i++)
	{
	  for(int j=0;j<setup[i].getNumberOfElements();j++)
	    delete TDChistograms[i][j];
	  delete []TDChistograms[i];
	}
      delete []TDChistograms;
      TDChistograms=NULL;
    }
  if(QDChistograms!=NULL)
    {
      for(unsigned int i=0;i<setup.size();i++)
	{
	  for(int j=0;j<setup[i].getNumberOfElements();j++)
	    delete QDChistograms[i][j];
	  delete []QDChistograms[i];
	}
      delete []QDChistograms;
      QDChistograms=NULL;
    }
  while(!cals.empty())
    {
      CommonCalibrationParser *tmp=cals.back();
      cals.pop_back();
      delete tmp;
    }
}
void cutsMW::getLast(vector<int> positions)
{
  vector<CommonCalibrationParser*> tmp;
  CommonCalibrationParser::getFileContent("lastFile.data",tmp);
  int pos;
  for(unsigned int i=0;i<positions.size();i++)
    {
      pos=-1;
      if(positions[i]<0||positions[i]>=(int)setup.size())continue;
      for(unsigned int j=0;j<tmp.size();j++)
	{
	  if(tmp[j]->getDetector()==setup[positions[i]].getID()&&tmp[j]->getParamID()==3)
	    {pos=j;break;}
	}
      if(pos<0)continue;
      for(int j=0;j<setup[positions[i]].getNumberOfElements();j++)
	{
	  for(int k=0;k<4;k++)
	    cals[positions[i]]->setParameter(j,k,tmp[pos]->getParameter(j,k));
	}
    }
}
void cutsMW::writeLast()
{
  ofstream output;
  output.open("lastFile.data");
  for(unsigned int i=0;i<cals.size();i++)
    if(cals[i]!=NULL)
      {
	cals[i]->write(output,QS2s(control->authorInput->text()),QS2s(control->beamInput->text()));
      }
  output.close();
}
void cutsMW::getParser(vector<string> filenames)
{
  while(!ncCals.empty())
    {
      while(!ncCals.back().empty())
	{
	  while(!ncCals.back().back().empty())
	    {
	      if(ncCals.back().back().back()!=NULL)
		delete ncCals.back().back().back();
	      ncCals.back().back().pop_back();
	    }
	  ncCals.back().pop_back();
	}
      ncCals.pop_back();
    }
  while(!curr.empty())
    {curr.pop_back();}
  if(!existing(filenames.back()))cout<<"file \""<<filenames.back().data()<<"\" doesn\'t extist."<<endl;
  vector<CommonCalibrationParser*> tmpparser;
  for(unsigned int i=0;i<filenames.size();i++)
  CommonCalibrationParser::getFileContent(filenames[i],tmpparser);
  //  vector<bool>used;
  //  for(unsigned int i=0;i<tmpparser.size();i++){used.push_back(false);}
  for(unsigned int i=0;i<setup.size();i++)
    {
      vector<vector<CommonCalibrationParser*> > apa;
      vector<CommonCalibrationParser*> tapa,tapb,tapc,tapd;
      vector<CommonCalibrationParser*> tmpContainer;
      for(unsigned int j=0;j<tmpparser.size();j++)
	{
	  if(tmpparser[j]->getDetector()==setup[i].getID())//&&!used[j])
	    {
	      if(tmpparser[j]->getParamID()==5)
		  tapa.push_back(tmpparser[j]);
	      else if(tmpparser[j]->getParamID()==1)
		  tapb.push_back(tmpparser[j]);
	      else if(tmpparser[j]->getParamID()==2)
		  tapc.push_back(tmpparser[j]);
	      else if(tmpparser[j]->getParamID()==0)
		  tapd.push_back(tmpparser[j]);
	      else
		  tmpContainer.push_back(tmpparser[j]);
	    }
	  else
	      tmpContainer.push_back(tmpparser[j]);
	}
      while(!tmpparser.empty())tmpparser.pop_back();
      for(unsigned int ii=0;ii<tmpContainer.size();ii++)tmpparser.push_back(tmpContainer[ii]);
      while(!tmpContainer.empty())tmpContainer.pop_back();
      apa.push_back(tapa);//offset
      apa.push_back(tapb);//tdcFactor
      apa.push_back(tapc);//walk
      apa.push_back(tapd);//pedestal
      vector<int> a;
      a.push_back(tapa.size()>0?0:-1);
      a.push_back(tapb.size()>0?0:-1);
      a.push_back(tapc.size()>0?0:-1);
      a.push_back(tapd.size()>0?0:-1);
      curr.push_back(a);
      ncCals.push_back(apa);
    }
  while(!tmpparser.empty())
    {
      CommonCalibrationParser* po=tmpparser.back();
      tmpparser.pop_back();
      delete po;
    }
  /*
    for(unsigned int i=0;i<ncCals.size();i++)
    {
    cout<<"pos "<<i<<" "<<endl;
    for(unsigned int j=0;j<ncCals[i].size();j++)
    {
    cout<<"\tnum "<<j<<" "<<ncCals[i][j].size()<<endl;
    for(unsigned int k=0;k<ncCals[i][j].size();k++)
    {
    cout<<"\t\t"<<ncCals[i][j][k]->getRange(2)<<" - "<<ncCals[i][j][k]->getRange(3)<<endl;
    }
    }
    }
  */
}
float cutsMW::getTDC(int pos, int el, int qdc, int tdc, int evtnr)
{
  float ret=tdc;
  float qret=qdc;
  float wlk=0;
  for(int i=0;i<4;i++)
    { 
//5 o_tdc//1 f_tdc//2 walk//0 pedestal
      if(ncCals[pos][i].size()<1)return tdc;
      if(ncCals[pos][i].size()>1){
	while(evtnr<ncCals[pos][i][curr[pos][i]]->getRange(2)&&curr[pos][i]>0)curr[pos][i]=curr[pos][i]-1;
	while(evtnr>ncCals[pos][i][curr[pos][i]]->getRange(3)&&curr[pos][i]+1<=(int)ncCals[pos][i].size())curr[pos][i]=curr[pos][i]+1;
      }
    }
  if(curr[pos][3]>=0&&curr[pos][3]<(int)ncCals[pos][3].size())//pedestal
    qret=qret-ncCals[pos][3][curr[pos][3]]->getParameter(el,0);
  if(curr[pos][2]>=0&&curr[pos][2]<(int)ncCals[pos][2].size())//walk
    wlk=ncCals[pos][2][curr[pos][2]]->getParameter(el,0)/
      (qret-ncCals[pos][2][curr[pos][2]]->getParameter(el,1))
      +ncCals[pos][2][curr[pos][2]]->getParameter(el,2)*
      log(abs(qret-ncCals[pos][2][curr[pos][2]]->getParameter(el,3)))
      +ncCals[pos][2][curr[pos][2]]->getParameter(el,4);
  if(curr[pos][1]>=0&&curr[pos][1]<(int)ncCals[pos][1].size())//f_tdc
    ret=ret*ncCals[pos][1][curr[pos][1]]->getParameter(el,0);
  ret=ret-wlk;
  if(curr[pos][0]>=0&&curr[pos][0]<(int)ncCals[pos][0].size())//o_tdc
    ret=ret-(ncCals[pos][0][curr[pos][0]]->getParameter(el,0)
	     +ncCals[pos][0][curr[pos][0]]->getParameter(el,1)*evtnr*1e-6);
  return ret;
}
void cutsMW::initObjects()
{
  TDChistograms=new TH1F**[setup.size()];
  QDChistograms=new TH1F**[setup.size()];
  for(unsigned int i=0;i<setup.size();i++)
    {
      TDChistograms[i]=new TH1F*[setup[i].getNumberOfElements()];
      QDChistograms[i]=new TH1F*[setup[i].getNumberOfElements()];
      for(int j=0;j<setup[i].getNumberOfElements();j++)
	{
	  string nn=string("det ")+string_number(i)+", el "+string_number(j);
	  bool isStart=(setup[i].getID()==15||setup[i].getID()==16);
	  TDChistograms[i][j]=new TH1F((string("TDC ")+nn).data(),(string("TDC ")+nn).data(),200,(isStart?-510:-520),(isStart?-490:-440));
	  QDChistograms[i][j]=new TH1F((string("QDC ")+nn).data(),(string("QDC ")+nn).data(),300,0,3000);
	}
      cals.push_back(new CommonCalibrationParser(setup[i].getNumberOfElements(),setup[i].getID()));
      cals.back()->setParamID(3);
      cals.back()->setNumParamsElements(setup[i].getNumberOfElements(),4);
      cals.back()->setValid(true);
      for(int j=0;j<setup[i].getNumberOfElements();j++)
	{
	  cals.back()->setParameter(j,0,0);
	  cals.back()->setParameter(j,1,9999999);
	  cals.back()->setParameter(j,2,0);
	  cals.back()->setParameter(j,3,9999999);
	}
    }
}
void cutsMW::writeCalibrationDB()
{
  QSingleWidgetDialog fenst(this);
  QListView listView(&fenst);
//   listView.setHeaderLabels(QStringList()<<"detectors");
  listView.addColumn("detectors");
  for(unsigned int i=0;i<setup.size();i++)
    {
      (new QCheckListItem(&listView,(setup[i].getName()).data(),QCheckListItem::CheckBoxController))->setOn(true);
    }
  fenst.setWidget(&listView);
  fenst.resize(400,400);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      string fn=getSaveFileName(this,"Save Calibration Constants",
				(string(getenv("KTOFPACK"))+"/data/cuts").data(),
				"data files (*.data) ;; any files (*)",true);
      if(fn=="")return;
      QListViewItem *it=listView.firstChild();
      vector<int> detsToWrite;
      while(it!=NULL)
	{
	  if(((QCheckListItem*)it)->isOn())
	    {
	      for(unsigned int i=0;i<setup.size();i++)
		if(it->text(0)==setup[i].getName().data())
		  detsToWrite.push_back(i);
	    }
	  it=it->nextSibling();
	}
      writeCalibrationDB(detsToWrite,fn);
    }
}
void cutsMW::writeCalibrationDB(vector<int> dets, const string &filename)
{
  if(dets.size()>0)
    {
      string s=string("file \"")+filename+"\" written";
      ofstream output;
      output.open(filename.data());
      for(unsigned int i=0;i<dets.size();i++)
	if(cals[dets[i]]!=NULL)
	  {
	    s=s+"\n"+setup[dets[i]].getName();
	    cals[dets[i]]->write(output,QS2s(control->authorInput->text()),QS2s(control->beamInput->text()));
	  }
      output.close();
      QMessageBox::information(this,"Writing",s.data(),QMessageBox::Ok,QMessageBox::NoButton); 
    }
}
void cutsMW::viewCalibrationDB()
{
  QSingleWidgetDialog fenst(this);
  QTabWidget tab(&fenst);
  fenst.resize(400,1000);
  fenst.setWidget(&tab);
  for(unsigned int i=0;i<setup.size();i++)
    {
      QTable *table=new QTable(&tab);
      table->setNumRows(setup[i].getNumberOfElements());
      table->setNumCols(4);
      table->setReadOnly(true);
      table->setColumnLabels(QStringList()<<"QDC-low"<<"QDC-high"<<"TDC-low"<<"TDC-high");
      for(int j=0;j<setup[i].getNumberOfElements();j++)
	for(int k=0;k<4;k++)
	  table->setText(j,k,string_number(cals[i]->getParameter(j,k)));
      tab.addTab(table,setup[i].getName().data());
    }
  fenst.exec();
}
#include <TLeaf.h>
void cutsMW::fill()
{
  if(TDChistograms==NULL)return;
  if(QDChistograms==NULL)return;
  if(hitTree==NULL)return;
  TLeaf *nH=hitTree->FindLeaf("numberOfHits");
  TLeaf *t=hitTree->FindLeaf("hit.tdc_r");
  TLeaf *q=hitTree->FindLeaf("hit.qdc_r");
  TLeaf *d=hitTree->FindLeaf("hit.det");
  TLeaf *e=hitTree->FindLeaf("hit.el");
  TLeaf *n=hitTree->FindLeaf("eventNumber");
  int D,E;
  if(nH==NULL||t==NULL||q==NULL||d==NULL||e==NULL||n==NULL)
    {
      QMessageBox::warning(this,"Tree invalid","Tree has invalid leaves",QMessageBox::Cancel,QMessageBox::NoButton);
      return;
    }
  int asi[100];
  for(int i=0;i<100;i++)
    {
      asi[i]=-1;
      for(unsigned int j=0;j<setup.size();j++)
	if(i==setup[j].getID())asi[i]=j;
    }
  for(unsigned int j=0;j<setup.size();j++)
    for(int i=0;i<setup[j].getNumberOfElements();i++)
      {
	TDChistograms[j][i]->Reset();
	QDChistograms[j][i]->Reset();
      }
  float neen=hitTree->GetEntries();
  int evtnr;
  for(int i=0;i<hitTree->GetEntries();i++)
    {
      hitTree->GetEntry(i);
      evtnr=(Int_t)n->GetValue();
      for(int j=0;j<nH->GetValue();j++)
	{
	  D=(int)d->GetValue(j);
	  E=(int)e->GetValue(j);
	  if(D<0||D>=100)continue;
	  if(asi[D]<0)continue;
	  if(E<0||E>=setup[asi[D]].getNumberOfElements())continue;
	  if(t->GetValue(j)!=-1)TDChistograms[asi[D]][E]->Fill((isMC?-450:0)-getTDC(asi[D],E,(Int_t)q->GetValue(j),(Int_t)t->GetValue(j),evtnr));
	  QDChistograms[asi[D]][E]->Fill(q->GetValue(j)*(isMC?10.:1.));
	}
      if(i%(int)(neen/10.)==0)cout<<"."<<flush;
    }
  cout<<endl;
}
void cutsMW::fillClicked(int num)
{
  if(num<0||num>=(int)setup.size())return;
  if(TDChistograms==NULL)return;
  if(QDChistograms==NULL)return;
  if(TDChistograms[num]==NULL)return;
  if(QDChistograms[num]==NULL)return;
  if(hitTree==NULL)return;
  QMessageBox::information(this,"filling",(string("Filling detector ")+setup[num].getName()).data(),QMessageBox::Ok);
  TLeaf *nH=hitTree->FindLeaf("numberOfHits");
  TLeaf *t=hitTree->FindLeaf("hit.tdc_c");
  TLeaf *q=hitTree->FindLeaf("hit.qdc_r");
  TLeaf *d=hitTree->FindLeaf("hit.det");
  TLeaf *e=hitTree->FindLeaf("hit.el");
  TLeaf *n=hitTree->FindLeaf("eventNumber");
  int D,E;
  if(nH==NULL||t==NULL||q==NULL||d==NULL||e==NULL||n==NULL)
    {
      QMessageBox::warning(this,"Tree invalid","Tree has invalid leaves",QMessageBox::Cancel,QMessageBox::NoButton);
      return;
    }
  int asi=setup[num].getID(),nEl=setup[num].getNumberOfElements();
  for(int i=0;i<nEl;i++)
    {
      TDChistograms[num][i]->Reset();
      QDChistograms[num][i]->Reset();
    }
  float neen=hitTree->GetEntries();
  int evtnr;
  for(int i=0;i<hitTree->GetEntries();i++)
    {
      hitTree->GetEntry(i);
      evtnr=(Int_t)n->GetValue();
      for(int j=0;j<nH->GetValue();j++)
	{
	  D=(int)d->GetValue(j);
	  E=(int)e->GetValue(j);
	  if(D!=asi)continue;
	  if(E<0||E>=nEl)continue;
	  if(t->GetValue(j)!=-1)TDChistograms[num][E]->Fill(-getTDC(asi,E,(Int_t)q->GetValue(j),(Int_t)t->GetValue(j),evtnr));//t->GetValue(j));
	  QDChistograms[num][E]->Fill(q->GetValue(j));
	}
      if(i%(int)(neen/10.)==0)cout<<"."<<flush;
    }
  cout<<endl;
}
void cutsMW::fit()
{
  for(unsigned int i=0;i<setup.size();i++)
    {
      cout<<"fit: "<<setup[i].getName().data()<<endl;
      fitClicked(i);
    }
}
#include <TF1.h>
Double_t myVoigt(Double_t *x, Double_t *pars);
Double_t myVoigt(Double_t *x, Double_t *pars){return pars[0]*TMath::Voigt((*x)-pars[1],pars[2],pars[3]);}

void cutsMW::fitClicked(int num)
{
  if(num<0||num>=(int)setup.size())return;
  if(TDChistograms==NULL)return;
  if(QDChistograms==NULL)return;
  if(TDChistograms[num]==NULL)return;
  if(QDChistograms[num]==NULL)return;
  int nEl=setup[num].getNumberOfElements();
  bool isStart=(setup[num].getID()==16||setup[num].getID()==15);
  bool findminimumTDC=(setup[num].getID()<3&&!isStart);
  string fittis="gaus";//(isStart?"gaus":(setup[num].getID()<3?"gaus(0)+landau(3)+gaus(6)":(setup[num].getID()<6?"landau":"[0]*TMath::Voigt(x-[1],[2],[3])")));
  string fitti="gaus(0)+landau(3)";
  switch(setup[num].getID())
    {
    case 0:fittis="gaus(0)+landau(3)+gaus(6)";break;
    case 1:fittis="gaus(0)+landau(3)+gaus(7)";break;
    case 2:fittis="gaus(0)+landau(3)+gaus(7)";break;
    case 3:fittis="landau";break;
    case 4:fittis="landau";break;
    case 5:fittis="landau";break;
    case 6:fittis="landau";break;
    case 7:fittis="landau";break;
    }
  //  fitti=fitti+"+"+"[3]/x*exp(-pow((log(x-[5])-[4]*[4])/[4],2)/2)";
  TF1 tfunction("tf1",fitti.data());
  TF1 tga("tg1","gaus");
  TF1 tla("tl1","landau");
  TF1 *qfunction;//("qf1",(fittis).data());
  if(fittis=="myVoigt(x,[2],[3],[1],[0])")
    {
      qfunction=new TF1("qf1",myVoigt,-600,-200,4);
      qfunction->SetParNames("constant","peak","sigma","lwidth");
    }
  else
    qfunction=new TF1("qf1",fittis.data());
  TF1 lan("lan","landau");
//   if(setup[num].getID()==7||setup[num].getID()==6)
//     {
//       for(int i=0;i<nEl;i++)
// 	cals[num]->setParameter(i,3,500);
//     }
//   else
    {
  for(int i=0;i<nEl;i++)
    if(TDChistograms[num][i]->GetEntries()>100)
      {
	qfunction->SetParameter(0,TDChistograms[num][i]->GetMaximum());
	qfunction->SetParameter(1,TDChistograms[num][i]->GetMean());//BinCenter(QDChistograms[num][i]->GetMaximumBin()));
	qfunction->SetParameter(2,TDChistograms[num][i]->GetRMS());
	if(findminimumTDC)
	  {
	    qfunction->SetParameters(TDChistograms[num][i]->GetMaximum()*0.1,
				    TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())-15,
				    1,
				    TDChistograms[num][i]->GetMaximum(), 
				    TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin()), 
				    1, 
				    TDChistograms[num][i]->GetMaximum()*0.01,
				    TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetNbinsX()/2), 
				    100);
	    qfunction->SetParLimits(1,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())-10,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin()));
	    qfunction->SetParLimits(4,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())-10,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())+10);
	    qfunction->SetParLimits(7,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())-10,TDChistograms[num][i]->GetBinCenter(TDChistograms[num][i]->GetMaximumBin())+10);
	    qfunction->SetParLimits(5,1e-4,4);
	    qfunction->SetParLimits(8,10,1000);
	    qfunction->SetParLimits(6,TDChistograms[num][i]->GetMaximum()*0.001,TDChistograms[num][i]->GetMaximum()*0.1);
	    qfunction->SetParLimits(0,TDChistograms[num][i]->GetMaximum()*0.001,TDChistograms[num][i]->GetMaximum()*0.2);
	  }
	TDChistograms[num][i]->Fit(qfunction,"Qw");//fittis.data());
	TF1 *fun=TDChistograms[num][i]->GetFunction(qfunction->GetName());//fittis.data());
	if(fun!=NULL)
	  {
	    if(!findminimumTDC)//isStart)
	      {
		if(isStart)
		  cals[num]->setParameter(i,2,-fun->GetX(0.01*fun->GetParameter(0),
							 fun->GetParameter(1),TDChistograms[num][i]->GetXaxis()->GetXmax()));
		cals[num]->setParameter(i,3,-fun->GetX(0.01*fun->GetParameter(0)*(isStart?1.:0.1),
						       TDChistograms[num][i]->GetXaxis()->GetXmin(),fun->GetParameter(1)));
	      }
	    else
	      {
		lan.SetParameters(fun->GetParameter(3),fun->GetParameter(4),fun->GetParameter(5));
		float mm=fun->GetParameter(1);
		if(fun->GetParameter(1)>fun->GetParameter(7))mm=fun->GetParameter(7);
		cals[num]->setParameter(i,3,-lan.GetX(0.0001*lan.GetParameter(0),
						      TDChistograms[num][i]->GetXaxis()->GetXmin(),lan.GetParameter(1)));
		//fun->GetMinimumX(mm,fun->GetParameter(4))+0.06*(abs(mm-fun->GetParameter(4))));
	      }

	  }
	//	cout<<"."<<flush;
      }}
  //  cout<<endl;
  float neQDC;
  for(int i=0;i<nEl;i++)
    if(QDChistograms[num][i]->GetEntries()>100)
      {
	int j=1;
	while(QDChistograms[num][i]->GetBinContent(j)==0&&QDChistograms[num][i]->GetNbinsX()>j)j++;
	float minf=QDChistograms[num][i]->GetBinCenter(j),maxf=QDChistograms[num][i]->GetBinCenter(QDChistograms[num][i]->GetNbinsX());
	tla.SetParameters(QDChistograms[num][i]->GetMaximum(),
				QDChistograms[num][i]->GetMean(),
				QDChistograms[num][i]->GetRMS());
	tfunction.SetParameters(QDChistograms[num][i]->GetMaximum(),0,
				QDChistograms[num][i]->GetRMS(),
				QDChistograms[num][i]->GetMaximum(),
				QDChistograms[num][i]->GetMean(),//BinCenter(QDChistograms[num][i]->GetMaximumBin())
				QDChistograms[num][i]->GetRMS());
	if(setup[num].getID()==6||setup[num].getID()==7)
	  {
	    minf=QDChistograms[num][i]->GetBinCenter(QDChistograms[num][i]->GetMaximumBin()-12);
	    QDChistograms[num][i]->Fit(&tla,"Qw","",minf,maxf);
	    TF1 *fufu=QDChistograms[num][i]->GetFunction(tla.GetName());
	    tfunction.FixParameter(3,fufu->GetParameter(0));//*0.9999999,fufu->GetParameter(0)*1.000001);
	    tfunction.FixParameter(4,fufu->GetParameter(1));//*0.9999999,fufu->GetParameter(1)*1.000001);
	    tfunction.FixParameter(5,fufu->GetParameter(2));//*0.9999999,fufu->GetParameter(2)*1.000001);
	    minf=QDChistograms[num][i]->GetBinCenter(j);
	  }
	//	cout<<setup[num].getID()<<" "<<i<<":"<<flush;
	QDChistograms[num][i]->Fit(&tla,"Q","",minf,maxf);
	TF1 *fun=QDChistograms[num][i]->GetFunction(tla.GetName());
	if(fun->GetChisquare()/fun->GetNDF()>2.)
	  {
	    QDChistograms[num][i]->Fit(&tfunction,"Q","",minf,maxf);
	    fun=QDChistograms[num][i]->GetFunction(tfunction.GetName());
	    if(fun!=NULL)
	      {
		if(fun->GetChisquare()/fun->GetNDF()>5)
		  {
		    QDChistograms[num][i]->Fit(&tfunction,"QW","",minf,maxf);
		    fun=QDChistograms[num][i]->GetFunction(tfunction.GetName());
		  }
		if(fun->GetChisquare()/fun->GetNDF()>5)
		  {
		    QDChistograms[num][i]->Fit(&tfunction,"Ql","",minf,maxf);
		    fun=QDChistograms[num][i]->GetFunction(tfunction.GetName());
		  }
// 		if(fun->GetChisquare()/fun->GetNDF()>5)
// 		  {
// 		    QDChistograms[num][i]->Fit(&tfunction,"Qll2","",minf,maxf);
// 		    fun=QDChistograms[num][i]->GetFunction(tfunction.GetName());
// 		  }
	      }
	    //cout<<"landau ("<<fun->GetParameter(4)<<" "<<fun->GetParameter(5)<<"), gaus ("<<fun->GetParameter(1)<<" "<<fun->GetParameter(2)<<")"<<endl;
	  }
	//else
	//cout<<"landau "<<fun->GetParameter(0)<<" "<<fun->GetParameter(1)<<" "<<fun->GetParameter(2)<<endl;
	if(fun!=NULL)
	  {
	    if(fun->GetName()==tla.GetName())
	      {
// 		tga.SetParameters(0,0,0);
 		tla.SetParameters(fun->GetParameter(0),fun->GetParameter(1),fun->GetParameter(2));
		neQDC=tla.GetX(0.005*fun->GetParameter(0),QDChistograms[num][i]->GetBinCenter(j),fun->GetParameter(1));
	      }
	    else
	      {
// 		tga.SetParameters(fun->GetParameter(0),fun->GetParameter(1),fun->GetParameter(2));
 		tla.SetParameters(fun->GetParameter(3),fun->GetParameter(4),fun->GetParameter(5));
		float upper=fun->GetParameter(4),lower=fun->GetParameter(1);
		if(lower<QDChistograms[num][i]->GetBinCenter(j))lower=QDChistograms[num][i]->GetBinCenter(j);
		if(upper>lower)
		  {
		    if(lower<fun->GetParameter(1)&&fun->GetParameter(1)<upper)//abs(j-QDChistograms[num][i]->GetMaximumBin())<15)
		      lower=fun->GetParameter(1);
		    neQDC=fun->GetMinimumX(lower,upper);
		  }
		else neQDC=lower;
		if(fun->GetChisquare()/fun->GetNDF()>5||fun->GetParameter(4)<fun->GetParameter(1))
		  neQDC=tla.GetX(0.04*fun->GetParameter(3),QDChistograms[num][i]->GetBinCenter(j),fun->GetParameter(4));
	      }
	    cals[num]->setParameter(i,0,neQDC);
	  }
	if(cals[num]->getParameter(i,0)<QDChistograms[num][i]->GetBinCenter(j))cals[num]->setParameter(i,0,QDChistograms[num][i]->GetBinCenter(j));
	if(cals[num]->getParameter(i,0)>QDChistograms[num][i]->GetBinCenter(QDChistograms[num][i]->GetNbinsX()))cals[num]->setParameter(i,0,QDChistograms[num][i]->GetBinCenter(j));

      }
  delete qfunction;
}
void cutsMW::check()
{
  for(unsigned int i=0;i<setup.size();i++)checkClicked(i);
}
void cutsMW::checkClicked(int num)
{
  if(num<0||num>=(int)setup.size())return;
  if(TDChistograms==NULL)return;
  if(QDChistograms==NULL)return;
  if(TDChistograms[num]==NULL)return;
  if(QDChistograms[num]==NULL)return;
  checkWizard wiz(this);
  wiz.setCaption((string("check det ")+setup[num].getName()).data());
  wiz.resize(1000,800);
  wiz.setHistograms(TDChistograms[num],QDChistograms[num],cals[num]);
  wiz.exec();
//   if(wiz.result()==QWizard::Accepted)
//     {
//       QMessageBox::information(this,"check","check ok",QMessageBox::Ok,QMessageBox::NoButton);
//     }
}
void cutsMW::BrowseClicked()
{
  string fn=getOpenFileName(this,"open file","/users/scr2_pc81/pion/ehrhardt/hits/","root files ( *H.root ) ;; any files (*)");
  readFile(fn);
}
void cutsMW::readFile(const string& fn)
{
  if(!existing(fn)) return;
  string base=fn;
  if(hasA(base,"/"))base=base.substr(base.rfind("/")+1,base.length()-base.rfind("/")-1);
  isMC=hasA(base,"MC");
  base=base.substr(3,4);
  vector<string> fanm;
  if(isMC)
    {
      fanm.push_back("/users/scr0_pc46/pion/TOF/Tof2004/Data/CalibrationDataBasis/MC/calmc.data");
    }
 else
   {
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/zbarrel.data");
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/tdcradpol.data");
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/tdcfactor.data");
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/pedestal.data");
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/cuts_qdc.data");
     fanm.push_back(string(getenv("KTOFPACK"))+"/data/calibration/Calibrationrun"+base+"_TDC_0.cal");
   }
  //  fanm.push_back(string(getenv("KTOFPACK"))+"/data/total_calibration/");
  getParser(fanm);
  TFile *tmp=new TFile(fn.data(),"read");
  TTree* tmpTree=(TTree*)tmp->Get("hits");
  if(tmpTree==NULL)
    {
      QMessageBox::warning(this,"File invalid",(string("File \"")+fn+"\" has no tree \"hits\".\nKeep old file").data(),QMessageBox::Cancel,QMessageBox::NoButton);
      return;
    }
  if(hitFile!=NULL)
    {
      hitFile->Close();
      delete hitFile;
    }
  hitFile=tmp;
  hitTree=tmpTree;
}
void cutsMW::logTDC(string directory,vector<int> detectors,string outfile)
{
  if(detectors.size()==0)return;
  int ndets=0;
  int found;
  for(unsigned int i=0;i<detectors.size();i++)
    {
      found=-1;
      for(unsigned int j=0;j<setup.size();j++)
	if(setup[j].getID()==detectors[i])found=j;
      if(found>=0)ndets++;
    }
  if(ndets==0)return;
  int indexes[ndets];
  ndets=0;
  for(unsigned int i=0;i<detectors.size();i++)
    {
      found=-1;
      for(unsigned int j=0;j<setup.size();j++)
	if(setup[j].getID()==detectors[i])found=j;
      if(found>=0)
	{
	  indexes[ndets]=found;
	  ndets++;
	}
    }
  QDir dir(QString(directory.data()),"*H.root");
  if(dir.count()<=0)return;
  vector<string> filenames;
  for(int i=0;i<(int)dir.count();i++)
    filenames.push_back(QS2s(dir[i]));

  Double_t x[filenames.size()],**y[ndets],**pos[ndets],**dy[ndets],**dpos[ndets];
  for(int i=0;i<ndets;i++)
    {
      y[i]=new Double_t*[setup[indexes[i]].getNumberOfElements()];
      pos[i]=new Double_t*[setup[indexes[i]].getNumberOfElements()];
      dy[i]=new Double_t*[setup[indexes[i]].getNumberOfElements()];
      dpos[i]=new Double_t*[setup[indexes[i]].getNumberOfElements()];
      for(int j=0;j<setup[indexes[i]].getNumberOfElements();j++)
	{
	  y[i][j]=new Double_t[filenames.size()];
	  pos[i][j]=new Double_t[filenames.size()];
	  dy[i][j]=new Double_t[filenames.size()];
	  dpos[i][j]=new Double_t[filenames.size()];
	  for(unsigned int k=0;k<filenames.size();k++)
	    {
	      y[i][j][k]=0;
	      pos[i][j][k]=-500;
	      dy[i][j][k]=2;
	      dpos[i][j][k]=-500;
	    }
	}
    }
  for(unsigned int i=0;i<filenames.size();i++)
    {
      string s=filenames[i];
      s=s.substr(3,s.length()-6-3-4);
      x[i]=toInt(s);
      cout<<filenames[i].data()<<flush;
      readFile(directory+filenames[i]);
      fill();
      for(int j=0;j<ndets;j++)
	{
	  fitClicked(indexes[j]);
	  for(int k=0;k<setup[indexes[j]].getNumberOfElements();k++)
	    {
	      if(TDChistograms[indexes[j]][k]->GetListOfFunctions()->GetEntries()>0)
		{
		  TF1* fun=(TF1*)TDChistograms[indexes[j]][k]->GetListOfFunctions()->At(0);
		  y[j][k][i]=fun->GetParameter(2);
		  pos[j][k][i]=fun->GetParameter(1);
		  dy[j][k][i]=fun->GetParError(2);
		  dpos[j][k][i]=fun->GetParError(1);
		}
	    }
	}
    }
  cout<<"write to ascii"<<flush;
  for(int i=0;i<ndets;i++)
    for(int j=0;j<setup[indexes[i]].getNumberOfElements();j++)
      {
	ofstream out;
	out.open((string("sigma-")+string_number(setup[indexes[i]].getID())+"-"+string_number(j)+".txt").data());
	out<<3<<" "<<filenames.size()<<" det "<<(string_number(setup[indexes[i]].getID())+" el "+string_number(j)).data()<<endl;
	for(unsigned int k=0;k<filenames.size();k++)
	  if(dy[i][j][k]<1)
	    out<<x[k]<<" "<<y[i][j][k]<<" "<<0.5<<" "<<dy[i][j][k]<<endl;
	out.close();
	ofstream out1;
	out1.open((string("position-")+string_number(setup[indexes[i]].getID())+"-"+string_number(j)+".txt").data());
	out1<<3<<" "<<filenames.size()<<" pos: det "<<(string_number(setup[indexes[i]].getID())+" el "+string_number(j)).data()<<endl;
	for(unsigned int k=0;k<filenames.size();k++)
	  if(dpos[i][j][k]<1)
	    out1<<x[k]<<" "<<pos[i][j][k]<<" "<<0.5<<" "<<dpos[i][j][k]<<endl;
	out1.close();
      }
  gSystem->Exec("/users/data12/pion/ehrhardt/ascii2Histos/ascii2Histo sigma*.txt position*.txt -o=theGraphs.root");
  cout<<"written to \"theGraphs.root\""<<endl;
  cout<<" make graphs:"<<endl;
  TGraphErrors ***graphs;
  graphs=new TGraphErrors**[ndets*2];

  for(int i=0;i<ndets;i++)
    {
      graphs[i]=new TGraphErrors*[setup[indexes[i]].getNumberOfElements()*2];
      for(int j=0;j<setup[indexes[i]].getNumberOfElements();j++)
	{
	  graphs[i][j]=new TGraphErrors(filenames.size(),x,y[i][j],NULL,dy[i][j]);
	  graphs[i][j+setup[indexes[i]].getNumberOfElements()]=new TGraphErrors(filenames.size(),x,pos[i][j],NULL,dpos[i][j]);
	  graphs[i][j]->SetTitle((string("det ")+string_number(setup[indexes[i]].getID())+" el "+string_number(j)).data());
	  graphs[i][j+setup[indexes[i]].getNumberOfElements()]->SetTitle((string("pos det ")+string_number(setup[indexes[i]].getID())+" el "+string_number(j)).data());
	}
    }
  cout<<" print:"<<endl;
//   TPostScript ps(outfile.data(),112);
  TCanvas *canvas=new TCanvas();//"canvas","canvas",800,600);
  canvas->Clear();
  canvas->Divide(4,3);
  canvas->Print((outfile+"[").data());
    //ps.NewPage();
  int nc=0;
  for(int k=0;k<2;k++)
    for(int i=0;i<ndets;i++)
      for(int j=0;j<setup[indexes[i]].getNumberOfElements();j++)
	{
	  canvas->cd(nc+1)->Clear();
	  graphs[i][j+k*setup[indexes[i]].getNumberOfElements()]->Draw("PA");
	  nc++;
	  if(nc==12)
	    {
	      canvas->Print(outfile.data());
	      //ps.NewPage();
	      nc=0;
	    }
	}

  cout<<"close file "<<nc<<endl;
  canvas->Print(outfile.data());
  if(nc==0)canvas->Print((outfile+"]").data());
  else 
    {
      for(int i=nc;i<12;i++)canvas->cd(i+1)->Clear();
      canvas->Print((outfile+")").data());
    }
  cout<<"show file"<<endl;
  gSystem->Exec((string("gv ")+outfile).data());
  cout<<"clean up"<<endl;
  delete canvas;
  for(int i=0;i<ndets;i++)
    {
      for(int j=0;j<setup[indexes[i]].getNumberOfElements();j++)
	{
	  delete [] y[i][j];
	  delete [] pos[i][j];
	  delete [] dy[i][j];
	  delete [] dpos[i][j];
	  delete graphs[i][j];
	  delete graphs[i][j+setup[indexes[i]].getNumberOfElements()];
	}
      delete [] y[i];
      delete [] pos[i];
      delete [] dy[i];
      delete [] dpos[i];
      delete []graphs[i];
    }
  delete [] graphs;
}
void cutsMW::logTDC2D(string directory,vector<int> detectors,string outfile)
{
  if(detectors.size()==0)return;
  int ndets=0;
  int found;
  for(unsigned int i=0;i<detectors.size();i++)
    {
      found=-1;
      for(unsigned int j=0;j<setup.size();j++)
	if(setup[j].getID()==detectors[i])found=j;
      if(found>=0)ndets++;
    }
  if(ndets==0)return;
  int indexes[ndets];
  int nEl[ndets];
  ndets=0;
  for(unsigned int i=0;i<detectors.size();i++)
    {
      found=-1;
      for(unsigned int j=0;j<setup.size();j++)
	if(setup[j].getID()==detectors[i])found=j;
      if(found>=0)
	{
	  nEl[ndets]=setup[found].getNumberOfElements();
	  indexes[ndets]=found;
	  ndets++;
	}
    }
  QDir dir(QString(directory.data()),"*H.root");
  if(dir.count()<=0)return;
  vector<string> filenames;
  for(int i=0;i<(int)dir.count();i++)
    filenames.push_back(QS2s(dir[i]));

  TH2F **histos[ndets];
  bool isStart[ndets];
  for(int i=0;i<ndets;i++)
    {
      histos[i]=new TH2F*[nEl[i]];
      isStart[i]=(setup[indexes[i]].getID()==15||setup[indexes[i]].getID()==16);
      for(int j=0;j<nEl[i];j++)
	{
	  string s="TDC vs event ";
	  s=s+"det "+string_number(setup[indexes[i]].getID())+" el "+string_number(j);
	  histos[i][j]=new TH2F(s.data(),s.data(),100,0,2e6,100,(isStart[i]?-510:-520),(isStart[i]?-490:-440));
	}
    }
  TCanvas *canvas=new TCanvas();
  TCanvas *tcanvas=new TCanvas();
  canvas->Print((outfile+"(").data());
  TH1 *prof[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  canvas->Divide(4,3);
  int nc=0;
  TLeaf *nH,*t,*q,*d,*e,*en;
//   ofstream parameterLog;
//   parameterLog.open("parameterLog.txt");
  int asi[100];
  for(int i=0;i<100;i++)
    {
      asi[i]=-1;
      for(int j=0;j<ndets;j++)
	if(i==setup[indexes[j]].getID())asi[i]=j;
    }
  TF1 line("line","[0]+[1]*x/1000000");
  CommonCalibrationParser* parser[ndets];
  CommonCalibrationParser* nParser[ndets];
  for(int j=0;j<ndets;j++){parser[j]=NULL;nParser[j]=NULL;}

  for(unsigned int i=0;i<filenames.size();i++)
    {
      string s=filenames[i];
      s=s.substr(3,s.length()-6-3-4);
      int x=toInt(s);
      for(int j=0;j<ndets;j++)
	{
	  if(parser[j]!=NULL)delete parser[j];
	  if(nParser[j]!=NULL)delete nParser[j];
	  parser[j]=NULL;
	  nParser[j]=NULL;
	}
      vector<CommonCalibrationParser*> tmpParser;
      CommonCalibrationParser::getFileContent(string(getenv("KTOFPACK"))+"/data/calibration/Calibrationrun"+string_number(x)+"_0.cal",tmpParser);
      while(!tmpParser.empty())
	{
	  bool used=false;
	  for(int k=0;k<ndets;k++)
	    {
	      if(tmpParser.back()->getDetector()==setup[indexes[k]].getID()&&tmpParser.back()->getParamID()==5&&parser[k]==NULL&&!used)
		{
		  parser[k]=tmpParser.back();
		  used=true;
		}
	    }
	  if(!used)delete tmpParser.back();
	  tmpParser.pop_back();
	}
      for(int k=0;k<ndets;k++)
	{
	  nParser[k]=new CommonCalibrationParser(nEl[k],setup[indexes[k]].getID());
	  nParser[k]->setNumParamsElements(nEl[k],2);
	  nParser[k]->setParamID(5);
	  nParser[k]->setValid(true);
	  nParser[k]->setRanges(x,x);
	  if(parser[k]!=NULL)
	    {
	      for(int j=0;j<nEl[k];j++)
		nParser[k]->setParameter(j,0,parser[k]->getParameter(j,0));
	    }
	}
      cout<<filenames[i].data()<<flush;
//       parameterLog<<"run "<<x<<endl;
      readFile(directory+filenames[i]);
      if(hitTree==NULL)return;
      nH=hitTree->FindLeaf("numberOfHits");
      t=hitTree->FindLeaf("hit.tdc_c");
      q=hitTree->FindLeaf("hit.qdc_r");
      d=hitTree->FindLeaf("hit.det");
      e=hitTree->FindLeaf("hit.el");
      en=hitTree->FindLeaf("eventNumber");
      int D,E;
      if(nH==NULL||t==NULL||q==NULL||d==NULL||e==NULL||en==NULL)
	{
	  QMessageBox::warning(this,"Tree invalid","Tree has invalid leaves",QMessageBox::Cancel,QMessageBox::NoButton);
	  continue;
	}
      float neen=hitTree->GetEntries();
      for(int j=0;j<ndets;j++)
	for(int k=0;k<nEl[j];k++)
	  {
	    histos[j][k]->Reset();
	    histos[j][k]->SetAxisRange(0,neen*2,"X");
	  }
      histos[0][0]->SetTitle(string_number(x).data());

      for(int ent=0;ent<neen;ent++)
	{
	  hitTree->GetEntry(ent);
	  for(int j=0;j<nH->GetValue();j++)
	    {
	      D=(int)d->GetValue(j);
	      E=(int)e->GetValue(j);
	      if(D<0||D>=100)continue;
	      if(asi[D]<0)continue;
	      if(E<0||E>=nEl[asi[D]])continue;
	      if(t->GetValue(j)!=-1)histos[asi[D]][E]->Fill(en->GetValue(),-t->GetValue(j));
	    }
	  if(ent%(int)(neen/10.)==0)cout<<"."<<flush;
	}
      cout<<endl;
      for(int j=0;j<ndets;j++)
	{
	  for(int k=0;k<nEl[j];k++)
	    {
	      tcanvas->cd();
	      if(prof[nc]!=NULL)delete prof[nc];
	      prof[nc]=getProfile((isStart[j]?"gaus":"landau"),1,histos[j][k]);//->ProfileX();
	      canvas->cd(nc+1);
	      histos[j][k]->Draw("col");
	      line.SetParameters(-500,0);
	      prof[nc]->Fit(&line,"Q","same");
	      if(prof[nc]->GetFunction(line.GetName())->GetChisquare()/prof[nc]->GetFunction(line.GetName())->GetNDF()>10)
		prof[nc]->Fit(&line,"Q","same",prof[nc]->GetBinCenter(10),prof[nc]->GetBinCenter(90));
	      //parameterLog<<setup[indexes[j]].getID()<<" "<<k<<" : x*["<<prof[nc]->GetFunction(line.GetName())->GetParameter(1)<<"] + ["<<prof[nc]->GetFunction(line.GetName())->GetParameter(0)<<"]"<<endl;
	      nParser[j]->setParameter(k,0,nParser[j]->getParameter(k,0)-((parser[j]==NULL?0:500)+prof[nc]->GetFunction(line.GetName())->GetParameter(0)));
	      nParser[j]->setParameter(k,1,prof[nc]->GetFunction(line.GetName())->GetParameter(1));
	      nc++;
	      if(nc==12)
		{
		  nc=0;
		  canvas->Print(outfile.data());
		}
	    }
	}
      if(nc!=0)
	{
	  nc=0;
	  canvas->Print(outfile.data());
	}
      for(int ii=0;ii<12;ii++){if(prof[ii]!=NULL)delete prof[ii];prof[ii]=NULL;}
//       for(int ii=0;ii<ndets;ii++)
	//	nParser[ii]->write(parameterLog,"NEWCUTS","Oct04");
//       parameterLog<<endl;
    }
  //  parameterLog.close();
  canvas->Print(outfile.data());
//   canvas->Print((outfile+")").data());
  gSystem->Exec((string("gv ")+outfile).data());
  //cleanup
  for(int j=0;j<ndets;j++)
    {
      if(parser[j]!=NULL)delete parser[j];
      if(nParser[j]!=NULL)delete nParser[j];
    }
  delete canvas;
  delete tcanvas;
  for(int i=0;i<ndets;i++)
    {
      for(int j=0;j<nEl[i];j++)
	delete histos[i][j];
      delete [] histos[i];
    }
}
TH1 *cutsMW::getProfile(string functionName,int parNumber,TH2F* histo)
{
  if(histo==NULL)return NULL;
  if(functionName=="")return histo->ProfileX();
  TF1 function("f1",functionName.data());
  if(function.GetNumberFreeParameters()<=parNumber)parNumber=function.GetNumberFreeParameters()-1;
  TH1F *ret=new TH1F((string(histo->GetName())+"_PFx").data(),histo->GetTitle(),histo->GetNbinsX(),histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax());
  TH1F tmp("tmpHisto","tmpHisto",histo->GetNbinsY(),histo->GetYaxis()->GetXmin(),histo->GetYaxis()->GetXmax());
  for(int i=1;i<histo->GetNbinsX()+1;i++)
    {
      tmp.Reset();
      for(int j=1;j<histo->GetNbinsY()+1;j++)
	tmp.SetBinContent(j,histo->GetBinContent(i,j));
      tmp.Fit(&function,"Q","0");
      ret->SetBinContent(i,tmp.GetFunction(function.GetName())->GetParameter(parNumber));
      ret->SetBinError(i,tmp.GetFunction(function.GetName())->GetParError(parNumber));
    }
  return ret;
}
