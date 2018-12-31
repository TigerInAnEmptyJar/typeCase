///*****************************************************************************
//** Form interface generated from reading ui file 'documentation_mw.ui'
//**
//** Created: Wed Jul 2 15:48:13 2008
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug
// 31 2005 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef DOCUMENTATION_MW_H
#define DOCUMENTATION_MW_H

#include "tmyclass.h"
#include <qmainwindow.h>
#include <qvariant.h>
//#include "doc_classes.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;
class QTextEdit;
class QPushButton;
class QCheckBox;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QComboBox;
class my_doc_classes;
using namespace std;

class documentation_mw : public QMainWindow
{
  Q_OBJECT

public:
#if QT_VERSION < 0x040000
  documentation_mw(QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel);
#else
  documentation_mw(QWidget* parent = 0, Qt::WindowFlags fl = 0);
#endif
  ~documentation_mw();
  QMenuBar* MenuBar;
  QPopupMenu* fileMenu;
  QPopupMenu* Edit;
  QPopupMenu* Help;
  QPopupMenu* HTML;
  QAction* fileNewAction;
  QAction* fileOpenAction;
  QAction* fileSaveAction;
  QAction* fileSaveAsAction;
  QAction* filePrintAction;
  QAction* fileExitAction;
  QAction* editUndoAction;
  QAction* editRedoAction;
  QAction* editCutAction;
  QAction* editCopyAction;
  QAction* editPasteAction;
  QAction* editFindAction;
  QAction* Action;
  QAction* editHTMLAction;
  QAction* editUpdateAction;
  QAction* fileCloseAction;
  QAction* fileOpen_DirectoryAction;
  QAction* editDependenciesAction;
  QAction* editHTML_allAction;
  QAction* helpKnown_ClassesAction;
  QAction* htmlboldAction;
  QAction* htmlitalicAction;
  QAction* htmlvarAction;
  QAction* htmlsuperscriptAction;
  QAction* htmlsubscriptAction;
  QAction* editApply_descriptionAction;
  QAction* editNext_itemAction;
  QAction* editRemove_itemAction;
  QAction* editShowClassAction;
  QAction* editInsertQt3DesAction;
  QAction* editInsertQt4DesAction;

  QTextEdit* classShow;
  QGridLayout* flayout;
  QLabel* textLabel2;
  QLabel* textLabel3;
  QTextEdit* textEdit1;
  QPushButton* pushButton1;
  QCheckBox* checkBox1;
  QComboBox* comboBox2;
  QCheckBox* checkBox8;
#if QT_VERSION < 0x040000
  QListView* listView2;
  QListView* listView1;
  virtual nameTypeDoc* getProperty(QListViewItem* it);
  bool checkWithProperty(QListViewItem* it, functionNameTypeDoc* prp);
#else
  QTreeWidget* listView2;
  QTreeWidget* listView1;
  virtual nameTypeDoc* getProperty(QTreeWidgetItem* it);
  bool checkWithProperty(QTreeWidgetItem* it, functionNameTypeDoc* prp);
#endif
  virtual vector<functionNameTypeDoc> associatedFunctions(const string& classname);
  int classNumByName(const string& nme) const;
  int classNumByName(const QString& nme) const;
public slots:
  virtual void OnFileOpen();
  virtual void OnFileImport();
  virtual void OnFileSaveAs();
  virtual void OnFileSave();
  virtual void OnFileExit();
  virtual void OnFileClose();
  virtual void OnFileOpenDirectory();
  virtual void OnClassClick();
  virtual void OnEditUpdate();
  virtual void OnRemoveItem();
  virtual void OnEditHTML();
  virtual void OnEditHTMLAll();
  virtual void OnDependencies();
  virtual void OnEditNextItem();
  virtual void OnApplyButtonClick();
  virtual void OnClassDescriptionCheck();
  virtual void OnClassShow();
#if QT_VERSION < 0x040000
  virtual void addParameterToListView(QListViewItem* it, functionNameTypeDoc& var);
  virtual void OnPropertyRightClick(QListViewItem* it);
  virtual void OnPropertyClick(QListViewItem* it);
  virtual void OnSuperClassClick(QListViewItem* it);
  virtual void OnSuperClassRightClick(QListViewItem* it);
  virtual void setItemOn(QListViewItem* it);
#else
  virtual void addParameterToListView(QTreeWidgetItem* it, functionNameTypeDoc& var);
  virtual void OnPropertyRightClick(QTreeWidgetItem* it);
  virtual void OnPropertyClick(QTreeWidgetItem* it);
  virtual void OnSuperClassClick(QTreeWidgetItem* it);
  virtual void OnSuperClassRightClick(QTreeWidgetItem* it);
  virtual void setItemOn(QTreeWidgetItem* it);
#endif
  virtual void OnHelpKnownClasses();
  virtual void OnHTMLitalic();
  virtual void OnHTMLbold();
  virtual void OnHTMLvar();
  virtual void OnHTMLsuper();
  virtual void OnHTMLsub();
  virtual void OnQt3Des();
  virtual void OnQt4Des();
  virtual void OnKnownFunctionsClick();

protected:
  vector<TMyClass> classes;
  vector<string> files;
  vector<QWidget*> displayers;
  my_doc_classes* knowncls;

protected slots:
  virtual void languageChange();

private:
  void init();
  void destroy();
};

#endif // DOCUMENTATION_MW_H
