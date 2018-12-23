#ifndef CONTROLWIDGETMW_H
#define CONTROLWIDGETMW_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QLineEdit;
class QFrame;
#if QT_VERSION < 0x040000
class QPopupMenu;
#else
class QMenu;
#endif
class QPushButton;
class QLabel;
class TFile;
class TTree;
class TH1F;
class CommonCalibrationParser;
#include "detectorparameter.h"

using namespace std;

class controlWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    controlWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    controlWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~controlWidget();

    QPushButton *browseButton;
    QLabel *runLabel;
    QLineEdit *runInput;
    QLabel *beamLabel;
    QLineEdit *beamInput;
    QLabel *authorLabel;
    QLineEdit *authorInput;
    QPushButton *fillAllButton;
    QPushButton *fitAllButton;
    QPushButton *checkAllButton;
    QPushButton *writeButton;
    QPushButton *viewButton;
    QGridLayout *layout;

    QLabel *fillLabel;
    QFrame *fillBox;
    QPushButton **fillButtons;
    QGridLayout *fillLayout;

    QLabel *fitLabel;
    QFrame *fitBox;
    QPushButton **fitButtons;
    QGridLayout *fitLayout;

    QLabel *checkLabel;
    QFrame *checkBox;
    QPushButton **checkButtons;
    QGridLayout *checkLayout;

#if QT_VERSION < 0x040000
#else
#endif
public slots:
  virtual void setDetectors(vector<string> names); 
  virtual void setDetectors(vector<detector_parameter> names); 
/*     virtual void (); */
protected:

protected slots:
    virtual void languageChange();
 virtual void fillClicked();
 virtual void fitClicked();
 virtual void checkClicked();
 signals:
 void browseClicked();
 void fillClicked(int);
 void fitClicked(int);
 void checkClicked(int);
 void fillAllClicked();
 void fitAllClicked();
 void checkAllClicked();
 void write();
 void view();
private:
 int nDets;
};

#endif // CONTROLWIDGETMW_H
