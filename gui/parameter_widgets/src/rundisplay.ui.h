#include "string_utilities.h"
#include "utilities.h"
bool RunDisplay::readOnly() const
{return ReadOnly;}
void RunDisplay::resize( const QSize & s )
{resize(s.width(),s.height());}
void RunDisplay::resizeEvent( QResizeEvent * e )
{resize(e->size().width(),e->size().height());}
void RunDisplay::resize( int w, int h )
{
  QWidget::resize(w,h);
/*   int bh=23; */
/*   int ew=w/2; */
/*   int abt=2; */
/*   int lw=60; */
/*   nameLabel->setGeometry(1,abt,lw,bh); */
/*   nameInput->setGeometry(lw+5,abt,ew-(lw+6),bh); */
/*   beamtimeLabel->setGeometry(1,abt*2+bh,lw,bh); */
/*   beamtimeInput->setGeometry(lw+5,abt*2+bh,ew-lw-6,bh); */
/*   typeLabel->setGeometry(1,abt*3+bh*2,lw,bh); */
/*   typeInput->setGeometry(lw+5,abt*3+bh*2,ew-lw-6,bh); */
/*   runNumberLabel->setGeometry(1,abt*4+bh*3,lw,bh); */
/*   runNumberInput->setGeometry(lw+5,abt*4+bh*3,ew-lw-6,bh); */
/*   descrLabel->setGeometry(1,abt*5+bh*4,ew-2,bh); */
/*   descrInput->setGeometry(1,abt*6+bh*5,ew-2,h-bh*9-abt*11); */

/*   startLabel->setGeometry(1,h-abt*4-bh*4,lw,bh); */
/*   startInput->setGeometry(lw+5,h-abt*4-bh*4,ew-lw-6,bh); */
/*   stopLabel->setGeometry(1,h-abt*3-bh*3,lw,bh); */
/*   stopInput->setGeometry(lw+5,h-abt*3-bh*3,ew-lw-6,bh); */
/*   setupLabel->setGeometry(1,h-abt*2-bh*2,w-3,bh); */
/*   setupInput->setGeometry(1,h-abt-bh,w-5-bh,bh); */
/*   browseButton->setGeometry(w-bh-3,h-abt-bh,bh,bh); */

/*   calibrationLabel->setGeometry(ew+1, (h-4*abt-bh)/2+2*abt,ew-2,bh); */
/*   calInput->setGeometry(ew+1,(h-4*abt-bh)/2+3*abt+bh ,ew-2, (h-4*abt-bh)/2-abt-bh); */
/*   filesInput->setGeometry(ew+1,abt,ew-2,(h-abt*4-bh)/2); */
}
void RunDisplay::setRun( run_parameter & runIn )
{
  sp=&runIn;
  copy=runIn;
  nameInput->setText(QString(copy.getName().data()));
  setupInput->setText(copy.getSetupFile().data()); 
  runNumberInput->setText(string_number(copy.getRunNumber()).data()); 
  tm timm(copy.getStartTime());
  QDateTime dt(QDate(timm.tm_year+1900,timm.tm_mon,timm.tm_mday),QTime(timm.tm_hour,timm.tm_min, timm.tm_sec));
  startInput->setDateTime(dt);
  timm=copy.getStopTime();
  dt=QDateTime(QDate(timm.tm_year+1900,timm.tm_mon,timm.tm_mday),QTime(timm.tm_hour,timm.tm_min, timm.tm_sec));
  stopInput->setDateTime(dt);
  setupLabel->setChecked(copy.hasOwnSetup());
  calibrationLabel->setChecked(copy.hasAdditionalCalibration());
  if(ReadOnly)
    {
      ((QLabel*)beamtimeInput)->setText(beamtimes[copy.getParentNumber()].data());
      ((QLabel*)typeInput)->setText(runtypes[copy.getType()].data());
    }
  else
    {
      setComboBoxCurrentItem((QComboBox*)beamtimeInput,copy.getParentNumber());
      setComboBoxCurrentItem((QComboBox*)typeInput,copy.getType());
    }
  descrInput->clear();
  int n=copy.getDescription().size();
  for(int i=0;i<n;i++)
    descrInput->append(copy.getDescription(i).data());
  calInput->clear();
  filesInput->clear();
#if QT_VERSION < 0x040000
  for(int i=0;i<copy.getNumberOfCalibrationFiles();i++)
    calInput->insertItem(copy.getCalibrationFile(i).data());
  for(int i=0;i<copy.getNumberOfFiles();i++)
    new QListViewItem(filesInput,string_number(copy.getFileType(i)).data(),copy.getFile(i).data());
#else
  for(int i=0;i<copy.getNumberOfCalibrationFiles();i++)
    calInput->addItem(copy.getCalibrationFile(i).data());
  for(int i=0;i<copy.getNumberOfFiles();i++)
    {
      QStringList tmmp;tmmp<<string_number(copy.getFileType(i)).data()
			   <<copy.getFile(i).data();
      new QTreeWidgetItem(filesInput,tmmp);
    }
#endif
}
void RunDisplay::setReadOnly( bool value )
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  nameInput->setReadOnly(ReadOnly);
  setupInput->setReadOnly(ReadOnly);
  descrInput->setReadOnly(ReadOnly);
  runNumberInput->setReadOnly(ReadOnly);
  setupLabel->setEnabled(!ReadOnly);
  calibrationLabel->setEnabled(!ReadOnly);
  stopInput->setEnabled(!ReadOnly);
  startInput->setEnabled(!ReadOnly);
  if(beamtimeInput!=NULL)
    {
      disconnect(beamtimeInput);
#if QT_VERSION < 0x040000
      layout->remove(beamtimeInput);
#else
      layout->removeWidget(beamtimeInput);
#endif
      delete beamtimeInput;
    }
  if(typeInput!=NULL)
    {
      disconnect(typeInput);
#if QT_VERSION < 0x040000
      layout->remove(typeInput);
#else
      layout->removeWidget(typeInput);
#endif
      delete typeInput;
    }
  if(ReadOnly)
    {
      beamtimeInput=new QLabel(this);
      ((QLabel*)beamtimeInput)->setText(string_number(copy.getMonth()).data());
      typeInput=new QLabel(this);
      ((QLabel*)typeInput)->setText(string_number(copy.getYear()).data());
    }
  else
    {
      beamtimeInput=new QComboBox(this);
      typeInput=new QComboBox(this);
      for(unsigned int i=0;i<beamtimes.size();i++)
	add2ComboBox((QComboBox*)beamtimeInput,beamtimes[i].data());
      for(unsigned int i=0;i<runtypes.size();i++)
	add2ComboBox((QComboBox*)typeInput,runtypes[i].data());
      setComboBoxCurrentItem((QComboBox*)typeInput,copy.getType());
      setComboBoxCurrentItem((QComboBox*)beamtimeInput,copy.getParentNumber());
      connect(beamtimeInput,SIGNAL(highlighted(int)),this,SLOT(onPropertyChange()));
      connect(typeInput,SIGNAL(highlighted(int)),this,SLOT(onPropertyChange()));
    }
