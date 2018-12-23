#include "qcanvaswidget.h"
#include <TPoint.h>
#if QT_VERSION < 0x040000
QCanvasWidget::QCanvasWidget( QWidget* parent, const char* name, WFlags f, bool embedded):TQtWidget(parent, name, f, embedded)
#else
  QCanvasWidget::QCanvasWidget( QWidget* parent,  Qt::WindowFlags f, bool embedded):TQtWidget(parent,  f, embedded)
#endif
{
  selectedPadN=0;
}

QCanvasWidget::~QCanvasWidget()
{
}
void QCanvasWidget::cd()
{
  selectedPadN=0;
  TQtWidget::cd();
}
void QCanvasWidget::cd(int subpadnumber)
{
  selectedPadN=subpadnumber;
  TQtWidget::cd(subpadnumber);
}

int QCanvasWidget::selectedPadNumber()const
{
  return selectedPadN;
}

TPad* QCanvasWidget::selectedPad()
{
  if(selectedPadN==0)return fCanvas;
  return (TPad*)fCanvas->GetPad(selectedPadN);
}
void QCanvasWidget::Clear()
{
  selectedPadN=0;
  fCanvas->Clear();
}

void QCanvasWidget::Divide(int nx, int ny)
{
  if(nx<=0||ny<=0)return;
  selectedPadN=1;
  if(nx==1&&ny==1)selectedPadN=0;
  fCanvas->Divide(nx,ny);
}
point2D QCanvasWidget::canvasCoordinates(const QPoint &p)
{
  return canvasCoordinates(0,p);
//   double x1,x2,y1,y2;
//   GetCanvas()->GetRange(x1,y1,x2,y2);
//   int w=geometry().width(),h=geometry().height();
//   double xp=x1+((double)(p.x()))* ((double)(x2-x1))/ ((double)w);
//   double yp=y2+((double)(p.y()))* ((double)(y1-y2))/ ((double)h);
//   return point2D(xp,yp);
}
#include <iostream>
point2D QCanvasWidget::canvasCoordinates(int subPad,const QPoint &p)
{
  TVirtualPad *tmp=fCanvas->GetPad(subPad);
  if(tmp==NULL)return point2D();
  Double_t x[4];
  tmp->GetPadPar(x[0],x[1],x[2],x[3]);
  int w=(int)(((double)geometry().width())*((x[2]-x[0]))),
    h=(int)(((double)geometry().height())*((x[3]-x[1])));
  QPoint ul((int)(geometry().width()*(x[0])),(int)(geometry().height()*(x[1])));
  float ndcx=((double)(p.x()-ul.x()))/((double)w),
    ndcy=((double)(p.y()-ul.y()))/((double)h);
  double x1,x2,y1,y2;
  tmp->GetRange(x1,y1,x2,y2);
  double xp=x1+ndcx*((double)(x2-x1));
  double yp=y1+(1-ndcy)*((double)(y2-y1));
  point2D ret;
  ret.setValues(xp,yp);
  return ret;//xpoint2D(xp,yp);
}
QPoint QCanvasWidget::canvasCoordinates(float x, float y)
{
  Double_t xmin,xmax,ymin,ymax;
  fCanvas->GetRange(xmin,ymin,xmax,ymax);
  int w=geometry().width(), h=geometry().height();
  int xx,yy;
  if(x<=xmin)xx=0;
  else if(x>=xmax)xx=w;
  else
    xx=(int)((x-xmin)*w/(xmax-xmin));
  if(y<=ymin)yy=0;
  else if(y>=ymax)yy=h;
  else
    yy=(int)((ymax-y)*h/(ymax-ymin));
  return QPoint(xx,yy);
}
QPoint QCanvasWidget::canvasCoordinates(const point2D &p)
{
  return canvasCoordinates(p.x(),p.y());
}

void QCanvasWidget::enterEvent ( QEvent * e)
{
}

void QCanvasWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    TQtWidget::mouseDoubleClickEvent(e);
    int subPad=subPadNumberAt(e->pos());
    emit doubleClicked(subPad,e->pos());
    emit doubleClicked(e->pos());
}

void QCanvasWidget::mouseMoveEvent   ( QMouseEvent * e)
{
    TQtWidget::mouseMoveEvent(e);
    int subPad=subPadNumberAt(e->pos());
    emit mouseMooved(subPad,e->pos());
    emit mouseMooved(e->pos());
}

void QCanvasWidget::mousePressEvent  ( QMouseEvent * e)
{
    TQtWidget::mousePressEvent(e);
    if(e->button()==Qt::LeftButton)
      {
	emit leftMouseButtonPressed(e->pos());
      }
    else if(e->button()==Qt::RightButton)
      {
	emit rightMouseButtonPressed(e->pos());
      }
}

