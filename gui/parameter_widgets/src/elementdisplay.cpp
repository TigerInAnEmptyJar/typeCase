#include "elementdisplay.h"

#include <qvariant.h>
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
#include "elementdisplay.ui.h"
/*
 *  Constructs a ElementDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
ElementDisplay::ElementDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ElementDisplay" );
#else
ElementDisplay::ElementDisplay( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName( "ElementDisplay" );
#endif
    nameLabel = new QLabel( this);
    nameLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    nameInput = new QLineEdit(this);
    nameInput->setGeometry( QRect( 10, 170, 74, 20 ) );
    massLabel = new QLabel( this);
    massLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    massInput = new QLineEdit(this);
    massInput->setGeometry( QRect( 10, 170, 74, 20 ) );
    chargeLabel = new QLabel( this);
    chargeLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    chargeInput = new QLineEdit(this);
    chargeInput->setGeometry( QRect( 10, 170, 74, 20 ) );
    weightLabel = new QLabel( this);
    weightLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    weightInput = new QLineEdit(this);
    weightInput->setGeometry( QRect( 10, 170, 74, 20 ) );
    setMinimumSize(100,23*4+4);
    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ElementDisplay::~ElementDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ElementDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Element" ) );
#else
    setWindowTitle(tr("Element"));
#endif

    nameLabel->setText( tr( "Name" ) );
    massLabel->setText( tr( "Mass" ) );
    chargeLabel->setText( tr( "Charge" ) );
    weightLabel->setText( tr( "Weight" ) );
}

