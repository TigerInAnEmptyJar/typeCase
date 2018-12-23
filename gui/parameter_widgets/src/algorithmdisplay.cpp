/****************************************************************************
** Form implementation generated from reading ui file 'algorithmdisplay.ui'
**
** Created: Mon Nov 30 16:00:28 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "algorithmdisplay.h"

#if QT_VERSION < 0x040000
#include <qvariant.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlistview.h>
#else
#include <QTreeWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#endif
#include "utilities.h"
#include <qlabel.h>
#include <qtextedit.h>
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
#include "algorithmdisplay.ui.h"
/*
 *  Constructs a AlgorithmDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
AlgorithmDisplay::AlgorithmDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "AlgorithmDisplay" );
#else
AlgorithmDisplay::AlgorithmDisplay( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
{
  setObjectName( "AlgorithmDisplay" );
#endif
    textLabel1 = new QLabel( this );
    textLabel1->setGeometry( QRect( 10, 170, 74, 20 ) );

    textEdit1 = new QTextEdit( this );
    textEdit1->setGeometry( QRect( 5, 5, 300, 150 ) );
    setWidgetColor(textEdit1, QColor( 255, 255, 255 ) );
    textEdit1->setFrameShape( QTextEdit::NoFrame );
    textEdit1->setFrameShadow( QTextEdit::Plain );
    textEdit1->setReadOnly( TRUE );

#if QT_VERSION < 0x040000
    listView1 = new QListView( this );
    listView1->addColumn( tr( "Name" ) );
    listView1->addColumn( tr( "Value" ) );
    listView1->addColumn( tr( "Type" ) );
#else
    listView1 = new QTreeWidget( this );
    QStringList hdrs;hdrs<<"Name"<<"Value"<<"Type";
    listView1->setHeaderLabels(hdrs);
#endif
    listView1->setGeometry( QRect( 10, 200, 300, 210 ) );

    line1 = new QFrame( this );
    line1->setGeometry( QRect( 130, 160, 200, 10 ) );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView1, SIGNAL( rightButtonClicked(QListViewItem*,const QPoint&,int) ), this, SLOT( OnParameterRightClick(QListViewItem*) ) );
#else
    listView1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( listView1, SIGNAL( customContextMenuRequested ( const QPoint & )), this, SLOT( OnParameterRightClick(const QPoint &) ) );
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AlgorithmDisplay::~AlgorithmDisplay()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AlgorithmDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Algorithm" ) );
    listView1->header()->setLabel( 0, tr( "Name" ) );
    listView1->header()->setLabel( 1, tr( "Value" ) );
    listView1->header()->setLabel( 2, tr( "Type" ) );
#else
    setWindowTitle( tr( "Algorithm" ) );
#endif
    textLabel1->setText( tr( "Parameters" ) );
}

