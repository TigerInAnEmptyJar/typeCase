#ifndef CHECKWIZARD_H
#define CHECKWIZARD_H

#include <qvariant.h>
#include <qwizard.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QWidget;
class QPushButton;
class QLabel;
class QCheckBox;
class QLineEdit;
class QListView;
class QListViewItem;
class QListBox;
class QListBoxItem;
class QComboBox;
class QTextEdit;
class QButtonGroup;
class QRadioButton;
class QFrame;
class QSpinBox;
class QCanvasWidget;
class TH1F;
class CommonCalibrationParser;
using namespace std;

class checkWizard : public QWizard
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    checkWizard( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    checkWizard( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~checkWizard();
    QCanvasWidget **canvases;
    QLabel **canvasLabels;
public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setHistograms(TH1F **histoT, TH1F **histoQ, CommonCalibrationParser *parameter);
    virtual void accept();
    virtual void setParameter(int el, bool tdc, bool lower, float value);
signals:
    void helpAsked(QString);

protected:
    TH1F **thistograms;
    TH1F **qhistograms;
    CommonCalibrationParser *parser;
    float **parameter;
    int getCurrentCanvas() ;
protected slots:
    virtual void languageChange();
 virtual void canvasDoubleClick(const QPoint& p); 
 virtual void canvasClick(const QPoint &p); 
 virtual void canvasRightClick(const QPoint &p); 

private:

};

#endif // CHECKWIZARD_H
