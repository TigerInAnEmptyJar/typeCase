//****************************************************************************
//** Form interface generated from reading ui file 'stylewidget.ui'
//**
//** Created: Wed Sep 23 19:53:51 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H

#include <qvariant.h>
#include <qwidget.h>
#include <TAttPad.h>
#include <TAttAxis.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QToolBox;
class QGroupBox;
class QLineEdit;
using namespace std;

class StyleWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    StyleWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    StyleWidget( QWidget* parent= 0,Qt::WindowFlags f=0);
#endif
    ~StyleWidget();

    QToolBox* toolBox1;
    QWidget* page1;
    QWidget* page2;
    QWidget* page;
    QGroupBox* groupBox1;
    QLineEdit* lineEdit1_2;
    QLineEdit* lineEdit1_3;
    QLineEdit* lineEdit1_4;
    QLineEdit* lineEdit1_1;
    QWidget* page_2;
    QWidget* page_3;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );

protected:

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // STYLEWIDGET_H
