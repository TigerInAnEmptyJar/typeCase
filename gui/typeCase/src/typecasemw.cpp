/****************************************************************************
** Form implementation generated from reading ui file 'typecasemw.ui'
**
** Created: Tue Feb 6 10:31:42 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "typecasemw.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <iostream>
#include <fstream>
#include "directories.h"
#include "utilities.h"
#include "typecasemw.ui.h"
/*
 *  Constructs a typeCaseMW as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
#if QT_VERSION < 0x040000
typeCaseMW::typeCaseMW( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "typeCaseMW" );
#else
typeCaseMW::typeCaseMW( QWidget* parent, Qt::WindowFlags f )
    : QMainWindow( parent, f )
{
    (void)statusBar();
    setObjectName( "typeCaseMW" );
#endif
    setCentralWidget( new QWidget( this ) );

    pushButton5 = new QPushButton( centralWidget());//, "pushButton5" );
    pushButton5->setGeometry( QRect( 300, 239, 161, 91 ) );
    pushButton5->setEnabled(false);

    textLabel1 = new QLabel( centralWidget());//,, "textLabel1" );
    textLabel1->setGeometry( QRect( 230, 140, 300, 60 ) );

    pushButton2 = new QPushButton( centralWidget());//,, "pushButton2" );
    pushButton2->setGeometry( QRect( 20, 20, 161, 90 ) );

    pushButton3 = new QPushButton( centralWidget());//,, "pushButton3" );
    pushButton3->setGeometry( QRect( 20, 240, 161, 91 ) );

    pushButton4 = new QPushButton( centralWidget());//,, "pushButton4" );
    pushButton4->setGeometry( QRect( 300, 20, 160, 90 ) );

    pushButton6 = new QPushButton( centralWidget());//,, "pushButton6" );
    pushButton6->setGeometry( QRect( 550, 10, 31, 100 ) );

    pushButton1 = new QPushButton( centralWidget());//,, "pushButton1" );
    pushButton1->setGeometry( QRect( 20, 130, 161, 91 ) );

    // actions
    // toolbars
    // menubar
#if QT_VERSION < 0x040000
    MenuBarEditor = new QMenuBar( this);
    File = new QPopupMenu( this );
    popupMenu = new QPopupMenu( this );
    popupMenu_7 = new QPopupMenu( this );
    popupMenu_13 = new QPopupMenu( this );
    popupMenu_19 = new QPopupMenu( this );
    QPopupMenu *dataMenu=new QPopupMenu(this);
    QPopupMenu *shaMenu=new QPopupMenu(this);
    QPopupMenu *geoMenu=new QPopupMenu(this);
    QPopupMenu *algoMenu=new QPopupMenu(this);
    for(int i=0;i<43;i++)
      menuactions.push_back(new QAction(this));
    connect( menuactions[4], SIGNAL( activated() ), this, SLOT( OnLoadAlgorithms() ) );
    connect( menuactions[5], SIGNAL( activated() ), this, SLOT( OnLoadAnalysis() ) );
    connect( menuactions[1], SIGNAL( activated() ), this, SLOT( OnLoadData() ) );
    connect( menuactions[3], SIGNAL( activated() ), this, SLOT( OnLoadGeometry() ) );
    connect( menuactions[2], SIGNAL( activated() ), this, SLOT( OnLoadShapes() ) );
    connect( menuactions[10], SIGNAL( activated() ), this, SLOT( OnSaveAlgorithms() ) );
    connect( menuactions[11], SIGNAL( activated() ), this, SLOT( OnSaveAnalysis() ) );
    connect( menuactions[7], SIGNAL( activated() ), this, SLOT( OnSaveData() ) );
    connect( menuactions[9], SIGNAL( activated() ), this, SLOT( OnSaveGeometry() ) );
    connect( menuactions[8], SIGNAL( activated() ), this, SLOT( OnSaveShapes() ) );
    connect( menuactions[16], SIGNAL( activated() ), this, SLOT( OnSaveAsAlgorithms() ) );
    connect( menuactions[17], SIGNAL( activated() ), this, SLOT( OnSaveAsAnalysis() ) );
    connect( menuactions[13], SIGNAL( activated() ), this, SLOT( OnSaveAsData() ) );
    connect( menuactions[15], SIGNAL( activated() ), this, SLOT( OnSaveAsGeometry() ) );
    connect( menuactions[14], SIGNAL( activated() ), this, SLOT( OnSaveAsShapes() ) );
    connect( menuactions[19], SIGNAL( activated() ), this, SLOT( OnChangeCalibrationDirectory() ) );
    connect( menuactions[20], SIGNAL( activated() ), this, SLOT( OnChangeDataDirectory() ) );
    connect( menuactions[21], SIGNAL( activated() ), this, SLOT( OnCopyToDataDirectory() ) );
    connect( menuactions[22], SIGNAL( activated() ),this,SLOT(OnLoadAlgorithms()));
    connect( menuactions[23], SIGNAL( activated() ),this,SLOT(OnSaveAlgorithms()));
    connect( menuactions[24], SIGNAL( activated() ),this,SLOT(OnSaveAsAlgorithms()));
    connect( menuactions[25], SIGNAL( activated() ),this,SLOT(OnAddAlgorithm()));
    connect( menuactions[26], SIGNAL( activated() ),this,SLOT(OnAlgorithmInstall()));
    connect( menuactions[27], SIGNAL( activated() ),this,SLOT(readInstallLog()));
    connect( menuactions[28], SIGNAL( activated() ),this,SLOT(OnLoadData()));
    connect( menuactions[29], SIGNAL( activated() ),this,SLOT(OnSaveData()));
    connect( menuactions[30], SIGNAL( activated() ),this,SLOT(OnSaveAsData()));
    connect( menuactions[31], SIGNAL( activated() ),this,SLOT(OnBTAdd()));
    connect( menuactions[32], SIGNAL( activated() ),this,SLOT(OnRunAdd()));
    connect( menuactions[33], SIGNAL( activated() ),this,SLOT(OnLoadGeometry()));
    connect( menuactions[34], SIGNAL( activated() ),this,SLOT(OnSaveGeometry()));
    connect( menuactions[35], SIGNAL( activated() ),this,SLOT(OnSaveAsGeometry()));
    connect( menuactions[36], SIGNAL( activated() ),this,SLOT(OnDetectorAdd()));
    connect( menuactions[37], SIGNAL( activated() ),this,SLOT(OnLoadShapes()));
    connect( menuactions[38], SIGNAL( activated() ),this,SLOT(OnSaveShapes()));
    connect( menuactions[39], SIGNAL( activated() ),this,SLOT(OnSaveAsShapes()));
    connect( menuactions[40], SIGNAL( activated() ),this,SLOT(OnShapeAdd()));
    connect( menuactions[41], SIGNAL( activated() ),this,SLOT(OnShapeInstall()));
    connect( menuactions[42], SIGNAL( activated() ),this,SLOT(OnShapeInstallLog()));
    for(unsigned int i=1;i<6;i++)
      menuactions[i]->addTo(popupMenu);
    for(unsigned int i=7;i<12;i++)
      menuactions[i]->addTo(popupMenu_7);
    for(unsigned int i=14;i<18;i++)
      menuactions[i]->addTo(popupMenu_13);
    for(unsigned int i=19;i<22;i++)
      menuactions[i]->addTo(popupMenu_19);
    for(unsigned int i=22;i<28;i++)
      menuactions[i]->addTo(algoMenu);
    for(unsigned int i=28;i<33;i++)
      menuactions[i]->addTo(dataMenu);
    for(unsigned int i=33;i<37;i++)
      menuactions[i]->addTo(geoMenu);
    for(unsigned int i=37;i<43;i++)
      menuactions[i]->addTo(shaMenu);

    File->insertItem( /* fileLoadAction->iconSet(), */ tr( "Load" ), popupMenu );
    File->insertItem( /*fileSaveAction->iconSet(),*/ tr( "Save" ), popupMenu_7 );
    File->insertItem( /*fileSave_asAction->iconSet(),*/ tr( "Save as" ), popupMenu_13 );
    File->insertItem( /*filenew_itemAction->iconSet(),*/ tr( "Change directories" ), popupMenu_19 );
    MenuBarEditor->insertItem( QString("File"), File, 1 );
    MenuBarEditor->insertItem( QString("&algorithm"), algoMenu, 2 );
    MenuBarEditor->insertItem( QString("&data-basis"), dataMenu, 3 );
    MenuBarEditor->insertItem( QString("&geometry"), geoMenu, 4 );
    MenuBarEditor->insertItem( QString("&shape"), shaMenu, 5 );
