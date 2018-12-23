#ifndef DETECTORDISPLAY_H
#define DETECTORDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"

#if QT_VERSION > 0x040000
class QMenu;
#endif
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QLabel;
class QTextEdit;
class QListView;
class QListViewItem;
class QGroupBox;
class QFrame;
class QCanvasWidget;
class ShapeDisplay;
class base_shape;
class TObject;
using namespace std;

class DetectorDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    DetectorDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    DetectorDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~DetectorDisplay();

    QLabel* nameLabel;
    QLabel* idLabel;
    QLabel* NElLabel;
    QLabel* stackLabel;
    QLabel* matLabel;

    QWidget* nameInput;
    QSpinBox* idBox;
    QSpinBox* elBox;
    QSpinBox* stackBox;
    QComboBox* matBox;

    QCheckBox* circularBox;

    QGroupBox* propertyGroup;
    QCanvasWidget* shapeShow;
    ShapeDisplay *shap;

    virtual bool readOnly();
    virtual bool doDraw();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setDetector( detector_parameter & detectorIn );
    virtual void OnDetectorDraw();
    virtual void setReadOnly( bool value );
    virtual void onPropertyChange();
    virtual void setDraw(bool value);
    virtual void setFraction(float f);
    virtual void setMaterials(const vector<string>& mats);
    virtual void setDefinedShapes(vector<shape_parameter> shp);
    virtual void shapeChanged();
signals:
    void changed();

protected:
    virtual void mouseReleaseEvent ( QMouseEvent * e );
    detector_parameter *sp;
    base_shape *single;
    base_shape *drawer;
    vector<TObject*> drawings;
    bool ReadOnly;
    detector_parameter copy;
    float fraction;
    bool DrawShape;
    int currentShapeType;
    shape_parameter copyshape;
    vector<string> materials;
    vector<shape_parameter> defined_shapes;
    vector<shape_parameter> modShapes;
protected slots:
  virtual void shapeChanged(int num);
#if QT_VERSION > 0x040000
  virtual void onshapeChanged(QAction* ac);
#endif
  virtual void mouseOnCanvas(const QPoint &p);
 virtual void languageChange();

private:
#if QT_VERSION > 0x040000
 QMenu *tmpMenu;
#endif
    point2D pointOnCanvas(const QPoint &p);
    void init();
    void destroy();

};

#endif // DETECTORDISPLAY_H
