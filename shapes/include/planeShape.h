#ifndef PLANESHAPE
#define PLANESHAPE
#include "baseShape.h"
#include "geometry.h"
class sphericTriangle;
class planeShape : public base_shape
{
protected:
  vector3D normal;
  float circRadius;

public:
  planeShape(const string& n);
  planeShape(const shape_parameter& description);
  virtual ~planeShape();
  vector3D getNormal() const;
  void setNormal(const vector3D& v);
  plane3D getPlane() const;
  void setPlane(const plane3D& p);
  virtual int getNumberOfPoints() const;
  virtual point3D getPoint(int num) const;
  virtual float area() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  virtual float circumscribedRadius() const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};

class triangle : public planeShape
{
protected:
  point3D A_, B_, C_;

public:
  triangle(point3D a = point3D(), point3D b = point3D(), point3D c = point3D());
  triangle(point3D a, vector3D v1, vector3D v2);
  triangle(const planeShape& s);
  triangle(const shape_parameter& description);
  virtual ~triangle();
  virtual point3D getPoint(int num) const;
  virtual int getNumberOfPoints() const;
  void setPoint(int num, point3D p);
  void setPoints(point3D a, point3D b, point3D c);
  point3D A() const;
  point3D B() const;
  point3D C() const;
  void setA(point3D p);
  void setB(point3D p);
  void setC(point3D p);
  virtual float area() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  void operator=(const triangle& t);
  void operator=(const sphericTriangle& t);
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};

class sphericTriangle : public planeShape
{
protected:
  bool realTriangle;
  point3D A_, B_, C_, D_, E_, F_;

public:
  sphericTriangle(point3D cen, point3D a = point3D(), point3D b = point3D(), point3D c = point3D(),
                  point3D d = point3D(), point3D e = point3D(), point3D f = point3D());
  sphericTriangle(const planeShape& s);
  sphericTriangle(const shape_parameter& description);
  virtual ~sphericTriangle();
  virtual int getNumberOfPoints() const;
  virtual point3D getPoint(int num) const;
  void setPoint(int num, point3D p);
  void setPoints(point3D cen, point3D a, point3D b, point3D c, point3D d = point3D(),
                 point3D e = point3D(), point3D f = point3D());
  point3D A() const;
  point3D B() const;
  point3D C() const;
  point3D D() const;
  point3D E() const;
  point3D F() const;
  void setA(point3D p);
  void setB(point3D p);
  void setC(point3D p);
  void setD(point3D p);
  void setE(point3D p);
  void setF(point3D p);
  virtual float area() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  bool isRealTriangle();
  triangle getTriangle() const;
  void operator=(const sphericTriangle& t);
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};

class rectangle : public planeShape
{
protected:
  lLine3D left;
  lLine3D right;

public:
  rectangle(point3D A = point3D(), point3D B = point3D(), vector3D norm = vector3D(),
            float width = 0);
  rectangle(point3D Ce, vector3D N, point3D A = point3D(), point3D B = point3D(),
            point3D C = point3D(), point3D D = point3D());
  rectangle(const planeShape& s);
  rectangle(const shape_parameter& description);
  virtual ~rectangle();
  virtual int getNumberOfPoints() const;
  virtual point3D getPoint(int num) const;
  point3D A() const;
  point3D B() const;
  point3D C() const;
  point3D D() const;
  virtual float area() const;
  float getWidth() const;  // distance AD
  float getLength() const; // distance AB
  void setPoints(point3D A, point3D B, vector3D norm, float width);
  lLine3D AB() const;
  lLine3D BC() const;
  lLine3D CD() const;
  lLine3D DA() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  void operator=(const rectangle& r);
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};

class hexagon : public planeShape
{
protected:
  point3D A_;

public:
  hexagon(point3D cen = point3D(), point3D a = point3D(), vector3D norm = point3D());
  hexagon(const planeShape& s);
  hexagon(const shape_parameter& description);
  virtual ~hexagon();
  virtual int getNumberOfPoints() const;
  virtual point3D getPoint(int num) const;
  void setA(point3D p);
  void setPoints(point3D cen, point3D a, vector3D norm);
  point3D A() const;
  point3D B() const;
  point3D C() const;
  point3D D() const;
  point3D E() const;
  point3D F() const;
  virtual float area() const;
  virtual float angularRange_Phi(const point3D& origin = point3D(0, 0, 0),
                                 const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual float angularRange_Theta(const point3D& origin = point3D(0, 0, 0),
                                   const vector3D& zDirection = vector3D(0, 0, 1)) const;
  virtual vector3D distancePlane(const plane3D& p);
  virtual vector3D distance(const sLine3D& line);
  lLine3D AB() const;
  lLine3D BC() const;
  lLine3D CD() const;
  lLine3D DE() const;
  lLine3D EF() const;
  lLine3D FA() const;
  void operator=(const hexagon& h);
  virtual void Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                    int fColor = 8, int fStyle = 1001) const;
  virtual void Draw(const point3D& eye = point3D(0, 0, 0),
                    const plane3D& plane = plane3D(point3D(0, 0, 1), vector3D(0, 0, 1)),
                    vector4D* boundingBox = NULL, TObject** ident = NULL, int lColor = 1,
                    int fColor = 8, int fStyle = 1001) const;
  virtual shape_parameter description() const;
  static shape_parameter getDescription();
};
inline planeShape::planeShape(const string& n) : base_shape(n)
{
  setCenter(point3D(0, 0, 0));
  normal = vector3D(0, 0, 1);
}

inline planeShape::~planeShape() {}

inline vector3D planeShape::getNormal() const { return normal; }

inline void planeShape::setNormal(const vector3D& v) { normal = v; }

inline float planeShape::area() const { return -1; }

#endif
