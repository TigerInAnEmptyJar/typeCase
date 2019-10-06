#include "fitAlgorithm.h"
using namespace std;
class AVertexFit : public AFitAlgorithm
{
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
  ~AVertexFit() override;
  void process() override;
  void add(Vector& data) override;
  void clear() override;
  Vector getResult() override;
  //  virtual float fit();
  void getAddPoint(Vector& data) override;
  float fit() override;
  AFitAlgorithm* getClone() override { return nullptr; }
};
