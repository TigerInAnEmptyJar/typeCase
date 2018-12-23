#ifndef QAGENERATEEVENTSWIDGET
#define QAGENERATEEVENTSWIDGET
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
class QAGenerateEventsWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *useFunction;
#if QT_VERSION < 0x040000
  QListBox *partIDs;
  QListBox *params;
#else
  QListWidget *partIDs;
  QListWidget *params;
#endif
  QLineEdit *function;
  QLabel *label1;
  QLabel *label2;
  QLabel *label3;
 public:
#if QT_VERSION < 0x040000
  QAGenerateEventsWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAGenerateEventsWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAGenerateEventsWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnUseFunctionClick();
#if QT_VERSION < 0x040000
 virtual void OnPartIDsClick(int button, QListBoxItem *it);
 virtual void OnParamsClick(int button, QListBoxItem *it);
#else
 virtual void OnPartIDsClick(QListWidgetItem *it);
 virtual void OnPartIDsClick(const QPoint& p);
 virtual void OnParamsClick(QListWidgetItem *it);
 virtual void OnParamsClick(const QPoint &p);
#endif
 virtual void OnFunctionChange();
};
#endif
