#include <qmessagebox.h>
void DetectorWidget::init()
{
  hatch=0;
  mode=-1;
  ReadOnly=false;
  pdisplay->setReadOnly(ReadOnly);
  fraction=0.3;
#if QT_VERSION < 0x040000
  listView->setSortColumn(-1);
#else
  listView->setSortingEnabled(false);
  current=NULL;
#endif
}
void DetectorWidget::resize(const QSize &s)
{resize(s.width(),s.height());}
void DetectorWidget::resizeEvent(QResizeEvent *e)
{resize(e->size().width(),e->size().height());}
void DetectorWidget::resize(int w, int h)
{
  QWidget::resize(w,h);
  int sh=pdisplay->minimumHeight()+40;
  listView->setGeometry(1,1,(int)(w*fraction)-5,h-2);
  line->setGeometry((int)(w*fraction),0,8,h);
  pdisplay->setGeometry((int)(w*fraction)+10,1, (int)(w*(1-fraction))-12,sh);
  canvas->setGeometry((int)(w*fraction)+10,sh+3,(int)(w*(1-fraction))-12,h-sh-5);
}
void DetectorWidget::setDetectors(vector<detector_parameter*> &algoIn)
{
  mode=1;
  ap2=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap2->size();i++)
    {
      if(ap2->at(i)==NULL)continue;
#if QT_VERSION < 0x040000
      new QCheckListItem(listView, ap2->at(i)->getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(listView, QStringList()<<ap2->at(i)->getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
    }
}
void DetectorWidget::setDetectors(vector<detector_parameter> &algoIn)
{
  mode=0;
  ap1=&algoIn;
  listView->clear();
  for(unsigned int i=0;i<ap1->size();i++)
    {
#if QT_VERSION < 0x040000
      new QCheckListItem(listView, ap1->at(i).getName().data(), QCheckListItem::CheckBox);
#else
      (new QTreeWidgetItem(listView, QStringList()<<ap1->at(i).getName().data(),0))->setCheckState(0,Qt::Unchecked);
#endif
    }
}
bool DetectorWidget::readOnly()
{
    return ReadOnly;
}
void DetectorWidget::setReadOnly(bool value)
{
    if(ReadOnly==value)return;
    ReadOnly=value;
    pdisplay->setReadOnly(ReadOnly);
}
void DetectorWidget::mouseMoveEvent ( QMouseEvent * e )
{
    QWidget *ch=childAt ( e->x(), e->y());
    if(ch==line)
    {
	if(e->pos().y()<height()-30&&e->pos().y()>30)
	{
	    fraction=((float)e->pos().x()-2)/ ((float)width());
	}
	e->accept();
	resize(width(),height());
	return;
    }
}
#if QT_VERSION < 0x040000
void DetectorWidget::onDetectorSelect(QListViewItem* it)
#else
void DetectorWidget::onDetectorSelect(QTreeWidgetItem* it,QTreeWidgetItem * prev)
#endif
{
  if(it==NULL)return;
  detector_parameter *dp=getParameterFromItem(it);
  if(dp==NULL)return;
#if QT_VERSION > 0x040000
  current=it;
#endif
  pdisplay->setDetector(*dp);
  resize(width(),height());
}
#if QT_VERSION < 0x040000
detector_parameter* DetectorWidget::getParameterFromItem(QListViewItem*it)
#else
detector_parameter* DetectorWidget::getParameterFromItem(QTreeWidgetItem*it)
#endif
{
  if(it==NULL)return NULL;
  detector_parameter *aps=NULL;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  aps=&(ap1->at(i));
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  aps=ap2->at(i);
    }
  return aps;
}
#if QT_VERSION < 0x040000
QListViewItem* DetectorWidget::getItemFromParameter(detector_parameter*al)
{
  if(al==NULL)return NULL;
  QListViewItem *items;
  items=listView->firstChild();
  QListViewItem *it=0,*ac;
  ac=items->firstChild();
  while(ac!=NULL)
    {
      if(ac->text(0)==al->getName().data())it=ac;
      ac=ac->nextSibling();
    }
  return it;
}
#else
QTreeWidgetItem* DetectorWidget::getItemFromParameter(detector_parameter*al)
{
  if(al==NULL)return NULL;
  QTreeWidgetItem *it=0;
  for(int i=0;i<listView->topLevelItemCount();i++)
    if(listView->topLevelItem(i)->text(0)==al->getName().data())
      it=listView->topLevelItem(i);
  return it;
}
#endif
#if QT_VERSION < 0x040000
int DetectorWidget::getPositionFromItem(QListViewItem*it)
#else
int DetectorWidget::getPositionFromItem(QTreeWidgetItem*it)
#endif
{
  if(it==NULL)return -1;
  if(it->parent()==NULL)return -1;
  int aps=-1;
  if(mode==0)
    {
      for(unsigned int i=0;i<ap1->size();i++)
	if(it->text(0)==ap1->at(i).getName().data())
	  aps=i;
    }
  else if(mode==1)
    {
       for(unsigned int i=0;i<ap2->size();i++)
	if(it->text(0)==ap2->at(i)->getName().data())
	  aps=i;
    }
  return aps;
}
point2D DetectorWidget::pointOnCanvas(const QPoint &p)
{
    double x1,x2,y1,y2;
    canvas->GetCanvas()->GetRange(x1,y1,x2,y2);
    int w=canvas->geometry().width(),h=canvas->geometry().height();
    double xp=x1+((double)(p.x()))* ((double)(x2-x1))/ ((double)w);
    double yp=y2+((double)(p.y()))* ((double)(y1-y2))/ ((double)h);
    return point2D(xp,yp);
}
void DetectorWidget::moveOnCanvas(const QPoint &p)
{
    point2D pp=pointOnCanvas(p);
    string ret="";
    for(unsigned int k=0;k<drawings.size();k++)
      {
	for(unsigned int i=0;i<drawings[k].size();i++)
	  {
	    if(drawings[k][i]==NULL)continue;
	    if(drawings[k][i]->IsA()==TPolyLine::Class())
	      {
		TCutG cut("cut1",((TPolyLine*)drawings[k][i])->GetN(),
			  ((TPolyLine*)drawings[k][i])->GetX(),
			  ((TPolyLine*)drawings[k][i])->GetY());
		if(cut.IsInside(pp.x(),pp.y()))
		  ret=ret+"det: "+string_number(drawPos[k])+", element:"+string_number(i)+"\n";
	      }
	  }
      }
#if QT_VERSION < 0x040000
    QToolTip::remove(canvas);
    QToolTip::add(canvas,ret);
#else
    canvas->setToolTip(ret.data());
#endif
}
extern base_shape* getShape(const shape_parameter& sp);
void DetectorWidget::drawDetectors()
{
  while(!drawings.empty())
    {
      while(!drawings.back().empty())
	{
	  TObject *tmp=drawings.back().back();
	  drawings.back().pop_back();
	  delete tmp;
	}
      drawings.pop_back();
    }
  while(!drawnObjects.empty())
    {
      while(!drawnObjects.back().empty())
	{
	  volumeShape *tmp=drawnObjects.back().back();
	  drawnObjects.back().pop_back();
	  delete tmp;
	}
      drawnObjects.pop_back();
    }
  vector4D bb,bbmax(1e10,1e10,-1e10,-1e10);
  canvas->GetCanvas()->cd()->Clear();
  int ndrawn=0;
#if QT_VERSION < 0x040000
  QListViewItem *it=listView->firstChild();
  while(it!=NULL)
    {
      if(((QCheckListItem*)it)->isOn())
	{
	  ndrawn+=drawDetector(it,bbmax);
	}
      it=it->nextSibling();
    }
#else
  for(int i=0;i<listView->topLevelItemCount();i++)
    {
      if(listView->topLevelItem(i)->checkState(0)==Qt::Checked)
	{
	  ndrawn+=drawDetector(listView->topLevelItem(i),bbmax);
	}
    }
#endif
  if(ndrawn<=0)
    {
      canvas->Refresh();
      return;
    }
  float dx,dy,mx,my;
  dx=bbmax[2]-bbmax[0];
  dy=bbmax[3]-bbmax[1];
  mx=bbmax[0]+dx/2.;
  my=bbmax[1]+dy/2.;
  canvas->GetCanvas()->Range(mx-dx*0.6,my-dy*0.6,mx+dx*.6,my+dy*.6);
  canvas->Refresh();
}
#if QT_VERSION < 0x040000
int DetectorWidget::drawDetector(QListViewItem *it,vector4D &bbmax)
#else
int DetectorWidget::drawDetector(QTreeWidgetItem *it,vector4D &bbmax)
#endif
{
  int nDrawn=0;
  vector4D bb;
  detector_parameter *d=getParameterFromItem(it);
  if(d!=NULL)
    {
      shape_parameter sp=d->getShape();
      volumeShape *sh=(volumeShape*)getShape(sp);
      if(sh!=NULL)
	{
	  drawnObjects.push_back(vector<volumeShape*>());
	  drawings.push_back(vector<TObject*>());
	  drawnObjects.back().push_back(sh);
	  if(hatch==0)
	    {
	      for(int i=1;i<d->getNumberOfElements();i++)
		drawnObjects.back().push_back(sh->getNext(i,d->getStackType()));
	    }
	  else
	    {
	      drawnObjects.back().push_back(sh->getEnvelope(d->getNumberOfElements(),d->getStackType()));
	    }
	  for(unsigned int i=0;i<drawnObjects.back().size();i++)
	    {
	      TObject *tmp;
	      drawnObjects.back().at(i)->Draw(point3D(0,0,0),plane3D(point3D(0,0,1),vector3D(0,0,1)),&bb,&tmp,(i==0?2:1),(i==0?2:1),hatch);
	      if(bb[0]<bbmax[0])bbmax.setValue(0,bb[0]);
	      if(bb[1]<bbmax[1])bbmax.setValue(1,bb[1]);
	      if(bb[2]>bbmax[2])bbmax.setValue(2,bb[2]);
	      if(bb[3]>bbmax[3])bbmax.setValue(3,bb[3]);
	      drawings.back().push_back(tmp);
 	      nDrawn++; 
	    }
	  drawings.back().back()->Draw();
	}
    }
  return nDrawn;
}
void DetectorWidget::setMaterials(const vector<string>& mats)
{
  pdisplay->setMaterials(mats);
}
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
void DetectorWidget::canvasRightClick(const QPoint& p)
{
  QPopupMenu pm;
  pm.insertItem("Set hatch mode",this,SLOT(setHatchMode()));
  pm.insertItem("Save to postscript",this,SLOT(printCanvas()));
  pm.exec(canvas->mapToGlobal(p));
}
#else
#include <QMenu>
void DetectorWidget::canvasRightClick(const QPoint& p)
{
  QMenu pm;
  pm.addAction("Set hatch mode",this,SLOT(setHatchMode()));
  pm.addAction("Save to postscript",this,SLOT(printCanvas()));
  pm.exec(canvas->mapToGlobal(p));
}
#endif