/*   beamtimeInput->setGeometry(60+5,1,width()/2-(60+6),23); */
/*   beamtimeInput->show(); */
/*   typeInput->setGeometry(60+5,1,width()/2-(60+6),23); */
/*   typeInput->show(); */
  resize(width(),height());
}
void RunDisplay::onPropertyChange()
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
	    copy.setName(QS2s(nameInput->text()));
	}
      else if(tmp==setupInput)
	{
	  if(ReadOnly)
	    setupInput->setText(copy.getSetupFile().data());
	  else
	    copy.setSetupFile(QS2s(setupInput->text()));
	}
      else if(tmp==runNumberInput)
	{
	  if(ReadOnly)
	    runNumberInput->setText(string_number(copy.getRunNumber()).data());
	  else
	    copy.setRunNumber(runNumberInput->text().toInt());
	}
      else if(tmp==startInput)
	{
	  if(ReadOnly)
	    {
	      tm timm=copy.getStartTime();
	      startInput->setDateTime(QDateTime(QDate(timm.tm_year+1900,timm.tm_mon,timm.tm_mday),QTime(timm.tm_hour,timm.tm_min, timm.tm_sec)));
	    }
	  else
	    {
	      tm t;
	      t.tm_year=startInput->dateTime().date().year();
	      t.tm_mon=startInput->dateTime().date().month();
	      t.tm_mday=startInput->dateTime().date().day();
	      t.tm_hour=startInput->dateTime().time().hour();
	      t.tm_min=startInput->dateTime().time().minute();
	      t.tm_sec=startInput->dateTime().time().second();
	      copy.setStartTime(t);
	    }
	}
      else if(tmp==stopInput)
	{
	  if(ReadOnly)
	    {
	      tm timm=copy.getStopTime();
	      stopInput->setDateTime(QDateTime(QDate(timm.tm_year+1900,timm.tm_mon,timm.tm_mday),QTime(timm.tm_hour,timm.tm_min, timm.tm_sec)));
	    }
	  else
	    {
	      tm t;
	      t.tm_year=stopInput->dateTime().date().year();
	      t.tm_mon=stopInput->dateTime().date().month();
	      t.tm_mday=stopInput->dateTime().date().day();
	      t.tm_hour=stopInput->dateTime().time().hour();
	      t.tm_min=stopInput->dateTime().time().minute();
	      t.tm_sec=stopInput->dateTime().time().second();
	      copy.setStopTime(t);
	    }
	}
    }
  if(!ReadOnly)
    {
      (*sp)=copy;
      emit changed();
    }
}
void RunDisplay::onBTChange()
{
  QObject *tmp=focusWidget();
  if(tmp==NULL)return;
  if(ReadOnly)
    ((QLabel*)beamtimeInput)->setText(beamtimes[copy.getParentNumber()].data());
  else
    {
      copy.setParentNumber(comboBoxCurrentItem((QComboBox*)beamtimeInput));
      (*sp)=copy;
      emit changed();
    }
}
void RunDisplay::onTPChange()
{
  QObject *tmp=focusWidget();
  if(tmp==NULL)return;
  if(ReadOnly)
    ((QLabel*)typeInput)->setText(runtypes[copy.getType()].data());
  else
    {
      copy.setType(comboBoxCurrentItem((QComboBox*)typeInput));
      (*sp)=copy;
      emit changed();
    }
}
void RunDisplay::init()
{
#if QT_VERSION < 0x040000
  filesInput->addColumn("Type");
  filesInput->addColumn("File-name");
#else
  QStringList tmmp;tmmp<<"Type"<<"File-name";
  filesInput->setHeaderLabels(tmmp);
#endif
  runNumberInput->setInputMask("00000009");
  tmpItem=0;
}
void RunDisplay::destroy()
{
}
extern bool existing(string filename);
void RunDisplay::onBrowseClick()
{
  if(ReadOnly||!setupLabel->isChecked())return;
  string s=getOpenFileName(this, "Open Setup file", "", "Detector description files (*.data)");
  if(s=="")return;
  if(!existing(s))return;
  copy.setSetupFile(s);
  sp->setSetupFile(s);
  setupInput->setText(s.data());
  emit changed();
}
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
void RunDisplay::onCalibrationRightClick(QListBoxItem *it, const QPoint&p)
{
  if(ReadOnly||!calibrationLabel->isChecked())return;
  QPopupMenu pm;
  pm.insertItem("Add calibration file",this,SLOT(onCalibrationAdd()),QKeySequence("CTRL+a"));
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
#include <QMenu>
void RunDisplay::onCalibrationRightClick(const QPoint&p)
{
  QListWidgetItem *it=calInput->itemAt(p);
  if(ReadOnly||!calibrationLabel->isChecked())return;
  QMenu pm;
  pm.addAction("Add calibration file",this,SLOT(onCalibrationAdd()),QKeySequence("CTRL+a"));
  if(it!=NULL)
    {
      int ind=calInput->row(it);
      if(ind>=0&&ind<(int)calInput->count())
	{
	  pm.addAction("Change calibration file"/*,this,SLOT(onCalibrationChange(QAction*)),QKeySequence("CTRL+g")*/)->setStatusTip(QString::number(ind));
	  pm.addAction("Remove calibration file"/*,this,SLOT(onCalibrationRemove(QAction*)),QKeySequence("CTRL+d")*/)->setStatusTip(QString::number(ind));
	}
    }
  connect(&pm,SIGNAL(triggered(QAction*)),this,SLOT(onCalibration(QAction*)));
  pm.exec(calInput->viewport()->mapToGlobal(p));
  disconnect(&pm);
}
void RunDisplay::onCalibration(QAction* it)
{
  if(it==NULL)return;
  int num=it->statusTip().toInt();
  if(it->text()=="Change calibration file")
    onCalibrationChange(num);
  else if(it->text()=="Remove calibration file")
    onCalibrationRemove(num);
}
#endif
void RunDisplay::onCalibrationRemove(int id)
{
  if(ReadOnly||!calibrationLabel->isChecked())return;
  if(id<0||id>=(int)calInput->count())return;
#if QT_VERSION < 0x040000
  string s=calInput->text(id).ascii();
#else
  string s=QS2s(calInput->item(id)->text());
#endif
  vector<string> tmp;
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
/* #if QT_VERSION > 0x040000 */
/* void RunDisplay::onCalibrationRemove(QAction* it) */
/*   { */
/*     if(it==NULL)return; */
/*     int id=it->statusTip().toInt(); */
/*     string s=QS2s(calInput->item(id)->text()); */
/*   vector<string> tmp; */
/*   for(int i=0;i<copy.getNumberOfCalibrationFiles();i++) */
/*     { */
/*       if(s==copy.getCalibrationFile(i).data())continue; */
/*       tmp.push_back(copy.getCalibrationFile(i)); */
/*     } */
/* #if QT_VERSION < 0x040000 */
/*   calInput->removeItem(id); */
/* #else */
/*   delete calInput->takeItem(id); */
/* #endif */
/*   copy.clearCalibrationFiles(); */
/*   sp->clearCalibrationFiles(); */
/*   for(unsigned int i=0;i<tmp.size();i++) */
/*     { */
/*       copy.addCalibrationFile(tmp[i]); */
/*       sp->addCalibrationFile(tmp[i]); */
/*     } */
/*   emit changed(); */
/* } */
/* #endif */
void RunDisplay::onCalibrationAdd()
{
  if(ReadOnly||!calibrationLabel->isChecked())return;
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

void RunDisplay::onCalibrationChange(int id)
{
  if(ReadOnly||!calibrationLabel->isChecked())return;
  int pos=-1;
  string s1=
#if QT_VERSION < 0x040000
  calInput->text(id).ascii();
#else
  QS2s(calInput->item(id)->text());
#endif
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
/* #if QT_VERSION > 0x040000 */
/* void RunDisplay::onCalibrationChange(QAction *it) */
/* { */
/*   if(it==NULL)return; */
/*   int id=it->statusTip().toInt(); */
/*   if(ReadOnly||!calibrationLabel->isChecked())return; */
/*   int pos=-1; */
/*   string s1= */
/* #if QT_VERSION < 0x040000 */
/*   calInput->text(id).ascii(); */
/* #else */
/*   QS2s(calInput->item(id)->text()); */
/* #endif */
/*   for(int i=0;i<copy.getNumberOfCalibrationFiles();i++) */
/*     if(s1==copy.getCalibrationFile(i).data())pos=i; */
/*   if(pos<0)return; */
/*   string s=getOpenFileName(this,"Open calibration file","","Calibration files (*.cal; *.dat)");  */
/*   if(s=="")return; */
/*   if(!existing(s))return; */
/*   copy.setCalibrationFile(pos,s); */
/*   sp->setCalibrationFile(pos,s); */
/* #if QT_VERSION < 0x040000 */
/*   calInput->changeItem(s.data(),id); */
/* #else */
/*   calInput->item(id)->setText(s.data()); */
/* #endif */
/*   emit changed(); */
/* } */
/* #endif */
void RunDisplay::onDescriptionChange()
{
  if(ReadOnly)
    {
    }
  else
    {
      emit changed();
    }
}
#if QT_VERSION < 0x040000
void RunDisplay::onFilesRightClick(QListViewItem *it,const QPoint&p)
{
  if(ReadOnly)return;
  QPopupMenu pm;
  pm.insertItem("Add file",this,SLOT(onFileAdd()),QKeySequence("CTRL+a"),-1);
  tmpItem=it;
  if(it!=NULL)
    {
      pm.insertItem("Change file type",this,SLOT(onFileTypeChange()),QKeySequence("CTRL+t"));
      pm.insertItem("Change file name",this,SLOT(onFileNameChange()),QKeySequence("CTRL+n"));
      pm.insertItem("Remove file",this,SLOT(onFileRemove()),QKeySequence("CTRL+d"));
    }
  pm.exec(p);
}
#else
void RunDisplay::onFilesRightClick(const QPoint&p)
{
  if(ReadOnly)return;
  QTreeWidgetItem *it=filesInput->itemAt(p);
  QMenu pm;
  pm.addAction("Add file",this,SLOT(onFileAdd()),QKeySequence("CTRL+a"));
  tmpItem=it;
  if(it!=NULL)
    {
      pm.addAction("Change file type",this,SLOT(onFileTypeChange()),QKeySequence("CTRL+t"));
      pm.addAction("Change file name",this,SLOT(onFileNameChange()),QKeySequence("CTRL+n"));
      pm.addAction("Remove file",this,SLOT(onFileRemove()),QKeySequence("CTRL+d"));
    }
  pm.exec(filesInput->viewport()->mapToGlobal(p));
}
#endif
void RunDisplay::onFileAdd()
{
  if(ReadOnly)return;
  string s=getOpenFileName(this,"Open file","","TADE files (*.evt; *.tade) ; root files (*.root)"); 
  if(s=="")return;
  if(!existing(s))return;
  copy.addFile(s,0,-1);
  sp->addFile(s,0,-1);
#if QT_VERSION < 0x040000
  new QListViewItem(filesInput,"0",s.data());
#else
  QStringList li;li<<"0"<<s.data();
  new QTreeWidgetItem(filesInput,li);
#endif
  emit changed();
}
void RunDisplay::onFileTypeChange()
{
  if(ReadOnly)return;
  if(tmpItem==NULL)return;
  int pos=-1;
  for(int i=0;i<copy.getNumberOfFiles();i++)
    if(tmpItem->text(1)==copy.getFile(i).data())pos=i;
  if(pos<0)return;
  int value=QIntegerInputDialog::getLine(this,"",copy.getFileType(pos));
  if(value==-10000)return;
  copy.setFileType(pos,value);
  sp->setFileType(pos,value);
  tmpItem->setText(0,string_number(value).data());
  tmpItem=NULL;
  emit changed();
}
void RunDisplay::onFileNameChange()
{
  if(ReadOnly)return;
  if(tmpItem==NULL)return;
  int pos=-1;
  for(int i=0;i<copy.getNumberOfFiles();i++)
    if(tmpItem->text(1)==copy.getFile(i).data())pos=i;
  if(pos<0)return;
  string value;
  string s=getOpenFileName(this,"Open file","","TADE files (*.evt; *.tade) ; root files (*.root)"); 
  if(s=="")return;
  if(!existing(s))return;
/*   copy.setFile(pos,s); */
/*   sp->setFile(pos,s); */
  tmpItem->setText(1,s.data());
  tmpItem=NULL;
  emit changed();
}
void RunDisplay::onFileRemove()
{
  if(ReadOnly)return;
  if(tmpItem==NULL)return;
  vector<string> tmp;
  vector<int> tmpi1,tmpi2;
  for(int i=0;i<copy.getNumberOfFiles();i++)
    {
      if(tmpItem->text(1)==copy.getFile(i).data())continue;
      tmp.push_back(copy.getFile(i));
      tmpi1.push_back(copy.getFileType(i));
      tmpi2.push_back(copy.getFileEvents(i));
    }
#if QT_VERSION < 0x040000
  filesInput->takeItem(tmpItem);
#else
  filesInput->takeTopLevelItem(filesInput->indexOfTopLevelItem(tmpItem));
#endif
  copy.clearFiles();
  sp->clearFiles();
  for(unsigned int i=0;i<tmp.size();i++)
    {
      copy.addFile(tmp[i],tmpi1[i],tmpi2[i]);
      sp->addFile(tmp[i],tmpi1[i],tmpi2[i]);
    }
  delete tmpItem;
  tmpItem=NULL;
  emit changed();
}
void RunDisplay::setBeamtimes(vector<string> btIn)
{
  while(!beamtimes.empty())beamtimes.pop_back();
  for(unsigned int i=0;i<btIn.size();i++)
      beamtimes.push_back(btIn[i]);
  if(!ReadOnly)
    {
      ((QComboBox*)beamtimeInput)->clear();
      for(unsigned int i=0;i<beamtimes.size();i++)
	add2ComboBox((QComboBox*)beamtimeInput,beamtimes[i].data());
    }
}
void RunDisplay::setRunTypes(vector<string> rtIn)
{
  while(!runtypes.empty())runtypes.pop_back();
  for(unsigned int i=0;i<rtIn.size();i++)
      runtypes.push_back(rtIn[i]);
  if(!ReadOnly)
    {
      ((QComboBox*)typeInput)->clear();
      for(unsigned int i=0;i<runtypes.size();i++)
	add2ComboBox((QComboBox*)typeInput,runtypes[i].data());
    }
}

void RunDisplay::onSetupChange()
{
  setupInput->setEnabled(setupLabel->isChecked());
  copy.setOwnSetup(setupLabel->isChecked());
  if(!ReadOnly)
    sp->setOwnSetup(setupLabel->isChecked());
}
void RunDisplay::onCalibrationChange()
{
  calInput->setEnabled(calibrationLabel->isChecked());
  copy.setAdditionalCalibration(calibrationLabel->isChecked());
  if(!ReadOnly)
    sp->setAdditionalCalibration(calibrationLabel->isChecked());
}
