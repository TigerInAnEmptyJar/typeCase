#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <qvariant.h>
#include <qwidget.h>

#if QT_VERSION < 0x040000
#include <qpopupmenu.h>
#else
#include <QMenu>
#endif
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QTextBrowser;
using namespace std;

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    HelpWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    HelpWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~HelpWidget();

    QTextBrowser* textEdit1;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setHelp(const string & filename);

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // HELPWIDGET_H
