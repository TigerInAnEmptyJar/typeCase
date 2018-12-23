/****************************************************************************
** Form interface generated from reading ui file 'typecasemw.ui'
**
** Created: Wed Jun 11 15:56:57 2008
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TYPECASEMW_H
#define TYPECASEMW_H

#include <qvariant.h>
#include <qmainwindow.h>
#include "databasiswidget.h"
#include "setupwidget.h"
#include "analysewidget.h"
#include "algorithmwidget.h"
#include "helpwidget.h"
//#include "viewwidget.h"
#include "detectorparameter.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
#if QT_VERSION < 0x040000
class QPopupMenu;
#else
class QMenu;
#endif
class QPushButton;
class QLabel;
class run_parameter;
class shape_parameter;
class algorithm_parameter;
class beamTime_parameter;
class material_parameter;
class paint_parameter;

using namespace std;

class typeCaseMW : public QMainWindow
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    typeCaseMW( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
#else
    typeCaseMW( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~typeCaseMW();

    QPushButton* pushButton5;
    QLabel* textLabel1;
    QPushButton* pushButton2;
    QPushButton* pushButton3;
    QPushButton* pushButton4;
    QPushButton* pushButton6;
    QPushButton* pushButton1;
    QMenuBar *MenuBarEditor;
#if QT_VERSION < 0x040000
    QPopupMenu *File;
    QPopupMenu *popupMenu;
    QPopupMenu *popupMenu_7;
    QPopupMenu *popupMenu_13;
    QPopupMenu *popupMenu_19;
#else
    QMenu *File;
    QMenu *popupMenu;
    QMenu *popupMenu_7;
    QMenu *popupMenu_13;
    QMenu *popupMenu_19;
#endif
    QAction* fileLoadAction;
    QAction* fileLoadDataAction;
    QAction* fileLoadGeometryAction;
    QAction* fileLoadAlgorithmsAction;
    QAction* fileLoadAnalysis_patternAction;
    QAction* fileSaveAction;
    QAction* fileSaveDataAction;
    QAction* fileSaveGeometryAction;
    QAction* fileSaveAlgorithmsAction;
    QAction* fileSaveAnalysis_patternAction;
    QAction* fileSave_asAction;
    QAction* fileSave_asDataAction;
    QAction* fileSave_asGeometryAction;
    QAction* fileSave_asAlgorithmsAction;
    QAction* fileSave_asAnalysis_patternAction;
    QAction* fileLoadShapesAction;
    QAction* fileSaveShapesAction;
    QAction* fileSave_asShapesAction;
    QAction* filenew_itemAction;
    QAction* filenew_itemCalibrationAction;
    QAction* filenew_itemData_defaultAction;
    QAction* fileCopy_to_Data_directoryAction;

public slots:
    virtual void closeEvent( QCloseEvent * e );
    virtual void prepareFirstRun( QString typecaseHome );
    virtual void OnDataBasisShow();
    virtual void OnSetupShow();
    virtual void OnAnalysisShow();
    virtual void OnAlgorithmsShow();
    virtual void OnViewShow();
    virtual void OnHelpShow();
    virtual void OnLoadData();
    virtual void OnLoadGeometry();
    virtual void OnLoadShapes();
    virtual void OnLoadAlgorithms();
    virtual void OnLoadAnalysis();
    virtual void OnSaveData();
    virtual void OnSaveGeometry();
    virtual void OnSaveShapes();
    virtual void OnSaveAlgorithms();
    virtual void OnSaveAnalysis();
    virtual void OnSaveAsData();
    virtual void OnSaveAsGeometry();
    virtual void OnSaveAsShapes();
    virtual void OnSaveAsAlgorithms();
    virtual void OnSaveAsAnalysis();
    virtual void writeInitFile();
    virtual void OnChangeCalibrationDirectory();
    virtual void OnChangeDataDirectory();
    virtual void OnCopyToDataDirectory();
    virtual void OnDetectorAdd();
    virtual void OnShapeNew();
    virtual void OnShapeAdd();
    virtual void OnShapeInstallLog();
    virtual void OnShapeInstall();
    virtual void OnBTAdd();
    virtual void OnRunAdd();
    virtual void OnAlgorithmNew();
    virtual void readInstallLog();
    virtual void OnAlgorithmInstall();
    virtual void OnAddAlgorithm();
    virtual void onAnalysisParameterWritten(const string & fn);
    virtual void setRunTypes(const string &filename);
/*     virtual void (); */
/*     virtual void (); */
protected:
    DataBasisWidget* Dataw;
    SetupWidget* Setup;
    AnalysisWidget* Analysis;
    AlgorithmWidget* Algorithm;
    HelpWidget* Help;
    //    ViewWidget *View;
    vector<beamTime_parameter> beamtimes;
    vector<run_parameter> runs;
    vector<detector_parameter> detectors;
    vector<material_parameter> materials;
    reaction_parameter experiment;
    vector<shape_parameter> default_shapes;
    vector<algorithm_parameter> algorithms;
    vector<string> readRunTypes(const string &filename);

protected slots:
    virtual void languageChange();
 signals:
 void selectAlgorithm(algorithm_parameter &, bool);
private:
    vector<string> filenames;
    vector<QAction*> menuactions;
    void init();
    void destroy();

};

#endif // TYPECASEMW_H
