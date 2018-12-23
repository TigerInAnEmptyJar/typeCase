/****************************************************************************
** Form interface generated from reading ui file 'qvectorchangedialog.ui'
**
** Created: Wed Jan 9 10:17:14 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef QVECTORCHANGEDIALOG_H
#define QVECTORCHANGEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <vector>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QDialogButtonBox;
#include <QListWidget>
#endif
class QPushButton;
using namespace std;
class QVectorChangeDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    QVectorChangeDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    QListBox* listBox;
#else
    QVectorChangeDialog( QWidget* parent = 0, Qt::WindowFlags f=0);
    QListWidget* listBox;
#endif
    ~QVectorChangeDialog();

#if QT_VERSION < 0x040000
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
#else
    QDialogButtonBox* buttons;
#endif

    template<class X>
    vector<X> getVector();

    template<class X>
    static vector<X> getVector( QWidget * par, const QString & cap, const vector<X> & beg );

public slots:
    virtual void resize( int w, int h );
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void setVector( const vector<QString> & vec );
    virtual void setVector( const vector<int> & vec );
    virtual void setVector( const vector<float> & vec );
    virtual void setVector( const vector<bool> & vec );
#if QT_VERSION < 0x040000
    virtual void OnListBoxClick( int button, QListBoxItem * it );
#else
    virtual void OnListBoxClick();
    virtual void OnListBoxClick(const QPoint & p);
#endif
protected:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // QVECTORCHANGEDIALOG_H
