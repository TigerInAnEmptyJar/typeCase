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
#include "q3dinputdialog.h"
#include "q3Dinput.h"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "geometry.h"
void Q3DInputDialog::init()
{
}
void Q3DInputDialog::destroy()
{
}
point3D Q3DInputDialog::line()
{
    if(this->result()!=QDialog::Accepted)
	return point3D();
    return point3D(q3DInput1->getX(),q3DInput1->getY(),q3DInput1->getZ());
}
point3D Q3DInputDialog::getLine(QWidget *parentIn, const QString &captionIn, const point3D &start)
{
    Q3DInputDialog fenst(parentIn);
#if QT_VERSION < 0x040000
    fenst.setCaption(captionIn);
#else
    fenst.setWindowTitle(captionIn);
#endif
    fenst.exec();
    fenst.q3DInput1->setValues(start.x(),start.y(),start.z());
    if(fenst.result()==QDialog::Accepted)
	return fenst.line();
    return point3D();
}

void Q3DInputDialog::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void Q3DInputDialog::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void Q3DInputDialog::resize( int w, int h )
{
  QDialog::resize(w,h);
}
