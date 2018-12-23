#include "analysewidget.h"

#if QT_VERSION < 0x040000
#include <qheader.h>
#else
#include <QResizeEvent>
#endif
#include <qframe.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcanvaswidget.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "viewselectionwidget.h"
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "colorbutton.h"
#include "analysewidget.ui.h"
/*
 *  Constructs a AnalysisWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
AnalysisWidget::AnalysisWidget( QWidget* parent, const char* name, WFlags fl )
  : QWidget( parent, name, fl ), analyser(200)
{
    if ( !name )
	setName( "AnalysisWidget" );
#else
AnalysisWidget::AnalysisWidget( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName( "AnalysisWidget" );
#endif

    initButton=new QPushButton(this);//,"initB");
    startButton=new QPushButton(this);//,"startB");
    stepButton=new QPushButton(this);//,"stepB");
    stopButton=new QPushButton(this);//,"stopB");
    finalButton=new QPushButton(this);//,"finalB");
    helpButton=new QPushButton(this);//,"helpB");
    orderButton=new QPushButton(this);//,"orderB");
    displayButton=new QPushButton(this);//,"displayB");
    stateButton=new ColorButton(this);//,"colorB");
    eventsInput=new QLineEdit(this);//,"eventInput");
    eventsLabel=new QLabel(this);//,"eventLabel");
    allInput=new QCheckBox(this);//,"allInput");
    canvas=new QCanvasWidget(this);//,"canvas");

    order=new OrderSelectionWidget(NULL);
    watch=new ViewSelectionWidget(NULL);

    initButton->setGeometry(QRect(5,5,300,150));
    startButton->setGeometry(QRect(5,5,300,150));
    stepButton->setGeometry(QRect(5,5,300,150));
    stopButton->setGeometry(QRect(5,5,300,150));
    finalButton->setGeometry(QRect(5,5,300,150));
    helpButton->setGeometry(QRect(5,5,300,150));
    orderButton->setGeometry(QRect(5,5,300,150));
    displayButton->setGeometry(QRect(5,5,300,150));
    stateButton->setGeometry(QRect(5,5,300,150));
    stateButton->setReadOnly(true);
    eventsInput->setGeometry(QRect(5,5,300,150));
    eventsLabel->setGeometry(QRect(5,5,300,150));
    allInput->setGeometry(QRect(5,5,300,150));
    canvas->setGeometry(QRect(5,5,300,150));

    //order=->setGeometry(QRect(0,0,600,450));
    //watch=->setGeometry(QRect(0,0,600,450));
    languageChange();
    isRunning=false;
    isInitialized=false;

    // signals and slots connections
    order->setAlgorithms(selectedAlgorithms);
    connect(initButton,SIGNAL(clicked()),this,SLOT(initClick()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(startClick()));
    connect(stopButton,SIGNAL(clicked()),this,SLOT(stopClick()));
    connect(stepButton,SIGNAL(clicked()),this,SLOT(stepClick()));
    connect(finalButton,SIGNAL(clicked()),this,SLOT(finalClick()));
    connect(helpButton,SIGNAL(clicked()),this,SLOT(helpClick()));
    connect(orderButton,SIGNAL(clicked()),this,SLOT(orderClick()));
    connect(displayButton,SIGNAL(clicked()),this,SLOT(displayClick()));
    connect(order,SIGNAL(algorithmSelected(algorithm_parameter&, bool)),this,SIGNAL(algorithmSelect(algorithm_parameter&, bool)));
    connect(order,SIGNAL(helpRequested(const string &)),this,SIGNAL(helpRequested(const string &)));
    connect(watch,SIGNAL(helpRequested(const string &)),this,SIGNAL(helpRequested(const string &)));
    connect(canvas,SIGNAL(mouseWheeled(int,bool,const QPoint&)),this,SLOT(canvasScrolled(int, bool, const QPoint &)));
    connect(canvas,SIGNAL(doubleClicked(int,const QPoint&)),this,SLOT(canvasDoubleClicked(int, const QPoint &)));
    connect(canvas,SIGNAL(mouseMooved(int,const QPoint &)),this,SLOT(canvasMouseMove(int, const QPoint &)));
    setMinimumSize(150,25*8);
    init();
    resize( QSize(600, 400).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
AnalysisWidget::~AnalysisWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AnalysisWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Analysis" ) );
#else
    setWindowTitle(tr("Analysis" ) );
#endif
    initButton->setText( tr ("Init"));
    startButton->setText( tr ("Start"));
    stepButton->setText( tr ("Step"));
    stopButton->setText( tr ("Stop"));
    finalButton->setText( tr ("Final"));
    helpButton->setText( tr ("Help"));
    orderButton->setText( tr ("Algorithm"));
    displayButton->setText( tr ("Watch"));
    stateButton->setText( tr (" "));
    eventsInput->setText( tr ("1"));
    eventsLabel->setText( tr ("Events"));
    allInput->setText( tr ("all"));
}

