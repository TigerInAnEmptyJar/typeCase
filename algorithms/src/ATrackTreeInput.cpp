#include "ATrackTreeInput.h"
#include "algorithmparameter.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
extern logger readWriteLog;
#include "ring.h"
#include "spiral.h"
#include "wedge.h"
#include <QtCore/QProcess>
#include <stdlib.h>
void ATrackTreeInput::setTrackErrors(TTrack* tr)
{
  if (tr->isSecondary()) {
    tr->setErrorTheta(2. * M_PI / 180.);
    tr->setErrorPhi(4. * M_PI / 180.);
    tr->setErrorSpeed(0.1);
    return;
  }
  float minPhi = 3, minth = 3;
  float tmp, f1, f2, f3, p, b, d;
  for (int i = 0; i < numDets; i++)
    for (int j = 0; j < tr->getNumberOfCalibHits(i); j++) {
      if (tr->getCalibSrcr(i, j) == NULL)
        continue;
      if (tr->getCalibSrcr(i, j)->getHitShape() == NULL)
        continue;
      if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "wedge") {
        tmp = abs(((wedge*)tr->getCalibSrcr(i, j)->getHitShape())->getPhiRange() / 2.);
        if (minPhi > tmp &&
            ((wedge*)tr->getCalibSrcr(i, j)->getHitShape())->getDistToBarrelCenter() < 0)
          minPhi = tmp;
      } else if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "spiral") {
        tmp = ((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getPhiRange() / 2.;
        if (minPhi > tmp)
          minPhi = tmp;
        d = tmp;
        b = abs(((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getBending());
        p = tr->getPath().Direction().Phi();
        f1 = ((spiral*)tr->getCalibSrcr(i, j)->getHitShape())->getCenter().Z();
        f2 = b * p;
        f3 = b * (p + d);
        tmp = atan(((b * d) / f1) / (1 + f2 * f3 / f1 / f1)) / 2.;
        if (minth > tmp)
          minth = tmp;
      } else if (tr->getCalibSrcr(i, j)->getHitShape()->getName() == "ring") {
        f1 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getCenter().Z();
        f2 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getInnerRadius();
        f3 = ((ring*)tr->getCalibSrcr(i, j)->getHitShape())->getOuterRadius();
        tmp = atan((f3 - f2) / f1 / (1 + f2 * f3 / f1 / f1)) / 2.;
        if (minth > tmp)
          minth = tmp;
      }
    }
  tr->setErrorTheta(minth);
  tr->setErrorPhi(minPhi);
  tr->setErrorSpeed(0.1);
}

ATrackTreeInput::ATrackTreeInput(int& evtNrIn, int& runNrIn, int& triIn, TTrack** trackIn,
                                 int& numTrackIn, int maxTrackIn, int numDetsIn, int** numHitsIn,
                                 TCalibHit*** hitIn, int** numPixIn, TPixel*** PixIn,
                                 bool& readInValid, const algorithm_parameter& param)
    : AAlgorithm("Track tree input"), numberOfTracks(numTrackIn), maxtracks(maxTrackIn),
      numDets(numDetsIn), valid(readInValid), eventNumber(evtNrIn), runNumber(runNrIn),
      trigger(triIn)
{
  valid = false;
  trackTree = NULL;
  currentEntry = 0;
  tracks = trackIn;
  hits = hitIn;
  numberOfCalibHits = numHitsIn;
  numberOfPixels = numPixIn;
  pixels = PixIn;
  searchForEvent = false;
  if (param.getNumberOfParam<bool>() > 0)
    searchForEvent = param.getParam<bool>(0).getData();
  doPattern = false;
  numPattern = 0;
  pattern = NULL;
  if (param.getNumberOfParam<bool>() > 2 && param.getNumberOfParam<vector<int>>() > 0)
    doPattern = param.getParam<bool>(2).getData();
  if (doPattern) {
    numPattern = param.getParam<vector<int>>(0).getData().size();
    numPattern = numPattern / 3;
    pattern = new int[numPattern * 3];
    for (int i = 0; i < numPattern * 3; i++)
      pattern[i] = param.getParam<vector<int>>(0).getData().at(i);
  }
  localDirectory = false;
  if (param.getNumberOfParam<bool>() > 3 && param.getNumberOfParam<string>() > 0) {
    localDirectory = param.getParam<bool>(3).getData();
    directory = param.getParam<string>(0).getData();
  }
  copyFile = "";
}
ATrackTreeInput::~ATrackTreeInput()
{
  disconnect(this);
  clearTree();
  if (doPattern)
    delete[] pattern;
}
void* ATrackTreeInput::process(void* ptr)
{
  if (trackTree == NULL)
    return ptr;
  numberOfTracks = 0;
  if (trackTree->GetEntries() <= currentEntry)
    return ptr;
  if (currentEntry < 0)
    return ptr;
  if (searchForEvent)
    readEvent(eventNumber);
  else
    readEntry(currentEntry);
  return ptr;
}
void ATrackTreeInput::prepareNextEntry()
{
  if (doPattern) {
    bool useEvent = false;
    while (trackTree->GetEntries() > currentEntry && !useEvent) {
      useEvent = false;
      for (int i = 0; i < 3; i++)
        numbers[i]->GetBranch()->GetEntry(currentEntry);
      bool patterntrue;
      for (int i = 0; i < numPattern; i++) {
        patterntrue = true;
        for (int j = 0; j < 3; j++) {
          if (pattern[i * 3 + j] > 0)
            if (numbers[j]->GetValue() < pattern[i * 3 + j])
              patterntrue = false;
        }
        if (patterntrue)
          useEvent = true;
      }
      if (!useEvent) {
        // 	      if(numbers[0]->GetValue()==2&&numbers[1]->GetValue()==0&&numbers[2]->GetValue()==1)
        // 		cout<<"skip event "<<numbers[0]->GetValue()<<"
        // "<<numbers[1]->GetValue()<<"
        // "<<numbers[2]->GetValue()<<" "<<currentEntry<<endl;
        currentEntry++;
      }
    }
    if (currentEntry == trackTree->GetEntries()) {
      valid = false;
      return;
    }
  }
  header[0]->GetBranch()->GetEntry(currentEntry);
  header[1]->GetBranch()->GetEntry(currentEntry);
  eventNumber = (int)header[0]->GetValue();
  runNumber = (int)header[1]->GetValue();
}

void ATrackTreeInput::readEvent(int EvtNr)
{
  //  cout<<"readEvent("<<EvtNr<<")"<<endl;
  if (!valid)
    return;
  if (trackTree == NULL)
    return;
  if (header[0] == NULL || header[1] == NULL || header[2] == NULL || numbers[0] == NULL ||
      numbers[1] == NULL || numbers[2] == NULL)
    return;
  numberOfTracks = 0;
  header[0]->GetBranch()->GetEntry(currentEntry);
  if (header[0]->GetValue() == EvtNr) {
    readEntry(currentEntry);
    return;
  }
  while (header[0]->GetValue() < EvtNr && trackTree->GetEntries() > currentEntry + 1) {
    currentEntry++;
    header[0]->GetBranch()->GetEntry(currentEntry);
  }
  while (header[0]->GetValue() > EvtNr && currentEntry > 0) {
    currentEntry--;
    header[0]->GetBranch()->GetEntry(currentEntry);
  }
  if (header[0]->GetValue() == EvtNr) {
    readEntry(currentEntry);
    return;
  }
  if (header[0]->GetValue() < EvtNr && trackTree->GetEntries() > currentEntry + 1) {
    readWriteLog << "ATrackTreeInput: Event not found: EventNumber " << EvtNr << " last Entry "
                 << currentEntry << endli;
    return;
  } else {
    cout << "what the heck!" << endl;
    currentEntry--;
    return;
  }
  if (currentEntry > 0)
    readEntry(currentEntry - 1);
  else
    readEntry(currentEntry);
}
void ATrackTreeInput::readEntry(int entry)
{
  // cout<<"readEntry("<<entry<<")"<<endl;
  if (!valid)
    return;
  if (trackTree == NULL)
    return;
  if (entry < 0 || entry >= trackTree->GetEntries()) {
    valid = false;
    return;
  }
  for (int i = 0; i < 14; i++)
    if (prompt[i] == NULL)
      return;
  for (int i = 0; i < 22; i++)
    if (vee[i] == NULL)
      return;
  for (int i = 0; i < 25; i++)
    if (kink[i] == NULL)
      return;
  for (int i = 0; i < 3; i++)
    if (header[i] == NULL)
      return;
  for (int i = 0; i < 3; i++)
    if (numbers[i] == NULL)
      return;
  trackTree->GetEntry(entry);
  eventNumber = (int)header[0]->GetValue();
  runNumber = (int)header[1]->GetValue();
  trigger = (int)header[2]->GetValue();
  readPrompt();
  readVee();
  readKink();
  currentEntry = entry + 1;
  //   if(numbers[0]->GetValue()!=2&&numbers[2]->GetValue()!=1)
  //     cout<<"bad! event "<<numbers[0]->GetValue()<<"
  //     "<<numbers[1]->GetValue()<<"
  //     "<<numbers[2]->GetValue()<<" "<<currentEntry<<endl;
  //   if(numbers[0]->GetValue()+numbers[2]->GetValue()*3!=numberOfTracks)
  //     cout<<"bad event "<<numbers[0]->GetValue()<<"
  //     "<<numbers[1]->GetValue()<<"
  //     "<<numbers[2]->GetValue()<<" "<<currentEntry<<"
  //     "<<numberOfTracks<<endl;

  //  cout<<numberOfTracks<<endl;
}
void ATrackTreeInput::assignHitsToTrack(TLeaf* det, TLeaf* el, TTrack* tr, int nel, int count)
{
  int d, e;
  TCalibHit* tmpHit;
  for (int j = 0; j < nel; j++) {
    tmpHit = NULL;
    d = (int)det->GetValue(count + j);
    e = (int)el->GetValue(count + j);
    if (d < 0 || d >= numDets)
      continue;
    for (int l = 0; l < (*(numberOfCalibHits[d])); l++) {
      if (hits[d][l]->getDetector() == d && hits[d][l]->getElement() == e && hits[d][l]->isValid())
        tmpHit = hits[d][l];
    }
    if (tmpHit != NULL) {
      // cout<<*tmpHit<<endl;
      tr->addCalibSrc(tmpHit);
    } else {
      // 	  cerr<<"ERROR: hit not found in ATrackTreeInput!
      // ("<<d<<","<<e<<")"<<endl;
      // 	  for(int l=0;l<(*(numberOfCalibHits[d]));l++)
      // 	    {
      // 	      cout<<*hits[d][l]<<endl;
      // 	    }
    }
  }
}
void ATrackTreeInput::assignPixelsToTrack(TTrack* tr)
{
  bool take;
  bool has;
  if (tr == NULL)
    return;
  for (int i = 0; i < numDets; i++) {
    for (int j = 0; j < (*(numberOfPixels[i])); j++) {
      take = true;
      for (int k = 0; k < pixels[i][j]->getNumberOfActualElements(); k++) {
        has = false;
        for (int l = 0; l < tr->getNumberOfCalibHits(pixels[i][j]->getElementr(k).getDetector());
             l++)
          if (tr->getCalibSrcr(pixels[i][j]->getElementr(k).getDetector(), l)->getElement() ==
              pixels[i][j]->getElementr(k).getElement())
            has = true;
        if (!has)
          take = false;
      }
      if (take)
        tr->addPixel(pixels[i][j]);
    }
  }
}
void ATrackTreeInput::readPrompt()
{
  vector3D v;
  point3D p;
  int ntmp = 0;
  // cout<<"readPrompt()"<<numbers[0]->GetValue()<<endl;
  for (int i = 0; i < numbers[0]->GetValue(); i++) {
    if (numberOfTracks >= maxtracks)
      continue;
    tracks[numberOfTracks]->reset();
    p.setValues(prompt[3]->GetValue(i), prompt[4]->GetValue(i), prompt[5]->GetValue(i));
    v.setValues(sin(prompt[0]->GetValue(i)) * cos(prompt[1]->GetValue(i)),
                sin(prompt[0]->GetValue(i)) * sin(prompt[1]->GetValue(i)),
                cos(prompt[0]->GetValue(i)));
    tracks[numberOfTracks]->setPath(sLine3D(p, v));
    tracks[numberOfTracks]->setSpeed(prompt[2]->GetValue(i));
    tracks[numberOfTracks]->setChiSquared(prompt[6]->GetValue(i));
    tracks[numberOfTracks]->setParticleID((int)prompt[7]->GetValue(i));
    assignHitsToTrack(prompt[9], prompt[10], tracks[numberOfTracks], (int)prompt[8]->GetValue(i),
                      ntmp);
    assignPixelsToTrack(tracks[numberOfTracks]);
    ntmp = ntmp + (int)prompt[8]->GetValue(i);
    if (((tracks[numberOfTracks]->getNumberOfCalibHits(15) > 0 &&
          tracks[numberOfTracks]->getNumberOfCalibHits(16) > 0) &&
         ((tracks[numberOfTracks]->getNumberOfCalibHits(0) > 0 &&
           tracks[numberOfTracks]->getNumberOfCalibHits(1) > 0 &&
           tracks[numberOfTracks]->getNumberOfCalibHits(2) > 0) ||
          (tracks[numberOfTracks]->getNumberOfCalibHits(3) > 0 &&
           tracks[numberOfTracks]->getNumberOfCalibHits(4) > 0 &&
           tracks[numberOfTracks]->getNumberOfCalibHits(5) > 0) ||
          (tracks[numberOfTracks]->getNumberOfCalibHits(6) > 0 &&
           tracks[numberOfTracks]->getNumberOfCalibHits(7) >
               0)))) { //(15*16)*((0*1*2)+(3*4*5)+(6*7))
      setTrackErrors(tracks[numberOfTracks]);
      numberOfTracks++;
    } else {
      /*	  cout<<prompt[8]->GetValue(i)<<" "<<flush;
      cout<<*(tracks[numberOfTracks])<<endl;
      cout<<tracks[numberOfTracks]->getNumberOfCalibHits()<<endl;*/
      cout << "bad track " << eventNumber << "(" << flush;
      for (int j = 0; j < prompt[8]->GetValue(i); j++) {
        cout << prompt[9]->GetValue(ntmp - ((int)prompt[8]->GetValue(i)) + j) << " "
             << prompt[10]->GetValue(ntmp - ((int)prompt[8]->GetValue(i)) + j) << "," << flush;
      }
      cout << ")" << endl;
    }
  }
  //  cout<<numbers[0]->GetValue()<<" "<<numberOfTracks<<endl;
}
void ATrackTreeInput::readVee()
{
  vector3D v1, v2;
  point3D p;
  int ntmp = 0;
  for (int i = 0; i < numbers[2]->GetValue(); i++) {
    if (numberOfTracks + 2 >= maxtracks)
      continue;
    tracks[numberOfTracks]->reset();
    tracks[numberOfTracks + 1]->reset();
    tracks[numberOfTracks + 2]->reset();
    p.setValues(vee[6]->GetValue(i), vee[7]->GetValue(i), vee[8]->GetValue(i));
    v1.setValues(sin(vee[0]->GetValue(i)) * cos(vee[1]->GetValue(i)),
                 sin(vee[0]->GetValue(i)) * sin(vee[1]->GetValue(i)), cos(vee[0]->GetValue(i)));
    v2.setValues(sin(vee[3]->GetValue(i)) * cos(vee[4]->GetValue(i)),
                 sin(vee[3]->GetValue(i)) * sin(vee[4]->GetValue(i)), cos(vee[3]->GetValue(i)));
    tracks[numberOfTracks + 1]->setPath(sLine3D(p, v1));
    tracks[numberOfTracks + 2]->setPath(sLine3D(p, v2));
    tracks[numberOfTracks + 1]->setSpeed(vee[2]->GetValue(i));
    tracks[numberOfTracks + 2]->setSpeed(vee[5]->GetValue(i));
    tracks[numberOfTracks]->setChiSquared(vee[9]->GetValue(i));
    tracks[numberOfTracks + 1]->setChiSquared(vee[9]->GetValue(i));
    tracks[numberOfTracks + 2]->setChiSquared(vee[9]->GetValue(i));
    tracks[numberOfTracks + 1]->setParticleID((int)vee[10]->GetValue(i));
    tracks[numberOfTracks + 2]->setParticleID((int)vee[11]->GetValue(i));
    assignHitsToTrack(vee[14], vee[15], tracks[numberOfTracks + 1], (int)vee[12]->GetValue(i),
                      ntmp);
    ntmp = ntmp + (int)vee[12]->GetValue(i);
    assignHitsToTrack(vee[14], vee[15], tracks[numberOfTracks + 2], (int)vee[13]->GetValue(i),
                      ntmp);
    assignPixelsToTrack(tracks[numberOfTracks + 1]);
    assignPixelsToTrack(tracks[numberOfTracks + 2]);
    ntmp = ntmp + (int)vee[13]->GetValue(i);
    tracks[numberOfTracks]->addSecondary(tracks[numberOfTracks + 1]);
    tracks[numberOfTracks]->addSecondary(tracks[numberOfTracks + 2]);
    tracks[numberOfTracks + 1]->setOrder(true);
    tracks[numberOfTracks + 2]->setOrder(true);
    setTrackErrors(tracks[numberOfTracks + 1]);
    setTrackErrors(tracks[numberOfTracks + 2]);
    numberOfTracks = numberOfTracks + 3;
  }
}
void ATrackTreeInput::readKink()
{
  vector3D v1, v2;
  point3D p1, p2;
  int ntmp = 0;
  for (int i = 0; i < numbers[1]->GetValue(); i++) {
    if (numberOfTracks + 1 >= maxtracks)
      continue;
    tracks[numberOfTracks]->reset();
    tracks[numberOfTracks + 1]->reset();
    p1.setValues(kink[6]->GetValue(i), kink[7]->GetValue(i), kink[8]->GetValue(i));
    p2.setValues(kink[9]->GetValue(i), kink[10]->GetValue(i), kink[11]->GetValue(i));
    v1.setValues(sin(kink[0]->GetValue(i)) * cos(kink[1]->GetValue(i)),
                 sin(kink[0]->GetValue(i)) * sin(kink[1]->GetValue(i)), cos(kink[0]->GetValue(i)));
    v2.setValues(sin(kink[3]->GetValue(i)) * cos(kink[4]->GetValue(i)),
                 sin(kink[3]->GetValue(i)) * sin(kink[4]->GetValue(i)), cos(kink[3]->GetValue(i)));
    tracks[numberOfTracks]->setPath(sLine3D(p1, v1));
    tracks[numberOfTracks + 1]->setPath(sLine3D(p2, v2));
    tracks[numberOfTracks]->setSpeed(kink[2]->GetValue(i));
    tracks[numberOfTracks + 1]->setSpeed(kink[5]->GetValue(i));
    tracks[numberOfTracks]->setChiSquared(kink[12]->GetValue(i));
    tracks[numberOfTracks + 1]->setChiSquared(kink[12]->GetValue(i));
    tracks[numberOfTracks]->setParticleID((int)kink[13]->GetValue(i));
    tracks[numberOfTracks + 1]->setParticleID((int)kink[14]->GetValue(i));
    assignHitsToTrack(kink[17], kink[18], tracks[numberOfTracks], (int)kink[15]->GetValue(i), ntmp);
    ntmp = ntmp + (int)kink[15]->GetValue(i);
    assignHitsToTrack(kink[17], kink[18], tracks[numberOfTracks], (int)kink[16]->GetValue(i), ntmp);
    assignPixelsToTrack(tracks[numberOfTracks]);
    assignPixelsToTrack(tracks[numberOfTracks + 1]);
    ntmp = ntmp + (int)kink[16]->GetValue(i);
    tracks[numberOfTracks]->addSecondary(tracks[numberOfTracks + 1]);
    tracks[numberOfTracks + 1]->setOrder(true);
    setTrackErrors(tracks[numberOfTracks]);
    setTrackErrors(tracks[numberOfTracks + 1]);
    numberOfTracks = numberOfTracks + 2;
  }
}
void ATrackTreeInput::clearTree()
{
  if (trackTree != NULL) {
    TFile* f = trackTree->GetCurrentFile();
    f->Close();
    delete f;
    trackTree = NULL;
  }
  currentEntry = -1;
  for (int i = 0; i < 14; i++)
    prompt[i] = NULL;
  for (int i = 0; i < 22; i++)
    vee[i] = NULL;
  for (int i = 0; i < 25; i++)
    kink[i] = NULL;
  for (int i = 0; i < 3; i++)
    header[i] = NULL;
  for (int i = 0; i < 6; i++)
    numbers[i] = NULL;
  valid = false;
  if (localDirectory) {
    if (copyFile != "") {
      string command = "rm -f ";
      command = command + copyFile;
      cout << "bash # " << command.data() << endl;
      system(command.data());
    }
  }
}
algorithm_parameter ATrackTreeInput::getDescription()
{
  algorithm_parameter ret("Read Tracks from Tree", 0, 0);
  string des = "This algorithm reads tracks from a root tree and"
               "requests the hits from the setup."
               "It does so for prompt, kink and vee tracks separately.";
  ret.setDescription(des);
  ret.addParam<bool>(single_parameter<bool>(string("search for event"), true));
  ret.addParam<bool>(single_parameter<bool>(string("use as event input list"), true));
  ret.addParam<bool>(single_parameter<bool>(string("read only certain event pattern"), true));
  ret.addParam<bool>(single_parameter<bool>(string("use local directory"), true));
  ret.addParam<string>(single_parameter<string>(string("local directory"), string("")));
  vector<int> tmpI;
  ret.addParam<vector<int>>(single_parameter<vector<int>>(string("event pattern"), tmpI));
  return ret;
}
extern bool existing(string filename);
void ATrackTreeInput::onNewRun(run_parameter& run)
{
  readWriteLog << "ATrackTreeInput::onNewRun()" << endli;
  clearTree();
  string filename = "";
  for (size_t i = 0; i < run.getNumberOfFiles(); i++) {
    //      cout<<run.getFileType(i)<<" "<<run.getFile(i).data()<<endl;
    if (run.getFileType(i) == 513 && existing(run.getFile(i)))
      filename = run.getFile(i);
  }
  if (!existing(filename))
    return;
  readWriteLog << "TrackTreeInput: " << filename.data() << endli;
  if (localDirectory) {
    if (!existing(directory)) {
      cout << "local directory \"" << directory.data() << "\"doesn\'t exist: create it" << endl;
      vector<string> parentdirs;
      string tmps = directory;
      while (tmps.find("/") < tmps.npos && tmps.rfind("/") > 0) {
        parentdirs.push_back(tmps.substr(0, tmps.rfind("/")));
        tmps = tmps.substr(0, tmps.rfind("/"));
        if (tmps.length() < 1) {
          tmps = parentdirs.back();
          parentdirs.pop_back();
        }
      }
      for (int i = parentdirs.size() - 1; i >= 0; i--) {
        if (existing(parentdirs[i])) {
          cout << parentdirs[i].data() << " exists" << endl;
          continue;
        }
        cout << "mkdir " << parentdirs[i].data() << endl;
        system((string("mkdir ") + parentdirs[i]).data());
      }
      system((string("mkdir ") + directory).data());
    }
    copyFile = filename;
    if (hasA(copyFile, "/"))
      copyFile =
          copyFile.substr(copyFile.rfind("/") + 1, copyFile.length() - copyFile.rfind("/") - 1);
    copyFile = directory + copyFile;
    if (copyFile == filename) {
      copyFile = "";
    } else {
      string command = "cp ";
      command = command + filename + " " + copyFile;
      cout << "bash # " << command.data() << endl;
      system(command.data());
      filename = copyFile;
      if (!existing(copyFile)) {
        cout << "error during copy file \"" << copyFile.data() << "\" doesn\'t exist" << endl;
      }
    }
  }
  TFile* f = new TFile(filename.data(), "read");
  trackTree = (TTree*)f->Get("trackTree");
  if (trackTree == NULL) {
    readWriteLog << "Bad track file" << endli;
    delete f;
    return;
  }
  readWriteLog << "Open file \"" << filename.data() << "\" for track tree input with "
               << (int)trackTree->GetEntries() << " Entries" << endli;
  header[0] = trackTree->FindLeaf("eventNumber");
  header[1] = trackTree->FindLeaf("runNumber");
  header[2] = trackTree->FindLeaf("trigger");
  numbers[0] = trackTree->FindLeaf("numberOfPrompt");
  numbers[1] = trackTree->FindLeaf("numberOfKink");
  numbers[2] = trackTree->FindLeaf("numberOfVee");
  numbers[3] = trackTree->FindLeaf("numberOfPromptHits");
  numbers[4] = trackTree->FindLeaf("numberOfKinkHits");
  numbers[5] = trackTree->FindLeaf("numberOfVeeHits");
  prompt[0] = trackTree->FindLeaf("prompt.theta");
  prompt[1] = trackTree->FindLeaf("prompt.phi");
  prompt[2] = trackTree->FindLeaf("prompt.beta");
  prompt[3] = trackTree->FindLeaf("prompt.vertex_x");
  prompt[4] = trackTree->FindLeaf("prompt.vertex_y");
  prompt[5] = trackTree->FindLeaf("prompt.vertex_z");
  prompt[6] = trackTree->FindLeaf("prompt.chi");
  prompt[7] = trackTree->FindLeaf("prompt.ID");
  prompt[8] = trackTree->FindLeaf("prompt.nEl");
  prompt[9] = trackTree->FindLeaf("prompt.det");
  prompt[10] = trackTree->FindLeaf("prompt.el");
  prompt[11] = trackTree->FindLeaf("prompt.theta_kf");
  prompt[12] = trackTree->FindLeaf("prompt.phi_kf");
  prompt[13] = trackTree->FindLeaf("prompt.beta_kf");
  vee[0] = trackTree->FindLeaf("vee.theta1");
  vee[1] = trackTree->FindLeaf("vee.phi1");
  vee[2] = trackTree->FindLeaf("vee.beta1");
  vee[3] = trackTree->FindLeaf("vee.theta2");
  vee[4] = trackTree->FindLeaf("vee.phi2");
  vee[5] = trackTree->FindLeaf("vee.beta2");
  vee[6] = trackTree->FindLeaf("vee.vertex_x");
  vee[7] = trackTree->FindLeaf("vee.vertex_y");
  vee[8] = trackTree->FindLeaf("vee.vertex_z");
  vee[9] = trackTree->FindLeaf("vee.chi");
  vee[10] = trackTree->FindLeaf("vee.ID1");
  vee[11] = trackTree->FindLeaf("vee.ID2");
  vee[12] = trackTree->FindLeaf("vee.nEl1");
  vee[13] = trackTree->FindLeaf("vee.nEl2");
  vee[14] = trackTree->FindLeaf("vee.det");
  vee[15] = trackTree->FindLeaf("vee.el");
  vee[16] = trackTree->FindLeaf("vee.theta1_kf");
  vee[17] = trackTree->FindLeaf("vee.phi1_kf");
  vee[18] = trackTree->FindLeaf("vee.beta1_kf");
  vee[19] = trackTree->FindLeaf("vee.theta2_kf");
  vee[20] = trackTree->FindLeaf("vee.phi2_kf");
  vee[21] = trackTree->FindLeaf("vee.beta2_kf");
  kink[0] = trackTree->FindLeaf("kink.theta_p");
  kink[1] = trackTree->FindLeaf("kink.phi_p");
  kink[2] = trackTree->FindLeaf("kink.beta_p");
  kink[3] = trackTree->FindLeaf("kink.theta_d");
  kink[4] = trackTree->FindLeaf("kink.phi_d");
  kink[5] = trackTree->FindLeaf("kink.beta_d");
  kink[6] = trackTree->FindLeaf("kink.vertex_p_x");
  kink[7] = trackTree->FindLeaf("kink.vertex_p_y");
  kink[8] = trackTree->FindLeaf("kink.vertex_p_z");
  kink[9] = trackTree->FindLeaf("kink.vertex_d_x");
  kink[10] = trackTree->FindLeaf("kink.vertex_d_y");
  kink[11] = trackTree->FindLeaf("kink.vertex_d_z");
  kink[12] = trackTree->FindLeaf("kink.chi");
  kink[13] = trackTree->FindLeaf("kink.ID_p");
  kink[14] = trackTree->FindLeaf("kink.ID_d");
  kink[15] = trackTree->FindLeaf("kink.nEl_p");
  kink[16] = trackTree->FindLeaf("kink.nEl_d");
  kink[17] = trackTree->FindLeaf("kink.det");
  kink[18] = trackTree->FindLeaf("kink.el");
  kink[19] = trackTree->FindLeaf("kink.theta_p_kf");
  kink[20] = trackTree->FindLeaf("kink.phi_p_kf");
  kink[21] = trackTree->FindLeaf("kink.beta_p_kf");
  kink[22] = trackTree->FindLeaf("kink.theta_d_kf");
  kink[23] = trackTree->FindLeaf("kink.phi_d_kf");
  kink[24] = trackTree->FindLeaf("kink.beta_d_kf");
  bool ready = true;
  for (int i = 0; i < 14; i++)
    if (prompt[i] == NULL)
      ready = false;
  for (int i = 0; i < 22; i++)
    if (vee[i] == NULL)
      ready = false;
  for (int i = 0; i < 25; i++)
    if (kink[i] == NULL)
      ready = false;
  for (int i = 0; i < 3; i++)
    if (header[i] == NULL)
      ready = false;
  for (int i = 0; i < 3; i++)
    if (numbers[i] == NULL)
      ready = false;
  valid = ready;
  currentEntry = 0;
}
