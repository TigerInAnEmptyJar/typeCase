#ifndef _EVENTPAINTER_
#define _EVENTPAINTER_
#include "container.h"
#include <TCanvas.h>
#include <QtCore/QObject>
#include <TObject.h>
#include <vector>
#include "shapes.h"
using namespace std;

class EventPainter:public QObject
{
    Q_OBJECT
private:
    TEvent *Event;
    TPad *canvas;
    point2D upperLeft;
    point2D lowerRight;
    vector<TObject*> drawings;
    vector<TBase*> drawnObjects;
    vector<int> detectors;
    vector<int> detectorColors;
    vector<int> pixels;
    vector<int> pixelColors;
    vector<int> pixelMarker;
    vector<int> trackColors;
    vector<int> particleColors;
    bool detSorted;
    void paintHits(/* float zoom */);
    void paintPixels(/* float zoom */);
    void paintTracks(/* float zoom */);
    bool trackLike;
    bool particleLike;
public:
    EventPainter();
    EventPainter(TEvent *e,TPad *c);
    EventPainter(TEvent *e,TPad *c,float left,float right, float up, float down);
    vector<TBase*> getDrawnObjects(double x,double y);
public slots:
    void setTrackLike(bool like=true);
    void setEvent(TEvent *e);
    void setCanvas(TPad *c);
    void setSize(float left,float right, float up, float down);
    void setSize(point2D upperLeftIn,point2D lowerRightIn); 
    void rePaint(TEvent *e);
    void rePaint();
    void addDetector(int detNum, int color=1);
    void addPixel(int pixNum, int color=1, int marker=1);
    void addTrackColor(int color);
    void addParticleColor(int ID, int color);
    void clearDetectors();
    void clearPixels();
    void clearTrackColor();
    void clearParticleColor();
    void removeDetector(int detNum);
    void removePixel(int pixNum);
    void setDetector(int detNum, int color);
    void setPixel(int pixNum, int color);
    void setPixel(int pixNum, int color, int marker);
    void setTrackSorted(bool sort=true);
    void setPaintMode(int mode);
    void setParticleMode(bool value=true);
};
    
#endif
