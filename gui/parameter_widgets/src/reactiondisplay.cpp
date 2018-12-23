#include "reactiondisplay.h"

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

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "baseShape.h"
#include "reactiondisplay.ui.h"
/*
 *  Constructs a ReactionDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
ReactionDisplay::ReactionDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ReactionDisplay" );
#else
ReactionDisplay::ReactionDisplay( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
	setObjectName( "ReactionDisplay" );
#endif

    mBLabel = new QLabel( this);
    mTLabel = new QLabel( this);
    beamLabel = new QLabel( this);
    beamInput = new QLineEdit(this);
    targetLabel = new QLabel( this);
    targetInput = new QLineEdit(this);

    matBeam = new QComboBox( this);
    matTarget = new QComboBox( this);

    twoBeam = new QCheckBox(this);

    shapeDraw = new QCanvasWidget( this);
    shape=new ShapeDisplay(this);

    myLayout=new QGridLayout(this);//,9,2);
    myLayout->addWidget(mBLabel,0,0);
    myLayout->addWidget(matBeam,1,0);
    myLayout->addWidget(beamLabel,2,0);
    myLayout->addWidget(beamInput,3,0);
    myLayout->addWidget(mTLabel,4,0);
    myLayout->addWidget(matTarget,5,0);
    myLayout->addWidget(targetLabel,6,0);
    myLayout->addWidget(targetInput,7,0);
    myLayout->addWidget(twoBeam,8,0);
#if QT_VERSION < 0x040000
    myLayout->addMultiCellWidget(shape,0,4,1,1);
    myLayout->addMultiCellWidget(shapeDraw,5,8,1,1);
#else
    myLayout->addWidget(shape,0,1,5,1);
    myLayout->addWidget(shapeDraw,5,1,4,1);
#endif
    myLayout->setMargin(5);
    myLayout->setSpacing(5);
    shape->setDraw(false);
    ReadOnly=false;
    languageChange();

    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    init();
    connect( shapeDraw, SIGNAL( doubleClicked(const QPoint &) ), this, SLOT( OnDraw() ) );
    connect( targetInput,SIGNAL(textChanged(const QString &)),this,SLOT(onPropertyChange()));
    connect( beamInput,SIGNAL(textChanged(const QString &)),this,SLOT(onPropertyChange()));
    //connect( twoBeam,SIGNAL(clicked()),this,SLOT(onPropertyChange()));
    //connect( twoBeam,SIGNAL(stateChanged(int)),this,SLOT(onPropertyChange()));
    connect( twoBeam,SIGNAL(toggled(bool)),this,SLOT(onBeamNumberChange()));

    // signals and slots connections
}

/*
 *  Destroys the object and frees any allocated resources
 */
ReactionDisplay::~ReactionDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ReactionDisplay::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Reaction" ) );
#else
    setWindowTitle(tr("Reaction"));
#endif
    mBLabel->setText("Beam (1) Material");
    mTLabel->setText("Target/Beam(2) Material");
    beamLabel->setText("Beam (1) Momentum");
    targetLabel->setText("Target/Beam (2) Momentum ");
    twoBeam->setText( tr( "Two beams" ) );
}

