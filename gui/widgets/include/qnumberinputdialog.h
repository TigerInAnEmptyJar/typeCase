/****************************************************************************
** Form interface generated from reading ui file 'qnumberinputdialog.ui'
**
** Created: Wed Oct 25 11:53:22 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.5   edited Aug 31 12:13 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QNUMBERINPUTDIALOG_H
#define QNUMBERINPUTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLineEdit;
#if QT_VERSION > 0x040000
class QDialogButtonBox;
#endif

class QNumberInputDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    QNumberInputDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    QNumberInputDialog( QWidget* parent = 0, Qt::WindowFlags f=0);
#endif
    ~QNumberInputDialog();

#if QT_VERSION < 0x040000
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
#else
    QDialogButtonBox* buttons;
#endif
    QLineEdit* lineEdit;

    virtual double line();
    static double getLine( QWidget * par, const QString & capt ,float start=0);
public slots:
    virtual void setStart(float n);
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );

protected:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // QNUMBERINPUTDIALOG_H
