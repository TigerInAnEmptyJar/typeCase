#ifndef QALGORITHMDEFINEWIDGET
#define QALGORITHMDEFINEWIDGET
#include <qwidget.h>
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
using namespace std;

class QPushButton;

class QAlgorithmDefineWidget: public QWidget
{
Q_OBJECT
 protected:
   QPushButton *insertButton;
   algorithm_parameter ap;
   int ID;
   bool isInput;
 public:
#if QT_VERSION < 0x040000
   QAlgorithmDefineWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
   QAlgorithmDefineWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
   ~QAlgorithmDefineWidget();
   int getID()const;
   void setID(int v);
 signals:
   void insertClick(algorithm_parameter *a, bool inputAlgorithm);
 public slots:
   virtual void resize(int w, int h);
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void setRuns(vector<run_parameter*> *selectedRuns);
 protected slots:
   virtual void OnInsertButtonClick();
};

#endif
