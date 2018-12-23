#include "databasiswidget.h"

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "materialdisplay.h"

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
#include "databasiswidget.ui.h"
/*
 *  Constructs a DataBasisWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
DataBasisWidget::DataBasisWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "DataBasisWidget" );
    listView = new QListView( this);
    listView->addColumn( tr( "DataBasis" ) );
#else
DataBasisWidget::DataBasisWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
     setObjectName( "DataBasisWidget" );
     listView = new QTreeWidget( this);
     listView->setHeaderLabels(QStringList()<< tr("DataBasis" ) );
#endif


    listView->setGeometry( QRect( 10, 200, 300, 210 ) );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
     connect( listView, SIGNAL(spacePressed(QListViewItem *)), this, SLOT(dataBasisChecked(QListViewItem*)));
    connect( listView, SIGNAL(mouseButtonClicked (int,QListViewItem *,const QPoint&,int)), this, SLOT(dataBasisClicked(int,QListViewItem*,const QPoint&,int)));
    //    connect( listView, SIGNAL(currentChanged (QListViewItem *)), this, SLOT(onDataBasisSelect(QListViewItem*)));
    connect( listView, SIGNAL(rightButtonClicked (QListViewItem *,const QPoint&,int)), this, SLOT(dataBasisRightClicked(QListViewItem*,const QPoint&)));
#else
    connect( listView, SIGNAL(itemChanged (QTreeWidgetItem *,int)), this, SLOT(dataBasisChecked(QTreeWidgetItem*)));
    connect( listView, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(dataBasisClicked(QTreeWidgetItem*)));
    //    connect( listView, SIGNAL(currentChanged (QTreeWidgetItem *)), this, SLOT(onDataBasisSelect(QTreeWidgetItem*)));
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( listView, SIGNAL(customContextMenuRequested ( const QPoint & )), this, SLOT(dataBasisRightClicked(const QPoint&)));
#endif
//     connect( pdisplay, SIGNAL(changed()), this, SLOT(dataBasisChanged()));
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
DataBasisWidget::~DataBasisWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DataBasisWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "DataBasis" ) );
#else
    setWindowTitle( tr( "DataBasis" ) );
#endif
}

