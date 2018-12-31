///****************************************************************************
//** Form interface generated from reading ui file 'my_doc_display.ui'
//**
//** Created: Wed Jul 2 15:48:13 2008
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug
// 31 2005 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef MY_DOC_DISPLAY_H
#define MY_DOC_DISPLAY_H

#include "tmyclass.h"
#include <qvariant.h>
#include <qwidget.h>
#include <vector>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTextEdit;
using namespace std;

class my_doc_display : public QWidget
{
  Q_OBJECT

public:
#if QT_VERSION < 0x040000
  my_doc_display(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
#else
  my_doc_display(QWidget* parent = 0, Qt::WindowFlags f = 0);
#endif
  ~my_doc_display();

  QTextEdit* textEdit1;
  QGridLayout* flayout;
  virtual TMyClass* getClass();

public slots:
  virtual void showText(const vector<string>& text, TMyClass* cls);
  virtual void showFile(const string& filename, TMyClass* cls);
  virtual void setClass(TMyClass* cls);

protected:
  TMyClass* displayedClass;

protected slots:
  virtual void languageChange();
};

#endif // MY_DOC_DISPLAY_H
