#ifndef AHITTREEINPUTWIDGET
#define AHITTREEINPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QListBox;
class QListBoxItem;
class QComboBox;
class QCheckBox;
class QLabel;
class QLineEdit;

class QAHitTreeInputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *searchForEvent;
  QCheckBox *useAsEventList;
  QCheckBox *useLocalDir;
  QLineEdit *localDir;
 public:
#if QT_VERSION < 0x040000
  QAHitTreeInputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QAHitTreeInputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QAHitTreeInputWidget();
 public slots:
   virtual void onUseLocalChange();
   virtual void onLocalChange();
   virtual void onUseEventListChange();
   virtual void onSearchChange();
 protected:
 void languageChange();
};

#endif
