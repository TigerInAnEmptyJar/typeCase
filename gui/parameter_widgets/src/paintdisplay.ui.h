#include "string_utilities.h"
#include <colorselectdialog.h>
void PaintDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void PaintDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void PaintDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
}
void PaintDisplay::setPaint( paint_parameter & paintIn )
{
  sp=&paintIn;
  copy=paintIn;
  nameInput->setText(copy.getName().data());
  idInput->setText(string_number(copy.getID()).data());
  doPaint->setChecked(copy.isToPaint());
  setComboBoxCurrentItem(markerInput,copy.getMarker()-25);
  colorInput->setColor(copy.getColor());
}

void PaintDisplay::setReadOnly( bool value )
{
  if(ReadOnly==value)return;
  ReadOnly=value;
  nameInput->setReadOnly(ReadOnly);
  idInput->setReadOnly(ReadOnly);
  markerInput->setEnabled(!ReadOnly);
  doPaint->setEnabled(!ReadOnly);
  colorInput->setReadOnly(ReadOnly);
}

void PaintDisplay::init()
{
  ReadOnly=false;
}
void PaintDisplay::destroy()
{
}
void PaintDisplay::onPropertyChange()
{
  if(nameInput==focusWidget())
    {
      copy.setName(QS2s(((QLineEdit*)nameInput)->text()));
    }
  else if(idInput==focusWidget())
    {
      copy.setID(((QLineEdit*)idInput)->text().toInt());
    }
  if(!ReadOnly)
    {
      (*sp)=copy;
      emit changed();
    }
}
bool PaintDisplay::readOnly()
{return ReadOnly;}
void PaintDisplay::onPaintClick()
{
  if(ReadOnly)doPaint->setChecked(copy.isToPaint());
  else
    {
      copy.setToPaint(doPaint->isChecked());
      (*sp)=copy;
      emit changed();
    }
}
void PaintDisplay::onMarkerClick()
{
  if(ReadOnly)setComboBoxCurrentItem(markerInput,copy.getMarker()-25);
  else
    {
      copy.setMarker(comboBoxCurrentItem(markerInput)+25);
      (*sp)=copy;
      emit changed();
    }
}
void PaintDisplay::onColorSelected(int c)
{
  if(ReadOnly)return;
  if(sp==NULL)return;
  copy.setColor(c);
  sp->setColor(c);
  emit changed();
}
void PaintDisplay::setMarker(vector<string> namesIn)
{
  while(!markerNames.empty())markerNames.pop_back();
  markerInput->clear();
  for(unsigned int i=0;i<namesIn.size();i++)
    {
      markerNames.push_back(namesIn[i]);
      add2ComboBox(markerInput,markerNames[i].data());
    }
  setComboBoxCurrentItem(markerInput,copy.getMarker()-25);
}
void PaintDisplay::setMarker(vector<QPixmap*> iconsIn)
{
  while(!markerIcons.empty())markerIcons.pop_back();
  markerInput->clear();
  for(unsigned int i=0;i<iconsIn.size();i++)
    {
      if(iconsIn[i]==NULL)continue;
      markerIcons.push_back(iconsIn[i]);
#if QT_VERSION < 0x040000
      markerInput->insertItem(*(markerIcons[i]));
#else
      markerInput->addItem(QIcon(*(markerIcons[i])),QString::number(i));
#endif
    }
  setComboBoxCurrentItem(markerInput,copy.getMarker()-25);
}
