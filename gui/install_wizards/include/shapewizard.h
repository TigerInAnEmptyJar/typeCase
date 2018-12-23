#ifndef SHAPEWIZARD_H
#define SHAPEWIZARD_H

#include <qvariant.h>
#include <gui/widgets/include/qmywizard.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QWidget;
class QPushButton;
class QLabel;
class QCheckBox;
class QLineEdit;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
class QListBox;
class QListBoxItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
class QListWidget;
class QListWidgetItem;
#endif
class QComboBox;
class QTextEdit;
class QButtonGroup;
class QRadioButton;
class QFrame;
class QSpinBox;
class ShapeDisplay;
#include "shapewriter.h"
using namespace std;

class ShapeWizard : public QMyWizard
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ShapeWizard( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    ShapeWizard( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~ShapeWizard();
    QWidget **wizardPages;
    QGridLayout **pageLayouts;
    QLabel **labels;
    //first page
    QLineEdit *nameInput;
    QLabel *logLabel;
    QPushButton* readButton;
    QPushButton* createButton;
#if QT_VERSION < 0x040000
    QListBox* definedBox;
#else
    QListWidget* definedBox;
#endif
    //second page
    ShapeDisplay *adisplay;
    QPushButton *addButton;
    QPushButton *removeButton;
    //third page
    QLineEdit *headerInput;
    QPushButton *headerBrowse;
    QComboBox *classesBox;
    QComboBox* copyLinkInput;
    QTextEdit* classDefinitionShow;
#if QT_VERSION < 0x040000
    QListBox *sourceList;
#else
    QListWidget *sourceList;
#endif
    //fourth page
    //fifth/last page
    QLineEdit *constrInput;
    QPushButton* takeButton;
    QLabel* tpLabel;
    QTextEdit* constructorEdit;
    QComboBox *constructorSelectBox;
#if QT_VERSION < 0x040000
    QListView *variablesList;
    QListView *parameterList;
#else
    QTreeWidget *variablesList;
    QTreeWidget *parameterList;
#endif
    shape_parameter getShape()const;
public slots:
    virtual void accept();
    virtual void browseHeader();
    virtual void installLogRead();
    virtual void installLogWrite();
    virtual void installLogIO(const string &filename, bool write);
    virtual void setShapes(const vector<shape_parameter> & algos);
    virtual void setShapes(const vector<shape_parameter*> & algos);
    virtual void setShapes(const vector<string> & algos);

signals:
    void helpRequested(const string&);

protected:
    shapeWriter algoWriter;
    shape_parameter parameter;
    vector<TMyClass> classesInHeader;
    void readHeader(const string &filename);
    void readIOFile(const string &filename);
    void setDefault();
    bool checkFinish();
    int parameterFromName(string nam, string tp,int &tpID);
protected slots:
    virtual void languageChange();
    virtual void updateWidgets();
    virtual void paramChanged();
    virtual void displayClass(int num);
    virtual void readParameters();
    virtual void addSource();
    virtual void removeSource(int num);
    virtual void constructorSelected(int num);
    virtual void parseSourceForDescription(const string &filename);
    virtual void pageSelected(const QString & pagename);
    virtual void requestHelp();
    virtual void addParameterClick();
    virtual void removeParameterClick();
#if QT_VERSION < 0x040000
    virtual void variableClick(QListViewItem *it);
    virtual void parameterDoubleClick(QListViewItem *it);
    virtual void sourcesContextMenu(QListBoxItem* it,const QPoint &p);
#else
    virtual void variableClick(QTreeWidgetItem *it);
    virtual void parameterDoubleClick(QTreeWidgetItem *it);
    virtual void sourcesContextMenu(const QPoint &p);
    virtual void onSourceContext(QAction* action);
#endif

#if QT_VERSION < 0x040000
#else
#endif

private:

};

#endif // SHAPEWIZARD_H
