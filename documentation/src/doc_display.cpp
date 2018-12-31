/****************************************************************************
** Form implementation generated from reading ui file 'doc_display.ui'
**
** Created: Wed Jul 2 15:50:35 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug
*31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "doc_display.h"

#include <fstream>
#include <qimage.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qtextedit.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwhatsthis.h>

#include "doc_display.ui.h"
/*
 *  Constructs a my_doc_display as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
my_doc_display::my_doc_display(QWidget* parent, const char* name, WFlags fl)
    : QWidget(parent, name, fl)
{
  if (!name)
    setName("my_doc_display");
#else
my_doc_display::my_doc_display(QWidget* parent, Qt::WindowFlags fl) : QWidget(parent, fl)
{
  setObjectName("my_doc_display");
#endif

  textEdit1 = new QTextEdit(this);
  flayout = new QGridLayout(this);
  flayout->addWidget(textEdit1, 0, 0);
  flayout->setMargin(10);
  flayout->setSpacing(5);
  //    textEdit1->setGeometry( QRect( 30, 40, 168, 112 ) );
  textEdit1->setReadOnly(TRUE);
  languageChange();
  resize(QSize(600, 480).expandedTo(minimumSizeHint()));
#if QT_VERSION < 0x040000
  clearWState(WState_Polished);
#else
  setWindowState(Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
my_doc_display::~my_doc_display()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void my_doc_display::languageChange()
{
#if QT_VERSION < 0x040000
  setCaption(tr("display"));
#else
  setWindowTitle(tr("display"));
#endif
}
