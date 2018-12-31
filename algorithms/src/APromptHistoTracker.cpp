#include "APromptHistoTracker.h"
#include "logger.h"
#include "ring.h"
#include "wedge.h"
#include <TFile.h>
APromptHistoTracker::APromptHistoTracker(TSetup& setup, TTrack** tracksIn, int& numTracks,
                                         int maxTracksIn, TCalibHit*** hitsIn, int** numHitsIn,
                                         bool flat, int minElementsIn, bool outOfSetup,
                                         float percentage, bool function, TFormula* f,
                                         AFitAlgorithm* fitIn, int binsP, int binsT, float minPIn,
                                         float minTIn, float maxPIn, float maxTIn,
                                         bool readFromFile, bool writeToFile, string filename,
                                         float maxChiIn)
    : AAlgorithm("Prompt Tracking using histogramms"), numDets(setup.getNumberOfDetectors()),
      numberOfTracks(numTracks), maxTracks(maxTracksIn), maxChi(maxChiIn), vertex(0, 0, 0)
{
  minT = minTIn;
  maxT = maxTIn;
  minP = minPIn;
  maxP = maxPIn;
  delete f;
  tracks = tracksIn;
  numHits = numHitsIn;
  hits = hitsIn;
  fit = fitIn;
  nel = new int[numDets];
  TH2C*** Elements = new TH2C**[numDets];
  TFile* fil = NULL;
  volumeShape* shape;
  if (readFromFile || writeToFile) {
    if (readFromFile && writeToFile)
      fil = new TFile(filename.data(), "UPDATE");
    else if (readFromFile)
      fil = new TFile(filename.data(), "READ");
    else
      fil = new TFile(filename.data(), "RECREATE");
    fil->cd();
    if (!fil->IsOpen())
      readFromFile = false;
  }
  anaLog << "prompt histo tracker:" << endli;
  vector3D** dirs = new vector3D*[binsT];
  for (int i = 0; i < binsT; i++)
    dirs[i] = new vector3D[binsP];
  for (int k = 0; k < binsT; k++)
    for (int l = 0; l < binsP; l++)
      dirs[k][l].setValues(
          sin(minT + k * (maxT - minT) / (binsT - 1)) * cos(minP + l * (maxP - minP) / (binsP - 1)),
          sin(minT + k * (maxT - minT) / (binsT - 1)) * sin(minP + l * (maxP - minP) / (binsP - 1)),
          cos(minT + k * (maxT - minT) / (binsT - 1)));
  vector3D d;
  float grH[4] = {0, 0, 0, 0}, grL[4] = {0, 0, 0, 0};
  float deltaTheta = dirs[1][0].Theta() - dirs[0][0].Theta();
  for (int i = 0; i < numDets; i++) {
    nel[i] = setup.getDetectorr(i).getNumberOfElements();
    Elements[i] = new TH2C*[nel[i]];
    anaLog << "define elements det " << i << endli;
    for (int j = 0; j < nel[i]; j++) {
      if (readFromFile) {
        Elements[i][j] =
            (TH2C*)fil->Get((string("Det") + string_number(i) + "el" + string_number(j)).data());
        Elements[i][j]->SetDirectory(0);
      } else {
        Elements[i][j] =
            new TH2C((string("Det") + string_number(i) + "el" + string_number(j)).data(),
                     (string("Det") + string_number(i) + "el" + string_number(j)).data(), binsT,
                     minT, maxT, binsP, minP, maxP);
        shape = setup.getDetectorr(i).getShape(j);
        // 	      if(shape->getName()!="wedge")continue;
        // 	      cout<<shape->getName().data()<<"
        // "<<((wedge*)shape)->getLowerPhiEdge()<<"
        // "<<((wedge*)shape)->getPhiRange()<<endl;
        if (shape->getName() != "wedge" && shape->getName() != "ring") {
          for (int k = 0; k < binsT; k++)
            for (int l = 0; l < binsP; l++) {
              sLine3D path(vertex, dirs[k][l]);
              //		    vector3D(sin(minT+k*(maxT-minT)/(binsT-1))*cos(minP+l*(maxP-minP)/(binsP-1)),
              //			     sin(minT+k*(maxT-minT)/(binsT-1))*sin(minP+l*(maxP-minP)/(binsP-1)),
              //			     cos(minT+k*(maxT-minT)/(binsT-1))));
              d = shape->distance(path);
              //		    cout<<k<<" "<<l<<" "<<d<<endl;;
              if (d.R() != 0)
                Elements[i][j]->SetBinContent(k + 1, l + 1, 0);
              else
                Elements[i][j]->SetBinContent(k + 1, l + 1, 1);
            }
        } else {
          if (shape->getName() == "wedge") {
            point3D p1 = ((wedge*)shape)->getInnerRadius() * ((wedge*)shape)->getLowerPhiEdge() +
                         shape->getCenter();
            point3D p2 = ((wedge*)shape)->getOuterRadius() * ((wedge*)shape)->getLowerPhiEdge() +
                         shape->getCenter();
            point3D p3 =
                ((wedge*)shape)->getInnerRadius() *
                    (matrix3D(((wedge*)shape)->getNormal(), ((wedge*)shape)->getPhiRange()) *
                     ((wedge*)shape)->getLowerPhiEdge()) +
                shape->getCenter();
            point3D p4 =
                ((wedge*)shape)->getOuterRadius() *
                    (matrix3D(((wedge*)shape)->getNormal(), ((wedge*)shape)->getPhiRange()) *
                     ((wedge*)shape)->getLowerPhiEdge()) +
                shape->getCenter();
            float minTh = p1.Theta();
            float maxTh = p2.Theta();
            float minPhi = p2.Phi();
            float maxPhi = p4.Phi();
            // // 		      if(minPhi>maxPhi)
            // // 			{
            // // 			  float tmp=minPhi;
            // // 			  minPhi=maxPhi;
            // // 			  maxPhi=tmp;
            // // 			}
            //		      cout<<minPhi<<" "<<maxPhi<<" "<<minTh<<"
            //"<<maxTh<<endl;
            for (int k = 0; k < binsT; k++)
              for (int l = 0; l < binsP; l++)
                if (minTh <= dirs[k][l].Theta() && maxTh >= dirs[k][l].Theta()) {
                  if (maxPhi > M_PI)
                    maxPhi = maxPhi - 2 * M_PI;
                  if ((minPhi <= dirs[k][l].Phi() && maxPhi >= dirs[k][l].Phi()) ||
                      (maxPhi < minPhi &&
                       (dirs[k][l].Phi() >= minPhi || dirs[k][l].Phi() <= maxPhi)))
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 1);
                  else
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 0);
                } else
                  Elements[i][j]->SetBinContent(k + 1, l + 1, 0);
            if (i == 0) {
              grL[0] = minTh;
              grH[0] = maxTh;
            }
            if (i == 3) {
              grL[1] = minTh;
              grH[1] = maxTh;
            }
            if (i == 6) {
              grL[2] = minTh;
              grH[2] = maxTh;
            }
            if (i == 22) {
              grL[3] = minTh;
              grH[3] = maxTh;
            }
          } else if (shape->getName() == "ring") {
            float minTh =
                (((ring*)shape)->getInnerRadius() * vector3D(1, 0, 0) + ((ring*)shape)->getCenter())
                    .Theta();
            float maxTh =
                (((ring*)shape)->getOuterRadius() * vector3D(1, 0, 0) + ((ring*)shape)->getCenter())
                    .Theta();
            if (maxTh - minTh < deltaTheta) {
              for (int k = 0; k < binsT; k++)
                if (abs(dirs[k][0].Theta() - minTh) < deltaTheta &&
                    abs(dirs[k][0].Theta() - maxTh) < deltaTheta)
                  for (int l = 0; l < binsP; l++)
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 1);
                else
                  for (int l = 0; l < binsP; l++)
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 0);

            } else {
              for (int k = 0; k < binsT; k++)
                for (int l = 0; l < binsP; l++)
                  if (minTh <= dirs[k][l].Theta() && maxTh >= dirs[k][l].Theta())
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 1);
                  else
                    Elements[i][j]->SetBinContent(k + 1, l + 1, 0);
            }
          }
        }
      }
      if (writeToFile) {
        Elements[i][j]->Write();
        Elements[i][j]->SetDirectory(0);
      }
    }
  }
  if (readFromFile) {
    minElements = (TH2C*)fil->Get("minElements");
    minElements->SetDirectory(0);
    for (int i = 0; i < binsT; i++)
      for (int j = 0; j < binsP; j++) {
        if (minElements->GetBinContent(i + 1, j + 1) == 0)
          minElements->SetBinContent(i + 1, j + 1, minElementsIn);
        if (minElements->GetBinContent(i + 1, j + 1) == 9)
          minElements->SetBinContent(i + 1, j + 1, 7);
      }
    binsT = minElements->GetNbinsX();
    binsP = minElements->GetNbinsY();
    minT = minElements->GetXaxis()->GetXmin();
    maxT = minElements->GetXaxis()->GetXmax();
    minP = minElements->GetYaxis()->GetXmin();
    maxP = minElements->GetYaxis()->GetXmax();
  } else {
    minElements = new TH2C("minElements", "minElements", binsT, minT, maxT, binsP, minP, maxP);
    for (int i = 0; i < binsT; i++)
      for (int j = 0; j < binsP; j++) {
        if (grL[0] > dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 100);
        if (grL[0] <= dirs[i][j].Theta() && grL[3] >= dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 5);
        if (grL[0] <= dirs[i][j].Theta() && grL[3] < dirs[i][j].Theta() &&
            grH[0] > dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 7);
        if (grL[1] <= dirs[i][j].Theta() && grH[1] > dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 7);
        if (grL[2] <= dirs[i][j].Theta() && grH[2] > dirs[i][j].Theta() &&
            grH[3] > dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 6);
        if (grL[2] <= dirs[i][j].Theta() && grH[2] > dirs[i][j].Theta() &&
            grH[3] < dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 4);
        if (grH[2] < dirs[i][j].Theta())
          minElements->SetBinContent(i + 1, j + 1, 100);
      }
  }
  if (writeToFile) {
    minElements->Write();
    minElements->SetDirectory(0);
  }
  if (fil != NULL)
    fil->Close();
  for (int i = 0; i < binsT; i++)
    delete[] dirs[i];
  delete[] dirs;
  nbinsT = binsT;
  nbinsP = binsP;
  axisT = new float[nbinsT];
  for (int i = 0; i < nbinsT; i++)
    axisT[i] = minT + ((maxT - minT) / nbinsT * (i + 0.5));
  axisP = new float[nbinsP];
  for (int i = 0; i < nbinsP; i++)
    axisP[i] = minP + ((maxP - minP) / nbinsP * (i + 0.5));
  Element = new char***[numDets];
  for (int i = 0; i < numDets; i++) {
    Element[i] = new char**[nel[i]];
    for (int j = 0; j < nel[i]; j++) {
      Element[i][j] = new char*[nbinsT];
      for (int k = 0; k < nbinsT; k++) {
        Element[i][j][k] = new char[nbinsP];
        for (int l = 0; l < nbinsP; l++)
          Element[i][j][k][l] = (int)Elements[i][j]->GetBinContent(k + 1, l + 1);
      }
    }
  }
  workHisto = new float*[nbinsT];
  for (int i = 0; i < nbinsT; i++)
    workHisto[i] = new float[nbinsP];
  for (int i = 0; i < numDets; i++) {
    for (int j = 0; j < nel[i]; j++)
      delete Elements[i][j];
    delete[] Elements[i];
  }
  delete[] Elements;
  if (flat)
    return;
  if (outOfSetup)
    return;
  if (percentage == 0)
    return;
  if (function)
    return;
}
APromptHistoTracker::~APromptHistoTracker()
{
  for (int i = 0; i < numDets; i++) {
    for (int j = 0; j < nel[i]; j++) {
      for (int k = 0; k < nbinsT; k++)
        delete[] Element[i][j][k];
      delete[] Element[i][j];
    }
    delete[] Element[i];
  }
  delete[] Element;
  delete[] nel;
  delete minElements;
  for (int i = 0; i < nbinsT; i++)
    delete[] workHisto[i];
  delete[] workHisto;
}
void* APromptHistoTracker::process(void* ptr)
{
  //  cout<<"APromptHistoTracker::process()"<<endl;
  for (int i = 0; i < nbinsT; i++)
    for (int j = 0; j < nbinsP; j++)
      workHisto[i][j] = 0;
  int numH = 0;
  for (int i = 0; i < numDets; i++)
    for (int j = 0; j < (*numHits[i]); j++)
      if (hits[i][j]->isValid() && hits[i][j]->getElement() < nel[i])
        numH++;
  //  cout<<numH<<" number of hits found"<<endl;
  int dete[numH];
  int ele[numH];
  int nn = 0;
  int eel;
  for (int i = 0; i < numDets; i++)
    for (int j = 0; j < (*numHits[i]); j++) {
      eel = hits[i][j]->getElement();
      if (hits[i][j]->isValid() && eel < nel[i]) {
        for (int k = 0; k < nbinsT; k++)
          for (int l = 0; l < nbinsP; l++)
            workHisto[k][l] = workHisto[k][l] + Element[i][eel][k][l];
        //	    workHisto->Add(Element[i][hits[i][j]->getElement()]);
        dete[nn] = i;
        ele[nn] = j;
        nn++;
      }
    }
  for (int i = 0; i < nbinsT; i++)
    for (int j = 0; j < nbinsP; j++)
      workHisto[i][j] = workHisto[i][j] / minElements->GetBinContent(i + 1, j + 1);
  //  workHisto->Divide(minElements);
  //  cout<<"added"<<endl;
  //  cout<<"Maximum: "<<workHisto->GetMaximum()<<endl;
  vector3D direction;
  direction.setRep(_spheric_);
  int effx, effy;
  numberOfTracks = 0;
  int loop = 0;
  float max = 0;
  int maxX = 0, maxY = 0;
  for (int i = 0; i < nbinsT; i++)
    for (int j = 0; j < nbinsP; j++)
      if (workHisto[i][j] > max) {
        max = workHisto[i][j];
        maxX = i;
        maxY = j;
      }
  while (max >= 1 && numberOfTracks < maxTracks && loop < 3 * maxTracks) {
    loop++;
    //      cout<<numberOfTracks<<endl;
    direction.setValues(sin(axisT[maxX]) * cos(axisP[maxY]), sin(axisT[maxX]) * sin(axisP[maxY]),
                        cos(axisT[maxX]));
    //      cout<<direction<<maxposx<<" "<<maxposy<<endl;
    fit->clear();
    bool found;
    for (int i = 0; i < numH; i++) {
      eel = hits[dete[i]][ele[i]]->getElement();
      found = false;
      for (int j = -2; j < 3; j++)
        for (int k = -2; k < 3; k++) {
          effx = maxX + j;
          effy = maxY + k;
          if (maxX + j < 0)
            effx = nbinsT + (maxX + j);
          if (maxY + k < 0)
            effy = nbinsP + (maxY + k);
          if (maxX + j >= nbinsT)
            effx = (maxX + j) - nbinsT;
          if (maxY + k >= nbinsP)
            effy = -nbinsP + (maxY + k);
          if (Element[dete[i]][eel][effx][effy] > 0)
            found = true;
        }
      if (found) {
        tracks[numberOfTracks]->addCalibSrc(hits[dete[i]][ele[i]]);
        Vector best(hits[dete[i]][ele[i]]->getHitShape()->HitParams(sLine3D(vertex, direction)));
        Vector tmp(5);
        tmp.setValues((best[4] * best[7] + best[5] * best[8]) / best[0], best[4] / best[0],
                      best[5] / best[0], best[4] * best[9] / best[0], best[5] * best[9] / best[0]);
        fit->add(tmp);
      }
    }
    float chi = fit->fit();
    if (chi < maxChi) {
      Vector res(fit->getResult());
      point3D f(res[0], res[1], 0);
      vector3D d(res[2], res[3], 1);
      tracks[numberOfTracks]->setPath(sLine3D(f, d));
      maxX = minElements->GetXaxis()->FindBin(d.Theta());
      maxY = minElements->GetYaxis()->FindBin(d.Phi());
      numberOfTracks++;
    }
    for (int i = -5; i < 5; i++)
      for (int j = -5; j < 5; j++) {
        effx = maxX + i;
        effy = maxY + j;
        if (maxX + i < 0)
          effx = nbinsT + (maxX + i);
        if (maxY + j < 0)
          effy = nbinsP + (maxY + j);
        if (maxX + i >= nbinsT)
          effx = (maxX + i) - nbinsT;
        if (maxY + j >= nbinsP)
          effy = -nbinsP + (maxY + j);
        workHisto[effx][effy] = 0;
      }
    max = 0;
    maxX = 0, maxY = 0;
    for (int i = 0; i < nbinsT; i++)
      for (int j = 0; j < nbinsP; j++)
        if (workHisto[i][j] > max) {
          max = workHisto[i][j];
          maxX = i;
          maxY = j;
        }
  }
  return ptr;
}