void DetectorWidget::setHatchMode()
{
  QIntegerInputDialog fenst(this);
  fenst.spinBox5->setValue(hatch);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      hatch=fenst.line();
      drawDetectors();
    }
}
extern bool existing(string filename);
#include <qmessagebox.h>
void DetectorWidget::printCanvas()
{
  string s=getSaveFileName(this,"Save canvas as PostScript","","PostScript files (*.ps) ;; any files (*)");
  if(s=="")return;
  int ret=QMessageBox::No;
  while(existing(s)&&ret!=QMessageBox::Yes)
    {
      ret=QMessageBox::warning(this,"File exists",QString("File \"")+s.data()+"\" exists.\nOverwrite?",QMessageBox::Yes|QMessageBox::Default,QMessageBox::No|QMessageBox::Escape);
      if(ret!=QMessageBox::Yes)
	{
	  s=getSaveFileName(this,"Save canvas as PostScript","","PostScript files (*.ps) ;; any files (*)");
	  if(s=="")return;
	}
    }
  canvas->GetCanvas()->Print(s.data());
}
void DetectorWidget::detectorChanged()
{
#if QT_VERSION < 0x040000
  QListViewItem *it=listView->selectedItem();
#else
  QTreeWidgetItem *it=current;//listView->selectedItems().first();
#endif
  if(it==NULL)return;
  if(selected<0)return;
  it->setText(0,(mode==0?ap1->at(selected).getName().data():ap2->at(selected)->getName().data()));
  emit changed();
}
#if QT_VERSION < 0x040000
void DetectorWidget::detectorRightClicked(QListViewItem* it,const QPoint& p) 
{
  if(ReadOnly)return;
  QPopupMenu pm(this);
  pm.insertItem("Add Detector",this,SLOT(addDetector()));
  if(it!=0)
      pm.insertItem("Delete Detector",this,SLOT(deleteDetector()));
  pm.exec(p);
#else
void DetectorWidget::detectorRightClicked(QTreeWidgetItem* it,const QPoint& p) 
{
  if(ReadOnly)return;
  QMenu pm(this);
  pm.addAction("Add Detector",this,SLOT(addDetector()));
  if(it!=0)
      pm.addAction("Delete Detector",this,SLOT(deleteDetector()));
  pm.exec(listView->viewport()->mapToGlobal(p));
#endif
}

void DetectorWidget::deleteDetector()
{
  if(ReadOnly)return;
  if(selected<0)return;
#if QT_VERSION < 0x040000
  QListViewItem *it
#else
  QTreeWidgetItem *it
#endif
    =getItemFromParameter((mode==0?&ap1->at(selected):ap2->at(selected)));
  if(mode==0)
    {
      if(selected>(int)ap1->size()-1)return;
      if(it==NULL)
	{
	  QMessageBox::warning(this,"Detector not Found in ListView",QString("Detector \"")+ap2->at(selected)->getName().data()+"\" not found in List", QMessageBox::Ok,QMessageBox::NoButton);
	  return;
	}
      vector<detector_parameter> tmp;
      while((int)ap1->size()>selected+1)
	{
	  tmp.push_back(ap1->back());
	  ap1->pop_back();
	}
      ap1->pop_back();
      while(!tmp.empty())
	{
	  ap1->push_back(tmp.back());
	  tmp.pop_back();
	}
    }
  else
    {
      if(selected>(int)ap2->size()-1)return;
      if(it==NULL)
	{
	  QMessageBox::warning(this,"Detector not Found in ListView",QString("Detector \"")+ap2->at(selected)->getName().data()+"\" not found in List", QMessageBox::Ok,QMessageBox::NoButton);
	  return;
	}
      vector<detector_parameter*> tmp;
      while((int)ap2->size()>selected+1)
	{
	  tmp.push_back(ap2->back());
	  ap2->pop_back();
	}
      ap2->pop_back();
      while(!tmp.empty())
	{
	  ap2->push_back(tmp.back());
	  tmp.pop_back();
	}
    }
#if QT_VERSION < 0x040000
  listView->takeItem(it);
  delete it;
#else
  listView->takeTopLevelItem(listView->indexOfTopLevelItem(it));
  delete it;
#endif
}
void DetectorWidget::addDetector()
{
  if(ReadOnly)return;
  QStringInputDialog fenst(this);
#if QT_VERSION < 0x040000
  fenst.setCaption("New Detector");
#else
  fenst.setWindowTitle("New Detector");
#endif
  fenst.setStart("new detector");
  fenst.exec();
  if(fenst.result()!=QDialog::Accepted)return;
#if QT_VERSION < 0x040000
  new QCheckListItem(listView,fenst.line(),QCheckListItem::CheckBox);
#else
  current=new QTreeWidgetItem(listView,QStringList()<<fenst.line(),0);current->setCheckState(0,Qt::Unchecked);
#endif
  if(mode==0)
    {
      ap1->push_back(detector_parameter());
      ap1->back().setName(QS2s(fenst.line()));
      pdisplay->setDetector(ap1->back());
    }
  else
    {
      ap2->push_back(new detector_parameter());
      ap2->back()->setName(QS2s(fenst.line()));
      pdisplay->setDetector(*ap2->back());
    }
}
void DetectorWidget::setDefinedShapes(vector<shape_parameter> shp)
{pdisplay->setDefinedShapes(shp);}
