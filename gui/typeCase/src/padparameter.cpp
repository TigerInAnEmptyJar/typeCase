#include "padparameter.h"
#include <TPaveText.h>
#include "utilities.h"
pad_parameter::pad_parameter():painter()
{
  drawPad=NULL;
  eventPointer=NULL;
  startTimePointer=NULL;
  nEvents=0;
  treePointer=NULL;
  histoPointer=NULL;
  paintMode=0;
  drawOption="";
  drawName="";
  zoom=1;
  painter.setTrackLike(false);
}
pad_parameter::pad_parameter(const pad_parameter &p)
{
  drawPad=p.drawPad;
  eventPointer=p.eventPointer;
  startTimePointer=p.startTimePointer;
  nEvents=p.nEvents;
  treePointer=p.treePointer;
  histoPointer=p.histoPointer;
  paintMode=p.paintMode;
  drawOption=p.drawOption;
  drawName=p.drawName;
  zoom=p.zoom;
  for(unsigned int i=0;i<p.hits.size();i++)
    hits.push_back(p.hits[i]);
  for(unsigned int i=0;i<p.pixel.size();i++)
    pixel.push_back(p.pixel[i]);
  for(unsigned int i=0;i<p.track.size();i++)
    track.push_back(p.track[i]);
  painter.setTrackLike(false);
  update();
}
pad_parameter& pad_parameter::operator=(const pad_parameter &p)
{
  drawPad=p.drawPad;
  eventPointer=p.eventPointer;
  startTimePointer=p.startTimePointer;
  nEvents=p.nEvents;
  treePointer=p.treePointer;
  histoPointer=p.histoPointer;
  paintMode=p.paintMode;
  zoom=p.zoom;
  drawOption=p.drawOption;
  drawName=p.drawName;
  while(!hits.empty())hits.pop_back();
  while(!pixel.empty())pixel.pop_back();
  while(!track.empty())track.pop_back();
  for(unsigned int i=0;i<p.hits.size();i++)
    hits.push_back(p.hits[i]);
  for(unsigned int i=0;i<p.pixel.size();i++)
    pixel.push_back(p.pixel[i]);
  for(unsigned int i=0;i<p.track.size();i++)
    track.push_back(p.track[i]);
  update();
  return *this;
}
pad_parameter::~pad_parameter()
{
}
void pad_parameter::draw()
{
  if(drawPad==NULL)return;
  TVirtualPad *tmpPad=gPad;
  drawPad->cd();
  switch(paintMode)
    {
    case 0: break;
    case 1://eventPainter
      {
	if(eventPointer!=NULL)
	  {
	    update();
	    projector::setProjector(plane3D(point3D(0,0,1000000),vector3D(1,0,0),vector3D(0,1,0)),point3D(0,0,0),projector::central);
	    painter.rePaint();
	    Double_t x[4];
	    drawPad->GetRange(x[0],x[1],x[2],x[3]);
	    double wid=(x[2]-x[0])*0.5,hei=(x[3]-x[1])*0.5;
	    drawPad->Range(center.X()-wid*zoom,center.Y()-hei*zoom,center.X()+wid*zoom,center.Y()+hei*zoom);
	  }
	break;
      }
    case 2://eventInfo
      {
	cout<<"draw event info " <<eventPointer<<endl;
	if(eventPointer!=NULL)
	  {
	    update();
	    int eventnumber=eventPointer->getEventNumber();
	    int runnumber=eventPointer->getRunNumber();
	    int nHits=eventPointer->getTotalNumberOfHits();
	    int nPixel=eventPointer->getTotalNumberOfPixels();
	    int nTracks=eventPointer->getNumberOfTracks();
	    cout<<eventnumber<<" "<<runnumber<<" "<<nHits<<" "<<nPixel<<" "<<nTracks<<endl;
	    drawPad->Clear();
	    drawPad->Range(0,0,1,1);
	    TPaveText *p1=new TPaveText(0.1,0.1,0.9,0.9,"tl NDC");
	    p1->AddText((string("Run   : ")+string_number(runnumber)).data());
	    p1->AddText((string("Event : ")+string_number(eventnumber)).data());
	    p1->AddText((string("Hits  : ")+string_number(nHits)).data());
	    p1->AddText((string("Pixels: ")+string_number(nPixel)).data());
	    p1->AddText((string("Tracks: ")+string_number(nTracks)).data());
	    p1->Draw();
	    //	    while(!toDelete.empty()){delete toDelete.back();toDelete.pop_back();}
	    //	    toDelete.push_back(p1);
	    drawPad->Update();
	  }
	break;
      }
    case 3://analysisInfo
      {
	TPaveText* p1=new TPaveText(0.1,0.1,0.9,0.9);
	p1->AddText((string("Events analysed : ")+string_number(nEvents)).data());
	p1->AddText((string("Events requested: ")+string_number(nEvents2A)).data());
	if(startTimePointer!=NULL)
	  {
	    p1->AddText((string("Start : ")+QS2s(startTimePointer->toString())).data());
	    int taken=startTimePointer->secsTo(QDateTime::currentDateTime());
	    p1->AddText((string(" taken: ")+string_number(taken)+" s").data());
	    QDateTime tmp=*startTimePointer;
	    tmp.addSecs((taken/nEvents)*(nEvents2A-nEvents));
	    p1->AddText((string(" eta  : ")+QS2s(tmp.toString())).data());
	  }
	p1->Draw();
	//	while(!toDelete.empty()){delete toDelete.back();toDelete.pop_back();}
	//	toDelete.push_back(p1);
	break;
      }
    case 4://tree
      {
	if(treePointer!=NULL&&drawName!="")
	  treePointer->Draw(drawName.data(),drawSelection.data(),drawOption.data());
	break;
      }
    case 5://histo
      {
	if(histoPointer!=NULL)
	  histoPointer->Draw(drawOption.data());
	break;
      }
    }
  drawPad->Update();
  if(tmpPad!=NULL)tmpPad->cd();
}
void pad_parameter::update()
{
  if(eventPointer!=NULL)
    painter.setEvent(eventPointer);
  if(drawPad!=NULL)
    painter.setCanvas((TPad*)drawPad);
  painter.clearDetectors();
  painter.clearPixels();
  painter.clearTrackColor();
  for(unsigned int i=0;i<hits.size();i++)
    if(hits[i].isToPaint())
      painter.addDetector(hits[i].getID(), hits[i].getColor());
  for(unsigned int i=0;i<pixel.size();i++)
    if(pixel[i].isToPaint())
      painter.addPixel(pixel[i].getID(),pixel[i].getColor(), pixel[i].getMarker());
  for(unsigned int i=0;i<track.size();i++)
    painter.addTrackColor(track[i].getColor());
}
ostream& operator<<(ostream& o, const pad_parameter &p)
{
  o<<p.paintMode<<" "<<p.nEvents2A<<" "<<p.nEvents<<endl;
  for(unsigned int i=0;i<p.track.size();i++)
    o<<"T"<<p.track[i];
  for(unsigned int i=0;i<p.pixel.size();i++)
    o<<"P"<<p.pixel[i];
  for(unsigned int i=0;i<p.hits.size();i++)
    o<<"H"<<p.hits[i];
  o<<"N"<<p.drawName.data()<<endl;
  o<<"S"<<p.drawSelection.data()<<endl;
  o<<"O"<<p.drawOption.data()<<endl;
  o<<"=";
  return o;
}
istream& operator>>(istream& i, pad_parameter &p)
{
  i>>p.paintMode>>p.nEvents2A>>p.nEvents;
  char c;
  string s="";
  i.get(c);while(c!='\n')i.get(c);
  i.get(c);
  while(c!='=')
    {
      switch(c)
	{
	case 'T':
	  {
	    paint_parameter pa;
	    i>>pa;
	    p.track.push_back(pa);
	    break;
	  }
	case 'P':
	  {
	    paint_parameter pa;
	    i>>pa;
	    p.pixel.push_back(pa);
	    break;
	  }
	case 'H':
	  {
	    paint_parameter pa;
	    i>>pa;
	    p.hits.push_back(pa);
	    break;
	  }
	case 'N':
	  {
	    s="";
	    i.get(c);while(c!='\n'){s+=c;i.get(c);}
	    p.drawName=s;
	    break;
	  }
	case 'S':
	  {
	    s="";
	    i.get(c);while(c!='\n'){s+=c;i.get(c);}
	    p.drawSelection=s;
	    break;
	  }
	case 'O':
	  {
	    s="";
	    i.get(c);while(c!='\n'){s+=c;i.get(c);}
	    p.drawOption=s;
	    break;
	  }
	}
      i.get(c);
    }
  p.update();
  return i;
}
