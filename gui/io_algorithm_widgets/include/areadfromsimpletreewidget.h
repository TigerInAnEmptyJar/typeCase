#ifndef AREADFROMSIMPLETREEWIDGET
#define AREADFROMSIMPLETREEWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QListView;
class QListViewItem;
class QComboBox;
class QCheckBox;
class QLineEdit;
class QLabel;
class QSpinBox;

class QAReadFromSimpleTreeWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QComboBox *runBox;
  //  QListView *fileBox;
  QLineEdit *trackFile;
  QLineEdit *clusterFile;
  QLineEdit *pixelFile;
  QLineEdit *hitFile;
  QCheckBox *doTrack;
  QCheckBox *doCluster;
  QCheckBox *doPixel;
  QCheckBox *doHits;
  QCheckBox *doShift;
  QCheckBox *resetUn;
  QCheckBox *UseLocal;
  QLineEdit *localDir;
  QSpinBox *shiftNumber;
  QLabel *label1;
  QLineEdit *Treename;
  vector<vector<string> > fileNames;
  vector<vector<int> >fileTypes;
  vector<int>acceptedFileTypes;
  vector<run_parameter*>*selectedRun;
 public:
#if QT_VERSION < 0x040000
  QAReadFromSimpleTreeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAReadFromSimpleTreeWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAReadFromSimpleTreeWidget();
 public slots:
   virtual void resize(int w, int h); virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void setRuns(vector<run_parameter*> *selectedRuns);
 virtual void OnRunSelect(int num);
/*  virtual void OnFileSelect(QListViewItem *it); */
 virtual void OnDoHitsClick();
 virtual void OnDoPixelClick();
 virtual void OnDoClusterClick();
 virtual void OnDoTrackClick();
 virtual void OnDoShiftClick();
 virtual void OnShiftValueChanged(int num);
 virtual void OnResetClick();
 virtual void OnHitFileChange();
 virtual void OnPixelFileChange();
 virtual void OnTrackFileChange();
 virtual void OnClusterFileChange();
 virtual void OnTreeNameChange();
 virtual void OnLocalDirectoryChange();
 virtual void OnUseLocalClick();
};

#endif
