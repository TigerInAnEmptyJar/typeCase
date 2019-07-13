#include "AReadFromSimpleTree.h"
#include "Eparticles.h"
#include "logger.h"
#include <TLeaf.h>
//#include <qfile.h>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <stdlib.h>
bool rever;
extern bool existing(string filename);
AReadFromSimpleTree::AReadFromSimpleTree(
    const string& fileName, const string& treename, const string& localDirec, bool useLocalD,
    TTrack** tracksIn, int& numberOfTracksIn, int& eventNumberIn, int maxTracksIn, TEvent& eventIn,
    int& maxEventsToRead, TSetup* setupIn, bool readT, bool readC, bool readP, bool readH,
    string TrackFile, string ClusterFile, string PixelFile, string HitFile, bool resetUnreadIn,
    bool startWithDelayedEventIn, int StartWithEventIn, bool& validInputIn, void* inputmutexIn)
    : AAlgorithm("Read track info from simple tree"), maxTrack(maxTracksIn),
      numberOfTracks(numberOfTracksIn), maxOfHits(eventIn.getMaxNumber<TCalibHit>()),
      maxOfPixel(eventIn.getMaxNumber<TPixel>()), maxOfCluster(eventIn.getMaxNumber<TCluster>()),
      validInput(validInputIn)
{
  if (fileName == treename && maxEventsToRead == 0)
    currentHitEntry = NULL;
  currentHitEntry = NULL;
  currentPixEntry = NULL;
  currentCluEntry = NULL;
  currentTrackEntry = NULL;
  inputmutex = inputmutexIn;
  rever = false;
  event = &eventIn;
  eventNumber = &eventNumberIn;
  readTracks = readT;
  readPixels = readP;
  readClusters = readC;
  readHits = readH;
  resetUnread = resetUnreadIn;
  c1 = -1;
  c2 = -1;
  c3 = -1;
  c4 = -1;
  numTrackTree = NULL;
  numPixTree = NULL;
  numCluTree = NULL;
  numHitTree = NULL;
  setup = (setupIn);
  numTrackTrees = 0;
  numCluTrees = 0;
  numPixTrees = 0;
  numHitTrees = 0;
  localDirectory = useLocalD;
  localDir = localDirec;
  if (localDirectory) {
    if (localDir == "")
      localDirectory = false;
    else if (!existing(localDir)) {
      QDir directory(localDir.data());
      if (!directory.exists()) {
        string tmpPath = localDir;
        vector<string> components;
        if (!directory.isRelative()) {
          tmpPath = tmpPath.substr(1, tmpPath.length() - 1);
          components.push_back(string("/") + tmpPath.substr(0, tmpPath.find("/")));
          tmpPath = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length() - tmpPath.find("/") - 1);
        } else {
          components.push_back(tmpPath.substr(0, tmpPath.find("/")));
          tmpPath = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length() - tmpPath.find("/") - 1);
        }
        while (tmpPath.find("/") < tmpPath.npos) {
          components.push_back(tmpPath.substr(0, tmpPath.find("/")));
          tmpPath = tmpPath.substr(tmpPath.find("/") + 1, tmpPath.length() - tmpPath.find("/") - 1);
        }
        if (tmpPath != "")
          components.push_back(tmpPath);
        tmpPath = components[0];
        QDir tmp(tmpPath.data());
        if (!tmp.exists()) {
          tmp.cdUp();
          tmp.mkdir(tmpPath.data());
        }
        tmpPath += "/";
        for (unsigned int i = 1; i < components.size(); i++) {
          tmp = (tmpPath + components[i]).data();
          if (!tmp.exists()) {
            QDir tmp2(tmpPath.data());
            tmp2.mkdir(components[i].data());
          }
          tmpPath += components[i] + "/";
        }
      }
    }
  }
  filenames.push_back(""); // hitfile
  filenames.push_back(""); // pixelfile
  filenames.push_back(""); // clusterfile
  filenames.push_back(""); // trackfile
  if (readTracks) {
    if (!existing(TrackFile)) {
      anaLog << "track file not found " << TrackFile << endli;
      readTracks = false;
    } else {
      if (localDirectory) {
        filenames[3] = localDir + "/" +
                       TrackFile.substr(TrackFile.rfind("/") + 1,
                                        TrackFile.length() - TrackFile.rfind("/") - 1);
#if QT_VERSION < 0x040000
        QProcess copy(QString("cp "));
        copy.addArgument((TrackFile.data()));
        copy.addArgument(filenames[3].data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start(QString((string("cp ") + TrackFile + " " + filenames[3]).data()));
        while (copy.waitForFinished()) {
        }
#endif
        cout << "Trackfile copied to local directory" << endl;
      } else
        filenames[3] = TrackFile;
      trackf = new TFile(filenames[3].data(), "READ");
      numTrackTree = (TTree*)trackf->Get("numberOfTracks");
      numTrackTree->SetBranchAddress("EventNumber", &evNT);
      numTrackTree->SetBranchAddress("NumberOfTracks", &ntr);
      // 	  numTrackTree->SetBranchAddress("Trigger",&tT);
      readTracks = setTrackTree(trackf);
      if (startWithDelayedEventIn > 0) {
        c1 = searchDelayedEvent(trackTree, numTrackTrees, StartWithEventIn, currentTrackEntry, c1);
        numTrackTree->GetEntry(c1 + 1);
        cout << "starting event number(Tracks): " << evNT << " Entry:" << c1 << endl;
        //	      int nt=getNumTrees(ntr,3);
        // 	      for(unsigned int i=nt;i<trackTree.size();i++)
        // 		currentTrackEntry[i]++;
      }
      if (numTrackTrees == 0)
        cout << "no tree \"Track\" found in " << TrackFile.data() << endl;
      else {
        tracks = tracksIn;
        counter = 0;
      }
    }
  }
  if (readPixels) {
    if (!existing(PixelFile))
      readPixels = false;
    else {
      if (localDirectory) {
        filenames[1] = localDir + "/" +
                       PixelFile.substr(PixelFile.rfind("/") + 1,
                                        PixelFile.length() - PixelFile.rfind("/") - 1);
#if QT_VERSION < 0x040000
        QProcess copy(QString("cp "));
        copy.addArgument(PixelFile.data());
        copy.addArgument(filenames[1].data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start(QString((string("cp ") + PixelFile + " " + filenames[1]).data()));
        while (copy.waitForFinished()) {
        }
#endif
      } else
        filenames[1] = PixelFile;
      pixelf = new TFile(filenames[1].data(), "READ");
      numPixTree = (TTree*)pixelf->Get("numberOfPixels");
      numPixTree->SetBranchAddress("EventNumber", &evNP);
      numPixTree->SetBranchAddress("NumberOfPixels", &numberOfPixels);
      numPixTree->SetBranchAddress("Trigger", &tP);
      readPixels = setPixelTree(pixelf);
      if (startWithDelayedEventIn > 0)
        c3 = searchDelayedEvent(pixTree, numPixTrees, StartWithEventIn, currentPixEntry, c3);
    }
  }
  if (readClusters) {
    if (!existing(ClusterFile))
      readClusters = false;
    else {
      if (localDirectory) {
        filenames[2] = localDir + "/" +
                       ClusterFile.substr(ClusterFile.rfind("/") + 1,
                                          ClusterFile.length() - ClusterFile.rfind("/") - 1);
#if QT_VERSION < 0x040000
        QProcess copy(QString("cp "));
        copy.addArgument(ClusterFile.data());
        copy.addArgument(filenames[2].data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start(QString((string("cp ") + ClusterFile + " " + filenames[2]).data()));
        while (copy.waitForFinished()) {
        }
#endif
      } else
        filenames[2] = ClusterFile;
      clusterf = new TFile(filenames[2].data(), "READ");
      numCluTree = (TTree*)clusterf->Get("numberOfCluster");
      numCluTree->SetBranchAddress("EventNumber", &evNC);
      numCluTree->SetBranchAddress("NumberOfCluster", &numberOfCluster);
      numCluTree->SetBranchAddress("Trigger", &tC);
      readClusters = setClusterTree(clusterf);
      if (startWithDelayedEventIn > 0)
        c2 = searchDelayedEvent(cluTree, numCluTrees, StartWithEventIn, currentCluEntry, c2);
    }
  }
  if (readHits) {
    if (!existing(HitFile)) {
      anaLog << "hit file not found " << HitFile << endli;
      readHits = false;
    } else {
      anaLog << "read Hits from file: " << HitFile.data() << endli;
      if (localDirectory) {
        filenames[0] =
            localDir + "/" +
            HitFile.substr(HitFile.rfind("/") + 1, HitFile.length() - HitFile.rfind("/") - 1);
        anaLog << "using the local directory " << localDir << endli;
        anaLog << "==>" << filenames[0] << endli;
#if QT_VERSION < 0x040000
        QProcess copy(QString("cp "));
        copy.addArgument(HitFile.data());
        copy.addArgument(filenames[0].data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start(QString((string("cp ") + HitFile + " " + filenames[0]).data()));
        while (copy.waitForFinished()) {
        }
#endif
      } else
        filenames[0] = HitFile;
      hitf = new TFile(filenames[0].data(), "READ");
      numHitTree = (TTree*)hitf->Get("numberOfHits");
      numHitTree->SetBranchAddress("EventNumber", &evNH);
      numHitTree->SetBranchAddress("NumberOfHits", &numberOfHits);
      numHitTree->SetBranchAddress("Trigger", &tH);
      numHitTree->GetEntry(0, 0);
      readHits = setHitTree(hitf);
      setHitStruct();
      anaLog << (numHitTrees) << " trees read with " << ((int)numHitTree->GetEntries())
             << " Entries" << endli;
      if (startWithDelayedEventIn > 0) {
        c4 = searchDelayedEvent(hitTree, numHitTrees, StartWithEventIn, currentHitEntry, c4);
        numHitTree->GetEntry(c4 + 1);
        cout << "starting event number(hits): " << evNH << " Entry:" << c4 << endl;
        // 	      int nt=getNumTrees(numberOfHits,0);
        // 	      for(int i=nt;i<numHitTrees;i++)
        // 		currentHitEntry[i]=currentHitEntry[i]+1;
      }
    }
  }
  *eventNumber = 0;
}

AReadFromSimpleTree::~AReadFromSimpleTree()
{
  if (readHits) {
    cleanHits();
    for (int i = 0; i < event->getMaxNumber<TDetector>(); i++)
      delete[] oHits[i];
    delete[] oHits;
    delete[] onumberOfHits;
  }
  if (readPixels) {
    cleanPixels();
    for (int i = 0; i < event->getMaxNumber<TDetector>(); i++)
      delete[] oPixels[i];
    delete[] oPixels;
    delete[] onumberOfPixels;
    //      delete []Pix;
  }
  if (readClusters) {
    cleanCluster();
    for (int i = 0; i < event->getMaxNumber<TDetector>(); i++)
      delete[] oClusters[i];
    delete[] oClusters;
    delete[] onumberOfClusters;
    //      delete []Clus;
  }
  if (readTracks) {
    cleanTracks();
  }
}

void* AReadFromSimpleTree::process(void* ptr)
{
  //   TEvent *tev=event;
  //   int *tmpsav=eventNumber;
  // cout<<"process()"<<endl;
  if (inputmutex != NULL)
    pthread_mutex_lock((pthread_mutex_t*)inputmutex);
  bool hasHit = true, hasCluster = true, hasPixel = true, hasTrack = true;
  //  for(unsigned int i=0;i<hitTree.size();i++)
  //    cout<<currentHitEntry[i]<<"\t";
  //  cout<<endl;
  int tmpevt = getCommonEvent(c1, hasTrack, c2, hasCluster, c3, hasPixel, c4, hasHit);

  //  cout<<"eventNumber: "<<tmpevt<<endl;
  if (tmpevt == -1) {
    validInput = false;
    if (resetUnread)
      event->reset();
    return ptr;
  }
  if (inputmutex != NULL) {
    if (readHits && hasHit)
      emit hitRead(numberOfHits);
    if (readPixels && hasPixel)
      emit pixelRead(numberOfPixels);
    if (readClusters && hasCluster)
      emit clusterRead(numberOfCluster);
    if (readTracks && hasTrack)
      emit trackRead(ntr);
    pthread_mutex_unlock((pthread_mutex_t*)inputmutex);
  }
  (*eventNumber) = tmpevt;
  if (resetUnread) {
    event->getNumberOfTracks() = 0;
    event->reset();
  }
  //   cout<<"read hits "<<flush;
  if (readHits && hasHit)
    readHit(c4);
  //  cout<<"done \nread pixels "<<flush;
  if (readPixels && hasPixel)
    readPixel(c3);
  //  cout<<"done \nread clusters "<<flush;
  if (readClusters && hasCluster)
    readCluster(c2);
  // cout<<"done \nread tracks "<<flush;
  if (readTracks && hasTrack)
    readTrack(c1);
  // cout<<"done \nset event number "<<flush;
  event->setEventNumber(tmpevt);
  return ptr;
}
int AReadFromSimpleTree::getCommonEvent(int& iT, bool& ht, int& iC, bool& hc, int& iP, bool& hp,
                                        int& iH, bool& hh)
{
  //  cout<<"getCommonEvent()"<<endl;
  int large;
  bool condition;
  if (readTracks)
    large = evNT;
  if (readClusters)
    large = evNC;
  if (readPixels)
    large = evNP;
  if (readHits)
    large = evNH;
  if (rever) {
    evNT = 1000000000;
    evNH = 1000000000;
    evNC = 1000000000;
    evNP = 1000000000;
  } else {
    evNT = *eventNumber;
    evNH = *eventNumber;
    evNC = *eventNumber;
    evNP = *eventNumber;
  }
  condition = true;
  int numtrees;
  int lowest = *eventNumber;
  if (readTracks) {
    if (numTrackTree->GetEntries() <= iT + 1)
      return -1;
    numTrackTree->GetBranch("EventNumber")->GetEntry(++iT);
    lowest = evNT;
  }
  if (readClusters) {
    if (numCluTree->GetEntries() <= iC + 1)
      return -1;
    numCluTree->GetBranch("EventNumber")->GetEntry(++iC);
    if (lowest == *eventNumber || lowest > evNC)
      lowest = evNC;
  }
  if (readPixels) {
    if (numPixTree->GetEntries() <= iP + 1)
      return -1;
    numPixTree->GetBranch("EventNumber")->GetEntry(++iP);
    if (lowest == *eventNumber || lowest > evNP)
      lowest = evNP;
  }
  if (readHits) {
    if (numHitTree->GetEntries() <= iH + 1)
      return -1;
    iH++;
    numHitTree->GetEntry(iH, 0);
    if (lowest == *eventNumber || lowest > evNH)
      lowest = evNH;
  }
  ht = ((evNT == lowest) && readTracks);
  hc = ((evNC == lowest) && readClusters);
  hp = ((evNP == lowest) && readPixels);
  hh = ((evNH == lowest) && readHits);
  if (ht) {
    numTrackTree->GetBranch("NumberOfTracks")->GetEntry(iT, 0);
    numtrees = getNumTrees(ntr, 3);
    for (int i = 0; i < numtrees; i++)
      if (trackTree[i]->GetEntries() <= currentTrackEntry[i] + 1) {
        anaLog << string("trackTree[") << i << "] read out" << endli;
        return -1;
      } else
        currentTrackEntry[i]++;
  } else
    iT--;
  if (hc) {
    numCluTree->GetBranch("NumberOfCluster")->GetEntry(iC, 0);
    numtrees = getNumTrees(numberOfCluster, 2);
    for (int i = 0; i < numtrees; i++)
      if (cluTree[i]->GetEntries() <= currentCluEntry[i] + 1)
        return -1;
      else
        currentCluEntry[i]++;
  } else
    iC--;
  if (hp) {
    numPixTree->GetBranch("NumberOfPixels")->GetEntry(iP, 0);
    numtrees = getNumTrees(numberOfPixels, 1);
    for (int i = 0; i < numtrees; i++)
      if (pixTree[i]->GetEntries() <= currentPixEntry[i] + 1)
        return -1;
      else
        currentPixEntry[i]++;
  } else
    iP--;
  if (hh) {
    numHitTree->GetBranch("NumberOfHits")->GetEntry(iH, 0);
    numtrees = getNumTrees(numberOfHits, 0);
    //       cout<<hitTree.size()<<" "<<currentHitEntry.size()<<endl;
    //      cout<<numtrees<<"<";
    for (int i = 0; i < numtrees; i++) {
      // 	  cout<<hitTree[i]<<flush;
      // 	  cout<<" "<<hitTree[i]->GetEntries()<<" "<<flush;
      // 	  cout<<currentHitEntry[i]<<"\t";
      if (hitTree[i]->GetEntries() <= (currentHitEntry[i] + 1)) {
        anaLog << "hitTree[" << i << "] read out at eventNr: " << lowest << endli;
        return -1;
      } else
        currentHitEntry[i] = currentHitEntry[i] + 1;
    }
    //      cout<<endl;
  } else
    iH--;
  return lowest;

  if (readTracks) {
    if (numTrackTree->GetEntries() <= iT + 1)
      return -1;
    numTrackTree->GetEntry(++iT);
    numtrees = getNumTrees(ntr, 3);
    for (int i = 0; i < numtrees; i++)
      if (trackTree[i]->GetEntries() <= currentTrackEntry[i] + 1)
        return -1;
      else
        currentTrackEntry[i]++;
  }
  if (readClusters) {
    if (numCluTree->GetEntries() <= iC + 1)
      return -1;
    numCluTree->GetEntry(++iC);
    condition = condition && ((evNT == evNC && readTracks) || (evNT == -1 && !readTracks));
    numtrees = getNumTrees(numberOfCluster, 2);
    for (int i = 0; i < numtrees; i++)
      if (cluTree[i]->GetEntries() <= currentCluEntry[i] + 1)
        return -1;
      else
        currentCluEntry[i]++;
  }
  if (readPixels) {
    if (numPixTree->GetEntries() <= iP + 1)
      return -1;
    numPixTree->GetEntry(++iP);
    condition = condition && ((evNT == evNP && readTracks) || (evNT == -1 && !readTracks));
    condition = condition && ((evNC == evNP && readClusters) || (evNC == -1 && !readClusters));
    numtrees = getNumTrees(numberOfPixels, 1);
    for (int i = 0; i < numtrees; i++)
      if (pixTree[i]->GetEntries() <= currentPixEntry[i] + 1)
        return -1;
      else
        currentPixEntry[i]++;
  }
  if (readHits) {
    if (numHitTree->GetEntries() <= iH + 1)
      return -1;
    numHitTree->GetEntry(++iH);
    condition = condition && ((evNT == evNH && readTracks) || (evNT == -1 && !readTracks));
    condition = condition && ((evNC == evNH && readClusters) || (evNC == -1 && !readClusters));
    condition = condition && ((evNP == evNH && readPixels) || (evNP = -1 && !readPixels));
    numtrees = getNumTrees(numberOfHits, 0);
    for (int i = 0; i < numtrees; i++)
      if (hitTree[i]->GetEntries() <= currentHitEntry[i] + 1)
        return -1;
      else
        currentHitEntry[i] = currentHitEntry[i] + 1;
  }
  large = evNH;
  if (rever) {
    if (evNP < large)
      large = evNP;
    if (evNC < large)
      large = evNC;
    if (evNT < large)
      large = evNT;
  } else {
    if (evNP > large)
      large = evNP;
    if (evNC > large)
      large = evNC;
    if (evNT > large)
      large = evNT;
  }
  if ((condition))
    return large;
  while (!condition) {
    condition = true;
    if (readTracks && evNT < large) {
      if (numTrackTree->GetEntries() <= iT + 1)
        return -1;
      numTrackTree->GetEntry(++iT);
      numtrees = getNumTrees(ntr, 3);
      for (int i = 0; i < numtrees; i++)
        if (trackTree[i]->GetEntries() <= currentTrackEntry[i] + 1)
          return -1;
        else
          currentTrackEntry[i]++;
    }
    if (readClusters && evNC < large) {
      if (numCluTree->GetEntries() <= iC + 1)
        return -1;
      numCluTree->GetEntry(++iC);
      condition = condition && ((evNT == evNC && readTracks) || (evNT == -1 && !readTracks));
      numtrees = getNumTrees(numberOfCluster, 2);
      for (int i = 0; i < numtrees; i++)
        if (cluTree[i]->GetEntries() <= currentCluEntry[i] + 1)
          return -1;
        else
          currentCluEntry[i]++;
    }
    if (readPixels && evNP < large) {
      if (numPixTree->GetEntries() <= iP + 1)
        return -1;
      numPixTree->GetEntry(++iP);
      condition = condition && ((evNT == evNP && readTracks) || (evNT == -1 && !readTracks));
      condition = condition && ((evNC == evNP && readClusters) || (evNC == -1 && !readClusters));
      numtrees = getNumTrees(numberOfPixels, 1);
      for (int i = 0; i < numtrees; i++)
        if (pixTree[i]->GetEntries() <= currentPixEntry[i] + 1)
          return -1;
        else
          currentPixEntry[i]++;
    }
    if (readHits && evNH < large) {
      if (numHitTree->GetEntries() <= iH + 1)
        return -1;
      numHitTree->GetEntry(++iH);
      condition = condition && ((evNT == evNH && readTracks) || (evNT == -1 && !readTracks));
      condition = condition && ((evNC == evNH && readClusters) || (evNC == -1 && !readClusters));
      condition = condition && ((evNP == evNH && readPixels) || (evNP = -1 && !readPixels));
      numtrees = getNumTrees(numberOfHits, 0);
      for (int i = 0; i < numtrees; i++)
        if (hitTree[i]->GetEntries() <= currentHitEntry[i] + 1)
          return -1;
        else
          currentHitEntry[i] = currentHitEntry[i] + 1;
    }
    large = evNH;
    if (rever) {
      if (evNP < large)
        large = evNP;
      if (evNC < large)
        large = evNC;
      if (evNT < large)
        large = evNT;
    } else {
      if (evNP > large)
        large = evNP;
      if (evNC > large)
        large = evNC;
      if (evNT > large)
        large = evNT;
    }
    if (condition)
      return large;
  }
  return large;
}

void AReadFromSimpleTree::readTrack(int i)
{
  if (numTrackTree == NULL)
    return;
  numberOfTracks = 0;
  if (i >= numTrackTree->GetEntries()) {
    validInput = false;
    return;
  }
  numTrackTree->GetEntry(i);
  //   event->getTrigger()=tT;
  int numtrees = getNumTrees(ntr, 3);
  if (numtrees > numTrackTrees)
    numtrees = numTrackTrees;
  if (numtrees < 0)
    numtrees = 0;
  int tmpev[numtrees + 1];
  for (int j = 0; j < numtrees; j++) {
    if (currentTrackEntry[j] < 0)
      currentTrackEntry[j]++;
    if (currentTrackEntry[j] >= trackTree[j]->GetEntries()) {
      cout << "run too far" << endl;
      validInput = false;
      return;
    }
    trackTree[j]->GetBranch("EventNumber")->GetEntry(currentTrackEntry[j]);
    tmpev[j] = (Int_t)trackTree[j]->GetLeaf("EventNumber")->GetValue();
    //      trackTree[j]->GetEntry(currentTrackEntry[j],0);
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
  if (ntr > maxTrack)
    ntr = maxTrack;
  numberOfTracks = ntr;
  int w = -1;
  for (int j = 0; j < ntr; j++) {
    if (j == 5)
      w++;
    else if (j == 10)
      w++;
    else if ((j % 20) == 0)
      w++;
    if (trackBranches[j]->GetEntries() <= currentTrackEntry[w])
      return;
    trackBranches[j]->GetEntry(currentTrackEntry[w]);
    trackHitLeaves[j]->GetBranch()->GetEntry(currentTrackEntry[w]);
    trackFromLeaves[j]->GetBranch()->GetEntry(currentTrackEntry[w]);
  }
  int which = 0;
  int beg = 0;
  point3D c;
  vector3D v;
  float th, ph;
  int pos;
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
    th = trackLeaves[j][0]->GetValue();
    ph = trackLeaves[j][1]->GetValue();
    if (th > 0.433)
      ph += -0.011;
    if (ph < -M_PI)
      ph += 2 * M_PI;
    tracks[j]->reset();
    c.setValues(trackLeaves[j][3]->GetValue(0), trackLeaves[j][3]->GetValue(1),
                trackLeaves[j][3]->GetValue(2));
    //      if(c.R()>10000)c.setValues(0,0,0);
    v.setValues(sin(th) * cos(ph), sin(th) * sin(ph), cos(th));
    tracks[j]->setPath(sLine3D(c, v));
    // sLine3D(
    // point3D(Tracks[which][j-beg].vertex[0],Tracks[which][j-beg].vertex[1],Tracks[which][j-beg].vertex[2]),
    // vector3D(sin(Tracks[which][j-beg].theta)*cos(Tracks[which][j-beg].phi),
    // sin(Tracks[which][j-beg].theta)*sin(Tracks[which][j-beg].phi),
    // cos(Tracks[which][j-beg].theta))));
    tracks[j]->setSpeed(trackLeaves[j][2]->GetValue()); // Tracks[which][j-beg].beta);
    tracks[j]->getParticler().setVM(tracks[j]->getPath().Direction() * tracks[j]->getSpeed(),
                                    Eparticles::getMassG(Tracks[which][j - beg].ID));
    tracks[j]->setCharge(1);
    if (1 == 0) // readHits&&!reconstructHits)
    {
      int n = (int)trackLeaves[j][4]->GetValue();
      if (n == 0)
        tracks[j]->setCharge(0);
      if (n > 30)
        n = 30;
      for (int k = 0; k < n; k++) {
        pos = (int)trackHitLeaves[j]->GetValue(k);
        if (pos > numberOfHits || pos < 0)
          continue;
        int id = Hits[pos / 20][pos % 20].det;
        int el = Hits[pos / 20][pos % 20].el;
        if (id > setup->getNumberOfDetectors())
          continue;
        for (int n = 0; n < (*onumberOfHits[id]); n++) {
          if (oHits[id][n]->getElement() ==
              el) //||oHits[id][n]->getRawTDC()==Hits[hitsOfTrack[which][j][k]/20][hitsOfTrack[which][j-beg][k]%20].tdcR)
            tracks[j]->addCalibSrc(oHits[id][n]);
        }
      }
      if (tracks[j]->getNumberOfCalibHits() != n)
        anaLog << "AReadFromSimpleTree::readTracks() Track[" << j
               << "] wrong number of hits: assigned :" << tracks[j]->getNumberOfCalibHits()
               << " named: " << n << endli;
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
    pos = (int)trackFromLeaves[j]->GetValue(j - beg);
    if (pos >= 0 && pos < ntr) {
      tracks[j]->setOrder(true);
      for (int k = 0; k < ntr; k++) {
        if (k == pos) {
          tracks[k]->addSecondary(tracks[j]);
          tracks[j]->setPrimary(tracks[k]);
        }
      }
    } else
      tracks[j]->setOrder(false);
  }
  //  if(reconstructHits)//(!readHits)&&event->getTotalNumberOfHits()<=0)
  {
    which = 0;
    beg = 0;
    float qdc, tdc;
    int de = -1, el;
    int isin;
    volumeShape* sh;
    TCalibHit* th;
    //      cout<<"try"<<endl;
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
      for (int i = 0; i < 17; i++) {
        el = (int)trackLeaves[j][6]->GetValue(i);
        qdc = trackLeaves[j][7]->GetValue(i);
        tdc = trackLeaves[j][8]->GetValue(i);
        if (el >= 0) {
          de = (i < 8 ? i : i + 7);
          th = &(event->getHit(de, event->getNumberOfHits(de)));
          isin = -1;
          for (int k = 0; k < event->getNumberOfHits(de); k++)
            if (event->getHit(de, k).getElement() == el)
              isin = k;
          if (isin >= 0) {
            tracks[j]->addCalibSrc(&event->getHit(de, isin));
            //		      cout<<"found:"<<event->getHit(de,isin)<<endl;
            continue;
          }
          // cout<<"reconstruct"<<endl;
          if (maxOfHits > event->getNumberOfHits(de)) {
            th->setADC(qdc);
            th->setTDC(tdc);
            th->getRawADCr() = (int)(qdc);
            th->getRawTDCr() = (int)(tdc);
            th->getElementr() = el;
            th->getDetectorr() = de;
            if (el < static_cast<int>(setup->getDetectorr(de).getNumberOfElements())) {
              sh = setup->getDetectorr(de).getShape(el);
              th->setHitShape(sh);
              th->setValid(true);
              tracks[j]->addCalibSrc(th);
            }
            event->getNumberOfHits(de)++;
          }
        }
      }
    }
  }
  //  for(int j=0;j<ntr;j++){for(int
  //  i=0;i<25;i++)cout<<tracks[j]->getNumberOfCalibHits(i)<<"
  //  "<<flush;  cout<<endl;}
  //  cout<<endl;
}
void AReadFromSimpleTree::readPixel(int i)
{
  if (numPixTrees == 0)
    return;
  if (i >= numPixTree->GetEntries())
    return;
  numPixTree->GetEntry(i);
  event->getTrigger() = tP;
  int numtrees = getNumTrees(numberOfPixels, 1);
  for (int i = 0; i < numtrees; i++)
    pixTree[i]->GetEntry(currentPixEntry[i]++);
  for (int j = 0; j < event->getMaxNumber<TDetector>(); j++)
    (*onumberOfPixels[j]) = 0;
  TPixel* tp;
  planeShape* ps;
  for (int j = 0; j < numberOfPixels; j++) {
    if (event->getMaxNumber<TDetector>() < Pix[j / 20][j % 20].ID)
      if (event->getMaxNumber<TPixel>() < (*onumberOfPixels[Pix[j / 20][j % 20].ID])) {
        tp = oPixels[Pix[j / 20][j % 20].ID][(*onumberOfPixels[Pix[j / 20][j % 20].ID])];
        tp->setID(Pix[j / 20][j % 20].ID);
        tp->reset();
        ps = tp->getShape();
        if (ps)
          delete ps;
        ps = new planeShape("temp");
        ps->setCenter(point3D(Pix[j / 20][j % 20].center[0], Pix[j / 20][j % 20].center[1],
                              Pix[j / 20][j % 20].center[2]));
        tp->setShape(ps);
        if (readHits) {
          for (int kk = 0; kk < Pix[j / 20][j % 20].numberOfElements; kk++) {
            int k = Pix[j / 20][j % 20].elementNumber[kk];
            int id = Hits[k / 20][k % 20].det;
            int el = Hits[k / 20][k % 20].el;
            for (int n = 0; n < (*onumberOfHits[id]); n++)
              if (el == oHits[id][n]->getElement() &&
                  Hits[k / 20][k % 20].tdcR == oHits[id][n]->getRawTDC())
                tp->addElement(*oHits[id][n]);
          }
        }
        (*onumberOfPixels[Pix[j / 20][j % 20].ID])++;
      }
  }
}

void AReadFromSimpleTree::readCluster(int i)
{
  if (numCluTrees == 0)
    return;
  if (i >= numCluTree->GetEntries())
    return;
  numCluTree->GetEntry(i);
  event->getTrigger() = tC;
  int numtrees = getNumTrees(numberOfCluster, 2);
  for (int i = 0; i < numtrees; i++)
    cluTree[i]->GetEntry(currentCluEntry[i]++);
  for (int j = 0; j < event->getMaxNumber<TDetector>(); j++)
    (*onumberOfClusters[j]) = 0;
  TCluster* tc;
  for (int j = 0; j < numberOfCluster; j++) {
    if (event->getMaxNumber<TDetector>() < Clus[j / 20][j % 20].ID)
      if (event->getMaxNumber<TCluster>() < (*onumberOfClusters[Clus[j / 20][j % 20].ID])) {
        tc = oClusters[Clus[j / 20][j % 20].ID][(*onumberOfClusters[Clus[j / 20][j % 20].ID])];
        //      tc->setID(Clus[j/20][j%20].ID);
        tc->reset();
        tc->setCenter(point3D(Clus[j / 20][j % 20].center[0], Clus[j / 20][j % 20].center[1],
                              Clus[j / 20][j % 20].center[2]));
        if (readPixels) {
          for (int kk = 0; kk < Clus[j / 20][j % 20].numberOfPixels; kk++) {
            int k = Clus[j / 20][j % 20].pixelNumber[kk];
            int id = Pix[k / 20][k % 20].ID;
            for (int n = 0; n < (*onumberOfPixels[id]); n++)
              if (Pix[k / 20][k % 20].center[0] == oPixels[id][n]->getShape()->getCenter().X() &&
                  Pix[k / 20][k % 20].center[1] == oPixels[id][n]->getShape()->getCenter().Y() &&
                  Pix[k / 20][k % 20].center[2] == oPixels[id][n]->getShape()->getCenter().Z())
                tc->addPixel(*oPixels[id][n]);
          }
        }
        (*onumberOfClusters[Clus[j / 20][j % 20].ID])++;
      }
  }
}
void AReadFromSimpleTree::readHit(int i)
{
  for (int j = 0; j < event->getMaxNumber<TDetector>(); j++)
    (*(onumberOfHits[j])) = 0;
  numHitTree->GetEntry(i);
  event->getTrigger() = tH;
  int numtrees = getNumTrees(numberOfHits, 0);
  if (numHitTrees == 0)
    return;
  //   int tmpev[numtrees];
  if (i >= hitTree[0]->GetEntries())
    return;
  for (int j = 0; j < numtrees; j++) {
    if (currentHitEntry[j] < 0)
      currentHitEntry[j]++;
    hitTree[j]->GetEntry(currentHitEntry[j]);
    //      tmpev[j]=hitTree[j]->GetLeaf("EventNumber")->GetValue();
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
  TCalibHit* th;
  volumeShape* sh;
  int de, el;
  for (int j = 0; j < numberOfHits; j++) {
    de = Hits[j / 20][j % 20].det;
    el = Hits[j / 20][j % 20].el;
    if (de < setup->getNumberOfDetectors()) {
      if (event->getMaxNumber<TCalibHit>() > (*onumberOfHits[de])) {
        th = oHits[de][(*onumberOfHits[de])];
        th->setADC(Hits[j / 20][j % 20].qdcC);
        th->setTDC(Hits[j / 20][j % 20].tdcC);
        th->getRaw().setADC(Hits[j / 20][j % 20].qdcR);
        th->getRaw().setTDC(Hits[j / 20][j % 20].tdcR);
        th->getRaw().setElement(el);
        th->getRaw().setDetector(de);
        if (Hits[j / 20][j % 20].el <
            static_cast<int>(setup->getDetectorr(de).getNumberOfElements())) {
          sh = setup->getDetectorr(de).getShape(el);
          th->setHitShape(sh);
          th->setValid(true);
        } else
          th->setValid(false);
        (*onumberOfHits[de])++;
      }
    }
  }
}
bool AReadFromSimpleTree::setTrackTree(TFile* f)
{
  if (readTracks) {
    int numTmp = 0;
    TTree* tmptree = (TTree*)f->Get("Track1");
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)f->Get((string("Track") + string_number(numTmp + 1)).data());
    }
    // Tracks=new track*[numTmp];////new trackstruct
    Tracks = new track1*[numTmp];
    fromID = new Int_t*[numTmp];
    hitsOfTrack = new Int_t**[numTmp];
    trackTree = new TTree*[numTmp];
    int accnum = 5;
    int beg = 0;
    maxTracks = (numTmp - 2) * 20;
    if (maxTracks <= 0 && numTmp > 0) {
      if (numTmp == 1)
        maxTracks = 5;
      if (numTmp == 2)
        maxTracks = 10;
    }
    trackBranches = new TBranch*[maxTracks];
    trackLeaves = new TLeaf**[maxTracks];
    trackHitLeaves = new TLeaf*[maxTracks];
    trackFromLeaves = new TLeaf*[maxTracks];
    for (int i = 0; i < maxTracks; i++)
      trackLeaves[i] = new TLeaf*[9];
    //      tmptree=(TTree*)f->Get("Track1");
    for (int j = 0; j < numTmp; j++) // while(tmptree!=NULL)
    {
      tmptree = (TTree*)f->Get((string("Track") + string_number(j + 1)).data());
      nextTrackEntry.push_back(-1);
      // currentTrackEntry.push_back(-1);
      track1* tmptracks = new track1[accnum];
      // track *tmptracks=new track[accnum];////new trackstruct
      Int_t* tmpf = new Int_t[accnum];
      Int_t** tmph = new Int_t*[accnum];
      tmptree->SetBranchStatus("fromID", 1);
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("fromID", tmpf);
      tmptree->SetBranchAddress("EventNumber", &nextTrackEntry.back());
      for (int i = 0; i < accnum; i++) {
        tmph[i] = new Int_t[30];
        tmptree->SetBranchStatus((string("hits") + string_number(beg + i + 1)).data(), 1);
        tmptree->SetBranchStatus((string("Track") + string_number(beg + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("hits") + string_number(beg + i + 1)).data(), tmph[i]);
        tmptree->SetBranchAddress((string("Track") + string_number(beg + i + 1)).data(),
                                  &tmptracks[i]);
        trackBranches[beg + i] =
            tmptree->GetBranch((string("Track") + string_number(beg + i + 1)).data());
        trackHitLeaves[beg + i] =
            tmptree->FindLeaf((string("hits") + string_number(beg + i + 1)).data());
        trackFromLeaves[beg + i] = tmptree->FindLeaf("fromID");
        trackLeaves[beg + i][0] = trackBranches[beg + i]->FindLeaf("theta");
        trackLeaves[beg + i][1] = trackBranches[beg + i]->FindLeaf("phi");
        trackLeaves[beg + i][2] = trackBranches[beg + i]->FindLeaf("beta");
        trackLeaves[beg + i][3] = trackBranches[beg + i]->FindLeaf("vertex");
        trackLeaves[beg + i][4] = trackBranches[beg + i]->FindLeaf("nEl");
        trackLeaves[beg + i][5] = trackBranches[beg + i]->FindLeaf("ID");
        trackLeaves[beg + i][6] = trackBranches[beg + i]->FindLeaf("ss");
        trackLeaves[beg + i][7] = trackBranches[beg + i]->FindLeaf("qdc");
        trackLeaves[beg + i][8] = trackBranches[beg + i]->FindLeaf("tdc");
      }
      Tracks[j] = (tmptracks);
      trackTree[j] = (tmptree);
      fromID[j] = (tmpf);
      hitsOfTrack[j] = (tmph);
      beg += accnum;
      if (j == 1)
        accnum = 10;
      else if (j == 2)
        accnum = 20;
      tmptree->GetEntry(1);
    }
    if (currentTrackEntry)
      delete[] currentTrackEntry;
    currentTrackEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentTrackEntry[i] = -1;
    anaLog << (numTmp) << " track trees found" << endli;
    numTrackTrees = numTmp;
    c1 = -1;
    //       cout<<"branchtest:"<<endl;
    //       for(int i=0;i<maxTracks;i++)
    // 	{
    // 	  cout<<trackBranches[i]->GetName()<<flush;
    // 	  for(int j=0;j<trackBranches[i]->GetEntries();j++)
    // 	    trackBranches[i]->GetEntry(j);
    // 	  cout<<" Ok"<<endl;
    // 	}
    return true;
  }
  return false;
}
bool AReadFromSimpleTree::setPixelTree(TFile* f)
{
  if (readPixels) {
    TTree* tmptree = (TTree*)f->Get("Pixels1");
    int beg = 0;
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)f->Get((string("Pixels") + string_number(numTmp + 1)).data());
    }
    pixTree = new TTree*[numTmp];
    for (int j = 0; j < numTmp; j++) {
      nextPixEntry.push_back(-1);
      // currentPixEntry.push_back(-1);
      pixel* tmptracks = new pixel[20];
      tmptree->SetBranchAddress("EventNumber", &nextPixEntry.back());
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchAddress((string("Pixel") + string_number(beg + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Pix.push_back(tmptracks);
      pixTree[j] = (tmptree);
      beg += 20;
      tmptree->GetEntry(1);
      tmptree = (TTree*)f->Get((string("Pixels") + string_number(j + 2)).data());
    }
    if (currentPixEntry)
      delete[] currentPixEntry;
    currentPixEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentPixEntry[i] = -1;
    anaLog << (numTmp) << " pixel trees found" << endli;
    numPixTrees = numTmp;
    setPixelStruct();
    c2 = -1;
    return true;
  }
  return false;
}
bool AReadFromSimpleTree::setClusterTree(TFile* f)
{
  if (readClusters) {
    TTree* tmptree = (TTree*)f->Get("Cluster1");
    int beg = 0;
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)f->Get((string("Cluster") + string_number(numTmp + 1)).data());
    }
    cluTree = new TTree*[numTmp];
    for (int j = 0; j < numTmp; j++) {
      nextCluEntry.push_back(-1);
      // currentCluEntry.push_back(-1);
      cluster* tmptracks = new cluster[20];
      tmptree->SetBranchAddress("EventNumber", &nextCluEntry.back());
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchAddress((string("Cluster") + string_number(beg + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Clus.push_back(tmptracks);
      cluTree[j] = (tmptree);
      beg += 20;
      tmptree->GetEntry(1);
      tmptree = (TTree*)f->Get((string("Cluster") + string_number(j + 2)).data());
    }
    if (currentCluEntry)
      delete[] currentCluEntry;
    currentCluEntry = new int[numCluTrees];
    for (int i = 0; i < numCluTrees; i++)
      currentCluEntry[i] = -1;
    anaLog << (numCluTrees) << " cluster trees found" << endli;
    c3 = -1;
    return true;
  }
  return false;
}
bool AReadFromSimpleTree::setHitTree(TFile* f)
{
  if (readHits) {
    TTree* tmptree = (TTree*)f->Get("Hits1");
    int beg = 0;
    int numTmp = 0;
    while (tmptree != NULL) {
      numTmp++;
      tmptree = (TTree*)f->Get((string("Hits") + string_number(numTmp + 1)).data());
    }
    tmptree = (TTree*)f->Get("Hits1");
    hitTree = new TTree*[numTmp];
    Hits = new hit*[numTmp];
    for (int j = 0; j < numTmp; j++) {
      nextHitEntry.push_back(-1);
      // currentHitEntry.push_back(-1);
      hit* tmptracks = new hit[20];
      tmptree->SetBranchStatus("EventNumber", 1);
      tmptree->SetBranchAddress("EventNumber", &nextHitEntry.back());
      for (int i = 0; i < 20; i++) {
        tmptree->SetBranchStatus((string("Hit") + string_number(beg + i + 1)).data(), 1);
        tmptree->SetBranchAddress((string("Hit") + string_number(beg + i + 1)).data(),
                                  &tmptracks[i]);
      }
      Hits[j] = (tmptracks);
      hitTree[j] = (tmptree);
      beg += 20;
      tmptree->GetEntry(0);
      tmptree = (TTree*)f->Get((string("Hits") + string_number(j + 2)).data());
    }
    if (currentHitEntry)
      delete[] currentHitEntry;
    currentHitEntry = new int[numTmp];
    for (int i = 0; i < numTmp; i++)
      currentHitEntry[i] = -1;
    anaLog << (numTmp) << " hit trees found" << endli;
    anaLog << "in file \"" << f->GetName() << "\" ";
    if (numTmp > 0)
      anaLog << (int)(hitTree[0]->GetEntries()) << " Entries";
    anaLog << endli;
    numHitTrees = numTmp;
    c4 = -1;
    return true;
  }
  return false;
}
int AReadFromSimpleTree::getNumTrees(int numEntr, int which)
{
  if (numEntr == 0)
    return 0;
  switch (which) {
  case 0:
    if ((numEntr / 20) + 1 > numHitTrees)
      return numHitTrees;
    return (numEntr / 20) + 1;
  case 1:
    if ((numEntr / 20) + 1 > numPixTrees)
      return numPixTrees;
    return (numEntr / 20) + 1;
  case 2: {
    if ((numEntr / 20) + 1 > numCluTrees)
      return numCluTrees;
    return (numEntr / 20) + 1;
  }
  case 3: {
    int num;
    if (numEntr <= 5)
      num = 1;
    else if (numEntr <= 10)
      num = 2;
    else
      num = (numEntr / 20) + 3;
    if (num > numTrackTrees)
      num = numTrackTrees;
    return num;
  }
  }
  return 0;
}

int AReadFromSimpleTree::searchDelayedEvent(TTree** theTree, int numTrees, int toEvent,
                                            int* theCounter, int acounter)
{
  int nev[numTrees];
  TLeaf** evtnr = new TLeaf*[numTrees];
  for (int i = 0; i < numTrees; i++)
    evtnr[i] = theTree[i]->FindLeaf("EventNumber");
  if (toEvent > 0) {
    for (int i = 0; i < numTrees; i++) {
      int nl, nh, nm;
      int pl = 0,
          ph = (toEvent >= theTree[i]->GetEntries() ? theTree[i]->GetEntries() - 1 : toEvent),
          pm = (pl + ph) / 2;
      evtnr[i]->GetBranch()->GetEntry(0);
      nl = (Int_t)evtnr[i]->GetValue();
      evtnr[i]->GetBranch()->GetEntry(ph);
      nh = (Int_t)evtnr[i]->GetValue();
      while (nl < toEvent && nh > toEvent && abs(pl - ph) > 1) {
        evtnr[i]->GetBranch()->GetEntry(pm);
        nm = (Int_t)evtnr[i]->GetValue(); // nev[i];
        if (nm == toEvent) {
          nl = nm;
          pl = pm;
        } else {
          if (nm < toEvent) {
            nl = nm;
            pl = pm;
          } else {
            nh = nm;
            ph = pm;
          }
          pm = (pl + ph) / 2;
        }
      }
      if (nl >= toEvent) {
        nev[i] = nl;
        theCounter[i] = pl;
      } else if (nh <= toEvent) {
        nev[i] = nh;
        theCounter[i] = ph;
      } else {
        nev[i] = nl;
        theCounter[i] = pl + 1;
      }
      theCounter[i]--;
    }
  }
  for (int i = 0; i < numTrees; i++) {
    if (nev[i] > nev[0])
      theCounter[i]--;
  }
  acounter = theCounter[0];
  delete[] evtnr;
  return acounter;
}
void AReadFromSimpleTree::getNewInput(string* files, int* type, int nFiles, bool grap)
{
  //  if(files.size()!=type.size())return;
  if (grap) {
    for (int i = 0; i < nFiles; i++) {
      switch (type[i]) {
      case 6: // hits
      {
        if (!readHits)
          continue;
        if (files[i] == hitf->GetName())
          continue;
        cleanHits();
        if (!existing(files[i]))
          readHits = false;
        else {
          if (localDirectory) {
            filenames[0] = localDir + "/" +
                           files[i].substr(files[i].rfind("/") + 1,
                                           files[i].length() - files[i].rfind("/") - 1);
#if QT_VERSION < 0x040000
            QProcess copy(QString("cp "));
            copy.addArgument((files[i].data()));
            copy.addArgument(filenames[0].data());
            copy.start();
            while (copy.isRunning()) {
            }
#else
            QProcess copy;
            copy.start(QString((string("cp ") + files[i] + " " + filenames[0]).data()));
            while (copy.waitForFinished()) {
            }
#endif
          } else
            filenames[0] = files[i];
          cout << "\"" << filenames[0].data() << "\"" << endl;
          hitf = new TFile(filenames[0].data(), "READ");
          numHitTree = (TTree*)hitf->Get("numberOfHits");
          numHitTree->SetBranchAddress("EventNumber", &evNH);
          numHitTree->SetBranchAddress("NumberOfHits", &numberOfHits);
          numHitTree->SetBranchAddress("Trigger", &tH);
          readHits = setHitTree(hitf);
        }
        break;
      }
      case 10: // pixels
      {
        if (!readPixels)
          continue;
        while (!Pix.empty()) {
          pixel* tmp = Pix.back();
          Pix.pop_back();
          delete[] tmp;
        }
        if (numPixTree)
          pixelf = numPixTree->GetCurrentFile();
        cleanPixels();
        if (!existing(files[i]))
          readPixels = false;
        else {
          if (localDirectory) {
            filenames[1] = localDir + "/" +
                           files[i].substr(files[i].rfind("/") + 1,
                                           files[i].length() - files[i].rfind("/") - 1);
#if QT_VERSION < 0x040000
            QProcess copy(QString("cp "));
            copy.addArgument((files[i].data()));
            copy.addArgument(filenames[1].data());
            copy.start();
            while (copy.isRunning()) {
            }
#else
            QProcess copy;
            copy.start(QString((string("cp ") + files[i] + " " + filenames[1]).data()));
            while (copy.waitForFinished()) {
            }
#endif
          } else
            filenames[1] = files[i];
          pixelf = new TFile(filenames[1].data(), "READ");
          numPixTree = (TTree*)pixelf->Get("numberOfPixels");
          numPixTree->SetBranchAddress("EventNumber", &evNP);
          numPixTree->SetBranchAddress("NumberOfPixels", &numberOfPixels);
          readPixels = setPixelTree(pixelf);
        }
        break;
      }
      case 18: // cluster
      {
        if (!readClusters)
          continue;
        while (!Clus.empty()) {
          cluster* tmp = Clus.back();
          Clus.pop_back();
          delete[] tmp;
        }
        if (numCluTree)
          clusterf = numCluTree->GetCurrentFile();
        cleanCluster();
        if (!existing(files[i]))
          readClusters = false;
        else {
          if (localDirectory) {
            filenames[2] = localDir + "/" +
                           files[i].substr(files[i].rfind("/") + 1,
                                           files[i].length() - files[i].rfind("/") - 1);
#if QT_VERSION < 0x040000

            QProcess copy(QString("cp "));
            copy.addArgument((files[i]));
            copy.addArgument(filenames[2].data());
            copy.start();
            while (copy.isRunning()) {
            }
#else
            QProcess copy;
            copy.start(QString((string("cp ") + files[i] + " " + filenames[2]).data()));
            while (copy.waitForFinished()) {
            }
#endif
          } else
            filenames[2] = files[i];
          clusterf = new TFile(filenames[2].data(), "READ");
          numCluTree = (TTree*)clusterf->Get("numberOfCluster");
          numCluTree->SetBranchAddress("EventNumber", &evNC);
          numCluTree->SetBranchAddress("NumberOfCluster", &numberOfCluster);
          readClusters = setClusterTree(clusterf);
        }
        break;
      }
      case 34: // tracks
      {
        if (!readTracks)
          continue;
        if (numTrackTree)
          trackf = numTrackTree->GetCurrentFile();
        cleanTracks();
        if (trackf)
          trackf->Close();
        if (!existing(files[i]))
          readTracks = false;
        else {
          if (localDirectory) {
            filenames[3] = localDir + "/" +
                           files[i].substr(files[i].rfind("/") + 1,
                                           files[i].length() - files[i].rfind("/") - 1);
#if QT_VERSION < 0x040000
            QProcess copy(QString("cp "));
            copy.addArgument((files[i].data()));
            copy.addArgument(filenames[3].data());
            copy.start();
            while (copy.isRunning()) {
            }
#else
            QProcess copy;
            copy.start(QString((string("cp ") + files[i] + " " + filenames[3]).data()));
            while (copy.waitForFinished()) {
            }
#endif
          } else
            filenames[3] = files[i];
          trackf = new TFile(filenames[3].data(), "READ");
          numTrackTree = (TTree*)trackf->Get("numberOfTracks");
          numTrackTree->SetBranchAddress("EventNumber", &evNT);
          numTrackTree->SetBranchAddress("NumberOfTracks", &ntr);
          readTracks = setTrackTree(trackf);
        }
        if (numTrackTrees == 0)
          cout << "no tree \"Track\" found in " << files[i].data() << endl;
        break;
      }
      }
    }
    grap = false;
    *eventNumber = 0;
  }
}
void AReadFromSimpleTree::OnHitRead(int numberOfHits)
{
  int numTrees = getNumTrees(numberOfHits, 0);
  //  cout<<"OnHitRead()";
  for (int i = 0; i < numTrees; i++)
    currentHitEntry[i] = currentHitEntry[i] + 1;
  c4++;
}
void AReadFromSimpleTree::OnPixelRead(int numberOfPixels)
{
  int numTrees = getNumTrees(numberOfPixels, 1);
  for (int i = 0; i < numTrees; i++)
    currentPixEntry[i]++;
  c3++;
}
void AReadFromSimpleTree::OnClusterRead(int numberOfClusters)
{
  int numTrees = getNumTrees(numberOfClusters, 2);
  for (int i = 0; i < numTrees; i++)
    currentCluEntry[i]++;
  c2++;
}
void AReadFromSimpleTree::OnTrackRead(int numberOfTracks)
{
  int numTrees = getNumTrees(numberOfTracks, 3);
  for (int i = 0; i < numTrees; i++)
    currentTrackEntry[i]++;
  c1++;
}
void AReadFromSimpleTree::getNewRun(run_parameter& rp)
{
  anaLog << "AReadFromSimpleTree::getNewRun(): next run for read-in: " << rp.getName().data()
         << endli;
  int nFiles = rp.getNumberOfFiles();
  string files[nFiles];
  int type[nFiles];
  //   vector<string> files;
  //   vector<int> type;
  for (size_t i = 0; i < rp.getNumberOfFiles(); i++) {
    files[i] = rp.getFile(i);    //.push_back(rp.getFile(i));
    type[i] = rp.getFileType(i); //.push_back(rp.getFileType(i));
  }
  getNewInput(files, type, nFiles, true);
  validInput = true;
}
void AReadFromSimpleTree::setHitStruct()
{
  //  cout<<event<<endl;
  if (event == NULL)
    return;
  int numdet = event->getMaxNumber<TDetector>();
  oHits = new TCalibHit**[numdet];
  onumberOfHits = new int*[numdet];
  for (int i = 0; i < numdet; i++) {
    onumberOfHits[i] = &event->getNumberOfHits(i);
    oHits[i] = new TCalibHit*[event->getMaxNumber<TCalibHit>()];
    for (int j = 0; j < event->getMaxNumber<TCalibHit>(); j++)
      oHits[i][j] = &event->getHit(i, j);
  }
}
void AReadFromSimpleTree::setPixelStruct()
{
  oPixels = new TPixel**[event->getMaxNumber<TDetector>()];
  onumberOfPixels = new int*[event->getMaxNumber<TDetector>()];
  for (int i = 0; i < event->getMaxNumber<TDetector>(); i++) {
    onumberOfPixels[i] = &event->getNumberOfPixels(i);
    oPixels[i] = new TPixel*[event->getMaxNumber<TPixel>()];
    for (int j = 0; j < event->getMaxNumber<TPixel>(); j++)
      oPixels[i][j] = &event->getPixel(i, j);
  }
}
void AReadFromSimpleTree::setClusterStruct()
{
  oClusters = new TCluster**[event->getMaxNumber<TDetector>()];
  onumberOfClusters = new int*[event->getMaxNumber<TDetector>()];
  for (int i = 0; i < event->getMaxNumber<TDetector>(); i++) {
    onumberOfClusters[i] = &event->getNumberOfClusters(i);
    oClusters[i] = new TCluster*[event->getMaxNumber<TCluster>()];
    for (int j = 0; j < event->getMaxNumber<TCluster>(); j++)
      oClusters[i][j] = &event->getCluster(i, j);
  }
}
void AReadFromSimpleTree::cleanTracks()
{
  if (numTrackTrees < 1)
    return;
  if (localDirectory) {
    string fn = numTrackTree->GetCurrentFile()->GetName();
    QDir dir(fn.substr(0, fn.rfind("/")).data());
    fn = fn.substr(fn.rfind("/") + 1, fn.length() - fn.rfind("/") - 1);
    dir.remove(fn.data());
    cout << "temporary file \"" << fn.data() << "\" removed from directory \"" << dir.path().data()
         << "\"." << endl;
  }
  for (int i = 0; i < numTrackTrees; i++) // while(!Tracks.empty())
  {
    //	  track *tmp1=Tracks.back();
    //	  Tracks.pop_back();
    //	  delete []tmp1;
    //	  Int_t *tmp2=fromID.back();
    // fromID.pop_back();
    delete[] fromID[i];
    // delete []tmp2;
    Int_t** tmp3 = hitsOfTrack[i];
    //	  hitsOfTrack.pop_back();
    int accnum = 0;
    if (i == 0 || i == 1)
      accnum = 5;
    else if (i == 2)
      accnum = 10;
    else
      accnum = 20;
    for (int j = 0; j < accnum; j++)
      delete[] tmp3[j];
    delete[] tmp3;
  }
  delete[] fromID;
  delete[] Tracks;
  delete[] hitsOfTrack;
  delete[] trackTree;
  delete[] trackBranches;
  for (int i = 0; i < maxTracks; i++)
    delete[] trackLeaves[i];
  delete[] trackLeaves;
}
void AReadFromSimpleTree::cleanHits()
{
  for (int i = 0; i < numHitTrees; i++)
    delete[] Hits[i];
  delete[] Hits;
  delete[] hitTree;
  if (numHitTree)
    hitf = numHitTree->GetCurrentFile();
  if (localDirectory) {
    string fn = hitf->GetName();
    QDir dir(fn.substr(0, fn.rfind("/")).data());
    fn = fn.substr(fn.rfind("/") + 1, fn.length() - fn.rfind("/") - 1);
    dir.remove(fn.data());
    cout << "temporary file \"" << fn.data() << "\" removed from directory \"" << dir.path().data()
         << "\"." << endl;
  }
  if (numHitTree != NULL)
    if (numHitTree->GetCurrentFile() != NULL)
      if (numHitTree->GetCurrentFile()->IsOpen())
        numHitTree->GetCurrentFile()->Close();
  delete hitf;
  hitf = NULL;
  numHitTree = NULL;
}
void AReadFromSimpleTree::cleanPixels()
{
  delete[] pixTree;
  pixelf->Close();
  if (localDirectory) {
    string fn = pixelf->GetName();
    QDir dir(fn.substr(0, fn.rfind("/")).data());
    fn = fn.substr(fn.rfind("/") + 1, fn.length() - fn.rfind("/") - 1);
    dir.remove(fn.data());
    cout << "temporary file \"" << fn.data() << "\" removed from directory \"" << dir.path().data()
         << "\"." << endl;
  }
  delete pixelf;
  pixelf = NULL;
  while (!Pix.empty()) {
    pixel* tmp = Pix.back();
    Pix.pop_back();
    delete[] tmp;
  }
}
void AReadFromSimpleTree::cleanCluster()
{
  delete[] cluTree;
  clusterf->Close();
  if (localDirectory) {
    string fn = clusterf->GetName();
    QDir dir(fn.substr(0, fn.rfind("/")).data());
    fn = fn.substr(fn.rfind("/") + 1, fn.length() - fn.rfind("/") - 1);
    dir.remove(fn.data());
    cout << "temporary file \"" << fn.data() << "\" removed from directory \"" << dir.path().data()
         << "\"." << endl;
  }
  delete clusterf;
  clusterf = NULL;
  while (!Clus.empty()) {
    cluster* tmp = Clus.back();
    Clus.pop_back();
    delete[] tmp;
  }
}
