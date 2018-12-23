#ifndef QAWRITEHISTOSTOROOTFILEWIDGET
#define QAWRITEHISTOSTOROOTFILEWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QLabel;
class QLineEdit;
class QAWriteHistosToRootFileWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QLineEdit *filename;
  QLabel *label1;
  QLineEdit *directory;
  QLabel *label2;
  QLineEdit *pattern;
  QLabel *label3;
 public:
#if QT_VERSION < 0x040000
  QAWriteHistosToRootFileWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAWriteHistosToRootFileWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAWriteHistosToRootFileWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnFileNameChange();
 virtual void OnDirectoryChange();
 virtual void OnPatternChange();
};
#endif
