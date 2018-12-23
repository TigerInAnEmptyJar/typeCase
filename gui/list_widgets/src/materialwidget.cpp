#include "materialwidget.h"

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
#include "materialwidget.ui.h"
/*
 *  Constructs a MaterialWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
MaterialWidget::MaterialWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "MaterialWidget" );

    listView = new QListView( this);
    listView->addColumn( tr( "Material" ) );
#else
MaterialWidget::MaterialWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
     setObjectName( "MaterialWidget" );
     listView = new QTreeWidget( this);
     listView->setHeaderLabels(QStringList()<< tr("Material" ) );
#endif


    pdisplay = new MaterialDisplay( this);
    pdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );
//     listView->addColumn( tr( "Value" ) );
//     listView->addColumn( tr( "Type" ) );
    listView->setGeometry( QRect( 10, 200, 300, 210 ) );

    line = new QFrame( this);
    line->setGeometry( QRect( 130, 160, 200, 10 ) );
    line->setFrameShape( QFrame::VLine );
    line->setFrameShadow( QFrame::Sunken );
    line->setFrameShape( QFrame::VLine );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView, SIGNAL(currentChanged (QListViewItem *)), this, SLOT(onMaterialSelect(QListViewItem*)));
    connect( listView, SIGNAL(rightButtonClicked (QListViewItem *,const QPoint&,int)), this, SLOT(materialRightClicked(QListViewItem*,const QPoint&)));
#else
    connect( listView, SIGNAL(currentItemChanged (QTreeWidgetItem *,QTreeWidgetItem *)), this, SLOT(onMaterialSelect(QTreeWidgetItem *)));
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( listView, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(materialRightClicked(const QPoint&)));
#endif
    connect( pdisplay, SIGNAL(changed()), this, SLOT(materialChanged()));
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
MaterialWidget::~MaterialWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MaterialWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Materials" ) );
#else
    setWindowTitle( tr( "Materials" ) );
#endif
}

