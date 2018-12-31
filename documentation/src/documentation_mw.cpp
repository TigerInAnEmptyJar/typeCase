/****************************************************************************
** Form implementation generated from reading ui file 'documentation_mw.ui'
**
** Created: Wed Jul 2 15:50:27 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug
*31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "documentation_mw.h"

#include "doc_update_dialog.h"
#include <fstream>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qvariant.h>
#if QT_VERSION < 0x040000
#include <qheader.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qpopupmenu.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <qmenu.h>
#endif
#include <qaction.h>
#include <qcombobox.h>
#include <qimage.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "doc_display.h"
#include "doc_selectclassesdialog.h"
#include "documentation_mw.ui.h"
#include "qstatusbar.h"
#include "qstringinputdialog.h"
/*
 *  Constructs a documentation_mw as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
#if QT_VERSION < 0x040000
documentation_mw::documentation_mw(QWidget* parent, const char* name, WFlags fl)
    : QMainWindow(parent, name, fl)
{
  (void)statusBar();
  if (!name)
    setName("documentation_mw");
#else
documentation_mw::documentation_mw(QWidget* parent, Qt::WindowFlags fl) : QMainWindow(parent, fl)
{
  (void)statusBar();
  setObjectName("documentation_mw");
#endif
  setCentralWidget(new QWidget(this));

#if QT_VERSION < 0x040000
  listView2 = new QListView(centralWidget(), "listView2");
  listView2->addColumn(tr("Superclass"));
  listView2->addColumn(tr("Documented"));
  listView1 = new QListView(centralWidget(), "listView1");
  listView1->addColumn(tr("Name"));
  listView1->addColumn(tr("return type"));
  listView1->addColumn(tr("Visibility"));
  listView1->addColumn(tr("Default"));
#else
  listView2 = new QTreeWidget(centralWidget());
  listView1 = new QTreeWidget(centralWidget());
  listView1->setHeaderLabels(QStringList() << tr("Name") << tr("return type") << tr("Visibility")
                                           << tr("Default"));
  listView2->setHeaderLabels(QStringList() << tr("Superclass") << tr("Documented"));
#endif
  textLabel2 = new QLabel(centralWidget());
  textLabel3 = new QLabel(centralWidget());
  textEdit1 = new QTextEdit(centralWidget());
  pushButton1 = new QPushButton(centralWidget());
  checkBox1 = new QCheckBox(centralWidget());
  comboBox2 = new QComboBox(FALSE, centralWidget());
  checkBox8 = new QCheckBox(centralWidget());
  flayout = new QGridLayout(centralWidget());
#if QT_VERSION < 0x040000
  flayout->addMultiCellWidget(textLabel2, 0, 0, 0, 1);
  flayout->addMultiCellWidget(checkBox8, 0, 0, 4, 5);
  flayout->addMultiCellWidget(comboBox2, 0, 0, 2, 3);
  flayout->addMultiCellWidget(listView1, 1, 1, 0, 5);
  flayout->addMultiCellWidget(textLabel3, 2, 2, 0, 2);
  flayout->addMultiCellWidget(pushButton1, 2, 2, 3, 5);
  flayout->addMultiCellWidget(textEdit1, 3, 4, 0, 2);
  flayout->addMultiCellWidget(checkBox1, 3, 3, 3, 5);
  flayout->addMultiCellWidget(listView2, 4, 4, 3, 5);
#else
  flayout->addWidget(textLabel2, 0, 0, 1, 2);
  flayout->addWidget(checkBox8, 0, 4, 1, 2);
  flayout->addWidget(comboBox2, 0, 2, 1, 2);
  flayout->addWidget(listView1, 1, 0, 1, 6);
  flayout->addWidget(textLabel3, 2, 0, 1, 3);
  flayout->addWidget(pushButton1, 2, 3, 1, 3);
  flayout->addWidget(textEdit1, 3, 0, 2, 3);
  flayout->addWidget(checkBox1, 3, 3, 1, 3);
  flayout->addWidget(listView2, 4, 3, 1, 3);
#endif
  flayout->setSpacing(5);
  flayout->setMargin(10);
  classShow = new QTextEdit;
  classShow->setReadOnly(true);
  //     checkBox8->setGeometry( QRect( 250, 30, 150, 20 ) );
  //     textLabel2->setGeometry( QRect( 10, 30, 50, 20 ) );
  //     listView1->setGeometry( QRect( 10, 70, 580, 185 ) );
  //     comboBox2->setGeometry( QRect( 70, 30, 150, 21 ) );

  //     textLabel3->setGeometry( QRect( 10, 260, 65, 20 ) );
  //     pushButton1->setGeometry( QRect( 320, 276, 101, 23 ) );
  //     listView2->setGeometry( QRect( 320, 330, 260, 100 ) );
  //     checkBox1->setGeometry( QRect( 320, 306, 120, 20 ) );
  //     textEdit1->setGeometry( QRect( 10, 280, 290, 150 ) );

  // actions
  fileNewAction = new QAction(this, "fileNewAction");
  fileNewAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  fileOpenAction = new QAction(this, "fileOpenAction");
  fileOpenAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  fileSaveAction = new QAction(this, "fileSaveAction");
  fileSaveAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  fileSaveAsAction = new QAction(this, "fileSaveAsAction");
  filePrintAction = new QAction(this, "filePrintAction");
  filePrintAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  fileExitAction = new QAction(this, "fileExitAction");
  editUndoAction = new QAction(this, "editUndoAction");
  editUndoAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  editRedoAction = new QAction(this, "editRedoAction");
  editRedoAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  editCutAction = new QAction(this, "editCutAction");
  editCutAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  editCopyAction = new QAction(this, "editCopyAction");
  editCopyAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  editPasteAction = new QAction(this, "editPasteAction");
  editPasteAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  editFindAction = new QAction(this, "editFindAction");
  editFindAction->setIconSet(QIconSet(QPixmap::fromMimeSource("")));
  Action = new QAction(this, "Action");
  editHTMLAction = new QAction(this, "editHTMLAction");
  editUpdateAction = new QAction(this, "editUpdateAction");
  fileCloseAction = new QAction(this, "fileCloseAction");
  fileOpen_DirectoryAction = new QAction(this, "fileOpen_DirectoryAction");
  editDependenciesAction = new QAction(this, "editDependenciesAction");
  editHTML_allAction = new QAction(this, "editHTML_allAction");
  helpKnown_ClassesAction = new QAction(this, "helpKnown_ClassesAction");
  htmlboldAction = new QAction(this, "htmlboldAction");
  htmlitalicAction = new QAction(this, "htmlitalicAction");
  htmlvarAction = new QAction(this, "htmlvarAction");
  htmlsuperscriptAction = new QAction(this, "htmlsuperscriptAction");
  htmlsubscriptAction = new QAction(this, "htmlsubscriptAction");
  editApply_descriptionAction = new QAction(this, "editApply_descriptionAction");
  editNext_itemAction = new QAction(this, "editNext_itemAction");
  editRemove_itemAction = new QAction(this, "editRemove_itemAction");
  editShowClassAction = new QAction(this, "editShowClassAction");
  editInsertQt3DesAction = new QAction(this, "editInsertQt3DesAction");
  editInsertQt4DesAction = new QAction(this, "editInsertQt4DesAction");

  // toolbars

  // menubar
  MenuBar = new QMenuBar(this, "MenuBar");

  fileMenu = new QPopupMenu(this);
  fileNewAction->addTo(fileMenu);
  fileOpenAction->addTo(fileMenu);
  fileOpen_DirectoryAction->addTo(fileMenu);
  Action->addTo(fileMenu);
  fileSaveAction->addTo(fileMenu);
  fileSaveAsAction->addTo(fileMenu);
  fileCloseAction->addTo(fileMenu);
  fileMenu->insertSeparator();
  filePrintAction->addTo(fileMenu);
  fileMenu->insertSeparator();
  fileExitAction->addTo(fileMenu);
  MenuBar->insertItem(QString(""), fileMenu, 1);

  Edit = new QPopupMenu(this);
  editUndoAction->addTo(Edit);
  editRedoAction->addTo(Edit);
  Edit->insertSeparator();
  editCutAction->addTo(Edit);
  editCopyAction->addTo(Edit);
  editPasteAction->addTo(Edit);
  Edit->insertSeparator();
  editUpdateAction->addTo(Edit);
  editDependenciesAction->addTo(Edit);
  editApply_descriptionAction->addTo(Edit);
  editNext_itemAction->addTo(Edit);
  editRemove_itemAction->addTo(Edit);
  Edit->insertSeparator();
  editFindAction->addTo(Edit);
  Edit->insertSeparator();
  editShowClassAction->addTo(Edit);
  MenuBar->insertItem(QString(""), Edit, 2);

  Help = new QPopupMenu(this);
  helpKnown_ClassesAction->addTo(Help);
  MenuBar->insertItem(QString(""), Help, 3);

  HTML = new QPopupMenu(this);
  editHTMLAction->addTo(HTML);
  editHTML_allAction->addTo(HTML);
  HTML->insertSeparator();
  htmlboldAction->addTo(HTML);
  htmlitalicAction->addTo(HTML);
  htmlvarAction->addTo(HTML);
  htmlsuperscriptAction->addTo(HTML);
  htmlsubscriptAction->addTo(HTML);
  MenuBar->insertItem(QString(""), HTML, 4);

  languageChange();
  resize(QSize(600, 480).expandedTo(minimumSizeHint()));
  clearWState(WState_Polished);

  // signals and slots connections
  connect(comboBox2, SIGNAL(activated(int)), this, SLOT(OnClassClick()));
  connect(Action, SIGNAL(activated()), this, SLOT(OnFileImport()));
  connect(fileOpenAction, SIGNAL(activated()), this, SLOT(OnFileOpen()));
  connect(listView1, SIGNAL(currentChanged(QListViewItem*)), this,
          SLOT(OnPropertyClick(QListViewItem*)));
  connect(fileSaveAction, SIGNAL(activated()), this, SLOT(OnFileSave()));
  connect(fileSaveAsAction, SIGNAL(activated()), this, SLOT(OnFileSaveAs()));
  connect(fileExitAction, SIGNAL(activated()), this, SLOT(OnFileExit()));
  connect(editHTMLAction, SIGNAL(activated()), this, SLOT(OnEditHTML()));
  connect(pushButton1, SIGNAL(clicked()), this, SLOT(OnApplyButtonClick()));
  connect(checkBox1, SIGNAL(clicked()), this, SLOT(OnClassDescriptionCheck()));
  connect(editUpdateAction, SIGNAL(activated()), this, SLOT(OnEditUpdate()));
  connect(listView1, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)), this,
          SLOT(OnPropertyRightClick(QListViewItem*)));
  connect(listView2, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)), this,
          SLOT(OnSuperClassRightClick(QListViewItem*)));
  connect(fileOpen_DirectoryAction, SIGNAL(activated()), this, SLOT(OnFileOpenDirectory()));
  connect(fileCloseAction, SIGNAL(activated()), this, SLOT(OnFileClose()));
  connect(editDependenciesAction, SIGNAL(activated()), this, SLOT(OnDependencies()));
  connect(listView2, SIGNAL(clicked(QListViewItem*)), this,
          SLOT(OnSuperClassClick(QListViewItem*)));
  connect(editHTML_allAction, SIGNAL(activated()), this, SLOT(OnEditHTMLAll()));
  connect(helpKnown_ClassesAction, SIGNAL(activated()), this, SLOT(OnHelpKnownClasses()));
  connect(htmlboldAction, SIGNAL(activated()), this, SLOT(OnHTMLbold()));
  connect(htmlitalicAction, SIGNAL(activated()), this, SLOT(OnHTMLitalic()));
  connect(htmlvarAction, SIGNAL(activated()), this, SLOT(OnHTMLvar()));
  connect(htmlsubscriptAction, SIGNAL(activated()), this, SLOT(OnHTMLsub()));
  connect(htmlsuperscriptAction, SIGNAL(activated()), this, SLOT(OnHTMLsuper()));
  connect(editApply_descriptionAction, SIGNAL(activated()), this, SLOT(OnApplyButtonClick()));
  connect(editNext_itemAction, SIGNAL(activated()), this, SLOT(OnEditNextItem()));
  connect(editRemove_itemAction, SIGNAL(activated()), this, SLOT(OnRemoveItem()));
  connect(editShowClassAction, SIGNAL(activated()), this, SLOT(OnClassShow()));
  connect(checkBox8, SIGNAL(clicked()), this, SLOT(OnKnownFunctionsClick()));
  connect(editInsertQt3DesAction, SIGNAL(activated()), this, SLOT(OnQt3Des()));
  connect(editInsertQt4DesAction, SIGNAL(activated()), this, SLOT(OnQt4Des()));
  init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
documentation_mw::~documentation_mw()
{
  destroy();
  delete classShow;
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void documentation_mw::languageChange()
{
#if QT_VERSION < 0x040000
  setCaption(tr("Class description"));
  listView2->header()->setLabel(0, tr("Superclass"));
  listView2->header()->setLabel(1, tr("Documented"));
  listView1->header()->setLabel(0, tr("Name"));
  listView1->header()->setLabel(1, tr("return type"));
  listView1->header()->setLabel(2, tr("Visibility"));
  listView1->header()->setLabel(3, tr("Default"));
#else
  setWindowTitle(tr("Class description"));
  listView2->setHeaderLabels(QStringList() << tr("Superclass") << tr("Documented") << tr("Name")
                                           << tr("return type") << tr("Visibility")
                                           << tr("Default"));
#endif
  textLabel2->setText(tr("Classes"));
  textLabel3->setText(tr("Description"));
  pushButton1->setText(tr("apply"));
  checkBox1->setText(tr("class description"));
  checkBox8->setText(tr("Functions"));
  fileNewAction->setText(tr("New"));
  fileNewAction->setMenuText(tr("&New"));
  fileNewAction->setAccel(tr("Ctrl+N"));
  fileOpenAction->setText(tr("Open"));
  fileOpenAction->setMenuText(tr("&Open..."));
  fileOpenAction->setAccel(tr("Ctrl+O"));
  fileSaveAction->setText(tr("Save"));
  fileSaveAction->setMenuText(tr("&Save"));
  fileSaveAction->setAccel(tr("Ctrl+S"));
  fileSaveAsAction->setText(tr("Save As"));
  fileSaveAsAction->setMenuText(tr("Save &As..."));
  fileSaveAsAction->setAccel(QString::null);
  filePrintAction->setText(tr("Print"));
  filePrintAction->setMenuText(tr("&Print..."));
  filePrintAction->setAccel(tr("Ctrl+P"));
  fileExitAction->setText(tr("Exit"));
  fileExitAction->setMenuText(tr("E&xit"));
  fileExitAction->setAccel(QString::null);
  editUndoAction->setText(tr("Undo"));
  editUndoAction->setMenuText(tr("&Undo"));
  editUndoAction->setAccel(tr("Ctrl+Z"));
  editRedoAction->setText(tr("Redo"));
  editRedoAction->setMenuText(tr("&Redo"));
  editRedoAction->setAccel(tr("Ctrl+Y"));
  editCutAction->setText(tr("Cut"));
  editCutAction->setMenuText(tr("Cu&t"));
  editCutAction->setAccel(tr("Ctrl+X"));
  editCopyAction->setText(tr("Copy"));
  editCopyAction->setMenuText(tr("&Copy"));
  editCopyAction->setAccel(tr("Ctrl+C"));
  editPasteAction->setText(tr("Paste"));
  editPasteAction->setMenuText(tr("&Paste"));
  editPasteAction->setAccel(tr("Ctrl+V"));
  editFindAction->setText(tr("Find"));
  editFindAction->setMenuText(tr("&Find..."));
  editFindAction->setAccel(tr("Ctrl+F"));
  Action->setText(tr("Import"));
  Action->setAccel(tr("Ctrl+I"));
  editHTMLAction->setText(tr("HTML"));
  editHTMLAction->setMenuText(tr("HTML"));
  editHTMLAction->setAccel(tr("Alt+H"));
  editUpdateAction->setText(tr("Update"));
  editUpdateAction->setMenuText(tr("Update"));
  editUpdateAction->setAccel(tr("Alt+U"));
  fileCloseAction->setText(tr("Close"));
  fileCloseAction->setMenuText(tr("Close"));
  fileCloseAction->setAccel(tr("Alt+C"));
  fileOpen_DirectoryAction->setText(tr("Open Directory"));
  fileOpen_DirectoryAction->setMenuText(tr("Open Directory"));
  fileOpen_DirectoryAction->setAccel(tr("Alt+D"));
  editDependenciesAction->setText(tr("Dependencies"));
  editDependenciesAction->setMenuText(tr("Dependencies"));
  editDependenciesAction->setAccel(tr("Alt+X"));
  editHTML_allAction->setText(tr("HTML all"));
  editHTML_allAction->setMenuText(tr("HTML all"));
  helpKnown_ClassesAction->setText(tr("Known Classes"));
  helpKnown_ClassesAction->setMenuText(tr("Known Classes"));
  htmlboldAction->setText(tr("bold"));
  htmlboldAction->setMenuText(tr("bold"));
  htmlboldAction->setAccel(tr("Ctrl+Shift+B"));
  htmlitalicAction->setText(tr("italic"));
  htmlitalicAction->setMenuText(tr("italic"));
  htmlitalicAction->setAccel(tr("Ctrl+Shift+I"));
  htmlvarAction->setText(tr("var"));
  htmlvarAction->setMenuText(tr("var"));
  htmlvarAction->setAccel(tr("Ctrl+Shift+A"));
  htmlsuperscriptAction->setText(tr("superscript"));
  htmlsuperscriptAction->setMenuText(tr("superscript"));
  htmlsuperscriptAction->setAccel(tr("Alt+Shift+Up"));
  htmlsubscriptAction->setText(tr("subscript"));
  htmlsubscriptAction->setMenuText(tr("subscript"));
  htmlsubscriptAction->setAccel(tr("Alt+Shift+Down"));
  editApply_descriptionAction->setText(tr("Apply description"));
  editApply_descriptionAction->setMenuText(tr("Apply description"));
  editApply_descriptionAction->setAccel(tr("Alt+W"));
  editNext_itemAction->setText(tr("Next item"));
  editNext_itemAction->setMenuText(tr("Next item"));
  editNext_itemAction->setAccel(tr("Alt+N"));
  editRemove_itemAction->setText(tr("Remove item"));
  editRemove_itemAction->setMenuText(tr("Remove item"));
  editRemove_itemAction->setAccel(tr("Ctrl+0"));
  editShowClassAction->setText(tr("Show class definition"));
  editShowClassAction->setMenuText(tr("Show class definition"));
  editShowClassAction->setAccel(tr("Ctrl+Shift+L"));
  editInsertQt3DesAction->setMenuText(tr("Insert Qt3-mode"));
  editInsertQt3DesAction->setAccel(tr("Ctrl+3"));
  editInsertQt4DesAction->setMenuText(tr("Insert Qt4-mode"));
  editInsertQt4DesAction->setAccel(tr("Ctrl+4"));
  if (MenuBar->findItem(1))
    MenuBar->findItem(1)->setText(tr("&File"));
  if (MenuBar->findItem(2))
    MenuBar->findItem(2)->setText(tr("&Edit"));
  if (MenuBar->findItem(3))
    MenuBar->findItem(3)->setText(tr("Help"));
  if (MenuBar->findItem(4))
    MenuBar->findItem(4)->setText(tr("HTML"));
}
