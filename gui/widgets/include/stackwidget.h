#ifndef STACKWIDGET
#define STACKWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"
#include "materialparameter.h"
#if QT_VERSION < 0x040000
class QWidgetStack;
#else
class QStackedWidget;
#endif
class QComboBox;
class ShapeWidget;
class ReactionDisplay;
class QTabWidget;
using namespace std;

class StackWidget: public QWidget
{
    Q_OBJECT
public:
    ~StackWidget();
#if QT_VERSION < 0x040000
    StackWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    QWidgetStack* stackw;
#else
    StackWidget( QWidget* parent = 0, Qt::WindowFlags f=0);
    QStackedWidget* stackw;
#endif
    QComboBox* selector;
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void addWidget(QWidget* widget, const QString &widgetName);
    virtual void widgetSelect(int num);
 private:
    void languageChange();
};

#endif //STACKWIDGET
