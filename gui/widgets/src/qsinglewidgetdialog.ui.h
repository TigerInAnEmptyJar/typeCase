void QSingleWidgetDialog::setWidget(QWidget* aWidget)
{
#if QT_VERSION < 0x040000
  if(theWidget!=NULL)
    layout->remove(theWidget);
  theWidget=aWidget;
  layout->addMultiCellWidget(theWidget,0,0,0,3);
#else
  if(theWidget!=NULL)
    layout->removeWidget(theWidget);
  theWidget=aWidget;
  layout->addWidget(theWidget,0,0,1,4);
#endif
}
void QSingleWidgetDialog::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void QSingleWidgetDialog::resize(const QSize&s){resize(s.width(),s.height());}
void QSingleWidgetDialog::resize(int w, int h)
{
  QDialog::resize(w,h);
}
