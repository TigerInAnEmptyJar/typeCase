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
#include "string_utilities.h"
void MaterialDisplay::init()
{
    setMinimumHeight(40+120);
    setMinimumWidth(400);
#if QT_VERSION < 0x040000
    listView1->setSorting(-1);
#endif
    if(!Eparticles::IsInit())Eparticles::init();
#if QT_VERSION < 0x040000
    for(int i=0;i<Eparticles::getNumberOfParticles();i++)
      elementMenu.insertItem(Eparticles::getName(i).data(),this,SLOT(OnElementInsert(int)),0,i,i);
    for(int i=0;i<Eparticles::getNumberOfParticles();i++)
      elementSet.insertItem(Eparticles::getName(i).data(),this,SLOT(OnElementSet(int)),0,i,i);
#else
    elementMenu.setTitle("Add element");
    for(int i=0;i<Eparticles::getNumberOfParticles();i++)
      elementMenu.addAction(Eparticles::getName(i).data(),this,SLOT(OnElementInsert()))->setStatusTip(QString::number(i));
    elementSet.setTitle("Set element");
    for(int i=0;i<Eparticles::getNumberOfParticles();i++)
      elementSet.addAction(Eparticles::getName(i).data(),this,SLOT(OnElementSet()))->setStatusTip(QString::number(i));
#endif
    connect(lineEdit1,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(lineEdit2,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(lineEdit3,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(checkBox1,SIGNAL(toggled(bool)),this,SLOT(onPropertyChange()));
}
void MaterialDisplay::destroy()
{
}
void MaterialDisplay::resizeEvent(QResizeEvent *e)
{
    resize(e->size().width(),e->size().height());
}
void MaterialDisplay::resize(const QSize &s)
{
    resize(s.width(),s.height());
}
void MaterialDisplay::resize(int w, int h)
{
    QWidget::resize(w,h);
/*     frame1->resize((w-15)/2,105); */
/*     lineEdit1->resize((w-15)/2-85,20); */
/*     lineEdit2->resize((w-15)/2-115,20); */
/*     lineEdit3->resize((w-15)/2-115,20); */
/*     listView1->setGeometry((w-15)/2+10,5,(w-15)/2-11,h-10); */
/*     textEdit1->resize((w-15)/2,h-115); */
}
void MaterialDisplay::onPropertyChange()
{
  if(focusWidget()==lineEdit1)
    {
      copy.setName(string(lineEdit1->text().
#if QT_VERSION < 0x040000
			  ascii()));
#else
      toAscii()));
#endif
      if(!ReadOnly)
	{
	  mp->setName(copy.getName());
	  emit changed();
	}
    }
  else if(focusWidget()==lineEdit2)
    {
      copy.setSpeed(lineEdit2->text().toFloat());
      if(!ReadOnly)
	{
	  mp->setSpeed(copy.Speed());
	  emit changed();
	}
    }
  else if(focusWidget()==lineEdit3)
    {
      copy.setDensity(lineEdit3->text().toFloat());
      if(!ReadOnly)
	{
	  mp->setDensity(copy.Density());
	  emit changed();
	}
    }
  else if(focusWidget()==checkBox1)
    {
      copy.setActive(checkBox1->isChecked());
      if(!ReadOnly)
	{
	  mp->setActive(copy.IsActive());
	  emit changed();
	}
    }
  else
    {
    }
}
void MaterialDisplay::setMaterial(material_parameter &material)
{
  mp=&material;
  copy=material;
  lineEdit1->setText(copy.getName().data());
  lineEdit2->setText(string_number(copy.Speed()).data());
  lineEdit3->setText(string_number(copy.Density()).data());
  checkBox1->setChecked(copy.IsActive());
  textEdit1->clear();
  vector<string> li=copy.getDescription();
  for(unsigned int i=0;i<li.size();i++)textEdit1->append(li[i].data());
  listView1->clear();
#if QT_VERSION < 0x040000
  for(int i=copy.NumberOfElements()-1;i>=0;i--)
     new QListViewItem(listView1,copy.Element(i).getName().data(),
       string_number(copy.Element(i).getMass()).data(),
       string_number(copy.Element(i).getCharge()).data(),
       string_number(copy.Element(i).getWeight()).data());
#else
     for(int i=copy.NumberOfElements()-1;i>=0;i--)
{
  QStringList tmp;tmp<<copy.Element(i).getName().data()<<string_number(copy.Element(i).getMass()).data()<<string_number(copy.Element(i).getCharge()).data()<<string_number(copy.Element(i).getWeight()).data();
  new QTreeWidgetItem(listView1,tmp);
}
#endif
}
void MaterialDisplay::setMode(int m)
{
    mode=m;
    setReadOnly(mode&1);
}
#if QT_VERSION < 0x040000
void MaterialDisplay::OnElementsRightClick(QListViewItem *it, const QPoint &p)
{
#else
void MaterialDisplay::OnElementsRightClick(const QPoint &p)
{
    QTreeWidgetItem *it=listView1->itemAt(p);
#endif
    if(ReadOnly)return;
#if QT_VERSION < 0x040000
    QPopupMenu popup(this);
    if(it!=NULL)
    {
	popup.insertItem("Remove Item",this,SLOT(OnElementRemove()));
	popup.insertItem("Set element",&elementSet);
	popup.insertItem("Set weight",this,SLOT(OnSetElementWeight()));
	popup.insertItem("Custom element",this,SLOT(OnCustom()));
    }
    popup.insertItem("Add element",&elementMenu);
    popup.insertItem("Add custom element",this,SLOT(OnCustom()));
    popup.exec(p);
#else
    QMenu popup(this);
    if(it!=NULL)
    {
	popup.addAction("Remove Item",this,SLOT(OnElementRemove()));
	popup.addMenu(&elementSet);
	popup.addAction("Set weight",this,SLOT(OnSetElementWeight()));
	popup.addAction("Custom element",this,SLOT(OnCustom()));
    }
    popup.addMenu(&elementMenu);
    popup.addAction("Add custom element",this,SLOT(OnCustom()));
    popup.exec(listView1->viewport()->mapToGlobal(p));
#endif
}
#if QT_VERSION > 0x040000
void MaterialDisplay::OnElementInsert()
{
  QAction *ac=elementMenu.activeAction();
  if(ac==NULL)return;
  int num=-1;
  num=ac->statusTip().toInt();
  if(num==-1)return;
  OnElementInsert(num);
}
#endif
void MaterialDisplay::OnElementInsert(int id)
{
#if QT_VERSION < 0x040000
    new QListViewItem(listView1,
		    Eparticles::getName(id).data(),
		    string_number(Eparticles::getMass(id)).data(),
		    string_number(Eparticles::getCharge(id)).data(),"-1");
#else
    QStringList tmpss;tmpss<<Eparticles::getName(id).data()
			   <<string_number(Eparticles::getMass(id)).data()
			   <<string_number(Eparticles::getCharge(id)).data()<<"-1";
    new QTreeWidgetItem(listView1,tmpss);
#endif
  element_parameter tmp(Eparticles::getMass(id),Eparticles::getCharge(id),-1,Eparticles::getName(id));
  copy.addElement(tmp);
  if(!ReadOnly)
    mp->addElement(tmp);
}
#if QT_VERSION > 0x040000
void MaterialDisplay::OnElementSet()
{
  QAction *ac=elementMenu.activeAction();
  if(ac==NULL)return;
  int num=ac->statusTip().toInt();
  if(num==-1)return;
  OnElementSet(num);
}
#endif
void MaterialDisplay::OnElementSet(int id)
{
#if QT_VERSION < 0x040000
    QListViewItem *it=listView1->selectedItem();
#else
    QTreeWidgetItem *it=listView1->currentItem();
#endif
    if(it==NULL)return;
    it->setText(0,Eparticles::getName(id).data());
    it->setText(1,string_number(Eparticles::getMass(id)).data());
    it->setText(2,string_number(Eparticles::getCharge(id)).data());
}
void MaterialDisplay::OnElementRemove()
{
#if QT_VERSION < 0x040000
    QListViewItem *it=listView1->selectedItem();
#else
    QTreeWidgetItem *it=listView1->currentItem();
#endif
    if(it==NULL)return;
    element_parameter tmp(it->text(1).toFloat(),it->text(2).toFloat(),-1,string(it->text(0).
#if QT_VERSION < 0x040000
ascii()));
   listView1->takeItem(it);
#else
toAscii()));
   listView1->takeTopLevelItem(listView1->indexOfTopLevelItem(it));
#endif
    delete it;
    int pos=-1;
    for(int i=0;i<copy.NumberOfElements();i++)
      if(copy.Element(i)==tmp)pos=i;
    if(pos<0){
      cout<<"element not found!!!"<<endl;
      return;
    }
    copy.deleteElement(pos);
    if(!ReadOnly)mp->deleteElement(pos);
}
void MaterialDisplay::OnSetElementWeight()
{
#if QT_VERSION < 0x040000
    QListViewItem *it=listView1->selectedItem();
#else
    QTreeWidgetItem *it=listView1->currentItem();
#endif
    if(it==NULL)return;
    element_parameter tmp(it->text(1).toFloat(),it->text(2).toFloat(),-1,string(it->text(0).
#if QT_VERSION < 0x040000
    ascii()));
#else
    toAscii()));
