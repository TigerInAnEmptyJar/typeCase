#ifndef ALGORITHMWIDGET
#define ALGORITHMWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "algorithmparameter.h"
class AlgorithmDisplay;
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

class AlgorithmWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    AlgorithmWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    AlgorithmWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~AlgorithmWidget();
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    QFrame* line;
    AlgorithmDisplay* pdisplay;
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setAlgorithms( vector<algorithm_parameter> &algoIn );
    virtual void setAlgorithms( vector<algorithm_parameter*> &algoIn );
    virtual void setReadOnly( bool value );
    virtual void algorithmSelect(algorithm_parameter &algo, bool value);
    virtual void mouseMoveEvent( QMouseEvent * e );
protected slots:
#if QT_VERSION < 0x040000
  virtual void onAlgorithmSelect(QListViewItem* it);
 virtual void algorithmChecked(QListViewItem* it);
 virtual void algorithmClicked(int b,QListViewItem* it,const QPoint& p, int c);
#else
  virtual void onAlgorithmSelect(QTreeWidgetItem* it);
 virtual void algorithmChecked(QTreeWidgetItem* it);
 virtual void algorithmClicked(QTreeWidgetItem* it);
#endif
signals:
    void changed();
    void algorithmSelected(algorithm_parameter &algo, bool value);
protected:
    bool ReadOnly;
    vector<algorithm_parameter> *ap1;
    vector<algorithm_parameter*> *ap2;
    float fraction;
    int mode;
#if QT_VERSION < 0x040000
    algorithm_parameter* getParameterFromItem(QListViewItem*it);
    QListViewItem* getItemFromParameter(algorithm_parameter*al);
#else
    algorithm_parameter* getParameterFromItem(QTreeWidgetItem*it);
    QTreeWidgetItem* getItemFromParameter(algorithm_parameter*al);
#endif
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //ALGORITHMWIDGET
