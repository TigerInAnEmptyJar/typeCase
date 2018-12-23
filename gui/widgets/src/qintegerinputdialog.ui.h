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

#include "qintegerinputdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

void QIntegerInputDialog::setStart(int n)
{
  spinBox5->setValue(n);
}
int QIntegerInputDialog::line()
{
    return spinBox5->value();
}


int QIntegerInputDialog::getLine( QWidget * par, const QString & capt , int start)
{
    QIntegerInputDialog f(par);
#if QT_VERSION < 0x040000
    f.setCaption(capt);
#else
    f.setWindowTitle(capt);
#endif
    f.setStart(start);
    f.exec();
    if(f.result()==QDialog::Accepted)
      return f.line();
    return -10000;
}
void QIntegerInputDialog::resize( const QSize & s )
{resize(s.width(),s.height());}
void QIntegerInputDialog::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
#include <iostream>
void QIntegerInputDialog::resize( int w, int h )
{
  QDialog::resize(w,h);
}
