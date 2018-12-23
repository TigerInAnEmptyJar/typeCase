#ifndef MYUTILITIES_H
#define MYUTILITIES_H
using namespace std;
#include <vector>
#ifdef QWIDGET_H
#ifdef QCOLOR_H
void setWidgetColor(QWidget *w,const QColor &c);
#endif
#endif
#ifdef QCOMBOBOX_H
void add2ComboBox(QComboBox *c, const QString & input);
void setComboBoxCurrentItem(QComboBox *c, int num);
int comboBoxCurrentItem(QComboBox *c);
QComboBox* defineComboBox(bool val,QWidget *parent,const char* name);
#endif
#if QT_VERSION < 0x040000
#ifdef QLISTBOX_H
void add2ListBox(QListBox *c, const QString &input);
#endif
#else
#ifdef QLISTWIDGET_H
void add2ListBox(QListWidget* c, const QString &input);
#endif
#endif
#ifdef QSPINBOX_H
QSpinBox* defineSpinBox(QWidget *parent,const char* name);
#endif
#ifdef QLABEL_H
QLabel* defineLabel(QWidget *parent,const char* name);
#endif
#ifdef QWIDGET_H
string getOpenFileName(QWidget *parent, const string &caption, const string &startWith, const string &filter, string* selectedFilter=NULL);
string getOpenDirectory(QWidget *parent, const string &caption, const string &startWith, const string &filter);
string getSaveFileName(QWidget *parent, const string &caption,  const string &startWith, const string &filter, bool reAsk, string* selectedFilter=NULL);
string getSaveFileName(QWidget *parent, const string &caption,  const string &startWith, const string &filter, string* selectedFilter=NULL);
vector<string> getOpenFileNames(QWidget *parent, const string &caption, const string &startWith, const string &filter, string* selectedFilter=NULL);
#endif
string QS2s(const QString &s);
#endif
