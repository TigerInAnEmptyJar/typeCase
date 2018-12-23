///****************************************************************************
//** Form interface generated from reading ui file 'doc_selectclassesdialog.ui'
//**
//** Created: Wed Jul 2 15:48:13 2008
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef DOC_SELECTCLASSESDIALOG_H
#define DOC_SELECTCLASSESDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include "tmyclass.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
class QDialogButtonBox;
#endif
class QLabel;
class QToolButton;

class doc_selectClassesDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    doc_selectClassesDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    doc_selectClassesDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~doc_selectClassesDialog();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QToolButton* toolButton2;
    QToolButton* toolButton1;
    QGridLayout *flayout;
#if QT_VERSION < 0x040000
    QListBox* listBox1;
    QListBox* listBox2;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QHBoxLayout *Layout1;
    QSpacerItem *Horizontal_Spacing2;
#else
    QListWidget* listBox1;
    QListWidget* listBox2;
    QDialogButtonBox *buttons;
#endif

public slots:
    virtual void setClasses( const vector<TMyClass> & classes );
    virtual void getClasses( vector<string> & ret );
    virtual void OnUseButtonClick();
    virtual void OnUnUseButtonClick();

protected:

protected slots:
    virtual void languageChange();

};

#endif // DOC_SELECTCLASSESDIALOG_H
