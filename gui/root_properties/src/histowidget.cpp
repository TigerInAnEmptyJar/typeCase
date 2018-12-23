/****************************************************************************
** Form implementation generated from reading ui file 'histowidget.ui'
**
** Created: Wed Sep 23 19:58:56 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "histowidget.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#include <math.h>
#endif
#include <qvariant.h>
#include <TProfile2D.h>
#include <TGraphErrors.h>
#include <TGraph2DErrors.h>
#include <iostream>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <attributewidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtoolbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "attributewidget.h"
#include "histowidget.ui.h"
/*
 *  Constructs a HistoWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
HistoWidget::HistoWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
#else
HistoWidget::HistoWidget( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
#endif
{
#if QT_VERSION < 0x040000
    if ( !name )
	setName( "HistoWidget" );
#else
	setObjectName( "HistoWidget" );
#endif

    lineEdit1 = new QLineEdit( this );
    lineEdit1->setEnabled( FALSE );
    lineEdit1->setGeometry( QRect( 80, 10, 290, 21 ) );
#if QT_VERSION < 0x040000
    lineEdit1->setFrameShape( QLineEdit::LineEditPanel );
    lineEdit1->setFrameShadow( QLineEdit::Sunken );
#else
    lineEdit1->setFrame(false);
#endif
    lineEdit2 = new QLineEdit( this );
    lineEdit2->setGeometry( QRect( 80, 40, 290, 21 ) );

    textLabel2 = new QLabel( this );
    textLabel2->setGeometry( QRect( 20, 40, 30, 20 ) );

    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 20, 10, 40, 20 ) );

    pushButton1 = new QPushButton( this );
    pushButton1->setGeometry( QRect( 10, 450, 70, 23 ) );

    pushButton2 = new QPushButton( this );
    pushButton2->setGeometry( QRect( 90, 450, 70, 23 ) );

    toolBox1 = new QToolBox( this );
    toolBox1->setGeometry( QRect( 0, 70, 400, 380 ) );
    toolBox1->setCurrentIndex( 0 );

    page1 = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page1->setBackgroundMode( QWidget::PaletteBackground );
#endif
    groupBox3 = new QGroupBox( page1 );
    groupBox3->setGeometry( QRect( 230, 250, 160, 80 ) );

    attributeWidget3 = new AttributeWidget( groupBox3 );
    attributeWidget3->setGeometry( QRect( 10, 20, 141, 51 ) );

    checkBox1 = new QCheckBox( page1 );
    checkBox1->setGeometry( QRect( 40, 300, 110, 20 ) );

    groupBox1 = new QGroupBox( page1 );
    groupBox1->setGeometry( QRect( 230, 10, 160, 110 ) );

    attributeWidget1 = new AttributeWidget( groupBox1 );
    attributeWidget1->setGeometry( QRect( 10, 20, 141, 81 ) );

    groupBox2 = new QGroupBox( page1 );
    groupBox2->setGeometry( QRect( 230, 130, 160, 110 ) );

    attributeWidget2 = new AttributeWidget( groupBox2 );
    attributeWidget2->setGeometry( QRect( 10, 20, 141, 81 ) );

    groupBox4 = new QGroupBox( page1 );
    groupBox4->setGeometry( QRect( 10, 0, 170, 290 ) );

#if QT_VERSION < 0x040000
    comboBox6 = new QComboBox( FALSE, groupBox4, "comboBox6" );
#else
    comboBox6 = new QComboBox(groupBox4);
#endif
    comboBox6->setGeometry( QRect( 20, 20, 130, 21 ) );

    textLabel21 = new QLabel( groupBox4 );
    textLabel21->setGeometry( QRect( 8, 46, 40, 20 ) );

    textLabel23 = new QLabel( groupBox4 );
    textLabel23->setGeometry( QRect( 8, 66, 40, 20 ) );

    lineEdit3 = new QLineEdit( groupBox4 );
    lineEdit3->setGeometry( QRect( 48, 66, 89, 21 ) );

    textLabel22 = new QLabel( groupBox4 );
    textLabel22->setGeometry( QRect( 48, 46, 65, 20 ) );
    toolBox1->addItem( page1, QString::fromLatin1("") );

    page2 = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page2->setBackgroundMode( QWidget::PaletteBackground );
#endif

    textLabel2_1 = new QLabel( page2 );
    textLabel2_1->setGeometry( QRect( 10, 0, 50, 20 ) );

    textLabel1_3 = new QLabel( page2 );
    textLabel1_3->setGeometry( QRect( 12, 20, 40, 20 ) );

    textLabel1_5 = new QLabel( page2 );
    textLabel1_5->setGeometry( QRect( 10, 40, 40, 20 ) );

    textLabel1_4 = new QLabel( page2 );
    textLabel1_4->setGeometry( QRect( 70, 20, 72, 20 ) );

    textLabel1_6 = new QLabel( page2 );
    textLabel1_6->setGeometry( QRect( 70, 40, 72, 20 ) );

    textLabel1_8 = new QLabel( page2 );
    textLabel1_8->setGeometry( QRect( 70, 60, 72, 20 ) );

    textLabel1_7 = new QLabel( page2 );
    textLabel1_7->setGeometry( QRect( 10, 60, 50, 20 ) );

    textLabel1_9 = new QLabel( page2 );
    textLabel1_9->setGeometry( QRect( 10, 80, 60, 20 ) );

    textLabel1_11 = new QLabel( page2);
    textLabel1_11->setGeometry( QRect( 32, 100, 30, 20 ) );

    textLabel1_10 = new QLabel( page2 );
    textLabel1_10->setGeometry( QRect( 70, 80, 79, 20 ) );

    textLabel1_12 = new QLabel( page2 );
    textLabel1_12->setGeometry( QRect( 70, 100, 79, 20 ) );

    textLabel1_13 = new QLabel( page2 );
    textLabel1_13->setGeometry( QRect( 10, 120, 60, 20 ) );

    textLabel1_15 = new QLabel( page2 );
    textLabel1_15->setGeometry( QRect( 32, 140, 30, 20 ) );

    textLabel1_14 = new QLabel( page2 );
    textLabel1_14->setGeometry( QRect( 70, 120, 79, 20 ) );

    textLabel1_16 = new QLabel( page2 );
    textLabel1_16->setGeometry( QRect( 70, 140, 79, 20 ) );

    textLabel1_2 = new QLabel( page2 );
    textLabel1_2->setGeometry( QRect( 70, 0, 72, 20 ) );
    toolBox1->addItem( page2, QString::fromLatin1("") );
    languageChange();
    resize( QSize(403, 578).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( OnApplyClick() ) );
    connect( comboBox6, SIGNAL( highlighted(int) ), this, SLOT( OnAxisSelect(int) ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( OnResetClick() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
HistoWidget::~HistoWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void HistoWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Histogram properties" ) );
#endif
    textLabel2->setText( tr( "Title" ) );
    textLabel1->setText( tr( "Name" ) );
    pushButton1->setText( tr( "Apply" ) );
    pushButton2->setText( tr( "Reset" ) );
    groupBox3->setTitle( tr( "Fill properties" ) );
    checkBox1->setText( tr( "create Errors" ) );
    groupBox1->setTitle( tr( "Line properties" ) );
    groupBox2->setTitle( tr( "Marker properties" ) );
    groupBox4->setTitle( tr( "Axes" ) );
    textLabel21->setText( tr( "Bins:" ) );
    textLabel23->setText( tr( "Title:" ) );
    textLabel22->setText( tr( "__________________" ) );
    textLabel2_1->setText( tr( "Entries:" ) );
    textLabel1_3->setText( tr( "Mean:" ) );
    textLabel1_5->setText( tr( "RMS:" ) );
    textLabel1_4->setText( tr( "textLabel1_4" ) );
    textLabel1_6->setText( tr( "textLabel1_6" ) );
    textLabel1_8->setText( tr( "textLabel1_8" ) );
    textLabel1_7->setText( tr( "Integral:" ) );
    textLabel1_9->setText( tr( "Minimum:" ) );
    textLabel1_11->setText( tr( "Bin:" ) );
    textLabel1_10->setText( tr( "textLabel1_10" ) );
    textLabel1_12->setText( tr( "textLabel1_12" ) );
    textLabel1_13->setText( tr( "Maximum:" ) );
    textLabel1_15->setText( tr( "Bin:" ) );
    textLabel1_14->setText( tr( "textLabel1_14" ) );
    textLabel1_16->setText( tr( "textLabel1_16" ) );
    textLabel1_2->setText( tr( "textLabel1_2" ) );
#if QT_VERSION < 0x040000
    toolBox1->setItemLabel( toolBox1->indexOf(page1), tr( "Properties" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page2), tr( "Info" ) );
#else
    toolBox1->setItemText( toolBox1->indexOf(page1), tr( "Properties" ) );
    toolBox1->setItemText( toolBox1->indexOf(page2), tr( "Info" ) );
#endif
}

