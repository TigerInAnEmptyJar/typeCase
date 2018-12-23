#ifndef AREADREACTIONFROMROOTWIDGET
#define AREADREACTIONFROMROOTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
#endif
class QComboBox;
class QCheckBox;
class QLabel;

class QAReadReactionFromRootWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
#if QT_VERSION < 0x040000
  QListBox *reactionsBox;
#else
  QListWidget *reactionsBox;
#endif
  QCheckBox *doTrack;
  QCheckBox *doCluster;
  QCheckBox *doPixel;
  QCheckBox *doHits;
  QCheckBox *reconstructHits;
  QCheckBox *minimum;
  QLabel *label1;
  //  vector<run_parameter*>*selectedRun;
 public:
#if QT_VERSION < 0x040000
  QAReadReactionFromRootWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAReadReactionFromRootWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAReadReactionFromRootWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 // virtual void setRuns(vector<run_parameter*> *selectedRuns);
 // virtual void OnRunSelect(int num);
#if QT_VERSION < 0x040000
 virtual void OnReactionClick(int button, QListBoxItem *it);
#else
 virtual void OnReactionClick(QListWidgetItem *it);
 virtual void OnReactionClick(const QPoint &p);
#endif
 virtual void OnDoHitsClick();
 virtual void OnDoPixelClick();
 virtual void OnDoClusterClick();
 virtual void OnDoTrackClick();
 virtual void OnReconstructClick();
 virtual void OnMinimumClicked();
};

#endif
