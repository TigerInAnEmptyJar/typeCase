#include "eventpainter.h"
#include "TCrown.h"
#include "TCutG.h"
#include "TMarker.h"
#include "TPolyLine.h"
#include "TText.h"
#include <TEllipse.h>

EventPainter::EventPainter()
{
  Event = 0;
  canvas = 0;
  upperLeft.setValues(-400, -400);
  lowerRight.setValues(400, 400);
  trackLike = true;
  setPaintMode(1);
  setParticleMode(false);
}

EventPainter::EventPainter(TEvent* e, TPad* c)
{
  Event = e;
  canvas = c;
  upperLeft.setValues(-400, -400);
  lowerRight.setValues(400, 400);
  trackLike = true;
  setPaintMode(1);
  setParticleMode(false);
}

EventPainter::EventPainter(TEvent* e, TPad* c, float left, float right, float up, float down)
{
  Event = e;
  canvas = c;
  upperLeft.setValues(left, up);
  lowerRight.setValues(right, down);
  trackLike = true;
  setPaintMode(1);
  setParticleMode(false);
}

void EventPainter::setEvent(TEvent* e) { Event = e; }

void EventPainter::setCanvas(TPad* c) { canvas = c; }

void EventPainter::setSize(float left, float right, float up, float down)
{
  upperLeft.setValues(left, up);
  lowerRight.setValues(right, down);
}

void EventPainter::setSize(point2D upperLeftIn, point2D lowerRightIn)
{
  if (upperLeftIn.getState() == _regular_)
    upperLeft = upperLeftIn;
  if (lowerRightIn.getState() == _regular_)
    lowerRight = lowerRightIn;
}

void EventPainter::addDetector(int detNum, int color)
{
  bool isin = false;
  for (unsigned int i = 0; i < detectors.size(); i++) {
    if (detectors[i] == detNum) {
      isin = true;
      detectorColors[i] = color;
    }
  }
  if (isin)
    return;
  detectors.push_back(detNum);
  detectorColors.push_back(color);
}

void EventPainter::addPixel(int pixNum, int color, int marker)
{
  bool isin = false;
  for (unsigned int i = 0; i < pixels.size(); i++) {
    if (pixels[i] == pixNum) {
      isin = true;
      pixelColors[i] = color;
      pixelMarker[i] = marker;
    }
  }
  if (isin)
    return;
  pixels.push_back(pixNum);
  pixelColors.push_back(color);
  pixelMarker.push_back(marker);
}

void EventPainter::addTrackColor(int color) { trackColors.push_back(color); }

void EventPainter::clearDetectors()
{
  while (!(detectors.empty())) {
    detectors.pop_back();
    detectorColors.pop_back();
  }
}

void EventPainter::clearPixels()
{
  while (!(pixels.empty())) {
    pixels.pop_back();
    pixelColors.pop_back();
    pixelMarker.pop_back();
  }
}

void EventPainter::clearTrackColor()
{
  while (!(trackColors.empty()))
    trackColors.pop_back();
}

void EventPainter::removeDetector(int detNum)
{
  bool isin = false;
  for (unsigned int i = 0; i < detectors.size(); i++) {
    if (detectors[i] == detNum) {
      isin = true;
    }
  }
  if (!isin)
    return;
  vector<int> tmpD, tmpC;
  while (detectors.back() != detNum) {
    tmpD.push_back(detectors.back());
    detectors.pop_back();
    tmpC.push_back(detectorColors.back());
    detectorColors.pop_back();
  }
  detectors.pop_back();
  detectorColors.pop_back();
  while (!(tmpD.empty())) {
    detectors.push_back(tmpD.back());
    tmpD.pop_back();
    detectorColors.push_back(tmpC.back());
    tmpC.pop_back();
  }
}

void EventPainter::removePixel(int pixNum)
{
  bool isin = false;
  for (unsigned int i = 0; i < pixels.size(); i++) {
    if (pixels[i] == pixNum) {
      isin = true;
    }
  }
  if (!isin)
    return;
  vector<int> tmpD, tmpC, tmpM;
  while (pixels.back() != pixNum) {
    tmpD.push_back(pixels.back());
    pixels.pop_back();
    tmpC.push_back(pixelColors.back());
    pixelColors.pop_back();
    tmpM.push_back(pixelMarker.back());
    pixelMarker.pop_back();
  }
  pixels.pop_back();
  pixelMarker.pop_back();
  pixelColors.pop_back();
  while (!(tmpD.empty())) {
    pixels.push_back(tmpD.back());
    tmpD.pop_back();
    pixelColors.push_back(tmpC.back());
    tmpC.pop_back();
    pixelMarker.push_back(tmpM.back());
    tmpM.pop_back();
  }
}

