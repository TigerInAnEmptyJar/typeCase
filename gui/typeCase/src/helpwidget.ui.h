void HelpWidget::resizeEvent( QResizeEvent * e ){resize(e->size().width(),e->size().height());}
void HelpWidget::resize( const QSize & s ){resize(s.width(),s.height());}
void HelpWidget::resize( int w, int h )
{
  QWidget::resize(w,h);
  textEdit1->setGeometry(1,1,w-2,h-2);
}
void HelpWidget::setHelp(const string & filename)
{
  textEdit1->setSource(
#if QT_VERSION < 0x040000
		       filename.data()
#else
		       QUrl(filename.data())
#endif
		       );
  this->show();
}
void HelpWidget::init()
{
  textEdit1->setSource(
#if QT_VERSION < 0x040000
		       "http://www.pit.physik.uni-tuebingen.de/~ehrhardt/KTOF/index.html"
#else
		       QUrl("http://www.pit.physik.uni-tuebingen.de/~ehrhardt/KTOF/index.html")
#endif
		       );
  textEdit1->setReadOnly(true);
}
