#ifndef _Q3DIMPUT_
#define _Q3DIMPUT_
#include <qlineedit.h>
class QGridLayout;
using namespace std;
class Q3DInput: public QWidget
{
  Q_OBJECT
 protected:
  QLineEdit *edX,*edY,*edZ;
  QGridLayout* layout;
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );
    virtual void mouseDoubleClickEvent( QMouseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void keyPressEvent( QKeyEvent * );
    virtual void keyReleaseEvent( QKeyEvent * );
    virtual void paintEvent( QPaintEvent * );
     virtual void resizeEvent( QResizeEvent * ); 
 public:
#if QT_VERSION < 0x040000
    Q3DInput(QWidget* parent=0, const char* name=0, WFlags f=Qt::WStyle_NoBorder);
#else
   Q3DInput(QWidget* parent=0, Qt::WindowFlags f = 0 );
#endif
    virtual ~Q3DInput();
    float getX()const;
    float getY()const;
    float getZ()const;
    virtual bool hasFocus()const;
    QSize sizeHint () const;
    protected slots:
      void setValues();
    public slots:
      void setValues(float X,float Y,float Z);
    virtual void valueChanged();
    virtual void	resize( int w, int h );
    virtual void	resize( const QSize & );
    virtual void	setGeometry( int x, int y, int w, int h );
    virtual void	setGeometry( const QRect & ); // ### make non virtual in Qt 4?
    virtual void setEnabled(bool e);
 signals:
    void clicked();
    void clicked(const QPoint &p,int button);
    void doubleClicked();
    void valueChanged(float X, float Y, float Z);    
};
#endif
