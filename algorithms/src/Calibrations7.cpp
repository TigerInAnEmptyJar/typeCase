#include "Calibrations.h"
#include "reactions.h"
#include <stdlib.h>
float getLightWay(volumeShape* sh, point3D onPoint, bool from);
int getPixelPoint(TPixel* pix);
void redefinePixels(TTrack* t);
TPixel tmpPixel_7;
AtdcRadialPixCalibration::AtdcRadialPixCalibration(TEvent& eventIn, TSetup& setup,
                                                   algorithm_parameter& descr)
    : CalibrationAlgorithm("Generate tdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(descr.value(1).value<int>())
{
  eventStart = 0;
  vector<int> tmp(descr.value(4).value<vector<int>>());
  vector<int> tmp4(descr.value(8).value<vector<int>>());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++)
    detectorIDs[i] = tmp[i];
  ///////////////////////////////
  vector<int> tmp1(descr.value(5).value<vector<int>>());
  vector<int> tmp2(descr.value(6).value<vector<int>>());
  vector<int> tmp3(descr.value(7).value<vector<int>>());
  CinSzinti = new float[numberOfDetectors];
  innerReadOut = new bool[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++) {
    innerReadOut[i] = (tmp3[i] == 1);
    if (setup.getNumberOfDetectors() > detectorIDs[i]) {
      CinSzinti[i] = 0.7;
      if (setup.getDetectorr(detectorIDs[i]).getMaterial() != NULL)
        CinSzinti[i] = setup.getDetectorr(detectorIDs[i]).getMaterial()->getSpeedOfLight();
    } else
      CinSzinti[i] = 0.7;
  }
  ////////////////////////////////
  numberOfStartDetectors = tmp1.size();
  startDetectors = new int[numberOfStartDetectors];
  for (unsigned int i = 0; i < tmp1.size(); i++)
    startDetectors[i] = tmp1[i];
  numberOfPixels = tmp2.size();
  pixelIDs = new int[numberOfPixels];
  for (int i = 0; i < numberOfPixels; i++)
    pixelIDs[i] = tmp2[i];
  postScriptHistos = descr.value(0).value<bool>();
  getReactions(descr.value(9).value<vector<string>>());
  //////////////////////////////////
  numberOfElements = new int[numberOfDetectors];
  pixelpoints = new int[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++) {
    pixelpoints[i] = tmp4[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i])
      numberOfElements[i] = setup.getDetectorr(detectorIDs[i]).getNumberOfElements();
    else
      numberOfElements[i] = 0;
  }
  int xmin = -100, xmax = -100;
  xmin = descr.value(2).value<int>();
  xmax = descr.value(3).value<int>();
  string nn1 = "\\Delta tdc: detID-";
  qdcHistograms = new TH1***[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++) {
    qdcHistograms[i] = new TH1**[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      qdcHistograms[i][j] = new TH1*[pixelpoints[i]];
      for (int k = 0; k < pixelpoints[i]; k++) {
        string nn = nn1 + string_number(detectorIDs[i]) + " el-" + string_number(j) + " pix-" +
                    string_number(k);
        qdcHistograms[i][j][k] = new TH1F(nn.data(), nn.data(), 100, xmin, xmax);
      }
    }
  }
  nn1 = "\\Delta tdc: detID-";
  qdcHistograms1 = new TH1***[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++) {
    qdcHistograms1[i] = new TH1**[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      qdcHistograms1[i][j] = new TH1*[pixelpoints[i]];
      for (int k = 0; k < pixelpoints[i]; k++) {
        string nn = nn1 + string_number(detectorIDs[i]) + " el-" + string_number(j) + " pix-" +
                    string_number(k);
        qdcHistograms1[i][j][k] = new TH1F(nn.data(), nn.data(), 100, xmin, xmax);
        // 	      delete qdcHistograms[i][j][k];
        // 	      qdcHistograms[i][j][k]=new
        // TH1F(nn.data(),nn.data(),100,xmin,xmax);
      }
    }
  }
  //  cout<<qdcHistograms[0][0][0]<<"
  //  "<<qdcHistograms[0][0][0]->GetName()<<endl;
}
AtdcRadialPixCalibration::AtdcRadialPixCalibration(TEvent& eventIn, TSetup& setup,
                                                   vector<int>& detectors, vector<int>& Stdetectors,
                                                   vector<int> pixIDs, vector<int> readSide,
                                                   vector<int> pointsPerPixel,
                                                   vector<string> reactionNames,
                                                   int minEntriesPerHistoIn)
    : CalibrationAlgorithm("Generate tdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  postScriptHistos = false;
  vector<int> tmp(detectors);
  getReactions(reactionNames);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1***[numberOfDetectors];
  numberOfStartDetectors = Stdetectors.size();
  startDetectors = new int[numberOfStartDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  for (unsigned int i = 0; i < Stdetectors.size(); i++)
    startDetectors[i] = Stdetectors[i];
  numberOfPixels = pixIDs.size();
  pixelIDs = new int[numberOfPixels];
  pixelpoints = new int[numberOfPixels];
  for (int i = 0; i < numberOfPixels; i++) {
    pixelIDs[i] = pixIDs[i];
  }
  string nn;
  int xmin = 0, xmax = 10000;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
    pixelpoints[i] = pointsPerPixel[i];
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i]) {
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
      CinSzinti[i] = 0.7;
      innerReadOut[i] = (readSide[i] == 1);
      if (setup.getDetectorr(i).getMaterial() != NULL)
        CinSzinti[i] = setup.getDetectorr(i).getMaterial()->getSpeedOfLight();
    } else {
      numberOfElements[i] = 0;
      innerReadOut[i] = false;
      CinSzinti[i] = 0.7;
    }
    qdcHistograms[i] = new TH1**[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      qdcHistograms[i][j] = new TH1*[pixelpoints[i]];
      for (int k = 0; k < pixelpoints[i]; k++) {
        nn = (string("\\Delta tdc: detID-") + string_number(detectorIDs[i]) + " el-" +
              string_number(j) + " pix-" + string_number(k));
        qdcHistograms[i][j][k] = new TH1F(nn.data(), nn.data(), 100, xmin, xmax);
      }
    }
  }
}
AtdcRadialPixCalibration::~AtdcRadialPixCalibration()
{
  //  cout<<"~AtdcRadialPixCalibration()"<<endl;
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++) {
      for (int k = 0; k < pixelpoints[i]; k++)
        delete qdcHistograms[i][j][k];
      delete[] qdcHistograms[i][j];
    }
    delete[] qdcHistograms[i];
  }
  delete[] qdcHistograms;
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++) {
      //	  for(int k=0;k<pixelpoints[i];k++)
      //	    delete qdcHistograms1[i][j][k];
      delete[] qdcHistograms1[i][j];
    }
    delete[] qdcHistograms1[i];
  }
  delete[] qdcHistograms1;
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i] != NULL)
      delete reactions[i];
  delete[] reactions;
  delete[] detectorIDs;
  delete[] innerReadOut;
  delete[] CinSzinti;
  delete[] numberOfElements;
  delete[] pixelIDs;
  delete[] startDetectors;
  delete[] pixelpoints;
}
void AtdcRadialPixCalibration::process()
{
  //  cout<<qdcHistograms[0][0][0]<<"
  //  "<<flush;cout<<qdcHistograms[0][0][0]->GetName()<<endl;
  if (event.getNumberOfReactions() == 0)
    return;
  // check for reactions:
  int found = -1;
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i]->name() == event.getReaction(0)->name())
      found = i;
  if (found < 0)
    return;
  RbaseReaction* react = event.getReaction(0);
  int pos, num;
  TCalibHit* tmp;
  float flightPath;
  float lightPath;
  float tdcCalculated;
  float startTime;
  point3D startPos;
  point3D stopPos;
  point3D origin(0, 0, 0);
  TTrack* track;
  momentum4D particle;
  TPixel* pix;
  int nh;
  int pixpos;
  //  int nsp=0;
  for (int i = 0; i < react->getNumberOfTracks(); i++) {
    track = react->getTrack(i);
    particle = react->getParticle(i);
    startTime = 0;
    startPos.setValues(0, 0, 0);
    nh = 0;
    for (int j = 0; j < numberOfStartDetectors; j++) {
      if (track->getNumberOfCalibHits(startDetectors[j]) > 0) {
        tmp = track->getCalibSrcr(startDetectors[j], 0);
        startTime += tmp->getTDC();
        startPos = startPos + (tmp->getHitShape()->entrance(track->getPath()) - origin);
        nh++;
      }
    }
    if (nh == 0)
      continue;
    startTime = startTime / nh;
    startPos = origin + (vector3D(startPos) * (1 / nh));
    for (int j = 0; j < numberOfPixels; j++) {
      if (track->getNumberOfPixels() == 0)
        redefinePixels(track);
      for (int k = 0; k < track->getNumberOfPixels(); k++) {
        pix = &track->getPixel(k, 0);
        if (pix->getID() != pixelIDs[j])
          continue;
        pixpos = getPixelPoint(pix);
        if (pixpos == -1)
          continue;
        for (int m = 0; m < pix->getNumberOfActualElements(); m++) {
          pos = -1;
          tmp = &pix->getElementr(m);
          for (int l = 0; l < numberOfDetectors; l++)
            if (tmp->getDetector() == detectorIDs[l])
              pos = l;
          if (pos < 0)
            continue;
          stopPos = tmp->getHitShape()->entrance(track->getPath());
          lightPath = getLightWay(tmp->getHitShape(), stopPos, innerReadOut[pos]);
          if (lightPath < 0)
            continue;
          flightPath = (stopPos - startPos).R();
          tdcCalculated = flightPath / particle.Beta() / 299.792;
          tdcCalculated += lightPath / CinSzinti[pos] / 299.792;
          tdcCalculated += startTime;
          num = tmp->getElement();
          // 		  cout<<pos<<" "<<num<<"/"<<numberOfElements[pos]<<"
          // "<<pixpos<<"/"<<pixelpoints[pos]<<"
          // "<<tdcCalculated-tmp->getTDC()<<endl;
          // 		  cout<<qdcHistograms[0][0][0]<<"
          // "<<qdcHistograms[pos][num][pixpos]<<endl;
          // 		  cout<<qdcHistograms[0][0][0]->GetEntries()<<endl;
          qdcHistograms[pos][num][pixpos]->Fill(tdcCalculated - tmp->getTDC());
          //		  cout<<qdcHistograms[pos][num][pixpos]->GetEntries()<<endl;
        }
      }
    }
  }
}
void AtdcRadialPixCalibration::toEvaluate()
{
  if (abs(eventStart - event.getEventNumber()) < 10)
    return;
  //  CommonCalibrationParser* tmp;
  //  for (int i = numberOfDetectors - 1; i >= 0; i--) {
  //    //      cout<<"evaluate(int num="<<i<<")="<<flush;;
  //    tmp = evaluate(i);
  //    //      cout<<tmp<<endl;
  ////    emit evaluated(tmp);
  //  }
  eventStart = event.getEventNumber();
}
void AtdcRadialPixCalibration::getReactions(const vector<string>& names)
{
  int num = 0;
  for (unsigned int i = 0; i < names.size(); i++) {
    if (names[i].find("ppelastic") < names[i].npos)
      num++;
    else if (names[i].find("dpi+") < names[i].npos)
      num++;
  }
  numberOfReactions = num;
  reactions = new RbaseReaction*[numberOfReactions];
  for (int i = 0; i < num; i++)
    reactions[i] = NULL;
  num = 0;
  for (unsigned int i = 0; i < names.size(); i++) {
    if (names[i].find("ppelastic") < names[i].npos) {
      reactions[num] = new Rppelastic();
      num++;
    } else if (names[i].find("dpi+") < names[i].npos) {
      reactions[num] = new Rpp_dpiPlus();
      num++;
    }
  }
}
#include <TCanvas.h>
#include <TPostScript.h>
CommonCalibrationParser* AtdcRadialPixCalibration::evaluate(int num)
{
  //  cout<<"AtdcRadialPixCalibration::evaluate(int num="<<num<<")"<<endl;
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(numberOfElements[num], pixelpoints[num]);
  ret->setParamID(7);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[pixelpoints[num]];
  TCanvas* canv = NULL;
  if (postScriptHistos) {
    TCanvas* canv = new TCanvas();
    canv->Divide(4, 3);
    string fn = string("histosDet-") + string_number(detectorIDs[num]) + ".ps";
    canv->Print((fn + "[").data());
    // TPostScript *ps=new
    // TPostScript((string("histosDet-")+string_number(detectorIDs[num])+".ps").data(),112);
    int canvasCount = 0;
    for (int i = 0; i < numberOfElements[num]; i++)
      for (int j = 0; j < pixelpoints[num]; j++) {
        if (canvasCount == 12) {
          canv->Print(fn.data()); // Update();
          //		ps->NewPage();
          canvasCount = 0;
        }
        canvasCount++;
        canv->cd(canvasCount);
        qdcHistograms[num][i][j]->Draw();
      }
    canv->Print(fn.data()); // Update();
    //       fn=fn+"]";
    //       canv->Print(fn.data());//Update();
    //      ps->NewPage();
    //       ps->Close();
    //       delete ps;
  }
  for (int i = 0; i < numberOfElements[num]; i++) {
    for (int j = 0; j < pixelpoints[num]; j++)
      v[j] = qdcHistograms[num][i][j]->GetMean();
    // insert calibration routine
    /*********************************

    **********************************/
    for (int j = 0; j < pixelpoints[num]; j++)
      ret->setParameter(i, j, v[j]);
  }
  for (int i = 0; i < numberOfElements[num]; i++)
    for (int j = 0; j < pixelpoints[num]; j++)
      qdcHistograms[num][i][j]->Reset();
  if (postScriptHistos) {
    //      canv->Print((string("histosDet-")+string_number(detectorIDs[num])+".ps]").data());//Update();
    delete canv;
  }
  return ret;
}
#include "fiber.h"
#include "spiral.h"
#include "wedge.h"
int getPixelPoint(TPixel* pix)
{
  if (pix->getNumberOfActualElements() == 3) {
    int r = -1, l = -1;
    int nsp = 0;
    for (int i = 0; i < pix->getNumberOfActualElements(); i++) {
      if (pix->getElementr(i).getHitShape()->getName() == "spiral") {
        nsp = (int)(2 * M_PI / ((spiral*)pix->getElementr(i).getHitShape())->getPhiRange() + 1);
        if (((spiral*)pix->getElementr(i).getHitShape())->getBending() < 0)
          r = pix->getElementr(i).getElement();
        else
          l = pix->getElementr(i).getElement();
      }
    }
    if (l < r)
      l += nsp;
    return l - r;
  }
  return -1000;
}
void redefinePixels(TTrack* t)
{
  tmpPixel_7.reset();
  if (t->getNumberOfCalibHits(0) > 0 && t->getNumberOfCalibHits(1) > 0 &&
      t->getNumberOfCalibHits(2) > 0) {
    tmpPixel_7.addElement(*t->getCalibSrcr(0, 0));
    tmpPixel_7.addElement(*t->getCalibSrcr(1, 0));
    tmpPixel_7.addElement(*t->getCalibSrcr(2, 0));
    tmpPixel_7.setID(0);
    t->addPixel(&tmpPixel_7);
  } else if (t->getNumberOfCalibHits(3) > 0 && t->getNumberOfCalibHits(4) > 0 &&
             t->getNumberOfCalibHits(5) > 0) {
    tmpPixel_7.addElement(*t->getCalibSrcr(3, 0));
    tmpPixel_7.addElement(*t->getCalibSrcr(4, 0));
    tmpPixel_7.setID(1);
    tmpPixel_7.addElement(*t->getCalibSrcr(5, 0));
    t->addPixel(&tmpPixel_7);
  }
}