vector<string> APromptHistoTracker::histogramNames()
{
  vector<string> tmp;
  tmp.push_back(getName());
  //  tmp.push_back(workHisto->GetName());
  return tmp;
}
TH1* APromptHistoTracker::histogram(string histoName)
{
  if (histoName == "")
    return NULL;
  //   if(histoName==workHisto->GetName())
  //     return workHisto;
  return NULL;
}
// TH1C*getHistoFromFiber(fiber *f, point3D center, vector3D x, vector3D z)
// {
//   point3D corner=f->getCorner();
//   vector3D dir1=f->getDirection(0);
//   vector3D dir2=f->getDirection(1);
//   vector3D dir3=f->getDirection(2);
//   plane3D p1,p2,p3;
//   if(dir1*z<0)
//     p1=plane3D(corner+dir1,dir2,dir3);
//   else
//     p1=plane3D(corner,dir3,dir2);
//   if(dir2*z<0)
//     p2=plane3D(corner+dir2,dir3,dir1);
//   else
//     p2=plane3D(corner,dir1,dir3);
//   if(dir3*z<0)
//     p3=plane3D(corner+dir3,dir1,dir2);
//   else
//     p3=plane3D(corner,dir2,dir1);

// }
// TH1C*getHistoFromSpiral(spiral *s, point3D center, vector3D x, vector3D z)
// {
// }
// TH1C*getHistoFromWedge(wedge *w, point3D center, vector3D x, vector3D z)
// {

// }
// TH1C*getHistoFromRing(ring *r, point3D center, vector3D x, vector3D z)
// {
// }
// TH1C*getHistoFromCylinder(cylinder *c, point3D center, vector3D x, vector3D
// z)
// {
// }
// TH1C*getHistoFromHexprism(hexPrism *h, point3D center, vector3D x, vector3D
// z)
// {
// }
