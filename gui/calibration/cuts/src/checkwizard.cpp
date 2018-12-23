#include "checkwizard.h"
#include "utilities.h"
#include <TH1.h>
#include <TF1.h>
#include "CommonCalibrationParser.h"
#include "qcanvaswidget.h"
#include <qwidget.h>
#include <qwizard.h>
#include <qmessagebox.h>
#include <TLine.h>
#include <qlayout.h>
#include <qlabel.h>


#if QT_VERSION < 0x040000
checkWizard::checkWizard( QWidget* parent, const char* name, bool modal , WFlags fl )
:QWizard(parent,name,modal,fl)
{
  if(!name)setName("checkWizard");
#else
checkWizard::checkWizard( QWidget* parent, Qt::WindowFlags f )
:QWizard(parent,f)
{
  setObjectName("checkWizard");
#endif
  parser=NULL;
  thistograms=NULL;
  qhistograms=NULL;
  canvases=NULL;
  canvasLabels=NULL;
  parameter=new float*[4];for(int i=0;i<4;i++)parameter[i]=NULL;
  languageChange();
  resize( QSize(584, 381).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
  clearWState( WState_Polished );
#else
  setWindowState( Qt::WindowNoState);
#endif
}
checkWizard::~checkWizard()
{
}
void checkWizard::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void checkWizard::resize( const QSize & s ){resize(s.width(),s.height());}
void checkWizard::resize( int w, int h )
{
  QWizard::resize(w,h);
}
void checkWizard::setHistograms(TH1F **histoT, TH1F **histoQ, CommonCalibrationParser *params)
{
  int n=pageCount();
  QWidget *wids[n];
  for(int i=0;i<n;i++)
    wids[i]=page(i);
  for(int i=0;i<n;i++)
    {
      removePage(wids[i]);
      delete wids[i];
    }
  thistograms=histoT;
  qhistograms=histoQ;
  parser=params;
  if(parser==NULL||thistograms==NULL||qhistograms==NULL)
    {
      reject();
      return;
    }
  for(int i=0;i<4;i++)if(parameter[i]!=NULL)delete []parameter[i];
  int nEl=parser->getNumElements();
  int nPa=nEl/6;
  for(int i=0;i<4;i++)
    {
      parameter[i]=new float[nEl];
      for(int j=0;j<nEl;j++)
	parameter[i][j]=parser->getParameter(j,i)*(i==2||i==3?-1:1);
    }
  if(canvases!=NULL) delete []canvases;
  canvases=new QCanvasWidget*[nEl*2];
  if(canvasLabels!=NULL)delete []canvasLabels;
  canvasLabels=new QLabel*[nEl*2];
  TLine line;
  QWidget* currentWidget=NULL;
  QGridLayout* pageLayout=NULL;
  for(int i=0;i<nEl*2;i++)
    {
      if((i%12)==0)
	{
	  if(i>0)
	    addPage(currentWidget,(string(((i-1)/nEl==0?"QDC ":"TDC "))+" Elements "+string_number(((i)-12)%nEl)+" to "+string_number(((i)%nEl)-1)).data());
	  currentWidget=new QWidget(this);
	  pageLayout=new QGridLayout(currentWidget,6,4);
	}
      canvases[i]=new QCanvasWidget(currentWidget);
      canvasLabels[i]=new QLabel(currentWidget);
      connect(canvases[i],SIGNAL(leftMouseButtonClick(const QPoint&)),this,SLOT(canvasClick(const QPoint&)));
      connect(canvases[i],SIGNAL(rightMouseButtonClick(const QPoint&)),this,SLOT(canvasRightClick(const QPoint&)));
      connect(canvases[i],SIGNAL(doubleClicked(const QPoint&)),this,SLOT(canvasDoubleClick(const QPoint&)));
      pageLayout->addWidget(canvases[i],(i%12)/4*2,(i%12)%4);
      pageLayout->addWidget(canvasLabels[i],(i%12)/4*2+1,(i%12)%4);
    }
  if(currentWidget!=NULL)
    addPage(currentWidget,(string("TDC  Elements ")+string_number(nEl-12)+" to "+string_number(nEl)).data());

  for(int i=0;i<nEl;i++)
    {
      setParameter(i, true, true, parameter[2][i]);
      setParameter(i, false, true, parameter[0][i]);
    }
  setFinishEnabled(page(((nEl%6)>0?nPa:nPa-1)),true);
}
void checkWizard::accept()
{
  if(parser==NULL||thistograms==NULL||qhistograms==NULL||parameter==NULL)
    {
      reject();
      return;
    }
  for(int i=0;i<parser->getNumElements();i++)
    for(int j=0;j<4;j++)
      parser->setParameter(i,j,parameter[j][i]*(j>1?-1:1));
  QWizard::accept();
}
void checkWizard::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Check Cuts" ) );
#else
    setWindowTitle( tr( "Check Cuts" ) );
#endif
}
int checkWizard::getCurrentCanvas()
{
  if(canvases==NULL||parser==NULL)return -1;
  for(int i=0;i<parser->getNumElements()*2;i++)
    if(canvases[i]!=NULL)
      if(canvases[i]->hasMouse())return i;
  return -1;
}
void checkWizard::setParameter(int el, bool tdc, bool lower, float value)
{
  int p=(tdc?2:0)+(lower?0:1);
  parameter[p][el]=value;
  TLine line;
  line.SetLineColor(2);
  int nEl=parser->getNumElements();
  if(tdc)
    {
      canvasLabels[el+nEl]->setText((string_number(parameter[2][el])+" < TDC < "+string_number(parameter[3][el])).data());
      canvases[el+nEl]->cd();
      thistograms[el]->Draw();
      line.DrawLine(parameter[2][el],0,parameter[2][el],thistograms[el]->GetMaximum());
      line.DrawLine(parameter[3][el],0,parameter[3][el],thistograms[el]->GetMaximum());
      canvases[el+nEl]->Refresh();
    }
  else
    {
      canvasLabels[el]->setText((string_number(parameter[0][el])+" < QDC < "+string_number(parameter[1][el])).data());
      canvases[el]->cd();
      qhistograms[el]->Draw();
      line.DrawLine(parameter[0][el],0,parameter[0][el],qhistograms[el]->GetMaximum());
      line.DrawLine(parameter[1][el],0,parameter[1][el],qhistograms[el]->GetMaximum());
      canvases[el]->Refresh();
    }
}
void checkWizard::canvasDoubleClick(const QPoint& p)
{
  int n=getCurrentCanvas();
  if(n<0)return;
  int nEl=parser->getNumElements();
  int el=n%nEl;
  int which=1-n/nEl;
  point2D pos=canvases[n]->canvasCoordinates(p);
  QMessageBox::information(this,"double click",(string("double clicked el=")+string_number(el)+" "+(which==0?"TDC":"QDC")+"\n("+string_number(p.x())+","+string_number(p.y())+")->("+string_number(pos.x())+","+string_number(pos.y())+")").data(),QMessageBox::Ok,QMessageBox::NoButton);
}
void checkWizard::canvasClick(const QPoint &p)
{
  int n=getCurrentCanvas();
  if(n<0)return;
  int nEl=parser->getNumElements();
  int el=n%nEl;
  int which=1-n/nEl;
  point2D pos=canvases[n]->canvasCoordinates(p);
  setParameter(el, which==0,(which==0)xor true, pos.x());
}
void checkWizard::canvasRightClick(const QPoint &p)
{
  int n=getCurrentCanvas();
  if(n<0)return;
  int nEl=parser->getNumElements();
  int el=n%nEl;
  int which=1-n/nEl;
  point2D pos=canvases[n]->canvasCoordinates(p);
  setParameter(el, which==0, (which==0)xor false, pos.x());
} 
