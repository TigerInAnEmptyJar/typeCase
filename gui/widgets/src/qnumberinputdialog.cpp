/****************************************************************************
** Form implementation generated from reading ui file 'qnumberinputdialog.ui'
**
** Created: Fri Dec 7 16:01:11 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "qnumberinputdialog.h"

#if QT_VERSION > 0x040000
#include <QResizeEvent>
#include <QDialogButtonBox>
#endif
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "qnumberinputdialog.ui.h"

/*
 *  Constructs a QNumberInputDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
QNumberInputDialog::QNumberInputDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "QNumberInputDialog" );
#else
QNumberInputDialog::QNumberInputDialog( QWidget* parent,  Qt::WindowFlags f)
    : QDialog( parent, f )
{
  setObjectName( "QIntegerInputDialog" );
#endif
    setSizeGripEnabled( TRUE );

    lineEdit = new QLineEdit( this);
    lineEdit->setGeometry( QRect( 10, 10, 300, 27 ) );
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
    layout->addMultiCellWidget(lineEdit,0,0,0,3);
#else
    layout=new QGridLayout(this);
    buttons=new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Help);
    layout->addWidget(buttons,1,0);
    layout->addWidget(lineEdit,0,0);
#endif
    layout->setMargin(10);
    layout->setSpacing(10);
    languageChange();
    resize( QSize(325, 99).expandedTo(minimumSizeHint()) );
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
}

/*
 *  Destroys the object and frees any allocated resources
 */
QNumberInputDialog::~QNumberInputDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QNumberInputDialog::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "MyDialog" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
#else
    setWindowTitle( tr( "MyDialog" ) );
//     buttonCancel->setShortcut( QKeySequence( QString::null ) );
//     buttonHelp->setShortcut( QKeySequence( tr( "F1" ) ) );
//     buttonOk->setShortcut( QKeySequence( QString::null ) );
#endif
}

