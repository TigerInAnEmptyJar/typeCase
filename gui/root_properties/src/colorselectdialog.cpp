/****************************************************************************
** Form implementation generated from reading ui file 'colorselectdialog.ui'
**
** Created: Wed Sep 23 19:59:03 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "colorselectdialog.h"
#include <qvariant.h>
#include <TROOT.h>
#include <TColor.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "colorselectdialog.ui.h"
/*
 *  Constructs a ColorSelectDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
#if QT_VERSION < 0x040000
    ColorSelectDialog::ColorSelectDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
#else
    ColorSelectDialog::ColorSelectDialog( QWidget* parent,  Qt::WindowFlags f)
      :QDialog(parent,f)
#endif
{
#if QT_VERSION < 0x040000
    if ( !name )
	setName( "ColorSelectDialog" );
#else
    setObjectName("ColorSelectDialog" );
#endif
    setEnabled( FALSE );
    languageChange();
    resize( QSize(225, 253).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ColorSelectDialog::~ColorSelectDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ColorSelectDialog::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Select color" ) );
#endif
}

