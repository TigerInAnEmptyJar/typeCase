#include "string_utilities.h"
#include <qfiledialog.h>
#include "utilities.h"
bool BeamTimeDisplay::readOnly() const
{return ReadOnly;}
void BeamTimeDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void BeamTimeDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void BeamTimeDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
}
void BeamTimeDisplay::setBeamTime( beamTime_parameter & beamTimeIn )
{
  sp=&beamTimeIn;
  copy=beamTimeIn;
  nameInput->setText(copy.getName().data());
  setupInput->setText(copy.getSetupFile().data());
  if(ReadOnly)
    {
      ((QLabel*)monthInput)->setText(string_number(copy.getMonth()).data());
      ((QLabel*)yearInput)->setText(string_number(copy.getYear()).data());
    }
  else
    {
      ((QSpinBox*)monthInput)->setValue(copy.getMonth());
      ((QSpinBox*)yearInput)->setValue(copy.getYear());
    }
  calInput->clear();
  for(int i=0;i<copy.getNumberOfCalibrationFiles();i++)
#if QT_VERSION < 0x040000
    calInput->insertItem(copy.getCalibrationFile(i).data());
#else
    calInput->addItem(copy.getCalibrationFile(i).data());
#endif
  descrInput->clear();
  int n=copy.getDescription().size();
  for(int i=0;i<n;i++)
    descrInput->append(copy.getDescription(i).data());
}
void BeamTimeDisplay::setReadOnly( bool value )
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  nameInput->setReadOnly(ReadOnly);
  setupInput->setReadOnly(ReadOnly);
  descrInput->setReadOnly(ReadOnly);
  if(monthInput!=NULL)
    {
      disconnect(monthInput);
#if QT_VERSION < 0x040000
      layout->remove(monthInput);
#else
      layout->removeWidget(monthInput);
#endif
      delete monthInput;
    }
  if(yearInput!=NULL)
    {
      disconnect(yearInput);
#if QT_VERSION < 0x040000
      layout->remove(yearInput);
#else
      layout->removeWidget(yearInput);
#endif
      delete yearInput;
    }
  if(ReadOnly)
    {
      monthInput=new QLabel(this);//,"monthInput");
      ((QLabel*)monthInput)->setText(string_number(copy.getMonth()).data());
      yearInput=new QLabel(this);//,"yearInput");
      ((QLabel*)yearInput)->setText(string_number(copy.getYear()).data());
    }
  else
    {
      monthInput=new QSpinBox(this);//,"monthInput");
      ((QSpinBox*)monthInput)->setValue(copy.getMonth());
      yearInput=new QSpinBox(this);//,"yearInput");
      ((QSpinBox*)yearInput)->setValue(copy.getYear());
      connect(monthInput,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
      connect(yearInput,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    }
#if QT_VERSION < 0x040000
    layout->addMultiCellWidget(monthInput,2,0,2,2);
    layout->addMultiCellWidget(yearInput,2,2,2,3);
#else
    layout->addWidget(monthInput,2,0,1,2);
    layout->addWidget(yearInput,2,2,1,2);
#endif
  resize(width(),height());
}
void BeamTimeDisplay::onPropertyChange()
{
  QObject *tmp=focusWidget();
  if(tmp==NULL)return;
  if(tmp->parent()==NULL)return;
  while(tmp->parent()!=NULL&&tmp->parent()!=this)tmp=tmp->parent();
  if(tmp->parent()==NULL)return;
  if(tmp!=NULL)
    {
      if(tmp==nameInput)
	{
	  if(ReadOnly)
	      nameInput->setText(copy.getName().data());
	  else
	    copy.setName(string(nameInput->text().
#if QT_VERSION < 0x040000
			 ascii()));
#else
	  toAscii())); 
#endif
	}
      else if(tmp==setupInput)
	{
	  if(ReadOnly)
	      setupInput->setText(copy.getSetupFile().data());
	  else
	    copy.setSetupFile(string(setupInput->text().
#if QT_VERSION < 0x040000
			 ascii()));
#else
	  toAscii())); 
