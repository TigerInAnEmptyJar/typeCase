#include "BarrelPixel.h"
#include "CommonCalibrationParser.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include "sphericRectangle.h"
#include "wedge.h"
#include <TH1.h>
#include <fstream>
#include <vector>
#define USEHPEFORDEF
//#define DEBUGBARRELPIXEL
#ifdef DEBUGBARRELPIXEL
#include <TFile.h>
#include <TTree.h>
TTree* bpixel_outTree;
Int_t tree_element;
Float_t tree_tdc[2];
Float_t tree_r;
Float_t tree_theta;
Float_t tree_phi;
Float_t tree_params[3];
TH1F rHisto("Theta", "Theta", 10000, -3, 5000);
#endif
ABarrelPixel::ABarrelPixel(int maxPixelsIn, int IDIn, int& numPixels, TPixel** pixelsIn,
                           int& numHits1, int& numHits2, TCalibHit** h1, TCalibHit** h2,
                           TDetector* front, float pixelSizeIn, int& eventNr, int& runNr,
                           const vector<string>& fileNames)
    : AAlgorithm("barrel pixel"), maxPixels(maxPixelsIn), ID(IDIn), numberOfPixels(numPixels),
      numberOfHits1(numHits1), numberOfHits2(numHits2), eventNumber(eventNr), runNumber(runNr)
{
  pixels = pixelsIn;
  hits1 = h1;
  hits2 = h2;
  volumeShape* shape = front->getShape(0);
  radius = ((wedge*)shape)->getOuterRadius();
  radiusIn = ((wedge*)shape)->getInnerRadius();
  center = ((wedge*)shape)->getCenter();
  dB = ((wedge*)shape)->getDistToBarrelCenter();
  radiusMid = sqrt(center.Z() * center.Z() + dB * dB); //(radius+radiusIn)*0.5;
  phiRange = ((wedge*)shape)->getPhiRange();
  coneLength = sqrt(radius * radius - dB * dB);
  lightSpeed = 60; // front->getMaterial()->getSpeedOfLight();
  ps = pixelSizeIn;
  //  int DetID=front->getID();
  NumEl = front->getNumberOfElements();
  for (int i = 0; i < maxPixels; i++)
    pixels[i]->setID(ID);
  // init parameter
  numRanges = 0;
  int numParams = 4;
  zParams = new CommonCalibrationParser*[numRanges];
  for (int i = 0; i < numRanges; i++)
    zParams[i] = NULL;
  range = new int*[numParams];
  for (int i = 0; i < numParams; i++)
    range[i] = new int[numRanges];
  for (int i = 0; i < numParams; i++)
    for (int j = 0; j < numRanges; j++)
      range[i][j] = 0;
// done

#ifdef DEBUGBARRELPIXEL
  TFile* f = new TFile("barrelpixel.root", "recreate");
  f->cd();
  bpixel_outTree = new TTree("barrelpixel", "barrelpixel");
  bpixel_outTree->Branch("element", &tree_element, "element/I");
  bpixel_outTree->Branch("tdc", &tree_tdc, "tdc[2]/F");
  bpixel_outTree->Branch("r", &tree_r, "r/F");
  bpixel_outTree->Branch("theta", &tree_theta, "theta/F");
  bpixel_outTree->Branch("phi", &tree_phi, "phi/F");
  bpixel_outTree->Branch("params", &tree_params, "params/F");
#endif
}
ABarrelPixel::~ABarrelPixel()
{
  if (zParams != NULL) {
    for (int i = 0; i < numRanges; i++) {
      if (zParams[i] != NULL)
        delete zParams[i];
    }
    delete[] zParams;
  }
  if (range != NULL) {
    for (int i = 0; i < 4; i++)
      delete[] range[i];
    delete[] range;
  }
#ifdef DEBUGBARRELPIXEL
  TFile* f = bpixel_outTree->GetCurrentFile();
  f->cd();
  bpixel_outTree->Write();
  f->Close();
  delete f;
#endif
}

void* ABarrelPixel::process(void* ptr)
{
  numberOfPixels = 0;
  if (!((runNumber >= range[0][actualRange]) && (runNumber <= range[1][actualRange]) &&
        (eventNumber >= range[2][actualRange]) && (eventNumber <= range[3][actualRange]))) {
    bool rangeFound = false;
    for (int i = 0; i < numRanges; i++) {
      if ((runNumber >= range[0][i]) && (runNumber <= range[1][i]) &&
          (eventNumber >= range[2][i]) && (eventNumber <= range[3][i])) {
        actualRange = i;
        rangeFound = true;
      }
    }
    if ((!rangeFound) && (numRanges > 0)) {
      cout << "range not found for zbarrel" << endl;
      return 0;
    }
  }
  planeShape* ttmp;
  int n;
  for (int i = 0; i < numberOfHits1; i++) {
    if (!(hits1[i]->isValid()))
      continue;
    if (hits1[i]->getRawTDC() == -1)
      continue;
    for (int j = 0; j < numberOfHits2; j++) {
      if (!(hits2[j]->isValid()))
        continue;
      if (hits2[j]->getRawTDC() == -1)
        continue;
      if (numberOfPixels < maxPixels) {
        pixels[numberOfPixels]->setValid(false);
        if (hits1[i]->getElement() == hits2[j]->getElement()) {
          n = hits1[i]->getElement();
          if (!(hits1[i]->getHitShape()))
            continue;
          if ((hits1[i]->getRawTDC() == -1) || (hits2[j]->getRawTDC() == -1))
            continue;
#ifdef USEHPEFORDEF
          norm = ((wedge*)hits1[i]->getHitShape())->getHigherPhiEdge();
#else
          norm = ((wedge*)hits1[i]->getHitShape())->getNormal();
#endif
          lpe = ((wedge*)hits1[i]->getHitShape())->getLowerPhiEdge();
          ttmp = getShape(n, hits1[i]->getTDC(), hits2[j]->getTDC());
          if (ttmp) {
            pixels[numberOfPixels]->reset();
            pixels[numberOfPixels]->addElement(*hits1[i]);
            pixels[numberOfPixels]->addElement(*hits2[j]);
            planeShape* tmp = pixels[numberOfPixels]->getShape();
            pixels[numberOfPixels]->setShape(ttmp);
            if (tmp)
              delete tmp;
            pixels[numberOfPixels]->setValid(true);
            numberOfPixels++;
          }
        }
      } else {
        anaLog << "Too many Pixels found in Barrel" << endli;
        return 0;
      }
    }
  }
  return ptr;
}

