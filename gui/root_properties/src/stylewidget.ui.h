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
#include "stylewidget.h"

#include <qvariant.h>
#include <TStyle.h>
#include <qtoolbox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
//void StyleWidget::init()
//{
//    histF=new AttributeWidget(toolBox1->item(2));
//    histL=new AttributeWidget(toolBox1->item(2));
//    func=new AttributeWidget(toolBox1->item(2));
//    stat=new AttributeWidget(toolBox1->item(2));
//    histF->setMode(0);
//    histL->setMode(1);
//    func->setMode(1);
//    stat->setMode(3);
//    
//    Xaxis, Yaxis, Zaxis Axis;
//    
//    defaultFill=new AttributeWidget(toolBox1->item(4));
//    defaultLine=new AttributeWidget(toolBox1->item(4));
//    defaultText =new AttributeWidget(toolBox1->item(4));
//    defaultMarker =new AttributeWidget(toolBox1->item(4));
//    defaultFill->setMode(0);
//    defaultLine->setMode(1);
//    defaultText->setMode(3);
//    defaultMarker->setMode(2);
//    
//    title=new AttributeWidget(toolBox1->item(3)); 
//    title->setMode(3);
//    
//    Date=new AttributeWidget(toolBox1->item(3)); 
//    Date->setMode(3);
//    datePos, 
//    
//    grid =new AttributeWidget(toolBox1->item(3)); 
//    grid->setMode(1);
//    frameBorder=new AttributeWidget(toolBox1->item(3)); 
//    frameLine =new AttributeWidget(toolBox1->item(3)); 
//    frameFill=new AttributeWidget(toolBox1->item(3)); 
//    frameBorder->setMode(1);
//    frameLine->setMode(1);
//    frameFill->setMode(0);
//    QRect canvasDef;
//    pad, canvas Pad;
//    
//Float_t	GetBarOffset() const
//Float_t	GetBarWidth() const
//Bool_t	GetCanvasPreferGL() const
//Int_t	GetColorModelPS() const
//Int_t	GetColorPalette(Int_t i) const
//Float_t	GetDateX() const
//Float_t	GetDateY() const
//Int_t	GetDrawBorder() const
//virtual Option_t*	TObject::GetDrawOption() const
//static Long_t	TObject::GetDtorOnly()
//Float_t	GetEndErrorSize() const
//Float_t	GetErrorX() const
//const char*	GetFitFormat() const
//
//Int_t	GetHatchesLineWidth() const
//Double_t	GetHatchesSpacing() const
//const char*	GetHeaderPS() const
//Bool_t	GetHistMinimumZero() const
//Width_t	GetLegendBorderSize() const
//Float_t	GetLegoInnerR() const
//Float_t	GetLineScalePS() const
//const char*	GetLineStyleString(Int_t i = 1) const
//Int_t	GetNumberContours() const
//Int_t	GetNumberOfColors() const
//
//Int_t	GetOptDate() const
//Int_t	GetOptFile() const
//Int_t	GetOptFit() const
//Int_t	GetOptLogx() const
//Int_t	GetOptLogy() const
//Int_t	GetOptLogz() const
//Int_t	GetOptStat() const
//Int_t	GetOptTitle() const
//
//Bool_t	GetPadGridX() const
//Bool_t	GetPadGridY() const
//Int_t	GetPadTickX() const
//Int_t	GetPadTickY() const
//
//const char*	GetPaintTextFormat() const
//void	GetPaperSize(Float_t& xsize, Float_t& ysize)
//Float_t	GetScreenFactor() const
//I
//Width_t	GetStatBorderSize() const
//const char*	GetStatFormat() const
//Style_t	GetStatStyle() const
//Float_t	GetStatH() const
//Float_t	GetStatW() const
//Float_t	GetStatX() const
//Float_t	GetStatY() const
//
//Int_t	GetStripDecimals() const
//Double_t	GetTimeOffset() const
//
//Width_t	GetTitleBorderSize() const
//Color_t	GetTitleFillColor() const
//const char*	GetTitlePS() const
//Style_t	GetTitleStyle() const
//Float_t	GetTitleH() const
//Float_t	GetTitleW() const
//Float_t	GetTitleX() const
//Float_t	GetTitleY() const
//Float_t	GetTitleXOffset() const
//Float_t	GetTitleXSize() const
//Float_t	GetTitleYOffset() const
//Float_t	GetTitleYSize() const
void StyleWidget::init()
{
}
void StyleWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void StyleWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void StyleWidget::resize(int w, int h)
{
    QWidget::resize(w,h);
}