#endif
	}
      else if(tmp==monthInput)
	{
	  if(ReadOnly)
	    ((QLabel*)monthInput)->setText(string_number(copy.getMonth()).data());
	  else
	    copy.setData(((QSpinBox*)monthInput)->value(),copy.getYear());
	}
      else if(tmp==yearInput)
	{
	  if(ReadOnly)
	    ((QLabel*)yearInput)->setText(string_number(copy.getYear()).data());
	  else
	    copy.setData(copy.getMonth(),((QSpinBox*)yearInput)->value());
	}
    }
  if(!ReadOnly)
    {
      (*sp)=copy;
      emit changed();
    }
}
void BeamTimeDisplay::init()
{

}
void BeamTimeDisplay::destroy()
{
}
extern bool existing(string filename);
void BeamTimeDisplay::onBrowseClick()
{
  if(ReadOnly)return;
  string s=getOpenFileName(this,"Open Setup file","","Detector description files (*.data)");
  if(!existing(s)||s=="")return;
  copy.setSetupFile(s);
  sp->setSetupFile(s);
  setupInput->setText(s.data());
  emit changed();
}
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
#if QT_VERSION < 0x040000
void BeamTimeDisplay::onCalibrationRightClick(QListBoxItem *it, const QPoint&p)
{
  if(ReadOnly)return;
  QPopupMenu pm;
  pm.insertItem("Add calibration file",this,SLOT(onCalibrationAdd()),QKeySequence("CTRL+a"),-1);
  if(it!=NULL)
    {
      int ind=calInput->index(it);
      if(ind>=0&&ind<(int)calInput->count())
	{
	  pm.insertItem("Change calibration file",this,SLOT(onCalibrationChange(int)),QKeySequence("CTRL+g"),ind);
	  pm.insertItem("Remove calibration file",this,SLOT(onCalibrationRemove(int)),QKeySequence("CTRL+d"),ind);
	}
    }
  pm.exec(p);
}
#else
void BeamTimeDisplay::onCalibrationRightClick(const QPoint&p)
{
  QListWidgetItem *it=calInput->itemAt(p);
  if(ReadOnly)return;
  QMenu pm;
  pm.addAction("Add calibration file");//,this,SLOT(onCalibrationAdd()),QKeySequence("CTRL+a"));
  if(it!=NULL)
    {
      int ind=calInput->row(it);
      if(ind>=0&&ind<(int)calInput->count())
	{
	  pm.addAction("Change calibration file")/*,this,SLOT(onCalibrationChange()),QKeySequence("CTRL+g"))*/->setStatusTip(QString::number(ind));
	  pm.addAction("Remove calibration file")/*,this,SLOT(onCalibrationRemove()),QKeySequence("CTRL+d"))*/->setStatusTip(QString::number(ind));
	}
    }
  connect(&pm,SIGNAL(triggered(QAction*)),this,SLOT(onCalibrationDo(QAction*)));
  pm.exec(calInput->viewport()->mapToGlobal(p));
}

void BeamTimeDisplay::onCalibrationDo(QAction* ac)
{
  if(ac==NULL)return;
  int i=ac->statusTip().toInt();
  string s=QS2s(ac->text());
  if(s=="Add calibration file")
    {
      onCalibrationAdd();
    }
  else if(s=="Change calibration file")
    {
      onCalibrationChange(i);
    }
  else if(s=="Remove calibration file")
    {
      onCalibrationRemove(i);
    }
}
#endif
void BeamTimeDisplay::onCalibrationRemove(int id)
{
  if(ReadOnly)return;
  if(id<0||id>=(int)calInput->count())return;
  vector<string> tmp;
  string s=QS2s(
#if QT_VERSION < 0x040000
  calInput->text(id)
#else
  calInput->item(id)->text()
#endif
  );
  for(int i=0;i<copy.getNumberOfCalibrationFiles();i++)
    {
      if(s==copy.getCalibrationFile(i).data())continue;
      tmp.push_back(copy.getCalibrationFile(i));
    }
#if QT_VERSION < 0x040000
  calInput->removeItem(id);
#else
  delete calInput->takeItem(id);
#endif
  copy.clearCalibrationFiles();
  sp->clearCalibrationFiles();
  for(unsigned int i=0;i<tmp.size();i++)
    {
      copy.addCalibrationFile(tmp[i]);
      sp->addCalibrationFile(tmp[i]);
    }
  emit changed();
}
void BeamTimeDisplay::onCalibrationChange(int id)
{
  if(ReadOnly)return;
  int pos=-1;
  string s1=QS2s(
#if QT_VERSION < 0x040000
  calInput->text(id)
#else
  calInput->item(id)->text()
#endif
  );
  for(int i=0;i<copy.getNumberOfCalibrationFiles();i++)
    if(s1==copy.getCalibrationFile(i).data())pos=i;
  if(pos<0)return;
  string s=getOpenFileName(this,"Open calibration file","","Calibration files (*.cal; *.dat)");
  if(s=="")return;
  if(!existing(s))return;
  copy.setCalibrationFile(pos,s);
  sp->setCalibrationFile(pos,s);
#if QT_VERSION < 0x040000
  calInput->changeItem(s.data(),id);
#else
  calInput->item(id)->setText(s.data());
#endif
  emit changed();
}
void BeamTimeDisplay::onCalibrationAdd()
{
  if(ReadOnly)return;
  string s=getOpenFileName(this,"Open calibration file","","Calibration files (*.cal *.dat);;any files (*.*)"); 
  if(s=="")return;
  if(!existing(s))return;
  copy.addCalibrationFile(s);
  sp->addCalibrationFile(s);
#if QT_VERSION < 0x040000
  calInput->insertItem(s.data());
#else
  calInput->addItem(s.data());
#endif
  emit changed();
}
void BeamTimeDisplay::onDescriptionChange()
{
  if(ReadOnly)
    {
    }
  else
    {
      emit changed();
    }
}
