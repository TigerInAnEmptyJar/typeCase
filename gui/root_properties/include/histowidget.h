//****************************************************************************
//** Form interface generated from reading ui file 'histowidget.ui'
//**
//** Created: Wed Sep 23 19:53:51 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef HISTOWIDGET_H
#define HISTOWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <TObject.h>
#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttMarker.h>
#include <TAttAxis.h>
#include "aattributewidget.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class AttributeWidget;
class QLineEdit;
class QLabel;
class QPushButton;
class QToolBox;
class QGroupBox;
class QCheckBox;
class QComboBox;
using namespace std;

class HistoWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    HistoWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
   HistoWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~HistoWidget();

    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QLabel* textLabel2;
    QLabel* textLabel1;
    QPushButton* pushButton1;
    QPushButton* pushButton2;
    QToolBox* toolBox1;
    QWidget* page1;
    QGroupBox* groupBox3;
    AttributeWidget* attributeWidget3;
    QCheckBox* checkBox1;
    QGroupBox* groupBox1;
    AttributeWidget* attributeWidget1;
    QGroupBox* groupBox2;
    AttributeWidget* attributeWidget2;
    QGroupBox* groupBox4;
    QComboBox* comboBox6;
    QLabel* textLabel21;
    QLabel* textLabel23;
    QLineEdit* lineEdit3;
    QLabel* textLabel22;
    QWidget* page2;
    QLabel* textLabel2_1;
    QLabel* textLabel1_3;
    QLabel* textLabel1_5;
    QLabel* textLabel1_4;
    QLabel* textLabel1_6;
    QLabel* textLabel1_8;
    QLabel* textLabel1_7;
    QLabel* textLabel1_9;
    QLabel* textLabel1_11;
    QLabel* textLabel1_10;
    QLabel* textLabel1_12;
    QLabel* textLabel1_13;
    QLabel* textLabel1_15;
    QLabel* textLabel1_14;
    QLabel* textLabel1_16;
    QLabel* textLabel1_2;

    AAttributeWidget *advAttributeWidget1;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void getHisto( TObject * histo );
    virtual void OnAxisSelect( int i );
    virtual void OnResetClick();
    virtual void OnApplyClick();

protected:
    int xN,yN,zN;
    int actualAxis;
    string xAt,yAt,zAt;
    TAttAxis xA,yA,zA;
    TAttMarker marker;
    TAttFill fill;
    TAttLine line;
    bool createErrors;
    TObject* histogram;


protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // HISTOWIDGET_H
