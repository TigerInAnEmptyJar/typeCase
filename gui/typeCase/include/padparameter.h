#ifndef PADPARAMETER
#define PADPARAMETER

#include "paintparameter.h"
#include "eventpainter.h"
#include <TCanvas.h>
#include <TTree.h>
#include <TH1.h>
#include <qdatetime.h>
using namespace std;

class pad_parameter
{
  vector<TObject*> toDelete;
 public:
  TVirtualPad *drawPad;
  vector<paint_parameter> track;
  vector<paint_parameter> pixel;
  vector<paint_parameter> hits;
  EventPainter painter;
  int paintMode;
  TEvent *eventPointer;
  QDateTime *startTimePointer;
  int nEvents;
  int nEvents2A;
  TTree *treePointer;
  TH1* histoPointer;
  string drawOption;
  string drawSelection;
  string drawName;
  float zoom;
  point2D center;

  pad_parameter();
  pad_parameter(const pad_parameter &p);
  pad_parameter& operator=(const pad_parameter &p);
  virtual ~pad_parameter();

  virtual void draw();
  virtual void update();
};

ostream& operator<<(ostream& o, const pad_parameter &p);
istream& operator>>(istream& i, pad_parameter &p);

#endif //PADPARAMETER
