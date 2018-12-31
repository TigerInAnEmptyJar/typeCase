#include <QtGui/QPalette>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#if QT_VERSION < 0x040000
#include <QtWidgets/QListBox>
#else
#include <QtWidgets/QListWidget>
#endif
using namespace std;

extern bool existing(string filename);

void setWidgetColor(QWidget* w, const QColor& c)
{
#if QT_VERSION < 0x040000
  w->setPaletteBackgroundColor(c);
#else
  QPalette pa(w->palette());
  pa.setColor(QPalette::Button, c);
  pa.setColor(QPalette::Window, c);
  w->setPalette(pa);
#endif
}

void add2ComboBox(QComboBox* c, const QString& input)
{
  if (c == NULL)
    return;
#if QT_VERSION < 0x040000
  c->insertItem(input);
#else
  c->addItem(input);
#endif
}
void setComboBoxCurrentItem(QComboBox* c, int num)
{
  if (c == NULL)
    return;
#if QT_VERSION < 0x040000
  c->setCurrentItem(num);
#else
  c->setCurrentIndex(num);
#endif
}
int comboBoxCurrentItem(QComboBox* c)
{
  if (c == NULL)
    return -1;
#if QT_VERSION < 0x040000
  return c->currentItem();
#else
  return c->currentIndex();
#endif
}
QSpinBox* defineSpinBox(QWidget* parent, const char* name)
{
#if QT_VERSION < 0x040000
  return new QSpinBox(parent, name);
#else
  return new QSpinBox(parent);
#endif
}
QLabel* defineLabel(QWidget* parent, const char* name)
{
#if QT_VERSION < 0x040000
  return new QLabel(parent, name);
#else
  return new QLabel(parent);
#endif
}
QComboBox* defineComboBox(bool val, QWidget* parent, const char* name)
{
#if QT_VERSION < 0x040000
  return new QComboBox(val, parent, name);
#else
  return new QComboBox(parent);
#endif
}
#include <QtWidgets/QFileDialog>
#include <string>
#include <vector>
string getOpenFileName(QWidget* parent, const string& caption, const string& startWith,
                       const string& filter, string* selectedFilter)
{
  QString sf, fn;
#if QT_VERSION < 0x040000
  fn =
      QFileDialog::getOpenFileName(startWith.data(), filter.data(), parent, 0, caption.data(), &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.ascii();
  if (fn == "")
    return "";
  return fn.ascii();
#else
  fn = QFileDialog::getOpenFileName(parent, caption.data(), startWith.data(), filter.data(), &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.toStdString();
  if (fn == "")
    return "";
  return fn.toStdString();
#endif
}
string getSaveFileName(QWidget* parent, const string& caption, const string& startWith,
                       const string& filter, string* selectedFilter)
{
  QString sf, fn;
#if QT_VERSION < 0x040000
  fn =
      QFileDialog::getSaveFileName(startWith.data(), filter.data(), parent, 0, caption.data(), &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.ascii();
  if (fn == "")
    return "";
  return fn.ascii();
#else
  fn = QFileDialog::getSaveFileName(parent, caption.data(), startWith.data(), filter.data(), &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.toStdString();
  if (fn == "")
    return "";
  return fn.toStdString();
#endif
}
string getSaveFileName(QWidget* parent, const string& caption, const string& startWith,
                       const string& filter, bool reAsk, string* selectedFilter)
{
  string s = getSaveFileName(parent, caption, startWith, filter, selectedFilter);
  if (!reAsk || s == "" || !existing(s))
    return s;
  int ret = QMessageBox::No;
  while (existing(s) && ret != QMessageBox::Yes) {
    ret = QMessageBox::warning(parent, "File exists",
                               (string("File \"") + s + "\" exists.\nOverwrite?").data(),
                               QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
                               QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Cancel)
      return "";
    if (ret == QMessageBox::No)
      s = getSaveFileName(parent, caption, startWith, filter, selectedFilter);
    if (s == "")
      return "";
  }
  return s;
}
vector<string> getOpenFileNames(QWidget* parent, const string& caption, const string& startWith,
                                const string& filter, string* selectedFilter)
{
  QString sf;
  QStringList fn;
  vector<string> ret;
#if QT_VERSION < 0x040000
  fn = QFileDialog::getOpenFileNames(startWith.data(), filter.data(), parent, 0, caption.data(),
                                     &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.ascii();
  for (int i = 0; i < (int)fn.size(); i++)
    if (fn[i] != NULL)
      ret.push_back(string(fn[i].ascii()));
#else
  fn = QFileDialog::getOpenFileNames(parent, caption.data(), startWith.data(), filter.data(), &sf);
  if (selectedFilter != NULL)
    *selectedFilter = sf.toStdString();
  for (int i = 0; i < (int)fn.size(); i++)
    if (fn[i] != NULL)
      ret.push_back(fn[i].toStdString());
#endif
  return ret;
}
string getOpenDirectory(QWidget* parent, const string& caption, const string& startWith,
                        const string& filter)
{
  QString fn;
#if QT_VERSION < 0x040000
  fn = QFileDialog::getExistingDirectory(startWith.data(), parent, 0, caption.data(), TRUE);
  if (fn == "")
    return "";
  return fn.ascii();
#else
  fn = QFileDialog::getExistingDirectory(parent, caption.data(), startWith.data());
  if (fn == "")
    return "";
  return fn.toStdString();
#endif
}
string QS2s(const QString& s)
{
#if QT_VERSION < 0x040000
  return string(s.ascii());
#else
  return s.toStdString();
#endif
}
#ifdef QLISTBOX_H
void add2ListBox(QListBox* c, const QString& input)
{
  if (c != NULL)
    c->insertItem(input);
}
#endif
#ifdef QLISTWIDGET_H
void add2ListBox(QListWidget* c, const QString& input)
{
  if (c != NULL)
    c->addItem(input);
}
#endif
