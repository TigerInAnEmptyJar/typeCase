/****************************************************************************
** Form implementation generated from reading ui file 'attributewidget.ui'
**
** Created: Wed Sep 23 19:53:51 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "attributewidget.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include <qvariant.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "colorselectdialog.h"
#include "attributewidget.ui.h"
/*
 *  Constructs a AttributeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */

#if QT_VERSION < 0x040000
AttributeWidget::AttributeWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
#else
  AttributeWidget::AttributeWidget( QWidget* parent ,  Qt::WindowFlags f )
    :QWidget(parent,f)
#endif
{
#if QT_VERSION < 0x040000
    if ( !name )
	setName( "AttributeWidget" );
#else
    setObjectName("AttributeWidget" );
#endif

    spinBox1 = defineSpinBox( this, "spinBox1" );
    spinBox1->setGeometry( QRect( 60, 40, 80, 21 ) );
    spinBox1->setValue( 1 );

    comboBox1 = defineComboBox( FALSE, this, "comboBox1" );
    comboBox1->setGeometry( QRect( 60, 70, 130, 21 ) );

    spinBox2 = defineSpinBox( this, "spinBox2" );
    spinBox2->setGeometry( QRect( 200, 70, 80, 21 ) );

    textLabel4 = defineLabel( this, "textLabel4" );
    textLabel4->setGeometry( QRect( 20, 100, 60, 20 ) );

    textLabel5 = defineLabel( this, "textLabel5" );
    textLabel5->setGeometry( QRect( 20, 160, 60, 20 ) );

    spinBox3 = defineSpinBox( this, "spinBox3" );
    spinBox3->setGeometry( QRect( 90, 160, 70, 21 ) );

    textLabel7 = defineLabel( this, "textLabel7" );
    textLabel7->setGeometry( QRect( 20, 220, 40, 20 ) );

    textLabel8 = defineLabel( this, "textLabel8" );
    textLabel8->setGeometry( QRect( 20, 240, 58, 20 ) );

    spinBox5 = defineSpinBox( this, "spinBox5" );
    spinBox5->setGeometry( QRect( 80, 240, 50, 21 ) );
    spinBox5->setValue( 1 );

    comboBox5 = defineComboBox( FALSE, this, "comboBox5" );
    comboBox5->setGeometry( QRect( 70, 220, 160, 21 ) );

    comboBox6 = defineComboBox( FALSE, this, "comboBox6" );
    comboBox6->setGeometry( QRect( 190, 140, 100, 21 ) );

    colorButton = new QPushButton( this
#if QT_VERSION < 0x040000
, "colorButton" 
#endif
);
    colorButton->setGeometry( QRect( 110, 10, 50, 23 ) );

    textLabel2 = defineLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 20, 20, 40, 20 ) );

    textLabel1 = defineLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 190, 10, 40, 20 ) );

    textLabel3 = defineLabel( this, "textLabel3" );
    textLabel3->setGeometry( QRect( 20, 70, 30, 20 ) );

    comboBox4 = defineComboBox( FALSE, this, "comboBox4" );
    comboBox4->setGeometry( QRect( 90, 130, 78, 21 ) );

    comboBox2 = defineComboBox( FALSE, this, "comboBox2" );
    comboBox2->setGeometry( QRect( 190, 100, 110, 21 ) );

    comboBox3 = defineComboBox( FALSE, this, "comboBox3" );
    comboBox3->setGeometry( QRect( 90, 100, 78, 21 ) );

    spinBox4 = defineSpinBox( this, "spinBox4" );
    spinBox4->setGeometry( QRect( 130, 190, 39, 21 ) );

    textLabel6 = defineLabel( this, "textLabel6" );
    textLabel6->setGeometry( QRect( 20, 190, 104, 20 ) );
#if QT_VERSION < 0x040000
    spinBox1->setMaxValue( 10 );
    spinBox2->setMaxValue( 4100 );
    spinBox3->setMaxValue( 360 );
    spinBox5->setMaxValue( 3 );
    spinBox4->setMaxValue( 100 );
    spinBox4->setMinValue( 3 );
    spinBox3->setLineStep( 5 );
#else
    spinBox1->setMaximum( 10 );
    spinBox2->setMaximum( 4100 );
    spinBox3->setMaximum( 360 );
    spinBox5->setMaximum( 3 );
    spinBox4->setMaximum( 100 );
    spinBox4->setMinimum( 3 );
    spinBox3->setSingleStep( 5 );
