#ifndef FITALGORITHM
#define FITALGORITHM
#include "algorithm.h"
#include "geometry.h"
class AFitAlgorithm: public AAlgorithm
{
  Q_OBJECT
 public:
  AFitAlgorithm(const string & name=string());
  virtual ~AFitAlgorithm();
  virtual void *process(void*ptr);
  virtual void add(Vector &data);
  virtual void clear();
  virtual Vector getResult();
  virtual AFitAlgorithm* getClone();
  public slots:
    virtual void getAddPoint(Vector &data);
  virtual float fit();
 signals:
  virtual void fitted(float chi, Vector &result);
};
#endif
