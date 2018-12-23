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
#include "algorithmdisplay.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
void AlgorithmDisplay::init()
{
    ap=NULL;
    IsConstant=false;
    ReadOnly=false;
    fraction=0.5;
}
void AlgorithmDisplay::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void AlgorithmDisplay::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void AlgorithmDisplay::resize(int w, int h)
{
    textEdit1->resize(w-10, (int)((h-40)*fraction));
    textLabel1->setGeometry(5,(int)((h-40)*fraction)+10,74,20);
    listView1->setGeometry(5,(int)((h-40)*fraction)+35, w-10,(int)((h-40)*(1-fraction)));
    line1->setGeometry(0,(int)((h-40)*fraction)+5,w,10);
}
void AlgorithmDisplay::setAlgorithm(algorithm_parameter *algoIn)
{
    ap=algoIn;
    copy=*ap;
    textEdit1->clear();
    listView1->clear();
    if(ap==NULL)
	return;
    vector<string> li=ap->getDescription();
    for(unsigned int i=0;i<li.size();i++)textEdit1->append(li[i].data());
    OnParameterTypeShow();
}
void AlgorithmDisplay::OnParameterTypeShow()
{
    if(ap==NULL)return;
    listView1->clear();
    int nPar=0;
    algorithm_parameter *ptr=ap;
    if(IsConstant)ptr=&copy;
    for(int pID=0;pID<9;pID++)
    {
	nPar=0;
	switch(pID)
	{
	case 0: nPar=ptr->getNumberOfParam<bool>();break;
	case 1: nPar=ptr->getNumberOfParam<point3D>();break;
	case 2: nPar=ptr->getNumberOfParam<vector3D>();break;
	case 3: nPar=ptr->getNumberOfParam<int>();break;
	case 4: nPar=ptr->getNumberOfParam<float>();break;
	case 5: nPar=ptr->getNumberOfParam<string>();break;
	case 6: nPar=ptr->getNumberOfParam<vector<int> >();break;
	case 7: nPar=ptr->getNumberOfParam<vector<float> >();break;
	case 8: nPar=ptr->getNumberOfParam<vector<string> >();break;
	}
	QString nam, val,tpe;
	for(int i=0;i<nPar;i++)
	{
	    switch(pID)
	    {
	    case 0: 
		nam=ptr->getParam<bool>(i).getName().data();
		val=(ptr->getParam<bool>(i).getData()?"true":"false");
		tpe="bool";
		break;
	    case 1: 
		nam=ptr->getParam<point3D>(i).getName().data();
		val=ptr->getParam<point3D>(i).getData().toString().data();
		tpe="point3D";
		break;
	    case 2: 
		nam=ptr->getParam<vector3D>(i).getName().data();
		val=ptr->getParam<vector3D>(i).getData().toString().data();
		tpe="vector3D";
		break;
	    case 3: 
		nam=ptr->getParam<int>(i).getName().data();
		val=QString::number(ptr->getParam<int>(i).getData());
		tpe="int";
		break;
	    case 4: 
		nam=ptr->getParam<float>(i).getName().data();
		val=QString::number(ptr->getParam<float>(i).getData());
		tpe="float";
		break;
	    case 5: 
		nam=ptr->getParam<string>(i).getName().data();
		val=ptr->getParam<string>(i).getData().data();
		tpe="QString";
		break;
	    case 6: {
		    nam=ptr->getParam<vector<int> >(i).getName().data();
		    val="(";
		    for(unsigned int j=0;j<ptr->getParam<vector<int> >(i).getData().size();j++)
			val+=QString::number(ptr->getParam<vector<int> >(i).getData().at(j))+", ";
		    if(val.length()>1)val=val.left(val.length()-2);
		    val+=")";
		    tpe="vector<int>";
		    break;}
	    case 7: {
		    nam=ptr->getParam<vector<float> >(i).getName().data();
		    val="(";
		    for(unsigned int j=0;j<ptr->getParam<vector<float> >(i).getData().size();j++)
			val+=QString::number(ptr->getParam<vector<float> >(i).getData().at(j))+", ";
		    if(val.length()>1)val=val.left(val.length()-2);
		    val+=")";
		    tpe="vector<float>";
		    break;}
	    case 8: {
		    nam=ptr->getParam<vector<string> >(i).getName().data();
		    val="(";
		    for(unsigned int j=0;j<ptr->getParam<vector<int> >(i).getData().size();j++)
			val+=QString(ptr->getParam<vector<string> >(i).getData().at(j).data())+", ";
		    if(val.length()>1)val=val.left(val.length()-2);
		    val+=")";
		    tpe="vector<string>";
		    break;
		}		
	    }
#if QT_VERSION < 0x040000
	    new QListViewItem(listView1, nam, val, tpe);
#else
	    QStringList stmp;stmp<<nam<<val<<tpe;
	    new QTreeWidgetItem(listView1, stmp);
#endif
	}
    }
}
#if QT_VERSION < 0x040000
void AlgorithmDisplay::OnParameterRightClick(QListViewItem *it)
{
#else
void AlgorithmDisplay::OnParameterRightClick(const QPoint &p)
{
  QTreeWidgetItem *it=listView1->itemAt(p);
#endif
    if(ap==NULL&&!IsConstant)return;
    if(it==NULL)return;
    algorithm_parameter *ptr=ap;
    if(IsConstant)ptr=&copy;
    string nme;
#if QT_VERSION < 0x040000
    nme=it->text(0).ascii();
#else
    nme=string(it->text(0).toAscii());
#endif
    if(it->text(2)=="bool")
    {		
	if(it->text(1)=="true")
	{
	    ptr->changeParam<bool>(nme,false);
	    it->setText(1,"false");
	}
	else
	{
	    ptr->changeParam<bool>(nme,true);
	    it->setText(1,"true");
	}
	if(!IsConstant)emit changed();
    }
    else if(it->text(2)=="point3D")
    {
      point3D val=Q3DInputDialog::getLine(this,(string("parameter: ")+nme).data());
      if(val.getState()!=_undefined_)
	{
	  if(!IsConstant&&(val-ptr->getParam<point3D>(nme).getData()).R()>__prec)emit changed();
	  ptr->changeParam<point3D>(nme,val);
	  it->setText(1,val.toString().data());
	}
    }
    else if(it->text(2)=="vector3D")
    {	
	point3D val=Q3DInputDialog::getLine(this,(string("parameter: ")+nme).data());
	if(val.getState()!=_undefined_)
	{
	    if(!IsConstant&&(val-ptr->getParam<vector3D>(nme).getData()).R()>__prec)emit changed();
	    ptr->changeParam<vector3D>(nme,vector3D(val));
	    it->setText(1,val.toString().data());
	}
    }
    else if(it->text(2)=="int")
    {
      int val=QIntegerInputDialog::getLine(this,(string("parameter : ")+nme).data(),it->text(1).toInt());
      if(val!=-10000)
	{
	  if(!IsConstant&&val!=ptr->getParam<int>(nme).getData())emit changed();
	  ptr->changeParam<int>(nme,val);
	  it->setText(1,QString::number(val));
	}
    }
    else if(it->text(2)=="float")
    {	
      float val=QNumberInputDialog::getLine(this,(string("parameter : ")+nme).data(),it->text(1).toFloat());
      if(val<0||val>=0)
	{
	  if(!IsConstant&&val!=ptr->getParam<float>(nme).getData())emit changed();
	  ptr->changeParam<float>(nme,val);
	  it->setText(1,QString::number(val));
	}
    }
    else if(it->text(2)=="QString")
    {
      QString val=QStringInputDialog::getLine(this,(string("parameter : ")+nme).data(),it->text(1));
      if(val!="")
	{
	    if(!IsConstant&&val!=ptr->getParam<string>(nme).getData().data())emit changed();
	    ptr->changeParam<string>(nme,QS2s(val));
	    it->setText(1,val);
	}
    }
    else if(it->text(2)=="vector<int>")
    {
	int nn=-1;
	for(int i=0;i<ptr->getNumberOfParam<vector<int> >();i++)
	    if(ptr->getParam<vector<int> >(i).getName()==nme)
		nn=i;
	if(nn==-1)return;
	vector<int> tmp=ptr->getParam<vector<int> >(nn).getData(), tmpp;
	tmpp=QVectorChangeDialog::getVector(this,(string("parameter : ")+nme).data(),tmp);
	if(tmpp.size()>0)
	{
	    ptr->changeParam<vector<int> >(nme,tmpp);
	    QString nnam="(";
	    for(unsigned int i=0;i<tmpp.size();i++)
		nnam=nnam+QString::number(tmpp[i])+", ";
	    if(tmpp.size()>0)nnam=nnam.left(nnam.length()-2);
	    nnam=nnam+")";
	    it->setText(1,nnam);
	    if(!IsConstant)emit changed();
 	}
    }
    else if(it->text(2)=="vector<float>")
    {
	int nn=-1;
	for(int i=0;i<ptr->getNumberOfParam<vector<float> >();i++)
	    if(ptr->getParam<vector<float> >(i).getName()==nme)
		nn=i;
	if(nn==-1)return;
	vector<float> tmp=ptr->getParam<vector<float> >(nn).getData(),tmpp;
	tmpp=QVectorChangeDialog::getVector(this,(string("parameter : ")+nme).data(),tmp);
	if(tmpp.size()>0)
	{
	    ptr->changeParam<vector<float> >(nme,tmpp);
	    QString nnam="(";
	    for(unsigned int i=0;i<tmpp.size();i++)
		nnam=nnam+QString::number(tmpp[i])+", ";
	    if(tmp.size()>0)nnam=nnam.left(nnam.length()-2);
	    nnam=nnam+")";
	    it->setText(1,nnam);
	    if(!IsConstant)emit changed();
 	}
    }
    else if(it->text(2)=="vector<string>")
    {
	int nn=-1;
	for(int i=0;i<ptr->getNumberOfParam<vector<string> >();i++)
	    if(ptr->getParam<vector<string> >(i).getName()==nme)
		nn=i;
	if(nn==-1)return;
	vector<string> tmp1=ptr->getParam<vector<string> >(nn).getData();
	vector<QString> tmp2;
	for(unsigned int i=0;i<tmp1.size();i++)tmp2.push_back(tmp1[i].data());
	vector<QString> tmpp;
	tmpp=QVectorChangeDialog::getVector(this,(string("parameter : ")+nme).data(),tmp2);
	if(tmpp.size()>0)
	{
	    while(!tmp1.empty())tmp1.pop_back();
	    for(unsigned int i=0;i<tmpp.size();i++)
	      tmp1.push_back(QS2s(tmpp[i]));
	    ptr->changeParam<vector<string> >(nme,tmp1);
	    QString nnam="(";
	    for(unsigned int i=0;i<tmpp.size();i++)
		nnam=nnam+tmpp[i]+", ";
	    if(tmpp.size()>0)nnam=nnam.left(nnam.length()-2);
	    nnam=nnam+")";
	    it->setText(1,nnam);
	    if(!IsConstant)emit changed();
 	}
    }
}
bool AlgorithmDisplay::readOnly()
{
    return ReadOnly;
}
void AlgorithmDisplay::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    if(value)
	disconnect(listView1);
    else
#if QT_VERSION < 0x040000
	connect(listView1,SIGNAL(rightButtonClicked(QListViewItem *,const QPoint &,int)),this, SLOT(OnParameterRightClick(QListViewItem*)));
#else
	connect(listView1,SIGNAL(customContextMenuRequested ( const QPoint & )),this, SLOT(OnParameterRightClick(constQPoint&)));
#endif
}
bool AlgorithmDisplay::isConstant()
{
    return IsConstant;
}
void AlgorithmDisplay::setConstant(bool value)
{
    IsConstant=value;
}
void AlgorithmDisplay::mouseMoveEvent ( QMouseEvent * e )
{
    QWidget *ch=childAt ( e->x(), e->y());
    if(ch==line1)
    {
	if(e->pos().y()<height()-30&&e->pos().y()>30)
	{
	    line1->setGeometry(0,e->pos().y()-5,width(),10);
	    fraction=((float)e->pos().y()-2)/ ((float)height());
	}
	e->accept();
	resize(width(),height());
	return;
    }
}
