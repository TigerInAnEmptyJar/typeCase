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
2*90+100+4*10=320
170+170+30=370
*****************************************************************************/
#include <aattributewidget.h>
#include <qvariant.h>
#include <q3Dinput.h>
#include <attributewidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "utilities.h"
#include "colorselectdialog.h"
void AAttributeWidget::init()
{
  mode=-1;
}
void AAttributeWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void AAttributeWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void AAttributeWidget::resize(int w, int h)
{
    QWidget::resize(w,h);
    int abt=0;
    int minHeight;
    int bH=25,lH=20;
    switch(mode)
    {
    case 0://pad
	{
	    minHeight=lH*2+bH*3+bH*4;
	    if(h>minHeight)abt=(h-minHeight)/(5+3+6);
	    int gH=bH*4+abt*3;
	    groupBox1->setGeometry(10,abt*2,(w-40)/3,gH);//margin
	    groupBox2->setGeometry(20+(w-40)/3,abt*2,(w-40)/3,gH);//stat
	    groupBox3->setGeometry(30+(w-40)/3*2,abt*2,(w-40)/3,gH);//file
	    groupBox4->setGeometry(10,abt*3+gH,(w-40)/3,bH*2+lH+abt*5+2);//frame
	    attributeWidget1->setGeometry(1,lH,(w-40)/3-2,bH*2+abt*5);
	    groupBox5->setGeometry(10+(w-40)/3,abt*3+gH,(w-40)/3,bH*2+lH+abt*5);//fill
	    attributeWidget2->setGeometry(1,lH,(w-40)/3-2,bH*2+abt*5);
	    groupBox6->setGeometry(10+(w-40)/3*2,abt*3+gH,(w-40)/3,bH*3+lH+abt*6);//line
	    attributeWidget3->setGeometry(1,lH,(w-40)/3-2,bH*3+abt*6);
	    break;
	}
    case 1://axis
	{
	    int gH=lH+bH*7;
	    minHeight=gH+(bH*3);
	    if(minHeight<h)abt=(h-minHeight)/ (7+10);
	    gH=gH+abt*10;
	    textLabel1->setGeometry(10,abt*2,40,lH);//color
	    colorButton->setGeometry(55,abt*2,w-65,bH);
	    textLabel2->setGeometry(10,abt*3+bH,70,lH);//Ndivisions
	    q3DInput1->setGeometry(85,abt*3+bH,w-95,bH);
	    textLabel3->setGeometry(10,abt*4+bH*2,70,lH);//Ticks
	    lineEdit1->setGeometry(90,abt*4+bH*2,w-100,bH);
	    groupBox4->setGeometry(10,abt*5+bH*3,(w-30)/2,bH*7+abt*10+lH+2);//label
	    attributeWidget1->setGeometry(1,lH,(w-30)/2-2,bH*7+abt*10);
	    groupBox5->setGeometry(20+(w-30)/2,abt*5+bH*3,(w-30)/2,bH*7+abt*10+lH+2);//title
	    attributeWidget2->setGeometry(1,lH,(w-30)/2-2,bH*7+abt*10);
	    break;
	}
    }
}
void AAttributeWidget::setMode(int m)
{
    if(mode==m)return;
    mode=m;
    textLabel1->setGeometry(-20,-20,10,10);//Color
    colorButton->setGeometry(-20,-20,10,10);
    textLabel2->setGeometry(-20,-20,10,10);//Ndiv
    q3DInput1->setGeometry(-20,-20,10,10);
    textLabel3->setGeometry(-20,-20,10,10);//tick
    lineEdit1->setGeometry(-20,-20,10,10);
    groupBox1->setGeometry(-20,-20,10,10);//margin
    groupBox2->setGeometry(-20,-20,10,10);//stat
    groupBox3->setGeometry(-20,-20,10,10);//file
    groupBox4->setGeometry(-20,-20,10,10);
    groupBox5->setGeometry(-20,-20,10,10);
    groupBox6->setGeometry(-20,-20,10,10);
    switch(mode)
    {
    case 0://pad
	{
	    groupBox4->setTitle("Border Frame");
	    groupBox5->setTitle("Border Fill");
	    groupBox6->setTitle("Border Line");
	    attributeWidget1->setMode(4);
	    attributeWidget2->setMode(0);
	    attributeWidget3->setMode(1);
	    setMinimumHeight(4*25+3*25);
	    setMinimumWidth(2*90+100+4*10);
	    break;
	}
    case 1://axis
	{
	    groupBox4->setTitle("Label");
	    groupBox5->setTitle("Title");
	    attributeWidget1->setMode(3);
	    attributeWidget2->setMode(3);
	    setMinimumHeight(3*25+7*25+20+2);
	    setMinimumWidth(2*170+3*10+4);
	    break;
	}
    }
    resize(width(),height());
}
void AAttributeWidget::setAttribute(TAttPad *pad, TAttAxis *axis)
{
    if(pad!=NULL)
    {
	setMode(0);
	lineEdit1_1->setText(QString::number(pad->GetTopMargin()));
	lineEdit1_2->setText(QString::number(pad->GetLeftMargin()));
	lineEdit1_3->setText(QString::number(pad->GetRightMargin()));
	lineEdit1_4->setText(QString::number(pad->GetBottomMargin()));
	attributeWidget1->setAtt(NULL, NULL, NULL, NULL, pad->GetFrameBorderSize(),  pad->GetFrameBorderMode());
	TAttFill tmp1(pad->GetFrameFillColor(),pad->GetFrameFillStyle());
	attributeWidget2->setAtt(&tmp1,NULL,NULL,NULL,0,0);
	TAttLine tmp2(pad->GetFrameLineColor(),pad->GetFrameLineStyle(),pad->GetFrameLineWidth());
	attributeWidget3->setAtt(NULL,&tmp2,NULL,NULL,0,0);
	lineEdit1_1->setText(QString::number(pad->GetXfile()));
	lineEdit1_2->setText(QString::number(pad->GetYfile()));
	lineEdit2_1->setText(QString::number(pad->GetXstat()));
	lineEdit2_2->setText(QString::number(pad->GetYstat()));
	setComboBoxCurrentItem(comboBox1_1,(int)(pad->GetAstat()/10));
	setComboBoxCurrentItem(comboBox1_2,(int)pad->GetAstat()%10);
	setComboBoxCurrentItem(comboBox2_1,(int)(pad->GetAstat()/10));
	setComboBoxCurrentItem(comboBox2_2,(int)pad->GetAstat()%10);
	return;
    }
    else if(axis!=NULL)
    {
	setMode(1);
	setWidgetColor(colorButton,ColorSelectDialog::Root2Qt(axis->GetAxisColor()));
	rColor=axis->GetAxisColor();
	colorButton->setText(QString::number(rColor));
	lineEdit1->setText(QString::number(axis->GetTickLength()));
	int tmp=axis->GetNdivisions(),n2,n3;
	n3=tmp%100;
	tmp=(tmp-n3)/100;
	n2=(tmp%10000);
	tmp=(tmp-n2)/100;
	q3DInput1->setValues(tmp,n2,n3);
	TAttText tmp1(2,axis->GetLabelOffset(),axis->GetLabelColor(), axis->GetLabelFont(),axis->GetLabelSize()*100);
//	tmp1.SetTextAlign();
	attributeWidget1->setAtt(NULL,NULL,NULL,&tmp1,0,0);
	TAttText tmp2(2,axis->GetTitleOffset(),axis->GetTitleColor(), axis->GetTitleFont(),axis->GetTitleSize()*100);
	attributeWidget2->setAtt(NULL,NULL,NULL,&tmp2,0,0);
	return;
    }
}
TAttPad *AAttributeWidget::getPad()
{
    if(mode!=0)return NULL;
    fpad.SetTopMargin(lineEdit1_1->text().toFloat());
    fpad.SetLeftMargin(lineEdit1_2->text().toFloat());
    fpad.SetRightMargin(lineEdit1_3->text().toFloat());
    fpad.SetBottomMargin(lineEdit1_4->text().toFloat());
    TAttFill *tmp1=attributeWidget3->getFill();
    TAttLine *tmp2=attributeWidget2->getLine();
    fpad.SetFrameFillColor(tmp1->GetFillColor());
    fpad.SetFrameFillStyle(tmp1->GetFillStyle());
    fpad.SetFrameLineColor(tmp2->GetLineColor());
    fpad.SetFrameLineStyle(tmp2->GetLineStyle());
    fpad.SetFrameLineWidth(tmp2->GetLineWidth());
    fpad.SetFrameBorderSize(attributeWidget1->getFrameSize());
    fpad.SetFrameBorderMode(attributeWidget1->getFrameStyle());
    return &fpad;
}
TAttAxis *AAttributeWidget::getAxis()
{
    if(mode!=1)return NULL;
    faxis.SetTickLength(lineEdit1->text().toFloat());
    faxis.SetNdivisions(((int)q3DInput1->getX())*10000
			+((int)q3DInput1->getY())*100
			+((int)q3DInput1->getZ()));
    TAttText *tmp1=attributeWidget1->getText();
    faxis.SetLabelOffset(tmp1->GetTextAngle());
    faxis.SetLabelColor(tmp1->GetTextColor()), 
    faxis.SetLabelFont(tmp1->GetTextFont()),
    faxis.SetLabelSize(tmp1->GetTextSize()/100.);
    TAttText *tmp2=attributeWidget2->getText();
    faxis.SetTitleOffset(tmp2->GetTextAngle());
    faxis.SetTitleColor(tmp2->GetTextColor());
    faxis.SetTitleFont(tmp2->GetTextFont());
    faxis.SetTitleSize(tmp2->GetTextSize()/100.);
    faxis.SetAxisColor(rColor);
    return &faxis;
}
void AAttributeWidget::OnColorButtonClick()
{
    ColorSelectDialog fens(this);
    fens.exec();
    if(fens.result()==QDialog::Accepted)
    {
	rColor=fens.getColor();
	setWidgetColor(colorButton,fens.getRGBColor());
	colorButton->setText(QString::number(rColor));
    }
}
