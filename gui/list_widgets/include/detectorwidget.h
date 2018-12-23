#ifndef DETECTORWIDGET
#define DETECTORWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"
class DetectorDisplay;
class QFrame;
class QCanvasWidget;
class QCheckBox;
class TObject;
class volumeShape;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
using namespace std;

class DetectorWidget: public QWidget
{
    Q_OBJECT
      
public:
#if QT_VERSION < 0x040000
    DetectorWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    DetectorWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~DetectorWidget();
protected:
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    QFrame* line;
    DetectorDisplay* pdisplay;
    QCanvasWidget *canvas;
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setDetectors( vector<detector_parameter> &algoIn );
    virtual void setDetectors( vector<detector_parameter*> &algoIn );
    virtual void setReadOnly( bool value );
/*     virtual void detectorSelect(detector_parameter &algo, bool value); */
    virtual void mouseMoveEvent( QMouseEvent * e );
    virtual void setMaterials(const vector<string>& mats);
 virtual void detectorChanged();
 virtual void deleteDetector();
 virtual void addDetector();
 virtual void setDefinedShapes(vector<shape_parameter> shp);
protected slots:
#if QT_VERSION < 0x040000
 virtual void detectorRightClicked(QListViewItem* it,const QPoint& p) ;
  virtual void onDetectorSelect(QListViewItem* it);
#else
 virtual void detectorRightClicked(QTreeWidgetItem* it,const QPoint& p) ;
  virtual void onDetectorSelect(QTreeWidgetItem* it,QTreeWidgetItem* prev);
#endif
/*  virtual void detectorChecked(QListViewItem* it); */
/*  virtual void detectorClicked(int b,QListViewItem* it,const QPoint& p, int c); */
 virtual void moveOnCanvas(const QPoint &p);
 virtual void drawDetectors();
 virtual void canvasRightClick(const QPoint& p);
 virtual void setHatchMode();
 virtual void printCanvas();
signals:
    void changed();
    void detectorSelected(detector_parameter &algo, bool value);
protected:
    bool ReadOnly;
    vector<detector_parameter> *ap1;
    vector<detector_parameter*> *ap2;
    vector<vector<TObject*> > drawings;
    vector<vector<volumeShape*> > drawnObjects;
    vector<int> drawPos;
    float fraction;
    int mode;
    int hatch;
    int selected;
#if QT_VERSION < 0x040000
    detector_parameter* getParameterFromItem(QListViewItem*it);
    int getPositionFromItem(QListViewItem*it);
    QListViewItem* getItemFromParameter(detector_parameter*al);
    int drawDetector(QListViewItem *it,vector4D &bbmax);
#else
    detector_parameter* getParameterFromItem(QTreeWidgetItem*it);
    int getPositionFromItem(QTreeWidgetItem*it);
    QTreeWidgetItem* getItemFromParameter(detector_parameter*al);
    int drawDetector(QTreeWidgetItem *it,vector4D &bbmax);
#endif
    point2D pointOnCanvas(const QPoint &p);
 private:
#if QT_VERSION > 0x040000
    QTreeWidgetItem* current;
#endif
    void init();
    void destroy();
    void languageChange();
};

#endif //DETECTORWIDGET
