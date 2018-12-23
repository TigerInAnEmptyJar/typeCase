/****************************************************************************
** Form implementation generated from reading ui file 'q3dinputdialog.ui'
**
** Created: Fri Dec 7 16:00:11 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "q3dinputdialog.h"

#include <qvariant.h>
#include <q3Dinput.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "geometry.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#include <QDialogButtonBox>
#endif
// #include <qgridlayout.h>
#include "q3dinputdialog.ui.h"

/*
 *  Constructs a Q3DInputDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
Q3DInputDialog::Q3DInputDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Q3DInputDialog" );
#else
Q3DInputDialog::Q3DInputDialog( QWidget* parent,Qt::WindowFlags f )
    : QDialog( parent, f )
{
	setObjectName( "Q3DInputDialog" );
#endif
    setSizeGripEnabled( TRUE );

    q3DInput1 = new Q3DInput( this);
    q3DInput1->setGeometry( QRect( 10, 10, 480, 41 ) );

#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,2,4);
    buttonHelp = new QPushButton( this);
    buttonHelp->setAutoDefault( TRUE );

    buttonOk = new QPushButton( this);
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    buttonOk->resize(20,23);

    buttonCancel = new QPushButton( this);
    buttonCancel->setAutoDefault( TRUE );

    layout->addWidget( buttonHelp ,1,0);
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout->addItem( Horizontal_Spacing2 ,1,1);
    layout->addWidget( buttonOk ,1,2);
    layout->addWidget( buttonCancel ,1,3);
    layout->addMultiCellWidget(q3DInput1,0,0,0,3,Qt::AlignJustify);
#else
    layout=new QGridLayout(this);
    buttons=new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Help);
    layout->addWidget(buttons,1,0);
    layout->addWidget(q3DInput1,0,0);
#endif
    layout->setMargin(10);
    layout->setSpacing(10);

    languageChange();
    resize( QSize(500, 108).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
#else
    connect(buttons,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(reject()));
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
Q3DInputDialog::~Q3DInputDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Q3DInputDialog::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "point/vector" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
#else
    setWindowTitle( tr( "point/vector" ) );
#endif
}

