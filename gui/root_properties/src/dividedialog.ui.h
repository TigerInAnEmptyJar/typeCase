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
#include "dividedialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

void DivideDialog::setNumbers( int nX, int nY )
{
    spinBox1->setValue(nX);
    spinBox2->setValue(nY);
}


int DivideDialog::getX()
{
    return spinBox1->value();
}


int DivideDialog::getY()
{
    return spinBox2->value();
}

void DivideDialog::accept()
{
    QDialog::accept();
}
