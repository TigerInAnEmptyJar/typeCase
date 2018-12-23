/****************************************************************************
** Form interface generated from reading ui file 'algorithmdisplay.ui'
**
** Created: Mon Nov 30 15:59:14 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ALGORITHMDISPLAY_H
#define ALGORITHMDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "algorithmparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QFrame;
using namespace std;

class AlgorithmDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    AlgorithmDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    AlgorithmDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif

    ~AlgorithmDisplay();

    QLabel* textLabel1;
    QTextEdit* textEdit1;
#if QT_VERSION < 0x040000
    QListView* listView1;
#else
    QTreeWidget* listView1;
#endif
    QFrame* line1;

    virtual bool readOnly();
    virtual bool isConstant();

public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setAlgorithm( algorithm_parameter * algoIn );
    virtual void OnParameterTypeShow();
#if QT_VERSION < 0x040000
    virtual void OnParameterRightClick( QListViewItem * it );
#else
    virtual void OnParameterRightClick( const QPoint &p);
#endif
    virtual void setReadOnly( bool value );
    virtual void setConstant( bool value );
    virtual void mouseMoveEvent( QMouseEvent * e );

signals:
    void changed();

protected:
    algorithm_parameter *ap;
    bool ReadOnly;
    bool IsConstant;
    algorithm_parameter copy;
    float fraction;


protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ALGORITHMDISPLAY_H