void EventPainter::setDetector(int detNum, int color)
{
  for (unsigned int i = 0; i < detectors.size(); i++) {
    if (detectors[i] == detNum)
      detectorColors[i] = color;
  }
}

void EventPainter::setPixel(int pixNum, int color, int marker)
{
  for (unsigned int i = 0; i < pixels.size(); i++) {
    if (pixels[i] == pixNum) {
      pixelColors[i] = color;
      pixelMarker[i] = marker;
    }
  }
}

void EventPainter::setPixel(int pixNum, int color)
{
  for (unsigned int i = 0; i < pixels.size(); i++) {
    if (pixels[i] == pixNum) {
      pixelColors[i] = color;
    }
  }
}

void EventPainter::setTrackSorted(bool sort) { detSorted = sort; }
void EventPainter::setTrackLike(bool like) { trackLike = like; }
vector<TBase*> EventPainter::getDrawnObjects(double x, double y)
{
  vector<TBase*> ret;
  double x1, x2, y1, y2;
  canvas->GetRange(x1, y1, x2, y2);
  double width = sqrt((x2 - x1) * (x2 - x1) + (y1 - y2) * (y1 - y2)) / 100.;
  for (unsigned int i = 0; i < drawings.size(); i++) {
    if (drawings[i]->IsA() == TCrown::Class()) {
      point2D p2(((TCrown*)drawings[i])->GetX1(), ((TCrown*)drawings[i])->GetY1());
      vector2D v((x - p2.X()), (y - p2.Y()));
      if (v.length() >= ((TCrown*)drawings[i])->GetR1() &&
          v.length() <= ((TCrown*)drawings[i])->GetR2())
        ret.push_back(drawnObjects[i]);
    } else if (drawings[i]->IsA() == TEllipse::Class()) {
      point2D p1(x, y);
      point2D p2(((TEllipse*)drawings[i])->GetX1(), ((TEllipse*)drawings[i])->GetY1());
      float ri = ((TEllipse *)drawings[i])->GetR1(), ro = ((TEllipse *)drawings[i])->GetR2();
      if ((p1 - p2).R() >= ri && (p1 - p2).R() <= ro)
        ret.push_back(drawnObjects[i]);
    } else if (drawings[i]->IsA() == TMarker::Class()) {
      point2D p1(x, y);
      point2D p2(((TMarker*)drawings[i])->GetX(), ((TMarker*)drawings[i])->GetY());
      double dist = (p1 - p2).length();
      if (dist < width)
        ret.push_back(drawnObjects[i]);
    } else if (drawings[i]->IsA() == TPolyLine::Class()) {
      if (drawnObjects[i]->name() == "TTrack") {
        lLine2D line(
            point2D(((TPolyLine*)drawings[i])->GetX()[0], ((TPolyLine*)drawings[i])->GetY()[0]),
            point2D(((TPolyLine*)drawings[i])->GetX()[1], ((TPolyLine*)drawings[i])->GetY()[1]));
        double dist = (line // point2D(((TPolyLine*)drawings[i])->GetX()[0],
                       //   ((TPolyLine*)drawings[i])->GetY()[0])
                       - point2D(x, y))
                          .length();
        if (dist < width)
          ret.push_back(drawnObjects[i]);
        // 	      else{
        // 		dist=(point2D(((TPolyLine*)drawings[i])->GetX()[1],
        // 			      ((TPolyLine*)drawings[i])->GetY()[1])
        // 		      -point2D(x,y)).length();
        // 		if(dist<width*3)
        // 		  ret.push_back(drawnObjects[i]);
        // 	      }
      } else {
        TCutG cut("cut1", ((TPolyLine*)drawings[i])->GetN(), ((TPolyLine*)drawings[i])->GetX(),
                  ((TPolyLine*)drawings[i])->GetY());
        if (cut.IsInside(x, y))
          ret.push_back(drawnObjects[i]);
      }
    }
  }
  return ret;
}
void EventPainter::rePaint(TEvent* e /*,float zoom*/)
{
  Event = e;
  rePaint(/*zoom*/);
}

