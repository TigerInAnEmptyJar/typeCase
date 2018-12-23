#ifndef QMYWIZARD_H
#define QMYWIZARD_H

#include <qvariant.h>
#include <qdialog.h>
#include <vector>

#include <qpushbutton.h>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
#if QT_VERSION > 0x040000
class QDialogButtonBox;
class QStackedWidget;
#else
class QWidgetStack;
#endif
class QFrame;
class QGridLayout;
class QLineEdit;

using namespace std;

class QMyWizard : public QDialog
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    QMyWizard( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
#else
    QMyWizard( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~QMyWizard();

    virtual void addPage ( QWidget * page, const QString & title ) ;
    virtual void insertPage ( QWidget * page, const QString & title, int index ) ;
    virtual void removePage ( QWidget * page ) ;
    QString title ( QWidget * page ) const ;
    void setTitle ( QWidget * page, const QString & title ) ;
    virtual void showPage ( QWidget * page ) ;
    virtual void showPageIndex ( int index ) ;
    QWidget * currentPage () const ;
    QWidget * page ( int index ) const ;
    int pageCount () const ;
    int indexOf ( QWidget * page ) const ;
    virtual bool appropriate ( QWidget * page ) const ;
    virtual void setAppropriate ( QWidget * page, bool appropriate ) ;
    QPushButton * backButton () const {return buttonBack;}
    QPushButton * nextButton () const {return buttonNext;}
    QPushButton * finishButton () const {return buttonFinish;}
    QPushButton * cancelButton () const {return buttonCancel;}
    QPushButton * helpButton () const {return buttonHelp;}

private:
    QGridLayout* layout;
    QSpacerItem* Horizontal_Spacing2;
    QLabel *pageTitleLabel;
    QFrame *separator;
    QPushButton* buttonHelp;
    QPushButton* buttonBack;
    QPushButton* buttonNext;
    QPushButton* buttonFinish;
    QPushButton* buttonCancel;
#if QT_VERSION < 0x040000
    QWidgetStack* thePages;
    QGridLayout* buttonlayout;
    QWidget *buttonWidget;
#else
    QDialogButtonBox* buttons;
    QStackedWidget* thePages;
#endif
    int current;
    vector<QString> titles;
    vector<bool> approp;
    vector<bool> hasBack;
    vector<bool> hasNext;
    vector<bool> hasHelp;
    vector<bool> hasFinish;
protected slots:
    virtual void languageChange();
 virtual void next();
 virtual void back();
 virtual void finish();
 virtual void help();
 virtual void cancel();
#if QT_VERSION > 0x040000
 virtual void buttonClicked(QAbstractButton* but);
#else
#endif
public slots:
  virtual void setBackEnabled ( QWidget * page, bool enable ) ;
  virtual void setNextEnabled ( QWidget * page, bool enable ) ;
  virtual void setFinishEnabled ( QWidget * page, bool enable ) ;
  virtual void setHelpEnabled ( QWidget * page, bool enable ) ;

 signals:
 void helpClicked ();
 void selected ( const QString & );
};

#endif // QMYWIZARD_H
