#ifndef QAREADFROMTADEWIDGET
#define QAREADFROMTADEWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QComboBox;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif

class QAReadFromTadeWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QComboBox *runBox;
#if QT_VERSION < 0x040000
  QListView *fileBox;
#else
  QTreeWidget *fileBox;
#endif
  vector<vector<string> > fileNames;
  vector<vector<int> >fileTypes;
  vector<run_parameter*>*selectedRun;
 public:
#if QT_VERSION < 0x040000
  QAReadFromTadeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAReadFromTadeWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAReadFromTadeWidget();
 public slots:
   virtual void resize(int w, int h); virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void setRuns(vector<run_parameter*> *selectedRuns);
 virtual void OnRunSelect(int num);
#if QT_VERSION < 0x040000
 virtual void OnFileSelect(QListViewItem *it);
#else
 virtual void OnFileSelect();
#endif
};
#endif
