#ifndef PAINTDISPLAY_H
#define PAINTDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "paintparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QCheckBox;
class QButton;
class QLineEdit;
class QComboBox;
class QFrame;
class ColorButton;
using namespace std;

class PaintDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    PaintDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    PaintDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~PaintDisplay();

    QLabel* nameLabel;
    QLineEdit* nameInput;
    QLabel* idLabel;
    QLineEdit* idInput;
    QLabel* markerLabel;
    QComboBox* markerInput;
    ColorButton* colorInput;
    QCheckBox* doPaint;
    virtual bool readOnly();

public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setPaint( paint_parameter & paintIn );
    virtual void setReadOnly( bool value );
    virtual void setMarker(vector<string> namesIn);
    virtual void setMarker(vector<QPixmap*> iconsIn);
    virtual void onPropertyChange();
    virtual void onPaintClick();
    virtual void onMarkerClick();
    virtual void onColorSelected(int color);
signals:
    void changed();

protected:
    paint_parameter *sp;
    bool ReadOnly;
    paint_parameter copy;
    vector<string> markerNames;
    vector<QPixmap*> markerIcons;
    QGridLayout *layout;
protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // PAINTDISPLAY_H
