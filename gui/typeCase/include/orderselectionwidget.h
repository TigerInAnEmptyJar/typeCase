#ifndef ORDERSELECTIONWIDGET
#define ORDERSELECTIONWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"
#include "materialparameter.h"
class AlgorithmOrderWidget;
class QComboBox;
class StackWidget;
class QTabWidget;
class ReactionRecognitionWidget;
class algorithm_parameter;
using namespace std;

class OrderSelectionWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    OrderSelectionWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    OrderSelectionWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~OrderSelectionWidget();
    QTabWidget* tabWidget;
    AlgorithmOrderWidget* order;
    QComboBox *inInput;
    QComboBox *outInput;
    StackWidget *inStack;
    StackWidget *outStack;
    ReactionRecognitionWidget *reactions;
    vector<algorithm_parameter*> getOrder();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setAlgorithms(vector<algorithm_parameter> &a);
    virtual void setAlgorithms(vector<algorithm_parameter*> &a);
    virtual void algorithmSelect(algorithm_parameter &algo, bool value);
protected slots:
/*   virtual void tabPageChanged(); */
signals:
    void changed();
    void algorithmSelected(algorithm_parameter &algo, bool value);
    void helpRequested(const string &filename);
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //ORDERSELECTIONWIDGET