#endif
    int pos=-1;
    for(int i=0;i<copy.NumberOfElements();i++)
      if(copy.Element(i)==tmp)pos=i;
    if(pos<0){
      cout<<"element not found!!!"<<endl;
      return;
    }
    float tmpf=QNumberInputDialog::getLine(this,"Set element weight",it->text(3).toInt());
     if(tmpf==-10000)return;
     it->setText(3,string_number(tmpf).data());
	tmp=copy.Element(pos);
	tmp.setWeight(tmpf);
	copy.setElement(pos,tmp);
	if(!ReadOnly)
	  mp->setElement(pos,tmp);
}
#include "qsinglewidgetdialog.h"
void MaterialDisplay::OnCustom()
{
  element_parameter tmp(1,1,-1,"new element");
  int pos=-1;
#if QT_VERSION < 0x040000
    QListViewItem *it=listView1->selectedItem();
#else
    QTreeWidgetItem *it=listView1->currentItem();
#endif
  if(it!=NULL) 
    { 
#if QT_VERSION < 0x040000
      tmp.setName(string(it->text(0).ascii())); 
#else
      tmp.setName(string(it->text(0).toAscii())); 
#endif
      tmp.setMass(it->text(1).toFloat()); 
      tmp.setCharge(it->text(2).toFloat()); 
      tmp.setWeight(it->text(3).toFloat()); 
      for(int i=0;i<copy.NumberOfElements();i++)
	if(copy.Element(i)==tmp)pos=i;
    } 
  QSingleWidgetDialog fenst(this);
  fenst.resize(201,23*4+36);
  ElementDisplay ed(&fenst);
  fenst.setWidget(&ed);
  ed.setReadOnly(false);
  ed.setElement(tmp);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      if(it==NULL)
	{
#if QT_VERSION < 0x040000
	  new QListViewItem(listView1,tmp.getName().data(),string_number(tmp.getMass()).data(),
			    string_number(tmp.getCharge()).data(),string_number(tmp.getWeight()).data());
#else
QStringList tmpps;tmpps<<tmp.getName().data()
     <<string_number(tmp.getMass()).data()
     <<string_number(tmp.getCharge()).data()
     <<string_number(tmp.getWeight()).data();
     new QTreeWidgetItem(listView1,tmpps);
#endif
	  copy.addElement(tmp);
	  if(!ReadOnly)mp->addElement(tmp);
	}
      else
	{
	  it->setText(0,tmp.getName().data());
	  it->setText(1,string_number(tmp.getMass()).data());
	  it->setText(2,string_number(tmp.getCharge()).data());
	  it->setText(3,string_number(tmp.getWeight()).data());
	  if(pos<0)
	    {
	      copy.addElement(tmp);
	      if(!ReadOnly)mp->addElement(tmp);
	    }
	  else
	    {
	      copy.setElement(pos,tmp);
	      if(!ReadOnly)mp->setElement(pos,tmp);
	    }
	}
    }
}
void MaterialDisplay::setReadOnly(bool value)
{
  if(value==ReadOnly)return;
  ReadOnly=value;
  lineEdit1->setReadOnly(ReadOnly);
  lineEdit2->setReadOnly(ReadOnly);
  lineEdit3->setReadOnly(ReadOnly);
  textEdit1->setReadOnly(ReadOnly);
  checkBox1->setEnabled(!ReadOnly);
}
bool MaterialDisplay::readOnly()const
{return ReadOnly;}
