/****************************************************************************
** Form implementation generated from reading ui file 'canvaspwidget.ui'
**
** Created: Wed Sep 23 19:58:52 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "canvaspwidget.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#include <TList.h>
#endif
#include <qvariant.h>
#include <iostream>
#include <attributewidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "canvaspwidget.ui.h"
//#include "dividedialog.h"
/*
 *  Constructs a CanvasPWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
CanvasPWidget::CanvasPWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
#else
CanvasPWidget::CanvasPWidget( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
#endif
{
#if QT_VERSION < 0x040000
    if ( !name )
	setName( "CanvasPWidget" );
#else
    setObjectName("CanvasPWidget" );
#endif

    textLabel1 = new QLabel( this);
    textLabel1->setGeometry( QRect( 10, 10, 58, 20 ) );

    textLabel2 = new QLabel( this);
    textLabel2->setGeometry( QRect( 70, 10, 58, 20 ) );

    textLabel3 = new QLabel( this);
    textLabel3->setGeometry( QRect( 10, 40, 58, 20 ) );

#if QT_VERSION < 0x040000
    comboBox1 = new QComboBox( FALSE, this, "comboBox1" );
#else
    comboBox1 = new QComboBox(this );
#endif
    comboBox1->setGeometry( QRect( 70, 40, 78, 21 ) );

    groupBox1 = new QGroupBox( this);
    groupBox1->setGeometry( QRect( 170, 200, 180, 120 ) );
    groupBox2 = new QGroupBox( this);
    groupBox2->setGeometry( QRect( 10, 70, 140, 120 ) );
    groupBox3 = new QGroupBox( this);
    groupBox3->setGeometry( QRect( 10, 200, 140, 120 ) );

    attributeWidget1 = new AttributeWidget( groupBox2);
    attributeWidget1->setGeometry( QRect( 0, 10, 121, 101 ) );


    attributeWidget2 = new AttributeWidget( groupBox3);
    attributeWidget2->setGeometry( QRect( 0, 10, 121, 101 ) );

#if QT_VERSION < 0x040000
    buttonGroup1 = new QButtonGroup( this);
#else
    buttonGroup1 = new QGroupBox( this);
#endif
    buttonGroup1->setGeometry( QRect( 170, 70, 120, 110 ) );

    checkBox2 = new QCheckBox(buttonGroup1);
    checkBox2->setGeometry( QRect( 10, 50, 100, 20 ) );

    checkBox3 = new QCheckBox(buttonGroup1);
    checkBox3->setGeometry( QRect( 10, 80, 100, 20 ) );

    checkBox1 = new QCheckBox(buttonGroup1);
    checkBox1->setGeometry( QRect( 10, 20, 100, 20 ) );


    textLabel6 = new QLabel( groupBox1);
    textLabel6->setGeometry( QRect( 10, 20, 58, 20 ) );

    textLabel7 = new QLabel( groupBox1);
    textLabel7->setGeometry( QRect( 10, 70, 58, 20 ) );

    lineEdit4 = new QLineEdit( groupBox1);
    lineEdit4->setGeometry( QRect( 90, 40, 80, 21 ) );

    lineEdit5 = new QLineEdit( groupBox1);
    lineEdit5->setGeometry( QRect( 10, 90, 70, 21 ) );

    lineEdit6 = new QLineEdit( groupBox1);
    lineEdit6->setGeometry( QRect( 90, 90, 80, 21 ) );

    lineEdit3 = new QLineEdit( groupBox1);
    lineEdit3->setGeometry( QRect( 10, 40, 70, 21 ) );

    textLabel4 = new QLabel( this);
    textLabel4->setGeometry( QRect( 10, 340, 40, 20 ) );

    textLabel5 = new QLabel( this);
    textLabel5->setGeometry( QRect( 160, 340, 40, 20 ) );

    lineEdit1 = new QLineEdit( this);
    lineEdit1->setGeometry( QRect( 60, 340, 89, 21 ) );

    lineEdit2 = new QLineEdit( this);
    lineEdit2->setGeometry( QRect( 210, 340, 89, 21 ) );

    pushButton2 = new QPushButton( this);
    pushButton2->setGeometry( QRect( 80, 380, 60, 23 ) );

    pushButton4 = new QPushButton( this);
    pushButton4->setGeometry( QRect( 150, 380, 60, 23 ) );

    pushButton3 = new QPushButton( this);
    pushButton3->setGeometry( QRect( 290, 380, 60, 23 ) );

    pushButton1 = new QPushButton( this);
    pushButton1->setGeometry( QRect( 10, 380, 60, 23 ) );

    pushButton5 = new QPushButton( this);
    pushButton5->setGeometry( QRect( 170, 40, 60, 23 ) );

    pushButton6 = new QPushButton( this);
    pushButton6->setGeometry( QRect( 250, 40, 70, 23 ) );
    languageChange();
    resize( QSize(362, 412).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( OnApplyClick() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( OnHelpClick() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( OnUpdateClick() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( OnClearClick() ) );
    connect( pushButton6, SIGNAL( clicked() ), this, SLOT( OnDivideClick() ) );
    connect( comboBox1, SIGNAL( textChanged(const QString&) ), this, SLOT( OnPadSelect() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CanvasPWidget::~CanvasPWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CanvasPWidget::languageChange()
{
  //    setCaption( tr( "Canvas Properties" ) );
    textLabel1->setText( tr( "Name" ) );
    textLabel2->setText( tr( "textLabel2" ) );
    textLabel3->setText( tr( "Pad" ) );
    comboBox1->clear();
#if QT_VERSION < 0x040000
    comboBox1->insertItem( tr( "0" ) );
#else
    comboBox1->addItem( tr( "0" ) );
#endif
    groupBox2->setTitle( tr( "Fill" ) );
    //    QToolTip::add( attributeWidget1, tr( "Canvas background fill. For change of  histogram background, redraw histogram." ) );
    groupBox3->setTitle( tr( "Line" ) );
    //    QToolTip::add( attributeWidget2, tr( "Canvas lines" ) );
    buttonGroup1->setTitle( tr( "Scale" ) );
    checkBox2->setText( tr( "logarithmic Y" ) );
    checkBox3->setText( tr( "logarithmic Z" ) );
    checkBox1->setText( tr( "logarithmic X" ) );
    groupBox1->setTitle( tr( "Range" ) );
    textLabel6->setText( tr( "X" ) );
    textLabel7->setText( tr( "Y" ) );
    textLabel4->setText( tr( "Theta" ) );
    textLabel5->setText( tr( "Phi" ) );
    pushButton2->setText( tr( "Cancel" ) );
    pushButton4->setText( tr( "Update" ) );
    pushButton3->setText( tr( "Help" ) );
    pushButton1->setText( tr( "Apply" ) );
    pushButton5->setText( tr( "clear" ) );
    pushButton6->setText( tr( "divide" ) );
}

