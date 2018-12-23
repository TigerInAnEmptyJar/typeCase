#include "controlwidget.h"
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "utilities.h"
#include <qframe.h>
#include <qmessagebox.h>
#if QT_VERSION < 0x040000
#include <qlayout.h>
#else
#include <QGridLayout>
#endif

#if QT_VERSION < 0x040000
controlWidget::controlWidget( QWidget* parent, const char* name, WFlags fl ):QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Cutscontrol" );
#else
controlWidget::controlWidget( QWidget* parent, Qt::WindowFlags f):QWidget( parent, f )
{
    setObjectName( "Cutscontrol" );
#endif

    browseButton=new QPushButton(this);
    runLabel=new QLabel(this);
    runInput=new QLineEdit(this);
    runInput->setInputMask("000000009");
    beamLabel=new QLabel(this);
    beamInput=new QLineEdit(this);
    authorLabel=new QLabel(this);
    authorInput=new QLineEdit(this);
    fillAllButton=new QPushButton(this);
    fitAllButton=new QPushButton(this);
    checkAllButton=new QPushButton(this);
    viewButton =new QPushButton(this);
    writeButton =new QPushButton(this);

    fillBox=new QFrame(this);
    fitBox=new QFrame(this);
    checkBox=new QFrame(this);
    fillBox->setFrameStyle(QFrame::GroupBoxPanel|QFrame::Raised);
    fillBox->setLineWidth(1);fillBox->setMidLineWidth(1);
    fitBox->setFrameStyle(QFrame::GroupBoxPanel|QFrame::Raised);
    fitBox->setLineWidth(1);fitBox->setMidLineWidth(1);
    checkBox->setFrameStyle(QFrame::GroupBoxPanel|QFrame::Raised);
    checkBox->setLineWidth(1);checkBox->setMidLineWidth(1);
    checkLabel=new QLabel(checkBox);
    fillLabel=new QLabel(fillBox);
    fitLabel=new QLabel(fitBox);

    fillButtons=new QPushButton*[0];// ;
    fitButtons=new QPushButton*[0];// **;
    checkButtons=new QPushButton*[0];// ;
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,6,6);
    layout->addMultiCellWidget(runInput,0,0,1,2);
    layout->addMultiCellWidget(beamInput,0,0,4,5);
    layout->addMultiCellWidget(authorInput,1,1,1,2);
    layout->addMultiCellWidget(fillBox,3,3,0,5);
    layout->addMultiCellWidget(fitBox,4,4,0,5);
    layout->addMultiCellWidget(checkBox,5,5,0,5);
#else
    layout=new QGridLayout(this);
    layout->addWidget(runInput,0,1,1,2);
    layout->addWidget(beamInput,0,4,1,2);
    layout->addWidget(authorInput,1,1,1,2);
    layout->addWidget(fillBox,3,0,1,6);
    layout->addWidget(fitBox,4,0,1,6);
    layout->addWidget(checkBox,5,0,1,6);
    setLayout(layout);
#endif
    layout->addWidget(runLabel,0,0);
    layout->addWidget(beamLabel,0,3);
    layout->addWidget(authorLabel,1,0);
    layout->addWidget(browseButton,1,3);
    layout->addWidget(fillAllButton,2,0);
    layout->addWidget(fitAllButton,2,1);
    layout->addWidget(checkAllButton,2,2);
    layout->addWidget(writeButton,2,4);
    layout->addWidget(viewButton,2,3);
    checkLayout=new QGridLayout(checkBox);
#if QT_VERSION < 0x040000
    checkLayout->addMultiCellWidget(checkLabel,0,0,0,2);
#else
    checkLayout->addWidget(checkLabel,0,0,1,2);
    checkBox->setLayout(checkLayout);
#endif
    fitLayout=new QGridLayout(fitBox);
#if QT_VERSION < 0x040000
    fitLayout->addMultiCellWidget(fitLabel,0,0,0,2);
#else
    fitLayout->addWidget(fitLabel,0,0,1,2);
    fitBox->setLayout(fitLayout);
#endif
    fillLayout=new QGridLayout(fillBox);
#if QT_VERSION < 0x040000
    fillLayout->addMultiCellWidget(fillLabel,0,0,0,2);
#else
    fillLayout->addWidget(fillLabel,0,0,1,2);
    fillBox->setLayout(fillLayout);
