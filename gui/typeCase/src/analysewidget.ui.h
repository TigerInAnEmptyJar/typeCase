#include <qmessagebox.h>
#include "string_utilities.h"
void *thread_run(void* ptr)
{
  void **pttr=(void**)ptr;
  bool step=*((bool*)pttr[2]);
  int nE=*((int*)pttr[1]);
  tofAnalysis *ana=((tofAnalysis*)pttr[0]);
  if(step)
    ana->step(nE);
  else
    ana->process();
  return NULL;
}
void AnalysisWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void AnalysisWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void AnalysisWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
  int bw=75;
  int bh=25;
  int abp=h-8*bh;
  int abt=abp/9;
  if(abt>bh)abt=bh;
  abp=(abp-abt*7)/2;
  initButton->setGeometry(1,abt*1+bh*0,bw,bh);
  startButton->setGeometry(1,abt*2+bh*1,bw,bh);
  stepButton->setGeometry(1,abt*3+bh*2,bw,bh);
  stopButton->setGeometry(1,abt*4+bh*3,bw,bh);
  finalButton->setGeometry(1,abt*5+bh*4,bw,bh);
  helpButton->setGeometry(1,abt*5+abp+bh*5,bw,bh);
  orderButton->setGeometry(1,abt*5+abp*2+bh*6,bw,bh);
  displayButton->setGeometry(1,abt*6+abp*2+bh*7,bw,bh);
  stateButton->setGeometry(bw+3,h-bh-3,bh,bh);
  eventsInput->setGeometry(bw+3+bh+5,h-bh-3,w-208-bw-bh,bh);
  eventsLabel->setGeometry(w-180,h-bh-3,60,bh);
  allInput->setGeometry(w-105,h-bh-3,100,bh);
  canvas->setGeometry(bw+3,1,w-bw-4,h-bh-5);
}
void AnalysisWidget::algorithmSelected(algorithm_parameter &algoIn, bool value)
{
  order->algorithmSelect(algoIn,value);
  return;
  int n=-1;
  for(unsigned int i=0;i<selectedAlgorithms.size();i++)
    if(selectedAlgorithms[i]==&algoIn)n=i;
  if(value)
    {
      if(n<0)selectedAlgorithms.push_back(&algoIn);
    }
  else
    {
      if(n>=0)
	{
	  vector<algorithm_parameter*> tmp;
	  while((int)selectedAlgorithms.size()<=n)
	    {
	      tmp.push_back(selectedAlgorithms.back());
	      selectedAlgorithms.pop_back();
	    }
	  selectedAlgorithms.pop_back();
	  while(!tmp.empty())
	    {
	      selectedAlgorithms.push_back(tmp.back());
	      tmp.pop_back();
	    }
	}
    }
  if(isInitialized) stateButton->setColor(6);
}
void AnalysisWidget::runSelected(run_parameter *run,bool value)
{
  if(run==NULL)return;
  //  QMessageBox::information(this,"Run selected",(string("Run \"")+run->getName()+" "+(value?"selected.":"unselected.")).data(),QMessageBox::Ok,QMessageBox::NoButton);
  int n=-1;
  for(unsigned int i=0;i<selectedRuns.size();i++)
    if(selectedRuns[i]->getName()==run->getName())n=i;
  if(value)
    {
      if(n<0)selectedRuns.push_back(run);
    }
  else
    {
      if(n>=0)
	{
	  vector<run_parameter*> tmp;
	  while((int)selectedRuns.size()>n+1)
	    {
	      tmp.push_back(selectedRuns.back());
	      selectedRuns.pop_back();
	    }
	  selectedRuns.pop_back();
	  while(!tmp.empty())
	    {
	      selectedRuns.push_back(tmp.back());
	      tmp.pop_back();
	    }
	}
    }
  allInput->setText((string("all ")+string_number((int)selectedRuns.size())+" run(s)").data());
  if(isInitialized)
    {
      if(value)
	analyser.addRun(*run);
      else
	analyser.removeRun(*run);
    }
}
void AnalysisWidget::algorithmsChanged()
{
  if(isInitialized) stateButton->setColor(6);
}
void AnalysisWidget::drawChanged()
{
}
void AnalysisWidget::initClick()
{
  if(isInitialized)
    {
      QMessageBox::warning(this,"Initialisation","Analyser is initialized.\nContinue with start or step.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  if(selectedRuns.size()<=0)
    {
      QMessageBox::warning(this,"Initialisation","No runs selected.\nGo to data-basis-widget and select run(s).",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  string tmps="";
  vector<algorithm_parameter*> oap=order->getOrder();
  for(unsigned int i=0;i<oap.size();i++)
    tmps+=oap[i]->getName()+"\n";
  QMessageBox::information(this,"initialize analyser",tmps.data(),QMessageBox::Ok,QMessageBox::NoButton);
  vector<algorithm_parameter> ap;
  vector<run_parameter> rp;
  vector<beamTime_parameter> bp;
  vector<detector_parameter> dp;
  vector<material_parameter> mp;
  reaction_parameter co;
  //copy selected runs and algorithms
  for(unsigned int i=0;i<oap.size();i++)ap.push_back(*(oap[i]));
/*   for(unsigned int i=0;i<selectedAlgorithms.size();i++)ap.push_back(*(selectedAlgorithms[i])); */
  int n;
  for(unsigned int i=0;i<selectedRuns.size();i++)
    {
      rp.push_back(*(selectedRuns[i]));
      n=-1;
      for(unsigned int j=0;j<bp.size();j++)if(bp[j].getName()==selectedRuns[i]->getParent()->getName())n=j;
      if(n<0)
	{
	  n=bp.size();
	  bp.push_back(*(selectedRuns[i]->getParent()));
	}
      rp.back().setParent(&bp[n]);
    }
  //read setup of first run
  if(rp[0].hasOwnSetup())
    parameterManager::readDetectorParameter_ascii(rp[0].getSetupFile(),dp,co);
  else
    parameterManager::readDetectorParameter_ascii(rp[0].getParent()->getSetupFile(),dp,co);
  if(rp[0].hasOwnSetup())
    cout<<rp[0].getSetupFile().data()<<" "<<dp.size()<<endl;
  else
    cout<<rp[0].getParent()->getSetupFile().data()<<" "<<dp.size()<<endl;
  parameterManager::readMaterialParameter_ascii(string(getenv("KTOFPACK"))+"/data/detectors/materials.data",mp);
  //inititialize
  analyser.initStep(ap, bp, rp, dp, mp, co);
  stateButton->setColor(5);
  isInitialized=true;
  watch->analyserInitialized(&analyser);
}
void AnalysisWidget::startClick()
{
  if(!isInitialized)
    {
      QMessageBox::warning(this,"Initialisation","Analyser is not initialized.\nPress init first.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  if(isRunning)
    {
      QMessageBox::warning(this,"Running Analysis","Analyser is running.\nContinue with stop or final.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  pointer[0]=&analyser;
  int nE=eventsInput->text().toInt();
  watch->setAnalysisParameter(nE,0,NULL);
  pointer[1]=&nE;
  bool isStep=false;
  pointer[2]=&isStep;
  stateButton->setColor(3);
  isRunning=true;
/*   for(unsigned int i=0;i<pads.size();i++)pads[i].drawPad=canvas->GetCanvas()->GetPad(pads.size()==1?1:1+i); */
  updater.start(2000);
  pthread_create(&analyserThread,NULL,thread_run,pointer);
}
void AnalysisWidget::stepClick()
{
  if(!isInitialized)
    {
      QMessageBox::warning(this,"Initialisation","Analyser is not initialized.\nPress init first.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  if(isRunning)
    {
      QMessageBox::warning(this,"Running Analysis","Analyser is running.\nContinue with stop or final.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  pointer[0]=&analyser;
  int nE=eventsInput->text().toInt();
  watch->setAnalysisParameter(nE,0,NULL);
  pointer[1]=&nE;
  bool isStep=true;
  pointer[2]=&isStep;
  stateButton->setColor(3);
  isRunning=true;
  updater.start(2000);
/*   for(unsigned int i=0;i<pads.size();i++)pads[i].drawPad=(TPad*)canvas->GetCanvas()->GetPad(pads.size()==1?1:1+i); */
  pthread_create(&analyserThread,NULL,thread_run,pointer);
}
void AnalysisWidget::stopClick()
{
  if(!isRunning)
    {
      QMessageBox::warning(this,"Running Analysis","Analyser is not running.\nContinue with start or step.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  stateButton->setColor(5);
  isRunning=false;
  updater.stop();
  analyser.stopAnalysis();
  updateCanvas();
}
void AnalysisWidget::stopped()
{
  stateButton->setColor(5);
  isRunning=false;
  updater.stop();
  updateCanvas();
}
void AnalysisWidget::finalClick()
{
  if(!isInitialized)
    {
      QMessageBox::warning(this,"Initialisation","Analyser is not initialized.\nInitialize, run and stop analyser first.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  if(isRunning)
    {
      QMessageBox::warning(this,"Running Analysis","Analyser is running.\nContinue with stop.",QMessageBox::Ok,QMessageBox::NoButton);
      return;
    }
  analyser.endStep();
  stateButton->setColor(2);
  isInitialized=false;
}
void AnalysisWidget::helpClick()
{
  emit helpRequested((string(getenv("HOME"))+"/WWW/KTOF/analysis.html").data());
}
void AnalysisWidget::orderClick()
{
  order->show();
}
void AnalysisWidget::displayClick()
{
  watch->show();
}
void AnalysisWidget::init()
{
  parameterManager::init();
  stateButton->setColor(2);
  pointer=new void*[3];
  connect(&analyser,SIGNAL(analysisFinished(int)),this,SLOT(stopped()));
  connect(&analyser,SIGNAL(changeEvent(TEvent *)),this,SLOT(updateEvent(TEvent*)));
  connect(&updater,SIGNAL(timeout()),this,SLOT(updateCanvas()));
  pads.push_back(pad_parameter());
  //  pads.push_back(pad_parameter());
  watch->setPads(pads,1,1);
  watch->setCanvas(canvas->GetCanvas());
/*   projector::setProjector(plane3D(point3D(0,0,1),vector3D(0,0,1)),point3D(0,0,0),projector::central); */
    projector::setProjector(plane3D(point3D(0,0,1000000),vector3D(1,0,0),vector3D(0,1,0)),point3D(0,0,0),projector::central);
}
void AnalysisWidget::destroy()
{
  disconnect(&analyser,SIGNAL(changeEvent(TEvent *)),this,SLOT(updateEvent(TEvent*)));
  if(isRunning)
    analyser.stopAnalysis();
  if(isInitialized)
    analyser.endStep();
  delete [] pointer;
}
void AnalysisWidget::closeEvent(QCloseEvent *e)
{
  QWidget::closeEvent(e);
  order->close();
  watch->close(); 
  close();
}
void AnalysisWidget::updateCanvas()
{
  for(unsigned int i=0;i<pads.size();i++)
    pads[i].draw();
  canvas->Refresh();
}
void AnalysisWidget::updateEvent(TEvent *e)
{
  for(unsigned int i=0;i<pads.size();i++)
    {
      pads[i].eventPointer=e;
    }
}
void AnalysisWidget::canvasScrolled(int subPad, bool up, const QPoint &p)
{
  float oldzoom=1;
  float newzoom=1;
  TVirtualPad *tmp=NULL;
  if(subPad==0)
    {
      oldzoom=pads[0].zoom;
      tmp=pads[0].drawPad;
      pads[0].zoom=pads[0].zoom*(up?0.95:1.05);
      newzoom=pads[0].zoom;
    }
  else
    {
      oldzoom=pads[subPad-1].zoom;
      tmp=pads[subPad-1].drawPad;
      pads[subPad-1].zoom=pads[subPad-1].zoom*(up?0.95:1.05);
      newzoom=pads[subPad-1].zoom;
    }
  if(tmp==NULL)return;
  Double_t x[4];
  tmp->GetRange(x[0],x[1],x[2],x[3]);
  tmp->Range(x[0]*newzoom/oldzoom,x[1]*newzoom/oldzoom,x[2]*newzoom/oldzoom,x[3]*newzoom/oldzoom);
  tmp->Update();
}
#include <TPoint.h>
void AnalysisWidget::canvasDoubleClicked(int subPad, const QPoint &p)
{
  point2D pp=canvas->canvasCoordinates(subPad,p);
  TVirtualPad *tmp=NULL;
  if(subPad==0)
    {
      tmp=pads[0].drawPad;
      pads[0].center=/*pads[0].center-vector*/point2D(pp.X(),pp.Y());
    }
  else
    {
      tmp=pads[subPad-1].drawPad;
      pads[subPad-1].center=/*pads[subPad-1].center-vector*/point2D(pp.X(),pp.Y());
    }
  if(tmp==NULL)return;
  Double_t x[4];
  tmp->GetRange(x[0],x[1],x[2],x[3]);
  double w=(x[2]-x[0])*0.5,h=(x[3]-x[1])*0.5;
  tmp->Range(pp.X()-w,pp.Y()-h,pp.X()+w,pp.Y()+h);
  tmp->Update();
}
void AnalysisWidget::canvasMouseMove(int subPad, const QPoint &p)
{
  point2D pp=canvas->canvasCoordinates(subPad,p);
  vector<TBase*> onPoint=pads[subPad==0?0:subPad-1].painter.getDrawnObjects(pp.X(),pp.Y());
  point3D f=projector::plane().Foot();
  point2D pit=pp/f.Z();
  point3D po=point3D(pit.X(),pit.Y(),1);
  //  cout<<po<<"\n\t("<<p.x()<<","<<p.y()<<")\n\t("<<pp.GetX()<<","<<pp.GetY()<<")\n"<<endl;
  string tip=string("theta: ")+string_number(po.theta()/M_PI*180.)+", phi: "+string_number(po.phi()/M_PI*180.);
  for(unsigned int i=0;i<onPoint.size();i++)
    {
      if(onPoint[i]==NULL)continue;
      if(onPoint[i]->name()=="TCalibHit")
	{
 	  TCalibHit* hit=(TCalibHit*)onPoint[i]; 
	  tip=tip+"\ndet "+string_number(hit->getDetector())+", el "+string_number(hit->getElement())
	    +": "+(hit->getHitShape()!=NULL?hit->getHitShape()->getName():"none");
	}
      else if(onPoint[i]->name()=="TPixel")
	{
 	  TPixel* pix=(TPixel*)onPoint[i]; 
	  tip=tip+"\npixel "+string_number(pix->getID())+":"+(pix->getShape()!=NULL?pix->getShape()->getName()+" "+pix->getShape()->getCenter().toString(_spheric_):"invalid");
	}
      else if(onPoint[i]->name()=="TTrack")
	{
 	  TTrack* trc=(TTrack*)onPoint[i]; 
	  tip=tip+"\nTrack (";
	  int n=0;
	  for(int k=0;k<trc->getNumberOfCalibHits(-2);k++)
	    {
	      if(trc->getNumberOfCalibHits(k)>0)tip=tip+(n>0?",":"")+string_number(k);
	      if(trc->getNumberOfCalibHits(k)>1)tip=tip+"+";
	      n=n+trc->getNumberOfCalibHits(k);
	    }
	  tip=tip+")"+string_number(n)+", beta: "+string_number(trc->getSpeed());
	}
    }
#if QT_VERSION < 0x040000
  QToolTip::remove(canvas);
  QToolTip::add(canvas,tip.data());
#else
  canvas->setToolTip(tip.data());
#endif
}
void AnalysisWidget::readAnalysisParameter(const string &filename)
{
  int nx=1,ny=1;
  ifstream out;
  out.open(filename.data());
  string s="";
  char c;
  out.get(c);
  while(c!='\n')
    {s=s+c;out.get(c);}
  if(!hasA(s,"Analyse-parameter")){out.close();return;}
  out>>nx>>nx;
  out.get(c);
  while(c!='\n')
    {s=s+c;out.get(c);}
  while(!pads.empty())pads.pop_back();
  out.get(c);
  while(c=='P')
    {
      pad_parameter pa;
      out>>pa;
      pads.push_back(pa);
      out.get(c);
    }
  while(c=='R')
    {
      s="";
      out.get(c);while(c!='\n'&&!out.eof()){s=s+c;out.get(c);}
      emit requestRun(s,true);
      out.get(c);
    }
  vector<string> algonames;
  while(c=='A')
    {
      s="";
      out.get(c);while(c!='\n'&&!out.eof()){s=s+c;out.get(c);}
      algonames.push_back(s);
      out.get(c);
    }
  for(unsigned int i=0;i<selectedAlgorithms.size();i++)
    {
      if(selectedAlgorithms[i]==NULL)continue;
      bool found=false;
      for(unsigned int j=0;j<algonames.size();j++)
	if(algonames[j]==selectedAlgorithms[i]->getName())found=true;
      if(!found)
	{
	  emit algorithmSelect(*selectedAlgorithms[i], false);
	  algorithmSelected(*selectedAlgorithms[i], false);
	  i--;
	}
    }
  out.close();
  canvas->Clear();
  if(nx>1||ny>1)
    canvas->Divide(nx,ny);
  for(int j=0;j<ny;j++)
    for(int i=0;i<nx;i++)
      pads[j*nx+i].drawPad=canvas->GetCanvas()->GetPad((ny==1&&nx==1?0:j*nx+i+1));
  watch->setPads(pads,nx,ny);
}
void AnalysisWidget::writeAnalysisParameter(const string &filename)
{
  int nx=1,ny=1;
  ofstream out;
  out.open(filename.data());
  out<<"############## Analyse-parameter######################"<<endl;
  out<<nx<<" "<<ny<<" "<<endl;
  for(unsigned int i=0;i<pads.size();i++)
    {
      out<<"P";
      out<<pads[i];
    }
  for(unsigned int i=0;i<selectedRuns.size();i++)
    {
      if(selectedRuns[i]!=NULL)
	out<<"R"<<selectedRuns[i]->getName().data()<<endl;
    }
  for(unsigned int i=0;i<selectedAlgorithms.size();i++)
    {
      if(selectedAlgorithms[i]!=NULL)
	if(selectedAlgorithms[i]->IsUsed())
	  out<<"A"<<selectedAlgorithms[i]->getName().data()<<endl;
    }
  out<<"="<<endl;
  out.close();
  emit analysisParameterWritten(filename);
}
