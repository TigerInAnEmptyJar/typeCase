#include "detectorwidget.h"

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "detectordisplay.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"

#include "qcanvaswidget.h"
#include "volumeShape.h"
#include <TObject.h>
#include <TPolyLine.h>
#include <TCutG.h>
#if QT_VERSION < 0x040000
#include <qlistview.h>
#else
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#endif
#include "utilities.h"
#include "detectorwidget.ui.h"
/*
 *  Constructs a DetectorWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
DetectorWidget::DetectorWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "DetectorWidget" );
    listView = new QListView( this);
    listView->addColumn( tr( "Detector" ) );
#else
DetectorWidget::DetectorWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
     setObjectName( "DetectorWidget" );
    listView = new QTreeWidget( this);
    listView->setHeaderLabels(QStringList()<< tr( "Detector" ) );
#endif

    pdisplay = new DetectorDisplay( this);
    pdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );

    listView->setGeometry( QRect( 10, 200, 300, 210 ) );

    line = new QFrame( this);
    line->setGeometry( QRect( 130, 160, 200, 10 ) );
    line->setFrameShape( QFrame::VLine );
    line->setFrameShadow( QFrame::Sunken );
    line->setFrameShape( QFrame::VLine );
    canvas= new QCanvasWidget(this);
    canvas->setGeometry( QRect( 130, 160, 200, 10 ) );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
    connect( listView, SIGNAL(currentChanged (QListViewItem *)), this, SLOT(onDetectorSelect(QListViewItem*)));
    connect( listView, SIGNAL(clicked (QListViewItem *)), this, SLOT(onDetectorSelect(QListViewItem*)));
    connect( listView, SIGNAL(spacePressed(QListViewItem *)), this, SLOT(drawDetectors()));
    connect( listView, SIGNAL(mouseButtonClicked (int,QListViewItem *,const QPoint&,int)), this, SLOT(drawDetectors()));
#else
    connect( listView, SIGNAL(currentItemChanged (QTreeWidgetItem *,QTreeWidgetItem *)), this, SLOT(onDetectorSelect(QTreeWidgetItem*,QTreeWidgetItem *)));
    //    connect( listView, SIGNAL(itemClicked (QTreeWidgetItem *,int)), this, SLOT(onDetectorSelect(QTreeWidgetItem*)));
    connect( listView, SIGNAL(itemChanged (QTreeWidgetItem *,int)), this, SLOT(drawDetectors()));
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
#endif
    connect( pdisplay, SIGNAL(changed()), this, SLOT(detectorChanged()));
    connect( canvas, SIGNAL(doubleClicked(const QPoint &)), this, SLOT(drawDetectors()));
    connect( canvas, SIGNAL(mouseMooved(const QPoint&)), this, SLOT(moveOnCanvas(const QPoint &)));
    connect(canvas,SIGNAL(rightMouseButtonClick(const QPoint &)),this,SLOT(canvasRightClick(const QPoint&)));
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
DetectorWidget::~DetectorWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DetectorWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Detectors" ) );
#else
    setWindowTitle( tr( "Detectors" ) );
#endif
}

