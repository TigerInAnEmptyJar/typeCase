#include <TTree.h>
#include "algorithm.h"
#include "pixel.h"
#include <TFile.h>
#include "setup.h"
typedef struct
{
  Float_t xc;
  Float_t yc;
  Float_t dx;
  Float_t dy;
  Float_t z;
  Int_t ID;
  Int_t n1;
  Int_t n2;
  Int_t n3;
}pixelStruct;

using namespace std;
class APixelTreeWriteOut: public AAlgorithm
{
 private:
  int nPixelTypes;
  int *PixelIDs;//[nPixelTypes]
  int maxPixelPerID;
  Int_t eventNumber;
  Int_t runNumber;
  Int_t *nPixels;//[nPixelTypes]
  int &evtNr;
  int &rnNr;
  pixelStruct **pixOut;//!
  TPixel ***pixels;//!
  int **numberOfPixels;//!
  TTree *outTree;//!
  TFile *outFile;//!
  point3D *centerPoints;//![nPixelTypes]
  void setDefault(int i, int j);
 public:
  APixelTreeWriteOut(TSetup &setup, int &eventNumberIn, int &runNumberIn,TPixel ***pixelsIn, int** numberOfPixelsIn, const algorithm_parameter& param);
  virtual ~APixelTreeWriteOut();
  virtual void *process(void* ptr);
  static algorithm_parameter getDescription();
};
