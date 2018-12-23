//****************************************************************************
//** Form interface generated from reading ui file '/users/data12/pion/ehrhardt/Packages/ROOT_descriptionWidgets/dividedialog.ui'
//**
//** Created: Wed Jun 11 15:56:57 2008
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef DIVIDEDIALOG_H
#define DIVIDEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QPushButton;

class DivideDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    DivideDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    DivideDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~DivideDialog();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QSpinBox* spinBox2;
    QSpinBox* spinBox1;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    virtual int getX();
    virtual int getY();

public slots:
    virtual void setNumbers( int nX, int nY );
    virtual void accept();

protected:
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // DIVIDEDIALOG_H
