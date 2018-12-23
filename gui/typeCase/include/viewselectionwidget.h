#ifndef VIEWSELECTIONWIDGET
#define VIEWSELECTIONWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "paintparameter.h"
#include <TCanvas.h>
#include <TTree.h>
#include <TH1.h>
#include "event.h"
#include "padparameter.h"
#if QT_VERSION < 0x040000
class QWidgetStack;
class QListBox;
class QListBoxItem;
class QListView;
class QListViewItem;
#else
class QStackedWidget;
class QGroupBox;
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
#endif
class tofAnalysis;
class QGridLayout;
class PaintWidget;
class QPushButton;
class QPushButton;
class QButtonGroup;
class QListBox;
class QLineEdit;
class QLabel;
using namespace std;

class ViewSelectionWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    ViewSelectionWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    QWidgetStack* widgets;
#else
    ViewSelectionWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    QStackedWidget* widgets;
#endif
    ~ViewSelectionWidget();
    QLabel *canvasLabel;
    QPushButton ***buttons;
    QPushButton* divideButton;
#if QT_VERSION > 0x040000
    QGroupBox *displayTypeBox;
    QGridLayout *displayTypeLayout;
#endif
    QGridLayout *layout;
    QGridLayout *buttonLayout;
    QButtonGroup *displayType;
    QListBox* histoBox;
    QListBox *treeBox;
    QLineEdit* treeDrawName;
    QLineEdit* treeDrawOption;
    QLineEdit* histoDrawOption;
    QLabel* treeDrawLabel;
    QLabel* histoDrawLabel;
    //in stacked widget
    PaintWidget *paints;
    //histos
    QWidget *histoWidget;
#if QT_VERSION < 0x040000
    QListBox *histoList;
#else
    QListWidget *histoList;
#endif
    QGridLayout *histoLayout;
    QLabel *histoLabel;
    QLabel *histoOptionLabel;
    QLineEdit *histoInput;
    QLineEdit *histoOptionInput;
    //trees
    QWidget *treeWidget;
#if QT_VERSION < 0x040000
    QListView *treeList;
#else
    QTreeWidget *treeList;
#endif
    QGridLayout *treeLayout;
    QLabel *treeLabel;
    QLabel *treeCutLabel;
    QLabel *treeOptionLabel;
    QLineEdit *treeInput;
    QLineEdit *treeCutInput;
    QLineEdit *treeOptionInput;

public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setPads(vector<pad_parameter> &p, int n1, int n2);
    virtual void setCanvas(TCanvas *can);
    virtual void setEvent(TEvent *e);
    virtual void setAnalysisParameter(int nE2a, int nE, QDateTime* startT);
    virtual void divideCanvas();
    virtual void analyserInitialized(tofAnalysis* pointer);
protected slots:
  virtual void onPaintChanged();
  virtual void onSelectedChanged();
  virtual void onDisplayTypeChange(int num);
  virtual void histoChanged(const QString &text);
  virtual void optionChanged(const QString &text);
  virtual void treeChanged(const QString &text);
  virtual void treeCutChanged(const QString &text);
#if QT_VERSION < 0x040000
  virtual void treeChanged(QListViewItem *it); 
  virtual void histoChanged(QListBoxItem *it); 
#else
  virtual void treeChanged(QTreeWidgetItem *it); 
  virtual void histoChanged(QListWidgetItem *it); 
#endif

/*   virtual void tabPageChanged(); */
signals:
  void helpRequested(const string &);
    void changed();
 private:
    vector<pad_parameter> *pp;
    int nx,ny;
    int selx,sely;
    int nevents;
    int nevents2A;
    tofAnalysis *analyserPointer;
    QDateTime* startTime;
    TCanvas *canvas;
    TEvent *event;
    void init();
    void destroy();
    void languageChange();
};

#endif //VIEWSELECTIONWIDGET
