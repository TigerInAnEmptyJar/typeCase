///****************************************************************************
//** Form interface generated from reading ui file 'colorselectdialog.ui'
//**
//** Created: Wed Sep 23 19:53:51 2009
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef COLORSELECTDIALOG_H
#define COLORSELECTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qpushbutton.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class TColor;
using namespace std;

class ColorSelectDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ColorSelectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    ColorSelectDialog( QWidget* parent = 0,  Qt::WindowFlags f = 0);
#endif
    ~ColorSelectDialog();


    virtual int getColor();
    virtual QColor getRGBColor();
    static QColor Root2Qt( int colo );
    static int Qt2Root( const QColor & c );
    static QColor Root2Qt( const TColor & c );

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void accept();
    virtual void OnButtonDown();

protected:
    int selectedColor;
    QPushButton**buttons;


protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // COLORSELECTDIALOG_H
