#include "fitAlgorithm.h"
using namespace std;
class AVertexFit : public AFitAlgorithm
{
  Q_OBJECT
private:
  const int maxLines;
  int numLines1, numLines2;     //!
  Vector *A_1, *A_2;            //!
  float *b_1, *b_2, *z_1, *z_2; //!
  Vector re, re1, re2;          //!
  float fit1();
  float fit2();

public:
  AVertexFit(int numberOflines);
  virtual ~AVertexFit();
  virtual void* process(void* ptr);
  virtual void add(Vector& data);
  virtual void clear();
  virtual Vector getResult();
  //  virtual float fit();
public slots:
  virtual void getAddPoint(Vector& data);
  virtual float fit();
signals:
  virtual void fitted(float chi, Vector& result);
};
