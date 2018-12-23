#include <q3Dinput.h>
#if QT_VERSION < 0x040000
#include <qlayout.h>
#else
#include <QMouseEvent>
#include <QResizeEvent>
#include <QGridLayout>
#include <qsize.h>
#endif
#if QT_VERSION < 0x040000
Q3DInput::Q3DInput(QWidget* parent, const char* name, WFlags f):QWidget(parent,name,f)
#else
   Q3DInput::Q3DInput(QWidget* parent, Qt::WindowFlags f ):QWidget(parent,f)
#endif
{
  edX=new QLineEdit(this);
  edY=new QLineEdit(this);
  edZ=new QLineEdit(this);
#if QT_VERSION < 0x040000
  layout=new QGridLayout(this,1,3);
  layout->addWidget(edX,0,0);
  layout->addWidget(edY,0,1);
  layout->addWidget(edZ,0,2);
#else
  layout=new QGridLayout(this);
  layout->addWidget(edX,0,0);
  layout->addWidget(edY,0,1);
  layout->addWidget(edZ,0,2);
  setLayout(layout);
#endif
  layout->setMargin(0);
  layout->setSpacing(1);

  edX->setText("0");
  edY->setText("0");
  edZ->setText("0");
  int wid=width();
  int cwid=wid/3;
  edX->setGeometry(0,0,cwid, height());
  edY->setGeometry(cwid,0,cwid, height());
  edZ->setGeometry(2*cwid,0,cwid, height());
  connect(edX,SIGNAL(textChanged(const QString &)),this,SLOT(setValues()));
  connect(edY,SIGNAL(textChanged(const QString &)),this,SLOT(setValues()));
  connect(edZ,SIGNAL(textChanged(const QString &)),this,SLOT(setValues()));
  setMinimumSize(edX->minimumWidth()*3,20);
#if QT_VERSION < 0x040000
    clearWState( WState_Polished );
#else
    setWindowState( Qt::WindowNoState);
#endif
}

Q3DInput::~Q3DInput()
{
  delete edX;
  delete edY;
  delete edZ;
}

bool Q3DInput::hasFocus()const
{
  return (edX->hasFocus()||edY->hasFocus()||edZ->hasFocus());
}

float Q3DInput::getX()const
{
  return edX->text().toFloat();
}

float Q3DInput::getY()const
{
  return edY->text().toFloat();
}

float Q3DInput::getZ()const
{
  return edZ->text().toFloat();
}

void Q3DInput::mousePressEvent( QMouseEvent * e)
{
  QWidget::mousePressEvent(e);
   if(edX->geometry().contains(e->pos()))
     {
       edX->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
  //  emit clicked();
}

void Q3DInput::mouseReleaseEvent( QMouseEvent * e)
{
  QWidget::mouseReleaseEvent(e);
   if(edX->geometry().contains(e->pos()))
     {
       edX->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
    emit clicked();
}

void Q3DInput::mouseDoubleClickEvent( QMouseEvent * e)
{
  QWidget::mouseDoubleClickEvent(e);
   if(edX->geometry().contains(e->pos()))
     {
       edX->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
   else if(edY->geometry().contains(e->pos()))
     {
       edY->setFocus();
     }
    emit doubleClicked();
}

void Q3DInput::mouseMoveEvent( QMouseEvent * e)
{
  QWidget::mouseMoveEvent(e);
}

void Q3DInput::keyPressEvent( QKeyEvent * e)
{
  QWidget::keyPressEvent(e);
}

void Q3DInput::keyReleaseEvent( QKeyEvent * e)
{
  QWidget::keyReleaseEvent(e);
}

void Q3DInput::paintEvent( QPaintEvent * e)
{
  QWidget::paintEvent(e);
//   edX->repaint();
//   edY->repaint();
//   edZ->repaint();
}


void Q3DInput::setValues(float X,float Y,float Z)
{
  edX->setText(QString::number(X));
  edY->setText(QString::number(Y));
  edZ->setText(QString::number(Z));
  //  emit valueChanged(X,Y,Z);
}
void Q3DInput::valueChanged()
{
  emit valueChanged(edX->text().toFloat(),
		    edY->text().toFloat(),
		    edZ->text().toFloat());
}

void Q3DInput::setValues()
{
  if(QString::number(edX->text().toFloat())!=edX->text())
    edX->setText(QString::number(edX->text().toFloat()));
  if(QString::number(edY->text().toFloat())!=edY->text())
    edY->setText(QString::number(edY->text().toFloat()));
  if(QString::number(edZ->text().toFloat())!=edZ->text())
    edZ->setText(QString::number(edZ->text().toFloat()));
  emit valueChanged(edX->text().toFloat(),
		    edY->text().toFloat(),
		    edZ->text().toFloat());
}

void Q3DInput::resize( int w, int h )
{
  QWidget::resize(w,h);
//   layout->setGeometry(QRect(0,-9,w,h+18));
}

void Q3DInput::resize( const QSize & _so)
{resize(_so.width(),_so.height());}
void Q3DInput::resizeEvent( QResizeEvent * e)
{resize(e->size().width(),e->size().height());}

void Q3DInput::setGeometry( int x, int y, int w, int h )
{
  QWidget::setGeometry(x,y,w,h);
  QResizeEvent e(QSize(w,h),QSize(this->width(),this->height()));
  resizeEvent(&e);
}

void Q3DInput::setGeometry( const QRect & re) // ### make non virtual in Qt 4?
{
  QWidget::setGeometry(re);
  QResizeEvent e(QSize(re.width(),re.height()),QSize(this->width(),this->height()));
  resizeEvent(&e);
}

void Q3DInput::setEnabled(bool e)
{
  QWidget::setEnabled(e);
  edX->setEnabled(e);
  edY->setEnabled(e);
  edZ->setEnabled(e);
}
QSize Q3DInput::sizeHint () const
{
  return QSize(edX->sizeHint().width()+edY->sizeHint().width()+edZ->sizeHint().width(),edX->sizeHint().height());
}
