#include "AHistoAlgorithm.h"
#include "reactions.h"
#include <stdlib.h>
TH2F betaHisto("beta geom vs TOF", "beta geom vs TOF", 100, 0, 2, 100, 0, 2);
TH1F dirHisto("angle cm", "angle cm", 100, 0, 180);
TH2F radpix0("tdc vs angle det0", "tdc vs angle det0", 100, 0, 0.3, 100, -100, 0);
TH2F radpix15("tdc vs angle det15", "tdc vs angle det15", 100, 0, 1, 100, -100, 0);
TH2F radpix16("tdc vs angle det16", "tdc vs angle det16", 100, 0, 1, 100, -100, 0);
TH2F radpix3("tdc vs angle det3", "tdc vs angle det3", 100, 0, 0.5, 100, -100, 0);
AHistoAlgorithm::AHistoAlgorithm() : AAlgorithm("Write histos to file") {}

AHistoAlgorithm::~AHistoAlgorithm()
{
  //   dir->cd();
  //   for(int i=0;i<numOD;i++)
  //     ODhistos[startO+i]->Write();
  //   for(int i=0;i<numTD;i++)
  //     TDhistos[startT+i]->Write();
  //   for(int i=0;i<numOD;i++)
  //     delete ODhistos[startO+i];
  //   for(int i=0;i<numTD;i++)
  //     delete TDhistos[startT+i];
}

int AHistoAlgorithm::getNumberOfHistos(int dim)
{
  if (dim == 1)
    return numOD;
  if (dim == 2)
    return numTD;
  return -1;
}

int AHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  return numOD;
}

int AHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  return numTD;
}

void* AHistoAlgorithm::process(void* ptr) { return ptr; }
void AHistoAlgorithm::write() {}

/***** Write raw hit properties to histogram: ADC, TDC, detector use */
ARawHitHistoAlgorithm::ARawHitHistoAlgorithm(TRawHit*** rawHits, int** numHits, TSetup& setup,
                                             TDirectory* parent, const string& opt, bool jf)
{
  justFill = jf;
  numD = 0;
  numOD = 0;
  numTD = 0;
  dir = parent->mkdir("Raw Hits");
  dir->mkdir("detector use");
  int nEl;
  raws = new TRawHit**[setup.getNumberOfDetectors()];
  numberOfHits = new int*[setup.getNumberOfDetectors()];
  maxEl = new int[setup.getNumberOfDetectors()];
  dID = new int[setup.getNumberOfDetectors()];
  TDirectory *tmpDir1, *tmpDir2;
  tmpDir1 = dir->mkdir("Raw ADC");
  tmpDir2 = dir->mkdir("Raw TDC");
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data())) {
      nEl = setup.getDetectorr(i).getNumberOfElements();
      numOD = numOD + 2 * nEl;
      if (nEl > 0) {
        numOD++;
        raws[numD] = rawHits[i];
        numberOfHits[numD] = numHits[i];
        maxEl[numD] = nEl;
        dID[numD] = setup.getDetectorr(i).getID();
        tmpDir1->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        tmpDir2->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        numD++;
      }
    }
  }
  numOD++;
}

