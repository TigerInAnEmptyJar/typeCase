/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
//#define JUSTDEFINE
//#include "utils.h"
extern vector<TMyClass*> knownClasses;
extern TMyClass* getKnownClass(const string& name);
void my_doc_classes::showClasses()
{
  listBox1->clear();
  for (unsigned int i = 0; i < knownClasses.size(); i++)
#if QT_VERSION < 0x040000
    listBox1->insertItem(knownClasses[i]->Name().data());
#else
    listBox1->addItem(knownClasses[i]->Name().data());
#endif
  textEdit1->clear();
}
#if QT_VERSION < 0x040000
void my_doc_classes::OnClassClick(QListBoxItem* it)
#else
void my_doc_classes::OnClassClick(QListWidgetItem* it)
#endif
{
  textEdit1->clear();
  if (it == NULL)
    return;
  TMyClass* tmp = getKnownClass(QS2s(it->text()));
  if (tmp == NULL)
    return;
  textEdit1->clear();
  textEdit1->append(QString("class ") + tmp->Name().data());
  if (tmp->nSuperClasses() > 0)
    textEdit1->append(QString::number(tmp->nSuperClasses()) + " superClasses");
  textEdit1->append("functions");
  if (tmp->nConstructors() > 0)
    textEdit1->append(QString::number(tmp->nConstructors()) + " constructors");
  if (tmp->nMethods(0) > 0)
    textEdit1->append(QString::number(tmp->nMethods(0)) + " public");
  if (tmp->nMethods(1) > 0)
    textEdit1->append(QString::number(tmp->nMethods(1)) + " protected");
  if (tmp->nMethods(2) > 0)
    textEdit1->append(QString::number(tmp->nMethods(2)) + " private");
  if (tmp->nMethods(3) > 0)
    textEdit1->append(QString::number(tmp->nMethods(3)) + " public slots");
  if (tmp->nMethods(4) > 0)
    textEdit1->append(QString::number(tmp->nMethods(4)) + " protected slots");
  if (tmp->nMethods(5) > 0)
    textEdit1->append(QString::number(tmp->nMethods(5)) + " private slots");
  if (tmp->nMethods(6) > 0)
    textEdit1->append(QString::number(tmp->nMethods(6)) + " signals");
  textEdit1->append("variables");
  if (tmp->nVariables(0) > 0)
    textEdit1->append(QString::number(tmp->nVariables(0)) + " public");
  if (tmp->nVariables(1) > 0)
    textEdit1->append(QString::number(tmp->nVariables(1)) + " protected");
  if (tmp->nVariables(2) > 0)
    textEdit1->append(QString::number(tmp->nVariables(2)) + " private");
}
