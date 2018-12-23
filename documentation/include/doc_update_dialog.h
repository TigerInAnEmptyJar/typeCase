#ifndef DOCUPDATEDIALOG_H
#define DOCUPDATEDIALOG_H

class QGridLayout;
class QPushButton;
class QLabel;
class TMyClass;
class nameTypeDoc;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
class QListView;
class QListViewItem;
#else
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
#endif

#include <qdialog.h>


class doc_update_dialog:public QDialog
{
Q_OBJECT
  private:
 TMyClass* oldClass;
 TMyClass* newClass;
 QPushButton *assignButton;
 QPushButton *helpButton;
 QPushButton *okButton;
 QPushButton *cancelButton;
 QLabel *oldLabel;
 QLabel *newLabel;
#if QT_VERSION < 0x040000
 QListBox *oldBox;
 QListBox *newBox;
 QListView *togetherView;
#else
 QListBox *oldBox;
 QListBox *newBox;
 QListView *togetherView;
#endif
 QGridLayout *flayout;
 void checkBoxes();
 nameTypeDoc* getDoc(int tp,const QString &nme);
#if QT_VERSION < 0x040000
 void addProps2Box(QListBox* lb, TMyClass *cls);
#else
 void addProps2Box(QListWidget* lb, TMyClass *cls);
#endif
 public:
#if QT_VERSION < 0x040000
 doc_update_dialog(QWidget* parent, bool modal=false,WFlags fl=0);
#else
 doc_update_dialog(QWidget* parent, Qt::WindowFlags fl=0);
#endif
 virtual ~doc_update_dialog();
 void setOldClass(TMyClass* old);
 void setNewClass(TMyClass* nCl);
 bool classesMatch();
 public slots:
   virtual void accept();
 virtual void onAssignClick();
#if QT_VERSION < 0x040000
 virtual void onOldClick(QListBoxItem *it);
 virtual void onNewClick(QListBoxItem *it);
 virtual void onTogeterClick(QListViewItem* it);
#else
 virtual void onOldClick(QListWidgetItem *it);
 virtual void onNewClick(QListWidgetItem *it);
 virtual void onTogeterClick(QTreeWidgetItem* it);
#endif
};

#endif
