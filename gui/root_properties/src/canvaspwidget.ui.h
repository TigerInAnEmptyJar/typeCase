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
#include "canvaspwidget.h"
#include "utilities.h"
#include <qvariant.h>
#include <iostream>
#include <attributewidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "dividedialog.h"
void CanvasPWidget::init()
{
//    cout<<"CanvasPWidget::init()"<<endl;
    canvas=NULL;
    attributeWidget1->setMode(0);
    attributeWidget2->setMode(1);
    setMinimumWidth(230);
    setMinimumHeight(20+25*3+(25*4)*2);
    resize(360,400);
}
void CanvasPWidget::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void CanvasPWidget::resize(const QSize & s)
{
    resize(s.width(),s.height());
}
void CanvasPWidget::resize(int w, int h)
{
    QWidget::resize(w,h);
    int abt=0;
    int bH=25, lH=20, aH=bH*4;
    int minH=lH+bH*3+aH*2;
    if(h>minH)
    {
	abt=(h-minH)/15;
	aH+=abt*3;
    }
    textLabel1->setGeometry(10,abt*2,80,lH);//name
    textLabel2->setGeometry(100,abt*2,w-110,lH);
    textLabel3->setGeometry(10,abt*3+lH,80,lH);//pad
    comboBox1->setGeometry(90,abt*3+lH,w-250,bH);
    pushButton5->setGeometry(w-150,abt*3+lH,60,bH);
    pushButton6->setGeometry(w-80,abt*3+lH,60,bH);
    groupBox2->setGeometry(10,abt*4+lH+bH,(w-30)/2,aH);//fill
    attributeWidget1->setGeometry(1,15,(w-30)/2-2,aH-16);
    groupBox3->setGeometry(10,abt*5+lH+bH+aH,(w-30)/2,aH);//line
    attributeWidget2->setGeometry(1,15,(w-30)/2-2,aH-16);
    buttonGroup1->setGeometry(QRect((w-30)/2+20,abt*4+lH+bH,(w-30)/2,aH));//Scale
    int za=(aH-20-abt*2-bH*3)/2;
    checkBox1->setGeometry(10,15,100,bH);
    checkBox2->setGeometry(10,15+bH+abt+za,100,bH);
    checkBox3->setGeometry(10,15+(bH+abt+za)*2,100,bH);
    groupBox1->setGeometry((w-30)/2+20,abt*5+lH+bH+aH,(w-30)/2,aH);//range
    textLabel6->setGeometry(10,15,10,lH);
    textLabel7->setGeometry(10,aH-abt-lH-bH-2,10,lH);
    int rs=((w-30)/2-30)/2;
    lineEdit3->setGeometry(10,15+abt+lH,rs,bH);
    lineEdit4->setGeometry(20+rs,15+abt+lH,rs,bH);
    lineEdit5->setGeometry(10,aH-bH-2,rs,bH);
    lineEdit6->setGeometry(20+rs,aH-bH-2,rs,bH);
    rs=(w-(80*2+50))/2;
    textLabel4->setGeometry(10,abt*6+lH+bH+aH*2,80,lH);//theta
    lineEdit1->setGeometry(100,abt*6+lH+bH+aH*2,rs,bH);
    textLabel5->setGeometry(rs+110,abt*6+lH+bH+aH*2,80,lH);//phi
    lineEdit2->setGeometry(rs+200,abt*6+lH+bH+aH*2,rs,bH);
    pushButton1->setGeometry(10,h-abt*2-bH,60,bH);//Apply
    pushButton2->setGeometry(80,h-abt*2-bH,60,bH);//Cancel
    pushButton4->setGeometry(150,h-abt*2-bH,60,bH);//Update
    pushButton3->setGeometry(w-70,h-abt*2-bH,60,bH);//Help
}
void CanvasPWidget::getCanvas(TCanvas* canv)
{
//    cout<<"CanvasPWidget::getCanvas()"<<endl;
    canvas=canv;
//    cout<<"CanvasPWidget::getCanvas(canv="<<canv<<") "<<canvas->GetName()<<endl;
    textLabel2->setText(canvas->GetName());
//    spinBox1->setValue(canvas->GetFillStyle());
//    comboBox4->setCurrentItem(canvas->GetLineStyle());
//    int c=canvas->GetFillColor();
//    if(c<9)comboBox3->setCurrentItem(c);
//    else comboBox3->setCurrentItem(9);
//    c=canvas->GetLineColor();
//    if(c<9)comboBox5->setCurrentItem(c);
//    else comboBox5->setCurrentItem(8);
    attributeWidget1->setAtt((TAttFill*)canv,NULL,NULL,NULL,0,0);
    attributeWidget2->setAtt(NULL,(TAttLine*)canv,NULL,NULL,0,0);
    checkBox1->setChecked(canvas->GetLogx());
    checkBox2->setChecked(canvas->GetLogy());
    checkBox3->setChecked(canvas->GetLogz());
    lineEdit1->setText(QString::number(canvas->GetTheta()));
    lineEdit2->setText(QString::number(canvas->GetPhi()));
    Double_t x1,x2,y1,y2;
    canvas->GetRange(x1,y1,x2,y2);
    lineEdit3->setText(QString::number(x1));
    lineEdit4->setText(QString::number(y1));
    lineEdit5->setText(QString::number(x2));
    lineEdit6->setText(QString::number(y2));
    while(!subPads.empty())subPads.pop_back();
    for(int i=0;i<canvas->GetListOfPrimitives()->GetEntries();i++)
	if(canvas->GetListOfPrimitives()->At(i)->IsA()==TPad::Class())
	    subPads.push_back((TPad*)canvas->GetListOfPrimitives()->At(i));
    comboBox1->clear();
#if QT_VERSION < 0x040000
    comboBox1->insertItem("canvas");
    for(unsigned int i=0;i<subPads.size();i++)
	comboBox1->insertItem(QString("pad ")+QString::number(i+1));
#else
    comboBox1->addItem("canvas");
    for(unsigned int i=0;i<subPads.size();i++)
	comboBox1->addItem(QString("pad ")+QString::number(i+1));
#endif
}
void CanvasPWidget::OnUpdateClick()
{
//    cout<<"CanvasPWidget::OnUpdateClick()"<<endl;
    if(canvas==NULL)return;
    textLabel2->setText(canvas->GetName());
    attributeWidget1->setAtt((TAttFill*)canvas,NULL,NULL,NULL,0,0);
    attributeWidget2->setAtt(NULL,(TAttLine*)canvas,NULL,NULL,0,0);
    checkBox1->setChecked(canvas->GetLogx());
    checkBox2->setChecked(canvas->GetLogy());
    checkBox3->setChecked(canvas->GetLogz());
    lineEdit1->setText(QString::number(canvas->GetTheta()));
    lineEdit2->setText(QString::number(canvas->GetPhi()));
    Double_t x1,x2,y1,y2;
    canvas->GetRange(x1,y1,x2,y2);
    lineEdit3->setText(QString::number(x1));
    lineEdit4->setText(QString::number(y1));
    lineEdit5->setText(QString::number(x2));
    lineEdit6->setText(QString::number(y2));
}
void CanvasPWidget::OnApplyClick()
{
//    cout<<"CanvasPWidget::OnApplyClick()"<<endl;
    if(canvas==NULL)return;
    TPad *selectedPad=canvas;
    TAttFill *fill=attributeWidget1->getFill();
    TAttLine *line=attributeWidget2->getLine();
    if(comboBoxCurrentItem(comboBox1)>0)
	selectedPad=subPads[comboBoxCurrentItem(comboBox1)-1];
    selectedPad->SetFillStyle(fill->GetFillStyle());
    selectedPad->SetLineStyle(line->GetLineStyle());
    selectedPad->SetFillColor(fill->GetFillColor());
    selectedPad->SetLineColor(line->GetLineColor());
    selectedPad->SetLineWidth(line->GetLineWidth());
    selectedPad->SetLogx(checkBox1->isChecked());
    selectedPad->SetLogy(checkBox2->isChecked());
    selectedPad->SetLogz(checkBox3->isChecked());
    selectedPad->SetTheta(lineEdit1->text().toDouble());
    selectedPad->SetPhi(lineEdit2->text().toDouble());
    Double_t x1,x2,y1,y2;
    x1=lineEdit3->text().toDouble();
    y1=lineEdit4->text().toDouble();
    x2=lineEdit5->text().toDouble();
    y2=lineEdit6->text().toDouble();
    selectedPad->Range(x1,y1,x2,y2);
}
void CanvasPWidget::OnHelpClick()
{
//    cout<<"CanvasPWidget::OnHelpClick()"<<endl;
//    emit helpAsked(helpPath.path()+"/canvasProperties.html");
}
void CanvasPWidget::OnPadSelect()
{
//    cout<<"CanvasPWidget::OnPadSelect()"<<endl;
    if(comboBoxCurrentItem(comboBox1)==0)
    {
	getCanvas(canvas);
	return;
    }
    TPad *pd=subPads[comboBoxCurrentItem(comboBox1)-1];
    attributeWidget1->setAtt((TAttFill*)pd,NULL,NULL,NULL,0,0);
    attributeWidget2->setAtt(NULL,(TAttLine*)pd,NULL,NULL,0,0);
//    spinBox1->setValue(pd->GetFillStyle());
//    comboBox4->setCurrentItem(pd->GetLineStyle());
//    int c=pd->GetFillColor();
//    if(c<9)comboBox3->setCurrentItem(c);
//    else comboBox3->setCurrentItem(9);
//    c=pd->GetLineColor();
//    if(c<9)comboBox5->setCurrentItem(c);
//    else comboBox5->setCurrentItem(8);
    checkBox1->setChecked(pd->GetLogx());
    checkBox2->setChecked(pd->GetLogy());
    checkBox3->setChecked(pd->GetLogz());
    lineEdit1->setText(QString::number(pd->GetTheta()));
    lineEdit2->setText(QString::number(pd->GetPhi()));
    Double_t x1,x2,y1,y2;
    pd->GetRange(x1,y1,x2,y2);
    lineEdit3->setText(QString::number(x1));
    lineEdit4->setText(QString::number(y1));
    lineEdit5->setText(QString::number(x2));
    lineEdit6->setText(QString::number(y2));
}

void CanvasPWidget::OnClearClick()
{
//    cout<<"CanvasPWidget::OnClearClick()"<<endl;
    if(canvas!=NULL)
	canvas->Clear();
    getCanvas(canvas);
}
void CanvasPWidget::OnDivideClick()
{
//    cout<<"CanvasPWidget::OnDivideClick()"<<endl;
    if(canvas==NULL)return;
    int nx=1,ny=1;
    if(subPads.size()>0)
    {
	Double_t x1,x2,y1,y2;
	subPads[0]->GetPadPar(x1,y1,x2,y2);
	nx=(int)(1./x2);
	ny=(int)(1./ (1-y1));
    }
    DivideDialog fenst(this);
    fenst.setNumbers(nx,ny);
    fenst.exec();
    if(fenst.result()==QDialog::Accepted)
    {	
	if(fenst.getX()==nx&&fenst.getY()==ny)
	    return;
	if(fenst.getX()==1&&fenst.getY()==1)
	{
	    OnClearClick();
	    return;
	}
	canvas->Divide(fenst.getX(),fenst.getY());
	getCanvas(canvas);
    }    
}
