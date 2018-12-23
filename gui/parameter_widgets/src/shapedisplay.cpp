/****************************************************************************
** Form implementation generated from reading ui file 'algorithmdisplay.ui'
**
** Created: Mon Nov 30 16:00:28 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "shapedisplay.h"

#if QT_VERSION < 0x040000
#include <qvariant.h>
#include <qheader.h>
#else
#include <QResizeEvent>
#include <QGridLayout>
#endif
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

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "shapedisplay.ui.h"
/*
 *  Constructs a ShapeDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#include "utilities.h"
#if QT_VERSION < 0x040000
ShapeDisplay::ShapeDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ShapeDisplay" );
#else
    ShapeDisplay::ShapeDisplay( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
{
  setObjectName( "ShapeDisplay" );
#endif
    nameLabel = new QLabel( this );
    nameLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    nameInput = NULL;

    descLabel = new QLabel( this );
    descLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    descriptionEdit = new QTextEdit( this );
    descriptionEdit->setGeometry( QRect( 5, 5, 300, 150 ) );
    setWidgetColor(descriptionEdit,QColor( 255, 255, 255 ) );
    descriptionEdit->setFrameShape( QTextEdit::NoFrame );
    descriptionEdit->setFrameShadow( QTextEdit::Plain );
    descriptionEdit->setReadOnly( TRUE );

    shapeShow = new QCanvasWidget( this);
    shapeShow->setGeometry( QRect( 10, 170, 74, 20));

#if QT_VERSION < 0x040000
    propertyGroup = new QGroupBox(2,Qt::Horizontal,"Properties", this, "propertyGroup" );
    propertyGroup->setGeometry( QRect( 10, 170, 74, 20) );
#else
    helpGroup= new QGroupBox(this);
    propertyGroup = new QGridLayout(helpGroup);
#endif

    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
 #if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( shapeShow, SIGNAL( doubleClicked(const QPoint &) ), this, SLOT( OnShapeDraw() ) );
    connect( shapeShow, SIGNAL( mouseMooved(const QPoint &) ), this, SLOT( mouseOnCanvas(const QPoint &) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ShapeDisplay::~ShapeDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ShapeDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Shape" ) );
    propertyGroup->setCaption( tr( "Properties" ) );
#else
    setWindowTitle( tr( "Shape" ) );
    helpGroup->setTitle( tr( "Properties" ) );
#endif
    nameLabel->setText( tr( "Name" ) );
    descLabel->setText( tr( "Description" ) );
}

