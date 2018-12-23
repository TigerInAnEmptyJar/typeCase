#include "shapewidget.h"

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "shapedisplay.h"

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
#include "shapewidget.ui.h"
/*
 *  Constructs a ShapeWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
ShapeWidget::ShapeWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ShapeWidget" );

    listView = new QListView( this);
    listView->addColumn( tr( "Shape" ) );
#else
ShapeWidget::ShapeWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
     setObjectName( "ShapeWidget" );

    listView = new QTreeWidget( this);
    listView->setHeaderLabels(QStringList()<< tr( "Shape" ) );
#endif


    pdisplay = new ShapeDisplay( this);
    pdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );

    listView->setGeometry( QRect( 10, 200, 300, 210 ) );

    line = new QFrame( this);
    line->setGeometry( QRect( 130, 160, 200, 10 ) );
    line->setFrameShape( QFrame::VLine );
    line->setFrameShadow( QFrame::Sunken );
    line->setFrameShape( QFrame::VLine );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView, SIGNAL(currentChanged (QListViewItem *)), this, SLOT(onShapeSelect(QListViewItem*)));
#else
    connect( listView, SIGNAL(currentItemChanged (QTreeWidgetItem *,QTreeWidgetItem *)), this, SLOT(onShapeSelect(QTreeWidgetItem*)));
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
ShapeWidget::~ShapeWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ShapeWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Shapes" ) );
#else
    setWindowTitle( tr( "Shapes" ) );
#endif
}

