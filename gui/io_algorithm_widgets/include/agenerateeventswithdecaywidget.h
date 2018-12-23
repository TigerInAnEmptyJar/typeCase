#ifndef QAGENERATEEVENTSWITHDECAYWIDGET
#define QAGENERATEEVENTSWITHDECAYWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QCheckBox;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
class QListView;
class QListViewItem;
#else
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QLabel;
class QLineEdit;
class QAGenerateEventsWithDecayWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *useFunction;
#if QT_VERSION < 0x040000
  QListView *partIDs;
  QListBox *params;
  QListBox *accAnd;
  QListBox* accOr;
#else
  QTreeWidget *partIDs;
  QListWidget *params;
  QListWidget *accAnd;
  QListWidget* accOr;
#endif
  QCheckBox *acceptance;
 public:
#if QT_VERSION < 0x040000
  QAGenerateEventsWithDecayWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAGenerateEventsWithDecayWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAGenerateEventsWithDecayWidget();
  void getParticlesFromView();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnUseFunctionClick();
 virtual void OnAcceptance();
#if QT_VERSION < 0x040000
 virtual void OnPartIDsClick(int button, QListViewItem *it, const QPoint &p, int column);
 virtual void OnParamsClick(int button, QListBoxItem *it);
 virtual void OnAccClick(int button,QListBoxItem* it);
#else
 virtual void OnPartIDsClick(QTreeWidgetItem *it, int column);
 virtual void OnParamsClick(QListWidgetItem *it);
 virtual void OnAccClick(QListWidgetItem* it);
 virtual void OnPartIDsClick(const QPoint &p);
 virtual void OnParamsClick(const QPoint &p);
 virtual void OnAccClick(const QPoint &p);
#endif
};
#endif
