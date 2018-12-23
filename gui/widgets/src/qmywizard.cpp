#include "qmywizard.h"

#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#if QT_VERSION > 0x040000
#include <QDialogButtonBox>
#include <QStackedWidget>
#else
#include <qwidgetstack.h>
#endif
#include <qlineedit.h>
#include <iostream>
#include <qtooltip.h>
#if QT_VERSION < 0x040000
QMyWizard::QMyWizard( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "QMyWizard" );
#else
QMyWizard::QMyWizard( QWidget* parent,Qt::WindowFlags f )
    : QDialog( parent, f )
{
	setObjectName( "QMyWizard" );
#endif
    setSizeGripEnabled( TRUE );
    pageTitleLabel=new QLabel(this);
    separator=new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Raised);
    separator->setLineWidth(2);
#if QT_VERSION < 0x040000
    thePages=new QWidgetStack(this);
    buttonWidget=new QWidget(this);
    buttonlayout=new QGridLayout(buttonWidget);
    buttonHelp = new QPushButton( buttonWidget);
    buttonCancel = new QPushButton( buttonWidget);
    buttonBack= new QPushButton( buttonWidget);
    buttonNext= new QPushButton( buttonWidget);
    buttonFinish= new QPushButton( buttonWidget);
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    buttonlayout->addWidget( buttonHelp ,0,0);
    buttonlayout->addItem( Horizontal_Spacing2 ,0,1);
    buttonlayout->addWidget( buttonBack ,0,2);
    buttonlayout->addWidget( buttonNext ,0,3);
    buttonlayout->addWidget( buttonFinish ,0,4);
    buttonlayout->addWidget( buttonCancel ,0,5);
    buttonlayout->setMargin(5);
    buttonlayout->setSpacing(5);
#else
    thePages=new QStackedWidget(this);
    buttons=new QDialogButtonBox(this);
    buttonHelp = new QPushButton(buttons);//
    buttonCancel = new QPushButton(buttons);//(QPushButton*)buttons->buttons().at(3);
    buttonBack= new QPushButton(buttons);//(QPushButton*)buttons->buttons().at(2);
    buttonNext= new QPushButton(buttons);//(QPushButton*)buttons->buttons().at(1);
    buttonFinish= new QPushButton(buttons);//(QPushButton*)buttons->buttons().at(0);
    buttons->addButton(buttonHelp,QDialogButtonBox::HelpRole);
    buttons->addButton(buttonBack,QDialogButtonBox::ActionRole);
    buttons->addButton(buttonNext,QDialogButtonBox::ActionRole);
    buttons->addButton(buttonFinish,QDialogButtonBox::AcceptRole);
    buttons->addButton(buttonCancel,QDialogButtonBox::RejectRole);
#endif

    layout=new QGridLayout(this);
    layout->addWidget(pageTitleLabel,0,0);
    layout->addWidget(separator,1,0);
    layout->addWidget(thePages,2,0);
#if QT_VERSION < 0x040000
    layout->addWidget(buttonWidget,3,0);
#else
    layout->addWidget(buttons,3,0);
    setLayout(layout);
#endif
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->setRowStretch(2,10);

    languageChange();
    resize( QSize(325, 99).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    current=0;
    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( buttonBack, SIGNAL( clicked() ), this, SLOT( back() ) );
    connect( buttonNext, SIGNAL( clicked() ), this, SLOT( next() ) );
    connect( buttonFinish, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonHelp,SIGNAL(clicked()),this,SIGNAL(helpRequested()));
#else
    connect(buttons,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(reject()));
    connect(buttons,SIGNAL(helpRequested()),this,SIGNAL(helpClicked()));
    connect(buttons,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));
#endif
}
QMyWizard::~QMyWizard()
{
}

