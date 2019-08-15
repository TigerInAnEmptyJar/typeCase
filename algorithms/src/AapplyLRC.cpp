#include "AapplyLRC.h"
//#define DISPLAYPARAMS
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include <math.h>
#include <stdlib.h>
extern float getLightWay(volumeShape* sh, point3D onPoint, bool from);

AapplyLRC::AapplyLRC(TSetup& setup, TTrack** trackIn, int& numTracksIn, int& evtNr, int& rnr,
                     const algorithm_parameter& descr)
    : AAlgorithm("apply signal-run-time"), numberOfTracks(numTracksIn), eventNumber(evtNr),
      runNumber(rnr)
{
  tracks = trackIn;
  nCorrectingDets = descr.value(0).value<vector<int>>().size();
  correctingDets = new int[nCorrectingDets];
  correctingType = new int[nCorrectingDets];
  parameters = new CommonCalibrationParser**[nCorrectingDets];
  currentRange = new int[nCorrectingDets];
  numRanges = new int[nCorrectingDets];
  speedOfLight = 299.792; // [mm/ns]
  speedInMaterial = new float[nCorrectingDets];
  for (int i = 0; i < nCorrectingDets; i++) {
    correctingDets[i] = descr.value(0).value<vector<int>>().at(i);
    correctingType[i] = descr.value(1).value<vector<int>>().at(i);
    speedInMaterial[i] = setup.getDetectorr(correctingDets[i]).getMaterial()->getSpeedOfLight();
    parameters[i] = NULL;
    currentRange[i] = 0;
    numRanges[i] = 0;
#ifdef DISPLAYPARAMS
    cout << "detector " << correctingDets[i] << " do: " << correctingType[i] << endl;
    cout << "\tdo simple signal run      : " << (((correctingType[i] % 10) > 0) ? "yes" : "no")
         << ", speed of light: " << speedInMaterial[i] << endl;
    cout << "\tdo theta dependent polynom: "
         << ((((correctingType[i] % 100) / 10) > 0) ? "yes" : "no") << endl;
    cout << "\tdo r dependent polynom    : "
         << ((((correctingType[i] % 1000) / 100) > 0) ? "yes" : "no") << endl;
// cout<<"\tmean with detector        :
// "<<((correctingType[i]%100000)/1000)-1<<endl;
#endif
  }
}
AapplyLRC::~AapplyLRC()
{
  delete[] correctingDets;
  delete[] correctingType;
  delete[] speedInMaterial;
  for (int i = 0; i < nCorrectingDets; i++) {
    if (parameters[i] != NULL) {
      for (int j = 0; j < numRanges[i]; j++)
        delete parameters[i][j];
      delete[] parameters[i];
    }
  }
  delete[] currentRange;
  delete[] numRanges;
}
void AapplyLRC::onNewRun(run_parameter& rp)
{
  anaLog << "AapplyLRC::onNewRun()" << endli;
  for (int i = 0; i < nCorrectingDets; i++) {
    if (parameters[i] != NULL) {
      for (int k = 0; k < numRanges[i]; k++)
        if (parameters[i][k] != NULL)
          delete parameters[i][k];
      parameters[i] = NULL;
    }
    numRanges[i] = 0;
    currentRange[i] = 0;
  }
  vector<CommonCalibrationParser*> tmp;
  vector<string> files;
  if (rp.hasOwnCalibration() || rp.getParent() == NULL || rp.hasAdditionalCalibration())
    for (size_t i = 0; i < rp.getNumberOfCalibrationFiles(); i++)
      files.push_back(rp.getCalibrationFile(i));
  if (rp.getParent() != NULL && (!rp.hasOwnCalibration() || rp.hasAdditionalCalibration()))
    for (size_t i = 0; i < rp.getParent()->getNumberOfCalibrationFiles(); i++)
      files.push_back(rp.getParent()->getCalibrationFile(i));
  //  int n=0;
  for (unsigned int i = 0; i < files.size(); i++) {
    CommonCalibrationParser::getFileContent(files[i], tmp);
  }
  vector<CommonCalibrationParser*> ttmp[nCorrectingDets];
  bool used;
  while (!tmp.empty()) {
    if (tmp.back()->getParamID() != 8) {
      CommonCalibrationParser* tst = tmp.back();
      tmp.pop_back();
      delete tst;
      continue;
    }
    used = false;
    CommonCalibrationParser* tst = tmp.back();
    tmp.pop_back();
    for (int i = 0; i < nCorrectingDets; i++)
      if (tst->getDetector() == correctingDets[i]) {
        ttmp[i].push_back(tst);
        used = true;
      }
    if (!used)
      delete tst;
  }
  for (int i = 0; i < nCorrectingDets; i++) {
    numRanges[i] = ttmp[i].size();
    anaLog << "detector:" << correctingDets[i] << ", doing " << correctingType[i] << " "
           << numRanges[i] << " range(s) found" << endli;
    parameters[i] = new CommonCalibrationParser*[numRanges[i]];
    for (int j = 0; j < numRanges[i]; j++)
      parameters[i][j] = ttmp[i][j];
    currentRange[i] = 0;
  }
}
#include "shape_utilities.h"
#include "spiral.h"
#include "wedge.h"
void* AapplyLRC::process(void* ptr)
{
  bool found;
  int closest, diff1, diff2, diff3, diff4;
  for (int j = 0; j < nCorrectingDets; j++)
    if ((((correctingType[j] % 1000) / 100 > 0 || correctingType[j] % 100) / 10 > 0) &&
        numRanges[j] != 0) // 1x
    {
      found = (parameters[j][currentRange[j]]->getRange(0) >= runNumber &&
               parameters[j][currentRange[j]]->getRange(1) <= runNumber &&
               parameters[j][currentRange[j]]->getRange(2) >= eventNumber &&
               parameters[j][currentRange[j]]->getRange(3) <= eventNumber);
      if (found)
        continue;
      closest = currentRange[j];
      diff1 = (int)abs(parameters[j][closest]->getRange(0) - runNumber);
      if (abs(parameters[j][closest]->getRange(1) - runNumber) < diff1)
        diff1 = (int)abs(parameters[j][closest]->getRange(1) - runNumber);
      diff2 = (int)abs(parameters[j][closest]->getRange(2) - eventNumber);
      if (abs(parameters[j][closest]->getRange(3) - eventNumber) < diff2)
        diff2 = (int)abs(parameters[j][closest]->getRange(3) - eventNumber);

      for (int k = 0; k < numRanges[j]; k++) {
        diff3 = (int)abs(parameters[j][k]->getRange(0) - runNumber);
        if (abs(parameters[j][k]->getRange(1) - runNumber) < diff3)
          diff3 = (int)abs(parameters[j][k]->getRange(1) - runNumber);
        diff4 = (int)abs(parameters[j][k]->getRange(2) - eventNumber);
        if ((int)abs(parameters[j][k]->getRange(3) - eventNumber) < diff4)
          diff4 = (int)abs(parameters[j][k]->getRange(3) - eventNumber);
        if (diff1 > diff3 || (diff1 == diff3 && diff2 > diff4)) {
          closest = k;
          diff1 = (int)abs(parameters[j][closest]->getRange(0) - runNumber);
          if (abs(parameters[j][closest]->getRange(1) - runNumber) < diff1)
            diff1 = (int)abs(parameters[j][closest]->getRange(1) - runNumber);
          diff2 = (int)abs(parameters[j][closest]->getRange(2) - eventNumber);
          if (abs(parameters[j][closest]->getRange(3) - eventNumber) < diff2)
            diff2 = (int)abs(parameters[j][closest]->getRange(3) - eventNumber);
        }
      }
      currentRange[j] = closest;
    }
  float theta;
  float lrp;
  float tdc;
  volumeShape* sh = NULL;
  TCalibHit* ch;
  sLine3D path;
  point3D p;
  int codd;
  int pos;
  int el;
  for (int i = 0; i < numberOfTracks; i++) {
    path = tracks[i]->getPath();
    theta = path.Direction().Theta();
    for (int j = 0; j < nCorrectingDets; j++) {
      codd = -1;
      if (((correctingType[j] % 100000) / 1000) > 0 && sh != NULL) // nnxxx
        codd = ((correctingType[j] % 100000) / 1000) - 1;
      for (int k = 0; k < tracks[i]->getNumberOfCalibHits(correctingDets[j]); k++) {
        ch = tracks[i]->getCalibSrcr(correctingDets[j], k);
        if (ch == NULL)
          continue;
        if (!ch->isValid())
          continue;
        el = ch->getElement();
        if (el < 0)
          continue;
        sh = ch->getHitShape();
        tdc = ch->getTDC();
        lrp = 0;
        if (sh !=
            NULL) //&&(((correctingType[j]%10)>0||((correctingType[j]%1000)/100)>0)||((correctingType[j]==0))))
        {
          p = getEntrance(sh, path); // getOnPoint(sh, path);
          if (sh->getName() == "wedge") {
            if (((wedge*)sh)->getDistToBarrelCenter() > 0) {
              float alpha =
                  asin(((wedge*)sh)->getDistToBarrelCenter() / ((wedge*)sh)->getOuterRadius());
              float s = (sh->getCenter().Z() * sin(theta) / sin(M_PI - alpha - theta));
              lrp = ((wedge*)sh)->getOuterRadius() - s;
              // 			  if(numberOfTracks==5)cout<<lrp<<endl;
              // lrp=getLightWay(sh,p,
              // false);//((wedge*)sh)->getOuterRadius()-(sh->getCenter()-p).R();
            } else
              lrp = ((wedge*)sh)->getOuterRadius() -
                    ((wedge*)sh)->getCenter().Z() * tan(theta); // getLightWay(sh,p, false);
          } else if (sh->getName() == "spiral")
            lrp = ((spiral*)sh)->getOuterRadius() -
                  ((spiral*)sh)->getCenter().Z() * tan(theta); // getLightWay(sh,p, false);
          else
            lrp = getLightWay(sh, p, false);
        }
        if (codd >= 0) // nnxxx
        {
          pos = -1;
          for (int l = 0; l < tracks[i]->getNumberOfCalibHits(codd); l++)
            if (tracks[i]->getCalibSrcr(codd, l)->getElement() == el)
              pos = l;
          tdc = (tdc + tracks[i]->getCalibSrcr(codd, pos)->getTDC()) / 2.;
        }
        if ((correctingType[j] % 10) > 0 && sh != NULL) // xxxx1
        {
          tdc = tdc + lrp / speedInMaterial[j] / speedOfLight;
        }
        if (((correctingType[j] % 1000) / 100) > 0) // xx1xx
        {
          float corr = 0;
          if (numRanges[j] != 0) {
            corr = (parameters[j][currentRange[j]]->getParameter(el, 0) +
                    (parameters[j][currentRange[j]]->getParameter(el, 1)) * lrp +
                    parameters[j][currentRange[j]]->getParameter(el, 2) * lrp * lrp +
                    parameters[j][currentRange[j]]->getParameter(el, 3) * lrp * lrp * lrp);
          }
          int np = 8;
          float a[np + 1];
          float b[np + 1];
          for (int l = 0; l <= np; l++)
            b[l] = 0;
          for (int l = 0; l <= np; l++)
            a[l] = 0;
          float ccc = 0;

          switch (correctingDets[j]) {
          case 0:
            a[0] = 0.0649565;
            a[1] = 0.00300933;
            a[2] = -1.95769e-05;
            a[3] = 3.48046e-08;
            break;
          case 1:
            a[0] = 0.226073;
            a[1] = 0.00170506;
            a[2] = -1.97018e-05;
            a[3] = 4.56216e-08;
            break;
          case 2:
            a[0] = 0.40712;
            a[1] = -0.00104419;
            a[2] = 3.56141e-06;
            a[3] = -3.26804e-09;
            break;
          case 3:
            a[0] = 0.79919;
            a[1] = 0.000320755;
            a[2] = -7.63443e-07;
            a[3] = 4.90536e-10;
            break;
          case 4:
            a[0] = 0.506389;
            a[1] = 0.00216491;
            a[2] = -3.92822e-06;
            a[3] = 2.24196e-09;
            break;
          case 5:
            a[0] = 0.687344;
            a[1] = 0.000862549;
            a[2] = -1.35606e-06;
            a[3] = 9.04119e-10;
            break;
          case 7:
          case 6:
            a[0] = 10.7335;
            a[1] = -0.0906329;
            a[2] = 0.000254456;
            a[3] = -3.32663e-07;
            a[4] = 2.32629e-10;
            a[5] = -9.01735e-14;
            a[6] = 1.83629e-17;
            a[7] = -1.53867e-21;
            break;
            //                     case
            //                     0:a[0]=-0.20307;a[1]=-0.000253692;a[2]=-2.70493e-06;a[3]=1.04929e-08;break;
            //                     case
            //                     1:a[0]=-0.0944389;a[1]=-0.00101485;a[2]=-1.10251e-06;a[3]=6.95387e-09;break;
            //                     case
            //                     2:a[0]=0.107579;a[1]=-0.00310572;a[2]=1.21451e-05;a[3]=-1.6635e-08;break;
            //                     case
            //                     3:a[0]=1.12719;a[1]=-0.000129904;a[2]=-2.14218e-06;a[3]=1.57669e-09;break;
            //                     case
            //                     4:a[0]=1.02129;a[1]=0.000692803;a[2]=-4.11973e-06;a[3]=2.99295e-09;break;
            //                     case
            //                     5:a[0]=0.930335;a[1]=0.000762831;a[2]=-3.66534e-06;a[3]=2.68965e-09;break;
            //                     case 7:
            //                     case
            //                     6:a[0]=-4.56047;a[1]=-0.000692807;a[2]=5.19447e-05;a[3]=-9.31643e-08;a[4]=6.90157e-11;a[5]=-2.51465e-14;a[6]=4.41586e-18;a[7]=-2.93468e-22;break;
            //                     case
            //                     0:a[0]=-0.408795;a[1]=-0.0020567;a[2]=7.3472e-06;a[3]=-1.22643e-08;break;
            //                     case
            //                     1:a[0]=-0.333372;a[1]=-0.00465605;a[2]=2.03798e-05;a[3]=-3.35683e-08;break;
            //                     case
            //                     2:a[0]=-0.203628;a[1]=-0.0052352;a[2]=2.23221e-05;a[3]=-3.57825e-08;break;
            //                     case
            //                     3:a[0]=1.15878;a[1]=-0.000448928;a[2]=-2.22037e-06;a[3]=1.80855e-09;break;
            //                     case
            //                     4:a[0]=0.972175;a[1]=0.000204541;a[2]=-3.59887e-06;a[3]=2.67106e-09;break;
            //                     case
            //                     5:a[0]=0.844269;a[1]=0.00100276;a[2]=-4.82944e-06;a[3]=3.45472e-09;break;
            //                     case 7:
            //                     case
            //                     6:a[0]=0.641087;a[1]=-0.00503898;a[2]=2.1044e-05;a[3]=-1.98011e-08;a[4]=2.83976e-12;a[5]=4.35714e-15;a[6]=-2.09817e-18;a[7]=2.7672e-22;break;
            //                     case
            //                     0:a[0]=-0.560626;a[1]=-0.000447005;a[2]=-2.8995e-06;a[3]=7.63192e-09;break;
            //                     case
            //                     1:a[0]=-0.455622;a[1]=-0.00385701;a[2]=1.58244e-05;a[3]=-2.54932e-08;break;
            //                     case
            //                     2:a[0]=-0.350862;a[1]=-0.00415544;a[2]=1.58917e-05;a[3]=-2.38114e-08;break;
            //                     case
            //                     3:a[0]=1.22167;a[1]=-0.00160518;a[2]=-2.02932e-07;a[3]=6.78247e-10;break;
            //                     case
            //                     4:a[0]=1.04904;a[1]=-0.000966601;a[2]=-1.48632e-06;a[3]=1.42869e-09;break;
            //                     case
            //                     5:a[0]=0.875786;a[1]=-1.64879e-05;a[2]=-2.70218e-06;a[3]=2.04808e-09;break;
            //                     case 7:
            //                     case
            //                     6:a[0]=37.5025;a[1]=-0.210216;a[2]=0.000481244;a[3]=-5.61629e-07;a[4]=3.64834e-10;a[5]=-1.3354e-13;a[6]=2.58041e-17;a[7]=-2.0502e-21;break;
            //                     case
            //                     0:a[0]=-0.395669;a[1]=-0.00180511;a[2]=2.60575e-06;a[3]=-1.21125e-09;break;
            //                     case
            //                     1:a[0]=-0.309653;a[1]=-0.00478887;a[2]=1.90098e-05;a[3]=-2.89544e-08;break;
            //                     case
            //                     2:a[0]=-0.200533;a[1]=-0.00450668;a[2]=1.5425e-05;a[3]=-2.28913e-08;break;
            //                     case
            //                     3:a[0]=1.41345;a[1]=-0.00159551;a[2]=-3.15002e-07;a[3]=6.42779e-10;break;
            //                     case
            //                     4:a[0]=1.2212;a[1]=-0.000873274;a[2]=-1.61346e-06;a[3]=1.31124e-09;break;
            //                     case
            //                     5:a[0]=1.0852;a[1]=8.29135e-05;a[2]=-3.36319e-06;a[3]=2.4828e-09;break;
            //                     case 7:
            //                     case
            //                     6:a[0]=9.05044;a[1]=-0.0893894;a[2]=0.000304129;a[3]=-4.56869e-07;a[4]=3.55036e-10;a[5]=-1.49239e-13;a[6]=3.23069e-17;a[7]=-2.82885e-21;break;
          }

          ccc = a[np - 1] + b[np - 1];
          for (int l = np - 2; l >= 0; l--)
            ccc = ccc * lrp + a[l] + b[l];
          corr = corr;
          tdc = tdc - corr - ccc;
        }
        if (((correctingType[j] % 100) / 10) > 0) // xxx1x
        {
          tdc = tdc - (parameters[j][currentRange[j]]->getParameter(el, 0) +
                       parameters[j][currentRange[j]]->getParameter(el, 1) * theta +
                       parameters[j][currentRange[j]]->getParameter(el, 2) * theta * theta +
                       parameters[j][currentRange[j]]->getParameter(el, 3) * theta * theta * theta);
        }
        ch->setTDC(tdc);
      }
    }
  }
  return ptr;
}
#include "fiber.h"
#include "ring.h"
#include "spiral.h"
point3D AapplyLRC::getOnPoint(volumeShape* sh, const sLine3D& path)
{
  point3D p;
  point3D cc = sh->getCenter();
  vector3D nor(0, 0, 1);
  if (sh->getName() == "wedge")
    nor = ((wedge*)sh)->getNormal();
  else if (sh->getName() == "spiral")
    nor = ((spiral*)sh)->getNormal();
  else if (sh->getName() == "ring")
    nor = ((ring*)sh)->getNormal();
  else if (sh->getName() == "fiber")
    nor = ((fiber*)sh)->getDirection(2);
  plane3D pl(cc, nor);
  if (sh->getName() == "wedge") {
    if (((wedge*)sh)->getDistToBarrelCenter() > 0) {
      wedge* w = (wedge*)sh;
      float alpha = asin(w->getDistToBarrelCenter() / w->getOuterRadius());
      float s = (w->getCenter().Z() * sin(path.Direction().Theta()) /
                 sin(M_PI - alpha - path.Direction().Theta()));
      p = w->getCenter() -
          vector3D(0, 0, 1) * sqrt(s * s - w->getDistToBarrelCenter() * w->getDistToBarrelCenter());
    } else
      p = pl - path;
  } else
    p = pl - path;
  return p;
}
algorithm_parameter AapplyLRC::getDescription()
{
  algorithm_parameter ret("Light run correction", -1, 0, 0);
  string des = "This algorithm applies a light-run-correction to the tdc of some hit.\n"
               "There are several possibilities:\n"
               "Apply a simple light run correction, "
               "path_of_signal_to_common_point * speed_of_signal_in_detector, "
               "with type \"1\""
               "With type \"10\" the algorithm applies also a theta dependent offset, "
               "a 4th-degree-polynom with the coefficients getting from a "
               " common-calibration-data-base of parameter-ID-8. Type \"100\" "
               "does the "
               "same thing but in orders of path_of_signal_to_common_point.";
  ret.setDescription(des);
  vector<int> tmp;
  ret.addValue("detectors to apply", tmp);
  ret.addValue("type of calibration", tmp);
  return ret;
}
