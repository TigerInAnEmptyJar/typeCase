#include "Calibrations.h"
#include "TF1.h"
#include "TProfile.h"
#include "fiber.h"
#include "logger.h"
#include "reactions.h"
#include "spiral.h"
#include "wedge.h"
#include <TFile.h>
#include <TTree.h>
#include <stdlib.h>

logger calLog("calibrationGeneration.log", "calibration generation log");
#define CALIBRATION_FILLTREE

extern float getLightWay(volumeShape* sh, point3D onPoint, bool from);
AtdcRadialPolCalibration::AtdcRadialPolCalibration(TEvent& eventIn, TSetup& setup,
                                                   algorithm_parameter& descr)
    : CalibrationAlgorithm("Generate tdcRadialPol calibration"), event(eventIn),
      minEntriesPerHisto(descr.value(1).value<int>())
{
  postScriptHistos = descr.value(0).value<bool>();
  vector<int> tmp(descr.value(6).value<vector<int>>());
  vector<int> tmp1(descr.value(7).value<vector<int>>());
  vector<int> tmp3(descr.value(8).value<vector<int>>());
  getReactions(descr.value(9).value<vector<string>>());
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  numberOfStartDetectors = tmp1.size();
  startDetectors = new int[numberOfStartDetectors];
  for (unsigned int i = 0; i < tmp1.size(); i++)
    startDetectors[i] = tmp1[i];
  int xmin = 0, xmax = 1000;
  xmin = descr.value(2).value<int>();
  xmax = descr.value(3).value<int>();
  int ymin = -100, ymax = -100;
  ymin = descr.value(4).value<int>();
  ymax = descr.value(5).value<int>();
  eventStart = 0;
  string nn;
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = tmp[i];
    if (setup.getNumberOfDetectors() > detectorIDs[i]) {
      numberOfElements[i] = setup.getDetectorr(i).getNumberOfElements();
      innerReadOut[i] = (tmp3[i] == 1);
      CinSzinti[i] = 0.7;
      if (setup.getDetectorr(i).getMaterial() != NULL)
        CinSzinti[i] = setup.getDetectorr(i).getMaterial()->getSpeedOfLight();
    } else {
      numberOfElements[i] = 0;
      innerReadOut[i] = false;
      CinSzinti[i] = 0.7;
    }
    cout << detectorIDs[i] << " " << CinSzinti[i] << endl;
  }
  qdcHistograms = new TH1**[numberOfDetectors];
  for (int i = 0; i < numberOfDetectors; i++) {
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("tdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
            string_number(j));
      qdcHistograms[i][j] = new /*TProfile(nn.data(),nn.data(),100,xmin,xmax);/*/ TH2F(
          nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
//   qdcHistograms1=new TH1**[numberOfDetectors];
//   for(int i=0;i<numberOfDetectors;i++)
//     {
//       qdcHistograms1[i]=new TH1*[numberOfElements[i]];
//       for(int j=0;j<numberOfElements[i];j++)
// 	{
// 	  nn=(string("tdc-difference: detID-")+string_number(detectorIDs[i])+"
// el-"+string_number(j));
// 	  qdcHistograms1[i][j]=new
// TH2F(nn.data(),nn.data(),100,xmin,xmax,100,ymin,ymax);
// 	}
//     }
#ifdef CALIBRATION_FILLTREE
  TFile* f = new TFile("calibrationTree.root", "recreate", "", 9);
  f->cd();
  tree = new TTree("CalibrationTree", "CalibrationTree");
  tree->Branch("runNumber", &ints[0], "runNumber/I");
  tree->Branch("eventNumber", &ints[1], "eventNumber/I");
  tree->Branch("trackNumber", &ints[2], "trackNumber/I");
  tree->Branch("reaction", &ints[3], "reaction/I");
  tree->Branch("det", &ints[4], "det/I");
  tree->Branch("el", &ints[5], "el/I");
  tree->Branch("start", &floats[0], "start/F");
  tree->Branch("stopM", &floats[1], "stopM/F");
  tree->Branch("stopC", &floats[2], "stopC/F");
  tree->Branch("betaC", &floats[3], "betaC/F");
  tree->Branch("lightPath", &floats[4], "lightPath/F");
  tree->Branch("flight", &floats[5], "flight/F");
  tree->Branch("theta", &floats[6], "theta/F");
  tree->Branch("C_sz", &floats[7], "C_sz/F");
  tree->Branch("mass", &floats[8], "mass/F");
  tree->Branch("qdc", &floats[9], "qdc/F");
#endif
}

AtdcRadialPolCalibration::AtdcRadialPolCalibration(TEvent& eventIn, TSetup& setup,
                                                   vector<int>& detectors, vector<int>& Stdetectors,
                                                   vector<int> readSide,
                                                   vector<string> reactionNames,
                                                   int minEntriesPerHistoIn)
    : CalibrationAlgorithm("Generate tdcRadialPix calibration"), event(eventIn),
      minEntriesPerHisto(minEntriesPerHistoIn)
{
  vector<int> tmp(detectors);
  getReactions(reactionNames);
  numberOfDetectors = tmp.size();
  detectorIDs = new int[numberOfDetectors];
  numberOfElements = new int[numberOfDetectors];
  qdcHistograms = new TH1**[numberOfDetectors];
  numberOfStartDetectors = Stdetectors.size();
  startDetectors = new int[numberOfStartDetectors];
  innerReadOut = new bool[numberOfDetectors];
  CinSzinti = new float[numberOfDetectors];
  for (unsigned int i = 0; i < Stdetectors.size(); i++)
    startDetectors[i] = Stdetectors[i];
  string nn;
  int xmin = 0, xmax = 1000;
  int ymin = -100, ymax = 100;
  eventStart = 0;
  for (int i = 0; i < numberOfDetectors; i++) {
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
    qdcHistograms[i] = new TH1*[numberOfElements[i]];
    for (int j = 0; j < numberOfElements[i]; j++) {
      nn = (string("tdc-difference: detID-") + string_number(detectorIDs[i]) + " el-" +
            string_number(j))
               .data();
      qdcHistograms[i][j] = new TH2F(nn.data(), nn.data(), 100, xmin, xmax, 100, ymin, ymax);
    }
  }
#ifdef CALIBRATION_FILLTREE
  TFile* f = new TFile("calibrationTree.root", "recreate", "", 9);
  f->cd();
  tree = new TTree("CalibrationTree", "CalibrationTree");
  tree->Branch("runNumber", &ints[0], "runNumber/I");
  tree->Branch("eventNumber", &ints[1], "eventNumber/I");
  tree->Branch("trackNumber", &ints[2], "trackNumber/I");
  tree->Branch("reaction", &ints[3], "reaction/I");
  tree->Branch("det", &ints[4], "det/I");
  tree->Branch("el", &ints[5], "el/I");
  tree->Branch("start", &floats[0], "start/F");
  tree->Branch("stopM", &floats[1], "stopM/F");
  tree->Branch("stopC", &floats[2], "stopC/F");
  tree->Branch("betaC", &floats[3], "betaC/F");
  tree->Branch("lightPath", &floats[4], "lightPath/F");
  tree->Branch("flight", &floats[5], "flight/F");
  tree->Branch("theta", &floats[6], "theta/F");
  tree->Branch("C_sz", &floats[7], "C_sz/F");
  tree->Branch("mass", &floats[8], "mass/F");
  tree->Branch("qdc", &floats[9], "qdc/F");
#endif
}
AtdcRadialPolCalibration::~AtdcRadialPolCalibration()
{
#ifdef CALIBRATION_FILLTREE
  long ne = tree->GetEntries();
  TFile* f = tree->GetCurrentFile();
  f->cd();
  tree->Write();
  f->Close();
  cout << "calibration TDC_radial_pol written: " << ne << " entries" << endl;
  delete f;
// delete tree;
#endif
  for (int i = 0; i < numberOfDetectors; i++) {
    for (int j = 0; j < numberOfElements[i]; j++)
      delete qdcHistograms[i][j];
    delete[] qdcHistograms[i];
  }
  delete[] qdcHistograms;
  for (int i = 0; i < numberOfDetectors; i++) {
    //      for(int j=0;j<numberOfElements[i];j++)
    //	delete qdcHistograms[i][j];
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
  delete[] startDetectors;
}
void AtdcRadialPolCalibration::process()
{
  if (event.getNumberOfReactions() == 0)
    return;
  // cout<<"AtdcRadialPolCalibration::process(void *ptr)
  // "<<(event.getReaction()->name()=="Rppelastic"?"1a":(event.getReaction()->name()=="Rpp_dpiPlus"?"2a":"no"))<<endl;
  // check for reactions:
  int found = -1;
  for (int i = 0; i < numberOfReactions; i++)
    if (reactions[i]->name() == event.getReaction(0)->name())
      found = i;
  // cout<<found<<endl;
  if (found < 0)
    return;
  RbaseReaction* react = event.getReaction(0);
  if (react == NULL)
    return;
  // calLog<<"reaction:"<<found<<endli;
  int num;
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
  int nh;
#ifdef CALIBRATION_FILLTREE
  ints[0] = event.getRunNumber();
  ints[1] = event.getEventNumber();
  ints[3] = found;
#endif
  // cout<<1<<endl;
  for (int i = 0; i < react->getNumberOfTracks(); i++) {
// cout<<i<<endl;
// cout<<"track"<<i<<"("<<react->getTrack(i)<<")"<<endl;
#ifdef CALIBRATION_FILLTREE
    ints[2] = i;
#endif
    if (react->name() == "Rpp_dpiPlus" && i == 0)
      continue;
    track = react->getTrack(i);
    if (track == NULL)
      continue;
    // cout<<track->getNumberOfCalibHits();
    particle = react->getParticle(i);
    startTime = 0;
    startPos.setValues(0, 0, 0);
    nh = 0;
    //      for(int j=0;j<25;j++)cout<<track->getNumberOfCalibHits(j)<<"
    //      "<<flush;
    //      cout<<endl;
    for (int j = 0; j < numberOfStartDetectors; j++) {
      //	  cout<<startDetectors[j]<<"
      //"<<track->getNumberOfCalibHits(startDetectors[j])<<flush;
      if (track->getNumberOfCalibHits(startDetectors[j]) > 0) {
        tmp = track->getCalibSrcr(startDetectors[j], 0);
        if (tmp == NULL)
          continue;
        startTime += tmp->getTDC();
        startPos = startPos + (tmp->getHitShape()->entrance(track->getPath()) - origin);
        //	      cout<<tmp->getRawTDC()<<endl;
        nh++;
      }
    }
    //      cout<<nh<<" "<<startTime<<endl;;
    if (nh == 0)
      continue;
    startTime = startTime / nh;
    startPos = origin + (vector3D(startPos) * (1 / nh));
// ut<<"nDet:"<<numberOfDetectors<<endl ;
#ifdef CALIBRATION_FILLTREE
    floats[0] = startTime;
    floats[6] = track->getPath().Direction().Theta();
    floats[8] = react->getParticle(i).Mass();
#endif
    for (int j = 0; j < numberOfDetectors; j++) {
      // cout<<"\""<<j<<" "<<track->getNumberOfCalibHits(detectorIDs[j]);
      if (track->getNumberOfCalibHits(detectorIDs[j]) <= 0)
        continue;
      tmp = track->getCalibSrcr(detectorIDs[j], 0);
      if (tmp == NULL)
        continue;
      stopPos = tmp->getHitShape()->entrance(track->getPath());
      lightPath = getLightWay(tmp->getHitShape(), stopPos, innerReadOut[j]);
      // cout<<" "<<lightPath<<stopPos<<endl;;
      if (lightPath < 0)
        continue;
      // cout<<" "<<j<<" "<<lightPath<<" "<<CinSzinti[j]<<" "<<endl;
      flightPath = (stopPos - startPos).R();
      tdcCalculated = flightPath / particle.Beta() / 299.792;
      tdcCalculated += lightPath / CinSzinti[j] / 299.792;
      tdcCalculated += (startTime);
      num = tmp->getElement();
#ifdef CALIBRATION_FILLTREE
      ints[4] = detectorIDs[j];
      ints[5] = num;
      floats[1] = tmp->getTDC();
      floats[2] = tdcCalculated;
      floats[3] = particle.Beta();
      floats[4] = lightPath;
      floats[5] = flightPath;
      floats[7] = CinSzinti[j];
      floats[9] = tmp->getADC();
      tree->Fill();
#endif
      // cout<<"fill";
      // qdcHistograms[j][num]->Fill(lightPath,tdcCalculated-tmp->getTDC());
    }
  }
  //  cout<<"next"<<endl;
  //  cout<<event.getRunNumber()<<"-"<<event.getEventNumber()<<":
  //  "<<tree->GetEntries()<<endl;
}
void AtdcRadialPolCalibration::toEvaluate()
{
  if (abs(eventStart - event.getEventNumber()) < 10)
    return;
  //  for (int i = 0; i < numberOfDetectors; i++)
  //    emit evaluated(evaluate(i));
  eventStart = event.getEventNumber();
}
#include <TCanvas.h>
#include <TStyle.h>
CommonCalibrationParser* AtdcRadialPolCalibration::evaluate(int num)
{
  gStyle->SetOptFit(1111111);
  CommonCalibrationParser* ret =
      new CommonCalibrationParser(numberOfElements[num], detectorIDs[num]);
  ret->setNumParamsElements(numberOfElements[num], 6);
  ret->setParamID(8);
  ret->setValid();
  ret->setRange(0, event.getRunNumber());
  ret->setRange(1, event.getRunNumber());
  ret->setRange(2, eventStart);
  ret->setRange(3, event.getEventNumber());
  float v[6] = {0, 0, 0, 0, 0, 0};
  TF1 function("function", "[0]+[1]*x"); //+[2]*x*x");//+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x");
  function.SetLineColor(3);
  function.SetParLimits(1, 0, 10000);
  function.SetParameter(0, 25);
  function.SetParameter(1, 0.02);
  TF1* f1;
  TProfile** prof = new TProfile*[numberOfElements[num]];
  //   TH1F** hists=new TH1F*[numberOfElements[num]];
  //   string s;
  for (int i = 0; i < numberOfElements[num]; i++) {
    prof[i] =
        /*(TProfile*)qdcHistograms[num][i];/*/ ((TH2*)qdcHistograms[num][i])->ProfileX();
    //       s=(string("tmpH-")+string_number(num)+"-"+string_number(i));
    //       hists[i]=new
    //       TH1F(s.data(),s.data(),qdcHistograms[num][i]->GetNbinsX(),qdcHistograms[num][i]->GetXaxis()->GetXmin(),qdcHistograms[num][i]->GetXaxis()->GetXmax());
    //       for(int j=1;j<qdcHistograms[num][i]->GetNbinsX()+1;j++)
    // 	{
    // 	  int maxBin=1;
    // 	  for(int k=2;k<qdcHistograms[num][i]->GetNbinsY()+1;k++)
    // 	    if(qdcHistograms[num][i]->GetBinContent(j,k)>qdcHistograms[num][i]->GetBinContent(j,maxBin))
    // 	      maxBin=k;
    // 	  if(maxBin!=1)
    // 	    hists[i]->Fill(qdcHistograms[num][i]->GetBinCenter(qdcHistograms[num][i]->GetBin(j,maxBin)),qdcHistograms[num][i]->GetBinContent(j,maxBin));
    // 	}
    //       hists[i]->SetMarkerColor(6);
    //       hists[i]->SetLineColor(6);
    //       function.SetLineColor(7);
    //       hists[i]->Fit(&function,"Q");
    //       function.SetLineColor(3);
    prof[i]->Fit(&function, "Q");
    prof[i]->SetLineColor(3);
    f1 = prof[i] /*/hists[i]*/->GetFunction("function");
    for (int j = 0; j < 6; j++)
      v[j] = f1->GetParameter(j);
    for (int j = 0; j < 6; j++)
      ret->setParameter(i, j, v[j]);
  }
  if (postScriptHistos) {
    TCanvas* canv = new TCanvas();
    canv->Divide(4, 3);
    string fn = string("histosDet-") + string_number(detectorIDs[num]) + ".ps";
    canv->Print((fn + "[").data());
    // TPostScript *ps=new
    // TPostScript((string("histosDet-")+string_number(detectorIDs[num])+".ps").data(),112);
    int canvasCount = 0;
    for (int i = 0; i < numberOfElements[num]; i++) {
      if (canvasCount == 12) {
        canv->Print(fn.data()); // Update();
        //		ps->NewPage();
        canvasCount = 0;
      }
      canvasCount++;
      canv->cd(canvasCount);
      prof[i]->Draw();
      f1 = prof[i]->GetFunction("function");
      //	  f1->Draw("same");
      prof[i]->Draw("same");
      // hists[i]->Draw();//"same");
      qdcHistograms[num][i]->Draw("same");
    }
    canv->Print((fn + ")").data()); // Update();
    //       fn=fn+"]";
    //       canv->Print(fn.data());//Update();
    //      ps->NewPage();
    //       ps->Close();
    //       delete ps;
    delete canv;
  }
  TFile f("output.root", "recreate");
  f.mkdir((string("tmp") + string_number(num)).data())->cd();
  for (int i = 0; i < numberOfElements[num]; i++) {
    qdcHistograms[num][i]->Write();
    qdcHistograms[num][i]->SetDirectory(0);
    qdcHistograms[num][i]->Reset();
    prof[i]->Write();
    prof[i]->SetDirectory(0);
    // delete hists[i];
    delete prof[i];
  }
  f.Close();
  //  delete []hists;
  delete[] prof;
  return ret;
}
void AtdcRadialPolCalibration::getReactions(const vector<string>& names)
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
// float getLightWay(volumeShape *sh, point3D onPoint, bool from)
// {
//   if(sh->getName()=="wedge")
//     {
//       float r=(onPoint-((wedge*)sh)->getCenter()).R();
//       if(from)return r-((wedge*)sh)->getInnerRadius();
//       else return ((wedge*)sh)->getOuterRadius()-r;
//     }
//   else if(sh->getName()=="spiral")
//     {
//       float r=(onPoint-((spiral*)sh)->getCenter()).R();
//       if(from)return r-((spiral*)sh)->getInnerRadius();
//       else return ((spiral*)sh)->getOuterRadius()-r;
//     }
//   else if(sh->getName()=="fiber")
//     {
//       point3D p2;
//       if(from)p2=((fiber*)sh)->getCorner();
//       else p2=((fiber*)sh)->getCorner()+((fiber*)sh)->getDirection(0);
//       return (onPoint-p2).R();
//     }
//   return -1;
// }
