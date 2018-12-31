#include "lineFitAlgorithm.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include <stdlib.h>
using namespace std;

ALineFitAlgorithm::ALineFitAlgorithm(int max)
    : AFitAlgorithm("line fit algorithm"), maxLines(max), numLines(0), re(4)
{
  A_ = new Vector[maxLines];
  for (int i = 0; i < maxLines; i++)
    A_[i].reDimension(4);
  b_ = new float[maxLines];
}

ALineFitAlgorithm::~ALineFitAlgorithm()
{
  delete[] A_;
  delete[] b_;
}

void* ALineFitAlgorithm::process(void* ptr) { return ptr; }

void ALineFitAlgorithm::clear() { numLines = 0; }
void ALineFitAlgorithm::add(Vector& data)
{
  if ((numLines == maxLines) || (data.getDimension() < 5))
    return;
  b_[numLines] = data.getValue(0);
  for (int i = 1; i < 5; i++)
    A_[numLines].setValue(i - 1, data.getValue(i));
  numLines++;
}

void ALineFitAlgorithm::getAddPoint(Vector& data)
{
  if ((numLines == maxLines) || (data.getDimension() < 5))
    return;
  b_[numLines] = data.getValue(0);
  for (int i = 1; i < 5; i++)
    A_[numLines].setValue(i - 1, data.getValue(i));
  numLines++;
}

Vector ALineFitAlgorithm::getResult() { return re; }

float ALineFitAlgorithm::fit()
{
  // cout<< numLines<<" Elements to fit"<<endl;
  if (numLines <= 0)
    return -1;
  matrixNxM A(numLines, 4), At(4, numLines);
  Vector b(numLines), ret(4);
  b.transpose();
  ret.transpose();
  for (int i = 0; i < numLines; i++) {
    b.setValue(i, b_[i]);
    A.setValue(i, 0, A_[i][0]);
    A.setValue(i, 1, A_[i][1]);
    A.setValue(i, 2, A_[i][2]);
    A.setValue(i, 3, A_[i][3]);
  }
  At = !A;
  matrixNxM A_1(At * A);
  //  cout<<A_1<<A_1.Determinant()<<flush;
  A_1 = -A_1;
  //  cout<<endl;
  if (A_1.Lines() == 0)
    return 100000000;
  ret = (A_1) * (At * b);
  re = ret;
  double chi2 = 0;
  //  double tmp;
  Vector cC(numLines);
  cC = A * ret;
  cC = cC - b;
  chi2 = cC * cC;
  if (4 != numLines)
    chi2 = chi2 / abs(4 - numLines);
  //   for (int i = 0; i < numLines; i++)
  //     {
  //       tmp = cC.getValue (i) - b.getValue (i);
  //       if (A.Columns () - A.Lines () != 0)
  // 	chi2 += tmp * tmp / abs(A.Columns () - A.Lines ());//strange
  // normalization
  //     }				// dangerous div by 0 possible
  return chi2;
}

// void ALineFitAlgorithm::calculateParams(int tr, int num)
// {
//   if(1>tracks[tr]->getNumberOfCalibHits(num))
//     {
//       cout<<"no hit in here"<<endl;
//     }
//   volumeShape *sh=tracks[tr]->getCalibSrcr(num,0)->getHitShape();
//   sLine3D li=sh->Normal(tracks[tr]->getPath());
//   coord=li.Foot();
//   vector3D atm;
//   if(sh->getName()=="fiber")
//     {
//       sigma=0.5*sh->getDirection(2).length();
//       sLine3D
//       ml(sh->getCorner()+sh->getDirection(1)*0.5+sh->getDirection(2)*0.5,sh->getDirection(0));
//       atm=ml-coord;
//       if(atm.length()<0.0001)
// 	atm=sh->getDirection(1);
//       atm.normalize();
//       atm=atm*(-1);
//     }
//   else if(sh->getName()=="wedge")
//     {
//       sigma=0.5*sh->getPhiRange()*(coord-sh->getCenter()).length();
//       matrix3D turn(sh->getNormal(),-sh->getPhiRange()*0.5);
//       atm=turn*sh->getLowerPhiEdge();
//       sLine3D ml(sh->getCenter(),atm);
//       atm=ml-coord;
//       if(atm.length()<0.0001)
// 	atm=ml.Direction()^sh->getNormal();
//       atm.normalize();
//     }
//   else if(sh->getName()=="spiral")
//     {
//       vector3D radiusV = coord - sh->getCenter();
//       float radius = radiusV.length ();
//       if (radiusV.length() < sh->getInnerRadius())
// 	radius = sh->getInnerRadius();
//       if (radiusV.length() > sh->getOuterRadius())
// 	radius = sh->getOuterRadius();
//       float phi=radius/sh->getBending();
//       matrix3D turn(sh->getNormal(),-phi);
//       atm=(sh->getCenter()+((turn*sh->getLowerPhiEdge())*radius))-coord;
//       atm.normalize();
//       sigma=sh->getPhiRange()*sh->getBending()/2;
//       sigma=sigma*(atm*(coord-sh->getCenter()));
//     }
//   else if(sh->getName()=="cylinder")
//     {
//       sigma=sh->getOuterRadius();
//       atm=sh->getLine()-coord;
//     }
//   pitch=atm;
//   npc=coord+atm;
// }
// void ALineFitAlgorithm::Streamer(TBuffer &b)
// {
// }
