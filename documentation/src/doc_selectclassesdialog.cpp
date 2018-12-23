/****************************************************************************
** Form implementation generated from reading ui file 'doc_selectclassesdialog.ui'
**
** Created: Wed Jul 2 15:50:38 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "doc_selectclassesdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QListWidget>
#include <QDialogButtonBox>
#endif
#include "utilities.h"
#include "string_utilities.h"
#include "doc_selectclassesdialog.ui.h"
/*
 *  Constructs a doc_selectClassesDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
doc_selectClassesDialog::doc_selectClassesDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "doc_selectClassesDialog" );
#else
doc_selectClassesDialog::doc_selectClassesDialog( QWidget* parent,Qt::WindowFlags fl )
    : QDialog( parent, fl )
{
    setObjectName( "doc_selectClassesDialog" );
#endif
    setSizeGripEnabled( TRUE );

    flayout = new QGridLayout( this); 

#if QT_VERSION < 0x040000
    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 10, 380, 476, 33 ) );
    Layout1 = new QHBoxLayout(privateLayoutWidget); 
    buttonHelp = new QPushButton( privateLayoutWidget, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( privateLayoutWidget, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );
    flayout->addMultiCellWidget(privateLayoutWidget,6,6,0,2);
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
#else
    buttons=new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->addButton(QDialogButtonBox::Help);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    flayout->addWidget(buttons,6,0,1,3);
    connect(buttons,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(reject()));
#endif
    textLabel1 = new QLabel( this);
    textLabel2 = new QLabel( this);
    toolButton2 = new QToolButton( this);
    toolButton1 = new QToolButton( this);
    flayout->addWidget(textLabel1,0,0);
    flayout->addWidget(textLabel2,0,2);
    flayout->addWidget(toolButton1,2,1);
    flayout->addWidget(toolButton2,4,1);
#if QT_VERSION < 0x040000
    listBox1 = new QListBox( this, "listBox1" );
    listBox2 = new QListBox( this, "listBox2" );
    flayout->addMultiCellWidget(listBox1,1,5,0,0);
    flayout->addMultiCellWidget(listBox2,1,5,2,2);
    toolButton1->setIconSet( QIconSet( QPixmap::fromMimeSource( "undo" ) ) );
    toolButton2->setIconSet( QIconSet( QPixmap::fromMimeSource( "redo" ) ) );
#else
    listBox1 = new QListWidget( this);
    listBox2 = new QListWidget( this);
    flayout->addWidget(listBox1,1,0,5,1);
    flayout->addWidget(listBox2,1,2,5,1);
    toolButton1->setIcon( QIcon( (string(getenv("KTOFPACK"))+"/documentation/images/undo").data()));//QPixmap::fromMimeSource( "undo" ) ) );
    toolButton2->setIcon( QIcon( (string(getenv("KTOFPACK"))+"/documentation/images/redo").data()));//QPixmap::fromMimeSource( "redo" ) ) );
#endif

//     listBox1->setGeometry( QRect( 10, 30, 220, 340 ) );
//     listBox2->setGeometry( QRect( 270, 30, 230, 340 ) );
//     textLabel1->setGeometry( QRect( 10, 0, 58, 20 ) );
//     textLabel2->setGeometry( QRect( 270, 0, 58, 20 ) );
//     toolButton1->setGeometry( QRect( 240, 120, 20, 20 ) );
//     toolButton2->setGeometry( QRect( 240, 90, 20, 20 ) );
    languageChange();
    resize( QSize(511, 423).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( toolButton1, SIGNAL( clicked() ), this, SLOT( OnUnUseButtonClick() ) );
    connect( toolButton2, SIGNAL( clicked() ), this, SLOT( OnUseButtonClick() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
doc_selectClassesDialog::~doc_selectClassesDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void doc_selectClassesDialog::languageChange()
{
    textLabel1->setText( tr( "Available" ) );
    textLabel2->setText( tr( "Document" ) );
    toolButton2->setText( QString::null );
    toolButton1->setText( QString::null );
#if QT_VERSION < 0x040000
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    setCaption( tr( "MyDialog" ) );
#else
    setWindowTitle( tr( "MyDialog" ) );
#endif
}

