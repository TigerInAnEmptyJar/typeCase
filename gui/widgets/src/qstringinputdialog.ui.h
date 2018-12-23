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
#include "qstringinputdialog.h"

#include <qvariant.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
QString QStringInputDialog::line()
{
    return lineEdit->text();
}


QString QStringInputDialog::getLine(QWidget *par, const QString &capt, const QString &start)
{
    QStringInputDialog fenst(par);
#if QT_VERSION < 0x040000
    fenst.setCaption(capt);
#else
    fenst.setWindowTitle(capt);
#endif
    fenst.setStart(start);
    fenst.exec();
    if(fenst.result()==QDialog::Accepted) return fenst.line();
    else return "";
}
void QStringInputDialog::resize( const QSize & s )
{resize(s.width(),s.height());}
void QStringInputDialog::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
#include <iostream>
void QStringInputDialog::resize( int w, int h )
{
  QDialog::resize(w,h);
/*   int hh=h-buttonOk->geometry().height()-18; */
/*   int abt=(hh-30-25)/2; */
/*   lineEdit->setGeometry(10,abt,w-20,25); */
/*   ((QWidget*)Layout1->parent())->setGeometry(QRect(10,hh,w-20,buttonOk->geometry().height()+18)); */
}
void QStringInputDialog::setStart(const QString& n)
{
  lineEdit->setText(n);
}
