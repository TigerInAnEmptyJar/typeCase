#include "viewselectionwidget.h"

#if QT_VERSION < 0x040000
#include <qheader.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qbutton.h>
#include <qwidgetstack.h>
#else
#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QResizeEvent>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include <qgroupbox.h>
#endif
#include "tofAnalysis_t.h"
#include "utilities.h"
#include <qframe.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "paintparameter.h"
#include "paintwidget.h"
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include "qnumberinputdialog.h"
#include "qstringinputdialog.h"
#include "q3dinputdialog.h"
#include "qvectorchangedialog.h"
#include "qintegerinputdialog.h"
#include "qspinbox.h"
#include "viewselectionwidget.ui.h"
/*
 *  Constructs a ViewSelectionWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
#if QT_VERSION < 0x040000
ViewSelectionWidget::ViewSelectionWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ViewSelectionWidget" );
#else
ViewSelectionWidget::ViewSelectionWidget( QWidget* parent, Qt::WindowFlags f )
    : QWidget( parent, f )
{
  setObjectName("ViewSelectionWidget" );
#endif
  analyserPointer=NULL;
#if QT_VERSION < 0x040000
  widgets=new QWidgetStack(this);//bWidget");
#else
  widgets=new QStackedWidget(this);//bWidget");
#endif
    paints = new PaintWidget(widgets);//, "paintWidget");

    histoWidget = new QWidget(widgets);
    treeWidget = new QWidget(widgets);

    histoLabel=new QLabel(histoWidget);
    histoOptionLabel=new QLabel(histoWidget);
    histoInput=new QLineEdit(histoWidget);
    histoOptionInput=new QLineEdit(histoWidget);

    treeLabel=new QLabel(treeWidget);
    treeOptionLabel=new QLabel(treeWidget);
    treeInput=new QLineEdit(treeWidget);
    treeOptionInput=new QLineEdit(treeWidget);
    treeCutLabel=new QLabel(treeWidget);
    treeCutInput=new QLineEdit(treeWidget);
    histoList=NULL;
    treeList=NULL;
#if QT_VERSION < 0x040000
    histoList=new QListBox(histoWidget);
    treeList=new QListView(treeWidget);
    histoLayout=new QGridLayout(histoWidget,4,2);
    treeLayout=new QGridLayout(treeWidget);
    treeLayout->addMultiCellWidget(treeLabel,1,1,0,0);
    treeLayout->addMultiCellWidget(treeInput,1,1,1,1);
    treeLayout->addMultiCellWidget(treeOptionLabel,2,2,0,0);
    treeLayout->addMultiCellWidget(treeOptionInput,2,2,1,1);
    treeLayout->addMultiCellWidget(treeList,0,0,0,1);
    treeLayout->addMultiCellWidget(treeLabel,3,3,0,0);
    treeLayout->addMultiCellWidget(treeInput,3,3,1,1);
    histoLayout->addMultiCellWidget(histoLabel,1,1,0,0);
    histoLayout->addMultiCellWidget(histoInput,1,1,1,1);
    histoLayout->addMultiCellWidget(histoOptionLabel,2,2,0,0);
    histoLayout->addMultiCellWidget(histoOptionInput,2,2,1,1);
    histoLayout->addMultiCellWidget(histoList,0,0,0,1);
#else
    histoList=new QListWidget(histoWidget);
    treeList=new QTreeWidget(treeWidget);
    treeList->setHeaderLabels(QStringList()<<"Trees");
    histoLayout=new QGridLayout(histoWidget);
    treeLayout=new QGridLayout(treeWidget);
    treeLayout->addWidget(treeLabel,1,0,1,1);
    treeLayout->addWidget(treeInput,1,1,1,1);
    treeLayout->addWidget(treeOptionLabel,2,0,1,1);
    treeLayout->addWidget(treeOptionInput,2,1,1,1);
    treeLayout->addWidget(treeList,0,0,1,2);
    treeLayout->addWidget(treeCutLabel,3,0,1,1);
    treeLayout->addWidget(treeCutInput,3,1,1,1);
    histoLayout->addWidget(histoLabel,1,0,1,1);
    histoLayout->addWidget(histoInput,1,1,1,1);
    histoLayout->addWidget(histoOptionLabel,2,0,1,1);
    histoLayout->addWidget(histoOptionInput,2,1,1,1);
    histoLayout->addWidget(histoList,0,0,1,2);
    treeWidget->setLayout(treeLayout);
    histoWidget->setLayout(histoLayout);
#endif
    

#if QT_VERSION < 0x040000
    widgets->addWidget(paints,0);
    widgets->addWidget(histoWidget,1);
    widgets->addWidget(treeWidget,2);
    widgets->raiseWidget(0);
#else
    widgets->addWidget(paints);
    widgets->addWidget(histoWidget);
    widgets->addWidget(treeWidget);
    widgets->setCurrentWidget(paints);
#endif

    canvasLabel=new QLabel(this);//,"canvasLabel");
    divideButton=new QPushButton(this);//,"divideButton");
    divideButton->setMaximumHeight(23);
    connect(divideButton,SIGNAL(clicked()),this,SLOT(divideCanvas()));
    nx=0;ny=0;
    pp=NULL;
    buttons=NULL;
#if QT_VERSION > 0x040000
    displayType=new QButtonGroup(this);//,"displayType");
    displayTypeBox=new QGroupBox();
    displayTypeLayout= new QGridLayout();
#else
    displayType=new QButtonGroup(tr("display-type"),this);//,"displayType");
#endif
    QWidget *insertTo=NULL;
#if QT_VERSION < 0x040000
    insertTo=displayType;
    QButton *w[6]={
#else
    insertTo=displayTypeBox;
    QAbstractButton *w[6]={
#endif
      new QRadioButton(insertTo),//,"nothing");
      new QRadioButton(insertTo),//,"eventpainter");
      new QRadioButton(insertTo),//,"eventinfo");
      new QRadioButton(insertTo),//,"analysisinfo");
      new QRadioButton(insertTo),//,"tree");
      new QRadioButton(insertTo)};//,"histogram");

    w[0]->setText(tr("nothing"));
    w[1]->setText(tr("paint event"));
    w[2]->setText(tr("event information"));
    w[3]->setText(tr("analysis information"));
    w[4]->setText(tr("tree"));
    w[5]->setText(tr("histogram"));
    for(int i=0;i<6;i++)
      {
#if QT_VERSION < 0x040000
	displayType->insert(w[i],i);
 	w[i]->setGeometry(QRect(10,25*(i+1),150,23 ) );
#else
	displayTypeLayout->addWidget(w[i],i,0,1,1);
	displayType->addButton(w[i]);
	displayType->setId(w[i],i);
#endif
      }
#if QT_VERSION < 0x040000
    connect(displayType,SIGNAL(clicked(int)),this,SLOT(onDisplayTypeChange(int)));
#else
    displayTypeBox->setLayout(displayTypeLayout);
    connect(displayType,SIGNAL(buttonClicked(int)),this,SLOT(onDisplayTypeChange(int)));
#endif
    displayType->setExclusive(true);
#if QT_VERSION < 0x040000
    layout=new QGridLayout(this,4,2);
    buttonLayout=new QGridLayout(this,1,1);
    layout->addMultiCellWidget(canvasLabel,0,0,0,0);
    layout->addMultiCellWidget(divideButton,2,2,0,0);
    layout->addMultiCellWidget(displayType,3,3,0,0);
    layout->addMultiCellWidget(widgets,0,3,1,1);
    layout->addMultiCellLayout(buttonLayout,1,1,0,0);
    layout->setColStretch(0,1);
    layout->setColStretch(1,2);
#else
    layout=new QGridLayout(this);
    buttonLayout=new QGridLayout();//layout);
    layout->addWidget(canvasLabel,0,0,1,1);
    layout->addWidget(divideButton,2,0,1,1);
    layout->addWidget(displayTypeBox,3,0,1,1);
    layout->addWidget(widgets,0,1,4,1);
    layout->addLayout(buttonLayout,1,0,1,1);
    layout->setRowStretch(0,0);
    layout->setRowStretch(1,5);
    layout->setRowStretch(2,0);
    layout->setRowStretch(3,0);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,2);
    setLayout(layout);
#endif
    layout->setMargin(5);
    layout->setSpacing(5);


    connect(histoInput,SIGNAL(textChanged(const QString&)),this,SLOT(histoChanged(const QString &)));
    connect(histoOptionInput,SIGNAL(textChanged(const QString&)),this,SLOT(optionChanged(const QString &)));
    connect(treeInput,SIGNAL(textChanged(const QString&)),this,SLOT(treeChanged(const QString &)));
    connect(treeCutInput,SIGNAL(textChanged(const QString&)),this,SLOT(treeCutChanged(const QString &)));
    connect(treeOptionInput,SIGNAL(textChanged(const QString&)),this,SLOT(optionChanged(const QString &)));
#if QT_VERSION < 0x040000
    connect(histoList,SIGNAL(currentChanged(QListBoxItem*)),this,SLOT(histoChanged(QListBoxItem*)));
    connect(treeList,SIGNAL(currentChanged(QListViewItem*)),this,SLOT(treeChanged(QListViewItem*)));
#else
    connect(histoList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(histoChanged(QListWidgetItem*)));
    connect(treeList,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(treeChanged(QTreeWidgetItem*)));
#endif
    languageChange();
    paints->setReadOnly(false);
    // signals and slots connections

    init();
    setMinimumSize(7*25+100,250);
    resize( QSize(600, 430).expandedTo(minimumSizeHint()) );
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

/*
 *  Destroys the object and frees any allocated resources
 */
ViewSelectionWidget::~ViewSelectionWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ViewSelectionWidget::languageChange()
{
#if QT_VERSION < 0x040000
  setCaption( tr( "ViewSelection-parameter" ) );
    displayType->setTitle(tr("display-type"));
#else
  setWindowTitle( tr( "ViewSelection-parameter" ) );
  displayTypeBox->setTitle(tr("display-type"));
#endif
    canvasLabel->setText(tr("Canvas:"));
    divideButton->setText(tr("Divide"));
    histoLabel->setText(tr("Histogram"));
    histoOptionLabel->setText(tr("Draw Option"));

    treeLabel->setText(tr("Tree-draw-expression"));
    treeCutLabel->setText(tr("Tree-cut-expression"));
    treeOptionLabel->setText(tr("Draw Option"));
}

