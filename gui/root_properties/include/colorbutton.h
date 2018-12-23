#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <qvariant.h>
#include <qdialog.h>
#include <qpushbutton.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class TColor;
using namespace std;

class ColorButton : public QPushButton
{
    Q_OBJECT

private:
    void init();
    void destroy();
protected:
 bool ReadOnly;
public:
#if QT_VERSION < 0x040000
    ColorButton( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ColorButton( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~ColorButton();


    virtual int getColor();
    virtual QColor getRGBColor();
    bool readOnly()const;

public slots:
  virtual void onButtonClick();
 virtual void setColor(int color);
 virtual void setColor(const QColor &c);
 virtual void setReadOnly(bool value);
protected slots:
    virtual void languageChange();
signals:
 void colorChanged(int color);
 void colorChanged(const QColor& color);

};

#endif // COLORBUTTON_H