#else
    File = menuBar()->addMenu(tr("&File"));
    popupMenu = File->addMenu(tr("&Load"));
    popupMenu_7 = File->addMenu(tr("&Save"));
    popupMenu_13 = File->addMenu(tr("Save &as"));
    popupMenu_19 = File->addMenu(tr("&Change directories"));
    menuactions.push_back(NULL);//popupMenu->addAction("Load");
    menuactions.push_back(popupMenu->addAction("Data-Basis", this, SLOT(OnLoadData())));
    menuactions.push_back(popupMenu->addAction("Shapes", this, SLOT(OnLoadShapes())));
    menuactions.push_back(popupMenu->addAction("Geometry", this, SLOT(OnLoadGeometry())));
    menuactions.push_back(popupMenu->addAction("Algorithms", this, SLOT(OnLoadAlgorithms())));
    menuactions.push_back(popupMenu->addAction("Analysis pattern", this, SLOT(OnLoadAnalysis())));
    menuactions.push_back(NULL);//popupMenu_7->addAction("Save");
    menuactions.push_back(popupMenu_7->addAction("Data", this, SLOT(OnSaveData())));
    menuactions.push_back(popupMenu_7->addAction("Shapes", this, SLOT(OnSaveShapes())));
    menuactions.push_back(popupMenu_7->addAction("Geometry", this, SLOT(OnSaveGeometry())));
    menuactions.push_back(popupMenu_7->addAction("Algorithms", this, SLOT(OnSaveAlgorithms())));
    menuactions.push_back(popupMenu_7->addAction("Analysis pattern", this, SLOT(OnSaveAnalysis())));
    menuactions.push_back(NULL);// popupMenu_13->addAction("Save as");
    menuactions.push_back(popupMenu_13->addAction("Data", this, SLOT(OnSaveAsData())));
    menuactions.push_back(popupMenu_13->addAction("Shapes", this, SLOT(OnSaveAsShapes())));
    menuactions.push_back(popupMenu_13->addAction("Geometry", this, SLOT(OnSaveAsGeometry())));
    menuactions.push_back(popupMenu_13->addAction("Algorithms", this, SLOT(OnSaveAsAlgorithms())));
    menuactions.push_back(popupMenu_13->addAction("Analysis pattern", this, SLOT(OnSaveAsAnalysis())));
    menuactions.push_back(NULL);//popupMenu_19->addAction("");
    menuactions.push_back(popupMenu_19->addAction("Calibration", this, SLOT(OnChangeCalibrationDirectory())));
    menuactions.push_back(popupMenu_19->addAction("Data default", this, SLOT(OnChangeDataDirectory())));
    menuactions.push_back(popupMenu_19->addAction("Copy to Data directory", this, SLOT(OnCopyToDataDirectory())));
    QMenu *algoMenu=menuBar()->addMenu(tr("&algorithm"));
    menuactions.push_back(algoMenu->addAction(tr("&load"),this,SLOT(OnLoadAlgorithms())));
    menuactions.push_back(algoMenu->addAction(tr("&save"),this,SLOT(OnSaveAlgorithms())));
    menuactions.push_back(algoMenu->addAction(tr("save &as"),this,SLOT(OnSaveAsAlgorithms())));
    algoMenu->addSeparator();
    menuactions.push_back(algoMenu->addAction(tr("a&dd single algorithm"),this,SLOT(OnAddAlgorithm())));
    algoMenu->addSeparator();
    menuactions.push_back(algoMenu->addAction(tr("&install"),this,SLOT(OnAlgorithmInstall())));
    menuactions.push_back(algoMenu->addAction(tr("l&oad install-log"),this,SLOT(readInstallLog())));
    QMenu *dataMenu=menuBar()->addMenu(tr("&data-basis"));
    menuactions.push_back(dataMenu->addAction(tr("&load"),this,SLOT(OnLoadData())));
    menuactions.push_back(dataMenu->addAction(tr("&save"),this,SLOT(OnSaveData())));
    menuactions.push_back(dataMenu->addAction(tr("save &as"),this,SLOT(OnSaveAsData())));
    dataMenu->addSeparator();
    menuactions.push_back(dataMenu->addAction(tr("add &beam-time"),this,SLOT(OnBTAdd())));
    menuactions.push_back(dataMenu->addAction(tr("add &run"),this,SLOT(OnRunAdd())));
    QMenu *geoMenu=menuBar()->addMenu(tr("&geometry"));
    menuactions.push_back(geoMenu->addAction(tr("&load"),this,SLOT(OnLoadGeometry())));
    menuactions.push_back(geoMenu->addAction(tr("&save"),this,SLOT(OnSaveGeometry())));
    menuactions.push_back(geoMenu->addAction(tr("save &as"),this,SLOT(OnSaveAsGeometry())));
    geoMenu->addSeparator();
    menuactions.push_back(geoMenu->addAction(tr("a&dd detector"),this,SLOT(OnDetectorAdd())));
    QMenu *shaMenu=menuBar()->addMenu(tr("&shapes"));
    menuactions.push_back(shaMenu->addAction(tr("&load"),this,SLOT(OnLoadShapes())));
    menuactions.push_back(shaMenu->addAction(tr("&save"),this,SLOT(OnSaveShapes())));
    menuactions.push_back(shaMenu->addAction(tr("save &as"),this,SLOT(OnSaveAsShapes())));
    shaMenu->addSeparator();
    menuactions.push_back(shaMenu->addAction(tr("a&dd single shape"),this,SLOT(OnShapeAdd())));
    shaMenu->addSeparator();
    menuactions.push_back(shaMenu->addAction(tr("&install"),this,SLOT(OnShapeInstall())));
    menuactions.push_back(shaMenu->addAction(tr("l&oad install-log"),this,SLOT(OnShapeInstallLog())));
