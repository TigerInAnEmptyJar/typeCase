#include "fitAlgorithm.h"
#include "track.h"

using namespace std;
class ALineFitAlgorithm : public AFitAlgorithm
{
private:
  const int maxLines;
  int numLines; //!
  Vector* A_;   //!
  float* b_;    //!
  Vector re;    //!
public:
  ALineFitAlgorithm(int maxLines);
  virtual ~ALineFitAlgorithm();
  void process() override;
  virtual void clear();
  virtual void add(Vector& data);
  virtual Vector getResult();
  virtual void getAddPoint(Vector& data);
  virtual float fit();
  AFitAlgorithm* getClone() override { return nullptr; }
};
