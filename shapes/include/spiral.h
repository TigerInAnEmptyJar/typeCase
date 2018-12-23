#ifndef SPIRAL
#define SPIRAL
#include "volumeShape.h"
class spiral: public volumeShape
{
protected:
    float innerR;
    float outerR;
    float bending;
    float dphi;
    float thickness;
    vector3D normal;
    vector3D lpe;
    vector3D hpe;
public:
    spiral(float i=0, float a=1000, float b=1, float d=1,float t=1, point3D c=point3D(0,0,0), vector3D n=vector3D(0,0,1), vector3D l=vector3D(1,0,0));
    spiral(const volumeShape &s);
    spiral(const shape_parameter &description);
    ~spiral();
    virtual float getInnerRadius()const;
    virtual float getOuterRadius()const;
    virtual float getBending() const;
    virtual float getPhiRange() const;
    virtual float getThickness()const;
    virtual vector3D getNormal() const;
    virtual vector3D getLowerPhiEdge() const;
    void setInnerRadius(float v);
    void setOuterRadius(float v);
    void setBending(float v);
    void setPhiRange(float v);
    void setThickness(float t);
    void setCenter(point3D c);
    void setNormal(vector3D v);
    void setLowerPhiEdge(vector3D v);
    virtual volumeShape *getNext(int times,int stackType);
    virtual volumeShape *getEnvelope(int times, int stackType);
    virtual vector3D distance(const sLine3D &line);
    virtual sLine3D Normal(const sLine3D &line);
    virtual bool cut(const fiber &f);
    virtual void operator=(volumeShape *s);
    virtual volumeShape* getClone();
    virtual Vector HitParams(const sLine3D &line);
    virtual Vector HitParams(const planeShape &shape, point3D origin);
    virtual Vector Hitting(const sLine3D &line);
    virtual float getFlightPathInShape(const sLine3D &line);
    virtual point3D entrance(const sLine3D &line);
    virtual int suspect(const sLine3D &line, int stackType);
  virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
  virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
  virtual shape_parameter description()const;
  static shape_parameter getDescription();
};
#endif
