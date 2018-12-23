#ifndef AMULTIPLETREEINPUTWIDGET
#define AMULTIPLETREEINPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QListBox;
class QListBoxItem;
class QComboBox;
class QCheckBox;
class QLabel;

class QAMultipleTreeInputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *minimum;
 public:
#if QT_VERSION < 0x040000
  QAMultipleTreeInputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAMultipleTreeInputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAMultipleTreeInputWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnMinimumClicked();
};

#endif
