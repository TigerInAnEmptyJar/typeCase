#ifndef SHAPE_UTILITIES
#define SHAPE_UTILITIES
#include "baseShape.h"
class planeShape;
class volumeShape;
vector3D getEntrance(base_shape* shape, const sLine3D& path);
vector3D getEntrance(volumeShape* shape, const sLine3D& path);
vector3D getEntrance(planeShape* shape, const sLine3D& path);
vector3D getPath(int nstart, base_shape** start, int nstop, base_shape** stop, const sLine3D& path);
vector3D ConeLine(const point3D& tip, const vector3D& rotaxis, const float& alpha,
                  const float& height, const sLine3D& path);
#endif
