/****************************************************************************
** Form implementation generated from reading ui file 'stylewidget.ui'
**
** Created: Wed Sep 23 19:58:58 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "stylewidget.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif


#include <qvariant.h>
#include <TStyle.h>
#include <qtoolbox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "stylewidget.ui.h"
/*
 *  Constructs a StyleWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
    StyleWidget::StyleWidget( QWidget* parent, const char* name, WFlags fl)
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "StyleWidget" );
#else
    StyleWidget::StyleWidget( QWidget* parent,Qt::WindowFlags f)
    : QWidget( parent, f )
{
  setObjectName( "StyleWidget" );
#endif

    toolBox1 = new QToolBox( this);
    toolBox1->setGeometry( QRect( 0, 0, 600, 480 ) );
    toolBox1->setCurrentIndex( 0 );

    page1 = new QWidget( toolBox1);
#if QT_VERSION < 0x040000
    page1->setBackgroundMode( QWidget::PaletteBackground );
#endif
    toolBox1->addItem( page1, QString::fromLatin1("") );

    page2 = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page2->setBackgroundMode( QWidget::PaletteBackground );
#endif
    toolBox1->addItem( page2, QString::fromLatin1("") );

    page = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page->setBackgroundMode( QWidget::PaletteBackground );
#endif

    groupBox1 = new QGroupBox( page );
    groupBox1->setGeometry( QRect( 280, 0, 210, 110 ) );

    lineEdit1_2 = new QLineEdit( groupBox1 );
    lineEdit1_2->setGeometry( QRect( 10, 50, 89, 21 ) );

    lineEdit1_3 = new QLineEdit( groupBox1 );
    lineEdit1_3->setGeometry( QRect( 110, 50, 89, 21 ) );

    lineEdit1_4 = new QLineEdit( groupBox1 );
    lineEdit1_4->setGeometry( QRect( 60, 80, 89, 21 ) );

    lineEdit1_1 = new QLineEdit( groupBox1 );
    lineEdit1_1->setGeometry( QRect( 60, 16, 89, 21 ) );
    toolBox1->addItem( page, QString::fromLatin1("") );

    page_2 = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page_2->setBackgroundMode( QWidget::PaletteBackground );
#endif
    toolBox1->addItem( page_2, QString::fromLatin1("") );

    page_3 = new QWidget( toolBox1 );
#if QT_VERSION < 0x040000
    page_3->setBackgroundMode( QWidget::PaletteBackground );
#endif
    toolBox1->addItem( page_3, QString::fromLatin1("") );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
StyleWidget::~StyleWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void StyleWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "gStyle" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page1), tr( "Canvas" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page2), tr( "Pad" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page), tr( "Histogram" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page_2), tr( "Title" ) );
    toolBox1->setItemLabel( toolBox1->indexOf(page_3), tr( "General" ) );
#else
    toolBox1->setItemText( toolBox1->indexOf(page1), tr( "Canvas" ) );
    toolBox1->setItemText( toolBox1->indexOf(page2), tr( "Pad" ) );
    toolBox1->setItemText( toolBox1->indexOf(page), tr( "Histogram" ) );
    toolBox1->setItemText( toolBox1->indexOf(page_2), tr( "Title" ) );
    toolBox1->setItemText( toolBox1->indexOf(page_3), tr( "General" ) );
#endif
    groupBox1->setTitle( tr( "Margins" ) );
}

