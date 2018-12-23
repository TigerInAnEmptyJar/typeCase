//****************************************************************************
//** Form interface generated from reading ui file 'attributewidget.ui'
//**
//** Created: Wed Sep 23 19:53:50 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <qvariant.h>
#include <qwidget.h>
#include <iostream>
#if QT_VERSION < 0x040000
#include <iostream.h>
#endif
#include <fstream>
#include <TAttFill.h>
#include <TAttMarker.h>
#include <TAttText.h>
#include <TAttLine.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSpinBox;
class QComboBox;
class QLabel;
class QPushButton;
class QResizeEvent;
using namespace std;

class AttributeWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    AttributeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    AttributeWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~AttributeWidget();

    QSpinBox* spinBox1;
    QComboBox* comboBox1;
    QSpinBox* spinBox2;
    QLabel* textLabel4;
    QLabel* textLabel5;
    QSpinBox* spinBox3;
    QLabel* textLabel7;
    QLabel* textLabel8;
    QSpinBox* spinBox5;
    QComboBox* comboBox5;
    QComboBox* comboBox6;
    QPushButton* colorButton;
    QLabel* textLabel2;
    QLabel* textLabel1;
    QLabel* textLabel3;
    QComboBox* comboBox4;
    QComboBox* comboBox2;
    QComboBox* comboBox3;
    QSpinBox* spinBox4;
    QLabel* textLabel6;

    virtual TAttText * getText();
    virtual TAttMarker * getMarker();
    virtual TAttLine * getLine();
    virtual TAttFill * getFill();
    virtual int getFrameStyle();
    virtual int getFrameSize();

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setMode( int modeIn );
    virtual void setAtt( TAttFill * fill, TAttLine * line, TAttMarker * marker, TAttText * textIn, int framewidth, int framestyle );
    virtual void OnColorButtonClick();

protected:

protected slots:
    virtual void languageChange();
private:
    int mode;
    TAttLine fline;
    TAttFill ffill;
    TAttMarker fmarker;
    TAttText ftext;
    int rColor;

    void init();

};

#endif // ATTRIBUTEWIDGET_H
