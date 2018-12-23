/****************************************************************************
** Form implementation generated from reading ui file 'qintegerinputdialog.ui'
**
** Created: Fri Dec 7 16:01:10 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "qintegerinputdialog.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#include <QDialogButtonBox>
#endif

#include <qvariant.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "qintegerinputdialog.ui.h"
/*
 *  Constructs a QIntegerInputDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
QIntegerInputDialog::QIntegerInputDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "QIntegerInputDialog" );
#else
QIntegerInputDialog::QIntegerInputDialog( QWidget* parent,  Qt::WindowFlags f)
    : QDialog( parent, f )
{
  setObjectName( "QIntegerInputDialog" );
#endif
    setSizeGripEnabled( TRUE );

    spinBox5 = new QSpinBox( this );
    spinBox5->setGeometry( QRect( 20, 10, 385, 27 ) );
    spinBox5->setRange(-2000000,2000000);
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

    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout->addWidget( buttonHelp ,1,0);
    layout->addItem( Horizontal_Spacing2 ,1,1);
    layout->addWidget( buttonOk ,1,2);
    layout->addWidget( buttonCancel ,1,3);
    layout->addMultiCellWidget(spinBox5,0,0,0,3);
#else
    layout=new QGridLayout(this);
    buttons=new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Help);
    layout->addWidget(buttons,1,0);
    layout->addWidget(spinBox5,0,0);
#endif
    layout->setMargin(10);
    layout->setSpacing(10);

    languageChange();
    resize( QSize(426, 97).expandedTo(minimumSizeHint()) );
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
    //    connect(buttons,SIGNAL(helpRequested()),this,SIGNAL(helpRequested()));
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
QIntegerInputDialog::~QIntegerInputDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QIntegerInputDialog::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "MyDialog" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
#else
    setWindowTitle( tr( "MyDialog" ) );
#endif
}

