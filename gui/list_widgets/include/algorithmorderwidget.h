#ifndef ALGORITHMORDERWIDGET
#define ALGORITHMORDERWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "algorithmparameter.h"
class AlgorithmDisplay;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
#endif
class QFrame;
class QCheckBox;
using namespace std;

class AlgorithmOrderWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    AlgorithmOrderWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
   AlgorithmOrderWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
   ~AlgorithmOrderWidget();
#if QT_VERSION < 0x040000
    QListBox* listView;
#else
    QListWidget* listView;
#endif
    QFrame* line;
    AlgorithmDisplay* pdisplay;
    vector<algorithm_parameter*> getOrder();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setAlgorithms( vector<algorithm_parameter> &algoIn );
    virtual void setAlgorithms( vector<algorithm_parameter*> &algoIn );
    virtual void algorithmSelect(algorithm_parameter &algo, bool value);
    virtual void algorithmAdd(algorithm_parameter *algo);
protected slots:
#if QT_VERSION < 0x040000
  virtual void onAlgorithmSelect(QListBoxItem* it); 
 virtual void onListMenu(QListBoxItem* it, const QPoint & pos);
#else
  virtual void onAlgorithmSelect(QListWidgetItem* it); 
  virtual void onListMenu(const QPoint &pos);
#endif
  virtual void algorithmSave();
 virtual void onAlgorithmDown(int n);
 virtual void onAlgorithmUp(int n);
 virtual void onAlgorithmPos1(int n);
 virtual void onAlgorithmEnd(int n);
 virtual void onAlgorithmDelete(int n);
signals:
    void changed();
    void algorithmSelected(algorithm_parameter &algo, bool value);
protected:
    bool ReadOnly;
    vector<algorithm_parameter> *ap1;
    vector<algorithm_parameter*> *ap2;
    float fraction;
    int mode;
    vector<algorithm_parameter*> added;
    vector<bool> untaken;
#if QT_VERSION < 0x040000
    algorithm_parameter* getParameterFromItem(QListBoxItem*it);
    QListBoxItem* getItemFromParameter(algorithm_parameter*al);
#else
    algorithm_parameter* getParameterFromItem(QListWidgetItem*it);
    QListWidgetItem* getItemFromParameter(algorithm_parameter*al);
#endif
    virtual void mouseMoveEvent( QMouseEvent * e );
    virtual void keyPressEvent( QKeyEvent * e );
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //ALGORITHMORDERWIDGET
