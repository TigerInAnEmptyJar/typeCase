void ViewSelectionWidget::init()
{
  canvas=NULL;
  event=NULL;
  startTime=NULL;
  selx=-1;sely=-1;
}
void ViewSelectionWidget::destroy(){}
void ViewSelectionWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void ViewSelectionWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void ViewSelectionWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
}
void ViewSelectionWidget::setPads(vector<pad_parameter> &p,int nxIn, int nyIn)
{
  pp=&p;
  if(buttons!=NULL)
    {
      for(int i=0;i<nx;i++)
	{for(int j=0;j<ny;j++)
	  {
#if QT_VERSION < 0x040000
	    buttonLayout->remove(buttons[i][j]);
#else
	    buttonLayout->removeWidget(buttons[i][j]);
#endif
	    delete buttons[i][j];
	  }
	delete []buttons[i];
	}
      delete []buttons;
    }
  nx=nxIn;
  ny=nyIn;
  buttons=new QPushButton**[nx];
  for(int i=0;i<nx;i++){buttons[i]=new QPushButton*[ny];
  for(int j=0;j<ny;j++){
    buttons[i][j]=new QPushButton(this);//,"button");
    buttons[i][j]->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
    connect(buttons[i][j],SIGNAL(clicked()),this,SLOT(onSelectedChanged()));
    buttonLayout->addWidget(buttons[i][j],i,j);
  }}
  resize(width(),height());
  while((int)pp->size()<nx*ny)
    pp->push_back(pad_parameter());
  selx=-1;sely=-1;
}
#include "dividedialog.h"
void ViewSelectionWidget::divideCanvas()
{
  if(canvas==NULL)return;
  DivideDialog fenst(this);
  fenst.setNumbers(ny,nx);
  fenst.exec();
  if(fenst.result()==QDialog::Accepted)
    {
      int mx=nx,my=ny;
      pad_parameter tmp[fenst.getY()][fenst.getX()];
      if(mx>fenst.getY())mx=fenst.getY();
      if(my>fenst.getX())my=fenst.getX();
      for(int j=0;j<my;j++)
	for(int i=0;i<mx;i++)
	  tmp[i][j]=pp->at(j*nx+i);
      while(!pp->empty())pp->pop_back();
      mx=fenst.getY();
      my=fenst.getX();
      canvas->Clear();
      canvas->Divide(my,mx);
      for(int j=0;j<my;j++)
	for(int i=0;i<mx;i++)
	  {
	    pp->push_back(tmp[i][j]);
	    pp->back().drawPad=canvas->GetPad(pp->size());
	  }
      setPads(*pp,mx,my);
    }
}
void ViewSelectionWidget::setCanvas(TCanvas *can){canvas=can;}
void ViewSelectionWidget::setEvent(TEvent *e){event=e;}
void ViewSelectionWidget::setAnalysisParameter(int nE2a, int nE, QDateTime* startT)
{
  startTime=startT;
  nevents=nE;
  nevents2A=nE2a;
}
void ViewSelectionWidget::onPaintChanged()
{
}
void ViewSelectionWidget::onSelectedChanged()
{
  if(buttons==NULL)return;
  if(pp==NULL)return;
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    buttons[selx][sely]->setDown(false);
  for(int i=0;i<nx;i++)
    for(int j=0;j<ny;j++)
#if QT_VERSION < 0x040000
      if(buttons[i][j]->hasMouse())
#else
      if(buttons[i][j]->underMouse())
#endif
	{sely=j;selx=i;}
  if(!(selx>=0&&selx<nx&&sely>=0&&sely<ny))return;
  buttons[selx][sely]->setDown(true);
  pad_parameter *par=&pp->at(selx*ny+sely);
  paints->setPaints(par->track,par->pixel,par->hits);
#if QT_VERSION < 0x040000
  displayType->setButton(par->paintMode);
#else
  ((QRadioButton*)displayType->button(par->paintMode))->setChecked(true);
#endif
  switch(par->paintMode)
    {
    case 0:
    case 1:
    case 2:
#if QT_VERSION < 0x040000
    case 3: widgets->raiseWidget(0);break;
    case 5: widgets->raiseWidget(1);break;
    case 4: widgets->raiseWidget(2);break;
#else
    case 3: widgets->setCurrentWidget(paints);break;
    case 5: widgets->setCurrentWidget(histoWidget);break;
    case 4: widgets->setCurrentWidget(treeWidget);break;
#endif
    }
  par->draw();
}