void QMyWizard::addPage ( QWidget * page, const QString & title ) 
{
  if(page==NULL)return;
#if QT_VERSION < 0x040000
  thePages->addWidget(page,(int)titles.size());
#else
  thePages->addWidget(page);
#endif
  titles.push_back(title);
  approp.push_back(true);
  hasBack.push_back(true);
  hasNext.push_back(true);
  hasHelp.push_back(true);
  hasFinish.push_back(false);
//   if(titles.size()==1)showPage(0);
}
void QMyWizard::insertPage ( QWidget * page, const QString & title, int index ) 
{
  if(index>=(int)titles.size()){addPage(page,title);return;}
#if QT_VERSION < 0x040000
  vector<QWidget*> tmpW;
#else
  thePages->insertWidget(index,page);
#endif
  vector<bool> tmp1,tmp2,tmp3,tmp4,tmp5;
  vector<QString> tmps;
  if(current>index)current++;
  while((int)titles.size()>=index)
    {
#if QT_VERSION < 0x040000
      tmpW.push_back(thePages->widget(titles.size()-1));
      thePages->removeWidget(tmpW.back());
#endif
      tmps.push_back(titles.back());
      tmp1.push_back(approp.back());
      tmp2.push_back(hasNext.back());
      tmp3.push_back(hasBack.back());
      tmp4.push_back(hasHelp.back());
      tmp5.push_back(hasFinish.back());
      approp.pop_back();
      hasNext.pop_back();
      hasBack.pop_back();
      hasHelp.pop_back();
      hasFinish.pop_back();
      titles.pop_back();
    }
#if QT_VERSION < 0x040000
  thePages->addWidget(page,titles.size());
#endif
  approp.push_back(true);
  hasNext.push_back(true);
  hasBack.push_back(true);
  hasHelp.push_back(true);
  hasFinish.push_back(false);
  titles.push_back(title);
  while(!tmps.empty())
    {
#if QT_VERSION < 0x040000
      thePages->addWidget(tmpW.back());
      tmpW.pop_back();
#endif
      approp.push_back(tmp1.back());
      hasNext.push_back(tmp2.back());
      hasBack.push_back(tmp3.back());
      hasHelp.push_back(tmp4.back());
      hasFinish.push_back(tmp5.back());
      titles.push_back(tmps.back());
      tmps.pop_back();
      tmp1.pop_back();
      tmp2.pop_back();
      tmp3.pop_back();
      tmp4.pop_back();
      tmp5.pop_back();
    }
}
void QMyWizard::removePage ( QWidget * page ) 
{
#if QT_VERSION < 0x040000
  int ind=indexOf(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
#if QT_VERSION < 0x040000
  if(thePages->visibleWidget()==page)
#else
  if(thePages->currentWidget()==page)
#endif
    {
      if(ind>0)	current=ind-1;
      else current=ind+1;
 #if QT_VERSION < 0x040000
     thePages->raiseWidget(current);
#else
     thePages->setCurrentIndex(current);
#endif
    }
  thePages->removeWidget(page);
  vector<bool> tmp1,tmp2,tmp3,tmp4,tmp5;
  vector<QString> tmps;
  while((int)titles.size()>=ind)
    {
      tmps.push_back(titles.back());
      tmp1.push_back(approp.back());
      tmp2.push_back(hasNext.back());
      tmp3.push_back(hasBack.back());
      tmp4.push_back(hasHelp.back());
      tmp5.push_back(hasFinish.back());
      approp.pop_back();
      hasNext.pop_back();
      hasBack.pop_back();
      hasHelp.pop_back();
      hasFinish.pop_back();
      titles.pop_back();
    }
  approp.pop_back();
  hasNext.pop_back();
  hasBack.pop_back();
  hasHelp.pop_back();
  hasFinish.pop_back();
  titles.pop_back();
  while(!tmps.empty())
    {
      approp.push_back(tmp1.back());
      hasNext.push_back(tmp2.back());
      hasBack.push_back(tmp3.back());
      hasHelp.push_back(tmp4.back());
      hasFinish.push_back(tmp5.back());
      titles.push_back(tmps.back());
      tmps.pop_back();
      tmp1.pop_back();
      tmp2.pop_back();
      tmp3.pop_back();
      tmp4.pop_back();
      tmp5.pop_back();
    }
}
QString QMyWizard::title ( QWidget * page ) const 
{
  for(unsigned int i=0;i<titles.size();i++)
    if(thePages->widget(i)==page)return titles[i];
  return "";
}
void QMyWizard::setTitle ( QWidget * page, const QString & title ) 
{
  for(unsigned int i=0;i<titles.size();i++)
    if(thePages->widget(i)==page)titles[i]=title;
}
void QMyWizard::showPage ( QWidget * page ) 
{
  for(unsigned int i=0;i<titles.size();i++)
    if(thePages->widget(i)==page)
      {
	showPageIndex(i);
	return;
      }
}
void QMyWizard::showPageIndex ( int index ) 
{
  if(index<0||index>=(int)titles.size())return;
  current=index;
#if QT_VERSION < 0x040000
  thePages->raiseWidget(index);
  buttonBack->setEnabled(index>0&&hasBack[index]);
  buttonNext->setEnabled(hasNext[index]&&index<(int)titles.size()-1);
  cout<<index<<" "<<hasFinish[index]<<endl;
  buttonFinish->setEnabled(hasFinish[index]);
#else
  thePages->setCurrentIndex(index);
  buttons->removeButton(buttonBack);
  buttons->removeButton(buttonNext);
  buttons->removeButton(buttonHelp);
  buttons->removeButton(buttonFinish);
  buttons->removeButton(buttonCancel);
  buttons->addButton(buttonHelp,QDialogButtonBox::HelpRole);
  if(index>0&&hasBack[index])
    buttons->addButton(buttonBack,QDialogButtonBox::ActionRole);
  if(hasNext[index]&&index<(int)titles.size()-1)
    buttons->addButton(buttonNext,QDialogButtonBox::ActionRole);
  if(hasFinish[index])
    buttons->addButton(buttonFinish,QDialogButtonBox::AcceptRole);
  buttons->addButton(buttonCancel,QDialogButtonBox::RejectRole);
#endif
  pageTitleLabel->setText(titles[index]);
  emit selected(titles[index]);
}
QWidget * QMyWizard::currentPage () const 
{
#if QT_VERSION < 0x040000
  return thePages->visibleWidget();
#else
  return thePages->currentWidget();
#endif
}
QWidget * QMyWizard::page ( int index ) const 
{
  return thePages->widget(index);
}
int QMyWizard::pageCount () const 
{
  return titles.size();
}
int QMyWizard::indexOf ( QWidget * page ) const 
{
#if QT_VERSION < 0x040000
  return thePages->id(page);
#else
  return thePages->indexOf(page);
#endif
}
bool QMyWizard::appropriate ( QWidget * page ) const 
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return false;
  return approp[ind];
}
void QMyWizard::setAppropriate ( QWidget * page, bool appropriate ) 
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
  approp[ind]=appropriate;
}
void QMyWizard::languageChange()
{
  buttonBack->setText(tr("Back"));
  buttonNext->setText(tr("Next"));
  buttonHelp->setText(tr("Help"));
  buttonCancel->setText(tr("Cancel"));
  buttonFinish->setText(tr("Finish"));
}

