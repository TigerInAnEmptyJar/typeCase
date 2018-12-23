/****************************************************************************
** Form interface generated from reading ui file 'materialdisplay.ui'
**
** Created: Mon Nov 30 15:59:13 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MATERIALDISPLAY_H
#define MATERIALDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "materialparameter.h"

#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QFrame;
class QLabel;
class QCheckBox;
class QLineEdit;
class QTextEdit;
using namespace std;

class MaterialDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    MaterialDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    MaterialDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~MaterialDisplay();

#if QT_VERSION < 0x040000
    QListView* listView1;
#else
    QTreeWidget* listView1;
#endif
    QFrame* frame1;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QCheckBox* checkBox1;
    QLabel* textLabel3;
    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QLineEdit* lineEdit3;
    QTextEdit* textEdit1;

    virtual bool readOnly() const;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setMode( int m );
    virtual void setMaterial(material_parameter &material);
#if QT_VERSION < 0x040000
    virtual void OnElementsRightClick( QListViewItem * it, const QPoint & p );
#else
    virtual void OnElementsRightClick( const QPoint & p );
    virtual void OnElementInsert();
    virtual void OnElementSet();
#endif
    virtual void OnElementInsert( int id );
    virtual void OnElementSet( int id );
    virtual void OnElementRemove();
    virtual void OnSetElementWeight();
    virtual void OnCustom();
    virtual void setReadOnly(bool value);
    virtual void onPropertyChange();
protected:
#if QT_VERSION < 0x040000
    QPopupMenu elementSet;
    QPopupMenu elementMenu;
#else
    QMenu elementSet;
    QMenu elementMenu;
#endif
    material_parameter *mp;
    material_parameter copy;
    int selected;
    int mode;
    bool ReadOnly;
    QGridLayout* layout;
 signals:
    void changed(); 
protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // MATERIALDISPLAY_H
