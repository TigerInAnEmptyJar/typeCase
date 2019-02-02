#include "Ringpixel.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include "planarShapes.h"
#include "spiral.h"
#include "wedge.h"
#include <stdlib.h>
//#define RINGHISTO
#ifdef RINGHISTO
#include <TFile.h>
#include <TH2.h>
TH1F histo_phi_R("ring phi", "ring: \\phi", 100, -M_PI * 1.05, M_PI * 1.05);
TH1F histo_theta_R("ring theta", "ring: \\theta", 100, 0.1, 0.45);
TH2F histo_xy_R("ring xy", "ring: x-y", 100, -0.5, 0.5, 100, -0.5, 0.5);
TH1F histo_phi_Q("quirl phi", "quirl: \\phi", 100, -M_PI * 1.05, M_PI * 1.05);
TH1F histo_theta_Q("quirl theta", "quirl: \\theta", 100, 0, 0.2);
TH2F histo_xy_Q("quirl xy", "quirl: x-y", 100, -0.2, 0.2, 100, -0.2, 0.2);
#endif

ARingpixel::ARingpixel(bool ParallelprojectionIn, bool SpecialprojectionplaneIn,
                       bool OriginforprojectionpointIn, bool TargetforprojectionpointIn,
                       bool UsetimedifferenceIn, bool CorrectlightflighttimeIn,
                       bool TriangleapproximationIn, point3D projectionpointIn,
                       point3D pointofprojectionplaneIn, vector3D normaltoprojectionplaneIn,
                       TDetector& detS, TDetector& detL, TDetector& detR, int minimumdeltaphiIn,
                       int maximumdeltaphiIn, int minimumnumberofelementsIn,
                       int numberOfSpiralSpiralCrossingsIn, int pixelIDIn, float maxChiIn,
                       float maxDeltaTIn, int* NumberOfPixelsIn, TCalibHit** straightE,
                       TCalibHit** leftE, TCalibHit** rightE, int& nStraight, int& nLeft,
                       int& nRight, TPixel** PixelsIn, int maxOfPixelsIn)
    : AAlgorithm("Ring pixel"), maxChi(maxChiIn), NumberOfRightElements(nRight),
      NumberOfLeftElements(nLeft), NumberOfStraightElements(nStraight), maxDeltaT(maxDeltaTIn),
      maxOfPixels(maxOfPixelsIn)
{
  rightElements = rightE;
  leftElements = leftE;
  straightElements = straightE;

  Parallelprojection = ParallelprojectionIn;
  Specialprojectionplane = SpecialprojectionplaneIn;
  Originforprojectionpoint = OriginforprojectionpointIn;
  Targetforprojectionpoint = TargetforprojectionpointIn;
  Usetimedifference = UsetimedifferenceIn;
  Correctlightflighttime = CorrectlightflighttimeIn;
  Triangleapproximation = TriangleapproximationIn;
  projectionpoint = projectionpointIn;
  minimumdeltaphi = minimumdeltaphiIn;
  maximumdeltaphi = maximumdeltaphiIn;
  minimumnumberofelements = minimumnumberofelementsIn;
  Pixels = PixelsIn;
  pixelID = pixelIDIn;
  NumberOfPixels = NumberOfPixelsIn;
  for (int i = 0; i < maxOfPixels; i++)
    Pixels[i]->setID(pixelID);

  wedge* Sshape = (wedge*)detS.getShape(0);
  centerPhi = Sshape->getLowerPhiEdge().Phi() + Sshape->getPhiRange() * .5;
  spiral* Lshape = (spiral*)detL.getShape(0);
  spiral* Rshape = (spiral*)detR.getShape(0);
  zeroPhiDir = Sshape->getLowerPhiEdge();
  if ((Lshape->getBending() != -Rshape->getBending()) ||
      (Lshape->getOuterRadius() != Rshape->getOuterRadius()) ||
      (Lshape->getOuterRadius() != Sshape->getOuterRadius()) ||
      (Lshape->getInnerRadius() != Rshape->getInnerRadius()) ||
      (Lshape->getInnerRadius() != Sshape->getInnerRadius()) ||
      (detL.getNumberOfElements() != detR.getNumberOfElements()) ||
      (!((Sshape->getNormal() * Lshape->getNormal() / Sshape->getNormal().R() /
          Lshape->getNormal().R()) > 0.99)) ||
      (!((Rshape->getNormal() * Lshape->getNormal() / Rshape->getNormal().R() /
          Lshape->getNormal().R()) > 0.99))) {
    anaLog << "Ring-Pixel: normal vectors: " << Sshape->getNormal().toString(_spheric_)
           << Lshape->getNormal().toString(_spheric_) << Rshape->getNormal().toString(_spheric_)
           << Sshape->getNormal() * Lshape->getNormal() / Sshape->getNormal().R() /
                  Lshape->getNormal().R()
           << " "
           << Rshape->getNormal() * Lshape->getNormal() / Rshape->getNormal().R() /
                  Lshape->getNormal().R()
           << endli;
    anaLog << "Ring-Pixel: inner radii: " << Sshape->getInnerRadius() << " "
           << Lshape->getInnerRadius() << " " << Rshape->getInnerRadius() << endli;
    anaLog << "Ring-Pixel: outer radii: " << Sshape->getOuterRadius() << " "
           << Lshape->getOuterRadius() << " " << Rshape->getOuterRadius() << endli;
    anaLog << "Error in Ringpixel: assigned detectors are not compatible" << endli;
  }
  if (Specialprojectionplane) {
    pointofprojectionplane = pointofprojectionplaneIn;
    normaltoprojectionplane = normaltoprojectionplaneIn;
  } else {
    point3D cR = Rshape->getCenter();
    cR = cR - Rshape->getNormal() * Rshape->getThickness() * 0.5;
    point3D cL = Lshape->getCenter();
    cL = cL - Lshape->getNormal() * Lshape->getThickness() * 0.5;
    point3D cS = Sshape->getCenter();
    cS = cS - Sshape->getNormal() * Sshape->getThickness() * 0.5;
    vector3D v1(cR), v2(cL), v3(cS);
    pointofprojectionplane = point3D((v1 + v2 + v3) / 3.0);
    normaltoprojectionplane = Sshape->getNormal();
  }
  if (Originforprojectionpoint)
    projectionpoint = point3D(0, 0, 0);
  if ((projectionpoint.getState() != _undefined_) && (!Parallelprojection)) {
    vector3D vpl, vS, vL, vR;
    vpl = plane3D(pointofprojectionplane, normaltoprojectionplane) & projectionpoint;
    vS = plane3D(Sshape->getCenter(), Sshape->getNormal()) & projectionpoint;
    vL = plane3D(Lshape->getCenter(), Lshape->getNormal()) & projectionpoint;
    vR = plane3D(Rshape->getCenter(), Rshape->getNormal()) & projectionpoint;
    deltaS = vpl.length() / vS.length();
    deltaL = vpl.length() / vL.length();
    deltaR = vpl.length() / vR.length();
  } else {
    deltaS = 1;
    deltaL = 1;
    deltaR = 1;
  }
  bending = abs(Lshape->getBending());
  outerR = Sshape->getOuterRadius();
  innerR = Sshape->getInnerRadius();
  coverage = outerR / bending;
  numberOfWedges = detS.getNumberOfElements();
  numberOfSpirals = detL.getNumberOfElements();
  numberOfSpiralSpiralCrossings = numberOfSpiralSpiralCrossingsIn;
  phiR = Sshape->getPhiRange();
}

