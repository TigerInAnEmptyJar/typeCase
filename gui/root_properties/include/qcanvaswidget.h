#ifndef _QCANVASWIDGET_
#define _QCANVASWIDGET_
#if QT_VERSION > 0x040000
#include <QEvent>
#include <QCustomEvent>
#include <QPoint>
#include <QSize>
#endif
#include <TQtWidget.h>
#include <geometry.h>
using namespace std;
class QCanvasWidget: public TQtWidget
{
    Q_OBJECT
private:
    int selectedPadN;
public:
#if QT_VERSION < 0x040000
  QCanvasWidget( QWidget* parent=0, const char* name=0, WFlags f=Qt::WStyle_NoBorder, bool embedded=TRUE);
#else
  QCanvasWidget( QWidget* parent=0,  Qt::WindowFlags f=0, bool embedded=TRUE);
#endif
  virtual ~QCanvasWidget();
  int selectedPadNumber()const;
  TPad* selectedPad();
  point2D canvasCoordinates(const QPoint &p);
  point2D canvasCoordinates(int subPad,const QPoint &p);
  QPoint canvasCoordinates(float x, float y);
  QPoint canvasCoordinates(const point2D &p);
  int subPadNumberAt(const QPoint &p);
  int subPadNumberAt(const point2D &p);
  TVirtualPad* subPadAt(const QPoint &p);
  TVirtualPad* subPadAt(const point2D &p);
  ///*   bool hasFocus()const; */
public slots:
   virtual void cd();
   virtual void cd(int subpadnumber);
   virtual void Clear();
   virtual void Divide(int nx, int ny);
protected:
   virtual void enterEvent       ( QEvent *      );
   virtual void mouseDoubleClickEvent(QMouseEvent* );
   virtual void mouseMoveEvent   ( QMouseEvent * );
   virtual void mousePressEvent  ( QMouseEvent * );
   virtual void mouseReleaseEvent( QMouseEvent * );
   virtual void keyPressEvent    ( QKeyEvent *   );
   virtual void keyReleaseEvent  ( QKeyEvent *   );
   virtual void wheelEvent ( QWheelEvent * e );
signals:
    void rightMouseButtonPressed(const QPoint &p);
    void leftMouseButtonPressed(const QPoint &p);
    void keyPressed();
    void doubleClicked(const QPoint &p);
    void doubleClicked(int subPad,const QPoint &p);
    void rightMouseButtonClick(int subPad,const QPoint &p);
    void rightMouseButtonClick(const QPoint &p);
    void leftMouseButtonClick(int subPad,const QPoint &p);
    void leftMouseButtonClick(const QPoint &p);
    void mouseMooved(const QPoint &p);
    void mouseMooved(int subPad,const QPoint &p);
    void mouseWheeled(int subPad, bool up, const QPoint &p);
    void mouseWheeled(bool up, const QPoint &p);
};
#endif
