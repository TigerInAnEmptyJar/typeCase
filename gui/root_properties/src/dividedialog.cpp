/****************************************************************************
** Form implementation generated from reading ui file 'dividedialog.ui'
**
** Created: Tue Feb 6 10:54:44 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "dividedialog.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif

#include <qvariant.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "dividedialog.ui.h"
/*
 *  Constructs a DivideDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
DivideDialog::DivideDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "DivideDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
 #else
DivideDialog::DivideDialog( QWidget* parent, Qt::WindowFlags f)
    : QDialog( parent, f )
{
  setObjectName( "DivideDialog" );
  setSizePolicy( QSizePolicy( (QSizePolicy::Policy)5, (QSizePolicy::Policy)5));//, 0, 0, sizePolicy().hasHeightForWidth() ) );

 #endif
  setSizeGripEnabled( TRUE );
    setModal( TRUE );

    textLabel1 = new QLabel( this);
    textLabel1->setGeometry( QRect( 20, 30, 127, 20 ) );

    textLabel2 = new QLabel( this );
    textLabel2->setGeometry( QRect( 20, 100, 120, 20 ) );

    spinBox2 = new QSpinBox( this );
    spinBox2->setGeometry( QRect( 30, 130, 110, 23 ) );

    spinBox1 = new QSpinBox( this);
    spinBox1->setGeometry( QRect( 30, 60, 110, 23 ) );
#if QT_VERSION < 0x040000
    spinBox1->setMaxValue( 10 );
    spinBox1->setMinValue( 1 );
    spinBox2->setMaxValue( 19 );
    spinBox2->setMinValue( 1 );
#else
    spinBox1->setMaximum( 10 );
    spinBox1->setMinimum( 1 );
    spinBox2->setMaximum( 19 );
    spinBox2->setMinimum( 1 );
#endif

    QWidget* privateLayoutWidget = new QWidget( this);
    privateLayoutWidget->setGeometry( QRect( 10, 170, 300, 33 ) );
#if QT_VERSION < 0x040000
    Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6); 
#else
    Layout1 = new QHBoxLayout( privateLayoutWidget); 
#endif

    buttonHelp = new QPushButton( privateLayoutWidget );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( privateLayoutWidget );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( privateLayoutWidget);
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    languageChange();
    resize( QSize(330, 219).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
DivideDialog::~DivideDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DivideDialog::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Divide Canvas" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
#else
    buttonHelp->setShortcut(QKeySequence( tr( "F1" ) ) );
#endif
    textLabel1->setText( tr( "Number of Columns" ) );
    textLabel2->setText( tr( "Number of Rows" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
}