#endif
    languageChange();
    resize( QSize(308, 267).expandedTo(minimumSizeHint()) );
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
AttributeWidget::~AttributeWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AttributeWidget::languageChange()
{
#if QT_VERSION < 0x040000
   setCaption( tr( "Attribute" ) );
#else
#endif
    comboBox1->clear();
    add2ComboBox(comboBox1, tr( "no line" ) );
    add2ComboBox(comboBox1, tr( "_____________" ) );
    add2ComboBox(comboBox1, tr( "- - - - - - - - - - - - - -" ) );
    add2ComboBox(comboBox1, tr( ". . . . . . . . . . . . . . . . " ) );
    add2ComboBox(comboBox1, tr( "- . - . - . - . - . - . - . -" ) );
    add2ComboBox(comboBox1, tr( "_ . _ . _ . _ . _ . _ ." ) );
    add2ComboBox(comboBox1, tr( "- . . . - . . . - . . . - . . " ) );
    add2ComboBox(comboBox1, tr( "_ _ _ _ _ _ _ _ _" ) );
    add2ComboBox(comboBox1, tr( "- . . - . . - . . - . . - . ." ) );
    add2ComboBox(comboBox1, tr( "__ __ __ __ __ __" ) );
    add2ComboBox(comboBox1, tr( "__ . __ . __ . __ ." ) );
    textLabel4->setText( tr( "alignment" ) );
    textLabel5->setText( tr( "angle" ) );
    textLabel7->setText( tr( "Font" ) );
    textLabel8->setText( tr( "Precision" ) );
    //QToolTip::add( spinBox5, tr( "Precision 0 fonts can not be rotated!" ) );
    comboBox5->clear();
    add2ComboBox(comboBox5, tr( "times-medium-i-normal" ) );
    add2ComboBox(comboBox5, tr( "times-bold-r-normal" ) );
    add2ComboBox(comboBox5, tr( "times-bold-i-normal" ) );
    add2ComboBox(comboBox5, tr( "helvetica-medium-r-normal" ) );
    add2ComboBox(comboBox5, tr( "helvetica-medium-o-normal" ) );
    add2ComboBox(comboBox5, tr( "helvetica-bold-r-normal" ) );
    add2ComboBox(comboBox5, tr( "helvetica-bold-o-normal" ) );
    add2ComboBox(comboBox5, tr( "courier-medium-r-normal" ) );
    add2ComboBox(comboBox5, tr( "courier-medium-o-normal" ) );
    add2ComboBox(comboBox5, tr( "courier-bold-r-normal" ) );
    add2ComboBox(comboBox5, tr( "courier-bold-o-normal" ) );
    add2ComboBox(comboBox5, tr( "symbol-medium-r-normal" ) );
    add2ComboBox(comboBox5, tr( "times-medium-r-normal" ) );
    add2ComboBox(comboBox5, tr( "\"Wingdings\"" ) );
    comboBox6->clear();
    add2ComboBox(comboBox6, tr( "sunken" ) );
    add2ComboBox(comboBox6, tr( "flat" ) );
    add2ComboBox(comboBox6, tr( "raised" ) );
    colorButton->setText( QString::null );
    textLabel2->setText( tr( "Width" ) );
    textLabel1->setText( tr( "Color" ) );
    textLabel3->setText( tr( "Style" ) );
    comboBox4->clear();
    add2ComboBox(comboBox4, tr( "left" ) );
    add2ComboBox(comboBox4, tr( "center" ) );
    add2ComboBox(comboBox4, tr( "right" ) );
    comboBox2->clear();
    add2ComboBox(comboBox2, tr( "no marker" ) );
    add2ComboBox(comboBox2, tr( "kDot" ) );
    add2ComboBox(comboBox2, tr( "kPlus" ) );
    add2ComboBox(comboBox2, tr( "kStar" ) );
    add2ComboBox(comboBox2, tr( "kCircle" ) );
    add2ComboBox(comboBox2, tr( "kMultiply" ) );
    add2ComboBox(comboBox2, tr( "kFullDotSmall" ) );
    add2ComboBox(comboBox2, tr( "kFullDotMedium" ) );
    add2ComboBox(comboBox2, tr( "kFullDotLarge" ) );
    add2ComboBox(comboBox2, tr( "kDot" ) );
    add2ComboBox(comboBox2, tr( "kFullCircle" ) );
    add2ComboBox(comboBox2, tr( "kFullSquare" ) );
    add2ComboBox(comboBox2, tr( "kFullTriangleUp" ) );
    add2ComboBox(comboBox2, tr( "kFullTriangleDown" ) );
    add2ComboBox(comboBox2, tr( "kOpenCircle" ) );
    add2ComboBox(comboBox2, tr( "kOpenCircle" ) );
    add2ComboBox(comboBox2, tr( "kOpenSquare" ) );
    add2ComboBox(comboBox2, tr( "kOpenTriangleUp" ) );
    add2ComboBox(comboBox2, tr( "kOpenDiamond" ) );
    add2ComboBox(comboBox2, tr( "kOpenCross" ) );
    add2ComboBox(comboBox2, tr( "kOpenStar" ) );
    add2ComboBox(comboBox2, tr( "kFullStar" ) );
    comboBox3->clear();
    add2ComboBox(comboBox3, tr( "bottom" ) );
    add2ComboBox(comboBox3, tr( "center" ) );
    add2ComboBox(comboBox3, tr( "top" ) );
    textLabel6->setText( tr( "Text height (%pad)" ) );
}

