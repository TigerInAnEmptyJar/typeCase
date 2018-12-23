#ifndef AHITTREEOUTPUTWIDGET
#define AHITTREEOUTPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QListBox;
class QListBoxItem;
class QComboBox;
class QCheckBox;
class QLabel;

class QAHitTreeOutputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *minimum;
 public:
#if QT_VERSION < 0x040000
  QAHitTreeOutputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAHitTreeOutputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAHitTreeOutputWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
};

#endif