#endif
    layout->setMargin(5);
    layout->setSpacing(5);
    fillLayout->setMargin(5);
    fillLayout->setSpacing(5);
    checkLayout->setMargin(5);
    checkLayout->setSpacing(5);
    fitLayout->setMargin(5);
    fitLayout->setSpacing(5);
    connect(browseButton,SIGNAL(clicked()),this,SIGNAL(browseClicked()));
    connect(fillAllButton,SIGNAL(clicked()),this,SIGNAL(fillAllClicked()));
    connect(fitAllButton,SIGNAL(clicked()),this,SIGNAL(fitAllClicked()));
    connect(checkAllButton,SIGNAL(clicked()),this,SIGNAL(checkAllClicked()));
    connect(writeButton,SIGNAL(clicked()),this,SIGNAL(write()));
    connect(viewButton,SIGNAL(clicked()),this,SIGNAL(view()));
    languageChange();
    resize( QSize(584, 381).expandedTo(minimumSizeHint()) );
    nDets=0;
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

controlWidget::~controlWidget()
{

}

void controlWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "controlcuts" ) );
#else
    setWindowTitle( tr( "controlcuts" ) );
#endif
    writeButton->setText(tr("Write"));
    viewButton->setText(tr("View"));
    browseButton->setText(tr("Browse"));
    runLabel->setText(tr("run"));
    beamLabel->setText(tr("beam-time"));
    authorLabel->setText(tr("author"));
    fillLabel->setText(tr("fill histograms"));
    fitLabel->setText(tr("fit histograms"));
    checkLabel->setText(tr("check histograms"));
    fillAllButton->setText(tr("fill all"));
    fitAllButton->setText(tr("fit all"));
    checkAllButton->setText(tr("check all"));
    authorInput->setText("K.Ehrhardt");
    beamInput->setText("Oct 04");
}
#include <iostream>
void controlWidget::fillClicked()
{
  if(fillButtons==NULL)return;
  for(int i=0;i<nDets;i++)
    {
      if(fillButtons[i]==NULL)continue;
#if QT_VERSION < 0x040000
      if(fillButtons[i]->hasMouse())
#else
      if(fillButtons[i]->underMouse())
#endif
	{
	  emit fillClicked(i);
	}
    }
}
void controlWidget::fitClicked()
{
  if(fitButtons==NULL)return;
  for(int i=0;i<nDets;i++)
    {
      if(fitButtons[i]==NULL)continue;
#if QT_VERSION < 0x040000
      if(fitButtons[i]->hasMouse())
#else
      if(fitButtons[i]->underMouse())
#endif
	  emit fitClicked(i);
    }
}
void controlWidget::checkClicked()
{
  if(checkButtons==NULL)return;
  for(int i=0;i<nDets;i++)
    {
      if(checkButtons[i]==NULL)continue;
#if QT_VERSION < 0x040000
      if(checkButtons[i]->hasMouse())
#else
      if(checkButtons[i]->underMouse())
#endif
	  emit checkClicked(i);
    }
}
void controlWidget::setDetectors(vector<detector_parameter> names)
{
  vector<string> tmp;
  for(unsigned int i=0;i<names.size();i++)tmp.push_back(names[i].getName());
  setDetectors(tmp);
}
void controlWidget::setDetectors(vector<string> names)
{
  for(int i=0;i<nDets;i++)
    {
      if(fillButtons[i]!=NULL)
	{
	  fillLayout->remove(fillButtons[i]);
	  disconnect(fillButtons[i]);
	  delete fillButtons[i];
	}
      if(fitButtons[i]!=NULL)
	{
	  fitLayout->remove(fitButtons[i]);
	  disconnect(fitButtons[i]);
	  delete fitButtons[i];
	}
      if(checkButtons[i]!=NULL)
	{
	  checkLayout->remove(checkButtons[i]);
	  disconnect(checkButtons[i]);
	  delete checkButtons[i];
	}
    }
  delete [] fillButtons;
  delete [] fitButtons;
  delete [] checkButtons;
  nDets=names.size();
  fillButtons=new QPushButton*[nDets];
  fitButtons=new QPushButton*[nDets];
  checkButtons=new QPushButton*[nDets];
  for(int i=0;i<nDets;i++)
    {
      fillButtons[i]=new QPushButton(fillBox);
      fitButtons[i]=new QPushButton(fitBox);
      checkButtons[i]=new QPushButton(checkBox);
      fillButtons[i]->setText(names[i].data());
      fitButtons[i]->setText(names[i].data());
      checkButtons[i]->setText(names[i].data());
      fillLayout->addWidget(fillButtons[i],i/4+1,i%4);
      fitLayout->addWidget(fitButtons[i],i/4+1,i%4);
      checkLayout->addWidget(checkButtons[i],i/4+1,i%4);
      connect(fillButtons[i],SIGNAL(clicked()),this,SLOT(fillClicked()));
      connect(fitButtons[i],SIGNAL(clicked()),this,SLOT(fitClicked()));
      connect(checkButtons[i],SIGNAL(clicked()),this,SLOT(checkClicked()));
    }
}
