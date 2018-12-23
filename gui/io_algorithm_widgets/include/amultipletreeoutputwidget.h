#ifndef QAMULTIPLETREEOUTPUTWIDGET
#define QAMULTIPLETREEOUTPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QCheckBox;
class QSpinBox;
class QLabel;
class QLineEdit;
class QAMultipleTreeOutputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *beginNew;
  QLineEdit *filename;
  QLabel *label1;
  QSpinBox *nTracksPer;
  QLabel *label2;
  QCheckBox* UseLocal;
  QLineEdit *LocalDir;
 public:
#if QT_VERSION < 0x040000
  QAMultipleTreeOutputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAMultipleTreeOutputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAMultipleTreeOutputWidget();
 public slots:
   virtual void resize(int w, int h); 
 virtual void resize(const QSize &s);
 virtual void resizeEvent(QResizeEvent *e);
 virtual void OnBeginNewClick();
 virtual void OnNTracksChange();
 virtual void OnFileNameChange();
 virtual void OnUseLocalClick();
 virtual void OnLocalDirChange();
};
#endif
