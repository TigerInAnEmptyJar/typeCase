#include "setupwidget.h"

#include <qframe.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "reactiondisplay.h"
#include "detectorwidget.h"
#include "materialwidget.h"
#include "shapewidget.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "setupwidget.ui.h"
/*
 *  Constructs a SetupWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
SetupWidget::SetupWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "SetupWidget" );
#else
SetupWidget::SetupWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
     setObjectName( "SetupWidget" );
#endif
    tabWidget=new QTabWidget(this);
    tabWidget->setGeometry(QRect(5,5,300,150));
    detwidget = new DetectorWidget( tabWidget);
    detwidget->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(detwidget,"Detectors");
    matwidget = new MaterialWidget( tabWidget);
    matwidget->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(matwidget,"Materials");
    rdisplay = new ReactionDisplay( tabWidget);
    rdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(rdisplay,"Experiment");
    shpwidget = new ShapeWidget( tabWidget);
    shpwidget->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(shpwidget,"Shapes");
    languageChange();
    detwidget->setReadOnly(false);
    matwidget->setReadOnly(false);
    shpwidget->setReadOnly(false);
    rdisplay->setReadOnly(false);

    // signals and slots connections
//     connect(tabWidget,SIGNAL(currentChanged ( QWidget * )),this,SLOT(tabPageChanged()));
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
SetupWidget::~SetupWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SetupWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Setup-parameter" ) );
#else
    setWindowTitle( tr( "Setup-parameter" ) );
#endif
}

