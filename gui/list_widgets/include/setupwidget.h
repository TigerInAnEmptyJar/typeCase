#ifndef SETUPWIDGET
#define SETUPWIDGET


#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"
#include "materialparameter.h"
class DetectorWidget;
class MaterialWidget;
class ShapeWidget;
class ReactionDisplay;
class QTabWidget;
using namespace std;

class SetupWidget: public QWidget
{
    Q_OBJECT
public:
#if QT_VERSION < 0x040000
    SetupWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    SetupWidget( QWidget* parent = 0,Qt::WindowFlags f = 0 );
#endif
    ~SetupWidget();
    QTabWidget* tabWidget;
    DetectorWidget* detwidget;
    ShapeWidget* shpwidget;
    MaterialWidget* matwidget;
    ReactionDisplay* rdisplay;
public slots:
    virtual void resize( const QSize & s );
    virtual void resizeEvent( QResizeEvent * e );
    virtual void resize( int w, int h );
    virtual void setSetup(vector<detector_parameter> &d,vector<material_parameter> &m,vector<shape_parameter> &s,reaction_parameter &r);
    virtual void setSetup(vector<detector_parameter*> &d,vector<material_parameter*> &m,vector<shape_parameter*> &s,reaction_parameter &r);
    virtual void setShapes(vector<shape_parameter> &s);
    virtual void setShapes(vector<shape_parameter*> &s);
    virtual void setDetectors(vector<detector_parameter> &d);
    virtual void setDetectors(vector<detector_parameter*> &d);
    virtual void setMaterials(vector<material_parameter> &m);
    virtual void setMaterials(vector<material_parameter*> &m);
    virtual void setReaction(reaction_parameter &r);
    virtual void setReaction(reaction_parameter* r);
protected slots:
/*   virtual void tabPageChanged(); */
signals:
    void changed();
 private:
    void init();
    void destroy();
    void languageChange();
};

#endif //SETUPWIDGET
