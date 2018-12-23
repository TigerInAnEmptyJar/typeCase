#include "orderselectionwidget.h"

#if QT_VERSION < 0x040000
#include <qheader.h>
#else
#include <QResizeEvent>
#endif
#include <qframe.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "algorithmorderwidget.h"
#include <qcombobox.h>
#include "stackwidget.h"
#include "reactionrecognitionwidget.h"
#include "algorithmparameter.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "orderselectionwidget.ui.h"
/*
 *  Constructs a OrderSelectionWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
OrderSelectionWidget::OrderSelectionWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "OrderSelectionWidget" );
#else
OrderSelectionWidget::OrderSelectionWidget( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName( "OrderSelectionWidget" );
#endif

  tabWidget=new QTabWidget(this);//,"tabWidget");
  tabWidget->setGeometry(QRect(5,5,300,150));

    order = new AlgorithmOrderWidget( tabWidget);//, "orderWidget");
    order->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(order,"Algorithms");

    inStack = new StackWidget( tabWidget);//, "inWidget" );
    inStack->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(inStack,"Input Algorithms");

    outStack = new StackWidget( tabWidget);//, "outWidget" );
    outStack->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(outStack,"Output Algorithms");

    reactions = new ReactionRecognitionWidget( tabWidget);//, "reactions" );
    reactions->setGeometry( QRect( 5, 5, 300, 150 ) );
    tabWidget->addTab(reactions,"Reaction Recognition");
    connect(reactions,SIGNAL(insertClick(algorithm_parameter*,bool)),order,SLOT(algorithmAdd(algorithm_parameter *)));
    connect(reactions,SIGNAL(helpRequested(const string&)),this,SIGNAL(helpRequested(const string&)));

    languageChange();

    // signals and slots connections
//     connect(tabWidget,SIGNAL(currentChanged ( QWidget * )),this,SLOT(tabPageChanged()));
    init();
    resize( QSize(500, 400).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
OrderSelectionWidget::~OrderSelectionWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void OrderSelectionWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "OrderSelection-parameter" ) );
#else
    setWindowTitle(tr("OrderSelection-parameter" ) );
#endif
}

