#ifndef SHAPEDISPLAY_H
#define SHAPEDISPLAY_H

#include <qwidget.h>
#include "shapeparameter.h"

#if QT_VERSION < 0x040000
#include <qvariant.h>
#else
class QResizeEvent;
class QGridLayout;
#endif
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
class QListView;
class QListViewItem;
class QGroupBox;
class QFrame;
class QCanvasWidget;
class base_shape;
using namespace std;

class ShapeDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ShapeDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ShapeDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~ShapeDisplay();

    QLabel* nameLabel;
    QWidget* nameInput;
    QLabel* descLabel;

    QTextEdit* descriptionEdit;
#if QT_VERSION < 0x040000
    QGroupBox* propertyGroup;
#else
    QGroupBox* helpGroup;
    QGridLayout* propertyGroup;
#endif
    QCanvasWidget* shapeShow;
    vector<QWidget*> properties;
    virtual bool readOnly();
    virtual bool isConstant();
    virtual bool doDraw();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setShape( shape_parameter & shapeIn );
    virtual void OnShapeDraw();
    virtual void setReadOnly( bool value );
    virtual void setConstant( bool value );
    virtual void onPropertyChange();
    virtual void setDraw(bool value);
    virtual void setFraction(float f);
    virtual void mouseOnCanvas(const QPoint &p);
signals:
    void changed();

protected:
    shape_parameter *sp;
    base_shape *drawer;
    bool ReadOnly;
    bool IsConstant;
    shape_parameter copy;
    float fraction;
    bool DrawShape;
    void addItemToLayout(int tp, string name,string widgetName);
protected slots:
    virtual void languageChange();

private:
 point2D pointOnCanvas(const QPoint &p);
    void init();
    void destroy();

};

#endif // SHAPEDISPLAY_H
