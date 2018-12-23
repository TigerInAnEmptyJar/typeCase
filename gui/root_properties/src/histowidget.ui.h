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
Contour
Entries => effective Entries
Functions
Integral
Axis, nBins
Maximum maximumBin, Minimum minimumBin
RMS, mean, 

*****************************************************************************/
#include "histowidget.h"

#include <qvariant.h>
#include <TProfile2D.h>
#include <TGraphErrors.h>
#include <TGraph2DErrors.h>
#include <iostream>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <attributewidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtoolbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "attributewidget.h"
void axisToAxis(const TAttAxis &source, TAttAxis &target);
void axisToAxis(const TAxis &source, TAttAxis &target);
void axisToAxis(const TAttAxis &source, TAxis &target);
void HistoWidget::init()
{
    histogram=NULL;
    advAttributeWidget1=new AAttributeWidget(groupBox4);
    advAttributeWidget1->setMode(1);
    textLabel22->setText("0");
    setMinimumWidth(370+100+60);
    setMinimumHeight(530);    
}
void HistoWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void HistoWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void HistoWidget::resize(int w, int h)
{
    QWidget::resize(w,h);
    int bH=25, lH=20, h1=bH*3+lH,h2=bH*2+lH,h3=lH+bH*3+140+lH*2+bH*3;
    int minHeight=bH*2+h1*2+h2+bH+bH*2;
    if(h1*2+h2<h3)minHeight=bH*2+h3+bH+bH*2;
    if(h<minHeight)return;
    int abt=0;
    if(h>minHeight)abt=(h-minHeight)/27;
    textLabel1->setGeometry(10,abt*2,40,lH);//name
    lineEdit1->setGeometry(60,abt*2,w-70,bH);
    textLabel2->setGeometry(10,abt*3+bH,40,lH);//title
    lineEdit2->setGeometry(60,abt*3+bH,w-70,bH);
    pushButton1->setGeometry(10,h-bH-abt*2,70,bH);//buttons
    pushButton2->setGeometry(90,h-bH-abt*2,70,bH);
    int tbH=h-bH*3-abt*7;
    toolBox1->setGeometry(0,abt*4+bH*2,w,tbH);
    int gBH=tbH-bH*3-abt;
    groupBox4->setGeometry(10,0,(w-30)/4*3,gBH);//axes
    comboBox6->setGeometry(10,lH,(w-30)/3*2-20,bH);
    textLabel21->setGeometry(10,lH+bH+abt,40,lH);
    textLabel22->setGeometry(50,lH+bH+abt,(w-30)/3*2-60,lH);
    textLabel23->setGeometry(10,lH*2+bH+abt*2,40,lH);
    lineEdit3->setGeometry(50,lH*2+bH+abt*2,(w-30)/3*2-60,lH);
    if(!textLabel22->text().contains("____________"))
	    advAttributeWidget1->setGeometry(1,lH*2+bH*2+abt*3,(w-30)/4*3-2,\
					     gBH-(lH*2+bH*2+abt*3+1));
    checkBox1->setGeometry(10,gBH+abt,(w-30)/4*3-20,bH);
    int abt1=(tbH-bH*10-lH*3)/21;
    gBH=lH+bH*3+abt1*6;
    groupBox1->setGeometry((w-30)/4*3+20,0,(w-30)/4,gBH);//line
    attributeWidget1->setGeometry(1,lH,(w-30)/4-2,gBH-lH-2);
    groupBox2->setGeometry((w-30)/4*3+20,gBH+abt1,(w-30)/4,gBH);//marker
    attributeWidget2->setGeometry(1,lH,(w-30)/4-2,gBH-lH-2);
    int gBH1=lH+bH*2+abt1*5;
    groupBox3->setGeometry((w-30)/4*3+20,gBH*2+abt1*2,(w-30)/4,gBH1);//fill
    attributeWidget3->setGeometry(1,lH,(w-30)/4-2,gBH1-lH-2);
    int abt2=(tbH-bH*2-lH*8)/11;
    textLabel2_1->setGeometry(10,abt2*2,60,lH);//info
    textLabel1_2->setGeometry(70,abt2*2,w-80,lH);
    textLabel1_3->setGeometry(10,abt2*3+lH,60,lH);
    textLabel1_4->setGeometry(70,abt2*3+lH,w-80,lH);
    textLabel1_5->setGeometry(10,abt2*4+lH*2,60,lH);
    textLabel1_6->setGeometry(70,abt2*4+lH*2,w-80,lH);
    textLabel1_7->setGeometry(10,abt2*5+lH*3,60,lH);
    textLabel1_8->setGeometry(70,abt2*5+lH*3,w-80,lH);
    textLabel1_9->setGeometry(10,abt2*6+lH*4,60,lH);
    textLabel1_10->setGeometry(70,abt2*6+lH*4,w-80,lH);
    textLabel1_11->setGeometry(20,abt2*7+lH*5,60,lH);
    textLabel1_12->setGeometry(70,abt2*7+lH*5,w-80,lH);
    textLabel1_13->setGeometry(10,abt2*8+lH*6,60,lH);
    textLabel1_14->setGeometry(70,abt2*8+lH*6,w-80,lH);
    textLabel1_15->setGeometry(20,abt2*9+lH*7,60,lH);
    textLabel1_16->setGeometry(70,abt2*9+lH*7,w-80,lH);
}
void HistoWidget::getHisto(TObject *histo)
{
    if(histo==NULL)return;
    lineEdit1->setText(histo->GetName());
    lineEdit2->setText(histo->GetTitle());
    if(!(histo->IsA()==TH1F::Class()||histo->IsA()==TH2F::Class()||
	 histo->IsA()==TProfile::Class()||histo->IsA()==TProfile2D::Class()||
	 histo->IsA()==TGraph::Class()||histo->IsA()==TGraph2D::Class()||
	 histo->IsA()==TGraphErrors::Class()||histo->IsA()==TGraph2DErrors::Class()))
	return;
    TAxis *x=NULL,*y=NULL,*z=NULL;
    if(histo->IsA()==TH1F::Class()||histo->IsA()==TH2F::Class()||
	 histo->IsA()==TProfile::Class()||histo->IsA()==TProfile2D::Class())
    {
	textLabel1_2->setText(QString::number(((TH1*)histo)->GetEntries()));
//	QToolTip::remove(textLabel1_2);
//	QToolTip::add(textLabel1_2,QString::number(((TH1*)histo)->GetEffectiveEntries()));
	textLabel1_4->setText(QString::number(((TH1*)histo)->GetMean()));
	textLabel1_6->setText(QString::number(((TH1*)histo)->GetRMS()));
	textLabel1_8->setText(QString::number(((TH1*)histo)->Integral()));
	textLabel1_10->setText(QString::number(((TH1*)histo)->GetMinimum()));
	textLabel1_12->setText(QString::number(((TH1*)histo)->GetMinimumBin()));
	textLabel1_14->setText(QString::number(((TH1*)histo)->GetMaximum()));
	textLabel1_16->setText(QString::number(((TH1*)histo)->GetMaximumBin()));
	if(histo->IsA()==TH2F::Class()||histo->IsA()==TProfile2D::Class())
	{
	    textLabel1_4->setText(QString::number(((TH1*)histo)->GetMean(1))+"/"+QString::number(((TH1*)histo)->GetMean(2)));
	    textLabel1_6->setText(QString::number(((TH1*)histo)->GetRMS(1))+"/"+QString::number(((TH1*)histo)->GetRMS(2)));
	    int x,y,z;
	    ((TH1*)histo)->GetMinimumBin(x,y,z);
	    textLabel1_12->setText(QString::number(x)+"/"+QString::number(y));
	    ((TH1*)histo)->GetMaximumBin(x,y,z);
	    textLabel1_16->setText(QString::number(x)+"/"+QString::number(y));
	}
	checkBox1->setEnabled(((TH1*)histo)->GetBinError(1)-sqrt(((TH1*)histo)->GetBinContent(1))<0.01);
	line=*(TAttLine*)((TH1*)histo);
	fill=*(TAttFill*)((TH1*)histo);
	marker=*(TAttMarker*)((TH1*)histo);
	x=((TH1*)histo)->GetXaxis();
	y=((TH1*)histo)->GetYaxis();
	xA=*(TAttAxis*)(((TH1*)histo)->GetXaxis());
	yA=*(TAttAxis*)(((TH1*)histo)->GetYaxis());
	xAt=((TH1*)histo)->GetXaxis()->GetTitle();
	yAt=((TH1*)histo)->GetYaxis()->GetTitle();
	xN=((TH1*)histo)->GetNbinsX();
	yN=0;
	if(histo->IsA()==TH2F::Class()||histo->IsA()==TProfile2D::Class())
	{
	    z=((TH1*)histo)->GetZaxis();
	    zA=*(TAttAxis*)(((TH1*)histo)->GetZaxis());
	    zAt=((TH1*)histo)->GetZaxis()->GetTitle();
	    yN=((TH1*)histo)->GetNbinsY();
	    zN=0;
	}
    }
    if(histo->IsA()==TGraph::Class()||histo->IsA()==TGraphErrors::Class())
    {
	textLabel1_2->setText(QString::number(((TGraph*)histo)->GetN()));
	textLabel1_4->setText(QString::number(((TGraph*)histo)->GetMean(1))+"/"+QString::number(((TH1*)histo)->GetMean(2)));
	textLabel1_6->setText(QString::number(((TGraph*)histo)->GetRMS(1))+"/"+QString::number(((TH1*)histo)->GetRMS(2)));
	textLabel1_8->setText("-");
	textLabel1_10->setText("-");
	textLabel1_12->setText("-");
	textLabel1_14->setText("-");
	textLabel1_16->setText("-");
	line=*(TAttLine*)((TGraph*)histo);
	fill=*(TAttFill*)((TGraph*)histo);
	marker=*(TAttMarker*)((TGraph*)histo);
	checkBox1->setEnabled(false);
	x=((TGraph*)histo)->GetXaxis();
	y=((TGraph*)histo)->GetYaxis();
	xA=*(TAttAxis*)(((TGraph*)histo)->GetXaxis());
	yA=*(TAttAxis*)(((TGraph*)histo)->GetYaxis());
	xAt=((TGraph*)histo)->GetXaxis()->GetTitle();
	yAt=((TGraph*)histo)->GetYaxis()->GetTitle();
	xN=0;
	yN=0;
    }
    if(histo->IsA()==TGraph2D::Class()||histo->IsA()==TGraph2DErrors::Class())
    {
	textLabel1_2->setText(QString::number(((TGraph2D*)histo)->GetN()));
	textLabel1_4->setText("-");
	textLabel1_6->setText("-");
	textLabel1_8->setText("-");
	textLabel1_10->setText("-");
	textLabel1_12->setText("-");
	textLabel1_14->setText("-");
	textLabel1_16->setText("-");
	line=*(TAttLine*)((TGraph2D*)histo);
	fill=*(TAttFill*)((TGraph2D*)histo);
	marker=*(TAttMarker*)((TGraph2D*)histo);
	checkBox1->setEnabled(false);
	x=((TGraph2D*)histo)->GetXaxis();
	y=((TGraph2D*)histo)->GetYaxis();
	z=((TGraph2D*)histo)->GetZaxis();
	xA=*(TAttAxis*)(((TGraph2D*)histo)->GetXaxis());
	yA=*(TAttAxis*)(((TGraph2D*)histo)->GetYaxis());
	zA=*(TAttAxis*)(((TGraph2D*)histo)->GetZaxis());
	xAt=((TGraph2D*)histo)->GetXaxis()->GetTitle();
	yAt=((TGraph2D*)histo)->GetYaxis()->GetTitle();
	zAt=((TGraph2D*)histo)->GetZaxis()->GetTitle();
	xN=0;
	yN=0;
	zN=0;
     }
    attributeWidget1->setAtt(NULL,&line,NULL,NULL,0,0);
    attributeWidget2->setAtt(NULL,NULL,&marker,NULL,0,0);
    attributeWidget3->setAtt(&fill,NULL,NULL,NULL,0,0);
    if(x!=NULL)	axisToAxis(*x,xA);
    if(y!=NULL)	axisToAxis(*y,yA);
    if(z!=NULL)	axisToAxis(*z,zA);
    histogram=histo;
    comboBox6->clear();
#if QT_VERSION < 0x040000
    comboBox6->insertItem("X-Axis");
    comboBox6->insertItem("Y-Axis");
    if(histo->IsA()==TH2F::Class()||histo->IsA()==TProfile2D::Class()||
       histo->IsA()==TGraph2D::Class()||histo->IsA()==TGraph2DErrors::Class())
	comboBox6->insertItem("Z-Axis");
    actualAxis=comboBox6->currentItem();
    switch(comboBox6->currentItem())
#else
    comboBox6->addItem("X-Axis");
    comboBox6->addItem("Y-Axis");
    if(histo->IsA()==TH2F::Class()||histo->IsA()==TProfile2D::Class()||
       histo->IsA()==TGraph2D::Class()||histo->IsA()==TGraph2DErrors::Class())
	comboBox6->addItem("Z-Axis");
    actualAxis=comboBox6->currentIndex();
    switch(comboBox6->currentIndex())
#endif
    {
    case 0:     
	advAttributeWidget1->setAttribute(NULL,&xA);
	lineEdit3->setText(xAt.data());
	textLabel22->setText(QString::number(xN));
	break;
    case 1:
	advAttributeWidget1->setAttribute(NULL,&yA);
	lineEdit3->setText(yAt.data());
	textLabel22->setText(QString::number(yN));
	break;
    case 2: 
	advAttributeWidget1->setAttribute(NULL,&zA);
	lineEdit3->setText(zAt.data());
	textLabel22->setText(QString::number(zN));
	break;
    }
}
void HistoWidget::OnAxisSelect(int i)
{
    if(histogram==NULL)return;
    switch(actualAxis)
    {
    case 0:     
	axisToAxis(*advAttributeWidget1->getAxis(),xA);
#if QT_VERSION < 0x040000
	xAt=lineEdit3->text().ascii();
#else
	xAt=string(lineEdit3->text().toAscii());
#endif
	break;
    case 1:
	axisToAxis(*advAttributeWidget1->getAxis(),yA);
#if QT_VERSION < 0x040000
	yAt=lineEdit3->text().ascii();
#else
	yAt=string(lineEdit3->text().toAscii());
#endif
	break;
    case 2: 
	axisToAxis(*advAttributeWidget1->getAxis(),zA);
#if QT_VERSION < 0x040000
	zAt=lineEdit3->text().ascii();
#else
	zAt=string(lineEdit3->text().toAscii());
#endif
	break;
    }
    actualAxis=i;//comboBox6->currentItem();
    switch(i)//comboBox6->currentItem())
    {
    case 0:     
	advAttributeWidget1->setAttribute(NULL,&xA);
	lineEdit3->setText(xAt.data());
	textLabel22->setText(QString::number(xN));
	break;
    case 1:
	advAttributeWidget1->setAttribute(NULL,&yA);
	lineEdit3->setText(yAt.data());
	textLabel22->setText(QString::number(yN));
	break;
    case 2: 
	advAttributeWidget1->setAttribute(NULL,&zA);
	lineEdit3->setText(zAt.data());
	textLabel22->setText(QString::number(zN));
	break;
    }
}
void HistoWidget::OnResetClick()
{
    getHisto(histogram);
}
void HistoWidget::OnApplyClick()
{
    if(histogram==NULL)return;
    line=*attributeWidget1->getLine();
    fill=*attributeWidget3->getFill();
    marker=*attributeWidget2->getMarker();
#if QT_VERSION < 0x040000
    OnAxisSelect(comboBox6->currentItem());
#else
    OnAxisSelect(comboBox6->currentIndex());
#endif
    TAttLine *aL=NULL;
    TAttMarker *aM=NULL;
    TAttFill *aF=NULL;
    TNamed *aN=NULL;
    TAxis *x=NULL,*y=NULL,*z=NULL;
    if(histogram->IsA()==TH1F::Class()||histogram->IsA()==TH2F::Class()||
	 histogram->IsA()==TProfile::Class()||histogram->IsA()==TProfile2D::Class())
    {
	aL=(TAttLine*)((TH1*)histogram);
	aF=(TAttFill*)((TH1*)histogram);
	aM=(TAttMarker*)((TH1*)histogram);
	aN=(TNamed*)((TH1*)histogram);
	if(checkBox1->isChecked()&&checkBox1->isEnabled())
	{
	    ((TH1*)histogram)->Sumw2();
	}
	x=((TH1*)histogram)->GetXaxis();
	y=((TH1*)histogram)->GetYaxis();
	if(histogram->IsA()==TH2F::Class()||histogram->IsA()==TProfile2D::Class())
	    z=((TH1*)histogram)->GetZaxis();
    }
    if(histogram->IsA()==TGraph::Class()||histogram->IsA()==TGraphErrors::Class())
    {
	aL=(TAttLine*)((TGraph*)histogram);
	aF=(TAttFill*)((TGraph*)histogram);
	aM=(TAttMarker*)((TGraph*)histogram);
	aN=(TNamed*)((TGraph*)histogram);
	x=((TGraph*)histogram)->GetXaxis();
	y=((TGraph*)histogram)->GetYaxis();
    }
    if(histogram->IsA()==TGraph2D::Class()||histogram->IsA()==TGraph2DErrors::Class())
    {
	aL=(TAttLine*)((TGraph2D*)histogram);
 	aF=(TAttFill*)((TGraph2D*)histogram);
	aM=(TAttMarker*)((TGraph2D*)histogram);
	aN=(TNamed*)((TGraph2D*)histogram);
	x=((TGraph2D*)histogram)->GetXaxis();
	y=((TGraph2D*)histogram)->GetYaxis();
	z=((TGraph2D*)histogram)->GetZaxis();
    }
    aL->SetLineColor(line.GetLineColor());
    aL->SetLineStyle(line.GetLineStyle());
    aL->SetLineWidth(line.GetLineWidth());
    aM->SetMarkerColor(marker.GetMarkerColor());
    aM->SetMarkerStyle(marker.GetMarkerStyle());
    aM->SetMarkerSize(marker.GetMarkerSize());
    aF->SetFillColor(fill.GetFillColor());
    aF->SetFillStyle(fill.GetFillStyle());
#if QT_VERSION < 0x040000
    string nnm=lineEdit1->text().ascii();
#else
    string nnm=string(lineEdit1->text().toAscii());
#endif
    aN->SetName(nnm.data());
    aN->SetTitle(nnm.data());
    if(x!=NULL)
    {	
	x->SetTitle(xAt.data());
	axisToAxis(xA,*x);
    }
    if(y!=NULL)
    {
	y->SetTitle(yAt.data());
	axisToAxis(yA,*y);
    }
    if(z!=NULL)
    {
	z->SetTitle(zAt.data());
	axisToAxis(zA,*z);
    }
}
void axisToAxis(const TAttAxis &source, TAttAxis &target)
{
    target.SetAxisColor(source.GetAxisColor());
    target.SetTitleColor(source.GetTitleColor());
    target.SetTitleFont(source.GetTitleFont());
    target.SetTitleSize(source.GetTitleSize());
    target.SetLabelColor(source.GetLabelColor());
    target.SetLabelFont(source.GetLabelFont());
    target.SetLabelSize(source.GetLabelSize());
    target.SetNdivisions(source.GetNdivisions());
    target.SetTickLength(source.GetTickLength());
}
void axisToAxis(const TAxis &source, TAttAxis &target)
{
    target.SetAxisColor(source.GetAxisColor());
    target.SetTitleColor(source.GetTitleColor());
    target.SetTitleFont(source.GetTitleFont());
    target.SetTitleSize(source.GetTitleSize());
    target.SetLabelColor(source.GetLabelColor());
    target.SetLabelFont(source.GetLabelFont());
    target.SetLabelSize(source.GetLabelSize());
    target.SetNdivisions(source.GetNdivisions());
    target.SetTickLength(source.GetTickLength());
}
void axisToAxis(const TAttAxis &source, TAxis &target)
{
    target.SetAxisColor(source.GetAxisColor());
    target.SetTitleColor(source.GetTitleColor());
    target.SetTitleFont(source.GetTitleFont());
    target.SetTitleSize(source.GetTitleSize());
    target.SetLabelColor(source.GetLabelColor());
    target.SetLabelFont(source.GetLabelFont());
    target.SetLabelSize(source.GetLabelSize());
    target.SetNdivisions(source.GetNdivisions());
    target.SetTickLength(source.GetTickLength());
}
