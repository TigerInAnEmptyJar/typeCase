#include "colorbutton.h"
#include <qvariant.h>
#include <TROOT.h>
#include <TColor.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "colorbutton.ui.h"
/*
 *  Constructs a ColorButton as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
ColorButton::ColorButton( QWidget* parent, const char* name, WFlags fl )
    : QPushButton( parent, name )
{
    if ( !name )
	setName( "ColorButton" );
#else
ColorButton::ColorButton( QWidget* parent, Qt::WindowFlags f)
    : QPushButton( parent )
{
	setObjectName( "ColorButton" );
#endif
    setEnabled( TRUE );
    languageChange();
    init();
    resize( QSize(23, 23).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
ColorButton::~ColorButton()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ColorButton::languageChange()
{
    setText( tr( " " ) );
}

