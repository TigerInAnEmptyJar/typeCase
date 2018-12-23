#include "rundisplay.h"

//#include <qvariant.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qgroupbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#if QT_VERSION < 0x040000
#include <qlistview.h>
#include <qlistbox.h>
#include <qlineedit.h>
#else
#include <QResizeEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QGridLayout>
#endif
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "rundisplay.ui.h"
#if QT_VERSION < 0x040000
RunDisplay::RunDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "RunDisplay" );
#else
RunDisplay::RunDisplay( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent,  f )
{
  setObjectName( "RunDisplay" );
#endif
    ReadOnly=false;
    nameLabel = new QLabel( this);
    nameLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    nameInput = new QLineEdit( this);
    nameInput->setGeometry( QRect(10,170,74,20));

    setupLabel = new QCheckBox( this);
    setupLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    setupInput = new QLineEdit( this);
    setupInput->setGeometry( QRect(10,170,74,20));
    browseButton=new QPushButton(this);
    browseButton->setGeometry( QRect(1,1,20,20));
    browseButton->setMaximumSize(QSize(23,23));
    beamtimeLabel = new QLabel( this);
    beamtimeLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    beamtimeInput = new QComboBox( this);
    beamtimeInput->setGeometry( QRect(10,170,74,20));
   
    typeLabel = new QLabel( this);
    typeLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    typeInput = new QComboBox( this);
    typeInput->setGeometry( QRect(10,170,74,20));

    calibrationLabel = new QCheckBox( this);
    calibrationLabel->setGeometry( QRect( 10, 170, 74, 20 ) );

#if QT_VERSION < 0x040000
    calInput = new QListBox( this);
    filesInput = new QListView(this);
#else
    calInput = new QListWidget( this);
    filesInput = new QTreeWidget(this);
#endif
    calInput->setGeometry( QRect(10,170,74,20));
    filesInput->setGeometry( QRect(10,170,74,20));

    descrLabel = new QLabel( this);
    descrLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    descrInput = new QTextEdit( this);
    descrInput->setGeometry( QRect(10,170,74,20));
    
    runNumberLabel= new QLabel(this);
    runNumberLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    runNumberInput= new QLineEdit(this);
    runNumberInput->setGeometry( QRect( 10, 170, 74, 20 ) );

    startLabel = new QLabel(this);
    startLabel->setGeometry( QRect(10,170,74,20));
    startInput = new QDateTimeEdit(this);
    startInput->setGeometry( QRect(10,170,74,20));

    stopLabel = new QLabel(this);
    stopLabel->setGeometry( QRect(10,170,74,20));
    stopInput = new QDateTimeEdit(this);
    stopInput->setGeometry( QRect(10,170,74,20));
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,12,5);
    layout->addWidget(nameLabel,0,0);
    layout->addMultiCellWidget(nameInput,0,0,1,2);
    layout->addMultiCellWidget(beamtimeLabel,1,1,0,0);
    layout->addMultiCellWidget(beamtimeInput,1,1,1,2);
    layout->addMultiCellWidget(typeLabel,2,2,0,0);
    layout->addMultiCellWidget(typeInput,2,2,1,2);
    layout->addMultiCellWidget(runNumberLabel,3,3,0,0);
    layout->addMultiCellWidget(runNumberInput,3,3,1,2);
    layout->addMultiCellWidget(descrLabel,4,4,0,2);
    layout->addMultiCellWidget(descrInput,5,7,0,2);
    layout->addMultiCellWidget(startLabel,8,8,0,0);
    layout->addMultiCellWidget(startInput,8,8,1,2);
    layout->addMultiCellWidget(stopLabel,9,9,0,0);
    layout->addMultiCellWidget(stopInput,9,9,1,2);
    layout->addMultiCellWidget(setupLabel,10,10,0,2);
    layout->addMultiCellWidget(setupInput,11,11,0,3);
    layout->addMultiCellWidget(browseButton,11,11,4,4);
    layout->addMultiCellWidget(filesInput,0,5,3,4);
    layout->addMultiCellWidget(calibrationLabel,6,6,3,4);
    layout->addMultiCellWidget(calInput,7,10,3,4);
    layout->setColStretch(0,0);
    layout->setColStretch(1,2);
    layout->setColStretch(2,2);
    layout->setColStretch(3,4);
    layout->setColStretch(4,0);
