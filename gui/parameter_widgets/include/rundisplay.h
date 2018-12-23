#ifndef RUNDISPLAY_H
#define RUNDISPLAY_H

#include <qwidget.h>
#include "beamtimeparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QDateTimeEdit;
class QFrame;
class QLabel;
class QCheckBox;
class QLineEdit;
class QTextEdit;
class QPushButton;
#if QT_VERSION < 0x040000
class QListBoxItem;
class QListView;
class QListViewItem;
class QListBox;
#else
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
#endif
using namespace std;

class RunDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    RunDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    RunDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~RunDisplay();

    QLabel* nameLabel;
    QLabel* beamtimeLabel;
    QLabel* typeLabel;
    QCheckBox* setupLabel;
    QCheckBox *calibrationLabel;
    QLabel* descrLabel;
    QLineEdit* nameInput;
    QWidget* beamtimeInput;
    QWidget* typeInput;
    QLineEdit* setupInput;
#if QT_VERSION < 0x040000
    QListBox* calInput;
    QListView* filesInput;
#else
    QListWidget *calInput;
    QTreeWidget* filesInput;
#endif
    QTextEdit* descrInput;
    QPushButton* browseButton;
    
    QLabel* startLabel;
    QDateTimeEdit* startInput;
    QLabel* stopLabel;
    QDateTimeEdit* stopInput;
    QLabel* runNumberLabel;
    QLineEdit* runNumberInput;

    virtual bool readOnly() const;

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void setRun(run_parameter &run);
    virtual void onBrowseClick();
    virtual void onDescriptionChange();
    virtual void onCalibrationAdd();
    virtual void setReadOnly(bool value);
    virtual void onPropertyChange();
    virtual void onSetupChange();
    virtual void onCalibrationChange();
    virtual void onCalibrationRemove(int id);
    virtual void onCalibrationChange(int id);
#if QT_VERSION < 0x040000
    virtual void onCalibrationRightClick(QListBoxItem *it, const QPoint&p);
    virtual void onFilesRightClick(QListViewItem *it,const QPoint&p);
#else
    virtual void onCalibrationRightClick( const QPoint&p);
    virtual void onFilesRightClick(const QPoint&p);
    virtual void onCalibration(QAction* it);
#endif
    virtual void onFileAdd();
    virtual void onFileTypeChange();
    virtual void onFileNameChange();
    virtual void onFileRemove();
    virtual void setBeamtimes(vector<string> btIn);
    virtual void setRunTypes(vector<string> rtIn);
    virtual void onBTChange();
    virtual void onTPChange();
protected:
    run_parameter *sp;
    run_parameter copy;
    vector<string> runtypes;
    vector<string> beamtimes;
    bool ReadOnly;
    QGridLayout *layout;
protected slots:
    virtual void languageChange();
 
 signals:
 void changed();
private:
#if QT_VERSION < 0x040000
 QListViewItem *tmpItem;
#else
 QTreeWidgetItem *tmpItem;
#endif
    void init();
    void destroy();

};

#endif // RUNDISPLAY_H