void QCanvasWidget::mouseReleaseEvent( QMouseEvent * e)
{
    TQtWidget::mouseReleaseEvent(e);
    int subPad=subPadNumberAt(e->pos());
    if (e->button()==Qt::LeftButton)
      {
	emit leftMouseButtonClick(e->pos());
	emit leftMouseButtonClick(subPad,e->pos());
      }
    else if(e->button()==Qt::RightButton)
      {
	emit rightMouseButtonClick(e->pos());
	emit rightMouseButtonClick(subPad,e->pos());
      }
}

void QCanvasWidget::keyPressEvent    ( QKeyEvent *   e)
{
    TQtWidget::keyPressEvent(e);
    emit keyPressed();
}

void QCanvasWidget::keyReleaseEvent  ( QKeyEvent *   e)
{
    TQtWidget::keyReleaseEvent(e);
    emit keyPressed();
}
#include <TList.h>
void QCanvasWidget::wheelEvent ( QWheelEvent * e )
{
  int subPad=subPadNumberAt(e->pos());
  emit mouseWheeled(subPad, e->delta()>0,e->pos());
  emit mouseWheeled(e->delta()>0,e->pos());
  e->accept();
}

int QCanvasWidget::subPadNumberAt(const QPoint &p)
{
  float ndcx=((double)p.x())/((double)width()),
    ndcy=1.-((double)p.y())/((double)height());
  TList *lop=GetCanvas()->GetListOfPrimitives();
  for(int i=0;i<lop->GetEntries();i++)
    {
      if(lop->At(i)->IsA()==TPad::Class()||
	 lop->At(i)->IsA()==TVirtualPad::Class())
	{
	  Double_t x1,x2,y1,y2;
	  ((TVirtualPad*)lop->At(i))->GetPadPar(x1,y1,x2,y2);
	  if(x1<=ndcx&&x2>=ndcx&&y1<=ndcy&&y2>=ndcy)
	    return ((TVirtualPad*)lop->At(i))->GetNumber();
	}
    }  
  return 0;
}
int QCanvasWidget::subPadNumberAt(const point2D &p)
{
  Double_t x[4];
  GetCanvas()->GetRange(x[0],x[1],x[2],x[3]);
  float ndcx=(p.x()-x[0])/(x[2]-x[0]),
    ndcy=1.-(p.y()-x[1])/(x[3]-x[1]);
  TList *lop=GetCanvas()->GetListOfPrimitives();
  for(int i=0;i<lop->GetEntries();i++)
    {
      if(lop->At(i)->IsA()==TPad::Class()||
	 lop->At(i)->IsA()==TVirtualPad::Class())
	{
	  Double_t x1,x2,y1,y2;
	  ((TVirtualPad*)lop->At(i))->GetPadPar(x1,y1,x2,y2);
	  if(x1<=ndcx&&x2>=ndcx&&y1<=ndcy&&y2>=ndcy)
	    return ((TVirtualPad*)lop->At(i))->GetNumber();
	}
    }  
  return 0;
}
TVirtualPad* QCanvasWidget::subPadAt(const QPoint &p)
{
  float ndcx=((double)p.x())/((double)width()),
    ndcy=1.-((double)p.y())/((double)height());
  TList *lop=GetCanvas()->GetListOfPrimitives();
  for(int i=0;i<lop->GetEntries();i++)
    {
      if(lop->At(i)->IsA()==TPad::Class()||
	 lop->At(i)->IsA()==TVirtualPad::Class())
	{
	  Double_t x1,x2,y1,y2;
	  ((TVirtualPad*)lop->At(i))->GetPadPar(x1,y1,x2,y2);
	  if(x1<=ndcx&&x2>=ndcx&&y1<=ndcy&&y2>=ndcy)
	    return ((TVirtualPad*)lop->At(i));
	}
    }  
  return GetCanvas();
}
TVirtualPad* QCanvasWidget::subPadAt(const point2D &p)
{
  Double_t x[4];
  GetCanvas()->GetRange(x[0],x[1],x[2],x[3]);
  float ndcx=(p.x()-x[0])/(x[2]-x[0]),
    ndcy=1.-(p.y()-x[1])/(x[3]-x[1]);
  TList *lop=GetCanvas()->GetListOfPrimitives();
  for(int i=0;i<lop->GetEntries();i++)
    {
      if(lop->At(i)->IsA()==TPad::Class()||
	 lop->At(i)->IsA()==TVirtualPad::Class())
	{
	  Double_t x1,x2,y1,y2;
	  ((TVirtualPad*)lop->At(i))->GetPadPar(x1,y1,x2,y2);
	  if(x1<=ndcx&&x2>=ndcx&&y1<=ndcy&&y2>=ndcy)
	    return ((TVirtualPad*)lop->At(i));
	}
    }  
  return GetCanvas();
}
// bool QCanvasWidget::hasFocus()const
// {

// }
