#include "AReadReactionFromRoot.h"
#include "Eparticles.h"
#include "logger.h"
#include <TBranch.h>
#include <TLeaf.h>
#ifndef INT_MAX
#define INT_MAX 1000000000;
#endif
extern bool existing(string filename);
int getNtrees(int num, int mode)
{
  if (mode == 3) {
    if (num <= 5)
      return 1;
    if (num <= 10)
      return 2;
    return ((int)num / 20) + 3;
  }
  return ((int)(num / 20) + 1);
}

int searchPosInTree(TBranch* bran, int minP, int maxP, Int_t value, Int_t& val, int mode)
{
  int posH = maxP, posL = minP, posM;
  if (posH < posL) {
    posH = minP;
    posL = maxP;
  }
  if (mode == 0) // search identity
  {
    bran->GetEntry(posH);
    if (val <= value)
      return posH;
    bran->GetEntry(posL);
    if (val >= value)
      return posL;
    while (posH - posL > 2) {
      posM = (int)((posH + posL) / 2);
      bran->GetEntry(posM);
      if (val == value)
        return posM;
      else if (val < value)
        posL = posM;
      else
        posH = posM;
    }
    return posH;
  } else if (mode == 1) // search begin
  {
    bran->GetEntry(posL);
    if (val >= value)
      return posL;
    while (posH - posL > 2) {
      posM = (int)((posH + posL) / 2);
      bran->GetEntry(posM);
      if (val < value)
        posL = posM;
      else
        posH = posM;
    }
    return posH;
  } else if (mode == 2) // search end
  {
    bran->GetEntry(posH);
    if (val <= value)
      return posH;
    while (posH - posL > 2) {
      posM = (int)((posH + posL) / 2);
      bran->GetEntry(posM);
      if (val > value)
        posH = posM;
      else
        posL = posM;
    }
    return posH;
  }
  return 0;
}
void AReadReactionFromRoot::cleanHit()
{
  if (Hits == NULL)
    return;
  for (int i = 0; i < NHit; i++)
    delete[] Hits[i];
  delete[] Hits;
  delete[] hitTree;
  if (NHit)
    hitf = numHitTree->GetCurrentFile();
  hitf->Close();
  delete hitf;
  hitf = NULL;
  Hits = NULL;
}
void AReadReactionFromRoot::cleanPix()
{
  if (Pix == NULL)
    return;
  for (int i = 0; i < NPix; i++)
    delete[] Pix[i];
  delete[] Pix;
  delete[] pixTree;
  if (NPix)
    pixf = numPixTree->GetCurrentFile();
  pixf->Close();
  delete pixf;
  pixf = NULL;
  Pix = NULL;
}
void AReadReactionFromRoot::cleanClu()
{
  if (Clus == NULL)
    return;
  for (int i = 0; i < NClu; i++)
    delete[] Clus[i];
  delete[] Clus;
  delete[] cluTree;
  if (numCluTree)
    cluf = numCluTree->GetCurrentFile();
  cluf->Close();
  delete cluf;
  cluf = NULL;
  Clus = NULL;
}
void AReadReactionFromRoot::cleanTra()
{
  if (Tracks == NULL)
    return;
  for (int i = 0; i < NTra; i++) {
    delete[] Tracks[i];
    for (int j = 0; j < (i <= 1 ? 5 : (i == 2 ? 10 : 20)); j++)
      delete[] hitsOfTrack[i][j];
    delete[] hitsOfTrack[i];
    delete[] fromID[i];
  }
  delete[] Tracks;
  delete[] hitsOfTrack;
  delete[] fromID;
  delete[] traTree;
  if (numTraTree)
    traf = numTraTree->GetCurrentFile();
  traf->Close();
  delete traf;
  traf = NULL;
  Tracks = NULL;
}
void AReadReactionFromRoot::cleanARea(int num)
{
  if (RTra[num] == NULL)
    return;
  delete[] RTra[num];
  for (int j = 0; j < nRtr[num]; j++)
    delete[] Rhits[num][j];
  delete[] Rhits[num];
  delete[] RFrom[num];
  if (reactionf[num]) {
    //      reactionf=reactionTree[num]->GetCurrentFile();
    // if(reactionf->IsOpen())
    reactionf[num]->Close();
    // delete reactionf;
  }
  RTra[num] = NULL;
  reactionf[num] = NULL;
}
void AReadReactionFromRoot::cleanRea()
{
  if (RTra == NULL)
    return;
  for (int i = 0; i < NRea; i++)
    cleanARea(i);
  delete[] RTra;
  delete[] Rhits;
  delete[] RFrom;
  delete[] nRtr;
  delete[] evNR;
  delete[] ruNR;
  delete[] tR;
  delete[] reactionTree;
  delete[] currentReaEntry;
  delete[] nextReaEntry;
  delete[] RfilePos;
  delete[] reactionf;
  reactionf = NULL;
  RTra = NULL;
}
void AReadReactionFromRoot::set_hit(string filename)
{
  if (readHits) {
    hitf = new TFile(filename.data(), "READ");
    numHitTree = (TTree*)hitf->Get("numberOfHits");
    numHitTree->SetBranchAddress("EventNumber", &evNH);
    numHitTree->SetBranchAddress("RunNumber", &ruNH);
    numHitTree->SetBranchAddress("NumberOfHits", &numberOfHits);
    numHitTree->SetBranchAddress("Trigger", &tH);
    TTree* tmptree = (TTree*)hitf->Get("Hits1");
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)hitf->Get((string("Hits") + string_number(numTmp + 1)).data());
    }
    hitTree = new TTree*[numTmp];
    Hits = new hit*[numTmp];
    if (currentHitEntry)
      delete[] currentHitEntry;
    currentHitEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentHitEntry[i] = -1;
    if (nextHitEntry)
      delete[] nextHitEntry;
    nextHitEntry = new int[numTmp];
    for (int j = 0; j < numTmp; j++) {
      tmptree = (TTree*)hitf->Get((string("Hits") + string_number(j + 1)).data());
      hit* tmptracks = new hit[20];
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("EventNumber", &nextHitEntry[j]);
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchStatus((string("Hit") + string_number(j * 20 + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("Hit") + string_number(j * 20 + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Hits[j] = (tmptracks);
      hitTree[j] = (tmptree);
      tmptree->GetEntry(0);
    }
    anaLog << (numTmp) << " hit trees found" << endli;
    NHit = numTmp;
  }
}
void AReadReactionFromRoot::set_pix(string filename)
{
  if (readPixel) {
    pixf = new TFile(filename.data(), "READ");
    numPixTree = (TTree*)pixf->Get("numberOfPixels");
    numPixTree->SetBranchAddress("EventNumber", &evNP);
    numPixTree->SetBranchAddress("RunNumber", &ruNP);
    numPixTree->SetBranchAddress("NumberOfPixels", &numberOfPixels);
    numPixTree->SetBranchAddress("Trigger", &tP);
    TTree* tmptree = (TTree*)pixf->Get("Pixels1");
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)pixf->Get((string("Pixels") + string_number(numTmp + 1)).data());
    }
    pixTree = new TTree*[numTmp];
    Pix = new pixel*[numTmp];
    if (currentPixEntry)
      delete[] currentPixEntry;
    currentPixEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentHitEntry[i] = -1;
    if (nextPixEntry)
      delete[] nextPixEntry;
    nextPixEntry = new int[numTmp];
    for (int j = 0; j < numTmp; j++) {
      tmptree = (TTree*)pixf->Get((string("Pixels") + string_number(j + 1)).data());
      pixel* tmptracks = new pixel[20];
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("EventNumber", &nextPixEntry[j]);
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchStatus((string("Pixel") + string_number(j * 20 + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("Pixel") + string_number(j * 20 + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Pix[j] = (tmptracks);
      pixTree[j] = (tmptree);
      tmptree->GetEntry(0);
    }
    anaLog << (numTmp) << " pixel trees found" << endli;
    NPix = numTmp;
  }
}
void AReadReactionFromRoot::set_clu(string filename)
{
  if (readCluster) {
    cluf = new TFile(filename.data(), "READ");
    numCluTree = (TTree*)cluf->Get("numberOfClusters");
    numCluTree->SetBranchAddress("EventNumber", &evNC);
    numCluTree->SetBranchAddress("RunNumber", &ruNC);
    numCluTree->SetBranchAddress("NumberOfClusters", &numberOfCluster);
    numCluTree->SetBranchAddress("Trigger", &tC);
    TTree* tmptree = (TTree*)cluf->Get("Clusters1");
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)cluf->Get((string("Clusters") + string_number(numTmp + 1)).data());
    }
    cluTree = new TTree*[numTmp];
    Clus = new cluster*[numTmp];
    if (currentCluEntry)
      delete[] currentCluEntry;
    currentCluEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentCluEntry[i] = -1;
    if (nextCluEntry)
      delete[] nextCluEntry;
    nextCluEntry = new int[numTmp];
    for (int j = 0; j < numTmp; j++) {
      tmptree = (TTree*)cluf->Get((string("Clusters") + string_number(j + 1)).data());
      cluster* tmptracks = new cluster[20];
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("EventNumber", &nextCluEntry[j]);
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchStatus((string("Cluster") + string_number(j * 20 + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("Cluster") + string_number(j * 20 + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Clus[j] = (tmptracks);
      cluTree[j] = (tmptree);
      tmptree->GetEntry(0);
    }
    anaLog << (numTmp) << " cluster trees found" << endli;
    NClu = numTmp;
  }
}
void AReadReactionFromRoot::set_tra(string filename)
{
  if (readTracks) {
    traf = new TFile(filename.data(), "READ");
    numTraTree = (TTree*)traf->Get("numberOfTracks");
    numTraTree->SetBranchAddress("EventNumber", &evNT);
    numTraTree->SetBranchAddress("RunNumber", &ruNT);
    numTraTree->SetBranchAddress("NumberOfTracks", &ntr);
    int numTmp = 0;
    TTree* tmptree = (TTree*)traf->Get("Track1");
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)traf->Get((string("Track") + string_number(numTmp + 1)).data());
    }
    Tracks = new track1*[numTmp];
    fromID = new Int_t*[numTmp];
    hitsOfTrack = new Int_t**[numTmp];
    traTree = new TTree*[numTmp];
    if (currentTraEntry)
      delete[] currentTraEntry;
    currentTraEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      if (nextTraEntry)
        delete[] nextTraEntry;
    nextTraEntry = new int[numTmp];
    int accnum = 5;
    int beg = 0;
    for (int j = 0; j < numTmp; j++) // while(tmptree!=NULL)
    {
      tmptree = (TTree*)traf->Get((string("Track") + string_number(j + 1)).data());
      currentTraEntry[j] = -1;
      track1* tmptracks = new track1[accnum];
      Int_t* tmpf = new Int_t[accnum];
      Int_t** tmph = new Int_t*[accnum];
      tmptree->SetBranchStatus("fromID", 1);
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("fromID", tmpf);
      tmptree->SetBranchAddress("EventNumber", &nextTraEntry[j]);
      for (int i = 0; i < accnum; i++) {
        tmph[i] = new Int_t[30];
        tmptree->SetBranchStatus((string("hits") + string_number(beg + i + 1)).data(), 1);
        tmptree->SetBranchStatus((string("Track") + string_number(beg + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("hits") + string_number(beg + i + 1)).data(), tmph[i]);
        tmptree->SetBranchAddress((string("Track") + string_number(beg + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Tracks[j] = (tmptracks);
      traTree[j] = (tmptree);
      fromID[j] = (tmpf);
      hitsOfTrack[j] = (tmph);
      beg += accnum;
      if (j == 1)
        accnum = 10;
      else if (j == 2)
        accnum = 20;
      tmptree->GetEntry(1);
    }
    anaLog << (numTmp) << " track trees found" << endli;
    NTra = numTmp;
  }
}
void AReadReactionFromRoot::set_rea(string filename, int num, string reactionName)
{
  cleanARea(num);
  //  cout<<existing(filename)<<" "<<filename.data()<<endl;
  reactionf[num] = new TFile(filename.data(), "READ");
  //  cout<<reactionf->IsZombie()<<endl;
  //  reactionf->ls();
  reactionTree[num] = (TTree*)reactionf[num]->Get(reactionName.data());
  //  cout<<reactionTree[num]<<endl;
  reactionTree[num]->SetBranchAddress("EventNumber", &evNR[num]);
  reactionTree[num]->SetBranchAddress("RunNumber", &ruNR[num]);
  reactionTree[num]->SetBranchAddress("Trigger", &tR[num]);
  int numTmp = 0;
  TBranch* tmptree = reactionTree[num]->FindBranch("Track1");
  while (tmptree != NULL) {
    numTmp++;
    tmptree = reactionTree[num]->FindBranch((string("Track") + string_number(numTmp + 1)).data());
  }
  RTra[num] = new track1[numTmp];
  RFrom[num] = new Int_t[numTmp];
  Rhits[num] = new Int_t*[numTmp];
  //  reaTree[num]=new TTree*[numTmp];
  currentReaEntry[num] = -1;
  //      reactionTree->SetBranchStatus("fromID",1);
  //      reactionTree->SetBranchAddress("fromID",RFrom[num]);
  for (int i = 0; i < numTmp; i++) // while(tmptree!=NULL)
  {
    Rhits[num][i] = NULL; // new Int_t[30];
    //      reactionTree->SetBranchStatus((string("hits")+string_number(i+1)).data(),1);
    reactionTree[num]->SetBranchStatus((string("Track") + string_number(i + 1)).data(), 1);
    //      reactionTree->SetBranchAddress((string("hits")+string_number(i+1)).data(),Rhits[num][i]);
    reactionTree[num]->SetBranchAddress((string("Track") + string_number(i + 1)).data(),
                                        &RTra[num][i]);
  }
  anaLog << "Reaction :\"" << reactionName << "\" " << numTmp << " tracks" << endli;
  nRtr[num] = numTmp;
}
void AReadReactionFromRoot::read_hit()
{
  for (int j = 0; j < event.getMaxNumber<TDetector>(); j++)
    event.getNumberOfHits(j) = 0;
  numHitTree->GetEntry(currentHitEntry[0]);
  event.getTrigger() = tH;
  int numtrees = getNtrees(numberOfHits, 0);
  if (NHit == 0)
    return;
  if (currentHitEntry[0] >= hitTree[0]->GetEntries()) {
    // return;
    searchEvent(event.getEventNumber(), event.getRunNumber(), 0);
    if (!validInput)
      return;
  }
  for (int j = 0; j < numtrees; j++) {
    if (currentHitEntry[j] < 0)
      currentHitEntry[j]++;
    hitTree[j]->GetEntry(currentHitEntry[j]);
    //      currentHitEntry[j]++;
  }
  //  cout<<"hit event nr:"<<evNH<<endl;
  //   bool same=true;
  //   for(int j=0;j<numtrees;j++)
  //     same=same&&(evNH==tmpev[j]);
  //   if(!same)
  //     {
  //       cout<<"Shift in read in Hits!!!!!"<<endl;
  //       cout<<evNH<<" "<<numtrees<<" "<<numberOfHits<<"-\t"<<flush;
  //       for(int j=0;j<numtrees;j++)
  // 	cout<<tmpev[j]<<"\t"<<flush;
  //       cout<<endl;
  //     }
  TCalibHit* th;
  volumeShape* sh;
  int de, el, n;
  for (int j = 0; j < numberOfHits; j++) {
    de = Hits[j / 20][j % 20].det;
    el = Hits[j / 20][j % 20].el;
    n = event.getNumberOfHits(de);
    if (de < setup.getNumberOfDetectors()) {
      if (event.getMaxNumber<TCalibHit>() > n) {
        th = &event.getHit(de, n);
        th->setADC(Hits[j / 20][j % 20].qdcC);
        th->setTDC(Hits[j / 20][j % 20].tdcC);
        th->getRaw().setADC(Hits[j / 20][j % 20].qdcR);
        th->getRaw().setTDC(Hits[j / 20][j % 20].tdcR);
        th->getRaw().setElement(el);
        th->getRaw().setDetector(de);
        if (Hits[j / 20][j % 20].el <
            static_cast<int>(setup.getDetectorr(de).getNumberOfElements())) {
          sh = setup.getDetectorr(de).getShape(el);
          th->setHitShape(sh);
          th->setValid(true);
        } else
          th->setValid(false);
        event.getNumberOfHits(de)++;
      }
    }
  }
}

void AReadReactionFromRoot::read_pix()
{
  for (int j = 0; j < event.getMaxNumber<TDetector>(); j++)
    event.getNumberOfPixels(j) = 0;
  numPixTree->GetEntry(currentPixEntry[0]);
  event.getTrigger() = tP;
  int numtrees = getNtrees(numberOfPixels, 0);
  if (NPix == 0)
    return;
  if (currentPixEntry[0] >= pixTree[0]->GetEntries()) {
    // return;
    searchEvent(event.getEventNumber(), event.getRunNumber(), 1);
    if (!validInput)
      return;
  }
  for (int j = 0; j < numtrees; j++) {
    if (currentPixEntry[j] < 0)
      currentPixEntry[j]++;
    pixTree[j]->GetEntry(currentPixEntry[j]);
    //      currentPixEntry[j]++;
  }
  //   bool same=true;
  //   for(int j=0;j<numtrees;j++)
  //     same=same&&(evNH==tmpev[j]);
  //   if(!same)
  //     {
  //       cout<<"Shift in read in Hits!!!!!"<<endl;
  //       cout<<evNH<<" "<<numtrees<<" "<<numberOfHits<<"-\t"<<flush;
  //       for(int j=0;j<numtrees;j++)
  // 	cout<<tmpev[j]<<"\t"<<flush;
  //       cout<<endl;
  //     }
  TPixel* tp;
  planeShape* sh;
  int de, el, n;
  for (int j = 0; j < numberOfPixels; j++) {
    de = Pix[j / 20][j % 20].ID;
    if (de < setup.getNumberOfDetectors()) {
      n = event.getNumberOfPixels(de);
      if (event.getMaxNumber<TPixel>() > n) {
        tp = &event.getPixel(de, n);
        tp->reset();
        tp->setID(de);
        sh = tp->getShape();
        if (sh)
          delete sh;
        sh = new planeShape("temp");
        sh->setCenter(point3D(Pix[j / 20][j % 20].center[0], Pix[j / 20][j % 20].center[1],
                              Pix[j / 20][j % 20].center[2]));
        tp->setShape(sh);
        if (readHits) {
          for (int kk = 0; kk < Pix[j / 20][j % 20].numberOfElements; kk++) {
            int k = Pix[j / 20][j % 20].elementNumber[kk];
            int id = Hits[k / 20][k % 20].det;
            el = Hits[k / 20][k % 20].el;
            int l = event.getNumberOfHits(id);
            for (int m = 0; m < l; m++)
              if (el == event.getHit(id, m).getElement() &&
                  Hits[k / 20][k % 20].tdcR == event.getHit(id, m).getRawTDC())
                tp->addElement(event.getHit(id, m));
          }
        }
        event.getNumberOfPixels(de)++;
      }
    }
  }
}

void AReadReactionFromRoot::read_clu()
{
  if (NClu == 0)
    return;
  if (currentCluEntry[0] >= numCluTree->GetEntries()) {
    // return;
    searchEvent(event.getEventNumber(), event.getRunNumber(), 2);
    if (!validInput)
      return;
  }
  numCluTree->GetEntry(currentCluEntry[0]);
  event.getTrigger() = tC;
  int numtrees = getNtrees(numberOfCluster, 2);
  for (int i = 0; i < numtrees; i++)
    cluTree[i]->GetEntry(currentCluEntry[i]);
  for (int j = 0; j < event.getMaxNumber<TDetector>(); j++)
    event.getNumberOfClusters(j) = 0;
  TCluster* tc;
  int n;
  int m;
  for (int j = 0; j < numberOfCluster; j++) {
    if (event.getMaxNumber<TDetector>() < Clus[j / 20][j % 20].ID) {
      n = event.getNumberOfClusters(Clus[j / 20][j % 20].ID);
      if (event.getMaxNumber<TCluster>() < n) {
        tc = &event.getCluster(Clus[j / 20][j % 20].ID, n);
        //      tc->setID(Clus[j/20][j%20].ID);
        tc->reset();
        tc->setCenter(point3D(Clus[j / 20][j % 20].center[0], Clus[j / 20][j % 20].center[1],
                              Clus[j / 20][j % 20].center[2]));
        if (readPixel) {
          TPixel* pi;
          for (int kk = 0; kk < Clus[j / 20][j % 20].numberOfPixels; kk++) {
            int k = Clus[j / 20][j % 20].pixelNumber[kk];
            int id = Pix[k / 20][k % 20].ID;
            m = event.getNumberOfPixels(id);
            for (int l = 0; l < m; l++) {
              pi = &event.getPixel(id, l);
              if (Pix[k / 20][k % 20].center[0] == pi->getShape()->getCenter().X() &&
                  Pix[k / 20][k % 20].center[1] == pi->getShape()->getCenter().Y() &&
                  Pix[k / 20][k % 20].center[2] == pi->getShape()->getCenter().Z())
                tc->addPixel(*pi);
            }
          }
        }
      }
      event.getNumberOfClusters()++;
    }
  }
}
void AReadReactionFromRoot::read_tra()
{
  if (NTra == 0)
    return;
  event.getNumberOfTracks() = 0;
  if (currentTraEntry[0] >= numTraTree->GetEntries())
    return;
  numTraTree->GetEntry(currentTraEntry[0]);
  //   event->getTrigger()=tT;
  int numtrees = getNtrees(ntr, 3);
  if (numtrees > NTra)
    numtrees = NTra;
  if (numtrees < 0)
    numtrees = 0;
  int tmpev[numtrees + 1];
  for (int j = 0; j < numtrees; j++) {
    if (currentTraEntry[j] < 0)
      currentTraEntry[j]++;
    if (currentTraEntry[j] >= traTree[j]->GetEntries()) {
      // validInput=false;
      // return;
      searchEvent(event.getEventNumber(), event.getRunNumber(), 3);
      if (!validInput)
        return;
    }
    //       traTree[j]->GetBranch("EventNumber")->GetEntry(currentTrackEntry[j]);
    tmpev[j] = (Int_t)traTree[j]->GetLeaf("EventNumber")->GetValue();
    traTree[j]->GetEntry(currentTraEntry[j], 0);
    //      currentTraEntry[j]++;
  }
  bool same = true;
  for (int j = 0; j < numtrees; j++)
    same = same && (evNT == tmpev[j]);
  if (!same) {
    cout << "Shift in read in Tracks!!!!!" << endl;
    cout << evNT << " " << numtrees << " " << ntr << "-\t" << flush;
    for (int j = 0; j < numtrees; j++)
      cout << tmpev[j] << "\t" << flush;
    cout << endl;
  }
  if (ntr > maxTra)
    ntr = maxTra;
  event.getNumberOfTracks() = ntr;
  int w = -1;
  int which = 0;
  int beg = 0;
  int pos;
  TTrack* tr;
  for (int j = 0; j < numtrees; j++) {
    if (j <= 1)
      w = 5;
    else if (j == 2)
      w = 10;
    else if (j > 2)
      w = 20;
    for (int k = 0; k < w; k++) {
      tr = &event.getTrack(beg + k);
      read_a_track(tr, Tracks[j][k], hitsOfTrack[j][k]);
    }
  }
  which = 0;
  beg = 0;
  for (int j = 0; j < ntr; j++) {
    if (j == 5) {
      which = 1;
      beg = 5;
    }
    if (j == 10) {
      which = 2;
      beg = 10;
    }
    if ((j % 20) == 0 && (j != 0)) {
      which = (j / 20) + 2;
      beg = j;
    }
    pos = (int)fromID[which][j - beg];
    if (pos >= 0 && pos < ntr) {
      event.getTrack(j).setOrder(true);
      event.getTrack(pos).addSecondary(&event.getTrack(j));
      event.getTrack(j).setPrimary(&event.getTrack(pos));
    } else
      event.getTrack(j).setOrder(false);
    if ((!readHits) && (reconstructHits) && event.getTotalNumberOfHits() <= 0)
      reconstruct_hits_from_track(Tracks[which][j - beg], &event.getTrack(j));
  }
}
void AReadReactionFromRoot::read_rea()
{
  if (NRea == 0)
    return;
  if (!readTracks)
    event.getNumberOfTracks() = 0;
  //   event->getTrigger()=tT;
  vector3D v;
  Int_t tmpev[NRea];
  bool val = false;
  bool hasA = false;
  //  bool mee=false;
  for (int j = 0; j < NRea; j++) {
    if (RTra[j] == NULL)
      continue;
    //      cout<<j<<" "<<reactionTree[j]->GetEntries()<<"
    //      "<<currentReaEntry[j]<<"
    //      "<<event.getEventNumber()<<flush;
    if (currentReaEntry[j] > reactionTree[j]->GetEntries()) {
      if (!(Rfiles[j].size() == 1 || (int)Rfiles[j].size() == RfilePos[j] - 1))
        searchEvent(event.getEventNumber(), event.getRunNumber(), 4);
      if (!validInput)
        return;
    }
    reactionTree[j]->GetEntry(currentReaEntry[j], 0);
    tmpev[j] = (Int_t)reactionTree[j]->FindLeaf("EventNumber")->GetValue();
    if (tmpev[j] != event.getEventNumber()) {
      currentReaEntry[j]--;
      //	  cout<<endl;
      continue;
    }
    hasA = true;
    if (currentReaEntry[j] < reactionTree[j]->GetEntries())
      val = true;
    else { /*cout<<j<<" "<<event.getRunNumber()<<" "<<event.getEventNumber()<<"
              "<<currentReaEntry[j]<<endl;mee=true;*/
    }
    //      cout<<" "<<reactionNames[j].data()<<endl;
    RbaseReaction* rb = theReaction[j]->getClone();
    for (int k = 0; k < nRtr[j]; k++) {
      int num = -1;
      for (int i = 0; i < event.getNumberOfTracks(); i++) {
        v = event.getTrack(i).getPath().Direction();
        if (RTra[j][k].theta == v.Theta() && RTra[j][k].phi == v.Phi())
          num = i;
      }
      if (num < 0) {
        num = event.getNumberOfTracks();
        TTrack* tr = &event.getTrack(num);
        event.getNumberOfTracks()++;
        tr->reset();
        rb->setTrack(k, tr);
        read_a_track(tr, RTra[j][k], Rhits[j][k]);
      } else {
        rb->setTrack(j, &event.getTrack(num));
      }
      if ((!readHits) && (reconstructHits)) //&&event.getTotalNumberOfHits()<=0)
        reconstruct_hits_from_track(RTra[j][k], &event.getTrack(num));
    }
    rb->calculate();
    event.addReaction(rb);
    //      cout<<" use "<<event.getRunNumber()<<"
    //      "<<rb->getTrack(0)->getNumberOfCalibHits(15)+rb->getTrack(0)->getNumberOfCalibHits(16)<<"
    //      "<<rb->getTrack(1)->getNumberOfCalibHits(15)+rb->getTrack(1)->getNumberOfCalibHits(16)<<endl;
  }
  // if(mee)cout<<"validInput:"<<(val||(!hasA))<<endl;
  validInput = val || (!hasA);
}
void AReadReactionFromRoot::read_a_track(TTrack* tr, track1 Track, Int_t* hi)
{
  float th = Track.theta;
  float ph = Track.phi;
  int pos;
  //      if(th>0.433)ph+=-0.011;
  if (ph < -M_PI)
    ph += 2 * M_PI;
  tr->reset();
  point3D c(Track.vertex[0], Track.vertex[1], Track.vertex[2]);
  vector3D v(sin(th) * cos(ph), sin(th) * sin(ph), cos(th));
  tr->setPath(sLine3D(c, v));
  tr->setSpeed(Track.beta);
  tr->getParticler().setVM(v * tr->getSpeed(), Eparticles::getMassG(Track.ID));
  tr->setCharge(1);
  if (readHits) {
    if (hi != NULL) {
      int n = Track.nEl;
      if (n == 0)
        tr->setCharge(0);
      if (n > 30)
        n = 30;
      for (int l = 0; l < n; l++) {
        pos = hi[l];
        if (pos > numberOfHits || pos < 0)
          continue;
        int id = Hits[pos / 20][pos % 20].det;
        int el = Hits[pos / 20][pos % 20].el;
        if (id > setup.getNumberOfDetectors())
          continue;
        for (int n = 0; n < event.getNumberOfHits(id); n++) {
          if (event.getHit(id, n).getElement() == el)
            tr->addCalibSrc(&event.getHit(id, n));
        }
      }
      if (tr->getNumberOfCalibHits() != n)
        anaLog << "AReadFromSimpleTree::readTracks() Track[] wrong number of "
                  "hits: assigned :"
               << tr->getNumberOfCalibHits() << " named: " << n << endli;
    } else {
      TCalibHit* ch = NULL;
      int det;
      for (int j = 0; j < 17; j++) {
        det = (j < 8 ? j : j + 7);
        if (Track.ss[j] == -1)
          continue;
        ch = NULL;
        for (int k = 0; k < event.getNumberOfHits(det); k++) {
          ch = &event.getHit(det, k);
          if (ch->getElement() == Track.ss[j])
            break;
        }
        if (ch == NULL) {
          cout << "hit not found " << det << endl;
          continue;
        }
        if (ch->getElement() != Track.ss[j])
          continue;
        tr->addCalibSrc(ch);
      }
    }
  }
}
void AReadReactionFromRoot::reconstruct_hits_from_track(track1 Track, TTrack* tr)
{
  float qdc, tdc;
  int de = -1, el;
  int isin;
  volumeShape* sh;
  TCalibHit* th;
  //   cout<<"reconstruct hits from track"<<endl;
  //   cout<<tr<<endl;
  //   cout<<Track.ss[0]<<" "<<Track.ss[1]<<" "<<Track.ss[2]<<"
  //   "<<Track.ss[3]<<" "<<Track.ss[4]<<"
  //   "<<Track.ss[5]<<" "<<Track.ss[6]<<" "<<Track.ss[7]<<" "<<endl;
  for (int i = 0; i < 17; i++) {
    //       if(i==8||i==9)
    // 	{
    // 	  cout<<Track.ss[i]<<" "<<Track.qdc[i]<<" "<<Track.tdc[i]<<endl;
    // 	}
    el = Track.ss[i];
    qdc = Track.qdc[i];
    tdc = Track.tdc[i];
    if (el >= 0) {
      de = (i < 8 ? i : i + 7);
      th = &(event.getHit(de, event.getNumberOfHits(de)));
      isin = -1;
      for (int k = 0; k < event.getNumberOfHits(de); k++)
        if (event.getHit(de, k).getElement() == el)
          isin = k;
      if (isin >= 0) {
        tr->addCalibSrc(&event.getHit(de, isin));
        continue;
      } else if (event.getMaxNumber<TCalibHit>() > event.getNumberOfHits(de)) {
        th->setADC(qdc);
        th->setTDC(tdc);
        th->getRaw().setADC((int)qdc);
        th->getRaw().setTDC((int)tdc);
        th->getRaw().setElement(el);
        th->getRaw().setDetector(de);
        if (el < static_cast<int>(setup.getDetectorr(de).getNumberOfElements())) {
          sh = setup.getDetectorr(de).getShape(el);
          th->setHitShape(sh);
          th->setValid(true);
          tr->addCalibSrc(th);
        }
        (event.getNumberOfHits(de))++;
      }
    }
  }
  //       for(int j=0;j<25;j++)cout<<tr->getNumberOfCalibHits(j)<<" "<<flush;
  //       cout<<endl;
}

void AReadReactionFromRoot::searchEvent(int eventNumber, int runNumber, int mode)
{
  //  cout<<"AReadReactionFromRoot::searchEvent()"<<endl;
  if (mode == 4) // search reacktion tree
  {
    int runBegin = 0, runEnd = 0;
    int nrL, nrH;
    TBranch *eventNrBranch, *runNrBranch;
    bool hasA = false;
    for (int i = 0; i < NRea; i++) {
      if (RTra[i] == NULL) {
        if (Rfiles[i].size() == 0)
          continue;
        // cleanARea(i);
        set_rea(Rfiles[i][RfilePos[i]], i, reactionNames[i]);
      }
      eventNrBranch = reactionTree[i]->GetBranch("EventNumber");
      runNrBranch = reactionTree[i]->GetBranch("RunNumber");

      runNrBranch->GetEntry(0);
      if (ruNR[i] > runNumber)
        continue;
      if (ruNR[i] == runNumber)
        runBegin = 0;
      else // search runBegin
        searchPosInTree(runNrBranch, 0, reactionTree[i]->GetEntries() - 1, runNumber, ruNR[i], 1);
      runNrBranch->GetEntry(reactionTree[i]->GetEntries() - 1);
      if (ruNR[i] < runNumber)
        continue;
      if (ruNR[i] == runNumber)
        runEnd = reactionTree[i]->GetEntries() - 1;
      else // search end
        searchPosInTree(runNrBranch, runBegin, reactionTree[i]->GetEntries() - 1, runNumber,
                        ruNR[i], 2);
      eventNrBranch->GetEntry(runBegin);
      nrL = evNR[i];
      eventNrBranch->GetEntry(runEnd);
      nrH = evNR[i];
      //	  cout<<eventNumber<<" "<<runBegin<<"-"<<runEnd<<"
      //"<<nrL<<"-"<<nrH<<"
      //"<<Rfiles[i].size()<<endl;
      currentReaEntry[i] = runBegin;
      if (nrL == eventNumber)
        currentReaEntry[i] = runBegin;
      else if (nrH == eventNumber)
        currentReaEntry[i] = runEnd;
      else if (nrL > eventNumber) {
        if (Rfiles[i].size() <= 1)
          currentReaEntry[i] = runBegin;
        else {
          if (RfilePos[i] == 0) {
            continue;
          }
          RfilePos[i]--;
          cleanARea(i);
          set_rea(Rfiles[i][RfilePos[i]], i, reactionNames[i]);
          searchEvent(eventNumber, runNumber, mode);
          continue;
        }
      } else if (nrH < eventNumber) {
        if (Rfiles[i].size() <= 1) {
          continue;
        } else {
          if (RfilePos[i] == (int)Rfiles[i].size() - 1) {
            continue;
          }
          RfilePos[i]++;
          cleanARea(i);
          set_rea(Rfiles[i][RfilePos[i]], i, reactionNames[i]);
          searchEvent(eventNumber, runNumber, mode);
          continue;
        }
      } else {
        currentReaEntry[i] =
            searchPosInTree(eventNrBranch, runBegin, runEnd, eventNumber, evNR[i], 0);
      }
      hasA = true;
    }
    if (!hasA)
      validInput = false;
    return;
  }
  //  TTree *numTree=NULL;
  TTree** dataTree = NULL;
  int nData = 0;
  Int_t* currentEntry = NULL;
  //  Int_t *nextEntry=NULL;
  //  TFile *treeFile=NULL;
  Int_t* evtNr = NULL;
  Int_t* runNr = NULL;
  TBranch* eventNrBranch = NULL;
  TBranch* runNrBranch = NULL;
  bool hasA = false;
  switch (mode) {
  case 0: {
    if (Hits == NULL)
      set_hit((files[0])[filePos[0]]);
    //	numTree=numHitTree;
    dataTree = hitTree;
    nData = NHit;
    currentEntry = currentHitEntry;
    //	nextEntry=nextHitEntry;
    //	treeFile=hitf;
    evtNr = &evNH;
    runNr = &ruNH;
    hasA = false;
    break;
  }
  case 1: {
    if (Pix == NULL)
      set_pix(files[1][filePos[1]]);
    //	numTree=numPixTree;
    dataTree = pixTree;
    nData = NPix;
    currentEntry = currentPixEntry;
    //	nextEntry=nextPixEntry;
    //	treeFile=pixf;
    evtNr = &evNP;
    runNr = &ruNP;
    hasA = readHits;
    break;
  }
  case 2: {
    if (Clus == NULL)
      set_clu(files[2][filePos[2]]);
    //	numTree=numCluTree;
    dataTree = cluTree;
    nData = NClu;
    currentEntry = currentCluEntry;
    //	nextEntry=nextCluEntry;
    //	treeFile=cluf;
    evtNr = &evNC;
    runNr = &ruNC;
    hasA = readHits || readPixel;
    break;
  }
  case 3: {
    if (Tracks == NULL)
      set_tra(files[3][filePos[3]]);
    //	numTree=numTraTree;
    dataTree = traTree;
    nData = NTra;
    currentEntry = currentTraEntry;
    //	nextEntry=nextTraEntry;
    //	treeFile=traf;
    evtNr = &evNT;
    runNr = &ruNT;
    hasA = readHits || readPixel || readCluster;
    break;
  }
  }
  int runBegin = 0, runEnd = 0;
  int nrL, nrH;
  for (int i = 0; i < nData; i++) {
    eventNrBranch = dataTree[i]->GetBranch("EventNumber");
    runNrBranch = dataTree[i]->GetBranch("RunNumber");

    runNrBranch->GetEntry(0);
    if (*runNr > runNumber)
      continue;
    if (*runNr == runNumber)
      runBegin = 0;
    else // search runBegin
      searchPosInTree(runNrBranch, 0, dataTree[i]->GetEntries() - 1, runNumber, *runNr, 1);
    runNrBranch->GetEntry(dataTree[i]->GetEntries() - 1);
    if (*runNr < runNumber)
      continue;
    if (*runNr == runNumber)
      runEnd = dataTree[i]->GetEntries() - 1;
    else // search end
      searchPosInTree(runNrBranch, runBegin, dataTree[i]->GetEntries() - 1, runNumber, *runNr, 2);
    eventNrBranch->GetEntry(runBegin);
    nrL = *evtNr;
    eventNrBranch->GetEntry(runEnd);
    nrH = *evtNr;
    if (nrL == eventNumber)
      currentEntry[i] = runBegin;
    else if (nrH == eventNumber)
      currentEntry[i] = runEnd;
    else if (nrL > eventNumber) {
      if (files[i].size() <= 1)
        currentEntry[i] = runBegin;
      else {
        if (filePos[mode] == 0)
          continue;
        filePos[mode]--;
        switch (mode) {
        case 0:
          cleanHit();
          set_hit(files[0][filePos[0]]);
          break;
        case 1:
          cleanPix();
          set_pix(files[1][filePos[1]]);
          break;
        case 2:
          cleanClu();
          set_clu(files[2][filePos[2]]);
          break;
        case 3:
          cleanTra();
          set_tra(files[3][filePos[3]]);
          break;
        }
        searchEvent(eventNumber, runNumber, mode);
        return;
      }
    } else if (nrH < eventNumber) {
      if (files[i].size() <= 1)
        currentEntry[i] = runEnd;
      else {
        if (filePos[mode] == (int)files[mode].size() - 1)
          continue;
        filePos[mode]++;
        switch (mode) {
        case 0:
          cleanHit();
          set_hit(files[0][filePos[0]]);
          break;
        case 1:
          cleanPix();
          set_pix(files[1][filePos[1]]);
          break;
        case 2:
          cleanClu();
          set_clu(files[2][filePos[2]]);
          break;
        case 3:
          cleanTra();
          set_tra(files[3][filePos[3]]);
          break;
        }
        searchEvent(eventNumber, runNumber, mode);
        continue;
      }
    } else {
      currentEntry[i] = searchPosInTree(eventNrBranch, runBegin, runEnd, eventNumber, *evtNr, 0);
    }
    hasA = true;
  }
  if (!hasA)
    validInput = false;
}
void AReadReactionFromRoot::getNextEvent()
{
  if (readMinimum) {
    bool hasA = false;
    //      reactionTree[0]->GetEntry(++currentReaEntry[0]);
    int nextEventNumber = INT_MAX; //(Int_t)reactionTree[0]->GetLeaf("EventNumber")->GetValue();
    for (int i = 0; i < NRea; i++) {
      if (RTra[i] == NULL)
        continue;
      hasA = true;
      reactionTree[i]->GetEntry(++currentReaEntry[i]);
      if (nextEventNumber > reactionTree[i]->GetLeaf("EventNumber")->GetValue()) {
        nextEventNumber = (Int_t)reactionTree[i]->GetLeaf("EventNumber")->GetValue();
      }
    }
    event.setEventNumber(nextEventNumber);
    validInput = hasA;
  } else {
    reactionTree[0]->GetEntry(++currentReaEntry[0]);
    int nextEventNumber = (Int_t)reactionTree[0]->GetLeaf("EventNumber")->GetValue();
    for (int i = 1; i < NRea; i++) {
      reactionTree[i]->GetEntry(++currentReaEntry[i]);
      if (nextEventNumber > reactionTree[i]->GetLeaf("EventNumber")->GetValue()) {
        nextEventNumber = (Int_t)reactionTree[i]->GetLeaf("EventNumber")->GetValue();
      }
    }
    if (readHits) {
      numHitTree->GetBranch("EventNumber")->GetEntry(currentHitEntry[0] + 1);
      TLeaf* evtLeaf = numHitTree->GetLeaf("EventNumber");
      if (evtLeaf->GetValue() < nextEventNumber)
        nextEventNumber = (Int_t)evtLeaf->GetValue();
    }
    if (readPixel) {
      numPixTree->GetBranch("EventNumber")->GetEntry(currentPixEntry[0] + 1);
      TLeaf* evtLeaf = numPixTree->GetLeaf("EventNumber");
      if (evtLeaf->GetValue() < nextEventNumber)
        nextEventNumber = (Int_t)evtLeaf->GetValue();
    }
    if (readCluster) {
      numCluTree->GetBranch("EventNumber")->GetEntry(currentCluEntry[0] + 1);
      TLeaf* evtLeaf = numCluTree->GetLeaf("EventNumber");
      if (evtLeaf->GetValue() < nextEventNumber)
        nextEventNumber = (Int_t)evtLeaf->GetValue();
    }
    if (readTracks) {
      numTraTree->GetBranch("EventNumber")->GetEntry(currentTraEntry[0] + 1);
      TLeaf* evtLeaf = numTraTree->GetLeaf("EventNumber");
      if (evtLeaf->GetValue() < nextEventNumber)
        nextEventNumber = (Int_t)evtLeaf->GetValue();
    }
    event.setEventNumber(nextEventNumber);
  }
}
void AReadReactionFromRoot::sortFiles()
{
  vector<int> begR, begE, endR, endE;
  vector<bool> used;
  vector<string> tmp;
  string treenames[4] = {"numberOfHits", "numberOfPixels", "numberOfClusters", "numberOfTracks"};
  TLeaf *branE, *branR;
  TTree* tree;
  int filesToSort;
  int minPos;
  for (int i = 0; i < 4; i++) {
    if (files[i].size() < 2) {
      if (files[i].size() == 0)
        continue;
      switch (i) {
      case 0:
        set_hit((files[0])[filePos[0]]);
        break;
      case 1:
        set_pix((files[1])[filePos[1]]);
        break;
      case 2:
        set_clu((files[2])[filePos[2]]);
        break;
      case 3:
        set_tra((files[3])[filePos[3]]);
        break;
      }
      continue;
    }
    for (unsigned int j = 0; j < files[i].size(); j++) {
      used.push_back(false);
      TFile f(files[i][j].data(), "read");
      tree = (TTree*)f.Get(treenames[i].data());
      branE = tree->GetLeaf("EventNumber");
      branR = tree->GetLeaf("RunNumber");
      branE->GetBranch()->GetEntry(0);
      branR->GetBranch()->GetEntry(0);
      begR.push_back((Int_t)branR->GetValue());
      begE.push_back((Int_t)branE->GetValue());
      branE->GetBranch()->GetEntry(tree->GetEntries() - 1);
      branR->GetBranch()->GetEntry(tree->GetEntries() - 1);
      endR.push_back((Int_t)branR->GetValue());
      endE.push_back((Int_t)branE->GetValue());
    }
    filesToSort = files[i].size();
    while (filesToSort > 1) {
      minPos = -1;
      for (unsigned int j = 0; j < files[i].size(); j++) {
        if (used[j] == true)
          continue;
        if (minPos == -1)
          minPos = j;
        else {
          if (begR[j] == begR[minPos] && endR[j] == endR[minPos]) {
            if (begE[j] <= begE[minPos]) {
              if (endE[j] > endE[minPos]) {
                used[minPos] = true;
                filesToSort--;
              }
              minPos = j;
            }
            if (begE[j] > begE[minPos] && endE[j] <= endE[minPos]) {
              used[j] = true;
              filesToSort--;
            }
          } else if (begR[j] < begR[minPos]) {
            if (endR[j] > endR[minPos]) {
              used[minPos] = true;
              filesToSort--;
            }
            minPos = j;
          } else if (begR[j] > begR[minPos] && endR[j] <= endR[minPos]) {
            used[j] = true;
            filesToSort--;
          }
        }
      }
      if (minPos >= 0) {
        tmp.push_back(files[i][minPos]);
        used[minPos] = true;
        filesToSort--;
      }
    }
    while (!files[i].empty())
      files[i].pop_back();
    while (!used.empty())
      used.pop_back();
    while (!begR.empty())
      begR.pop_back();
    while (!begE.empty())
      begE.pop_back();
    while (!endR.empty())
      endR.pop_back();
    while (!endE.empty())
      endE.pop_back();
    for (unsigned int j = 0; j < tmp.size(); j++)
      files[i].push_back(tmp[j]);
    while (!tmp.empty())
      tmp.pop_back();
  }
  for (int i = 0; i < NRea; i++) {
    if (Rfiles[i].size() == 0) {
      continue;
    }
    if (Rfiles[i].size() < 2) {
      set_rea(Rfiles[i][0], i, reactionNames[i]);
      continue;
    }
    filesToSort = Rfiles[i].size();
    for (unsigned int j = 0; j < Rfiles[i].size(); j++) {
      used.push_back(false);
      TFile f(Rfiles[i][j].data(), "read");
      tree = (TTree*)f.Get(reactionNames[i].data());
      if (tree == NULL) {
        used.back() = true;
        filesToSort--;
        begR.push_back(0);
        begE.push_back(0);
        endR.push_back(0);
        endE.push_back(0);
      } else {
        branE = tree->GetLeaf("EventNumber");
        branR = tree->GetLeaf("RunNumber");
        branE->GetBranch()->GetEntry(0);
        branR->GetBranch()->GetEntry(0);
        begR.push_back((Int_t)branR->GetValue());
        begE.push_back((Int_t)branE->GetValue());
        branE->GetBranch()->GetEntry(tree->GetEntries() - 1);
        branR->GetBranch()->GetEntry(tree->GetEntries() - 1);
        endR.push_back((Int_t)branR->GetValue());
        endE.push_back((Int_t)branE->GetValue());
      }
    }
    while (filesToSort > 1) {
      minPos = -1;
      for (unsigned int j = 0; j < files[i].size(); j++) {
        if (used[j] == true)
          continue;
        if (minPos == -1)
          minPos = j;
        else {
          if (begR[j] == begR[minPos] && endR[j] == endR[minPos]) {
            if (begE[j] <= begE[minPos]) {
              if (endE[j] > endE[minPos]) {
                used[minPos] = true;
                filesToSort--;
              }
              minPos = j;
            }
            if (begE[j] > begE[minPos] && endE[j] <= endE[minPos]) {
              used[j] = true;
              filesToSort--;
            }
          } else if (begR[j] < begR[minPos]) {
            if (endR[j] > endR[minPos]) {
              used[minPos] = true;
              filesToSort--;
            }
            minPos = j;
          } else if (begR[j] > begR[minPos] && endR[j] <= endR[minPos]) {
            used[j] = true;
            filesToSort--;
          }
        }
      }
      if (minPos >= 0) {
        tmp.push_back(Rfiles[i][minPos]);
        used[minPos] = true;
        filesToSort--;
      }
    }
    while (!Rfiles[i].empty())
      Rfiles[i].pop_back();
    for (unsigned int j = 0; j < tmp.size(); j++)
      Rfiles[i].push_back(tmp[j]);
    while (!tmp.empty())
      tmp.pop_back();
    while (!used.empty())
      used.pop_back();
    while (!begR.empty())
      begR.pop_back();
    while (!begE.empty())
      begE.pop_back();
    while (!endR.empty())
      endR.pop_back();
    while (!endE.empty())
      endE.pop_back();
  }
}
AReadReactionFromRoot::AReadReactionFromRoot(TSetup& setupIn, TEvent& eventIn, bool& validInputIn,
                                             const algorithm_parameter& param)
    : AAlgorithm("Read reaction properties from root file"), setup(setupIn), event(eventIn),
      maxHit(eventIn.getMaxNumber<TCalibHit>()), maxPix(eventIn.getMaxNumber<TPixel>()),
      maxClu(eventIn.getMaxNumber<TCluster>()), maxTra(eventIn.getMaxNumber<TTrack>()),
      validInput(validInputIn)
{
  readHits = param.getParam<bool>(0).getData();
  readPixel = param.getParam<bool>(1).getData();
  readCluster = param.getParam<bool>(2).getData();
  readTracks = param.getParam<bool>(3).getData();
  reconstructHits = param.getParam<bool>(4).getData();
  readMinimum = param.getParam<bool>(5).getData();
  NRea = param.getParam<vector<string>>(0).getData().size();
  reactionNames = new string[NRea];
  for (int i = 0; i < NRea; i++)
    reactionNames[i] = param.getParam<vector<string>>(0).getData().at(i);
  init();
}
AReadReactionFromRoot::AReadReactionFromRoot(TSetup& setupIn, TEvent& eventIn, bool& validInputIn,
                                             bool readH, bool readP, bool readC, bool readT,
                                             bool minimum, bool reconstruct, vector<string> Rnames)
    : AAlgorithm("Read reaction properties from root file"), setup(setupIn), event(eventIn),
      maxHit(eventIn.getMaxNumber<TCalibHit>()), maxPix(eventIn.getMaxNumber<TPixel>()),
      maxClu(eventIn.getMaxNumber<TCluster>()), maxTra(eventIn.getMaxNumber<TTrack>()),
      validInput(validInputIn)
{
  readHits = readH;
  readPixel = readP;
  readCluster = readC;
  readTracks = readT;
  reconstructHits = reconstruct;
  readMinimum = minimum;
  NRea = Rnames.size();
  reactionNames = new string[NRea];
  for (int i = 0; i < NRea; i++)
    reactionNames[i] = Rnames[i];
  init();
}
void AReadReactionFromRoot::init()
{
  reactionTree = new TTree*[NRea];
  theReaction = new RbaseReaction*[NRea];
  reactionf = new TFile*[NRea];
  momentum4D p[2];
  p[0] = setup.getBeamr(0).getParticle();
  if (setup.hasTarget())
    p[1] = setup.getTargetr().getParticle();
  else
    p[1] = setup.getBeamr(1).getParticle();
  for (int i = 0; i < NRea; i++) {
    theReaction[i] = getReactionFromName(reactionNames[i]);
    if (theReaction[i] != NULL)
      theReaction[i]->setInitParticles(2, p);
  }
  for (int i = 0; i < 5; i++)
    files.push_back(vector<string>());
  filePos = new int[5];
  RfilePos = new int[NRea];
  for (int i = 0; i < NRea; i++)
    Rfiles.push_back(vector<string>());
  for (int i = 0; i < 4; i++)
    filePos[i] = 0;
  for (int i = 0; i < NRea; i++)
    RfilePos[i] = 0;
  Hits = NULL;
  Pix = NULL;
  Clus = NULL;
  Tracks = NULL;
  RTra = new track1*[NRea];
  for (int i = 0; i < NRea; i++)
    RTra[i] = NULL;
  RFrom = new Int_t*[NRea];
  Rhits = new Int_t**[NRea];
  nRtr = new Int_t[NRea];
  evNR = new Int_t[NRea];
  ruNR = new Int_t[NRea];
  tR = new Int_t[NRea];
  numHitTree = NULL;
  numPixTree = NULL;
  numCluTree = NULL;
  numTraTree = NULL;
  currentHitEntry = NULL;
  currentPixEntry = NULL;
  currentCluEntry = NULL;
  currentTraEntry = NULL;
  currentReaEntry = new Int_t[NRea];
  nextHitEntry = NULL;
  nextPixEntry = NULL;
  nextCluEntry = NULL;
  nextTraEntry = NULL;
  nextReaEntry = new Int_t[NRea];
}
AReadReactionFromRoot::~AReadReactionFromRoot()
{
  cleanHit();
  cleanPix();
  cleanClu();
  cleanTra();
  cleanRea();
  delete[] filePos;
  delete[] RfilePos;
  delete[] reactionNames;
  for (int i = 0; i < NRea; i++)
    if (theReaction[i] != NULL)
      delete theReaction[i];
  delete[] theReaction;
}
void* AReadReactionFromRoot::process(void* ptr)
{
  getNextEvent();
  if (!validInput)
    return ptr;
  // cout<<"nextEvent()="<<event.getEventNumber()<<endl;
  int nextEventNumber = event.getEventNumber();
  TBranch *evtBranch, *numBranch;
  bool val = false;
  if (readHits) {
    evtBranch = numHitTree->GetBranch("EventNumber");
    numBranch = numHitTree->GetBranch("NumberOfHits");
    for (int i = 0; i < NHit; i++)
      if (currentHitEntry[i] < 0)
        currentHitEntry[i] = 0;
    evtBranch->GetEntry(currentHitEntry[0]);
    numBranch->GetEntry(currentHitEntry[0]);
    while (evNH < nextEventNumber) {
      for (int i = 0; i < ((int)numberOfHits / 20) + 1; i++)
        currentHitEntry[i]++;
      if (currentHitEntry[0] < numHitTree->GetEntries()) {
        evtBranch->GetEntry(currentHitEntry[0]);
        numBranch->GetEntry(currentHitEntry[0]);
      } else
        evNH = INT_MAX;
    }
    if (evNH == nextEventNumber)
      read_hit();
    else {
      evtBranch->GetEntry(--currentHitEntry[0]);
      numBranch->GetEntry(currentHitEntry[0]);
      for (int i = 1; i < ((int)numberOfHits / 20) + 1; i++)
        currentHitEntry[i]--;
    }
    if (currentHitEntry[0] < numHitTree->GetEntries())
      val = true;
  }
  if (readPixel) {
    evtBranch = numPixTree->GetBranch("EventNumber");
    numBranch = numPixTree->GetBranch("NumberOfHits");
    for (int i = 0; i < NPix; i++)
      if (currentPixEntry[i] < 0)
        currentPixEntry[i] = 0;
    evtBranch->GetEntry(currentPixEntry[0]);
    numBranch->GetEntry(currentPixEntry[0]);
    while (evNP < nextEventNumber) {
      for (int i = 0; i < ((int)numberOfPixels / 20) + 1; i++)
        currentPixEntry[i]++;
      if (currentPixEntry[0] < numPixTree->GetEntries()) {
        evtBranch->GetEntry(currentPixEntry[0]);
        numBranch->GetEntry(currentPixEntry[0]);
      } else
        evNP = INT_MAX;
    }
    if (evNP == nextEventNumber)
      read_pix();
    else {
      evtBranch->GetEntry(--currentPixEntry[0]);
      numBranch->GetEntry(currentPixEntry[0]);
      for (int i = 1; i < ((int)numberOfPixels / 20) + 1; i++)
        currentPixEntry[i]--;
    }
    if (currentPixEntry[0] < numPixTree->GetEntries())
      val = true;
  }
  if (readCluster) {
    evtBranch = numCluTree->GetBranch("EventNumber");
    numBranch = numCluTree->GetBranch("NumberOfHits");
    for (int i = 0; i < NClu; i++)
      if (currentCluEntry[i] < 0)
        currentCluEntry[i] = 0;
    evtBranch->GetEntry(currentCluEntry[0]);
    numBranch->GetEntry(currentCluEntry[0]);
    while (evNC < nextEventNumber) {
      for (int i = 0; i < ((int)numberOfCluster / 20) + 1; i++)
        currentCluEntry[i]++;
      if (currentCluEntry[0] < numCluTree->GetEntries()) {
        evtBranch->GetEntry(currentCluEntry[0]);
        numBranch->GetEntry(currentCluEntry[0]);
      } else
        evNC = INT_MAX;
    }
    if (evNC == nextEventNumber)
      read_clu();
    else {
      evtBranch->GetEntry(--currentCluEntry[0]);
      numBranch->GetEntry(currentCluEntry[0]);
      for (int i = 1; i < ((int)numberOfCluster / 20) + 1; i++)
        currentCluEntry[i]--;
    }
    if (currentCluEntry[0] < numCluTree->GetEntries())
      val = true;
  }
  if (readTracks) {
    evtBranch = numTraTree->GetBranch("EventNumber");
    numBranch = numTraTree->GetBranch("NumberOfHits");
    for (int i = 0; i < NTra; i++)
      if (currentTraEntry[i] < 0)
        currentTraEntry[i] = 0;
    evtBranch->GetEntry(currentTraEntry[0]);
    numBranch->GetEntry(currentTraEntry[0]);
    int ntree;
    while (evNT < nextEventNumber) {
      ntree = getNtrees(ntr, 3);
      for (int i = 0; i < ntree; i++)
        currentTraEntry[i]++;
      if (currentTraEntry[0] < numTraTree->GetEntries()) {
        evtBranch->GetEntry(currentTraEntry[0]);
        numBranch->GetEntry(currentTraEntry[0]);
      } else
        evNT = INT_MAX;
    }
    if (evNT == nextEventNumber)
      read_tra();
    else {
      evtBranch->GetEntry(--currentTraEntry[0]);
      numBranch->GetEntry(currentTraEntry[0]);
      ntree = getNtrees(ntr, 3);
      for (int i = 1; i < ntree; i++)
        currentTraEntry[i]--;
    }
    if (currentTraEntry[0] < numTraTree->GetEntries())
      val = true;
  }
  validInput = val;
  read_rea();
  return ptr;
}
void AReadReactionFromRoot::OnNewRun(run_parameter& r)
{
  //   if(currentHitEntry!=NULL)
  //     cout<<currentHitEntry[0]<<" "<<currentReaEntry[0]<<"
  //     "<<currentReaEntry[1]<<endl;
  anaLog << "next run for read-in: " << r.getName().data() << endli;
  for (int i = 0; i < 4; i++)
    while (!files[i].empty())
      files[i].pop_back();
  for (int i = 0; i < NRea; i++)
    while (!Rfiles[i].empty())
      Rfiles[i].pop_back();
  string s;
  for (int i = 0; i < r.getNumberOfFiles(); i++) {
    s = r.getFile(i);
    switch (r.getFileType(i)) {
    case 6: // hits
      if (readHits && existing(s))
        files[0].push_back(s);
      break;
    case 10: // pixels
      if (readPixel && existing(s))
        files[1].push_back(s);
      break;
    case 18: // cluster
      if (readCluster && existing(s))
        files[2].push_back(s);
      break;
    case 34: // tracks
      if (readTracks && existing(s))
        files[3].push_back(s);
      break;
    case 66: // reactions
      if (existing(s)) {
        TFile f(s.data(), "read");
        for (int j = 0; j < NRea; j++)
          if (f.Get(reactionNames[j].data()) != NULL)
            Rfiles[j].push_back(s);
        f.Close();
      }
      break;
    }
  }
  // cout<<files[0].size()<<" "<<files[1].size()<<" "<<files[2].size()<<"
  // "<<files[3].size()<<"
  // "<<Rfiles[0].size()<<endl;
  event.setRunNumber(r.getRunNumber());
  for (int i = 0; i < 4; i++)
    filePos[i] = 0;
  //  cout<<"sortFiles"<<endl;
  sortFiles();
  if (readHits) {
    cleanHit();
    searchEvent(0, r.getRunNumber(), 0);
  }
  if (readPixel) {
    cleanPix();
    searchEvent(0, r.getRunNumber(), 1);
  }
  if (readCluster) {
    cleanClu();
    searchEvent(0, r.getRunNumber(), 2);
  }
  if (readTracks) {
    cleanTra();
    searchEvent(0, r.getRunNumber(), 3);
  }
  for (int i = 0; i < NRea; i++)
    cleanARea(i);
  //  cout<<"searchEvent-Reactions"<<endl;
  searchEvent(0, r.getRunNumber(), 4);
  // cout<<"done"<<endl;
  for (int i = 0; i < NRea; i++)
    currentReaEntry[i]--;
  // cout<<r.getRunNumber()<<" : "<<reactionTree[0]->GetEntries()<<"
  // "<<reactionTree[1]->GetEntries()<<endl;
  event.setEventNumber(0);
  validInput = true;
}