ARingpixel::~ARingpixel()
{
#ifdef RINGHISTO
  TFile f("ring.root", "recreate");
  f.cd();
  histo_phi_R.Write();
  histo_theta_R.Write();
  histo_xy_R.Write();
  histo_phi_R.SetDirectory(0);
  histo_theta_R.SetDirectory(0);
  histo_xy_R.SetDirectory(0);
  histo_phi_Q.Write();
  histo_theta_Q.Write();
  histo_xy_Q.Write();
  histo_phi_Q.SetDirectory(0);
  histo_theta_Q.SetDirectory(0);
  histo_xy_Q.SetDirectory(0);
  f.Close();
  cout << "file \"data/12/analyser/ring.root\" written" << endl;
#endif
}

void* ARingpixel::process(void* ptr)
{
  //  anaLog<<"ring pixel: ";
  int phi, delphi;
  int rL, lL, gL;
  int le, re, ge;
  //  int phigerade;
  int radpix;
  (*NumberOfPixels) = 0;
  //  trackLog<<"pixelNumber:"<<pixelID<<endli;
  for (lL = 0; lL < NumberOfLeftElements; lL++) {
    if ((leftElements[lL]->getHitShape() == 0) || (!(leftElements[lL]->isValid())))
      continue;
    for (rL = 0; rL < NumberOfRightElements; rL++) {
      if ((rightElements[rL]->getHitShape() == 0) || (!(rightElements[rL]->isValid())))
        continue;
      le = leftElements[lL]->getElement();
      re = rightElements[rL]->getElement();
      if ((le < 0) || (le >= numberOfSpirals) || (re < 0) || (re >= numberOfSpirals))
        continue;
      radpix = le - re; // l-r is a measure for theta angle (or radius) of
      if (radpix < 0)
        radpix += numberOfSpirals;
      // pixel, with the outermost pixel having the number 0, JK 08/2000
      if (radpix < numberOfSpiralSpiralCrossings) {
        phi = le + re; // l+r is a measure for the phi angle of pixel
        // JK 08/2000, see thesis M.Dahmen, pp.105/106
        // if (lastPhiLR >= numberOfWedges)
        //	lastPhiLR -= numberOfWedges;
        for (gL = 0; gL < NumberOfStraightElements; gL++) {
          if ((straightElements[gL]->getHitShape() == 0) || (!(straightElements[gL]->isValid())))
            continue;
          ge = straightElements[gL]->getElement();
          if ((ge < 0) || (ge >= numberOfWedges))
            continue;
          delphi = phi - ge;
          if ((abs(delphi) == numberOfSpirals) || (delphi == 0))
            delphi = 0;
          else if ((abs(delphi + 1) == numberOfSpirals) || (delphi == -1))
            delphi = -1;
          else if ((abs(delphi - 1) == numberOfSpirals) || (delphi == 1))
            delphi = 1;
          /*********************************/
          // int ppp=le-re+(le<re?numberOfSpirals:0);
          if (!((Usetimedifference) &&
                ((abs(straightElements[gL]->getTDC() - leftElements[lL]->getTDC()) > maxDeltaT) ||
                 (abs(rightElements[rL]->getTDC() - leftElements[lL]->getTDC()) > maxDeltaT) ||
                 (abs(straightElements[gL]->getTDC() - rightElements[rL]->getTDC()) >
                  maxDeltaT)))) {
            if (delphi >= minimumdeltaphi && delphi <= maximumdeltaphi) {
              Pixels[*NumberOfPixels]->setValid(false);
              planeShape* sh = calculateCenter(rL, lL, gL, delphi, radpix);
              // 			  cout<<sh<<endl;
              if (sh != NULL) {
                // trackLog<<QString::number(le)<<"|"<<QString::number(re)<<"|"<<QString::number(ge)<<"|"<<sh<<"
                // ";
                Pixels[*NumberOfPixels]->reset();
                Pixels[*NumberOfPixels]->addElement(*straightElements[gL]);
                Pixels[*NumberOfPixels]->addElement(*leftElements[lL]);
                Pixels[*NumberOfPixels]->addElement(*rightElements[rL]);
                planeShape* tmp;
                tmp = Pixels[*NumberOfPixels]->getShape();
                Pixels[*NumberOfPixels]->setShape(sh); // calculateCenter(re,le,ge,delphi,radpix));
                if (tmp)
                  delete tmp;
                Pixels[*NumberOfPixels]->setMaxChiSquared(maxChi);
                Pixels[*NumberOfPixels]->setValid(true);
                // cout<<Pixels[*NumberOfPixels]->getShape()<<" "<<sh<<"
                // "<<maxOfPixels<<"
                // "<<*NumberOfPixels<<endl;
                if (Pixels[*NumberOfPixels]->getShape() != sh) {
                  delete sh;
                  Pixels[*NumberOfPixels]->setValid(false);
                  continue;
                }
//			      trackLog<<Pixels[*NumberOfPixels]->getShape()<<endli;
#ifdef RINGHISTO
                if (pixelID == 0) {
                  histo_phi_Q.Fill(sh->getCenter().Phi());
                  histo_theta_Q.Fill(sh->getCenter().Theta());
                  histo_xy_Q.Fill(sin(sh->getCenter().Theta()) * cos(sh->getCenter().Phi()),
                                  sin(sh->getCenter().Theta()) * sin(sh->getCenter().Phi()));
                } else {
                  histo_phi_R.Fill(sh->getCenter().Phi());
                  histo_theta_R.Fill(sh->getCenter().Theta());
                  histo_xy_R.Fill(sin(sh->getCenter().Theta()) * cos(sh->getCenter().Phi()),
                                  sin(sh->getCenter().Theta()) * sin(sh->getCenter().Phi()));
                }
#endif
                if (*NumberOfPixels == maxOfPixels - 1) {
                  anaLog << "max number of pixels found in Quirl/Ring" << endli;
                  return 0;
                }
                (*NumberOfPixels)++;
              }
            }
          }
        }
      }
    }
  }
  return ptr;
}
planeShape* ARingpixel::calculateCenter(int re, int le, int ge, int dPhi, int rp)
{
  int r = rightElements[re]->getElement();
  int l = leftElements[le]->getElement();
  int g = straightElements[ge]->getElement();
  //  float dp=((spiral*)straightElements[ge]->getHitShape())->getPhiRange();
  if (Triangleapproximation)
    return triangleApprox(r, l, g, dPhi, rp);
  if ((straightElements[ge]->getHitShape() == 0) || (leftElements[le]->getHitShape() == 0) ||
      (rightElements[re]->getHitShape() == 0)) {
    return NULL;
  }

  planeShape* ret = NULL; //=new sphericTriangle();

  float dRdPhi = bending;
  // Definition of constants
  // projection constants
  float dLq = deltaL * deltaL;
  float dLk = deltaL * deltaL * deltaL;
  float dRq = deltaR * deltaR;
  float dRk = deltaR * deltaR * deltaR;
  float offs = (abs(l + r - g) > 2 ? ((l + r - g) > 0 ? M_PI : -M_PI) : 0);
  // offsets for the bent elements
  float phiOff1l = offs + coverage - 2 * M_PI * (l + 1) / (float)numberOfSpirals; // inner left
  float phiOff2l = offs + coverage - 2 * M_PI * l / (float)numberOfSpirals;       // outer left
  float phiOff1r = offs - coverage - 2 * M_PI * r / (float)numberOfSpirals;       // inner right
  float phiOff2r = offs - coverage - 2 * M_PI * (r + 1) / (float)numberOfSpirals; // outer right
  //   if ((abs(M_PI-coverage)*360/(2*M_PI))>10)
  //   {
  //     phiOff1l +=- (M_PI-coverage);
  //     phiOff2l +=- (M_PI-coverage);
  //     phiOff1r += (M_PI-coverage);
  //     phiOff2r += (M_PI-coverage);
  //     }
  // hit points
  float phi1 = 2 * M_PI * g / numberOfWedges;
  float phi2 = 2 * M_PI * (g + 1) / numberOfWedges;
  float phi11 = -(deltaL * phiOff1l + deltaR * phiOff1r) / (deltaL + deltaR); //-M_PI;
  float phi22 = -(deltaL * phiOff2l + deltaR * phiOff2r) / (deltaL + deltaR); //-M_PI;
  float phi12 = -(deltaL * phiOff1l + deltaR * phiOff2r) / (deltaL + deltaR); //-M_PI;
  float phi21 = -(deltaL * phiOff2l + deltaR * phiOff1r) / (deltaL + deltaR); //-M_PI;
  //  float ra=straightElements[ge]->getHitShape()->getOuterRadius();
  if (abs(phi11 - phi1) > M_PI / 2)
    return NULL;
  if (phiOff1l + phi12 > M_PI) {
    phiOff1l += -M_PI;
    phiOff2l += -M_PI;
  }
  if (phiOff1r + phi21 < -M_PI) {
    phiOff1r += M_PI;
    phiOff2r += M_PI;
  }
  // triangle to the left, triangle to the right, sixangle
  vector3D centerpoint;
  centerpoint.setRep(_cylindric_);
  // triangle to the left
  if (phi12 < phi2) {
    float x1 = phi1;
    float x2 = phi12;
    float area = 0.5 * dRdPhi * dRdPhi *
                 ((dRq - dLq) * x2 * x2 * x2 / 3.0 + (dRq * phiOff2r - dLq * phiOff1l) * x2 * x2 +
                  (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * x2 -
                  (dRq - dLq) * x1 * x1 * x1 / 3.0 - (dRq * phiOff2r - dLq * phiOff1l) * x1 * x1 -
                  (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * x1);
    float phiCP = 0.5 * dRdPhi * dRdPhi / area *
                  ((dRq - dLq) * x2 * x2 * x2 * x2 / 4.0 +
                   (dRq * phiOff2r - dLq * phiOff1l) * x2 * x2 * x2 * 2.0 / 3.0 +
                   (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * x2 * x2 / 2.0 -
                   (dRq - dLq) * x1 * x1 * x1 * x1 / 4.0 -
                   (dRq * phiOff2r - dLq * phiOff1l) * x1 * x1 * x1 * 2.0 / 3.0 -
                   (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * x1 * x1 / 2.0);
    float rCP =
        -dRdPhi * dRdPhi * dRdPhi / 3.0 / area *
        ((dRk + dLk) * (x2 * x2 * x2 * x2 - x1 * x1 * x1 * x1) / 4.0 +
         (dRk * phiOff2r + dLk * phiOff1l) * (x2 * x2 * x2 - x1 * x1 * x1) +
         (dRk * phiOff2r * phiOff2r + dLk * phiOff1l * phiOff1l) * (x2 * x2 - x1 * x1) * 1.5 +
         (dRk * phiOff2r * phiOff2r * phiOff2r + dLk * phiOff1l * phiOff1l * phiOff1l) * (x2 - x1));
    centerpoint.setValues(rCP * cos(phiCP), rCP * sin(phiCP), pointofprojectionplane.Z());
    float t1 = dRdPhi * (x1 + phiOff1l);
    point3D p1(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
    t1 = dRdPhi * (x2 + phiOff1l);
    point3D p2(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
    t1 = -dRdPhi * (x1 + phiOff2r);
    point3D p3(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
    ret = new sphericTriangle(centerpoint, p1, p2, p3);
  }

  // triangle to the right
  else if (phi21 > phi1) {
    float x1 = phi21;
    float x2 = phi2;
    float area = 0.5 * dRdPhi * dRdPhi *
                 ((dLq - dRq) * (x2 * x2 * x2 - x1 * x1 * x1) / 3.0 +
                  (dLq * phiOff2l - dRq * phiOff1r) * (x2 * x2 - x1 * x1) +
                  (dLq * phiOff2l * phiOff2l - dRq * phiOff1r * phiOff1r) * (x2 - x1));
    float phiCp =
        0.5 * dRdPhi * dRdPhi / area *
        ((dLq - dRq) * (x2 * x2 * x2 * x2 - x1 * x1 * x1 * x1) / 4.0 +
         (dLq * phiOff2l - dRq * phiOff1r) * (x2 * x2 * x2 - x1 * x1 * x1) * 2.0 / 3.0 +
         (dLq * phiOff2l * phiOff2l - dRq * phiOff1r * phiOff1r) * (x2 * x2 - x1 * x1) / 2.0);
    float rCP =
        dRdPhi * dRdPhi * dRdPhi *
        ((dLk + dRk) * (x2 * x2 * x2 * x2 - x1 * x1 * x1 * x1) / 4.0 +
         (dLk * phiOff2l + dRk * phiOff1r) * (x2 * x2 * x2 - x1 * x1 * x1) +
         (dLk * phiOff2l * phiOff2l + dRk * phiOff1r * phiOff1r) * (x2 * x2 - x1 * x1) * 1.5 +
         (dLk * phiOff2l * phiOff2l * phiOff2l + dRk * phiOff1r * phiOff1r * phiOff1r) *
             (x2 - x1)) /
        (3.0 * area);
    centerpoint.setValues(rCP * cos(phiCp), rCP * sin(phiCp), pointofprojectionplane.Z());
    float t1 = dRdPhi * (x2 + phiOff2l);
    point3D p1(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
    t1 = dRdPhi * (x1 + phiOff2l);
    point3D p2(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
    t1 = -dRdPhi * (x2 + phiOff1r);
    point3D p3(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
    ret = new sphericTriangle(centerpoint, p1, p2, p3);
  }

  // sixangle
  else // if(!((phi12<phi2)||(phi21>phi1)))
  {    // sixangle
    float x1 = phi1;
    float x2 = (phi11 < phi22 ? phi11 : phi22);
    float x3 = (phi11 < phi22 ? phi22 : phi11);
    float x4 = phi2;
    float aH;
    float pH;
    float rH;
    if (phi11 < phi22) {
      aH = dLq * ((phiOff2l - phiOff1l) * (phi22 * phi22 - phi11 * phi11) +
                  (phiOff2l * phiOff2l - phiOff1l * phiOff1l) * (phi22 - phi11));
      pH = dLq *
           ((phiOff2l - phiOff1l) * (phi22 * phi22 * phi22 - phi11 * phi11 * phi11) / 1.5 +
            (phiOff2l * phiOff2l - phiOff1l * phiOff1l) * (phi22 * phi22 - phi11 * phi11) * 0.5);
      rH = dLk *
           ((phiOff2l - phiOff1l) * (phi22 * phi22 * phi22 - phi11 * phi11 * phi11) +
            (phiOff2l * phiOff2l - phiOff1l * phiOff1l) * (phi22 * phi22 - phi11 * phi11) * 1.5 +
            (phiOff2l * phiOff2l * phiOff2l - phiOff1l * phiOff1l * phiOff1l) * (phi22 - phi11));
    } else {
      aH = dRq * ((phiOff2r - phiOff1r) * (-phi22 * phi22 + phi11 * phi11) +
                  (phiOff2r * phiOff2r - phiOff1r * phiOff1r) * (-phi22 + phi11));
      pH = dRq *
           ((phiOff2r - phiOff1r) * (-phi22 * phi22 * phi22 + phi11 * phi11 * phi11) / 1.5 +
            (phiOff2r * phiOff2r - phiOff1r * phiOff1r) * (-phi22 * phi22 + phi11 * phi11) * 0.5);
      rH = dLk *
           ((phiOff2r - phiOff1r) * (phi22 * phi22 * phi22 - phi11 * phi11 * phi11) +
            (phiOff2r * phiOff2r - phiOff1r * phiOff1r) * (phi22 * phi22 - phi11 * phi11) * 1.5 +
            (phiOff2r * phiOff2r * phiOff2r - phiOff1r * phiOff1r * phiOff1r) * (phi22 - phi11));
    }
    float area = 0.5 * dRdPhi * dRdPhi *
                 (((dLq - dRq) * (x2 * x2 * x2 - x1 * x1 * x1) / 3.0 +
                   (dLq * phiOff2l - dRq * phiOff1r) * (x2 * x2 - x1 * x1) +
                   (dLq * phiOff2l * phiOff2l - dRq * phiOff1r * phiOff1r) * (x2 - x1)) +
                  aH + ((dRq - dLq) * (x4 * x4 * x4 - x3 * x3 * x3) / 3.0 +
                        (dRq * phiOff2r - dLq * phiOff1l) * (x4 * x4 - x3 * x3) +
                        (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * (x4 - x3)));
    float phiCP =
        0.5 * dRdPhi * dRdPhi / area *
        (((dLq - dRq) * (x2 * x2 * x2 * x2 - x1 * x1 * x1 * x1) / 4.0 +
          (dLq * phiOff2l - dRq * phiOff1r) * (x2 * x2 * x2 - x1 * x1 * x1) / 1.5 +
          (dLq * phiOff2l * phiOff2l - dRq * phiOff1r * phiOff1r) * (x2 * x2 - x1 * x1) / 2.0) +
         pH +
         ((dRq - dLq) * (x4 * x4 * x4 * x4 - x3 * x3 * x3 * x3) / 4.0 +
          (dRq * phiOff2r - dLq * phiOff1l) * (x4 * x4 * x4 - x3 * x3 * x3) / 1.5 +
          (dRq * phiOff2r * phiOff2r - dLq * phiOff1l * phiOff1l) * (x4 * x4 - x3 * x3) / 2.0));
    float rCP =
        dRdPhi * dRdPhi * dRdPhi / 3.0 / area *
        (((dLk + dRk) * (x2 * x2 * x2 * x2 - x1 * x1 * x1 * x1) / 4.0 +
          (dLk * phiOff2l + dRk * phiOff1r) * (x2 * x2 * x2 - x1 * x1 * x1) +
          (dLk * phiOff2l * phiOff2l + dRk * phiOff1r * phiOff1r) * (x2 * x2 - x1 * x1) * 1.5 +
          (dLk * phiOff2l * phiOff2l * phiOff2l + dRk * phiOff1r * phiOff1r * phiOff1r) *
              (x2 - x1)) +
         rH - ((dRk + dLk) * (x4 * x4 * x4 * x4 - x3 * x3 * x3 * x3) / 4.0 +
               (dRk * phiOff2r + dLk * phiOff1l) * (x4 * x4 * x4 - x3 * x3 * x3) +
               (dRk * phiOff2r * phiOff2r + dLk * phiOff1l * phiOff1l) * (x4 * x4 - x3 * x3) * 1.5 +
               (dRk * phiOff2r * phiOff2r * phiOff2r + dLk * phiOff1l * phiOff1l * phiOff1l) *
                   (x4 - x3)));
    centerpoint.setValues(rCP * cos(phiCP), rCP * sin(phiCP), pointofprojectionplane.Z());
    point3D p1, p2, p3, p4, p5, p6;
    if (phi11 < phi22) {
      float t1 = -dRdPhi * (x1 + phiOff1r);
      p1.setValues(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
      t1 = dRdPhi * (x2 + phiOff2l);
      p2.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
      t1 = -dRdPhi * (x2 + phiOff1r);
      p3.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
      t1 = dRdPhi * (x1 + phiOff1l);
      p4.setValues(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
      t1 = -dRdPhi * (x3 + phiOff2r);
      p5.setValues(t1 * cos(x3), t1 * sin(x3), centerpoint.Z());
      t1 = -dRdPhi * (x2 + phiOff2r);
      p6.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
    } else {
      float t1 = -dRdPhi * (x2 + phiOff2r);
      p1.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
      t1 = dRdPhi * (x1 + phiOff2l);
      p2.setValues(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
      t1 = -dRdPhi * (x2 + phiOff2r);
      p3.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
      t1 = dRdPhi * (x2 + phiOff2l);
      p4.setValues(t1 * cos(x2), t1 * sin(x2), centerpoint.Z());
      t1 = -dRdPhi * (x3 + phiOff1r);
      p5.setValues(t1 * cos(x3), t1 * sin(x3), centerpoint.Z());
      t1 = -dRdPhi * (x1 + phiOff1r);
      p6.setValues(t1 * cos(x1), t1 * sin(x1), centerpoint.Z());
    }
    ret = new sphericTriangle(centerpoint, p1, p2, p3, p4, p5, p6);
  }

  return ret;
}

planeShape* ARingpixel::triangleApprox(int r, int l, int g, int dPhi, int rp)
{
  float z;
  float dR = phiR * bending;

  z = pointofprojectionplane.Z();
  float lichtLeitWegS = rp * dR;

  float rad1 = outerR - lichtLeitWegS;
  float rad2 = r - l;
  if (rp) {
    rad2 = outerR - lichtLeitWegS + dR;
  } else {
    rad2 = outerR;
  }
  float rad3 = outerR - lichtLeitWegS - dR;

  point3D p1, p2, p3;
  //   double phi1=g*phiR;
  //   double phi2=(g+1)*phiR;
  p1.setRep(_spheric_);
  p2.setRep(_spheric_);
  p3.setRep(_spheric_);
  if (!dPhi) {
    p1.setValues(cos(g * phiR - phiR / 2. + centerPhi) * rad1,
                 sin(g * phiR - phiR / 2. + centerPhi) * rad1, z);
    p2.setValues(cos(g * phiR + phiR / 2. + centerPhi) * rad2,
                 sin(g * phiR + phiR / 2. + centerPhi) * rad2, z);
    p3.setValues(cos(g * phiR + phiR / 2. + centerPhi) * rad3,
                 sin(g * phiR + phiR / 2. + centerPhi) * rad3, z);
  } else {
    p1.setValues(cos(g * phiR + phiR / 2. + centerPhi) * rad1,
                 sin(g * phiR + phiR / 2. + centerPhi) * rad1, z);
    p2.setValues(cos(g * phiR - phiR / 2. + centerPhi) * rad2,
                 sin(g * phiR - phiR / 2. + centerPhi) * rad2, z);
    p3.setValues(cos(g * phiR - phiR / 2. + centerPhi) * rad3,
                 sin(g * phiR - phiR / 2. + centerPhi) * rad3, z);
  }
  //    point3D H=p1+(p2-p1)*0.5;
  //    point3D sp=H+((p3-H)*(1/3));
  int nn = -1, nm = -1;
  for (int i = 0; i < NumberOfStraightElements; i++)
    if (straightElements[i]->getElement() == g)
      nn = i;
  for (int i = 0; i < NumberOfLeftElements; i++)
    if (leftElements[i]->getElement() == l)
      nm = i;
  vector3D cc(straightElements[nn]->getHitShape()->getCenter().X(),
              straightElements[nn]->getHitShape()->getCenter().Y(), 0);
  if (!(nm == -1 || nn == -1)) {
    vector3D vs = ((wedge *)straightElements[nn]->getHitShape())->getLowerPhiEdge(),
             vl = ((spiral *)leftElements[nm]->getHitShape())->getLowerPhiEdge(), r = p1,
             nl = ((spiral *)leftElements[nm]->getHitShape())->getNormal();
    r = r - nl * (r * nl);
    float b = ((spiral*)leftElements[nm]->getHitShape())->getBending();
    matrix3D turn(nl, r.R() / b); //-
    vl = turn * vl;
    if (vs * vl < 0)
      return NULL;
  }
  planeShape* ret = new triangle(p1 + cc, p2 + cc, p3 + cc);
  return ret;
}
