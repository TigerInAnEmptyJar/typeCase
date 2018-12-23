/****************************************************************************
** Form implementation generated from reading ui file 'aattributewidget.ui'
**
** Created: Wed Sep 23 19:59:00 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/


#include "aattributewidget.h"
#include <qvariant.h>
#include <q3Dinput.h>
#include <attributewidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "q3Dinput.h"
#include "colorselectdialog.h"
#include "aattributewidget.ui.h"
/*
 *  Constructs a AAttributeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
AAttributeWidget::AAttributeWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
#else
AAttributeWidget::AAttributeWidget( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
#endif
{
#if QT_VERSION < 0x040000
    if ( !name )
	setName( "AAttributeWidget" );
#else
    setObjectName("AAttributeWidget" );
#endif

    q3DInput1 = new Q3DInput( this );
    q3DInput1->setGeometry( QRect( 90, 60, 180, 30 ) );

    lineEdit1 = new QLineEdit( this );
    lineEdit1->setGeometry( QRect( 100, 100, 89, 21 ) );

    textLabel2 = new QLabel( this );
    textLabel2->setGeometry( QRect( 10, 60, 64, 20 ) );

    textLabel3 = new QLabel( this );
    textLabel3->setGeometry( QRect( 10, 100, 62, 20 ) );

    colorButton = new QPushButton( this);
    colorButton->setGeometry( QRect( 90, 30, 108, 23 ) );

    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 20, 30, 58, 20 ) );

    groupBox5 = new QGroupBox( this );
    groupBox5->setGeometry( QRect( 190, 300, 170, 140 ) );

    attributeWidget2 = new AttributeWidget( groupBox5 );
    attributeWidget2->setGeometry( QRect( 0, 20, 161, 111 ) );

    groupBox6 = new QGroupBox( this );
    groupBox6->setGeometry( QRect( 390, 310, 150, 120 ) );

    attributeWidget3 = new AttributeWidget( groupBox6 );
    attributeWidget3->setGeometry( QRect( 10, 10, 131, 101 ) );

    groupBox3 = new QGroupBox( this);
    groupBox3->setGeometry( QRect( 20, 140, 140, 140 ) );

    lineEdit3_2 = new QLineEdit( groupBox3 );
    lineEdit3_2->setGeometry( QRect( 30, 110, 89, 21 ) );

#if QT_VERSION < 0x040000
    comboBox2_1 = new QComboBox( FALSE, groupBox3 );
    comboBox2_2 = new QComboBox( FALSE, groupBox3 );
    comboBox1_1 = new QComboBox( FALSE, groupBox2 );
    comboBox1_2 = new QComboBox( FALSE, groupBox2 );
#else
    comboBox2_1 = new QComboBox(groupBox3 );
    comboBox2_2 = new QComboBox(groupBox3 );
    comboBox1_1 = new QComboBox(groupBox2 );
    comboBox1_2 = new QComboBox(groupBox2 );
#endif
    comboBox2_2->setGeometry( QRect( 10, 50, 78, 21 ) );
    comboBox2_1->setGeometry( QRect( 10, 20, 78, 21 ) );
    comboBox1_1->setGeometry( QRect( 10, 20, 78, 21 ) );
    comboBox1_2->setGeometry( QRect( 10, 50, 78, 21 ) );

    textLabel4_2 = new QLabel( groupBox3 );
    textLabel4_2->setGeometry( QRect( 10, 80, 16, 20 ) );

    lineEdit3_1 = new QLineEdit( groupBox3 );
    lineEdit3_1->setGeometry( QRect( 30, 80, 89, 21 ) );

    textLabel5_2 = new QLabel( groupBox3 );
    textLabel5_2->setGeometry( QRect( 10, 110, 16, 20 ) );

    groupBox2 = new QGroupBox( this );
    groupBox2->setGeometry( QRect( 190, 140, 140, 140 ) );

    lineEdit2_2 = new QLineEdit( groupBox2 );
    lineEdit2_2->setGeometry( QRect( 30, 110, 89, 21 ) );

    lineEdit2_1 = new QLineEdit( groupBox2 );
    lineEdit2_1->setGeometry( QRect( 30, 80, 89, 21 ) );

    textLabel5 = new QLabel( groupBox2 );
    textLabel5->setGeometry( QRect( 10, 110, 16, 20 ) );

    textLabel4 = new QLabel( groupBox2 );
    textLabel4->setGeometry( QRect( 10, 80, 16, 20 ) );

    groupBox1 = new QGroupBox( this );
    groupBox1->setGeometry( QRect( 350, 150, 171, 111 ) );

    lineEdit1_1 = new QLineEdit( groupBox1 );
    lineEdit1_1->setGeometry( QRect( 60, 20, 89, 21 ) );

    lineEdit1_2 = new QLineEdit( groupBox1 );
    lineEdit1_2->setGeometry( QRect( 10, 50, 89, 21 ) );

    lineEdit1_3 = new QLineEdit( groupBox1 );
    lineEdit1_3->setGeometry( QRect( 100, 50, 89, 21 ) );

    lineEdit1_4 = new QLineEdit( groupBox1 );
    lineEdit1_4->setGeometry( QRect( 40, 80, 89, 21 ) );

    groupBox4 = new QGroupBox( this );
    groupBox4->setGeometry( QRect( 10, 300, 170, 140 ) );

    attributeWidget1 = new AttributeWidget( groupBox4 );
    attributeWidget1->setGeometry( QRect( 0, 20, 160, 110 ) );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( colorButton, SIGNAL( clicked() ), this, SLOT( OnColorButtonClick() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AAttributeWidget::~AAttributeWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AAttributeWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Advanced Attrribute" ) );
#endif
    textLabel2->setText( tr( "N-divisions" ) );
    textLabel3->setText( tr( "Tick-length" ) );
    colorButton->setText( QString::null );
    textLabel1->setText( tr( "Color" ) );
    groupBox5->setTitle( tr( "groupBox5" ) );
    groupBox6->setTitle( tr( "groupBox6" ) );
    groupBox3->setTitle( tr( "Filename" ) );
    comboBox2_1->clear();
    comboBox2_2->clear();
    comboBox1_1->clear();
    comboBox1_2->clear();
#if QT_VERSION < 0x040000
    comboBox2_1->insertItem( tr( "top" ) );
    comboBox2_1->insertItem( tr( "center" ) );
    comboBox2_1->insertItem( tr( "bottom" ) );
    comboBox2_2->insertItem( tr( "left" ) );
    comboBox2_2->insertItem( tr( "center" ) );
    comboBox2_2->insertItem( tr( "right" ) );
    comboBox1_1->insertItem( tr( "top" ) );
    comboBox1_1->insertItem( tr( "center" ) );
    comboBox1_1->insertItem( tr( "bottom" ) );
    comboBox1_2->insertItem( tr( "left" ) );
    comboBox1_2->insertItem( tr( "center" ) );
    comboBox1_2->insertItem( tr( "right" ) );
#else
    comboBox2_1->addItem( tr( "top" ) );
    comboBox2_1->addItem( tr( "center" ) );
    comboBox2_1->addItem( tr( "bottom" ) );
    comboBox2_2->addItem( tr( "left" ) );
    comboBox2_2->addItem( tr( "center" ) );
    comboBox2_2->addItem( tr( "right" ) );
    comboBox1_1->addItem( tr( "top" ) );
    comboBox1_1->addItem( tr( "center" ) );
    comboBox1_1->addItem( tr( "bottom" ) );
    comboBox1_2->addItem( tr( "left" ) );
    comboBox1_2->addItem( tr( "center" ) );
    comboBox1_2->addItem( tr( "right" ) );
#endif
    textLabel4_2->setText( tr( "x" ) );
    textLabel5_2->setText( tr( "y" ) );
    groupBox2->setTitle( tr( "Stat" ) );
    textLabel5->setText( tr( "y" ) );
    textLabel4->setText( tr( "x" ) );
    groupBox1->setTitle( tr( "Margins" ) );
    groupBox4->setTitle( tr( "groupBox4" ) );
}

