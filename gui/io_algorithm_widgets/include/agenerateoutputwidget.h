#ifndef QAGENERATEOUTPUTWIDGET
#define QAGENERATEOUTPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QCheckBox;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
#endif
class QLabel;
class QLineEdit;
class QAGenerateOutputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *doHits;
  QCheckBox *doPixels;
  QCheckBox *doClusters;
  QCheckBox *doTracks;
  QCheckBox *doReactions;
#if QT_VERSION < 0x040000
  QListBox *reactNames;
#else
  QListWidget *reactNames;
#endif
  QLineEdit *filename;
  QLabel *label1;
  QCheckBox* UseLocal;
  QLineEdit *LocalDir;
 public:
#if QT_VERSION < 0x040000
  QAGenerateOutputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAGenerateOutputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAGenerateOutputWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnDoHitsClick();
 virtual void OnDoPixelsClick();
 virtual void OnDoClustersClick();
 virtual void OnDoTracksClick();
 virtual void OnDoReactionsClick();
#if QT_VERSION < 0x040000
 virtual void OnReactNamesClick(int button, QListBoxItem *it);
#else
 virtual void OnReactNamesClick(QListWidgetItem *it);
 virtual void OnReactNamesClick(const QPoint &p);
#endif
 virtual void OnFileNameChange();
 virtual void OnUseLocalClick();
 virtual void OnLocalDirChange();
};
#endif
