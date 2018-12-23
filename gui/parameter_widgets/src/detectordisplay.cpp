#include "detectordisplay.h"

#include <TCutG.h>
#include <TPolyLine.h>
#include <qvariant.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qcanvaswidget.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "shapedisplay.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#else
#include <qheader.h>
#endif

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "baseShape.h"
#include "detectordisplay.ui.h"
/*
 *  Constructs a DetectorDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
DetectorDisplay::DetectorDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "DetectorDisplay" );
#else
    DetectorDisplay::DetectorDisplay( QWidget* parent, Qt::WindowFlags f)
    : QWidget( parent, f )
{
  setObjectName( "DetectorDisplay" );
#endif
    nameLabel = new QLabel( this);
    nameLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    nameInput = NULL;

    idLabel = new QLabel( this);
    idLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    idBox = new QSpinBox( this);
    idBox->setGeometry( QRect( 10, 170, 74, 20 ) );
    
    NElLabel = new QLabel( this);
    NElLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    elBox = new QSpinBox( this);
    elBox->setGeometry( QRect( 10, 170, 74, 20 ) );

    stackLabel = new QLabel( this);
    stackLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    stackBox = new QSpinBox( this);
    stackBox->setGeometry( QRect( 10, 170, 74, 20 ) );

    matLabel = new QLabel( this);
    matLabel->setGeometry( QRect( 10, 170, 74, 20 ) );
    matBox = new QComboBox( this);
    matBox->setGeometry( QRect( 10, 170, 74, 20 ) );

    circularBox = new QCheckBox( this);
    circularBox->setGeometry( QRect( 10, 170, 74, 20 ) );

    shapeShow = new QCanvasWidget( this);
    shapeShow->setGeometry( QRect( 10, 170, 74, 20));

    shap=new ShapeDisplay(this);
    shap->setDraw(false);
    languageChange();
    connect( shapeShow, SIGNAL( doubleClicked(const QPoint &) ), this, SLOT( OnDetectorDraw() ) );

    connect( idBox,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    connect( elBox,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    connect( stackBox,SIGNAL(valueChanged(int)),this,SLOT(onPropertyChange()));
    connect( matBox,SIGNAL(highlighted(int)),this,SLOT(onPropertyChange()));
    connect( circularBox,SIGNAL(toggled(bool)),this,SLOT(onPropertyChange()));
    connect( shapeShow,SIGNAL(mouseMooved(const QPoint&)),this,SLOT(mouseOnCanvas(const QPoint&)));
    connect( shap,SIGNAL(changed()),this,SLOT(shapeChanged()));
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
    tmpMenu=NULL;
#endif
    init();
    idBox->setRange(0,200);
    elBox->setRange(0,500);

    // signals and slots connections
}

/*
 *  Destroys the object and frees any allocated resources
 */
DetectorDisplay::~DetectorDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DetectorDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Detector" ) );
#else
    setWindowTitle(tr("Detector"));
#endif

    nameLabel->setText( tr( "Name" ) );
    idLabel->setText( tr( "ID" ) );
    NElLabel->setText( tr( "N Elements" ) );
    matLabel->setText( tr( "Material" ) );
    stackLabel->setText( tr( "Stack-type" ) );
    circularBox->setText( tr( "Circular detector" ) );
}