#else
    layout=new QGridLayout(this);
    layout->addWidget(nameLabel,0,0);
    layout->addWidget(nameInput,0,1,1,2);
    layout->addWidget(beamtimeLabel,1,0,1,1);
    layout->addWidget(beamtimeInput,1,1,1,2);
    layout->addWidget(typeLabel,2,0,1,1);
    layout->addWidget(typeInput,2,1,1,2);
    layout->addWidget(runNumberLabel,3,0,1,1);
    layout->addWidget(runNumberInput,3,1,1,2);
    layout->addWidget(descrLabel,4,0,1,3);
    layout->addWidget(descrInput,5,0,3,3);
    layout->addWidget(startLabel,8,0,1,1);
    layout->addWidget(startInput,8,1,1,2);
    layout->addWidget(stopLabel,9,0,1,1);
    layout->addWidget(stopInput,9,1,1,2);
    layout->addWidget(setupLabel,10,0,1,3);
    layout->addWidget(setupInput,11,0,1,4);
    layout->addWidget(browseButton,11,4,1,1);
    layout->addWidget(filesInput,0,3,6,2);
    layout->addWidget(calibrationLabel,6,3,1,2);
    layout->addWidget(calInput,7,3,4,2);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,2);
    layout->setColumnStretch(2,2);
    layout->setColumnStretch(3,4);
    layout->setColumnStretch(4,0);
    setLayout(layout);
#endif
    layout->setMargin(0);
    layout->setSpacing(5);

    // signals and slots connections
    connect(nameInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(setupInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(runNumberInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(beamtimeInput,SIGNAL(activated(int)),this,SLOT(onBTChange()));
    connect(typeInput,SIGNAL(activated(int)),this,SLOT(onTPChange()));
#if QT_VERSION < 0x040000
    connect(stopInput,SIGNAL(valueChanged(const QDateTime&)),this,SLOT(onPropertyChange()));
    connect(startInput,SIGNAL(valueChanged(const QDateTime&)),this,SLOT(onPropertyChange()));
    connect(calInput,SIGNAL(rightButtonClicked( QListBoxItem *, const QPoint & )),this,SLOT(onCalibrationRightClick(QListBoxItem*, const QPoint&)));
    connect(filesInput,SIGNAL(rightButtonClicked( QListViewItem *, const QPoint & ,int)),this,SLOT(onFilesRightClick(QListViewItem*, const QPoint&)));
    connect(browseButton,SIGNAL(clicked()),this,SLOT(onBrowseClick()));
    connect(descrInput,SIGNAL(doubleClicked( int, int)),this,SLOT(onDescriptionChange()));
#else
    connect(stopInput,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(onPropertyChange()));
    connect(startInput,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(onPropertyChange()));
    calInput->setContextMenuPolicy(Qt::CustomContextMenu);
    filesInput->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(calInput,SIGNAL(customContextMenuRequested ( const QPoint & )),this,SLOT(onCalibrationRightClick(const QPoint&)));
    connect(filesInput,SIGNAL(customContextMenuRequested ( const QPoint & )),this,SLOT(onFilesRightClick(const QPoint&)));
//     connect(browseButton,SIGNAL(clicked()),this,SLOT(onBrowseClick()));
//    connect(descrInput,SIGNAL(doubleClicked( int, int)),this,SLOT(onDescriptionChange()));
#endif
    init();
    setMinimumSize(100,23*4+4);
    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    setMinimumSize(100,23*8);
}

/*
 *  Destroys the object and frees any allocated resources
 */
RunDisplay::~RunDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void RunDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Run" ) );
#else
    setWindowTitle(tr( "Run" ) );
#endif
    nameLabel->setText( tr( "Name" ) );
    setupLabel->setText( tr( "Setup-file" ) );
    beamtimeLabel->setText( tr( "Beamtime" ) );
    typeLabel->setText( tr( "Type" ) );
    calibrationLabel->setText( tr( "Calibration files" ) );
    descrLabel->setText( tr( "Description" ) );
    browseButton->setText(tr("..."));
    runNumberLabel->setText( tr( "Number" ) );
    startLabel->setText( tr( "Start" ) );
    stopLabel->setText( tr( "Stop" ) );
}

