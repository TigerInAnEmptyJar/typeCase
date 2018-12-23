/****************************************************************************
** Form interface generated from reading ui file 'q3dinputdialog.ui'
**
** Created: Wed Oct 25 09:31:39 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.5   edited Aug 31 12:13 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef Q3DINPUTDIALOG_H
#define Q3DINPUTDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include "q3Dinput.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class Q3DInput;
class QPushButton;
#if QT_VERSION > 0x040000
class QDialogButtonBox;
#endif
class QGridLayout;
#include "geometry.h"

class Q3DInputDialog : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    Q3DInputDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    Q3DInputDialog( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~Q3DInputDialog();

    Q3DInput* q3DInput1;
#if QT_VERSION < 0x040000
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
#else
    QDialogButtonBox *buttons;
#endif

    virtual point3D line();
    static point3D getLine( QWidget * parentIn, const QString & captionIn, const point3D &start=point3D());

public slots:
    virtual void resize( int w, int h );
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );

protected:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();
    void destroy();

};

#endif // Q3DINPUTDIALOG_H