#if QT_VERSION > 0x040000
void QMyWizard::buttonClicked(QAbstractButton* but)
{
  if(but==buttonHelp)help();
  else if(but==buttonNext)next();
  else if(but==buttonBack)back();
  else if(but==buttonCancel)cancel();
  else if(but==buttonFinish)finish();
}
#endif
void QMyWizard::next()
{
  if(current>=(int)titles.size()-1)
    return;
  int nextPage=current+1;
  while(!approp[nextPage]&&nextPage<(int)titles.size()-1)nextPage++;
  buttonNext->setEnabled(approp[nextPage]);
  if(!approp[nextPage])
    return;
  showPageIndex(nextPage);
}
void QMyWizard::back()
{
  if(current==0)return;
  int nextPage=current-1;
  while(!approp[nextPage]&&nextPage>0)nextPage--;
  buttonBack->setEnabled(approp[nextPage]);
  if(!approp[nextPage])
    return;
  showPageIndex(nextPage);
}
void QMyWizard::finish()
{
  QDialog::accept();
}
void QMyWizard::help()
{
  emit helpClicked();
}
void QMyWizard::cancel()
{
  QDialog::reject();
  close();
}

void QMyWizard::setBackEnabled ( QWidget * page, bool enable ) 
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
  hasBack[ind]=enable;
}
void QMyWizard::setNextEnabled ( QWidget * page, bool enable )
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
  hasNext[ind]=enable;
}
void QMyWizard::setFinishEnabled ( QWidget * page, bool enable ) 
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
#if QT_VERSION < 0x040000
  if(current==ind&&enable!=hasFinish[ind])
    buttonFinish->setEnabled(enable);
#else
  if(current==ind&&enable!=hasFinish[ind])
    {
      hasFinish[ind]=enable;
      showPageIndex(current);
    }
#endif
  hasFinish[ind]=enable;
}
void QMyWizard::setHelpEnabled ( QWidget * page, bool enable ) 
{
#if QT_VERSION < 0x040000
  int ind=thePages->id(page);
#else
  int ind=thePages->indexOf(page);
#endif
  if(ind<0||ind>(int)titles.size())return;
  hasHelp[ind]=enable;
}


