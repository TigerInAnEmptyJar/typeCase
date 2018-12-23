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
#include "colorselectdialog.h"

#include <qvariant.h>
#include <TROOT.h>
#include <TColor.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <iostream>
#include "utilities.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
void ColorSelectDialog::init()
{
    setEnabled(true);
    selectedColor=0;
    buttons=new QPushButton*[50];
    for(int i=0;i<50;i++)buttons[i]=new QPushButton(this);
    for(int i=0;i<50;i++)
    {
	buttons[i]->setText(QString::number(i));
	setWidgetColor(buttons[i],ColorSelectDialog::Root2Qt(i));
	connect(buttons[i],SIGNAL(pressed()),this,SLOT(OnButtonDown()));
	connect(buttons[i],SIGNAL(released()),this,SLOT(accept()));
    }
    resize(200,100);
}
void ColorSelectDialog::destroy()
{
    for(int i=0;i<50;i++)
    {
	disconnect(buttons[i]);
	delete buttons[i];
    }
    delete []buttons;
}
void ColorSelectDialog::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void ColorSelectDialog::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void ColorSelectDialog::resize(int w, int h)
{
    if(!isEnabled())return;
    QWidget::resize(w,h);
    int bw=w/10,bh=h/5;
    for(int i=0;i<10;i++)
    {
	for(int j=0;j<5;j++)
	{
	    buttons[i+j*10]->setGeometry(i*bw,j*bh,bw,bh);
	}
    }
}
void ColorSelectDialog::accept()
{
    QDialog::accept();
}
int ColorSelectDialog::getColor()
{
    return selectedColor;
}
void ColorSelectDialog::OnButtonDown()
{
    for(int i=0;i<50;i++)
	if(buttons[i]->isDown())
	    selectedColor=i;
}
QColor ColorSelectDialog::getRGBColor()
{
    TColor *col=gROOT->GetColor(selectedColor);
    return QColor((int)(col->GetRed()*255),
		  (int)(col->GetGreen()*255),
		  (int)(col->GetBlue()*255));
}


QColor ColorSelectDialog::Root2Qt( int colo )
{
    if(colo<0)return QColor();
    TColor *col=gROOT->GetColor(colo);
    if(col==NULL)return QColor(0,0,0);
    return QColor((int)(col->GetRed()*255),
		  (int)(col->GetGreen()*255),
		  (int)(col->GetBlue()*255));
}


int ColorSelectDialog::Qt2Root( const QColor & c )
{
    int r,g,b;
    for(int i=0;i<50;i++)
    {
	TColor *col=gROOT->GetColor(i);
	r=(int)(col->GetRed()*255);
	g=(int)(col->GetGreen()*255);
	b=(int)(col->GetBlue()*255);
	if(r==c.red()&&g==c.green()&&b==c.blue())
	    return i;
    }
    return -1;
}


QColor ColorSelectDialog::Root2Qt( const TColor &c )
{
    return QColor((int)(c.GetRed()*255),
		  (int)(c.GetGreen()*255),
		  (int)(c.GetBlue()*255));
}
