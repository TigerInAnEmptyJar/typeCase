//****************************************************************************
//** Form interface generated from reading ui file 'treewidget.ui'
//**
//** Created: Wed Jun 11 15:56:57 2008
//**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
//**
//** WARNING! All changes made in this file will be lost!
//****************************************************************************/

#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <qvariant.h>
#include <qwidget.h>
#include <TTree.h>
#include <vector>
#include <TF3.h>
#include <TLeaf.h>
#include <TCut.h>
#include <TCutG.h>
#include "geometry.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QLineEdit;
using namespace std;

class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    TreeWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~TreeWidget();

    QLabel* textLabel3;
    QLabel* textLabel7;
    QListView* listView2;
    QLabel* textLabel1_2;
    QLabel* textLabel2;
    QLabel* textLabel4;
    QLabel* textLabel1;
    QLabel* textLabel5;
    QPushButton* pushButton1;
    QLineEdit* lineEdit2;
    QLabel* textLabel6;
    QLineEdit* lineEdit1;
    QListView* listView1;

    virtual bool regularExpression( QString exp );
    virtual int getBranchType( TBranch * br );
    virtual float getParticleValue( const momentum4D & m, const QString & s );
    virtual bool cutNameExists( QString testName );
    virtual QString getNewName( QString testName );
    virtual TObject * getCut( QListViewItem * it );
    virtual TCut getApplyCut();

public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setTree( TTree * treeIn );
    virtual void OnDrawEnter();
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void complicatedDraw( QString expression );
    virtual void OnBranchDoubleClick( QListViewItem * it );
    virtual void OnContextMenuCutsRequest( QListViewItem * it, const QPoint & p );
    virtual void define1DCut();
    virtual void define2DCut();
    virtual void removeCut();
    virtual void changeCutName();
    virtual void changeCutDefinition();
    virtual void cutOR();
    virtual void cutAND();
    virtual void cutNot();
    virtual void loadCuts();
    virtual void writeCuts();
    virtual void setInitParticles();

signals:
    void treeDrawn(TH1*);
    void drawingDone();

protected:
    TTree*tree;
    vector<QString> mathexpressions;
    vector<QString> ownexpressions;
    vector<QString> particleManips;
    int binning;
    int binning2D;
    int binning3D;
    vector<TCut> cuts1D;
    vector<TCutG> cuts2D;
    momentum4D initParticle1;
    momentum4D initParticle2;
    QListViewItem* currentCut;
    vector<QString> drawHistory;
    int drawHistoryPos;
    vector<TH1*> drawnHistos;


protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // TREEWIDGET_H
