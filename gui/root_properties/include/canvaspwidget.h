//****************************************************************************
//** Form interface generated from reading ui file 'canvaspwidget.ui'
//**
//** Created: Wed Sep 23 19:53:50 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef CANVASPWIDGET_H
#define CANVASPWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <TCanvas.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class AttributeWidget;
class QLabel;
class QComboBox;
class QGroupBox;
class QButtonGroup;
class QCheckBox;
class QLineEdit;
class QPushButton;
using namespace std;

class CanvasPWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    CanvasPWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    CanvasPWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~CanvasPWidget();

    QLabel* textLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QComboBox* comboBox1;
    QGroupBox* groupBox2;
    AttributeWidget* attributeWidget1;
    QGroupBox* groupBox3;
    AttributeWidget* attributeWidget2;
#if QT_VERSION > 0x040000
    QGroupBox* buttonGroup1;
#else
    QButtonGroup* buttonGroup1;
#endif
    QCheckBox* checkBox2;
    QCheckBox* checkBox3;
    QCheckBox* checkBox1;
    QGroupBox* groupBox1;
    QLabel* textLabel6;
    QLabel* textLabel7;
    QLineEdit* lineEdit4;
    QLineEdit* lineEdit5;
    QLineEdit* lineEdit6;
    QLineEdit* lineEdit3;
    QLabel* textLabel4;
    QLabel* textLabel5;
    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QPushButton* pushButton2;
    QPushButton* pushButton4;
    QPushButton* pushButton3;
    QPushButton* pushButton1;
    QPushButton* pushButton5;
    QPushButton* pushButton6;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void getCanvas( TCanvas * canv );
    virtual void OnUpdateClick();
    virtual void OnApplyClick();
    virtual void OnHelpClick();
    virtual void OnPadSelect();
    virtual void OnClearClick();
    virtual void OnDivideClick();

signals:
    void helpAsked(const QString &);

protected:
    TCanvas *canvas;
    vector<TPad*> subPads;


protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // CANVASPWIDGET_H
