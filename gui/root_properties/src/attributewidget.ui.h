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
height=bH*2+abt*5
height=bH*3+abt*6
height=bH*3+abt*6
height=bH*7+abt*10
height=bH*2+abt*5
*****************************************************************************/
#include "attributewidget.h"
#include <qvariant.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "utilities.h"
#include "colorselectdialog.h"
void AttributeWidget::init()
{
    mode=-1;
    setMode(0);
    resize(width(),height());
}
void AttributeWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void AttributeWidget::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void AttributeWidget::resize(int w, int h)
{
    QWidget::resize(w,h);
    int abt;
    int bH=25;
    int lH=20;
    int colormin=bH;
    switch(mode)
    {
    case 0:
	{
	    int minHeight=bH+colormin;
	    if(h<minHeight)return;
	    abt=(h-minHeight)/5;
	    textLabel1->setGeometry(10,abt*2,40,lH);//color
	    colorButton->setGeometry(60,abt*2,w-70,colormin);
	    textLabel3->setGeometry(10,abt*3+colormin, 40,lH);//style
	    spinBox2->setGeometry(60,abt*3+colormin,w-70,bH);
	    break;
	}
    case 1:
	{
	    int minHeight=bH*2+colormin;
	    if(h<minHeight)return;
	    abt=(h-minHeight)/6;
	    textLabel1->setGeometry(10,abt*2,40,lH);//color
	    colorButton->setGeometry(60,abt*2,w-70,colormin);
	    textLabel2->setGeometry(10,abt*3+colormin, 40,lH);//width
	    spinBox1->setGeometry(60,abt*3+colormin, w-70,bH);
	    textLabel3->setGeometry(10,abt*4+colormin+bH, 40,lH);//style
	    comboBox1->setGeometry(60,abt*4+colormin+bH, w-70,bH);
	    break;
	}
    case 2:
	{
	    int minHeight=bH*2+colormin;
	    if(h<minHeight)return;
	    abt=(h-minHeight)/6;
	    textLabel1->setGeometry(10,abt*2,40,lH);//color
	    colorButton->setGeometry(60,abt*2,w-70,colormin);
	    textLabel2->setGeometry(10,abt*3+colormin, 40,lH);//width
	    spinBox1->setGeometry(60,abt*3+colormin, w-70,bH);
	    textLabel3->setGeometry(10,abt*4+colormin+bH, 40,lH);//style
	    comboBox2->setGeometry(60,abt*4+colormin+bH, w-70,bH);
	    break;
	}
    case 3:
	{
	    int minHeight=bH*6+colormin;
	    if(h<minHeight)return;
	    abt=(h-minHeight)/10;
	    textLabel1->setGeometry(10,abt*2,40,lH);//color
	    colorButton->setGeometry(60,abt*2,w-70,colormin);
	    textLabel4->setGeometry(10,abt*3+colormin, 60,lH);//allignment
	    comboBox3->setGeometry(80,abt*3+colormin,w-90,bH);
	    comboBox4->setGeometry(80,abt*4+colormin+bH,w-90,bH);
	    textLabel5->setGeometry(10,abt*5+colormin+bH*2, 40,lH);//angle
	    spinBox3->setGeometry(60,abt*5+colormin+bH*2,w-70,bH);
	    textLabel6->setGeometry(10,abt*6+colormin+bH*3, 105,lH);//textHeight
	    spinBox4->setGeometry(125,abt*6+colormin+bH*3,w-135,bH);
	    textLabel7->setGeometry(10,abt*7+colormin+bH*4, 40,lH);//font
	    comboBox5->setGeometry(60,abt*7+colormin+bH*4,w-70,bH);
	    textLabel8->setGeometry(10,abt*8+colormin+bH*5, 60,lH);//precision
	    spinBox5->setGeometry(80,abt*8+colormin+bH*5,w-90,bH);
	    break;
	}
    case 4:
	{
	    int minHeight=bH*2;
	    if(h<minHeight)return;
	    abt=(h-minHeight)/5;
	    textLabel2->setGeometry(10,abt*2, 40,lH);//width
	    spinBox1->setGeometry(60,abt*2, w-70,bH);
	    textLabel3->setGeometry(10,abt*3+bH, 40,lH);//style
	    comboBox6->setGeometry(60,abt*3+bH, w-70,bH);
	    break;
	}
    }
}
void AttributeWidget::setMode(int modeIn)
{
    if(mode==modeIn)return;
    mode=modeIn;
    textLabel1->setGeometry(-20,-20,10,10);//color
    colorButton->setGeometry(-20,-20,10,10);
    textLabel2->setGeometry(-20,-20,10,10);//width
    spinBox1->setGeometry(-20,-20,10,10);
    textLabel3->setGeometry(-20,-20,10,10);//style
    comboBox1->setGeometry(-20,-20,10,10);
    spinBox2->setGeometry(-20,-20,10,10);
    comboBox2->setGeometry(-20,-20,10,10);
    comboBox6->setGeometry(-20,-20,10,10);
    textLabel4->setGeometry(-20,-20,10,10);//allignment
    comboBox3->setGeometry(-20,-20,10,10);
    comboBox4->setGeometry(-20,-20,10,10);
    textLabel5->setGeometry(-20,-20,10,10);//angle
    spinBox3->setGeometry(-20,-20,10,10);
    textLabel6->setGeometry(-20,-20,10,10);//text height
    spinBox4->setGeometry(-20,-20,10,10);
    textLabel6->setGeometry(-20,-20,10,10);//font
    comboBox5->setGeometry(-20,-20,10,10);
    textLabel8->setGeometry(-20,-20,10,10);//text precision
    spinBox5->setGeometry(-20,-20,10,10);
    int minHeight=0, minWidth=0,bH=25,colormin=25;
    switch(mode)
    {
    case 0: minHeight=bH+colormin;minWidth=90;break;
    case 1: minHeight=bH*2+colormin;minWidth=100;break;
    case 2: minHeight=bH*2+colormin;minWidth=100;break;
    case 3: minHeight=bH*6+colormin;minWidth=170;break;
    case 4: minHeight=bH*2;minWidth=90;break;
    }
    setMinimumWidth (minWidth ) ;
    setMinimumHeight ( minHeight ) ;
    resize(width(),height());
}
void AttributeWidget::setAtt(TAttFill *fill, TAttLine *line, TAttMarker *marker, TAttText *textIn, int framewidth, int framestyle)
{
    if(fill!=NULL)
    {
	setMode(0);
	spinBox2->setValue(fill->GetFillStyle());
	rColor=fill->GetFillColor();
	setWidgetColor(colorButton,ColorSelectDialog::Root2Qt(rColor));
	colorButton->setText(QString::number(rColor));
	return;
    }
    else if(line!=NULL)
    {
	setMode(1);
	setComboBoxCurrentItem(comboBox1,line->GetLineStyle());
	spinBox1->setValue(line->GetLineWidth());
	rColor=line->GetLineColor();
	setWidgetColor(colorButton,ColorSelectDialog::Root2Qt(rColor));
	colorButton->setText(QString::number(rColor));
	return;
    }
    else if(marker!=NULL)
    {
	setMode(2);
	spinBox1->setValue((int)marker->GetMarkerSize());
	int tmp=marker->GetMarkerStyle();
	setComboBoxCurrentItem(comboBox2,(tmp<=9?tmp:(tmp>=20?tmp-10:9)));
	rColor=marker->GetMarkerColor();
	setWidgetColor(colorButton,ColorSelectDialog::Root2Qt(rColor));
	colorButton->setText(QString::number(rColor));
	return;
    }
    else if(textIn!=NULL)
    {
	setMode(3);
	rColor=textIn->GetTextColor();
	setWidgetColor(colorButton,ColorSelectDialog::Root2Qt(rColor));
	int tmp=textIn->GetTextAlign();
	setComboBoxCurrentItem(comboBox3,(tmp/10)-1);
	setComboBoxCurrentItem(comboBox4,(tmp%10)-1);
	spinBox3->setValue((int)textIn->GetTextAngle());
	spinBox4->setValue((int)textIn->GetTextSize());
	tmp=textIn->GetTextFont();
	setComboBoxCurrentItem(comboBox5,(tmp/10)-1);
	spinBox5->setValue(tmp%10);
	colorButton->setText(QString::number(rColor));
	return;
    }
    setMode(4);
    setComboBoxCurrentItem(comboBox6,framestyle+1);
    spinBox1->setValue(framewidth);
}
TAttText *AttributeWidget::getText()
{
    if(mode!=3)return NULL;
    ftext.SetTextAngle(spinBox3->value());
    ftext.SetTextAlign((comboBoxCurrentItem(comboBox3)+1)*10+comboBoxCurrentItem(comboBox4)+1);
    ftext.SetTextSize(spinBox4->value());
    ftext.SetTextFont((comboBoxCurrentItem(comboBox5)+1)*10+spinBox5->value());
    ftext.SetTextColor(rColor);
    return &ftext;
}
TAttMarker *AttributeWidget::getMarker()
{
    if(mode!=2)return NULL;
    fmarker.SetMarkerSize(spinBox1->value());
    int tmp=comboBoxCurrentItem(comboBox2);
    fmarker.SetMarkerStyle(tmp<=9?tmp:tmp+10);
    fmarker.SetMarkerColor(rColor);
    return &fmarker;
}
TAttLine *AttributeWidget::getLine()
{
    if(mode!=1)return NULL;
    fline.SetLineStyle(comboBoxCurrentItem(comboBox1));
    fline.SetLineWidth(spinBox1->value());
    fline.SetLineColor(rColor);
    return &fline;
}
TAttFill *AttributeWidget::getFill()
{
    if(mode!=0)return NULL;
    ffill.SetFillStyle(spinBox2->value());
    ffill.SetFillColor(rColor);
    return &ffill;
}
int AttributeWidget::getFrameStyle()
{
    if(mode==4)return comboBoxCurrentItem(comboBox6)-1;
    return 0;
}
int AttributeWidget::getFrameSize()
{
   if(mode==4)return spinBox1->value();
    return 0;
}
void AttributeWidget::OnColorButtonClick()
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