planeShape* ABarrelPixel::getShape(int e, float tdcF, float tdcB)
{
  float zBarrel = ((zParams[actualRange]->getParameter(e, 0) * (tdcB - tdcF) / 2) +
                   zParams[actualRange]->getParameter(e, 1)) *
                  10; // zBarrel*10; from cm to mm!!!

  zBarrel = (coneLength - zBarrel) / coneLength * radius;
  if (zBarrel < radiusIn) {
    if (zBarrel / radiusIn < 0.99) {
      anaLog << "Barrel Pixel position too small: " << radiusIn << ">" << zBarrel << endli;
      return NULL;
    }
    zBarrel = radiusIn;
  }
  if (zBarrel > radius) {
    if (zBarrel / radius > 1.01) {
      anaLog << "Barrel Pixel position too large" << radius << "<" << zBarrel << endli;
      return NULL;
    }
    zBarrel = radius;
  }
  lpe.normalize();
  norm.normalize();
#ifdef USEHPEFORDEF
  planeShape* ret = new sphericRectangle(center, lpe, norm, zBarrel + ps, zBarrel - ps);
#else
  planeShape* ret =
      new sphericRectangle(center + (lpe * (zBarrel - ps)), center + (lpe * (zBarrel + ps)),
                           center + (norm * (zBarrel + ps)), center + (norm * (zBarrel - ps)));
#endif
#ifdef DEBUGBARRELPIXEL
  tree_element = e;
  tree_tdc[0] = tdcF;
  tree_tdc[1] = tdcB;
  tree_r = zBarrel;
  tree_theta = ret->getCenter().Theta();
  tree_phi = ret->getCenter().Phi();
  tree_params[0] = zParams[actualRange]->getParameter(e, 0);
  tree_params[1] = zParams[actualRange]->getParameter(e, 1);
  tree_params[2] = zParams[actualRange]->getParameter(e, 2);
  bpixel_outTree->Fill();
#endif
  return ret;
}

void ABarrelPixel::onNewRun(run_parameter& rp)
{
  anaLog << "ABarrelPixel::onNewRun()" << endli;
  if (zParams != NULL) {
    for (int i = 0; i < numRanges; i++)
      if (zParams[i] != NULL)
        delete zParams[i];
    delete[] zParams;
  }
  if (range != NULL) {
    for (int i = 0; i < numRanges; i++)
      delete[] range[i];
    delete[] range;
  }
  vector<CommonCalibrationParser*> Parser;
  vector<CommonCalibrationParser*> tmp;
  if (rp.hasOwnCalibration() && !rp.hasAdditionalCalibration())
    for (int i = 0; i < rp.getNumberOfCalibrationFiles(); i++) {
      CommonCalibrationParser::getFileContent(rp.getCalibrationFile(i), tmp);
    }
  else {
    if (rp.hasAdditionalCalibration()) {
      for (int i = 0; i < rp.getNumberOfCalibrationFiles(); i++) {
        CommonCalibrationParser::getFileContent(rp.getCalibrationFile(i), tmp);
      }
    }
    if (rp.getParent() != NULL) {
      for (int i = 0; i < rp.getParent()->getNumberOfCalibrationFiles(); i++) {
        CommonCalibrationParser::getFileContent(rp.getParent()->getCalibrationFile(i), tmp);
      }
    }
  }
  while (!tmp.empty()) {
    if (tmp.back()->getDetector() == 6 && tmp.back()->getParamID() == 6) {
      if (tmp.back()->getParamID() == 6)
        Parser.push_back(tmp.back());
    } else {
      CommonCalibrationParser* tm = tmp.back();
      delete tm;
    }
    tmp.pop_back();
  }
  numRanges = Parser.size();
  actualRange = 0;
  int numParams = 4;
  zParams = new CommonCalibrationParser*[numRanges];
  for (int i = 0; i < numRanges; i++)
    zParams[i] = Parser[i];
  range = new int*[numParams];
  for (int i = 0; i < numParams; i++)
    range[i] = new int[numRanges];
  for (int i = 0; i < numParams; i++)
    for (int j = 0; j < numRanges; j++)
      range[i][j] = Parser[j]->getRange(i);

  anaLog << "detID-" << 6 << " PID-" << 6 << " #-ranges:" << numRanges;
  for (int k = 0; k < numRanges; k++)
    anaLog << " from run " << zParams[k]->getRange(0) << " to " << zParams[k]->getRange(1) << endli;
  if (numRanges == 0)
    anaLog << endli;
}
