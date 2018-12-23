#ifndef BEAMTIMEDISPLAY_H
#define BEAMTIMEDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "beamtimeparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QCheckBox;
class QLineEdit;
class QTextEdit;
class QMenu;
#if QT_VERSION < 0x040000
class QListBox;
class QListBoxItem;
#else
class QListWidget;
class QListWidgetItem;
#endif
class QGridLayout;
class QPushButton;
using namespace std;

class BeamTimeDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    BeamTimeDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    BeamTimeDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~BeamTimeDisplay();

    QLabel* nameLabel;
    QLabel* monthLabel;
    QLabel* yearLabel;
    QLabel* setupLabel;
    QLabel *calibrationLabel;
    QLabel* descrLabel;
    QLineEdit* nameInput;
    QWidget* monthInput;
    QWidget* yearInput;
    QLineEdit* setupInput;
#if QT_VERSION < 0x040000
    QListBox* calInput;
#else
    QListWidget* calInput;
#endif
    QTextEdit* descrInput;
    QPushButton* browseButton;
    virtual bool readOnly() const;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setBeamTime(beamTime_parameter &beamTime);
    virtual void onBrowseClick();
    virtual void onCalibrationRemove(int id);
    virtual void onCalibrationChange(int id);
#if QT_VERSION < 0x040000
    virtual void onCalibrationRightClick(QListBoxItem *it, const QPoint&p);
#else
    virtual void onCalibrationRightClick(const QPoint&p);
    virtual void onCalibrationDo(QAction* ac);
#endif
    virtual void onDescriptionChange();
    virtual void onCalibrationAdd();
    virtual void setReadOnly(bool value);
    virtual void onPropertyChange();
protected:
    beamTime_parameter *sp;
    beamTime_parameter copy;
    bool ReadOnly;
    QGridLayout *layout;
    QMenu* tmpMenu;
protected slots:
    virtual void languageChange();
 
 signals:
 void changed();
private:
    void init();
    void destroy();

};

#endif // BEAMTIMEDISPLAY_H
