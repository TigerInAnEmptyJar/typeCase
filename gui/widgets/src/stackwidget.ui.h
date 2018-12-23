void StackWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void StackWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void StackWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
  selector->setGeometry(w/5,1,3*w/5,25);
  stackw->setGeometry(1,27,w-2,h-28);
}
void StackWidget::addWidget(QWidget* widget, const QString &widgetName)
{
  if(widget==NULL)return;
#if QT_VERSION < 0x040000
  stackw->addWidget(widget,selector->count());
  selector->insertItem(widgetName);
#else
  stackw->addWidget(widget);
  selector->addItem(widgetName);
#endif    
}
void StackWidget::widgetSelect(int num)
{
  if(num<0||num>=(int)selector->count())return;
#if QT_VERSION < 0x040000
  stackw->raiseWidget(num);
#else
  stackw->setCurrentIndex(num);
#endif
}
