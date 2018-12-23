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
#include "qnumberinputdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <math.h>
#include "string_utilities.h"
double QNumberInputDialog::line()
{
    return lineEdit->text().toDouble();
}


double QNumberInputDialog::getLine(QWidget *par, const QString &capt,float start)
{
    QNumberInputDialog fenst(par);
#if QT_VERSION < 0x040000
    fenst.setCaption(capt);
#else
    fenst.setWindowTitle( capt);
#endif
    fenst.setStart(start);
    fenst.exec();
    if(fenst.result()==QDialog::Accepted) return fenst.line();
    else return sqrt(-1.);
}
void QNumberInputDialog::setStart(float n)
{
  lineEdit->setText(string_number(n).data());
}
void QNumberInputDialog::resize( const QSize & s )
{resize(s.width(),s.height());}
void QNumberInputDialog::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void QNumberInputDialog::resize( int w, int h )
{
  QDialog::resize(w,h);
/*   int hh=h-buttonOk->geometry().height()-18; */
/*   int abt=(hh-30-25)/2; */
/*   lineEdit->setGeometry(10,abt,w-20,25); */
/*   ((QWidget*)Layout1->parent())->setGeometry(QRect(10,hh,w-20,buttonOk->geometry().height()+18)); */
}
