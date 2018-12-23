#include "ioalgorithms.h"
void OrderSelectionWidget::init()
{
QAlgorithmDefineWidget *w;
#include "addInputAlgoWidget.h"
#include "addOutputAlgoWidget.h"
}
void OrderSelectionWidget::destroy()
{
}
vector<algorithm_parameter*> OrderSelectionWidget::getOrder(){ return order->getOrder();}
void OrderSelectionWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void OrderSelectionWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void OrderSelectionWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
  tabWidget->resize(w-10,h-10);
}
void OrderSelectionWidget::setAlgorithms(vector<algorithm_parameter> &a){order->setAlgorithms(a);}
void OrderSelectionWidget::setAlgorithms(vector<algorithm_parameter*> &a){order->setAlgorithms(a);}
void OrderSelectionWidget::algorithmSelect(algorithm_parameter &algo, bool value){order->algorithmSelect(algo,value);}
