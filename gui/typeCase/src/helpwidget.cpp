#include "helpwidget.h"

#include <qvariant.h>
#if QT_VERSION < 0x040000
#include <qheader.h>
#else
#include <QResizeEvent>
#endif
#include <qframe.h>
#include <qlabel.h>
#include <qtextbrowser.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


#include "qnumberinputdialog.h"
#include "elementdisplay.h"
#include "Eparticles.h"
#include "helpwidget.ui.h"
/*
 *  Constructs a HelpWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
HelpWidget::HelpWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "HelpWidget" );
#else
HelpWidget::HelpWidget( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName( "HelpWidget" );
#endif

  textEdit1 = new QTextBrowser( this);//, "textEdit1" );
    textEdit1->setGeometry( QRect( 5, 115, 260, 160 ) );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
HelpWidget::~HelpWidget()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void HelpWidget::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Help" ) );
#else
    setWindowTitle(tr("Help"));
#endif
}

