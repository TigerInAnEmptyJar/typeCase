#ifndef SHAPEWIDGET
#define SHAPEWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "shapeparameter.h"
class ShapeDisplay;
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

class ShapeWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    ShapeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ShapeWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~ShapeWidget();
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    QFrame* line;
    ShapeDisplay* pdisplay;
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setShapes( vector<shape_parameter> &algoIn );
    virtual void setShapes( vector<shape_parameter*> &algoIn );
    virtual void setReadOnly( bool value );
    virtual void mouseMoveEvent( QMouseEvent * e );
protected slots:
#if QT_VERSION < 0x040000
  virtual void onShapeSelect(QListViewItem* it);
#else
  virtual void onShapeSelect(QTreeWidgetItem* it);
#endif
signals:
    void changed();
protected:
    bool ReadOnly;
    vector<shape_parameter> *ap1;
    vector<shape_parameter*> *ap2;
    float fraction;
    int mode;
#if QT_VERSION < 0x040000
    shape_parameter* getParameterFromItem(QListViewItem*it);
    QListViewItem* getItemFromParameter(shape_parameter*al);
#else
    shape_parameter* getParameterFromItem(QTreeWidgetItem*it);
    QTreeWidgetItem* getItemFromParameter(shape_parameter*al);
#endif
 private:
    point2D pointOnCanvas(const QPoint &p);
    void init();
    void destroy();
    void languageChange();
};

#endif //SHAPEWIDGET
