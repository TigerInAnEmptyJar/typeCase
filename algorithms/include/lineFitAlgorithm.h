#include "fitAlgorithm.h"
#include "track.h"

using namespace std;
class ALineFitAlgorithm : public AFitAlgorithm
{
  Q_OBJECT
private:
  const int maxLines;
  int numLines; //!
  Vector* A_;   //!
  float* b_;    //!
  Vector re;    //!
public:
  ALineFitAlgorithm(int maxLines);
  virtual ~ALineFitAlgorithm();
  virtual void* process(void* ptr);
  virtual void clear();
  virtual void add(Vector& data);
  virtual Vector getResult();
public slots:
  virtual void getAddPoint(Vector& data);
  virtual float fit();
signals:
  void fitted(float chi, Vector& result);
};
