#ifndef DATABASISWIDGET
#define DATABASISWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "beamtimeparameter.h"
class QFrame;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
class QCheckBox;
using namespace std;

class DataBasisWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    DataBasisWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    DataBasisWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~DataBasisWidget();
#if QT_VERSION < 0x040000
    QListView* listView;
#else
    QTreeWidget* listView;
#endif
    virtual bool readOnly();
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setDataBasis( vector<beamTime_parameter> &algoIn, vector<run_parameter> &runsIn);
    virtual void setDataBasis( vector<beamTime_parameter*> &algoIn, vector<run_parameter*> &runsIn );
    virtual void setDataBasis( vector<beamTime_parameter> &algoIn, vector<vector<run_parameter> > &runsIn );
    virtual void setDataBasis( vector<beamTime_parameter*> &algoIn, vector<vector<run_parameter> > &runsIn );
    virtual void setReadOnly( bool value );
    virtual void dataBasisSelect(run_parameter &algo, bool value); 
    virtual void setRunTypes(const vector<string> & nmsIn);
    virtual void runSelect(const string & runName, bool selected);
protected slots:

#if QT_VERSION < 0x040000
 virtual void dataBasisRightClicked(QListViewItem* it,const QPoint& p) ;
 virtual void dataBasisChecked(QListViewItem* it);
 virtual void dataBasisClicked(int b,QListViewItem* it,const QPoint& p, int c);
#else
 virtual void dataBasisRightClicked(const QPoint& p) ;
 virtual void dataBasisChecked(QTreeWidgetItem* it);
 virtual void dataBasisClicked(QTreeWidgetItem* it);
#endif
/*  virtual void dataBasisChanged(); */
 virtual void deleteBeamTime();
 virtual void addBeamTime();
 virtual void showBeamTime();
 virtual void deleteRun();
 virtual void addRun();
 virtual void showRun();
 virtual void changes();
signals:
    void changed();
    void runSelected(run_parameter *rp, bool value);
protected:
    bool ReadOnly;
    vector<beamTime_parameter> *bp1;
    vector<beamTime_parameter*> *bp2;
    vector<run_parameter> *rp1;
    vector<run_parameter*> *rp2;
    vector<vector<run_parameter> > *rp3;
    int mode;
    int selectedr;
    int selectedb;
    vector<string> runTypes;
#if QT_VERSION < 0x040000
    QListViewItem *currentItem;
#else
    QTreeWidgetItem *currentItem;
#endif
    bool hasChanged;
#if QT_VERSION < 0x040000
    beamTime_parameter* getParameterFromItem(QListViewItem*it, int &s);
    QListViewItem* getItemFromParameter(beamTime_parameter*al);
    QListViewItem* getItemFromParameter(run_parameter*al);
#else
    beamTime_parameter* getParameterFromItem(QTreeWidgetItem*it, int &s);
    QTreeWidgetItem* getItemFromParameter(beamTime_parameter*al);
    QTreeWidgetItem* getItemFromParameter(run_parameter*al);
#endif
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //DATABASISWIDGET