void EventPainter::rePaint(/*float zoom*/)
{
  if (Event == NULL)
    return;
  if ((canvas == 0) || (Event == 0))
    return;
  canvas->cd();
  canvas->Clear();
  TObject* tmp;
  while (!drawnObjects.empty())
    drawnObjects.pop_back();
  while (!(drawings.empty())) {
    tmp = drawings.back();
    drawings.pop_back();
    if (tmp != NULL)
      delete tmp;
  }
  //     projector::setProjector(plane3D(point3D(0,0,1),vector3D(1,0,0),vector3D(0,1,0)),point3D(0,0,0),projector::fishEye);
  paintHits(/*zoom*/);
  paintPixels(/*zoom*/);
  paintTracks(/*zoom*/);
  double ar = canvas->GetWw() / canvas->GetWh();
  double width = lowerRight.X() - upperLeft.X(), height = lowerRight.Y() - upperLeft.Y();
  double cx = upperLeft.X() + width / 2., cy = upperLeft.Y() + height / 2.;
  if (ar > width / height)
    width = width * ar / width * height;
  else
    height = height / (ar / width * height);
  canvas->Range(cx - width / 2., cy - height / 2., cx + width / 2., cy + height / 2.);
  double x1, y1, x2, y2;
  canvas->GetRange(x1, y1, x2, y2);
  TText* text = new TText(
      x1, y1 + 75, (string("#hits: ") + string_number(Event->getTotalNumberOfHits())).data());
  text->Draw();
  drawings.push_back(text);
  text = new TText(x1, y1 + 50,
                   (string("#pixels: ") + string_number(Event->getTotalNumberOfPixels())).data());
  text->Draw();
  drawings.push_back(text);
  text = new TText(x1, y1 + 25,
                   (string("#tracks: ") + string_number(Event->getNumberOfTracks())).data());
  text->Draw();
  drawings.push_back(text);
  text = new TText(x1, y1, (string("eventNr: ") + string_number(Event->getEventNumber())).data());
  text->Draw();
  drawings.push_back(text);
}

void EventPainter::paintHits(/*float zoom*/)
{
  volumeShape* sh;
  int col;
  vector4D boundingBox, maxBoundingBox(100000, 100000, -100000, -100000);
  vector3D eye(0, 0, 0);
  plane3D plane(eye + (vector3D(0, 0, 1)), vector3D(0, 0, 1));
  //    cout<<"paint "<<detectors.size()<<" detectors"<<endl;
  for (int j = detectors.size() - 1; j >= 0; j--) {
    if (detectors[j] < 0 || Event->getMaxNumber<TDetector>() <= detectors[j])
      continue;
    col = detectorColors[j];
    for (int i = 0; i < Event->getNumberOfHits(detectors[j]); i++) {
      if (trackLike) {
        col = -1;
        for (int l = 0; l < Event->getNumberOfTracks(); l++)
          for (int k = 0; k < Event->getTrack(l).getNumberOfCalibHits(detectors[j]); k++)
            if (Event->getTrack(l).getCalibSrcr(detectors[j], k) == &Event->getHit(detectors[j], i))
              col = trackColors[l];
        if (col == -1)
          col = 1;
      }
      if (Event->getHit(detectors[j], i).isValid()) {
        sh = Event->getHit(detectors[j], i).getHitShape();
        if (sh != NULL) {
          TObject* no = NULL;
          drawnObjects.push_back(&Event->getHit(detectors[j], i));
          sh->Draw(eye, plane, &boundingBox, &no, col, col, 0);
          drawings.push_back(no);
          if (boundingBox[0] < maxBoundingBox[0])
            maxBoundingBox.setValue(0, boundingBox[0]);
          if (boundingBox[1] < maxBoundingBox[1])
            maxBoundingBox.setValue(1, boundingBox[1]);
          if (boundingBox[2] > maxBoundingBox[2])
            maxBoundingBox.setValue(2, boundingBox[2]);
          if (boundingBox[3] > maxBoundingBox[3])
            maxBoundingBox.setValue(3, boundingBox[3]);
        }
      }
    }
  }
  upperLeft.setValues(maxBoundingBox[0], maxBoundingBox[1]);
  lowerRight.setValues(maxBoundingBox[2], maxBoundingBox[3]);
}

