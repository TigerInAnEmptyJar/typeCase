#include "paintdisplay.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "colorbutton.h"
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "utilities.h"
#include "paintdisplay.ui.h"
/*
 *  Constructs a PaintDisplay as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
PaintDisplay::PaintDisplay( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "PaintDisplay" );
#else
PaintDisplay::PaintDisplay( QWidget* parent,Qt::WindowFlags f )
    : QWidget( parent, f )
{
	setObjectName( "PaintDisplay" );
#endif
    idLabel = new QLabel( this);
    idInput = new QLineEdit(this);
    idInput->setInputMask("0009");
    nameLabel = new QLabel( this);
    nameInput = new QLineEdit(this);
    markerLabel = new QLabel( this);
    markerInput = new QComboBox(this);
    colorInput = new ColorButton( this);
    doPaint = new QCheckBox( this);
//     setMinimumSize(100,23*5+6);
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,6,7);
    layout->addMultiCellWidget(nameLabel,0,0,0,0);
    layout->addMultiCellWidget(nameInput,0,0,1,1);
    layout->addMultiCellWidget(idLabel,1,1,0,0);
    layout->addMultiCellWidget(idInput,1,1,1,1);
    layout->addMultiCellWidget(markerLabel,2,2,0,0);
    layout->addMultiCellWidget(markerInput,2,2,1,1);
    layout->addMultiCellWidget(colorInput,3,3,0,0);
    layout->addMultiCellWidget(doPaint,4,4,0,2);
    layout->setColStretch(0,0);
    layout->setColStretch(1,1);
#else
    layout=new QGridLayout(this);
    layout->addWidget(nameLabel,0,0,1,1);
    layout->addWidget(nameInput,0,1,1,1);
    layout->addWidget(idLabel,1,0,1,1);
    layout->addWidget(idInput,1,1,1,1);
    layout->addWidget(markerLabel,2,0,1,1);
    layout->addWidget(markerInput,2,1,1,1);
    layout->addWidget(colorInput,3,0,1,1);
    layout->addWidget(doPaint,4,0,1,2);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);
    setLayout(layout);
#endif
    layout->setMargin(0);
    layout->setSpacing(5);

    languageChange();
    resize( QSize(390, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    connect(nameInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(idInput,SIGNAL(textChanged(const QString&)),this,SLOT(onPropertyChange()));
    connect(markerInput,SIGNAL(activated(int)),this,SLOT(onMarkerClick()));
    connect(colorInput,SIGNAL(colorChanged(int)),this,SLOT(onColorSelected(int)));
    connect(doPaint,SIGNAL(toggled(bool)),this,SLOT(onPaintClick()));
    // signals and slots connections
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
PaintDisplay::~PaintDisplay()
{
  destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PaintDisplay::languageChange()
{
#if QT_VERSION < 0x040000
  setCaption( tr( "Paint" ) );
#else
  setWindowTitle(tr( "Paint" ) );
#endif
     idLabel->setText( tr( "ID" ) );
    nameLabel->setText( tr( "Name" ) );
    markerLabel->setText( tr( "Marker" ) );
    doPaint->setText( tr("Paint") );
}

