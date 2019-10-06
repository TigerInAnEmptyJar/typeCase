#include "APixelTreeWriteOut.h"
#include "algorithmparameter.h"
#include "fiber.h"
#include "ring.h"
#include "spiral.h"
#include "strawTube.h"
#include "wedge.h"
void APixelTreeWriteOut::setDefault(int i, int j)
{
  pixOut[i][j].dx = 0;
  pixOut[i][j].dy = 0;
  pixOut[i][j].xc = centerPoints[i][0];
  pixOut[i][j].yc = centerPoints[i][1];
  pixOut[i][j].z = 0;
  pixOut[i][j].ID = -1;
  pixOut[i][j].n1 = -1;
  pixOut[i][j].n2 = -1;
  pixOut[i][j].n3 = -1;
}
APixelTreeWriteOut::APixelTreeWriteOut(TSetup& setup, int& eventNumberIn, int& runNumberIn,
                                       TPixel*** pixelsIn, int** numberOfPixelsIn,
                                       const algorithm_parameter& param)
    : AAlgorithm("Generate pixel information for calibration"), evtNr(eventNumberIn),
      rnNr(runNumberIn)
{
  nPixelTypes = param.value(3).value<vector<int>>().size();
  PixelIDs = new int[nPixelTypes];
  pixOut = new pixelStruct*[nPixelTypes];
  pixels = new TPixel**[nPixelTypes];
  numberOfPixels = new int*[nPixelTypes];
  maxPixelPerID = param.value(0).value<int>();
  nPixels = new Int_t[nPixelTypes];
  centerPoints = new point3D[nPixelTypes];
  int n = 0, m;
  for (int i = 0; i < nPixelTypes; i++) {
    PixelIDs[i] = param.value(3).value<vector<int>>().at(i);
    pixOut[i] = new pixelStruct[maxPixelPerID];
    pixels[i] = pixelsIn[PixelIDs[i]];
    numberOfPixels[i] = numberOfPixelsIn[PixelIDs[i]];
    m = param.value(4).value<vector<int>>().at(n++);
    point3D cp[m];
    vector3D cc(0, 0, 0);
    for (int j = 0; j < m; j++) {
      TDetector* det = &setup.getDetectorr(param.value(4).value<vector<int>>().at(n++));
      if (det->getNumberOfElements() <= 0) {
        centerPoints[i].setValues(0, 0, 0);
        break;
      }
      volumeShape* shape = det->getShape(0);
      if (shape == NULL) {
        centerPoints[i].setValues(0, 0, 0);
        break;
      }
      if (shape->getName() == "wedge" || shape->getName() == "spiral" ||
          shape->getName() == "ring") {
        cp[j] = shape->getCenter();
      } else if (shape->getName() == "fiber" || shape->getName() == "strawTube") {
        if (shape->getName() == "fiber")
          cp[j] = ((fiber*)shape)->getCorner() + 0.5 * ((fiber*)shape)->getDirection(0) +
                  (det->getNumberOfElements() - ((fiber*)shape)->getHalved()) * 0.5 *
                      ((fiber*)shape)->getDirection(1);
        else if (shape->getName() == "strawTube")
          cp[j] = ((strawTube*)shape)->getCenter() +
                  (det->getNumberOfElements() - ((strawTube*)shape)->getNumberOfHalvedElements()) *
                      0.5 * ((strawTube*)shape)->getDirection(1);
      } else
        cp[j] = shape->getCenter();
      cc = cc + vector3D(cp[j]);
    }
    centerPoints[i] = point3D(cc * (1. / ((double)m)));
  }
  outFile = new TFile(param.value(1).value<string>().data(), "recreate");
  outTree = new TTree(param.value(2).value<string>().data(), param.value(2).value<string>().data());
  outTree->Branch("EventNumber", &eventNumber, "EventNumber/I");
  outTree->Branch("RunNumber", &runNumber, "RunNumber/I");
  outTree->Branch("numberOfPixels", nPixels,
                  (string("numberOfPixels[") + string_number(nPixelTypes) + "]/I").data());
  for (int i = 0; i < nPixelTypes; i++)
    for (int j = 0; j < maxPixelPerID; j++)
      outTree->Branch((string("pix_") + string_number(i) + "_" + string_number(j)).data(),
                      &pixOut[i][j], "xc/F:yc:dx:dy:z:ID/I:n1:n2:n3");
}
APixelTreeWriteOut::~APixelTreeWriteOut()
{
  outFile->cd();
  outTree->Write();
  cout << outTree->GetEntries() << " Entries in Pixel-tree written to \"" << outFile->GetName()
       << "\"" << endl;
  outFile->Close();
  delete outFile;
  for (int i = 0; i < nPixelTypes; i++)
    delete[] pixOut[i];
  delete[] pixOut;
  delete[] nPixels;
  delete[] PixelIDs;
  delete[] pixels;
  delete[] numberOfPixels;
  delete[] centerPoints;
}
void APixelTreeWriteOut::process()
{
  eventNumber = evtNr;
  runNumber = rnNr;
  int nTot = 0;
  for (int i = 0; i < nPixelTypes; i++)
    nTot += (*(numberOfPixels[i]));
  if (nTot == 0)
    return;
  for (int i = 0; i < nPixelTypes; i++) {
    nPixels[i] = (*(numberOfPixels[i]));
    if (nPixels[i] > maxPixelPerID)
      nPixels[i] = maxPixelPerID;
    for (int j = 0; j < maxPixelPerID; j++)
      setDefault(i, j);
    for (int j = 0; j < nPixels[i]; j++) {
      if (pixels[i][j]->getShape() == NULL)
        continue;
      vector3D dist = pixels[i][j]->getShape()->getCenter() - centerPoints[i];
      pixOut[i][j].dx = dist[0];
      pixOut[i][j].dy = dist[1];
      pixOut[i][j].z = pixels[i][j]->getShape()->getCenter().Z();
      pixOut[i][j].n1 = pixels[i][j]->getElementr(0).getElement();
      pixOut[i][j].n2 = pixels[i][j]->getElementr(1).getElement();
      if (pixels[i][j]->getNumberOfActualElements() > 2)
        pixOut[i][j].n3 = pixels[i][j]->getElementr(2).getElement();
    }
  }
  outTree->Fill();
}
algorithm_parameter APixelTreeWriteOut::getDescription()
{
  algorithm_parameter ret("Pixel tree for calibration", algorithm_parameter::Category::OUTPUT, 0);
  vector<int> tmp;
  ret.addValue("max Pixels per ID", static_cast<int>(20));
  ret.addValue("file name", std::string{"tmp.root"});
  ret.addValue("tree name", std::string{"pixelTree"});
  ret.addValue("log pixel IDs", tmp);
  ret.addValue("pix made up of", tmp);
  string des = "This algorithm produces a tree output for pixel position "
               "information meant for geometry calibration purposes. Each "
               "pixel has here information about element number of elements "
               "it consists of, ID, z-Position in Detector, center-of-envelope "
               "of the pixel forming detectors and the distances of the "
               "pixel-center from the center-of-envelope.";
  ret.setDescription(des);
  return ret;
}