void EventPainter::paintPixels(/*float zoom*/)
{
  vector3D eye(0, 0, 0);
  plane3D plane(eye + (vector3D(0, 0, 1)), vector3D(0, 0, 1));
  planeShape* ps;
  for (unsigned j = 0; j < pixels.size(); j++)
  //    for(int i=Event->getNumberOfHits(-2)-1;i>=0;i--)
  {
    if (pixels[j] < 0 || pixels[j] >= Event->getMaxNumber<TDetector>())
      continue;
    if (Event->getNumberOfPixels(pixels[j]) <= 0)
      continue;
    point3D center;
    point2D p;
    for (int i = 0; i < Event->getNumberOfPixels(pixels[j]); i++) {
      ps = Event->getPixel(pixels[j], i).getShape();
      if (ps) {
        TObject* no = NULL;
        ps->Draw(eye, plane, NULL, &no, pixelColors[j], pixelColors[j], 1001);
        if (no == NULL)
          continue;
        drawings.push_back(no);
        drawnObjects.push_back(&Event->getPixel(pixels[j], i));
      }
    }
  }
}

void EventPainter::paintTracks(/*float zoom*/)
{
  //     sLine3D path;
  //     vector3D d;
  //     point3D c;
  //     point2D p;
  vector3D eye(0, 0, 0);
  plane3D plane(eye + (vector3D(0, 0, 1)), vector3D(0, 0, 1));
  point3D vertex, endpoint;
  vector3D direction;
  point2D tmp1_2, tmp2_2;
  for (int i = 0; i < Event->getNumberOfTracks(); i++) {
    TObject* no = NULL;
    drawnObjects.push_back(&Event->getTrack(i));
    vertex = Event->getTrack(i).getPath().Foot();
    direction = Event->getTrack(i).getPath().Direction();
    if (Event->getTrack(i).getNumberOfSecondaryTracks() > 0)
      endpoint = Event->getTrack(i).getSecondary(0)->getPath().Foot();
    else
      endpoint = vertex + direction * 50000;
    tmp1_2 = projector::project(vertex);
    tmp2_2 = projector::project(endpoint);
    //      cout<<i<<":"<<Event->getTrack(i).getPath()<<"
    //      \n\t"<<tmp1_2<<tmp2_2<<endl;
    int color = trackColors[i];
    if (particleLike && Event->getTrack(i).getParticleID() > 0 &&
        (int)particleColors.size() > Event->getTrack(i).getParticleID())
      color = particleColors[Event->getTrack(i).getParticleID()];
    if (!Event->getTrack(i).isSecondary()) {
      Double_t pox = tmp2_2.X(), poy = tmp2_2.Y();
      no = new TMarker(pox, poy, 29);
      ((TMarker*)no)->SetMarkerSize(1); //,.02);
      ((TMarker*)no)->SetMarkerColor(color);
      if (Event->getTrack(i).getCharge() == 0)
        ((TMarker*)no)->SetMarkerStyle(30);
      else
        ((TMarker*)no)->SetMarkerStyle(29);
      no->Draw();
      drawings.push_back(no);
    } else {
      //	  cout<<"  draw secondary:"<<endl;
      Double_t pox[2] = {tmp1_2.X(), tmp2_2.X()}, poy[2] = {tmp1_2.Y(), tmp2_2.Y()};
      no = new TPolyLine(2, pox, poy);
      //	  no->SetLineWidth(2);
      ((TPolyLine*)no)->SetLineColor(color);
      ((TPolyLine*)no)->SetLineWidth(3);
      if (Event->getTrack(i).getCharge() == 0)
        ((TPolyLine*)no)->SetLineStyle(2); // dashed
      else
        ((TPolyLine*)no)->SetLineStyle(1); // solid
      no->Draw();
      drawings.push_back(no);
    }
  }
}

void EventPainter::setPaintMode(int mode)
{
  switch (mode) {
  case 0:
    projector::setProjector(plane3D(point3D(0, 0, 100000), vector3D(1, 0, 0), vector3D(0, 1, 0)),
                            point3D(0, 0, 0), projector::parallel);
    break;
  case 1:
    projector::setProjector(plane3D(point3D(0, 0, 100000), vector3D(1, 0, 0), vector3D(0, 1, 0)),
                            point3D(0, 0, 0), projector::central);
    break;
  case 2:
    projector::setProjector(plane3D(point3D(0, 0, 100000), vector3D(1, 0, 0), vector3D(0, 1, 0)),
                            point3D(0, 0, 0), projector::fishEye);
    break;
  }
}
void EventPainter::addParticleColor(int ID, int color)
{
  while (ID >= (int)particleColors.size() - 1)
    particleColors.push_back(1);
  particleColors[ID] = color;
}
void EventPainter::clearParticleColor()
{
  while (!particleColors.empty())
    particleColors.pop_back();
}
void EventPainter::setParticleMode(bool value) { particleLike = value; }
