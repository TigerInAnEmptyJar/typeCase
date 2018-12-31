/****************************************************************************
** Form implementation generated from reading ui file 'my_doc_classes.ui'
**
** Created: Thu Jun 5 10:30:56 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug
*31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "doc_classes.h"

#include <qvariant.h>
#if QT_VERSION < 0x040000
#include <qlistbox.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#endif
#include "doc_classes.ui.h"
#include "string_utilities.h"
#include "tmyclass.h"
#include "utilities.h"
#include <qimage.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qtextedit.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
/*
 *  Constructs a my_doc_classes as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
my_doc_classes::my_doc_classes(QWidget* parent, const char* name, WFlags fl)
    : QWidget(parent, name, fl)
{
  if (!name)
    setName("my_doc_classes");
#else
my_doc_classes::my_doc_classes(QWidget* parent, Qt::WindowFlags fl) : QWidget(parent, fl)
{
  setObjectName("my_doc_classes");
#endif
#if QT_VERSION < 0x040000
  listBox1 = new QListBox(this, "listBox1");
#else
  listBox1 = new QListWidget(this);
#endif
  textEdit1 = new QTextEdit(this);
  flayout = new QGridLayout(this);
  flayout->addWidget(listBox1, 0, 0);
  flayout->addWidget(textEdit1, 1, 0);
  flayout->setMargin(10);
  flayout->setSpacing(5);
  //     listBox1->setGeometry( QRect( 30, 20, 100, 60 ) );
  //     textEdit1->setGeometry( QRect( 330, 50, 168, 112 ) );
  textEdit1->setReadOnly(TRUE);
  languageChange();
  resize(QSize(600, 480).expandedTo(minimumSizeHint()));
#if QT_VERSION < 0x040000
  clearWState(WState_Polished);
  connect(listBox1, SIGNAL(currentChanged(QListBoxItem*)), this, SLOT(OnClassClick(QListBoxItem*)));
#else
  setWindowState(Qt::WindowNoState);
  connect(listBox1, SIGNAL(currentChanged(QListWidgetItem*)), this,
          SLOT(OnClassClick(QListWidgetItem*)));
#endif

  // signals and slots connections
}

/*
 *  Destroys the object and frees any allocated resources
 */
my_doc_classes::~my_doc_classes()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void my_doc_classes::languageChange()
{
  listBox1->clear();
#if QT_VERSION < 0x040000
  setCaption(tr("Known Classes"));
  listBox1->insertItem(tr("New Item"));
#else
  setWindowTitle(tr("Known Classes"));
  listBox1->addItem(tr("New Item"));
#endif
}
