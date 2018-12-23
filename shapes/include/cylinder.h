#ifndef CYLINDER
#define CYLINDER
#include "volumeShape.h"
class cylinder: public volumeShape
{
protected:
    vector3D direction;
    float radius;
public:
    cylinder(point3D c=point3D(0,0,0), vector3D d=vector3D(0,1,0), float v=5);
    virtual ~cylinder();
    cylinder(const volumeShape &s);
    cylinder(const shape_parameter &description);
    virtual vector3D getDirection(int num=0) const;
    virtual lLine3D getLine() const;
    float getRadius()const;
    void setLine(const lLine3D &l);
    void setLine(point3D p, point3D q);
    void setLine(point3D c, vector3D d);
    void setRadius(float r);
    virtual volumeShape *getNext(int times,int stackType);
    virtual volumeShape *getEnvelope(int stackType);
    virtual vector3D distance(const sLine3D &l);
    virtual sLine3D Normal(const sLine3D &line);
    virtual bool cut(const fiber &f);
    virtual void operator=(const volumeShape &s);
    virtual volumeShape* getClone();
    virtual Vector HitParams(const sLine3D &line);
    virtual Vector HitParams(const planeShape &shape, point3D origin);
    virtual point3D entrance(const sLine3D &line);
    virtual Vector Hitting(const sLine3D &line);
    virtual void Draw(const point3D &eye, const plane3D &plane, vector4D* boundingBox, int lColor, int fColor=8, int fStyle=1001)const;
    virtual void Draw(const point3D &eye=point3D(0,0,0), const plane3D &plane=plane3D(point3D(0,0,1),vector3D(0,0,1)), vector4D* boundingBox=NULL, TObject **ident=NULL, int lColor=1, int fColor=8, int fStyle=1001)const;
    virtual shape_parameter description()const;
  static shape_parameter getDescription();
};


#endif
