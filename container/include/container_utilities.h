#ifndef CONTAINER_UTILITIES
#define CONTAINER_UTILITIES
class TTrack;
class TCalibHit;
vector3D getPath(int nstart, int *start, int nstop, int *stop, TTrack* tr, const sLine3D &path);
vector3D getPath(int nstart, int *start, int nstop, int *stop, int nhits, TCalibHit** hits, const sLine3D &path);
vector3D getPath(int nstart, TCalibHit **start, int nstop, TCalibHit** stop, const sLine3D &path);
#endif
