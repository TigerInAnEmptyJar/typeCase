#ifndef ALGORITHMWIZARD_H
#define ALGORITHMWIZARD_H

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
class AlgorithmDisplay;
#include "algorithmwriter.h"
using namespace std;

class AlgorithmWizard : public QMyWizard
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    AlgorithmWizard( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    AlgorithmWizard( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~AlgorithmWizard();
    QWidget **wizardPages;
    QGridLayout **pageLayouts;
    QLabel **labels;
    //first page
    QLineEdit *nameInput;
    QCheckBox *copyInput;
    QLabel *logLabel;
    QPushButton* readButton;
    QPushButton* createButton;
    QComboBox* catBox;
    QCheckBox* ioCheckBox;
    QPushButton* ioBrowseButton;
    QComboBox *ioSelectBox;
    QComboBox *ioClassBox;
    QLabel *ioLabel;
    QLabel *ioSourceLabel;
#if QT_VERSION < 0x040000
    QListBox* definedBox;
#else
    QListWidget* definedBox;
#endif
    //second page
    AlgorithmDisplay *adisplay;
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
    QButtonGroup *frequencyGroup;
    QComboBox* frequencyBox;
    QSpinBox* fitBox;
    QLineEdit *commentInput;
#if QT_VERSION < 0x040000
    QListBox *signalList;
    QListBox *slotList;
    QListView *connectionList;
    QListViewItem *currentConnection;
#else
    QListWidget *signalList;
    QListWidget *slotList;
    QTreeWidget *connectionList;
    QTreeWidgetItem *currentConnection;
#endif
    //fifth/last page
    QLineEdit *constrInput;
    QPushButton* takeButton;
    QLabel* tpLabel;
    QTextEdit* constructorEdit;
#if QT_VERSION < 0x040000
    QListView *variablesList;
    QListView *parameterList;
#else
    QTreeWidget *variablesList;
    QTreeWidget *parameterList;
#endif
    algorithm_parameter getAlgorithm()const;
public slots:
    virtual void accept();
    virtual void browseHeader();
    virtual void browseIOHeader();
    virtual void installLogRead();
    virtual void installLogWrite();
    virtual void installLogIO(const string &filename, bool write);
    virtual void setAlgorithms(const vector<algorithm_parameter> & algos);
    virtual void setAlgorithms(const vector<algorithm_parameter*> & algos);
    virtual void setAlgorithms(const vector<string> & algos);

signals:
    void helpRequested(const string&);

protected:
    algorithmWriter algoWriter;
    algorithm_parameter parameter;
    vector<TMyClass> classesInHeader;
    vector<TMyClass> IOClasses;
    TMyClass anaClass;
    void readHeader(const string &filename);
    void readIOFile(const string &filename);
    void setDefault();
    bool checkFinish();
protected slots:
    virtual void languageChange();
    virtual void copyClick();
    virtual void ioClick();
    virtual void updateWidgets();
    virtual void paramChanged();
    virtual void displayClass(int num);
    virtual void readParameters();
    virtual void addSource();
    virtual void removeSource(int num);
    virtual void parseSourceForDescription(const string &filename);
    virtual void addConnection();
    virtual void removeConnection();
    virtual void pageSelected(const QString & pagename);
    virtual void requestHelp();
#if QT_VERSION < 0x040000
    virtual void connectionsContextMenu(QListViewItem *it,const QPoint &p);
    virtual void variableClick(QListViewItem *it);
    virtual void parameterDoubleClick(QListViewItem *it);
    virtual void sourcesContextMenu(QListBoxItem* it,const QPoint &p);
#else
    virtual void connectionsContextMenu(const QPoint &p);
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

#endif // ALGORITHMWIZARD_H
