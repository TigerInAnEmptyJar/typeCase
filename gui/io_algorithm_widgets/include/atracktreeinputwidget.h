#ifndef ATRACKTREEINPUTWIDGET
#define ATRACKTREEINPUTWIDGET
#include "qalgorithmdefinewidget.h"
#include <vector>
using namespace std;
class QListBox;
class QListBoxItem;
class QComboBox;
class QCheckBox;
class QLabel;
class QLineEdit;
class QSpinBox;


class QATrackTreeInputWidget: public QAlgorithmDefineWidget
{
  Q_OBJECT
    protected:
  QCheckBox *searchForEvent;
  QCheckBox *useAsEventList;
  QCheckBox *useLocalDir;
  QLineEdit *localDir;
  QCheckBox *useEventPattern;
  QSpinBox *prptt;
  QSpinBox *kiptt;
  QSpinBox *veptt;
  QLabel* label1;
 public:
#if QT_VERSION < 0x040000
  QATrackTreeInputWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
  QATrackTreeInputWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
  ~QATrackTreeInputWidget();
 public slots:
   virtual void onUseLocalChange();
   virtual void onLocalChange();
   virtual void onUseEventListChange();
   virtual void onSearchChange();
   virtual void onUsePatternChange();
   virtual void onPatternPChange();
   virtual void onPatternKChange();
   virtual void onPatternVChange();
 protected:
 void languageChange();
};

#endif
