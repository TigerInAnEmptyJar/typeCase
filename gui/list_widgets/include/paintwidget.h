#ifndef PAINTWIDGET
#define PAINTWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "paintparameter.h"
class PaintDisplay;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QFrame;
class QCheckBox;
using namespace std;

class PaintWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    PaintWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    PaintWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~PaintWidget();
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    QFrame* line;
    PaintDisplay* pdisplay;
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setPaints( vector<vector<paint_parameter> > &algoIn );
    virtual void setPaints( vector<paint_parameter> &algoInT, vector<paint_parameter> &algoInP, vector<paint_parameter> &algoInH);
    virtual void setReadOnly( bool value );
    virtual void mouseMoveEvent( QMouseEvent * e );
protected slots:
#if QT_VERSION < 0x040000
  virtual void onPaintSelect(QListViewItem* it);
 virtual void paintRightClicked(QListViewItem* it,const QPoint& p) ;
#else
  virtual void onPaintSelect(QTreeWidgetItem* it);
 virtual void paintRightClicked(const QPoint& p) ;
 virtual void onPaint(QAction* ac);
#endif
 virtual void paintChanged();
 virtual void deletePaint(int num);
 virtual void addPaint(int num);
signals:
    void changed();
protected:
    bool ReadOnly;
    vector<vector<paint_parameter> > *ap1;
    vector<vector<paint_parameter>*> ap2;
    float fraction;
    int mode;
    int selected;
    int selectedT;
#if QT_VERSION < 0x040000
    paint_parameter* getParameterFromItem(QListViewItem*it, int &n, int &m);
    QListViewItem* getItemFromParameter(paint_parameter*al);
#else
    QTreeWidgetItem *current;
    paint_parameter* getParameterFromItem(QTreeWidgetItem*it, int &n, int &m);
    QTreeWidgetItem* getItemFromParameter(paint_parameter*al);
#endif
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //PAINTWIDGET
