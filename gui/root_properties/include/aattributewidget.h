//****************************************************************************
//** Form interface generated from reading ui file 'aattributewidget.ui'
//**
//** Created: Wed Sep 23 19:53:51 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef AATTRIBUTEWIDGET_H
#define AATTRIBUTEWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <TAttPad.h>
#include <iostream>
#include <TAttAxis.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class Q3DInput;
class AttributeWidget;
class QLineEdit;
class QLabel;
class QPushButton;
class QGroupBox;
class QComboBox;
using namespace std;

class AAttributeWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    AAttributeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    AAttributeWidget( QWidget* parent = 0, Qt::WindowFlags f = 0);
#endif
    ~AAttributeWidget();

    Q3DInput* q3DInput1;
    QLineEdit* lineEdit1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QPushButton* colorButton;
    QLabel* textLabel1;
    QGroupBox* groupBox5;
    AttributeWidget* attributeWidget2;
    QGroupBox* groupBox6;
    AttributeWidget* attributeWidget3;
    QGroupBox* groupBox3;
    QLineEdit* lineEdit3_2;
    QComboBox* comboBox2_1;
    QComboBox* comboBox2_2;
    QLabel* textLabel4_2;
    QLineEdit* lineEdit3_1;
    QLabel* textLabel5_2;
    QGroupBox* groupBox2;
    QLineEdit* lineEdit2_2;
    QComboBox* comboBox1_1;
    QComboBox* comboBox1_2;
    QLineEdit* lineEdit2_1;
    QLabel* textLabel5;
    QLabel* textLabel4;
    QGroupBox* groupBox1;
    QLineEdit* lineEdit1_1;
    QLineEdit* lineEdit1_2;
    QLineEdit* lineEdit1_3;
    QLineEdit* lineEdit1_4;
    QGroupBox* groupBox4;
    AttributeWidget* attributeWidget1;

    virtual TAttPad * getPad();
    virtual TAttAxis * getAxis();

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setMode( int m );
    virtual void setAttribute( TAttPad * pad, TAttAxis * axis );
    virtual void OnColorButtonClick();

protected:
    TAttPad fpad;
    int mode;
    TAttAxis faxis;
    int rColor;


protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

    void init();

};

#endif // AATTRIBUTEWIDGET_H
