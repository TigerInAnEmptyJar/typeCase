#include "algorithmorderwidget.h"

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "algorithmdisplay.h"

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "utilities.h"
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#include <qpopupmenu.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QResizeEvent>
#include <QMenu>
#endif
#include "parameterManager.h"
#include "algorithmorderwidget.ui.h"
/*
 *  Constructs a AlgorithmWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
AlgorithmOrderWidget::AlgorithmOrderWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "AlgorithmOrderWidget" );
    listView = new QListBox( this);
#else
AlgorithmOrderWidget::AlgorithmOrderWidget( QWidget* parent,  Qt::WindowFlags f )
    : QWidget( parent, f )
{
	setObjectName( "AlgorithmOrderWidget" );
    listView = new QListWidget( this);
#endif

    listView->setGeometry( QRect( 10, 200, 300, 210 ) );

    pdisplay = new AlgorithmDisplay( this);
    pdisplay->setGeometry( QRect( 5, 5, 300, 150 ) );

    line = new QFrame( this);
    line->setGeometry( QRect( 130, 160, 200, 10 ) );
    line->setFrameShape( QFrame::VLine );
    line->setFrameShadow( QFrame::Sunken );
    line->setFrameShape( QFrame::VLine );
    languageChange();

    // signals and slots connections
#if QT_VERSION < 0x040000
   connect( listView, SIGNAL(selectionChanged ( QListBoxItem *)), this, SLOT(onAlgorithmSelect(QListBoxItem*)));
   connect(listView,SIGNAL(contextMenuRequested ( QListBoxItem *, const QPoint &)),this,SLOT(onListMenu(QListBoxItem*, const QPoint &)));
#else
   listView->setContextMenuPolicy(Qt::CustomContextMenu);
   connect( listView, SIGNAL(currentItemChanged ( QListWidgetItem *,QListWidgetItem *)), this, SLOT(onAlgorithmSelect(QListWidgetItem*)));
   connect(listView,SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(onListMenu(const QPoint&)));
#endif
//     connect( pdisplay, SIGNAL(changed()), this, SIGNAL(changed()));
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
AlgorithmOrderWidget::~AlgorithmOrderWidget()
{
    // no need to delete child widgets, Qt does it all for us
  while(!added.empty())
    {
      algorithm_parameter *ap=added.back();
      added.pop_back();
      if(getItemFromParameter(ap)==NULL||untaken.back())
	delete ap;
    }
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AlgorithmOrderWidget::languageChange()
{
#if QT_VERSION < 0x040000
   setCaption( tr( "Algorithms" ) );
#else
   setWindowTitle( tr( "Algorithms" ) );
#endif
}

#if QT_VERSION < 0x040000
void AlgorithmOrderWidget::onListMenu(QListBoxItem* it, const QPoint & pos)
{
  QPopupMenu pm;
  pm.insertItem("save anaylsis algorithms",this,SLOT(algorithmSave()));
  pm.exec(pos);
}
#else
void AlgorithmOrderWidget::onListMenu(const QPoint &pos)
{
  QMenu pm;
  pm.addAction("save anaylsis algorithms",this,SLOT(algorithmSave()));
  pm.exec(listView->viewport()->mapFromGlobal(pos));
}
#endif
void AlgorithmOrderWidget::algorithmSave()
{
  string filename="";
  filename=getSaveFileName(this, "Save ordered algorithms to parameter file", string(getenv("KTOFPACK"))+"/data/algorithms", "parameter files (*.data)", true);
  if(filename=="")return;
  vector<algorithm_parameter*> algos=getOrder();
  parameterManager::writeAlgorithmParameter_ascii(filename,algos);
}
