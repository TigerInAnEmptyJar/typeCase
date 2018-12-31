// ****************************************************************************
// ** Form interface generated from reading ui file 'my_doc_classes.ui'
// **
// ** Created: Thu Jun 5 10:30:47 2008
// **      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited
// Aug 31 2005 $)
// **
// ** WARNING! All changes made in this file will be lost!
// ****************************************************************************/

#ifndef MY_DOC_CLASSES_H
#define MY_DOC_CLASSES_H

#include <QtCore/QVariant>
#include <QtWidgets/QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
#endif
class QTextEdit;
using namespace std;

class my_doc_classes : public QWidget
{
  Q_OBJECT

public:
#if QT_VERSION < 0x040000
  my_doc_classes(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
#else
  my_doc_classes(QWidget* parent = 0, Qt::WindowFlags f = 0);
#endif
  ~my_doc_classes();

#if QT_VERSION < 0x040000
  QListBox* listBox1;
#else
  QListWidget* listBox1;
#endif
  QTextEdit* textEdit1;
  QGridLayout* flayout;

public slots:
  virtual void showClasses();
#if QT_VERSION < 0x040000
  virtual void OnClassClick(QListBoxItem* it);
#else
  virtual void OnClassClick(QListWidgetItem* it);
#endif

protected slots:
  virtual void languageChange();
};

#endif // MY_DOC_CLASSES_H
