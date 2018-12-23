#ifndef REACTIONDISPLAY_H
#define REACTIONDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include "detectorparameter.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QCheckBox;
class QFrame;
class QCanvasWidget;
class ShapeDisplay;
class base_shape;
using namespace std;

class ReactionDisplay : public QWidget
{
    Q_OBJECT

public:
#if QT_VERSION < 0x040000
    ReactionDisplay( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
#else
    ReactionDisplay( QWidget* parent = 0, Qt::WindowFlags f = 0 );
#endif
    ~ReactionDisplay();
    virtual bool readOnly();
    virtual bool doDraw();
 protected:
    QCheckBox* twoBeam;
    QLabel* mBLabel;
    QLabel* mTLabel;
    QLabel* beamLabel;
    QLabel* targetLabel;
    QWidget* matBeam;
    QWidget* matTarget;
    QLineEdit* beamInput;
    QLineEdit* targetInput;
    QCanvasWidget *shapeDraw;
    ShapeDisplay *shape;
    QGridLayout* myLayout;

public slots:
    virtual void setReaction( reaction_parameter & reactionIn );
    virtual void setReadOnly( bool value );
    virtual void onPropertyChange();
    virtual void setMaterials(const vector<string>& mats);
    virtual void OnDraw();
    virtual void setDraw(bool value);

    virtual void onBeamNumberChange();
signals:
    void changed();

protected:
    reaction_parameter *sp;
    bool ReadOnly;
    reaction_parameter copy;
    base_shape *drawer;
    bool DrawShape;
    vector<string> materials;
    shape_parameter copyshape;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // REACTIONDISPLAY_H