void ViewSelectionWidget::onDisplayTypeChange(int num)
{
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      if(par->paintMode==num)return;
      par->paintMode=num;
      par->draw();
      switch(num)
	{
	case 0:
	case 1:
	case 2:
#if QT_VERSION < 0x040000
	case 3: widgets->raiseWidget(0);break;
	case 5: widgets->raiseWidget(1);break;
	case 4: widgets->raiseWidget(2);break;
#else
	case 3: widgets->setCurrentWidget(paints);break;
	case 5: widgets->setCurrentWidget(histoWidget);break;
	case 4: widgets->setCurrentWidget(treeWidget);break;
#endif
	}
    }
}
void ViewSelectionWidget::histoChanged(const QString &text)
{
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      if(analyserPointer!=NULL)
	par->histoPointer=analyserPointer->getHisto(QS2s(text));
      par->draw();
    }
}
void ViewSelectionWidget::optionChanged(const QString &text)
{
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      par->drawOption=QS2s(text);
      par->draw();
    }
}
void ViewSelectionWidget::treeChanged(const QString &text)
{
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      par->drawName=QS2s(text);
      par->draw();
    }
}
void ViewSelectionWidget::treeCutChanged(const QString &text)
{
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      par->drawSelection=QS2s(text);
      par->draw();
    }
}
#if QT_VERSION < 0x040000
void ViewSelectionWidget::treeChanged(QListViewItem *it)
#else
void ViewSelectionWidget::treeChanged(QTreeWidgetItem *it)
#endif
{
  if(it==NULL)return;
  if(it->parent()==NULL)return;
  while(it->parent()->parent()!=NULL)it=it->parent();
  string text=QS2s(it->text(0));
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      if(analyserPointer!=NULL)
	par->treePointer=analyserPointer->getTree(text);
      par->draw();
    }
}
#if QT_VERSION < 0x040000
void ViewSelectionWidget::histoChanged(QListBoxItem *it)
#else
void ViewSelectionWidget::histoChanged(QListWidgetItem *it)
#endif
{
  if(it==NULL)return;
  string text=QS2s(it->text());
  if(selx>=0&&selx<nx&&sely>=0&&sely<ny)
    {
      pad_parameter *par=&pp->at(selx*ny+sely);
      if(analyserPointer!=NULL)
	par->histoPointer=analyserPointer->getHisto(text);
      par->draw();
    }
}
void ViewSelectionWidget::analyserInitialized(tofAnalysis* pointer)
{
  analyserPointer=pointer;
  if(analyserPointer==NULL)return;
  treeList->clear();
  histoList->clear();
  vector<vector<string> > histoNames=analyserPointer->getHistogramNames();
  vector<vector<string> > treeNames=analyserPointer->getTreeNames();
  for(unsigned int i=0;i<treeNames.size();i++)
    {
      if(treeNames[i].size()<2)continue;
#if QT_VERSION < 0x040000
      QListViewItem *it=new QListViewItem(treeList,treeNames[i][0].data());
#else
      QTreeWidgetItem *it=new QTreeWidgetItem(treeList,QStringList()<<treeNames[i][0].data());
#endif
      for(unsigned int j=0;j<treeNames.size();j++)
	{
#if QT_VERSION < 0x040000
	  /*QListViewItem *ac=*/new QListViewItem(it,treeNames[i][0].data());
#else
	  /*QTreeWidgetItem *ac=*/new QTreeWidgetItem(it,QStringList()<<treeNames[i][0].data());
#endif

	}
    }
  for(unsigned int i=0;i<histoNames.size();i++)
    {
      if(histoNames[i].size()<2)continue;
      for(unsigned int j=1;j<treeNames.size();j++)
	{
#if QT_VERSION < 0x040000
	  histoList->insertItem(treeNames[i][0].data());
#else
	  histoList->addItem(treeNames[i][0].data());
#endif
	}
    }
}
