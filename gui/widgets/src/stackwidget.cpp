#include "stackwidget.h"

#if QT_VERSION < 0x040000
#include <qheader.h>
#include <qwidgetstack.h>
#else
#include <QStackedWidget>
#endif
#include <qcombobox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#if QT_VERSION > 0x040000
#include <QResizeEvent>
#endif
#include "stackwidget.ui.h"
/*
 *  Constructs a StackWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
StackWidget::StackWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "StackWidget" );
    stackw=new QWidgetStack(this);
#else
StackWidget::StackWidget( QWidget* parent,Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName( "StackWidget" );
  stackw=new QStackedWidget(this);
#endif
  stackw->setGeometry(QRect(5,5,300,150));
    selector=new QComboBox(this);
    selector->setGeometry(QRect(5,5,300,150));
    languageChange();

    // signals and slots connections
    connect(selector,SIGNAL(activated(int)),this,SLOT(widgetSelect(int)));
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
StackWidget::~StackWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void StackWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Stack" ) );
#else
    setWindowTitle( tr("Stack"));
#endif
}

