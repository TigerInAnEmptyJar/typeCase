/****************************************************************************
** Form interface generated from reading ui file 'qintegerinputdialog.ui'
**
** Created: Tue Jul 19 10:45:24 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.2   edited Nov 24 13:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QINTEGERINPUTDIALOG_H
#define QINTEGERINPUTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QSpinBox;
#if QT_VERSION > 0x040000
class QDialogButtonBox;
#endif
class QGridLayout;
using namespace std;
class QIntegerInputDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    QIntegerInputDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    QIntegerInputDialog( QWidget* parent = 0, Qt::WindowFlags f=0);
#endif
    ~QIntegerInputDialog();

#if QT_VERSION < 0x040000
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
#else
    QDialogButtonBox* buttons;
#endif
    QSpinBox* spinBox5;

    virtual int line();
    static int getLine( QWidget * par, const QString & capt ,int start=0);
    public slots:
  virtual void setStart(int n);
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );

protected:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // QINTEGERINPUTDIALOG_H
