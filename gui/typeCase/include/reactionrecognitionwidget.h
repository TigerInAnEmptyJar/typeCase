/****************************************************************************
** Form interface generated from reading ui file 'reactionrecognitionwidget.ui'
**
** Created: Mon Nov 30 15:59:14 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef REACTIONRECOGNITIONWIDGET_H
#define REACTIONRECOGNITIONWIDGET_H

#include <qvariant.h>
#include <qwidget.h>
#include "algorithmparameter.h"
#include "algorithmdisplay.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
#if QT_VERSION < 0x040000
class QListView;
class QListViewItem;
#else
class QTreeWidget;
class QTreeWidgetItem;
#endif
using namespace std;

class ReactionRecognitionWidget : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ReactionRecognitionWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ReactionRecognitionWidget( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~ReactionRecognitionWidget();

    QLabel* textLabel2;
    QPushButton* pushButton1;
#if QT_VERSION < 0x040000
    QListView* listView1;
#else
    QTreeWidget* listView1;
#endif
    virtual int getReactionPosByName( QString an );
    virtual algorithm_parameter * getAlgorithm();

public slots:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( const QSize & s );
    virtual void resize( int w, int h );
    virtual void OnHelpAsked();
    virtual void OnHelpAsked( QString filename );
    virtual void OnReactionsShow();
#if QT_VERSION < 0x040000
    virtual void OnReactionSelect( QListViewItem * it );
    virtual void OnReactionRightClick( QListViewItem * it, const QPoint & p );
#else
    virtual void OnReactionSelect( QTreeWidgetItem * it );
    virtual void OnReactionRightClick( const QPoint & p );
    virtual void useReaction( QAction *ac );
#endif
    virtual void newReaction();
    virtual void includeReaction();
    virtual void useReaction( int id );
    virtual void OnReactionSelected( algorithm_parameter * ap );
    virtual void OnReactionUnselected( algorithm_parameter * ap );
    virtual void ReadFromFile( QString fn );
    virtual void WriteToFile( QString fn );
    virtual void OnInsertClick();

signals:
    void insertClick(algorithm_parameter*,bool);
    void helpAsked(QString);
    void helpRequested(const string &);
    void reactionSelected(algorithm_parameter*,bool);
    void reactionUnselected(algorithm_parameter*);

protected:
    AlgorithmDisplay *algoDisp;
    algorithm_parameter RecognitionAlgorithm;
    vector<algorithm_parameter> reacts;
    int selectedR;


protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // REACTIONRECOGNITIONWIDGET_H
