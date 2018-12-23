#ifndef MATERIALWIDGET
#define MATERIALWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "materialparameter.h"
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class MaterialDisplay;
class QFrame;
class QCheckBox;
using namespace std;

class MaterialWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    MaterialWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    MaterialWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~MaterialWidget();
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    QFrame* line;
    MaterialDisplay* pdisplay;
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setMaterials( vector<material_parameter> &algoIn );
    virtual void setMaterials( vector<material_parameter*> &algoIn );
    virtual void setReadOnly( bool value );
/*     virtual void materialSelect(material_parameter &algo, bool value); */
    virtual void mouseMoveEvent( QMouseEvent * e );
protected slots:
#if QT_VERSION < 0x040000
  virtual void onMaterialSelect(QListViewItem* it);
 virtual void materialRightClicked(QListViewItem* it,const QPoint& p) ;
#else
  virtual void onMaterialSelect(QTreeWidgetItem* it);
 virtual void materialRightClicked(const QPoint& p) ;
#endif
 virtual void materialChanged();
 virtual void deleteMaterial();
 virtual void addMaterial();
signals:
    void changed();
protected:
    bool ReadOnly;
    vector<material_parameter> *ap1;
    vector<material_parameter*> *ap2;
    float fraction;
    int mode;
    int selected;
#if QT_VERSION < 0x040000
    material_parameter* getParameterFromItem(QListViewItem*it);
    QListViewItem* getItemFromParameter(material_parameter*al);
#else
    material_parameter* getParameterFromItem(QTreeWidgetItem*it);
    QTreeWidgetItem* getItemFromParameter(material_parameter*al);
#endif
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //MATERIALWIDGET
