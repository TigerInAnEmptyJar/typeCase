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
#include "qvectorchangedialog.h"

#include "qstringinputdialog.h"
void QVectorChangeDialog::init()
{
 /*    connect(listBox,SIGNAL(mouseButtonClicked(int, QListBoxItem *, const QPoint &)),this,SLOT(OnListBoxClick(int, QListBoxItem *))); */
}
void QVectorChangeDialog::destroy()
{
    disconnect(listBox);
}
void QVectorChangeDialog::resize(int w, int h)
{
  QDialog::resize(w,h);
/*     listBox->setGeometry(10,10,w-20,h-50); */
/*     Layout1->setGeometry(QRect(10,h-23-18,w-20,23+18)); */
}
void QVectorChangeDialog::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void QVectorChangeDialog::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void QVectorChangeDialog::setVector(const vector<QString> &vec)
{
    listBox->clear();
    for(unsigned int i=0;i<vec.size();i++)
#if QT_VERSION < 0x040000
	listBox->insertItem(vec[i]);
#else
	listBox->addItem(vec[i]);
#endif
}
void QVectorChangeDialog::setVector(const vector<int> &vec)
{
    listBox->clear();
    for(unsigned int i=0;i<vec.size();i++)
#if QT_VERSION < 0x040000
	listBox->insertItem(QString::number(vec[i]));
#else
	listBox->addItem(QString::number(vec[i]));
#endif
}
void QVectorChangeDialog::setVector(const vector<float> &vec)
{
    listBox->clear();
    for(unsigned int i=0;i<vec.size();i++)
#if QT_VERSION < 0x040000
	listBox->insertItem(QString::number(vec[i]));
#else
	listBox->addItem(QString::number(vec[i]));
#endif
}
void QVectorChangeDialog::setVector(const vector<bool> &vec)
{
    listBox->clear();
    for(unsigned int i=0;i<vec.size();i++)
#if QT_VERSION < 0x040000
	listBox->insertItem((vec[i]?"true":"false"));
#else
	listBox->addItem((vec[i]?"true":"false"));
#endif
}
template<class X> 
vector<X> QVectorChangeDialog::getVector()
{
    vector<X> tmp;
    if(this->result()!=QDialog::Accepted)
	return tmp;
    for(int i=0;i<(int)listBox->count();i++)
    {
	void* tt=new X();
#if QT_VERSION < 0x040000
	QString s=listBox->text(i);
#else
	QString s=listBox->item(i)->text();
#endif
	if(typeid(X)==typeid(int))
	    *((int*)tt)=s.toInt();
	else if(typeid(X)==typeid(float))
	    *((float*)tt)=s.toFloat();
	else if(typeid(X)==typeid(QString))
	    *((QString*)tt)=s;
	else if(typeid(X)==typeid(bool))
	    *((bool*)tt)=(s==QString("true")? true:false);
	tmp.push_back(*(X*)tt);
	delete ((X*)tt);
    }
    return tmp;
}
template vector<bool> QVectorChangeDialog::getVector();
template vector<int> QVectorChangeDialog::getVector();
template vector<float> QVectorChangeDialog::getVector();
template vector<QString> QVectorChangeDialog::getVector();

template<class X> 
vector<X> QVectorChangeDialog::getVector(QWidget *par, const QString &cap, const vector<X> &beg)
{
    QVectorChangeDialog fenst(par);
#if QT_VERSION < 0x040000
    fenst.setCaption(cap);
#else
    fenst.setWindowTitle(cap);
#endif
    fenst.setVector(beg);
    fenst.exec();
    return fenst.getVector<X>();
}
template vector<bool> QVectorChangeDialog::getVector(QWidget *par, const QString &cap, const vector<bool> &beg);
template vector<int> QVectorChangeDialog::getVector(QWidget *par, const QString &cap, const vector<int> &beg);
template vector<float> QVectorChangeDialog::getVector(QWidget *par, const QString &cap, const vector<float> &beg);
template vector<QString> QVectorChangeDialog::getVector(QWidget *par, const QString &cap, const vector<QString> &beg);

#if QT_VERSION < 0x040000
void QVectorChangeDialog::OnListBoxClick(int button, QListBoxItem *it)
{
    if(button==Qt::LeftButton)
    {
      QString s=QStringInputDialog::getLine(this,"insert");
      if(s!="")
	listBox->insertItem(s);
    }
    else if(button==Qt::RightButton)
    {
	if(it==NULL)
	    return;
	listBox->takeItem(it);
	delete it;
    }
    else if(button==Qt::MidButton)
    {
	if(it==NULL)
	    return;
      QString s=QStringInputDialog::getLine(this,"replace");
      if(s!="")
	listBox->changeItem(s,listBox->index(it));
    }
}
#else
void QVectorChangeDialog::OnListBoxClick()
{
  QString s=QStringInputDialog::getLine(this,"insert");
  if(s!="")
    listBox->addItem(s);
}
void QVectorChangeDialog::OnListBoxClick(const QPoint &p)
{
  QListWidgetItem *it=listBox->itemAt(p);
  if(it==NULL)
    {
      if(listBox->count()==0)OnListBoxClick();
      return;
    }
  listBox->takeItem(listBox->row(it));
  delete it;
/*     else if(button==Qt::MidButton) */
/*     { */
/* 	if(it==NULL) */
/* 	    return; */
/*       QString s=QStringInputDialog::getLine(this,"replace"); */
/*       if(s!="") */
/* 	it->setText(s); */
/*     } */
}
#endif