//     QMenu *hlpMenu=menuBar()->addMenu(tr("&help"));
//     menuactions.push_back(hlpMenu->addAction(tr("&help"),this,SLOT(OnHelpShow())));
#endif

#if QT_VERSION < 0x040000
#endif

    init();
    languageChange();
    resize( QSize(584, 381).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( OnSetupShow() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( OnDataBasisShow() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( OnAnalysisShow() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( OnAlgorithmsShow() ) );
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( OnViewShow() ) );
    connect( pushButton6, SIGNAL( clicked() ), this, SLOT( OnHelpShow() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
typeCaseMW::~typeCaseMW()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void typeCaseMW::languageChange()
{
#if QT_VERSION < 0x040000
    setCaption( tr( "Type Case" ) );
#else
    setWindowTitle( tr( "Type Case" ) );
#endif
    pushButton5->setText( tr( "View" ) );
    textLabel1->setText( tr( "Analysis program, formerly known as Tof2004\n"
"	programmed by K. Ehrhardt" ) );
    pushButton2->setText( tr( "Databasis \n"
"and \n"
"Calibration" ) );
    pushButton3->setText( tr( "Algorithms" ) );
    pushButton4->setText( tr( "Analysis" ) );
    pushButton6->setText( tr( "H\n"
"e\n"
"l\n"
"p" ) );
    pushButton1->setText( tr( "Geometry" ) );
    vector<string> names;
    names.push_back("&Load");
    names.push_back("&Data");
    names.push_back("&Shapes");
    names.push_back("&Geometry");
    names.push_back("&Algorithms");
    names.push_back("Analysis &pattern");
    names.push_back("&Save");
    names.push_back("&Data");
    names.push_back("&Shapes");
    names.push_back("&Geometry");
    names.push_back("&Algorithms");
    names.push_back("Analysis &pattern");
    names.push_back("Save &as");
    names.push_back("&Data");
    names.push_back("&Shapes");
    names.push_back("&Geometry");
    names.push_back("&Algorithms");
    names.push_back("Analysis &pattern");
    names.push_back("Change directories");
    names.push_back("Calibration");
    names.push_back("Data");
    names.push_back("Copy to Data-directory");
    names.push_back("&load");
    names.push_back("&save");
    names.push_back("save &as");
    names.push_back("a&dd algorithm");
    names.push_back("&install algorithm");
    names.push_back("l&oad install-log");
    names.push_back("&load");
    names.push_back("&save");
    names.push_back("save &as");
    names.push_back("add &beam-time");
    names.push_back("add &run");
    names.push_back("&load");
    names.push_back("&save");
    names.push_back("save &as");
    names.push_back("a&dd detector");
    names.push_back("&load");
    names.push_back("&save");
    names.push_back("save &as");
    names.push_back("a&dd shape");
    names.push_back("&install shape");
    names.push_back("l&oad install-log");
    for(unsigned int i=0;i<menuactions.size();i++)
      {
#if QT_VERSION < 0x040000
	menuactions[i]->setText(tr(names[i].data()));
	menuactions[i]->setMenuText(tr(names[i].data()));
#else
	if(menuactions[i]!=NULL)
	  {
	    menuactions[i]->setText(tr(names[i].data()));
	    menuactions[i]->setStatusTip(tr(names[i].data()));
	  }
#endif
      }

#if QT_VERSION < 0x040000
    File->changeItem( File->idAt( 0 ), tr( "&load" ) );
    File->changeItem( File->idAt( 1 ), tr( "&save" ) );
    File->changeItem( File->idAt( 2 ), tr( "save &as" ) );
    File->changeItem( File->idAt( 3 ), tr( "change &directories" ) );
    if (MenuBarEditor->findItem(1))
        MenuBarEditor->findItem(1)->setText( tr( "file" ) );
    if (MenuBarEditor->findItem(2))
        MenuBarEditor->findItem(2)->setText( tr( "&algorithm" ) );
    if (MenuBarEditor->findItem(3))
        MenuBarEditor->findItem(3)->setText( tr( "&data-basis" ) );
    if (MenuBarEditor->findItem(4))
        MenuBarEditor->findItem(4)->setText( tr( "&geometry" ) );
    if (MenuBarEditor->findItem(5))
        MenuBarEditor->findItem(5)->setText( tr( "&shape" ) );

#else
//     File->changeItem( File->idAt( 0 ), tr( "Load" ) );
//     File->changeItem( File->idAt( 1 ), tr( "Save" ) );
//     File->changeItem( File->idAt( 2 ), tr( "Save as" ) );
//     File->changeItem( File->idAt( 3 ), tr( "Change directories" ) );
#endif
}

