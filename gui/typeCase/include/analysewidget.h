#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <qvariant.h>
#include <qwidget.h>
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
#include "tofAnalysis_t.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "orderselectionwidget.h"
#include <pthread.h>
#include <qtimer.h>
#include "padparameter.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class ColorButton;
class QFrame;
class QLabel;
class QCheckBox;
class QLineEdit;
class QCanvasWidget;
class ViewSelectionWidget;
using namespace std;

class AnalysisWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    AnalysisWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    AnalysisWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~AnalysisWidget();
    
    QPushButton *initButton;
    QPushButton *startButton;
    QPushButton *stepButton;
    QPushButton *stopButton;
    QPushButton *finalButton;
    QPushButton *helpButton;
    QPushButton *orderButton;
    QPushButton *displayButton;
    ColorButton *stateButton;
    QLineEdit *eventsInput;
    QLabel *eventsLabel;
    QCheckBox *allInput;
    QCanvasWidget *canvas;

    OrderSelectionWidget* order; 
    ViewSelectionWidget* watch; 

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void algorithmSelected(algorithm_parameter &algoIn, bool value);
    virtual void runSelected(run_parameter *run,bool value);
    virtual void algorithmsChanged();
    virtual void drawChanged();
    virtual void closeEvent( QCloseEvent * e );
    virtual void readAnalysisParameter(const string &filename);
    virtual void writeAnalysisParameter(const string &filename);
    virtual void canvasScrolled(int subPad, bool up, const QPoint &p);
    virtual void canvasDoubleClicked(int subPad, const QPoint &p);
protected:
    vector<algorithm_parameter*> selectedAlgorithms;
    vector<run_parameter*> selectedRuns;
    bool isRunning;
    bool isInitialized;
    QTimer updater;
    tofAnalysis analyser;
    vector<pad_parameter> pads;
 signals:
    void helpRequested(const string&); 
    void algorithmSelect(algorithm_parameter&, bool);
    void analysisParameterWritten(const string &fn);
    void requestRun(const string& runName,bool selected);
protected slots:
    virtual void languageChange();
 virtual void initClick();
 virtual void startClick();
 virtual void stepClick();
 virtual void stopClick();
 virtual void finalClick();
 virtual void helpClick();
 virtual void orderClick();
 virtual void displayClick();
 virtual void stopped();
 virtual void updateCanvas();
 virtual void updateEvent(TEvent *e);
 virtual void canvasMouseMove(int subPad, const QPoint &p);
private:
 pthread_t analyserThread;
 void **pointer;
    void init();
    void destroy();

};

#endif // ANALYSISWIDGET_H