ARawHitHistoAlgorithm::~ARawHitHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++)
    if (ODhistos[startO + i] != NULL)
      delete ODhistos[startO + i];
  delete[] maxEl;
  delete[] dID;
}
void ARawHitHistoAlgorithm::write()
{
  int offs = 0;
  dir->cd();
  for (int i = 0; i < numD; i++) {
    dir->cd("detector use");
    ODhistos[startO + offs]->Write();
    for (int j = 0; j < maxEl[i]; j++) {
      dir->cd(
          (string("Raw ADC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i])).data());
      ODhistos[startO + 2 * j + offs + 1]->Write();
      dir->cd(
          (string("Raw TDC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i])).data());
      ODhistos[startO + 2 * j + offs + 2]->Write();
    }
    offs = offs + 2 * maxEl[i] + 1;
  }
  dir->cd();
  ODhistos[startO + numOD - 1]->Write();
  for (int i = 0; i < numOD; i++)
    ODhistos[startO + i]->SetDirectory(0);
}
int ARawHitHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  if (!justFill) {
    int offs = 0;
    for (int i = 0; i < numD; i++) {
      ODhistos[startO + offs] = new TH1F(
          (string("Raw Detector use ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          (string("Raw Detector Use ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          maxEl[i], 0, maxEl[i]);
      for (int j = 0; j < maxEl[i]; j++) {
        ODhistos[startO + offs + j * 2 + 1] =
            new TH1F((string("Raw ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     (string("Raw ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     1000, 0, 1000);
        ODhistos[startO + offs + j * 2 + 2] =
            new TH1F((string("Raw TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     (string("Raw TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     1000, 0, 1000);
      }
      offs = offs + 2 * maxEl[i] + 1;
    }
    ODhistos[startO + numOD - 1] =
        new TH1F("Raw Number of Hits", "Raw Number of Hits", 200, 0, 200);
  }
  return numOD;
}

int ARawHitHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  return numTD;
}

void* ARawHitHistoAlgorithm::process(void* ptr)
{
  int offs = 0;
  int elNum;
  int num = 0;
  for (int i = 0; i < numD; i++) {
    num = num + (*(numberOfHits[i]));
    for (int j = 0; j < *(numberOfHits[i]); j++) {
      elNum = raws[i][j]->getElement();
      ODhistos[startO + offs]->Fill(elNum);
      ODhistos[startO + offs + elNum * 2 + 1]->Fill(raws[i][j]->getADC());
      ODhistos[startO + offs + elNum * 2 + 2]->Fill(raws[i][j]->getTDC());
    }
    offs = offs + maxEl[i] * 2 + 1;
  }
  ODhistos[startO + numOD - 1]->Fill(num);
  return ptr;
}

/***** Write raw hit with cuts properties to histogram: ADC, TDC, detector use
 */
ARawHitCutHistoAlgorithm::ARawHitCutHistoAlgorithm(TCalibHit*** Hits, int** numHits, TSetup& setup,
                                                   TDirectory* parent, const string& opt, bool jf)
{
  justFill = jf;
  numD = 0;
  numOD = 0;
  numTD = 0;
  dir = parent->mkdir("Raw Hits with cuts");
  dir->mkdir("detector use");
  TDirectory *tmpDir1, *tmpDir2;
  tmpDir1 = dir->mkdir("Cut Raw ADC");
  tmpDir2 = dir->mkdir("Cut Raw TDC");
  int nEl;
  hits = new TCalibHit**[setup.getNumberOfDetectors()];
  numberOfHits = new int*[setup.getNumberOfDetectors()];
  maxEl = new int[setup.getNumberOfDetectors()];
  dID = new int[setup.getNumberOfDetectors()];
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data())) {
      nEl = setup.getDetectorr(i).getNumberOfElements();
      numOD = numOD + 2 * nEl;
      if (nEl > 0) {
        numOD++;
        hits[numD] = Hits[i];
        numberOfHits[numD] = numHits[i];
        maxEl[numD] = nEl;
        dID[numD] = setup.getDetectorr(i).getID();
        tmpDir1->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        tmpDir2->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        numD++;
      }
    }
  }
  numOD++;
}

ARawHitCutHistoAlgorithm::~ARawHitCutHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++)
    delete ODhistos[startO + i];
  delete[] maxEl;
  delete[] dID;
}
void ARawHitCutHistoAlgorithm::write()
{
  int offs = 0;
  dir->cd();
  for (int i = 0; i < numD; i++) {
    dir->cd("detector use");
    ODhistos[startO + offs]->Write();
    for (int j = 0; j < maxEl[i]; j++) {
      dir->cd((string("Cut Raw ADC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
                  .data());
      ODhistos[startO + 2 * j + offs + 1]->Write();
      dir->cd((string("Cut Raw TDC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
                  .data());
      ODhistos[startO + 2 * j + offs + 2]->Write();
    }
    offs = offs + 2 * maxEl[i] + 1;
  }
  dir->cd();
  ODhistos[startO + numOD - 1]->Write();
  for (int i = 0; i < numOD; i++)
    ODhistos[startO + i]->SetDirectory(0);
}
int ARawHitCutHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  if (!justFill) {
    int offs = 0;
    for (int i = 0; i < numD; i++) {
      ODhistos[startO + offs] = new TH1F(
          (string("Detector use Cuts ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          (string("Detector Use Cuts ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          maxEl[i], 0, maxEl[i]);
      for (int j = 0; j < maxEl[i]; j++) {
        ODhistos[startO + offs + j * 2 + 1] = new TH1F(
            (string("Cut Raw ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
             " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                .data(),
            (string("Cut Raw ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
             " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                .data(),
            1000, 0, 1000);
        ODhistos[startO + offs + j * 2 + 2] = new TH1F(
            (string("Cut Raw TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
             " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                .data(),
            (string("Cut Raw TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
             " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                .data(),
            1000, 0, 1000);
      }
      offs = offs + 2 * maxEl[i] + 1;
    }
    ODhistos[startO + numOD - 1] =
        new TH1F("Cut Number of Hits", "Cut Number of Hits", 100, 0, 100);
  }
  return numOD;
}

int ARawHitCutHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  return numTD;
}

void* ARawHitCutHistoAlgorithm::process(void* ptr)
{
  int offs = 0;
  int elNum;
  int num = 0;
  for (int i = 0; i < numD; i++) {
    for (int j = 0; j < *(numberOfHits[i]); j++) {
      if (hits[i][j]->isValid()) {
        num++;
        elNum = hits[i][j]->getElement();
        ODhistos[startO + offs]->Fill(elNum);
        ODhistos[startO + offs + elNum * 2 + 1]->Fill(hits[i][j]->getRawADC());
        ODhistos[startO + offs + elNum * 2 + 2]->Fill(hits[i][j]->getRawTDC());
      }
    }
    offs = offs + 2 * maxEl[i] + 1;
  }
  ODhistos[startO + numOD - 1]->Fill(num);
  return ptr;
}

/***** Write raw hit with cuts properties to histogram: ADC, TDC, detector use
 */
ACalibHitHistoAlgorithm::ACalibHitHistoAlgorithm(TCalibHit*** Hits, int** numHits, TSetup& setup,
                                                 TDirectory* parent, const string& opt, bool jf)
{
  justFill = jf;
  numD = 0;
  numOD = 0;
  numTD = 0;
  dir = parent->mkdir("Calibrated Hits");
  dir->mkdir("detector use");
  TDirectory *tmpDir1, *tmpDir2;
  tmpDir1 = dir->mkdir("ADC");
  tmpDir2 = dir->mkdir("TDC");
  int nEl;
  hits = new TCalibHit**[setup.getNumberOfDetectors()];
  numberOfHits = new int*[setup.getNumberOfDetectors()];
  maxEl = new int[setup.getNumberOfDetectors()];
  dID = new int[setup.getNumberOfDetectors()];
  for (int i = 0; i < setup.getNumberOfDetectors(); i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data())) {
      nEl = setup.getDetectorr(i).getNumberOfElements();
      numOD = numOD + 2 * nEl;
      if (nEl > 0) {
        numOD++;
        hits[numD] = Hits[i];
        numberOfHits[numD] = numHits[i];
        maxEl[numD] = nEl;
        dID[numD] = setup.getDetectorr(i).getID();
        tmpDir1->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        tmpDir2->mkdir(
            (string("detector ") + (dID[numD] < 10 ? "0" : "") + string_number(dID[numD])).data());
        numD++;
      }
    }
  }
  numOD++;
}

ACalibHitHistoAlgorithm::~ACalibHitHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++)
    delete ODhistos[startO + i];
  delete[] maxEl;
  delete[] dID;
}
void ACalibHitHistoAlgorithm::write()
{
  int offs = 0;
  dir->cd();
  for (int i = 0; i < numD; i++) {
    dir->cd("detector use");
    ODhistos[startO + offs]->Write();
    for (int j = 0; j < maxEl[i]; j++) {
      dir->cd((string("ADC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i])).data());
      ODhistos[startO + 2 * j + offs + 1]->Write();
      dir->cd((string("TDC/detector ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i])).data());
      ODhistos[startO + 2 * j + offs + 2]->Write();
    }
    offs = offs + 2 * maxEl[i] + 1;
  }
  dir->cd();
  ODhistos[startO + numOD - 1]->Write();
  for (int i = 0; i < numOD; i++)
    ODhistos[startO + i]->SetDirectory(0);
}
int ACalibHitHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  if (!justFill) {
    int offs = 0;
    for (int i = 0; i < numD; i++) {
      ODhistos[startO + offs] = new TH1F(
          (string("Calibrated Detector use ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          (string("Calibrated Detector Use ID ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]))
              .data(),
          maxEl[i], 0, maxEl[i]);
      for (int j = 0; j < maxEl[i]; j++) {
        ODhistos[startO + offs + j * 2 + 1] =
            new TH1F((string("ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     (string("ADC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     1000, 0, 10000);
        ODhistos[startO + offs + j * 2 + 2] =
            new TH1F((string("TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     (string("TDC Det ") + (dID[i] < 10 ? "0" : "") + string_number(dID[i]) +
                      " El " + (j < 10 ? "00" : (j < 100 ? "0" : "")) + string_number(j))
                         .data(),
                     1000, -10000, 0);
      }
      offs = offs + 2 * maxEl[i] + 1;
    }
    ODhistos[startO + numOD - 1] =
        new TH1F("Calibrated Number of Hits", "Calibrated Number of Hits", 100, 0, 100);
  }
  return numOD;
}

int ACalibHitHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  return numTD;
}

void* ACalibHitHistoAlgorithm::process(void* ptr)
{
  int offs = 0;
  int elNum;
  int num = 0;
  for (int i = 0; i < numD; i++) {
    for (int j = 0; j < *(numberOfHits[i]); j++) {
      if (hits[i][j]->isValid()) {
        num++;
        elNum = hits[i][j]->getElement();
        ODhistos[startO + offs]->Fill(elNum);
        ODhistos[startO + offs + elNum * 2 + 1]->Fill(hits[i][j]->getADC());
        ODhistos[startO + offs + elNum * 2 + 2]->Fill(hits[i][j]->getTDC());
      }
    }
    offs = offs + maxEl[i] * 2 + 1;
  }
  ODhistos[startO + numOD - 1]->Fill(num);
  return ptr;
}

APixelHistoAlgorithm::APixelHistoAlgorithm(TPixel*** Pixel, int** numPix, int maxDet,
                                           TDirectory* parent, const string& opt, bool jf)
{
  justFill = jf;
  numD = 0;
  numOD = 0;
  numTD = 0;
  dir = parent->mkdir("Pixel");
  dir->mkdir("number of pixels");
  pixels = new TPixel**[maxDet];
  numberOfPixels = new int*[maxDet];
  pID = new int[maxDet];
  for (int i = 0; i < maxDet; i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data())) {
      numOD = numOD + 3; // numberOfPixels,theta,phi
      pixels[numD] = Pixel[i];
      numberOfPixels[numD] = numPix[i];
      pID[numD] = i;
      numD++;
    }
  }
  numOD = numOD + 5;
  numTD = 2;
}

APixelHistoAlgorithm::~APixelHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++) {
    if (ODhistos[startO + i])
      delete ODhistos[startO + i];
    ODhistos[startO + i] = NULL;
  }
  for (int i = 0; i < numTD; i++) {
    if (TDhistos[startT + i])
      delete TDhistos[startT + i];
    TDhistos[startT + i] = NULL;
  }
  if (pID)
    delete[] pID;
  //  cout<<"a"<<endl;
}
void APixelHistoAlgorithm::write()
{
  dir->cd();
  for (int i = 0; i < numD; i++) {
    if (ODhistos[startO + 3 * i + 1] != NULL)
      ODhistos[startO + 3 * i + 1]->Write();
    if (ODhistos[startO + 3 * i + 2] != NULL)
      ODhistos[startO + 3 * i + 2]->Write();
  }
  //  for(int i=0;i<5;i++)
  if (ODhistos[startO + 3 * numD + 0] != NULL)
    ODhistos[startO + 3 * numD + 0]->Write();
  if (ODhistos[startO + 3 * numD + 1] != NULL)
    ODhistos[startO + 3 * numD + 1]->Write();
  if (ODhistos[startO + 3 * numD + 2] != NULL)
    ODhistos[startO + 3 * numD + 2]->Write();
  if (ODhistos[startO + 3 * numD + 3] != NULL)
    ODhistos[startO + 3 * numD + 3]->Write();
  if (ODhistos[startO + 3 * numD + 4] != NULL)
    ODhistos[startO + 3 * numD + 4]->Write();
  for (int i = 0; i < numTD; i++)
    TDhistos[startT + i]->Write();
  dir->cd("number of pixels");
  for (int i = 0; i < numD; i++)
    ODhistos[startO + 3 * i]->Write();
  for (int i = 0; i < numTD; i++)
    TDhistos[startT + i]->SetDirectory(0);
  for (int i = 0; i < numOD; i++)
    ODhistos[startO + i]->SetDirectory(0);
}
int APixelHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  if (!justFill) {
    for (int i = 0; i < numD; i++) {
      ODhistos[startO + 3 * i + 1] = new TH1F(
          (string("Phi angle of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i]))
              .data(),
          (string("Phi angle of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i]))
              .data(),
          1000, -M_PI, M_PI);
      ODhistos[startO + 3 * i + 2] = new TH1F(
          (string("Theta angle of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i]))
              .data(),
          (string("Theta angle of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i]))
              .data(),
          1000, 0, M_PI);
      ODhistos[startO + 3 * i + 0] = new TH1F(
          (string("Number of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i])).data(),
          (string("Number of Pixel-ID ") + (pID[i] < 10 ? "0" : "") + string_number(pID[i])).data(),
          100, 0, 100);
    }
    ODhistos[startO + 3 * numD + 0] =
        new TH1F("Number of Pixels in Event", "Number of Pixels in Event", 100, 0, 100);
    ODhistos[startO + 3 * numD + 1] =
        new TH1F("Phi angle of all Pixels", "Phi angle of all Pixels", 1000, -M_PI, M_PI);
    ODhistos[startO + 3 * numD + 2] =
        new TH1F("Theta angle of all Pixels", "Theta angle of all Pixels", 1000, 0, M_PI);
    ODhistos[startO + 3 * numD + 3] = new TH1F("Phi difference quirl vs. barrel",
                                               "Phi difference quirl vs. barrel", 100, 0, 2 * M_PI);
    ODhistos[startO + 3 * numD + 4] = new TH1F("Phi difference ring vs. barrel",
                                               "Phi difference ring vs. barrel", 100, 0, 2 * M_PI);
  }
  for (int i = 0; i < numOD; i++)
    if (ODhistos[startO + i])
      ODhistos[startO + i]->SetDirectory(0);
  return numOD;
}

int APixelHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  if (!justFill) {
    TDhistos[startT] = new TH2F("phi-angle barrel vs. quirl", "phi-angle barrel vs. quirl", 100, 0,
                                2 * M_PI, 100, 0, M_PI);
    TDhistos[startT + 1] = new TH2F("phi-angle barrel vs. ring", "phi-angle barrel vs. ring", 100,
                                    0, 2 * M_PI, 100, 0, M_PI);
  }
  for (int i = 0; i < numTD; i++)
    if (TDhistos[startT + i])
      TDhistos[startT + i]->SetDirectory(0);
  return numTD;
}

void* APixelHistoAlgorithm::process(void* ptr)
{
  int num = 0;
  point3D center;
  TPixel** tmp0 = NULL;
  TPixel** tmp1 = NULL;
  TPixel** tmp2 = NULL;
  planeShape* s1 = NULL; //,*s2=NULL,*s3=NULL;
  int *n0 = NULL, *n1 = NULL, *n2 = NULL;
  //  int sumP=0;
  for (int i = 0; i < numD; i++) {
    if (pID[i] == 0) {
      tmp0 = pixels[i];
      n0 = numberOfPixels[i];
    }
    if (pID[i] == 1) {
      tmp1 = pixels[i];
      n1 = numberOfPixels[i];
    }
    if (pID[i] == 2) {
      tmp2 = pixels[i];
      n2 = numberOfPixels[i];
    }
    num = num + *(numberOfPixels[i]);
    for (int j = 0; j < *(numberOfPixels[i]); j++) {
      s1 = pixels[i][j]->getShape();
      if (s1) {
        center = s1->getCenter();
        ODhistos[startO + i * 3 + 0]->Fill(*(numberOfPixels[i]));
        ODhistos[startO + 3 * i + 1]->Fill(center.Phi());
        ODhistos[startO + 3 * i + 2]->Fill(center.Theta());
        ODhistos[startO + 3 * numD + 1]->Fill(center.Phi());
        ODhistos[startO + 3 * numD + 2]->Fill(center.Theta());
      }
    }
  }
  ODhistos[startO + 3 * numD + 0]->Fill(num);
  if (num == 2) {
    if (n2 != NULL) {
      if (*n2 == 2) {
        TDhistos[startT]->Fill(
            abs(tmp2[0]->getShape()->getCenter().Phi() - tmp2[1]->getShape()->getCenter().Phi()),
            tmp2[0]->getShape()->getCenter().Theta() + tmp2[1]->getShape()->getCenter().Theta());
      } else if (*n2 == 1) {
        if (n0 != NULL) {
          if (*n0 == 1) {
            TDhistos[startT + 1]->Fill(abs(tmp2[0]->getShape()->getCenter().Phi() -
                                           tmp0[0]->getShape()->getCenter().Phi()),
                                       tmp2[0]->getShape()->getCenter().Theta() +
                                           tmp0[0]->getShape()->getCenter().Theta());
          }
        } else if (n1 != NULL) {
          if (*n1 == 1)

          {
            TDhistos[startT + 1]->Fill(abs(tmp2[0]->getShape()->getCenter().Phi() -
                                           tmp1[0]->getShape()->getCenter().Phi()),
                                       tmp2[0]->getShape()->getCenter().Theta() +
                                           tmp1[0]->getShape()->getCenter().Theta());
          }
        }
      }
    }
  }
  return ptr;
}
ATrackHistoAlgorithm::ATrackHistoAlgorithm(TTrack** Tracks, int& numTracks, TDirectory* parent,
                                           const string& opt, bool jf)
    : numberOfTracks(numTracks)
{
  justFill = jf;
  tracks = Tracks;
  numOD = 4;
  numTD = 0;
  options = opt.data();
  dir = parent->mkdir("Tracks");
  nTracks = new int[100];
  numD = 0;
  if (hasA(opt, "_v_"))
    numD++;
  if (hasA(opt, "_k_"))
    numD++;
  if (hasA(opt, "_p_"))
    numD++;
  for (int i = 0; i < 100; i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data()))
      numD++;
  }
  st1 = new int[numD];
  st2 = new int[numD];
  numD = 0;
  if (hasA(opt, "_v_")) {
    nTracks[numD] = -1;
    dir->mkdir("Tracks-Neutral-Decays");
    st1[numD] = numOD;
    st2[numD] = numTD;
    numOD = numOD + 6 + 3; // directions of tracks
    numOD = numOD + 3;     // speed of tracks
    numOD++;               // z-position of vertex
    numTD = numTD + 2;     // vertex of each track
    numD++;
  }
  if (hasA(opt, "_k_")) {
    st1[numD] = numOD;
    st2[numD] = numTD;
    nTracks[numD] = -2;
    numD++;
    dir->mkdir("Tracks-Charged-Decays");
    numOD = numOD + 4 + 1; // directions of tracks
    numOD = numOD + 3;     // speed of tracks
    numTD = numTD + 2;     // vertex of each track
  }
  if (hasA(opt, "_p_")) {
    st1[numD] = numOD;
    st2[numD] = numTD;
    nTracks[numD] = -3;
    numD++;
    dir->mkdir("Tracks-Prompt");
    numOD = numOD + 3; // beta, theta, phi
    numTD = numTD + 1; // vertex
  }
  for (int i = 0; i < 100; i++) {
    if (hasA(opt, (string("_") + string_number(i) + "_").data())) {
      st1[numD] = numOD;
      st2[numD] = numTD;
      nTracks[numD] = i;
      numD++;
      dir->mkdir((string("Tracks-") + string_number(i)).data());
      numOD = numOD + 2 * i + 1; // directions of tracks
      numOD = numOD + i;         // speed of tracks
      numTD = numTD + i;         // vertex of each track
      for (int j = 0; j < i; j++)
        numTD = numTD + j; // phi1-phi2 vs theta1-theta2 for each two tracks
    }
  }
  numOD += 4;
  numTD += 2;
}

ATrackHistoAlgorithm::~ATrackHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++) {
    if (ODhistos[startO + i])
      delete ODhistos[startO + i];
    ODhistos[startO + i] = NULL;
  }
  for (int i = 0; i < numTD; i++) {
    if (TDhistos[startT + i])
      delete TDhistos[startT + i];
    TDhistos[startT + i] = NULL;
  }
  delete[] st1;
  delete[] st2;
  //   TCanvas *canv=new TCanvas();
  //   betaHisto.Draw();
  //   canv->Print("betaTest.ps");
  //   delete canv;
}
void ATrackHistoAlgorithm::write()
{
  dir->cd();
  int offs = 3, offs1 = 0;
  for (int i = 1; i < 6; i++)
    if (ODhistos[startO + numOD - i])
      ODhistos[startO + numOD - i]->Write();
  for (int i = 0; i < 3; i++)
    if (ODhistos[startO + i])
      ODhistos[startO + i]->Write();
  betaHisto.Write();
  radpix0.Write();
  radpix3.Write();
  radpix15.Write();
  radpix16.Write();
  if (TDhistos[startT + numTD - 1])
    TDhistos[startT + numTD - 1]->Write();
  if (TDhistos[startT + numTD - 2])
    TDhistos[startT + numTD - 2]->Write();
  for (int i = 0; i < numD; i++) {
    if (nTracks[i] > 0) {
      dir->cd((string("Tracks-") + string_number(nTracks[i])).data());
      if (ODhistos[startO + offs])
        ODhistos[startO + offs]->Write();
      for (int j = 0; j < nTracks[i]; j++) {
        if (ODhistos[startO + offs + 3 * j + 1])
          ODhistos[startO + offs + 3 * j + 1]->Write();
        if (ODhistos[startO + offs + 3 * j + 2])
          ODhistos[startO + offs + 3 * j + 2]->Write();
        if (ODhistos[startO + offs + 3 * j + 3])
          ODhistos[startO + offs + 3 * j + 3]->Write();
        if (TDhistos[startT + offs1 + j])
          TDhistos[startT + offs1 + j]->Write();
        for (int k = 0; k < j; k++)
          if (TDhistos[startT + offs1 + j + k + 1])
            TDhistos[startT + offs1 + j + k + 1]->Write();
        offs1 = offs1 + j;
      }
      offs = offs + nTracks[i] * 3 + 1;
      offs1 = offs1 + nTracks[i];
    } else {
      int n1 = 0, n2 = 0;
      if (nTracks[i] == -1) {
        dir->cd("Tracks-Neutral-Decays");
        n1 = 13;
        n2 = 2;
      } else if (nTracks[i] == -2) {
        dir->cd("Tracks-Charged-Decays");
        n1 = 8;
        n2 = 2;
      } else if (nTracks[i] == -3) {
        dir->cd("Tracks-Prompt");
        n1 = 3;
        n2 = 1;
      }
      for (int j = 0; j < n1; j++)
        if (ODhistos[startO + st1[i] + j] != NULL) {
          ODhistos[startO + st1[i] + j]->Write();
          ODhistos[startO + st1[i] + j]->SetDirectory(0);
        }
      offs += n1;
      for (int j = 0; j < n2; j++)
        if (TDhistos[startT + st2[i] + j] != NULL) {
          TDhistos[startT + st2[i] + j]->Write();
          TDhistos[startT + st2[i] + j]->SetDirectory(0);
        }
      offs1 += n2;
    }
  }
  for (int i = 0; i < numTD; i++)
    if (TDhistos[startT + i] != NULL)
      TDhistos[startT + i]->SetDirectory(0);
  for (int i = 0; i < numOD; i++)
    if (ODhistos[startO + i] != NULL)
      ODhistos[startO + i]->SetDirectory(0);
}
int ATrackHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  if (!justFill) {
    for (int i = 0; i < numOD; i++)
      ODhistos[startO + i] = NULL;
    ODhistos[startO + 0] =
        new TH1F("Any track \\theta-angle", "Any track \\theta-angle", 100, 0, M_PI);
    ODhistos[startO + 1] =
        new TH1F("Any track \\phi-angle", "Any track \\phi-angle", 100, -M_PI, M_PI);
    ODhistos[startO + 2] = new TH1F("Any track \\beta", "Any track \\beta", 100, 0, 1.2);
    for (int i = 0; i < numD; i++) {
      if (nTracks[i] > 0) {
        ODhistos[startO + st1[i]] =
            new TH1F((string("Number of hits per track-") + string_number(nTracks[i])).data(),
                     (string("Number of hits per track-") + string_number(nTracks[i])).data(), 20,
                     -0.5, 19.5);
        for (int j = 0; j < nTracks[i]; j++) {
          ODhistos[startO + st1[i] + 3 * j + 1] = new TH1F(
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + " Phi angle")
                  .data(),
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + " Phi angle")
                  .data(),
              1000, -M_PI, M_PI);
          ODhistos[startO + st1[i] + 3 * j + 2] = new TH1F(
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + " Theta angle")
                  .data(),
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + " Theta angle")
                  .data(),
              1000, 0, M_PI);
          ODhistos[startO + st1[i] + 3 * j + 3] = new TH1F(
              (string_number(nTracks[i]) + "-Tracks: velocity" + string_number(j)).data(),
              (string_number(nTracks[i]) + "-Tracks: Track velocity" + string_number(j)).data(),
              1000, 0, 1);
        }
      } else if (nTracks[i] == -1) {
        ODhistos[startO + st1[i]] =
            new TH1F("Neutral Decay: Neutral Track: \\phi", "Neutral Decay: Neutral Track: \\phi",
                     1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("Neutral Decay: Neutral Track: \\vartheta",
                     "Neutral Decay: Neutral Track: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 2] =
            new TH1F("Neutral Decay: Decay Track 1: \\phi", "Neutral Decay: Decay Track 1: \\phi",
                     1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 3] =
            new TH1F("Neutral Decay: Decay Track 1: \\vartheta",
                     "Neutral Decay: Decay Track 1: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 4] =
            new TH1F("Neutral Decay: Decay Track 2: \\phi", "Neutral Decay: Decay Track 2: \\phi",
                     1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 5] =
            new TH1F("Neutral Decay: Decay Track 2: \\vartheta",
                     "Neutral Decay: Decay Track 2: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 6] =
            new TH1F("Neutral Decay: \\alpha_{decay Tracks}",
                     "Neutral Decay: \\alpha_{decay Tracks}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 7] =
            new TH1F("Neutral Decay: \\alpha_{neutral, decay Track 1}",
                     "Neutral Decay: \\alpha_{neutral, decay Track 1}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 8] =
            new TH1F("Neutral Decay: \\alpha_{neutral, decay Track 2}",
                     "Neutral Decay: \\alpha_{neutral, decay Track 2}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 9] =
            new TH1F("Neutral Decay: \\beta_{neutral Track}",
                     "Neutral Decay: \\beta_{neutral Track }", 1000, 0, 1);
        ODhistos[startO + st1[i] + 10] =
            new TH1F("Neutral Decay: \\beta_{decay Track 1}",
                     "Neutral Decay: \\beta_{decay Track 1}", 1000, 0, 1);
        ODhistos[startO + st1[i] + 11] =
            new TH1F("Neutral Decay: \\beta_{decay Track 2}",
                     "Neutral Decay: \\beta_{decay Track 2}", 1000, 0, 1);
        ODhistos[startO + st1[i] + 12] = new TH1F(
            "Neutral Decay: Z_{decay vertex}", "Neutral Decay: Z_{decay vertex}", 10000, 0, 1000);
      } else if (nTracks[i] == -2) {
        ODhistos[startO + st1[i]] =
            new TH1F("Charged Decay: Prompt Track: \\phi", "Charged Decay: Prompt Track: \\phi",
                     1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("Charged Decay: Prompt Track: \\vartheta",
                     "Charged Decay: Prompt Track: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 2] =
            new TH1F("Charged Decay: Decay Track: \\phi", "Charged Decay: Decay Track: \\phi", 1000,
                     -M_PI, M_PI);
        ODhistos[startO + st1[i] + 3] =
            new TH1F("Charged Decay: Decay Track: \\vartheta",
                     "Charged Decay: Decay Track: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 4] =
            new TH1F("Charged Decay: \\alpha_{prompt, decay Track}",
                     "Charged Decay: \\alpha_{prompt, decay Track}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 5] =
            new TH1F("Charged Decay: \\beta_{prompt Track}",
                     "Charged Decay: \\beta_{prompt Track }", 1000, 0, 1);
        ODhistos[startO + st1[i] + 6] = new TH1F("Charged Decay: \\beta_{decay Track}",
                                                 "Charged Decay: \\beta_{decay Track}", 1000, 0, 1);
        ODhistos[startO + st1[i] + 7] = new TH1F("Charged Decay: Z_{decay vertex}",
                                                 "Charged Decay: Z_{decay vertex}", 10000, 0, 1000);
      } else if (nTracks[i] == -3) {
        ODhistos[startO + st1[i]] =
            new TH1F("Prompt Tracks: \\vartheta", "Prompt Tracks: \\vartheta", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("Prompt Tracks: \\beta", "Prompt Tracks: \\beta", 1000, 0, 2);
        ODhistos[startO + st1[i] + 2] =
            new TH1F("Prompt Tracks: \\phi", "Prompt Tracks: \\phi", 1000, -M_PI, M_PI);
      }
    }
    ODhistos[startO + numOD - 4] =
        new TH1F("Number of tracks in event", "Number of tracks in event", 20, -0.5, 19.5);
    ODhistos[startO + numOD - 2] = new TH1F("Number of prompt tracks in event",
                                            "Number of prompt tracks in event", 20, -0.5, 19.5);
    ODhistos[startO + numOD - 5] = new TH1F("Number of neutral decays in event",
                                            "Number of neutral decays in event", 20, -0.5, 19.5);
    ODhistos[startO + numOD - 3] =
        new TH1F("Number of charged decays in event", "Number of charged decays tracks in event",
                 20, -0.5, 19.5);
    ODhistos[startO + numOD - 1] =
        new TH1F("Missing Mass 2 primaries", "Missing Mass 2 primaries", 100, 0, 2);
  }
  for (int i = 0; i < numOD; i++)
    if (ODhistos[startO + i] != NULL)
      ODhistos[startO + i]->SetDirectory(0);
  return numOD;
}

int ATrackHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  if (!justFill) {
    for (int i = 0; i < numTD; i++)
      TDhistos[startT + i] = NULL;
    int offs1 = 0;
    offs1 = 0;
    for (int i = 0; i < numD; i++) {
      if (nTracks[i] > 0) {
        offs1 = st2[i];
        for (int j = 0; j < nTracks[i]; j++) {
          TDhistos[startT + offs1 + j] = new TH2F(
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + "Vertex x vs. y")
                  .data(),
              (string_number(nTracks[i]) + "-Tracks: Track " + string_number(j) + "Vertex x vs. y")
                  .data(),
              1000, -100, 100, 1000, -100, 100);
          for (int k = 0; k < j; k++) {
            TDhistos[startT + offs1 + j + k + 1] =
                new TH2F((string_number(nTracks[i]) + "-Tracks: delta phi vs. theta sum tracks " +
                          string_number(j) + " vs. " + string_number(k))
                             .data(),
                         (string_number(nTracks[i]) + "-Tracks: delta phi vs. theta sum tracks " +
                          string_number(j) + " vs. " + string_number(k))
                             .data(),
                         1000, 0, 2 * M_PI, 1000, 0, M_PI);
          }
          offs1 = offs1 + j;
        }
      } else if (nTracks[i] == -1) {
        TDhistos[startT + st2[i]] =
            new TH2F("Neutral Decay: Prompt Track vertex", "Neutral Decay: Prompt Track vertex",
                     1000, -100, 100, 1000, -100, 100);
        TDhistos[startT + st2[i] + 1] =
            new TH2F("Neutral Decay: Decay vertex", "Neutral Decay: Decay vertex", 1000, -1000,
                     1000, 1000, -1000, 1000);
      } else if (nTracks[i] == -2) {
        TDhistos[startT + st2[i]] =
            new TH2F("Charged Decay: Prompt Track vertex", "Charged Decay: Prompt Track: vertex",
                     1000, -100, 100, 1000, -100, 100);
        TDhistos[startT + st2[i] + 1] =
            new TH2F("Charged Decay: Decay vertex", "Charged Decay: Decay vertex", 1000, -1000,
                     1000, 1000, -1000, 1000);
      } else if (nTracks[i] == -3) {
        TDhistos[startT + st2[i]] = new TH2F("Prompt Tracks vertex", "Prompt Tracks: vertex", 1000,
                                             -100, 100, 1000, -100, 100);
      }
    }
    TDhistos[startT + numTD - 1] =
        new TH2F("Pixel-Track-Phi vs. number of elements", "Pixel-Track-Phi vs. number of elements",
                 1000, 0, 2 * M_PI, 10, 0, 10);
    TDhistos[startT + numTD - 2] =
        new TH2F("Pixel-Track-Theta vs. number of elements",
                 "Pixel-Track-Theta vs. number of elements", 1000, 0, 2 * M_PI, 10, 0, 10);
  }
  for (int i = 0; i < numTD; i++)
    if (TDhistos[startT + i])
      TDhistos[startT + i]->SetDirectory(0);
  return numTD;
}
#include "Eparticles.h"
float MissingMass(vector3D dir1, float speed1, vector3D dir2, float speed2, momentum4D in1,
                  momentum4D in2, float m1, float m2, float mx)
{
  momentum4D m11, m12, m21, m22, r1, r2;
  dir1.normalize();
  dir2.normalize();
  dir1 = dir1 * speed1;
  dir2 = dir2 * speed2;
  m11.setVM(dir1, m1);
  m12.setVM(dir2, m2);
  m21.setVM(dir1, m2);
  m22.setVM(dir2, m1);
  r1 = in1 + in2 - m11 - m12;
  r2 = in1 + in2 - m21 - m22;
  float mm1 = (in1.Energy() + in2.Energy() - m11.Energy() - m12.Energy()) *
                  (in1.Energy() + in2.Energy() - m11.Energy() - m12.Energy()) -
              (m11.Momentum() + m12.Momentum() - in1.Momentum() - in2.Momentum()) *
                  (m11.Momentum() + m12.Momentum() - in1.Momentum() - in2.Momentum());
  float mm2 = (in1.Energy() + in2.Energy() - m21.Energy() - m22.Energy()) *
                  (in1.Energy() + in2.Energy() - m21.Energy() - m22.Energy()) -
              (m21.Momentum() + m22.Momentum() - in1.Momentum() - in2.Momentum()) *
                  (m21.Momentum() + m22.Momentum() - in1.Momentum() - in2.Momentum());
  if (abs(mm1 - mx) < abs(mm2 - mx))
    return mm1;
  else
    return mm2;
}

void* ATrackHistoAlgorithm::process(void* ptr)
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  int offs1 = 0;
  sLine3D path1, path2;
  ODhistos[startO + numOD - 4]->Fill(numberOfTracks);
  int n1 = 0, n2 = 0, n3 = 0, nc = 0;
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->isSecondary())
      n2++;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 0)
      n1++;
    else if (tracks[i]->getNumberOfSecondaryTracks() == 1)
      nc++;
    else
      n3++;
  }
  ODhistos[startO + numOD - 2]->Fill(n1);
  ODhistos[startO + numOD - 3]->Fill(nc);
  ODhistos[startO + numOD - 5]->Fill(n3);
  for (int i = 0; i < numberOfTracks; i++) {
    ODhistos[startO]->Fill(tracks[i]->getPath().Direction().Theta());
    ODhistos[startO + 1]->Fill(tracks[i]->getPath().Direction().Phi());
    ODhistos[startO + 2]->Fill(tracks[i]->getSpeed());
  }
  TPixel* pix;
  //  cout<<numD<<endl;
  for (int i = 0; i < numD; i++) {
    //      cout<<i<<" "<<nTracks[i]<<endl;
    if (nTracks[i] > 0) {
      int m;
      if (nTracks[i] == n1) {
        //	      cout<<nTracks[i]<<" found"<<endl;
        m = 0;
        offs1 = st2[i];
        for (int j = 0; j < numberOfTracks; j++) {
          if (tracks[j]->isSecondary() || tracks[j]->getNumberOfSecondaryTracks() > 0)
            continue;
          path1 = tracks[j]->getPath();
          ODhistos[startO + st1[i]]->Fill(tracks[j]->getNumberOfCalibHits());
          ODhistos[startO + st1[i] + 3 * j + 1]->Fill(path1.Direction().Phi());
          ODhistos[startO + st1[i] + 3 * j + 2]->Fill(path1.Direction().Theta());
          ODhistos[startO + st1[i] + 3 * j + 3]->Fill(tracks[j]->getSpeed());
          TDhistos[startT + offs1 + j]->Fill(path1.Foot().X(), path1.Foot().Y());
          for (int k = 0; k < j; k++) {
            if (tracks[k]->isSecondary() || tracks[k]->getNumberOfSecondaryTracks() > 0)
              continue;
            path2 = tracks[k]->getPath();
            TDhistos[startT + offs1 + j + k + 1]->Fill(
                abs(path1.Direction().Phi() - path2.Direction().Phi()),
                path1.Direction().Theta() + path2.Direction().Theta());
          }
          if (tracks[j]->getNumberOfPixels() == 0 || tracks[j]->getNumberOfCalibHits() == 0)
            continue;
          pix = &tracks[j]->getPixel(0, 0);
          switch (pix->getID()) {
          case 0:
            break;
          case 1:
            break;
          case 2:
            TDhistos[startT + numTD - 2]->Fill(
                abs(pix->getShape()->getCenter().Theta() - path1.Direction().Theta()),
                tracks[j]->getNumberOfCalibHits());
            TDhistos[startT + numTD - 1]->Fill(
                abs(pix->getShape()->getCenter().Phi() - path1.Direction().Phi()),
                tracks[j]->getNumberOfCalibHits());
            break;
          }
          offs1 = offs1 + m;
          m++;
        }
      }
    } else if (nTracks[i] == -1) {
      for (int k = 0; k < numberOfTracks; k++) {
        if (!tracks[k]->isSecondary() && tracks[k]->getNumberOfSecondaryTracks() == 2 &&
            tracks[k]->getCharge() == 0) {
          vector3D v = tracks[k]->getPath().Direction(),
                   v1 = tracks[k]->getSecondary(0)->getPath().Direction(),
                   v2 = tracks[k]->getSecondary(1)->getPath().Direction();
          ODhistos[startO + st1[i]]->Fill(v.Phi());
          ODhistos[startO + st1[i] + 1]->Fill(v.Theta());
          ODhistos[startO + st1[i] + 2]->Fill(v1.Phi());
          ODhistos[startO + st1[i] + 3]->Fill(v1.Theta());
          ODhistos[startO + st1[i] + 4]->Fill(v2.Phi());
          ODhistos[startO + st1[i] + 5]->Fill(v2.Theta());
          ODhistos[startO + st1[i] + 6]->Fill(acos(v2 * v1 / v1.R() / v2.R()));
          ODhistos[startO + st1[i] + 7]->Fill(acos(v * v1 / v1.R() / v.R()));
          ODhistos[startO + st1[i] + 8]->Fill(acos(v2 * v / v.R() / v2.R()));
          ODhistos[startO + st1[i] + 9]->Fill(tracks[k]->getSpeed());
          ODhistos[startO + st1[i] + 10]->Fill(tracks[k]->getSecondary(0)->getSpeed());
          ODhistos[startO + st1[i] + 11]->Fill(tracks[k]->getSecondary(1)->getSpeed());
          ODhistos[startO + st1[i] + 12]->Fill(tracks[k]->getSecondary(1)->getPath().Foot().Z());
          TDhistos[startT + st2[i]]->Fill(tracks[k]->getPath().Foot().X(),
                                          tracks[k]->getPath().Foot().Y());
          TDhistos[startT + st2[i] + 1]->Fill(tracks[k]->getSecondary(1)->getPath().Foot().X(),
                                              tracks[k]->getSecondary(1)->getPath().Foot().Y());
        }
      }
    } else if (nTracks[i] == -2) {
      for (int k = 0; k < numberOfTracks; k++) {
        if (!tracks[k]->isSecondary() && tracks[k]->getNumberOfSecondaryTracks() >= 2 &&
            abs(tracks[k]->getCharge()) >= 1) {
          vector3D v = tracks[k]->getPath().Direction(),
                   v1 = tracks[k]->getSecondary(0)->getPath().Direction();
          ODhistos[startO + st1[i]]->Fill(v.Phi());
          ODhistos[startO + st1[i] + 1]->Fill(v.Theta());
          ODhistos[startO + st1[i] + 2]->Fill(v1.Phi());
          ODhistos[startO + st1[i] + 3]->Fill(v1.Theta());
          ODhistos[startO + st1[i] + 4]->Fill(acos(v * v1 / v1.R() / v.R()));
          ODhistos[startO + st1[i] + 5]->Fill(tracks[k]->getSpeed());
          ODhistos[startO + st1[i] + 6]->Fill(tracks[k]->getSecondary(0)->getSpeed());
          ODhistos[startO + st1[i] + 7]->Fill(tracks[k]->getSecondary(0)->getPath().Foot().Z());
          TDhistos[startT + st2[i]]->Fill(tracks[k]->getPath().Foot().X(),
                                          tracks[k]->getPath().Foot().Y());
          TDhistos[startT + st2[i] + 1]->Fill(tracks[k]->getSecondary(0)->getPath().Foot().X(),
                                              tracks[k]->getSecondary(0)->getPath().Foot().Y());
        }
      }
    } else if (nTracks[i] == -3) {
      for (int k = 0; k < numberOfTracks; k++) {
        if (!tracks[k]->isSecondary() && tracks[k]->getNumberOfSecondaryTracks() <= 0) {
          vector3D v = tracks[k]->getPath().Direction();
          ODhistos[startO + st1[i] + 2]->Fill(v.Phi());
          ODhistos[startO + st1[i]]->Fill(v.Theta());
          ODhistos[startO + st1[i] + 1]->Fill(tracks[k]->getSpeed());
          TDhistos[startT + st2[i]]->Fill(tracks[k]->getPath().Foot().X(),
                                          tracks[k]->getPath().Foot().Y());
        }
      }
    }
  }
  for (int i = 0; i < numberOfTracks; i++) {
    if (tracks[i]->getNumberOfCalibHits(0) > 0)
      radpix0.Fill(tracks[i]->getPath().Direction().Theta(),
                   tracks[i]->getCalibSrcr(0, 0)->getTDC());
    if (tracks[i]->getNumberOfCalibHits(3) > 0)
      radpix3.Fill(tracks[i]->getPath().Direction().Theta(),
                   tracks[i]->getCalibSrcr(3, 0)->getTDC());
    if (tracks[i]->getNumberOfCalibHits(15) > 0)
      radpix15.Fill(tracks[i]->getPath().Direction().Theta(),
                    tracks[i]->getCalibSrcr(15, 0)->getTDC());
    if (tracks[i]->getNumberOfCalibHits(16) > 0)
      radpix16.Fill(tracks[i]->getPath().Direction().Theta(),
                    tracks[i]->getCalibSrcr(16, 0)->getTDC());
  }
  if (n1 == 2) //&&n2>0)
  {
    vector3D d1 = tracks[0]->getPath().Direction(), d2 = tracks[1]->getPath().Direction();
    if (abs(abs(d1.Phi() - d2.Phi()) - M_PI) > 0.1) {
      int i = 0;
      int j = 1;
      momentum4D pi1, pi2, pI;
      pi1.setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
      pi2.setPM(vector3D(0, 0, 3.059), Eparticles::getMass("proton"));
      pI = pi1 + pi2;
      matrixNxM A(3, 2);
      Vector pi(3);
      Vector px(2);
      pi.transpose();
      px.transpose();
      pi.setValues(pI.X(), pI.Y(), pI.Z());
      A.setValue(0, 0, d1.X());
      A.setValue(1, 0, d1.Y());
      A.setValue(2, 0, d1.Z());
      A.setValue(0, 1, d2.X());
      A.setValue(1, 1, d2.Y());
      A.setValue(2, 1, d2.Z());
      px = (-((!A) * A)) * ((!A) * pi);
      pi1.setPM(d1 * px.getValue(0), Eparticles::getMass("proton"));
      pi2.setPM(d2 * px.getValue(1), Eparticles::getMass("proton"));
      betaHisto.Fill(pi1.Velocity().length(), tracks[0]->getSpeed());
      betaHisto.Fill(pi2.Velocity().length(), tracks[1]->getSpeed());
      pi1.setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
      pi2.setPM(vector3D(0, 0, 3.059), Eparticles::getMass("proton"));
      pI = pi1 + pi2;
      momentum4D p11, p12, p21, p22;
      p11.setVM(tracks[i]->getPath().Direction() * tracks[i]->getSpeed(),
                Eparticles::getMass("proton"));
      //	  p21.setVM(tracks[i]->getPath().Direction()*tracks[i]->getSpeed(),Eparticles::getMass("kaon
      //+"));
      p12.setVM(tracks[j]->getPath().Direction() * tracks[j]->getSpeed(),
                Eparticles::getMass("kaon +"));
      //	  p22.setVM(tracks[j]->getPath().Direction()*tracks[j]->getSpeed(),Eparticles::getMass("proton"));
      momentum4D r1 = pI - (p11 + p12); //,r2=pI-(p21+p22);
      //	  if(abs(r1.Mass()-Eparticles::getMass("lambda"))<abs(r2.Mass()-Eparticles::getMass("lambda")))
      ODhistos[startO + numOD - 1]->Fill(r1.Mass());
      //	  else
      //	    ODhistos[startO+numOD-1]->Fill(r2.Mass());
    }
  }
  return ptr;
}
#include <fstream>
ifstream input("fitrun4992.txt");
AReactionHistoAlgorithm::AReactionHistoAlgorithm(TEvent* EventIn, TDirectory* parent,
                                                 const string& opt, bool jf)
{
  event = EventIn;
  char li[100];
  input.getline(li, 100);
  input.getline(li, 100);
  justFill = jf;
  numOD = 1;
  numTD = 0;
  options = opt.data();
  dir = parent->mkdir("Reactions");
  int n = 0;
  if (hasA(opt, "_ppel_"))
    n++;
  if (hasA(opt, "_dpi+_"))
    n++;
  if (hasA(opt, "_pp-pkl-pkppi_"))
    n++;
  numD = n;
  reactions = new int[n];
  st1 = new int[n];
  st2 = new int[n];
  n = 0;
  if (hasA(opt, "_ppel_")) {
    dir->mkdir("pp-elastic");
    reactions[n] = 0;
    st1[n] = numOD;
    st2[n] = numTD;
    n++;
    numOD += 15;
    numTD += 1 + 2;
  }
  if (hasA(opt, "_pp-pkl-pkppi_")) {
    dir->mkdir("pklambda");
    reactions[n] = 10;
    st1[n] = numOD;
    st2[n] = numTD;
    n++;
    numOD += 5 * 3 + 6 + 5;
    numTD += 2;
  }
  if (hasA(opt, "_dpi+_")) {
    dir->mkdir("dpi+");
    reactions[n] = 1;
    st1[n] = numOD;
    st2[n] = numTD;
    n++;
    numOD += 15;
    numTD += 4;
  }
}
AReactionHistoAlgorithm::~AReactionHistoAlgorithm()
{
  for (int i = 0; i < numOD; i++) {
    delete ODhistos[startO + i];
    ODhistos[startO + i] = NULL;
  }
  for (int i = 0; i < numTD; i++) {
    if (TDhistos[startT + i])
      delete TDhistos[startT + i];
    TDhistos[startT + i] = NULL;
  }
  delete[] reactions;
  delete[] st1;
  delete[] st2;
}
int AReactionHistoAlgorithm::declareHistos(TH1** histos, int start)
{
  ODhistos = histos;
  startO = start;
  int n = 0;
  if (!justFill) {
    ODhistos[startO] = new TH1F("reactions", "reactions", numD + 5, -1, numD);
    for (int i = 0; i < numD; i++) {
      switch (reactions[i]) {
      case 0: {
        ODhistos[startO + st1[i]] = new TH1F("pp-elastic: Missing mass squared",
                                             "pp-elastic: Missing mass squared", 100, -1, 1);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("pp-elastic: Invariant mass", "pp-elastic: Invariant mass", 1000, 1.5, 5);
        ODhistos[startO + st1[i] + 2] = new TH1F("pp-elastic: \\Theta_{proton1}",
                                                 "pp-elastic: \\Theta_{proton1}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 3] = new TH1F("pp-elastic: \\Theta_{proton2}",
                                                 "pp-elastic: \\Theta_{proton2}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 4] =
            new TH1F("pp-elastic: \\Theta_{proton}", "pp-elastic: \\Theta_{proton}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 5] = new TH1F("pp-elastic: \\phi_{proton1}",
                                                 "pp-elastic: \\phi_{proton1}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 6] = new TH1F("pp-elastic: \\phi_{proton2}",
                                                 "pp-elastic: \\phi_{proton2}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 7] =
            new TH1F("pp-elastic: \\phi_{proton}", "pp-elastic: \\phi_{proton}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 8] =
            new TH1F("pp-elastic: \\beta_{proton1}", "pp-elastic: \\beta_{proton1}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 9] =
            new TH1F("pp-elastic: \\beta_{proton2}", "pp-elastic: \\beta_{proton2}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 10] =
            new TH1F("pp-elastic: \\beta_{proton}", "pp-elastic: \\beta_{proton}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 11] = new TH1F("pp-elastic: \\Theta_{proton} CM",
                                                  "pp-elastic: \\Theta_{proton} CM", 180, 0, 180);
        ODhistos[startO + st1[i] + 12] = new TH1F("pp-elastic: \\beta_{proton} CM",
                                                  "pp-elastic: \\beta_{proton} CM", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 13] =
            new TH1F("pp-elastic: \\beta_{proton, measured} - \\beta_{proton, "
                     "reconstructed}",
                     "pp-elastic: \\beta_{proton, measured} - \\beta_{proton, "
                     "reconstructed}",
                     1000, -2, 2);
        ODhistos[startO + st1[i] + 14] =
            new TH1F("pp-elastic: \\beta_{proton, measured}",
                     "pp-elastic: \\beta_{proton, measured}", 1000, 0, 2);
        n += 15;
        break;
      }
      case 1: {
        ODhistos[startO + st1[i]] =
            new TH1F("dpi+: Missing mass squared", "dpi+: Missing mass squared", 100, -1, 1);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("dpi+: Invariant mass", "dpi+: Invariant mass", 1000, 1.5, 5);
        ODhistos[startO + st1[i] + 2] =
            new TH1F("dpi+: \\Theta_{deuteron}", "dpi+: \\Theta_{deuteron}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 3] =
            new TH1F("dpi+: \\Theta_{pion}", "dpi+: \\Theta_{pion}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 4] =
            new TH1F("dpi+: \\Theta_{any}", "dpi+: \\Theta_{any}", 1000, 0, M_PI);
        ODhistos[startO + st1[i] + 5] =
            new TH1F("dpi+: \\phi_{deuteron}", "dpi+: \\phi_{deuteron}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 6] =
            new TH1F("dpi+: \\phi_{pion}", "dpi+: \\phi_{pion}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 7] =
            new TH1F("dpi+: \\phi_{any}", "dpi+: \\phi_{any}", 1000, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 8] =
            new TH1F("dpi+: \\beta_{deuteron}", "dpi+: \\beta_{deuteron}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 9] =
            new TH1F("dpi+: \\beta_{pion}", "dpi+: \\beta_{pion}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 10] =
            new TH1F("dpi+: \\beta_{any}", "dpi+: \\beta_{any}", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 11] =
            new TH1F("dpi+: \\Theta_{deueron} CM", "dpi+: \\Theta_{deuteron} CM", 180, 0, 180);
        ODhistos[startO + st1[i] + 12] =
            new TH1F("dpi+: \\beta_{deuteron} CM", "dpi+: \\beta_{deuteron} CM", 1000, 0, 1.2);
        ODhistos[startO + st1[i] + 13] = new TH1F(
            "dpi+: \\beta_{proton, measured} - \\beta_{proton, reconstructed}",
            "dpi+: \\beta_{proton, measured} - \\beta_{proton, reconstructed}", 1000, -2, 2);
        ODhistos[startO + st1[i] + 14] = new TH1F("dpi+: \\beta_{deuteron, measured}",
                                                  "dpi+: \\beta_{deuteron, measured}", 1000, 0, 2);
        n += 15;
        break;
      }
      case 10: {
        ODhistos[startO + st1[i]] =
            new TH1F("pK\\Lambda: \\theta_{proton}", "pK\\Lambda: \\theta_{proton}", 100, 0, M_PI);
        ODhistos[startO + st1[i] + 1] =
            new TH1F("pK\\Lambda: \\phi_{proton}", "pK\\Lambda: \\phi_{proton}", 100, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 2] =
            new TH1F("pK\\Lambda: \\beta_{proton}", "pK\\Lambda: \\beta_{proton}", 100, 0, 1.2);
        ODhistos[startO + st1[i] + 3] =
            new TH1F("pK\\Lambda: \\theta_{kaon}", "pK\\Lambda: \\theta_{kaon}", 100, 0, M_PI);
        ODhistos[startO + st1[i] + 4] =
            new TH1F("pK\\Lambda: \\phi_{kaon}", "pK\\Lambda: \\phi_{kaon}", 100, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 5] =
            new TH1F("pK\\Lambda: \\beta_{kaon}", "pK\\Lambda: \\beta_{kaon}", 100, 0, 1.2);
        ODhistos[startO + st1[i] + 6] = new TH1F("pK\\Lambda: \\theta_{\\Lambda}",
                                                 "pK\\Lambda: \\theta_{\\Lambda}", 100, 0, M_PI);
        ODhistos[startO + st1[i] + 7] = new TH1F("pK\\Lambda: \\phi_{\\Lambda}",
                                                 "pK\\Lambda: \\phi_{\\Lambda}", 100, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 8] =
            new TH1F("pK\\Lambda: \\beta_{\\Lambda}", "pK\\Lambda: \\beta_{\\Lambda}", 100, 0, 1.2);
        ODhistos[startO + st1[i] + 9] =
            new TH1F("pK\\Lambda: \\theta_{decay proton}", "pK\\Lambda: \\theta_{decay proton}",
                     100, 0, M_PI);
        ODhistos[startO + st1[i] + 10] =
            new TH1F("pK\\Lambda: \\phi_{decay proton}", "pK\\Lambda: \\phi_{decay proton}", 100,
                     -M_PI, M_PI);
        ODhistos[startO + st1[i] + 11] = new TH1F("pK\\Lambda: \\beta_{decay proton}",
                                                  "pK\\Lambda: \\beta_{decay proton}", 100, 0, 1.2);
        ODhistos[startO + st1[i] + 12] = new TH1F("pK\\Lambda: \\theta_{decay pion}",
                                                  "pK\\Lambda: \\theta_{decay pion}", 100, 0, M_PI);
        ODhistos[startO + st1[i] + 13] = new TH1F(
            "pK\\Lambda: \\phi_{decay pion}", "pK\\Lambda: \\phi_{decay pion}", 100, -M_PI, M_PI);
        ODhistos[startO + st1[i] + 14] = new TH1F("pK\\Lambda: \\beta_{decay pion}",
                                                  "pK\\Lambda: \\beta_{decay pion}", 100, 0, 1.2);
        ODhistos[startO + st1[i] + 15] = new TH1F("pk\\Lambda: Invariant Mass pK",
                                                  "pk\\Lambda: Invariant Mass pK", 30, 1.4, 1.7);
        ODhistos[startO + st1[i] + 16] =
            new TH1F("pk\\Lambda: Invariant Mass p\\Lambda", "pk\\Lambda: Invariant Mass p\\Lambda",
                     30, 2.02, 2.32); // 2,4);
        ODhistos[startO + st1[i] + 17] =
            new TH1F("pk\\Lambda: Invariant Mass \\Lambda K",
                     "pk\\Lambda: Invariant Mass \\Lambda K", 30, 1.58, 1.88); // 1,2);
        ODhistos[startO + st1[i] + 18] =
            new TH1F("pk\\Lambda: Invariant Mass decay particles",
                     "pk\\Lambda: Invariant Mass decay particles", 30, 0.9, 2);
        ODhistos[startO + st1[i] + 19] =
            new TH1F("pk\\Lambda: Missing Mass pK", "pk\\Lambda: Missing Mass pK", 100, 0.7, 1.5);
        ODhistos[startO + st1[i] + 20] =
            new TH1F("pk\\Lambda: Missing Mass pK\\Lambda", "pk\\Lambda: Missing Mass pK\\lambda",
                     100, -0.2, 0.2);
        ODhistos[startO + st1[i] + 21] =
            new TH1F("pK\\Lambda: cos(\\theta_{proton}) (CMS)",
                     "pK\\Lambda: cos(\\theta_{proton}) (CMS)", 30, -1, 1);
        ODhistos[startO + st1[i] + 22] =
            new TH1F("pK\\Lambda: cos(\\theta_{kaon}) (CMS)",
                     "pK\\Lambda: cos(\\theta_{kaon}) (CMS)", 30, -1, 1);
        ODhistos[startO + st1[i] + 23] =
            new TH1F("pK\\Lambda: cos(\\theta_{\\Lambda}) (CMS)",
                     "pK\\Lambda: cos(\\theta_{\\lambda}) (CMS)", 30, -1, 1);
        ODhistos[startO + st1[i] + 24] =
            new TH1F("pK\\lambda: cos(\\theta_{decay proton}) (CMS)",
                     "pK\\Lambda: cos(\\theta_{decay proton}) (CMS)", 30, -1, 1);
        ODhistos[startO + st1[i] + 25] =
            new TH1F("pK\\lambda: cos(\\theta_{decay pion}) (CMS)",
                     "pK\\Lambda: cos(\\theta_{decay pion}) (CMS)", 30, -1, 1);
        n += 26;
        break;
      }
      }
    }
  }
  return n;
}
int AReactionHistoAlgorithm::declare2DHistos(TH2** histos, int start)
{
  TDhistos = histos;
  startT = start;
  //   for(int i=0;i<numTD;i++)
  //     {
  //       TDhistos[i]=NULL;
  //     }
  if (!justFill) {
    for (int i = 0; i < numD; i++) {
      switch (reactions[i]) {
      case 0: {
        TDhistos[startT + st2[i]] =
            new TH2F("pp-elastic: transverse vs. parallel momentum",
                     "pp-elastic: transverse vs. parallel momentum", 100, 0, 4, 100, 0, 1.5);
        TDhistos[startT + st2[i] + 1] =
            new TH2F("pp-elastic: \\Delta\\beta_{measured-reconstructed} vs. \\vartheta",
                     "pp-elastic: \\Delta\\beta_{measured-reconstructed} vs. \\vartheta", 100, 0,
                     M_PI, 100, -0.5, 0.5);
        TDhistos[startT + st2[i] + 2] =
            new TH2F("pp-elastic: \\Delta\\beta_{measured-reconstructed} vs. \\phi",
                     "pp-elastic: \\Delta\\beta_{measured-reconstructed} vs. \\phi", 100, -M_PI,
                     M_PI, 100, -0.5, 0.5);
        break;
      }
      case 1: {
        TDhistos[startT + st2[i]] =
            new TH2F("dpi+: transverse vs. parallel momentum",
                     "dpi+: transverse vs. parallel momentum", 100, 0, 4, 100, 0, 1.5);
        TDhistos[startT + st2[i] + 1] =
            new TH2F("dpi+: \\vartheta_{pion} vs. \\vartheta_{deuteron}",
                     "dpi+: \\vartheta_{pion} vs. \\vartheta_{deuteron}", 100, 0, 0.5, 100, 0, 1.1);
        TDhistos[startT + st2[i] + 2] =
            new TH2F("dpi+: \\beta_{deuteron} vs. \\vartheta_{deuteron}",
                     "dpi+: \\beta_{deuteron} vs. \\vartheta_{deuteron}", 100, 0, 0.5, 100, 0, 2.0);
        TDhistos[startT + st2[i] + 3] =
            new TH2F("dpi+: \\beta_{pion} vs. \\vartheta_{pion}",
                     "dpi+: \\beta_{pion} vs. \\vartheta_{pion}", 100, 0, 1.1, 100, 0, 2.0);
        break;
      }
      case 10: {
        TDhistos[startT + st2[i]] = new TH2F("pk\\lambda: Darliz plot", "pk\\lambda: Darliz plot",
                                             100, 1.4, 1.7, 100, 2.2, 2.4);
        TDhistos[startT + st2[i] + 1] =
            new TH2F("pk\\lambda: transverse vs. parallel momentum",
                     "pk\\lambda: transverse vs. parallel momentum", 100, 0, 4, 100, 0, 1.5);
        break;
      }
      }
    }
  }
  return numTD;
}
#include <TLorentzVector.h>
void* AReactionHistoAlgorithm::process(void* ptr)
{
  if (event->getNumberOfReactions() <= 0) {
    ODhistos[startO]->Fill(-1);
    return ptr;
  }
  for (int i = 0; i < numD; i++) {
    switch (reactions[i]) {
    case 0: {
      if ((*event).getReaction(0)->name() == "Rppelastic") {
        ODhistos[startO]->Fill(i);
        Rppelastic* react = (Rppelastic*)((*event).getReaction(0));
        momentum4D p1, p2;
        momentum4D pi1, pi2;
        momentum4D pc1, pc2;
        momentum4D tmp;
        p1 = react->getParticle(0);
        p2 = react->getParticle(1);
        pi1 = react->getInitParticle(0);
        pi2 = react->getInitParticle(1);
        // 		pi1.setPM(vector3D(0,0,3.059),Eparticles::getMass("proton"));
        // 		pi2.setPM(vector3D(0,0,0),Eparticles::getMass("proton"));
        // 		float f1,f2,f3,f4,f5;
        // 		int i1,i2,i3,i4,i5;
        // 		char li[100];
        // 		long l1;
        // 		input>>l1>>i3>>f4>>f5;
        // 		input.getline(li,100);
        // 		input.getline(li,100);
        // 		input>>i1>>f1>>f2>>f3>>i2;
        // 		p1.setPM(vector3D(f1,f2,f3),Eparticles::getMassG(i2));
        // 		input>>i1>>f1>>f2>>f3>>i2;
        // 		p2.setPM(vector3D(f1,f2,f3),Eparticles::getMassG(i2));
        // 		input>>i1;
        //	if(f5>2.0)break;
        momentum4D cm = momentum4D::CM(pi1, pi2);
        vector3D vel = cm.Velocity() * -1;
        pc1 = p1.boost(vel);
        pc2 = p2.boost(vel);
        tmp = pi1 + pi2 - (p1 + p2);
        ODhistos[startO + st1[i]]->Fill(tmp.Energy() * tmp.Energy() - (tmp.R() * tmp.R()));
        tmp = p1 + p2;
        ODhistos[startO + st1[i] + 1]->Fill(sqrt(tmp.Energy() * tmp.Energy() - tmp.R() * tmp.R()));
        vector3D d = vector3D(0, 0, 1) * (p1.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i]]->Fill(d.length(), (p1.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (p2.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i]]->Fill(d.length(), (p2.Momentum() - d).length());
        ODhistos[startO + st1[i] + 4]->Fill(p1.Momentum().Theta());
        ODhistos[startO + st1[i] + 4]->Fill(p2.Momentum().Theta());
        ODhistos[startO + st1[i] + 10]->Fill(p1.Velocity().length());
        ODhistos[startO + st1[i] + 10]->Fill(p2.Velocity().length());
        ODhistos[startO + st1[i] + 7]->Fill(p1.Momentum().Phi());
        ODhistos[startO + st1[i] + 7]->Fill(p2.Momentum().Phi());
        ODhistos[startO + st1[i] + 12]->Fill(pc1.Velocity().length());
        ODhistos[startO + st1[i] + 12]->Fill(pc2.Velocity().length());
        ODhistos[startO + st1[i] + 11]->Fill((pc1.Momentum().Theta()) * 180 / M_PI);
        ODhistos[startO + st1[i] + 11]->Fill((pc2.Momentum().Theta() * 180 / M_PI));
        ODhistos[startO + st1[i] + 13]->Fill(react->getTrack(0)->getSpeed() -
                                             p1.Velocity().length());
        ODhistos[startO + st1[i] + 13]->Fill(react->getTrack(1)->getSpeed() -
                                             p2.Velocity().length());
        ODhistos[startO + st1[i] + 14]->Fill(react->getTrack(0)->getSpeed());
        ODhistos[startO + st1[i] + 14]->Fill(react->getTrack(1)->getSpeed());
        TDhistos[startT + st2[i] + 1]->Fill(p1.Momentum().Theta(), react->getTrack(0)->getSpeed() -
                                                                       p1.Velocity().length());
        TDhistos[startT + st2[i] + 2]->Fill(p1.Momentum().Phi(), react->getTrack(0)->getSpeed() -
                                                                     p1.Velocity().length());
        TDhistos[startT + st2[i] + 1]->Fill(p2.Momentum().Theta(), react->getTrack(1)->getSpeed() -
                                                                       p2.Velocity().length());
        TDhistos[startT + st2[i] + 2]->Fill(p2.Momentum().Phi(), react->getTrack(1)->getSpeed() -
                                                                     p2.Velocity().length());
        dirHisto.Fill(acos(pc1.Direction() * pc2.Direction()) * 180 / M_PI);
        if (p2.Momentum().Theta() < p1.Momentum().Theta()) {
          ODhistos[startO + st1[i] + 3]->Fill(p1.Momentum().Theta());
          ODhistos[startO + st1[i] + 2]->Fill(p2.Momentum().Theta());
          ODhistos[startO + st1[i] + 6]->Fill(p1.Momentum().Phi());
          ODhistos[startO + st1[i] + 5]->Fill(p2.Momentum().Phi());
          ODhistos[startO + st1[i] + 9]->Fill(p1.Velocity().length());
          ODhistos[startO + st1[i] + 8]->Fill(p2.Velocity().length());
        } else {
          ODhistos[startO + st1[i] + 2]->Fill(p1.Momentum().Theta());
          ODhistos[startO + st1[i] + 3]->Fill(p2.Momentum().Theta());
          ODhistos[startO + st1[i] + 5]->Fill(p1.Momentum().Phi());
          ODhistos[startO + st1[i] + 6]->Fill(p2.Momentum().Phi());
          ODhistos[startO + st1[i] + 8]->Fill(p1.Velocity().length());
          ODhistos[startO + st1[i] + 9]->Fill(p2.Velocity().length());
        }
      }
      break;
    }
    case 1: {
      if ((*event).getReaction(0)->name() == "Rpp_dpiPlus") {
        ODhistos[startO]->Fill(i);
        Rpp_dpiPlus* react = (Rpp_dpiPlus*)((*event).getReaction(0));
        momentum4D p1, p2;
        momentum4D pi1, pi2;
        momentum4D pc1, pc2;
        momentum4D tmp;
        p1 = react->getParticle(0);
        p2 = react->getParticle(1);
        pi1 = react->getInitParticle(0);
        pi2 = react->getInitParticle(1);
        momentum4D cm = momentum4D::CM(pi1, pi2);
        vector3D vel = cm.Velocity() * -1;
        pc1 = p1.boost(vel);
        pc2 = p2.boost(vel);
        tmp = pi1 + pi2 - (p1 + p2);
        TDhistos[startT + st2[i] + 1]->Fill(react->getTrack(0)->getPath().Direction().Theta(),
                                            react->getTrack(1)->getPath().Direction().Theta());
        TDhistos[startT + st2[i] + 2]->Fill(react->getTrack(0)->getPath().Direction().Theta(),
                                            react->getTrack(0)->getSpeed());
        TDhistos[startT + st2[i] + 3]->Fill(react->getTrack(1)->getPath().Direction().Theta(),
                                            react->getTrack(1)->getSpeed());
        ODhistos[startO + st1[i]]->Fill(tmp.Energy() * tmp.Energy() - (tmp.R() * tmp.R()));
        tmp = p1 + p2;
        ODhistos[startO + st1[i] + 1]->Fill(sqrt(tmp.Energy() * tmp.Energy() - tmp.R() * tmp.R()));
        vector3D d = vector3D(0, 0, 1) * (p1.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i]]->Fill(d.length(), (p1.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (p2.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i]]->Fill(d.length(), (p2.Momentum() - d).length());
        ODhistos[startO + st1[i] + 4]->Fill(p1.Momentum().Theta());
        ODhistos[startO + st1[i] + 4]->Fill(p2.Momentum().Theta());
        ODhistos[startO + st1[i] + 10]->Fill(p1.Velocity().length());
        ODhistos[startO + st1[i] + 10]->Fill(p2.Velocity().length());
        ODhistos[startO + st1[i] + 7]->Fill(p1.Momentum().Phi());
        ODhistos[startO + st1[i] + 7]->Fill(p2.Momentum().Phi());
        ODhistos[startO + st1[i] + 12]->Fill(pc1.Velocity().length());
        ODhistos[startO + st1[i] + 12]->Fill(pc2.Velocity().length());
        ODhistos[startO + st1[i] + 11]->Fill((pc1.Momentum().Theta()) * 180 / M_PI);
        ODhistos[startO + st1[i] + 11]->Fill((pc2.Momentum().Theta() * 180 / M_PI));
        ODhistos[startO + st1[i] + 13]->Fill(p1.Velocity().length() -
                                             react->getTrack(0)->getSpeed());
        ODhistos[startO + st1[i] + 13]->Fill(p2.Velocity().length() -
                                             react->getTrack(1)->getSpeed());
        ODhistos[startO + st1[i] + 14]->Fill(react->getTrack(0)->getSpeed());
        ODhistos[startO + st1[i] + 14]->Fill(react->getTrack(1)->getSpeed());
        ODhistos[startO + st1[i] + 2]->Fill(p1.Momentum().Theta());
        ODhistos[startO + st1[i] + 3]->Fill(p2.Momentum().Theta());
        ODhistos[startO + st1[i] + 5]->Fill(p1.Momentum().Phi());
        ODhistos[startO + st1[i] + 6]->Fill(p2.Momentum().Phi());
        ODhistos[startO + st1[i] + 8]->Fill(p1.Velocity().length());
        ODhistos[startO + st1[i] + 9]->Fill(p2.Velocity().length());
      }
      break;
    }
    case 10: {
      ODhistos[startO]->Fill(i);
      if (event->getReaction(0)->name() == "Rpp_pkl_pkppi") {
        Rpp_pkl_pkppi* react = (Rpp_pkl_pkppi*)(*event).getReaction(0);
        momentum4D pi1 = react->getInitParticle(0), pi2 = react->getInitParticle(1);
        momentum4D p = react->getPromptProton();
        momentum4D k = react->getKaon();
        momentum4D l = react->getLambda();
        momentum4D dp = react->getDecayProton();
        momentum4D dpi = react->getDecayPion();
        momentum4D pc[5];
        for (int j = 0; j < 5; j++)
          pc[j] = react->getCMParticle(j);
        ODhistos[startO + st1[i]]->Fill(p.Direction().Theta());
        ODhistos[startO + st1[i] + 1]->Fill(p.Direction().Phi());
        ODhistos[startO + st1[i] + 2]->Fill(p.Velocity().length());
        ODhistos[startO + st1[i] + 3]->Fill(k.Direction().Theta());
        ODhistos[startO + st1[i] + 4]->Fill(k.Direction().Phi());
        ODhistos[startO + st1[i] + 5]->Fill(k.Velocity().length());
        ODhistos[startO + st1[i] + 6]->Fill(l.Direction().Theta());
        ODhistos[startO + st1[i] + 7]->Fill(l.Direction().Phi());
        ODhistos[startO + st1[i] + 8]->Fill(l.Velocity().length());
        ODhistos[startO + st1[i] + 9]->Fill(dp.Direction().Theta());
        ODhistos[startO + st1[i] + 10]->Fill(dp.Direction().Phi());
        ODhistos[startO + st1[i] + 11]->Fill(dp.Velocity().length());
        ODhistos[startO + st1[i] + 12]->Fill(dpi.Direction().Theta());
        ODhistos[startO + st1[i] + 13]->Fill(dpi.Direction().Phi());
        ODhistos[startO + st1[i] + 14]->Fill(dpi.Velocity().length());
        ODhistos[startO + st1[i] + 15]->Fill(sqrt((p + k) * (p + k)));
        ODhistos[startO + st1[i] + 16]->Fill(sqrt((p + l) * (p + l)));
        ODhistos[startO + st1[i] + 17]->Fill(sqrt((l + k) * (l + k)));
        ODhistos[startO + st1[i] + 18]->Fill(sqrt((dp + dpi) * (dp + dpi)));
        ODhistos[startO + st1[i] + 19]->Fill(sqrt(abs((pi1 + pi2 - p - k) * (pi1 + pi2 - p - k))));
        ODhistos[startO + st1[i] + 20]->Fill((pi1 + pi2 - p - k - l) * (pi1 + pi2 - p - k - l));
        TDhistos[startT + st2[i]]->Fill(sqrt((p + l) * (p + l)), sqrt((p + k) * (p + k)));
        vector3D d = vector3D(0, 0, 1) * (dpi.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i] + 1]->Fill(d.length(), (dpi.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (p.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i] + 1]->Fill(d.length(), (p.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (k.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i] + 1]->Fill(d.length(), (k.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (l.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i] + 1]->Fill(d.length(), (l.Momentum() - d).length());
        d = vector3D(0, 0, 1) * (dp.Momentum() * vector3D(0, 0, 1));
        TDhistos[startT + st2[i] + 1]->Fill(d.length(), (dp.Momentum() - d).length());
        for (int j = 0; j < 5; j++)
          ODhistos[startO + st1[i] + 21 + j]->Fill(cos(pc[j].Momentum().Theta()));
      }
      break;
    }
    default: {
      ODhistos[startO]->Fill(-1);
      break;
    }
    }
  }
  return ptr;
}
void AReactionHistoAlgorithm::write()
{
  //  int offs=0;
  int n1 = 0, n2 = 0;
  //   for(int i=0;i<numOD;i++)
  //     cout<<i<<" "<<ODhistos[startO+i]->GetName()<<endl;
  //   for(int i=0;i<numTD;i++)
  //     cout<<numTD<<" "<<i<<" "<<TDhistos[startT+i]->GetName()<<endl;
  int stB1, stB2;
  dir->cd();
  dirHisto.Write();
  for (int i = 0; i < numD; i++) {
    if (i == numD - 1) {
      stB1 = numOD + startO;
      stB2 = numTD + startT;
    } else {
      stB1 = st1[i + 1];
      stB2 = st2[i + 1];
    }
    n1 = stB1 - st1[i];
    n2 = stB2 - st2[i];
    switch (reactions[i]) {
    case 0:
      dir->cd("pp-elastic");
      // 	  n1=15;
      // 	  n2=3;
      break;
    case 1:
      dir->cd("dpi+");
      // 	  n1=15;
      // 	  n2=4;
      break;
    case 10:
      dir->cd("pklambda");
      // 	  n1=26;
      // 	  n2=2;
      break;
    }
    for (int j = 0; j < n1; j++) {
      //	  cout<<ODhistos[startO+st1[i]+j]->GetName()<<flush;
      ODhistos[startO + st1[i] + j]->Write();
      //	  cout<<" done"<<endl;
    }
    for (int j = 0; j < n2; j++) {
      //	  cout<<TDhistos[startT+st2[i]+j]->GetName()<<flush;
      if (TDhistos[startT + st2[i] + j])
        TDhistos[startT + st2[i] + j]->Write();
      //	  cout<<" done"<<endl;
    }
  }
  for (int i = 0; i < numTD; i++)
    TDhistos[startT + i]->SetDirectory(0);
  for (int i = 0; i < numOD; i++)
    ODhistos[startO + i]->SetDirectory(0);
}
