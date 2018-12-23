#include "beamtimedisplay.h"

#include <qlabel.h>
#include <qtextedit.h>
#include <qgroupbox.h>
#include <qcanvaswidget.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#include <qlayout.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#endif
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "beamtimedisplay.ui.h"
#if QT_VERSION < 0x040000
BeamTimeDisplay::BeamTimeDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "BeamTimeDisplay" );
    calInput = new QListBox( this);
#else
BeamTimeDisplay::BeamTimeDisplay( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
{
  setObjectName( "BeamTimeDisplay" );
    calInput = new QListWidget(this);
#endif
    ReadOnly=false;
    nameLabel = new QLabel( this);
    nameLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    nameInput = new QLineEdit( this);
    nameInput->setGeometry( QRect(10,170,74,20));

    setupLabel = new QLabel( this);
    setupLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    setupInput = new QLineEdit( this);
    setupInput->setGeometry( QRect(10,170,74,20));
    browseButton=new QPushButton(this);
    browseButton->setGeometry( QRect(1,1,20,20));
    browseButton->setMaximumSize(QSize(23,23));
    monthLabel = new QLabel( this);
    monthLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    monthInput = new QSpinBox( this);
    monthInput->setGeometry( QRect(10,170,74,20));
    ((QSpinBox*)monthInput)->setRange(1,12);
   
    yearLabel = new QLabel( this);
    yearLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    yearInput = new QSpinBox( this);
    yearInput->setGeometry( QRect(10,170,74,20));
    ((QSpinBox*)yearInput)->setRange(1900,2500);

    calibrationLabel = new QLabel( this);
    calibrationLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    calInput->setGeometry( QRect(10,170,74,20));

    descrLabel = new QLabel( this);
    descrLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    descrInput = new QTextEdit( this);
    descrInput->setGeometry( QRect(10,170,74,20));
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,6,7);
    layout->addWidget(nameLabel,0,0);
    layout->addMultiCellWidget(nameInput,0,0,1,3);
    layout->addMultiCellWidget(setupLabel,3,3,0,3);
    layout->addMultiCellWidget(setupInput,4,4,0,2);
    layout->addMultiCellWidget(browseButton,4,4,3,3);
    layout->addMultiCellWidget(monthLabel,1,1,0,1);
    layout->addMultiCellWidget(monthInput,2,2,0,1);
    layout->addMultiCellWidget(yearLabel,1,1,2,3);
    layout->addMultiCellWidget(yearInput,2,2,2,3);
    layout->addMultiCellWidget(calibrationLabel,5,5,0,3);
    layout->addMultiCellWidget(calInput,6,6,0,5);
    layout->addMultiCellWidget(descrLabel,0,0,4,4);
    layout->addMultiCellWidget(descrInput,1,5,4,4);
    layout->setColStretch(0,0);
    layout->setColStretch(1,2);
    layout->setColStretch(2,2);
    layout->setColStretch(3,2);
    layout->setColStretch(4,0);
    layout->setColStretch(5,0);
#else
    layout=new QGridLayout(this);
    layout->addWidget(nameLabel,0,0);
    layout->addWidget(nameInput,0,1,1,3);
    layout->addWidget(setupLabel,3,0,1,4);
    layout->addWidget(setupInput,4,0,1,3);
    layout->addWidget(browseButton,4,3,1,1);
    layout->addWidget(monthLabel,1,0,1,2);
    layout->addWidget(monthInput,2,0,1,2);
    layout->addWidget(yearLabel,1,2,1,2);
    layout->addWidget(yearInput,2,2,1,2);
    layout->addWidget(calibrationLabel,5,0,1,4);
    layout->addWidget(calInput,6,0,1,5);
    layout->addWidget(descrLabel,0,4,1,1);
    layout->addWidget(descrInput,1,4,5,1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,2);
    layout->setColumnStretch(2,2);
    layout->setColumnStretch(3,2);
    layout->setColumnStretch(4,0);
    layout->setColumnStretch(5,0);
    setLayout(layout);
#endif
    layout->setMargin(0);
    layout->setSpacing(5);
    setMinimumSize(100,23*4+4);
    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    setMinimumSize(100,23*8);

    // signals and slots connections
    connect(nameInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(setupInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(monthInput,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    connect(yearInput,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    connect(browseButton,SIGNAL(clicked()),this,SLOT(onBrowseClick()));
#if QT_VERSION < 0x040000
    connect(calInput,SIGNAL(rightButtonClicked( QListBoxItem *, const QPoint & )),this,SLOT(onCalibrationRightClick(QListBoxItem*, const QPoint&)));
    connect(descrInput,SIGNAL(doubleClicked( int, int)),this,SLOT(onDescriptionChange()));
#else
    calInput->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(calInput,SIGNAL(customContextMenuRequested ( const QPoint & )),this,SLOT(onCalibrationRightClick(const QPoint&)));
    //connect(descrInput,SIGNAL(doubleClicked( int, int)),this,SLOT(onDescriptionChange()));
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
BeamTimeDisplay::~BeamTimeDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void BeamTimeDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Beam-time" ) );
#else
    setWindowTitle( tr( "Beam-time" ) );
#endif
    nameLabel->setText( tr( "Name" ) );
    setupLabel->setText( tr( "Setup-file" ) );
    monthLabel->setText( tr( "Month" ) );
    yearLabel->setText( tr( "Year" ) );
    calibrationLabel->setText( tr( "Calibration files" ) );
    descrLabel->setText( tr( "Description" ) );
    browseButton->setText(tr("..."));
}

