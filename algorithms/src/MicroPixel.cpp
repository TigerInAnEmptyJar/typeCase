#include "MicroPixel.h"
#include "logger.h"
#include "ring.h"
#include "sphericRectangle.h"
#include "wedge.h"
//#define OUTPUT
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#ifdef OUTPUT
#include <TCanvas.h>
#include <TH1.h>
TH1F histo_micro("Micro elements", "Micro elements", 100, 0, 99);
#endif
AMicroPixel::AMicroPixel(int maxPixelIn, int& numPixel, TPixel** pixelsIn, int** numHit,
                         int** numHitC, TCalibHit*** hits, THitCluster*** hc,
                         const algorithm_parameter& ap)
    : AAlgorithm("micro strip pixels"), maxPixel(maxPixelIn), numberOfPixels(numPixel)
{
  ID = ap.value(1).value<int>();
  pixels = pixelsIn;
  if (ap.value(0).value<bool>()) {
    hits1 = (TCalibHit**)hc[ap.value(2).value<int>()];
    hits2 = (TCalibHit**)hc[ap.value(3).value<int>()];
    numHits1 = numHitC[ap.value(2).value<int>()];
    numHits2 = numHitC[ap.value(3).value<int>()];
  } else {
    hits1 = hits[ap.value(2).value<int>()];
    hits2 = hits[ap.value(3).value<int>()];
    numHits1 = numHit[ap.value(2).value<int>()];
    numHits2 = numHit[ap.value(3).value<int>()];
  }
  for (int i = 0; i < maxPixel; i++)
    pixels[i]->setID(ID);
}
AMicroPixel::AMicroPixel(int IDIn, int maxPixelIn, int& numPixel, TPixel** pixelsIn, int& numHit1In,
                         int& numHit2In, TCalibHit** h1, TCalibHit** h2)
    : AAlgorithm("micro strip pixels"), maxPixel(maxPixelIn), numberOfPixels(numPixel), ID(IDIn),
      numHits1(&numHit1In), numHits2(&numHit2In)
{
  pixels = pixelsIn;
  hits1 = h1;
  hits2 = h2;
  for (int i = 0; i < maxPixel; i++)
    pixels[i]->setID(ID);
}

AMicroPixel::~AMicroPixel()
{
#ifdef OUTPUT
  TCanvas* canv = new TCanvas();
  canv->cd();
  histo_micro.Draw();
  canv->Print("MicroPixel.ps");
  delete canv;
#endif
}

void AMicroPixel::process()
{
  numberOfPixels = 0;
  for (int i = 0; i < *numHits1; i++) {
    if (!(hits1[i]->isValid()))
      continue;
    for (int j = 0; j < *numHits2; j++) {
      if (!(hits2[j]->isValid()))
        continue;
      if (numberOfPixels < maxPixel) {
        pixels[numberOfPixels]->reset();
        pixels[numberOfPixels]->setID(ID);
        pixels[numberOfPixels]->addElement(*hits1[i]);
        pixels[numberOfPixels]->addElement(*hits2[j]);
        volumeShape* shape1 = hits1[i]->getHitShape();
        volumeShape* shape2 = hits2[j]->getHitShape();
        if ((shape1->getName() == "wedge") && (shape2->getName() == "ring")) {
          // cout<<shape1->getName().ascii()<<"
          // "<<shape1->getCenter()<<shape1->getLowerPhiEdge()<<shape1->getNormal()<<shape2->getInnerRadius()<<"
          // "<<shape2->getOuterRadius()<<" "<<shape1->getPhiRange()<<endl;
          cout << hits2[j]->getElement() << ":"
               << atan(((ring*)shape2)->getInnerRadius() / shape1->getCenter().Z()) * 180. / M_PI
               << " " << flush;
          planeShape* sh = new sphericRectangle(
              shape1->getCenter(), ((wedge*)shape1)->getLowerPhiEdge(),
              ((wedge*)shape1)->getNormal(), ((ring*)shape2)->getOuterRadius(),
              ((ring*)shape2)->getInnerRadius(), -((wedge*)shape1)->getPhiRange());
          cout << sh->getCenter().Theta() * 180. / M_PI << endl;
#ifdef OUTPUT
          histo_micro.Fill(hits2[j]->getElement());
#endif
          planeShape* shh = pixels[numberOfPixels]->getShape();
          pixels[numberOfPixels]->setShape(sh);
          if (shh)
            delete shh;
          numberOfPixels++;
        } else if ((shape2->getName() == "wedge") && (shape1->getName() == "ring")) {
          // cout<<shape2->getName().ascii()<<"
          // "<<shape2->getCenter()<<shape2->getLowerPhiEdge()<<shape2->getNormal()<<shape1->getInnerRadius()<<"
          // "<<shape1->getOuterRadius()<<" "<<shape2->getPhiRange()<<endl;
          // cout<<hits1[i]->getElement()<<":"<<atan(((ring*)shape1)->getInnerRadius()/shape2->getCenter().Z())*180./M_PI<<"
          // "<<flush;
          planeShape* sh = new sphericRectangle(
              ((wedge*)shape2)->getCenter(), ((wedge*)shape2)->getLowerPhiEdge(),
              ((wedge*)shape2)->getNormal(), ((ring*)shape1)->getOuterRadius(),
              ((ring*)shape1)->getInnerRadius(), -((wedge*)shape2)->getPhiRange());
// cout<<sh->getCenter().Theta()*180./M_PI<<endl;
#ifdef OUTPUT
          histo_micro.Fill(hits1[i]->getElement());
#endif
          planeShape* shh = pixels[numberOfPixels]->getShape();
          pixels[numberOfPixels]->setShape(sh);
          if (shh)
            delete shh;
          numberOfPixels++;
        }
      } else {
        anaLog << "Too many Pixels found in UStrip" << endli;
        return;
      }
    }
  }
}
// void AMicroPixel::Streamer(TBuffer &b)
// {
// }
