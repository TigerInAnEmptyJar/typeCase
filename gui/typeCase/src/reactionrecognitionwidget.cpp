/****************************************************************************
** Form implementation generated from reading ui file 'reactionrecognitionwidget.ui'
**
** Created: Mon Nov 30 16:00:11 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "reactionrecognitionwidget.h"

#include <qvariant.h>
#include <fstream>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#if QT_VERSION < 0x040000
#include <qheader.h>
#include <qlistview.h>
#include <qpopupmenu.h>
#else
#include <QResizeEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#endif
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "directories.h"
#include "qintegerinputdialog.h"
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "reactionrecognitionwidget.ui.h"
/*
 *  Constructs a ReactionRecognitionWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
ReactionRecognitionWidget::ReactionRecognitionWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ReactionRecognitionWidget" );
    listView1 = new QListView( this);//, "listView1" );
    listView1->addColumn( tr( "Algorithm" ) );
    listView1->addColumn( tr( "ID" ) );
#else
ReactionRecognitionWidget::ReactionRecognitionWidget( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
   setObjectName( "ReactionRecognitionWidget" );
    listView1 = new QTreeWidget( this);//, "listView1" );
    listView1->setHeaderLabels(QStringList()<<tr( "Algorithm" ) << tr( "ID" ) );
#endif

   textLabel2 = new QLabel( this);//, "textLabel2" );
    textLabel2->setGeometry( QRect( 250, 10, 73, 20 ) );

    pushButton1 = new QPushButton( this);//, "pushButton1" );
    pushButton1->setGeometry( QRect( 90, 440, 111, 23 ) );

    listView1->setGeometry( QRect( 10, 10, 210, 420 ) );
    languageChange();
    init();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( OnInsertClick() ) );
#if QT_VERSION < 0x040000
    connect( listView1, SIGNAL( rightButtonClicked(QListViewItem*,const QPoint&,int) ), this, SLOT( OnReactionRightClick(QListViewItem*,const QPoint&) ) );
    connect( listView1, SIGNAL( clicked(QListViewItem*) ), this, SLOT( OnReactionSelect(QListViewItem*) ) );
    clearWState( WState_Polished );
#else
    listView1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( listView1, SIGNAL( customContextMenuRequested (const QPoint &)), this, SLOT( OnReactionRightClick(const QPoint&) ) );
    connect( listView1, SIGNAL( itemClicked(QTreeWidgetItem*,int) ), this, SLOT( OnReactionSelect(QTreeWidgetItem*) ) );
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
ReactionRecognitionWidget::~ReactionRecognitionWidget()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ReactionRecognitionWidget::languageChange()
{
 #if QT_VERSION < 0x040000
    setCaption( tr( "reactionRecognition" ) );
    listView1->header()->setLabel( 0, tr( "Algorithm" ) );
    listView1->header()->setLabel( 1, tr( "ID" ) );
#else
    setWindowTitle(tr("reactionRecognition" ) );
    listView1->setHeaderLabels(QStringList()<<tr( "Algorithm" ) << tr( "ID" ) );
#endif
    textLabel2->setText( tr( "Description" ) );
    pushButton1->setText( tr( "insert" ) );
}

