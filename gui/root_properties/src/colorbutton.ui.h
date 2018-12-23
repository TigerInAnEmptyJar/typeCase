#include "colorselectdialog.h"
#include "colorbutton.h"

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
#include "string_utilities.h"
void ColorButton::init()
{
    setEnabled(true);
    ReadOnly=false;	
    setWidgetColor((QWidget*)this,ColorSelectDialog::Root2Qt(1));
    connect(this,SIGNAL(clicked()),this,SLOT(onButtonClick()));
}
void ColorButton::destroy()
{
}
int ColorButton::getColor()
{
  return ColorSelectDialog::Qt2Root(getRGBColor());//paletteBackgroundColor());
}
QColor ColorButton::getRGBColor()
{
#if QT_VERSION < 0x040000
  return paletteBackgroundColor();
#else
  return palette().color(QPalette::Background);
#endif
}

bool ColorButton::readOnly()const
{return ReadOnly;}
void ColorButton::onButtonClick()
{
  if(ReadOnly)return;
  ColorSelectDialog fenst(this);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      int c=fenst.getColor();
      QColor cc=fenst.getRGBColor();
      setWidgetColor(this,cc);
      setText(string_number(c).data());
      emit colorChanged(c);
      emit colorChanged(cc);
    }
}
void ColorButton::setColor(int color)
{setWidgetColor(this,ColorSelectDialog::Root2Qt(color));
 setText(string_number(color).data());}
void ColorButton::setColor(const QColor &c)
{setWidgetColor(this,c);
 setText(string_number(ColorSelectDialog::Qt2Root(c)).data());}
void ColorButton::setReadOnly(bool value)
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  if(ReadOnly)
    {
      disconnect(this,SIGNAL(clicked()),this,SLOT(onButtonClick()));
    }
  else
    {
      connect(this,SIGNAL(clicked()),this,SLOT(onButtonClick()));
    }
}
