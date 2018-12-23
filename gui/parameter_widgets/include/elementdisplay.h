#ifndef ELEMENTDISPLAY_H
#define ELEMENTDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "materialparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
class QLineEdit;
class QGroupBox;
class QFrame;
class QCanvasWidget;
class base_shape;
using namespace std;

class ElementDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ElementDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ElementDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~ElementDisplay();

    QLabel* nameLabel;
    QLineEdit* nameInput;
    QLabel* massLabel;
    QLineEdit* massInput;
    QLabel* chargeLabel;
    QLineEdit* chargeInput;
    QLabel* weightLabel;
    QLineEdit* weightInput;
    virtual bool readOnly();

public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setElement( element_parameter & elementIn );
    virtual void setReadOnly( bool value );
    virtual void onPropertyChange();
signals:
    void changed();

protected:
    element_parameter *sp;
    bool ReadOnly;
    element_parameter copy;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // ELEMENTDISPLAY_H
