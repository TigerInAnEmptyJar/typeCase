/****************************************************************************
** Form implementation generated from reading ui file 'materialdisplay.ui'
**
** Created: Mon Nov 30 16:00:40 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "materialdisplay.h"

#if QT_VERSION < 0x040000
#include <qlistview.h>
#include <qvariant.h>
#include <qheader.h>
#else
#include <QResizeEvent>
#include <QTreeWidget>
#include <QGridLayout>
#endif
#include <qframe.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "qnumberinputdialog.h"
#include "elementdisplay.h"
#include "Eparticles.h"
#include "materialdisplay.ui.h"
/*
 *  Constructs a MaterialDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
MaterialDisplay::MaterialDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "MaterialDisplay" );
    listView1 = new QListView( this);
    listView1->addColumn( tr( "Element" ) );
    listView1->addColumn( tr( "Mass" ) );
    listView1->addColumn( tr( "Charge" ) );
    listView1->addColumn( tr( "Weight" ) );
#else
    MaterialDisplay::MaterialDisplay( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent,f )
{
	setObjectName( "MaterialDisplay" );
    listView1 = new QTreeWidget( this);
    QStringList tmpss;tmpss<<tr( "Element" )<<tr( "Mass" )<<tr( "Charge" )<<tr( "Weight" );
    listView1->setHeaderLabels(tmpss);
#endif
    listView1->setGeometry( QRect( 320, 10, 230, 290 ) );

    frame1 = new QFrame( this);
    frame1->setGeometry( QRect( 5, 5, 260, 105 ) );
    frame1->setFrameShape( QFrame::StyledPanel );
    frame1->setFrameShadow( QFrame::Raised );

    textLabel1 = new QLabel( frame1);
    textLabel1->setGeometry( QRect( 10, 5, 58, 20 ) );

    textLabel2 = new QLabel( frame1);
    textLabel2->setGeometry( QRect( 10, 30, 96, 20 ) );

    checkBox1 = new QCheckBox( frame1);
    checkBox1->setGeometry( QRect( 10, 80, 110, 20 ) );

    textLabel3 = new QLabel( frame1);
    textLabel3->setGeometry( QRect( 10, 55, 83, 20 ) );

    lineEdit1 = new QLineEdit( frame1);
    lineEdit1->setGeometry( QRect( 80, 5, 120, 21 ) );

    lineEdit2 = new QLineEdit( frame1);
    lineEdit2->setGeometry( QRect( 110, 30, 89, 21 ) );

    lineEdit3 = new QLineEdit( frame1);
    lineEdit3->setGeometry( QRect( 110, 55, 89, 21 ) );

    textEdit1 = new QTextEdit( this);
    textEdit1->setGeometry( QRect( 5, 115, 260, 160 ) );
    listView1->setMinimumSize(QSize(100,50));
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,2,2);
    QGridLayout *tmpLayout=new QGridLayout(frame1,4,3);
    tmpLayout->addMultiCellWidget(textLabel1,0,0,0,0);
    tmpLayout->addMultiCellWidget(lineEdit1,0,0,1,2);
    tmpLayout->addMultiCellWidget(textLabel2,1,1,0,1);
    tmpLayout->addMultiCellWidget(lineEdit2,1,1,2,2);
    tmpLayout->addMultiCellWidget(textLabel3,2,2,0,1);
    tmpLayout->addMultiCellWidget(lineEdit3,2,2,2,2);
    tmpLayout->addMultiCellWidget(checkBox1,3,3,0,2);
    layout->addMultiCellWidget(frame1,0,0,0,0);
    layout->addMultiCellWidget(textEdit1,1,1,0,0);
    layout->addMultiCellWidget(listView1,0,1,1,1);
    layout->setColStretch(0,3);
    layout->setColStretch(1,4);
#else
    layout=new QGridLayout(this);
    QGridLayout *tmpLayout=new QGridLayout(frame1);
    tmpLayout->addWidget(textLabel1,0,0,1,1);
    tmpLayout->addWidget(lineEdit1,0,1,1,2);
    tmpLayout->addWidget(textLabel2,1,0,1,2);
    tmpLayout->addWidget(lineEdit2,1,2,1,1);
    tmpLayout->addWidget(textLabel3,2,0,1,2);
    tmpLayout->addWidget(lineEdit3,2,2,1,1);
    tmpLayout->addWidget(checkBox1,3,0,1,3);
    frame1->setLayout(tmpLayout);
    layout->addWidget(frame1,0,0,1,1);
    layout->addWidget(textEdit1,1,0,1,1);
    layout->addWidget(listView1,0,1,2,1);
    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1,4);
    setLayout(layout);
#endif
    layout->setMargin(0);
    layout->setSpacing(5);
    tmpLayout->setMargin(2);
    tmpLayout->setSpacing(5);

    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView1, SIGNAL( rightButtonClicked(QListViewItem*,const QPoint&,int) ), this, SLOT( OnElementsRightClick(QListViewItem*,const QPoint&) ) );
#else
    listView1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( listView1, SIGNAL( customContextMenuRequested ( const QPoint & ) ), this, SLOT( OnElementsRightClick(const QPoint&) ) );
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MaterialDisplay::~MaterialDisplay()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MaterialDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Material" ) );
    listView1->header()->setLabel( 0, tr( "Element" ) );
    listView1->header()->setLabel( 1, tr( "Mass" ) );
    listView1->header()->setLabel( 2, tr( "Charge" ) );
    listView1->header()->setLabel( 3, tr( "Weight" ) );
#else
    setWindowTitle( tr( "Material" ) );

#endif
    textLabel1->setText( tr( "Name" ) );
    textLabel2->setText( tr( "Speed of Light [c]" ) );
    checkBox1->setText( tr( "Active material" ) );
    textLabel3->setText( trUtf8( "\x44\x65\x6e\x73\x69\x74\x79\x20\x5b\x67\x2f\x63\x6d\xc2\xb3\x5d" ) );
}

