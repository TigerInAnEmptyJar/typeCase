/****************************************************************************
** Form interface generated from reading ui file 'qstringinputdialog.ui'
**
** Created: Tue Jul 19 10:19:43 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.2   edited Nov 24 13:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QSTRINGINPUTDIALOG_H
#define QSTRINGINPUTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QPushButton;
#if QT_VERSION > 0x040000
class QDialogButtonBox;
#endif
using namespace std;
class QStringInputDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    QStringInputDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    QStringInputDialog( QWidget* parent = 0, Qt::WindowFlags f=0);
#endif
    ~QStringInputDialog();

    QLineEdit* lineEdit;
#if QT_VERSION < 0x040000
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
#else
    QDialogButtonBox* buttons;
#endif

    virtual QString line();
    static QString getLine( QWidget * par, const QString & capt , const QString &start=QString());

    public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
  virtual void setStart(const QString & n);
protected:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // QSTRINGINPUTDIALOG_H
