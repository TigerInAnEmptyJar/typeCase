#include "algorithmwidget.h"

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "algorithmdisplay.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#if QT_VERSION < 0x040000
#include <qlistview.h>
#else
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#endif
#include "algorithmwidget.ui.h"
/*
 *  Constructs a AlgorithmWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */

#if QT_VERSION < 0x040000
AlgorithmWidget::AlgorithmWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "AlgorithmWidget" );
#else
AlgorithmWidget::AlgorithmWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
	setObjectName( "AlgorithmWidget" );
#endif


    pdisplay = new AlgorithmDisplay( this);
    pdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );

#if QT_VERSION < 0x040000
    listView = new QListView( this);
    listView->addColumn( tr( "Algorithm" ) );
//     listView->addColumn( tr( "Value" ) );
//     listView->addColumn( tr( "Type" ) );
#else
    listView = new QTreeWidget( this);
    listView->setHeaderLabels(QStringList()<<"Algorithm");
#endif
    listView->setGeometry( QRect( 10, 200, 300, 210 ) );

    line = new QFrame( this);
    line->setGeometry( QRect( 130, 160, 200, 10 ) );
    line->setFrameShape( QFrame::VLine );
    line->setFrameShadow( QFrame::Sunken );
    line->setFrameShape( QFrame::VLine );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView, SIGNAL(spacePressed(QListViewItem *)), this, SLOT(algorithmChecked(QListViewItem*)));
    connect( listView, SIGNAL(currentChanged (QListViewItem *)), this, SLOT(onAlgorithmSelect(QListViewItem*)));
    connect( listView, SIGNAL(mouseButtonClicked (int,QListViewItem *,const QPoint&,int)), this, SLOT(algorithmClicked(int,QListViewItem*,const QPoint&,int)));
#else
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
//     connect( listView, SIGNAL(spacePressed(QTreeWidgetItem *)), this, SLOT(algorithmChecked(QTreeWidgetItem*)));
    connect( listView, SIGNAL(currentItemChanged (QTreeWidgetItem *,QTreeWidgetItem *)), this, SLOT(onAlgorithmSelect(QTreeWidgetItem*)));
    connect( listView, SIGNAL(itemClicked (QTreeWidgetItem *,int)), this, SLOT(algorithmClicked(QTreeWidgetItem*)));
#endif
    connect( pdisplay, SIGNAL(changed()), this, SIGNAL(changed()));
    init();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
AlgorithmWidget::~AlgorithmWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AlgorithmWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Algorithms" ) );
#else
    setWindowTitle( tr( "Algorithms" ) );
#endif
}

