/****************************************************************************
** Form interface generated from reading ui file 'typecasemw.ui'
**
** Created: Wed Jun 11 15:56:57 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CUTSMW_H
#define CUTSMW_H

#include <qvariant.h>
#include <qmainwindow.h>
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
class TH1;
class TH1F;
class TH2F;
class CommonCalibrationParser;
#include "detectorparameter.h"
#include "controlwidget.h"

using namespace std;

class cutsMW : public QMainWindow
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    cutsMW( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
#else
    cutsMW( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~cutsMW();
    controlWidget *control;

    QMenuBar *MenuBarEditor;
#if QT_VERSION < 0x040000
#else
#endif
public slots:
  virtual void closeEvent( QCloseEvent * e );
 virtual void setCommandLineArguments(int argc, char** argv); 
 virtual void resize(int w,int h); 
 virtual void resize(const QSize &s); 
 virtual void resizeEvent(QResizeEvent *e); 
    virtual void fillClicked(int num);
    virtual void fitClicked(int num);
    virtual void checkClicked(int num);
    virtual void BrowseClicked();
    virtual void readFile(const string& fn);
/*     virtual void (); */
protected:
    void clearObjects();
    void initObjects();
protected slots:
    virtual void languageChange();
 virtual void fill();
 virtual void fit();
 virtual void check();
 virtual void logTDC(string directory,vector<int> detectors,string outfile);
 virtual void logTDC2D(string directory,vector<int> detectors,string outfile);
 virtual void writeCalibrationDB();
 virtual void writeCalibrationDB(vector<int> dets, const string &filename);
 virtual void viewCalibrationDB();
 virtual void getParser(vector<string> filenames);
 signals:

private:
//    vector<QAction*> menuactions;
 vector<detector_parameter> setup;
 TH1F ***TDChistograms;
 TH1F ***QDChistograms;
 TFile *hitFile;
 TTree *hitTree;
 vector<CommonCalibrationParser*> cals;
 TH1 *getProfile(string functionName,int parNumber,TH2F* histo);
 vector<vector<vector<CommonCalibrationParser*> > > ncCals;
 vector<vector<int> >curr;
 bool isMC;
 float getTDC(int pos, int el, int qdc, int tdc, int evtnr);
 void getLast(vector<int> positions);
 void writeLast();
  //    void init();
  //void destroy();

};

#endif // CUTSMW_H
