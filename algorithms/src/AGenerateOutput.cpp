#include "AGenerateOutput.h"
#include <TFile.h>
//#include "/users/scr0_pc46/pion/TOF/analysisCompare/OutFiles.h"
#include "beamtimeparameter.h"
#include "logger.h"
#include "reactions.h"
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <pthread.h>
float mass[50];
Int_t** fromSave;
TTree** treesave;
extern bool existing(string strFilename);
logger outLogged("filesFinished.txt", "");
AGenerateOutput::AGenerateOutput(TEvent& evIn, const string& mcFileName,
                                 const string& outFileNameIn, const string& localD, bool useLocalD,
                                 int maxt, bool umc, bool writeT, bool writeC, bool writeP,
                                 bool writeH, bool writeR, vector<string> reacts, int serverIn,
                                 bool addFileIn, bool exchangeFileIn, void* outmutexIn)
    : AAlgorithm("Generate output for track comparison"), event(evIn),
      maxOfHits(evIn.getMaxNumber<TCalibHit>() * 2), maxOfPixel(evIn.getMaxNumber<TPixel>() * 3),
      maxOfCluster(evIn.getMaxNumber<TCluster>() * 3), server(serverIn == 0), addFile(addFileIn),
      exchangeFile(exchangeFileIn)
{
  fromSave = new Int_t*[50];
  treesave = new TTree*[50];
  outmutex = outmutexIn;
  //  cout<<1<<endl;
  writeTracks = writeT;
  writeCluster = writeC;
  writePixel = writeP;
  writeHits = writeH;
  writeReactions = writeR;
  outfileName = outFileNameIn;
  useLocal = useLocalD;
  localDir = localD;
  if (useLocal) {
    if (localD == "")
      useLocal = false;
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
          tmp.setPath((tmpPath + components[i]).data());
          if (!tmp.exists()) {
            QDir tmp2(tmpPath.data());
            tmp2.mkdir(components[i].data());
          }
          tmpPath += components[i] + "/";
        }
      }
    }
  }
  numT = 0;
  numC = 0;
  numP = 0;
  numH = 0;
  //  outp=new float[20];
  //  for(int i=0;i<5;i++)outp[i]=0;
  maxtr = maxt;
  if (maxtr > 1000)
    maxtr = 1000;

  eventNr = -1;
  //   TFile *f=new TFile((outfileName+".root").data(),"RECREATE","",3);
  //   f->cd();

  // cout<<3<<endl;
  if ((mcFileName == "" || (!existing(mcFileName))) || (!umc))
    useMC = false;
  else
    useMC = true;
  if (useMC) {
    getMCTree(mcFileName);
    mcFile.open(mcFileName.data());
  }
  filepattern = outFileNameIn;
  string extR;
  string extT;
  string extP;
  string extC;
  string extH;
  string fnpt;
  if (server)
    fnpt = "";
  else
    fnpt = string_number(serverIn) + string("_tmp_");
  filepattern = outFileNameIn + fnpt;
  extR = outFileNameIn + fnpt + string("R.root");
  extT = outFileNameIn + fnpt + string("T.root");
  extP = outFileNameIn + fnpt + string("P.root");
  extC = outFileNameIn + fnpt + string("C.root");
  extH = outFileNameIn + fnpt + string("H.root");
  if (useLocal) {
    string fn = outFileNameIn.substr(outFileNameIn.rfind("/") + 1,
                                     outFileNameIn.length() - outFileNameIn.rfind("/") - 1);
    extR = localDir + "/" + fn + fnpt + "R.root";
    extT = localDir + "/" + fn + fnpt + "T.root";
    extP = localDir + "/" + fn + fnpt + "P.root";
    extC = localDir + "/" + fn + fnpt + "C.root";
    extH = localDir + "/" + fn + fnpt + "H.root";
  }
  // cout<<4<<" "<<writeTracks<<" "<<outfileName.data()<<endl;
  if (writeTracks || writeReactions) {
    trackFile = NULL;
    trackTree = new TTree*[maxtr];
    for (int i = 0; i < maxtr; i++)
      trackTree[i] = NULL;
    Track = new track1*[maxtr];
    fromTrack = new Int_t*[maxtr];
    hitsOfTrack = new Int_t**[maxtr];
    for (int i = 0; i < maxtr; i++) {
      Track[i] = NULL;
      fromTrack[i] = NULL;
      hitsOfTrack[i] = NULL;
      trackTree[i] = NULL;
    }
    // cout<<5<<" "<<extT.data()<<endl;
    if (writeTracks) {
      trackFile = new TFile(extT.data(), "RECREATE");
      trackFile->cd();
      // cout<<6<<endl;
      numTrackTree = new TTree("numberOfTracks", "numberOfTracks");
      // cout<<7<<endl;
      numTrackTree->Branch("NumberOfTracks", &numTr, "NumberOfTracks/I");
      numTrackTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
      numTrackTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
      numTrackTree->Branch("Trigger", &event.getTrigger(), "Trigger/I");
      // cout<<8<<endl;
      saver = new Int_t**[50];
    }
    expandTree(3, 5);
    // cout<<9<<endl;
    // cout<<hitsOfTrack.size()<<" "<<hitsOfTrack[0]<<endl;
    if (writeTracks && server)
      for (int i = 0; i < numT; i++)
        saver[i] = (hitsOfTrack[i]);
    // cout<<hitsOfTrack[0]<<endl;
    reactionFile = NULL;
    if (writeReactions) {
      reactionFile = new TFile(extR.data(), "RECREATE");
      if (reactionFile != NULL)
        reactionFile->cd();
      setReactions(reacts, outfileName);
    }
  }
  if (writeHits) {
    hitTree = new TTree*[maxtr];
    for (int i = 0; i < maxtr; i++)
      hitTree[i] = NULL;
    Hits = new hit*[maxtr];
    hitFile = new TFile(extH.data(), "RECREATE", "", 3);
    numHitTree = new TTree("numberOfHits", "numberOfHits");
    numHitTree->Branch("NumberOfHits", &numberOfHits, "NumberOfHits/I");
    numHitTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
    numHitTree->Branch("Trigger", &event.getTrigger(), "Trigger/I");
    numHitTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
    expandTree(0, 20);
  }
  if (writePixel) {
    pixTree = new TTree*[maxtr];
    Pix = new pixel*[maxtr];
    pixFile = new TFile(extP.data(), "RECREATE", "", 3);
    numPixTree = new TTree("numberOfPixels", "numberOfPixels");
    numPixTree->Branch("NumberOfPixels", &numberOfPixels, "NumberOfPixels/I");
    numPixTree->Branch("Trigger", &event.getTrigger(), "Trigger/I");
    numPixTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
    numPixTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
    expandTree(1, 20);
  }
  if (writeCluster) {
    cluTree = new TTree*[maxtr];
    Clus = new cluster*[maxtr];
    cluFile = new TFile(extC.data(), "RECREATE", "", 3);
    numCluTree = new TTree("numberOfClusters", "numberOfClusters");
    numCluTree->Branch("NumberOfClusters", &numberOfCluster, "NumberOfClusters/I");
    numCluTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
    numCluTree->Branch("Trigger", &event.getTrigger(), "Trigger/I");
    numCluTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
    expandTree(2, 20);
  }
  //  InitWriteOut("Tuebingen");
}
#include "logger.h"
#include <TLeaf.h>
AGenerateOutput::~AGenerateOutput()
{
  delete[] fromSave;
  delete[] treesave;
  if (outmutex != NULL)
    pthread_mutex_lock((pthread_mutex_t*)outmutex);
  //  if(server)
  {
    if (writeTracks || writeReactions) {
      TFile* f = NULL;
      if (writeTracks) {
        f = trackTree[0]->GetCurrentFile(); //((outfileName+".root").data(),"RECREATE");
        f->cd();
        //      numTrackTree->GetCurrentFile()->cd();
        TTree* tmpTree;
        for (int i = 0; i < numT; i++) {
          tmpTree = trackTree[i];
          if (tmpTree != NULL)
            tmpTree->Write();
          // tmpTree->SetDirectory(0);
          //	trackTree[i]->Write();
        }
        tmpTree = numTrackTree;
        tmpTree->Write();
        // tmpTree->SetDirectory(0);
        trackTree[0]->SetBranchStatus("*", 0);
        trackTree[0]->SetBranchStatus("EventNumber", 1);
        cout << "last Event number for tracks: " << endl;
        trackTree[0]->Show(trackTree[0]->GetEntries() - 1);
        cout << f->GetName() << " file written" << endl;
        outLogged << "file written: " << f->GetName() << endli;
        trackTree[0]->GetBranch("EventNumber")->GetEntry(trackTree[0]->GetEntries() - 1);
        outLogged << "last Event number for tracks: "
                  << trackTree[0]->FindLeaf("EventNumber")->GetValue();
        outLogged << ", " << (int)trackTree[0]->GetEntries() << " Entries" << endli;
        if (f->IsOpen())
          f->Close();
        if (useLocal) {
#if QT_VERSION < 0x040000
          string command = string("cp ") + f->GetName() + " " + filepattern.data() + "T.root";
          QProcess copy(QString(command.data()));
          copy.addArgument((f->GetName()));
          copy.addArgument((filepattern + "T.root").data());
          copy.start();
          while (copy.isRunning()) {
          }
#else
          QProcess copy;
          copy.start("cp", QStringList{} << f->GetName() << (filepattern + "T.root").data(), {});
          while (copy.waitForFinished()) {
          }
#endif
          cout << "copied to:" << filepattern.data() << "T.root "
               << (existing(filepattern + "T.root") ? "successful" : "failed") << endl;
        }
      }
      if (writeReactions) {
        reactionFile->cd();
        TFile** fil = new TFile*[numReact];
        for (int i = 0; i < numReact; i++)
          fil[i] = NULL;
        for (int i = 0; i < numReact; i++) {
          delete[] reactIDs[i];
          fil[i] = reactTree[i]->GetCurrentFile();
          fil[i]->cd();
          reactTree[i]->Write();
          for (int j = 0; j < i; j++)
            if (fil[i] == fil[j])
              fil[i] = NULL;
          cout << reactTree[i]->GetName() << " written to file " << reactTree[i]->GetEntries()
               << " Entries" << endl;
        }
        for (int i = 0; i < numReact; i++)
          if (fil[i] != NULL) {
            fil[i]->Close();
            delete fil[i];
          }
        delete[] fil;
        if (reactionFile->IsOpen())
          reactionFile->Close();
        if (useLocal) {
#if QT_VERSION < 0x040000
          string command = string("cp ") + f->GetName() + " " + filepattern.data() + "R.root";
          QProcess copy(QString(command.data()));
          copy.addArgument((f->GetName()));
          copy.addArgument((filepattern + "T.root").data());
          copy.start();
          while (copy.isRunning()) {
          }
#else
          QProcess copy;
          copy.start("cp", QStringList{} << f->GetName() << (filepattern + "R.root").data(), {});
          while (copy.waitForFinished()) {
          }
#endif
          cout << "copied to:" << filepattern.data() << "R.root" << endl;
        }
        delete[] reactIDs;
      }
      //       if(writeTracks)
      // 	if(f->IsOpen())f->Close();
    }
    if (writePixel) {
      TFile* f = pixTree[0]->GetCurrentFile(); //((outfileName+".root").data(),"RECREATE");
      f->cd();
      for (int i = 0; i < numP; i++) {
        pixTree[i]->Write();
        pixTree[i]->SetDirectory(0);
      }
      pixTree[0]->SetBranchStatus("*", 0);
      pixTree[0]->SetBranchStatus("EventNumber", 1);
      cout << "last Event number for pixels: " << endl;
      pixTree[0]->Show(pixTree[0]->GetEntries() - 1);
      numPixTree->GetCurrentFile()->cd();
      numPixTree->Write();
      // numPixTree->SetDirectory(0);
      f->Close();
      if (useLocal) {
#if QT_VERSION < 0x040000
        string command = string("cp ") + f->GetName() + " " + filepattern.data() + "P.root";
        QProcess copy(QString(command.data()));
        copy.addArgument((f->GetName()));
        copy.addArgument((filepattern + "T.root").data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start("cp", QStringList{} << f->GetName() << (filepattern + "P.root").data(), {});
        while (copy.waitForFinished()) {
        }
#endif
        cout << "copied to:" << filepattern.data() << "P.root" << endl;
      }
      cout << f->GetName() << " file written" << endl;
    }
    if (writeHits) {
      TFile* f = hitTree[0]->GetCurrentFile(); //((outfileName+".root").data(),"RECREATE");
      f->cd();
      // 	  for(int i=0;i<numH;i++)
      // 	    {
      // 	      hitTree[i]->Write();
      // 	      //hitTree[i]->SetDirectory(0);
      // 	    }
      hitTree[0]->SetBranchStatus("*", 0);
      hitTree[0]->SetBranchStatus("EventNumber", 1);
      cout << "last Event number for hits: " << endl;
      hitTree[0]->Show(hitTree[0]->GetEntries() - 1);
      numHitTree->GetCurrentFile()->cd();
      // 	  numHitTree->Write();
      // numHitTree->SetDirectory(0);
      f->Write();
      f->Close();
      if (useLocal) {
#if QT_VERSION < 0x040000
        string command = string("cp ") + f->GetName() + " " + filepattern.data() + "H.root";
        QProcess copy(QString(command.data()));
        copy.addArgument((f->GetName()));
        copy.addArgument((filepattern + "T.root").data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start("cp", QStringList{} << f->GetName() << (filepattern + "H.root").data(), {});
        while (copy.waitForFinished()) {
        }
#endif
        cout << "copied to:" << filepattern.data() << "H.root" << endl;
      }
      cout << f->GetName() << " file written" << endl;
    }
    if (writeCluster) {
      TFile* f = cluTree[0]->GetCurrentFile(); //((outfileName+".root").data(),"RECREATE");
      f->cd();
      for (int i = 0; i < numC; i++) {
        cluTree[i]->Write();
        // cluTree[i]->SetDirectory(0);
      }
      cluTree[0]->SetBranchStatus("*", 0);
      cluTree[0]->SetBranchStatus("EventNumber", 1);
      cout << "last Event number for clusters: " << endl;
      cluTree[0]->Show(cluTree[0]->GetEntries() - 1);
      numCluTree->GetCurrentFile()->cd();
      numCluTree->Write();
      // numCluTree->SetDirectory(0);
      f->Close();
      if (useLocal) {
#if QT_VERSION < 0x040000
        string command = string("cp ") + f->GetName() + " " + filepattern.data() + "C.root";
        QProcess copy(QString(command.data()));
        copy.addArgument((f->GetName()));
        copy.addArgument((filepattern + "T.root").data());
        copy.start();
        while (copy.isRunning()) {
        }
#else
        QProcess copy;
        copy.start("cp", QStringList{} << f->GetName() << (filepattern + "C.root").data(), {});
        while (copy.waitForFinished()) {
        }
#endif
        cout << "copied to:" << filepattern.data() << "C.root" << endl;
      }
      cout << f->GetName() << " file written" << endl;
    }
  }
  //   else
  //     {
  //       if(writeTracks)
  // 	{
  // 	  vector<TTree*> tmp;
  // 	  for(int i=0;i<numT;i++)tmp.push_back(trackTree[i]);
  // 	  emit finalizeTracks(&tmp,numTrackTree);
  // 	  trackFile->Delete();
  // 	}
  //       if(writeReactions)
  // 	{
  // 	  vector<TTree*> tmp;
  // 	  for(int i=0;i<numReact;i++)tmp.push_back(reactTree[i]);
  // 	  emit finalizeReactions(&tmp);
  // 	}
  //       if(writeHits)
  // 	{
  // 	  vector<TTree*> tmp;
  // 	  for(int i=0;i<numH;i++)tmp.push_back(hitTree[i]);
  // 	  emit finalizeHits(&tmp,numHitTree);
  // 	  hitFile->Delete();
  // 	}
  //       if(writeCluster)
  // 	{
  // 	  vector<TTree*> tmp;
  // 	  for(int i=0;i<numC;i++)tmp.push_back(cluTree[i]);
  // 	  emit finalizeCluster(&tmp,numCluTree);
  // 	  cluFile->Delete();
  // 	}
  //       if(writePixel)
  // 	{
  // 	  vector<TTree*> tmp;
  // 	  for(int i=0;i<numP;i++)tmp.push_back(pixTree[i]);
  // 	  emit finalizePixel(&tmp,numPixTree);
  // 	  pixFile->Delete();
  // 	}
  //     }
  if (outmutex != NULL)
    pthread_mutex_unlock((pthread_mutex_t*)outmutex);
  if (outmutex != NULL && server)
    delete (pthread_mutex_t*)outmutex;
  if (useMC) {
    mcFile.close();
    delete mcTree;
  }
  if (writeTracks || writeReactions) {
    if (writeReactions) {
      for (int i = 0; i < numReact; i++) {
        // TTree *tmpTree=reactTree[i];
        // delete tmpTree;
        delete reactions[i];
      }
      delete[] reactions;
    }
    if (writeTracks) {
      int n = 5;
      for (int i = 0; i < numT; i++) {
        if (fromTrack[i] != NULL)
          delete[] fromTrack[i];
        if (i == 2)
          n = 10;
        if (i == 3)
          n = 20;
        for (int j = 0; j < n; j++)
          if (hitsOfTrack[i][j] != NULL)
            delete[] hitsOfTrack[i][j];
        if (hitsOfTrack[i] != NULL)
          delete[] hitsOfTrack[i];
      }
    }
    for (int i = 0; i < numT; i++)
      delete[] Track[i];
    delete[] trackTree;
    delete[] Track;
    delete[] fromTrack;
    delete[] hitsOfTrack;
  }
  if (writeCluster) {
    for (int i = 0; i < numC; i++) {
      // TTree *tmpTree=cluTree[i];
      // delete tmpTree;
      cluster* tmpclu = Clus[i];
      delete tmpclu;
    }
    // delete numCluTree;
    // delete cluTree;
    delete[] Clus;
    delete[] cluTree;
  }
  if (writePixel) {
    for (int i = 0; i < numP; i++) {
      // TTree *tmpTree=pixTree[i];
      // delete tmpTree;
      pixel* tmpclu = Pix[i];
      delete tmpclu;
    }
    // delete numPixTree;
    delete[] Pix;
    delete[] pixTree;
  }
  if (writeHits) {
    for (int i = 0; i < numH; i++) {
      // TTree *tmpTree=hitTree[i];
      // if(tmpTree)delete tmpTree;
      hit* tmpclu = Hits[i];
      delete tmpclu;
    }
    // delete numHitTree;
    delete[] Hits;
    delete[] hitTree;
  }
  //  anaLog<<" open file "<<outfileName.data()<<".root for tree write"<<endli;
  //  EndWriteOut();
  //  outFile.close();
}
void AGenerateOutput::process()
{
  eventNr++;
  valid = (numTr > 0 && numTr < 6 ? 1 : 0);
  numTr = 0;
  if (writeHits) {
    write_Hits();
  }
  if (writePixel) {
    write_Pixels();
  }
  if (writeCluster) {
    write_Clusters();
  }
  if (writeTracks || writeReactions) {
    if (writeTracks) {

      //  	  for(int i=0;i<(numT)&&i<50;i++)
      //  	    fromTrack[i]=fromSave[i];
      //  	  for(int i=0;i<(numT)&&i<50;i++)
      //  	    trackTree[i]=treesave[i];
      // 	  for(int i=0;i<(numT)&&i<50;i++)
      // 	    hitsOfTrack[i]=saver[i];
      write_Tracks();
    }
    if (writeReactions)
      write_Reactions();
    clearData();
  }
}
void AGenerateOutput::clearData()
{
  trigger = -1;
  numTr = 0;
  valid = 0;
  int accnum;
  //  cout<<"clearData()"<<endl;
  for (int j = 0; j < numT; j++) {
    //      cout<<Track[j]<<endl;
    accnum = 20;
    if (j == 0 || j == 1)
      accnum = 5;
    if (j == 2)
      accnum = 10;
    for (int i = 0; i < accnum; i++) {
      Track[j][i].theta = -3;
      Track[j][i].phi = -5;
      Track[j][i].beta = -2;
      Track[j][i].thetaIn = -3;
      Track[j][i].phiIn = -5;
      Track[j][i].betaIn = -2;
      Track[j][i].nEl = 0;
      Track[j][i].nElIn = 0;
      fromTrack[j][i] = -1;
    }
  }
}

void AGenerateOutput::write_Tracks()
{
  eventNr = event.getEventNumber();
  numTr = event.getNumberOfTracks();
  if (numTr <= 0)
    return;
  int num = numTr;
  int numTrees = getNumTrees(numTr, 3);
  if (numTrees > numT)
    expandTree(3, numTr);
  if (num > maxtr)
    num = maxtr;
  TTrack* tr;
  int j = -1, b = 0;
  //  cout<<"write_Tracks() "<<num<<endl;
  for (int i = 0; i < numTr; i++) {
    if (i == 5) {
      b = 5;
      j++;
    }
    if (i == 10) {
      b = 10;
      j++;
    }
    if ((i % 20) == 0) {
      b = i;
      j++;
    }
    //      cout<<Track[j]<<" "<<fromTrack[j]<<" "<<flush;
    tr = &event.getTrack(i);
    //      cout<<1<<flush;
    Track[j][i - b].phi = tr->getPath().Direction().Phi();
    Track[j][i - b].theta = tr->getPath().Direction().Theta();
    Track[j][i - b].beta = tr->getSpeed();
    Track[j][i - b].vertex[0] = tr->getPath().Foot().X();
    Track[j][i - b].vertex[1] = tr->getPath().Foot().Y();
    Track[j][i - b].vertex[2] = tr->getPath().Foot().Z();
    Track[j][i - b].ID = -1;
    Track[j][i - b].nEl = tr->getNumberOfCalibHits();
    Track[j][i - b].momentum = tr->getParticler().Momentum().length();
    //      cout<<2<<flush;
    fromTrack[j][i - b] = -1;
    //      cout<<1<<flush;
    if (tr->isSecondary()) {
      TTrack* pttr = tr->getPrimary();
      for (int k = 0; k < num; k++)
        if (pttr == &event.getTrack(k))
          fromTrack[j][i - b] = k;
    }
    //      cout<<2<<flush;
    int n = 0;
    for (int jj = 0; jj < event.getMaxNumber<TDetector>(); jj++) {
      if (jj < 24) {
        if (jj <= 7) {
          Track[j][i - b].ss[jj] = -1;
          Track[j][i - b].qdc[jj] = -1;
          Track[j][i - b].tdc[jj] = -1;
        } else if (jj >= 15) {
          Track[j][i - b].ss[jj - 7] = -1;
          Track[j][i - b].qdc[jj - 7] = -1;
          Track[j][i - b].tdc[jj - 7] = -1;
        }
        //	      cout<<3<<flush;
        if (tr->getNumberOfCalibHits(jj) > 0 && (tr->getCalibSrcr(jj, 0))) {
          if (tr->getCalibSrcr(jj, 0)->isValid()) {
            if (tr->getCalibSrcr(jj, 0)->name() == "TCalibHit") {
              Track[j][i - b].ss[(jj <= 7 ? jj : jj - 7)] = tr->getCalibSrcr(jj, 0)->getElement();
              Track[j][i - b].qdc[(jj <= 7 ? jj : jj - 7)] = tr->getCalibSrcr(jj, 0)->getADC();
              Track[j][i - b].tdc[(jj <= 7 ? jj : jj - 7)] = tr->getCalibSrcr(jj, 0)->getTDC();
              int l = -1;
              for (int k = 0; k < event.getMaxNumber<TDetector>(); k++)
                for (int m = 0; m < event.getNumberOfHits(k); m++) {
                  if (event.getHit(k, m).isValid()) {
                    l++;
                    if (tr->getCalibSrcr(jj, 0) == &event.getHit(k, m)) {
                      hitsOfTrack[j][i - b][n] = l;
                      if (n < 30)
                        n++;
                    }
                  }
                }
            } else if (tr->getCalibSrcr(jj, 0)->name() == "THitCluster") {
              THitCluster* tmp = (THitCluster*)tr->getCalibSrcr(jj, 0);
              TCalibHit* tt;
              tt = tmp->getHit(0);
              Track[j][i - b].ss[(jj <= 7 ? jj : jj - 7)] = tt->getElement();
              Track[j][i - b].qdc[(jj <= 7 ? jj : jj - 7)] = tt->getADC();
              Track[j][i - b].tdc[(jj <= 7 ? jj : jj - 7)] = tt->getTDC();
              // cout<<"track "<<i<<" hitcluster found :
              // "<<tmp->getNumberOfHits()<<"
              // elements"<<endl;
              for (int iter = 0; iter < tmp->getNumberOfHits(); iter++) {
                tt = tmp->getHit(iter);
                int l = -1;
                for (int k = 0; k < event.getMaxNumber<TDetector>(); k++)
                  for (int m = 0; m < event.getNumberOfHits(k); m++) {
                    if (event.getHit(k, m).isValid()) {
                      l++;
                      if (tt == &event.getHit(k, m)) {
                        hitsOfTrack[j][i - b][n] = l;
                        if (n < 30)
                          n++;
                      }
                    }
                  }
              }
            }
          }
        }
      }
    }
    //      cout<<3<<flush;
    Track[j][i - b].nEl = n;
    for (int jj = n; jj < 30; jj++) {
      hitsOfTrack[j][i - b][jj] = -1;
    }
    //            cout<<4<<endl;
  }
  //  cout<<5<<endl;
  for (int i = 0; i < numTrees; i++) { // cout<<trackTree[i]<<endl;
    trackTree[i]->Fill();
  }
  //  cout<<6<<endl;
  numTrackTree->Fill();
  //  cout<<7<<endl;
}
void AGenerateOutput::write_Pixels()
{
  eventNr = event.getEventNumber();
  int l = 0;
  int o = -1;
  TPixel* tp;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfPixels(i); j++)
      l++;
  int numTrees = getNumTrees(l, 1);
  if (numTrees > numP)
    expandTree(1, l);
  l = 0;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfPixels(i); j++) {
      //	if(l<maxOfPixel)
      {
        tp = &event.getPixel(i, j);
        if (tp->getShape()) {
          Pix[l / 20][l % 20].numberOfElements = tp->getNumberOfActualElements();
          if (Pix[l / 20][l % 20].numberOfElements > 10)
            Pix[l / 20][l % 20].numberOfElements = 10;
          Pix[l / 20][l % 20].center[0] = tp->getShape()->getCenter().X();
          Pix[l / 20][l % 20].center[1] = tp->getShape()->getCenter().Y();
          Pix[l / 20][l % 20].center[2] = tp->getShape()->getCenter().Z();
          Pix[l / 20][l % 20].ID = i;
          for (int k = 0; k < Pix[l / 20][l % 20].numberOfElements; k++) {
            for (int m = 0; m < event.getMaxNumber<TDetector>(); m++)
              for (int n = 0; m < event.getNumberOfHits(m); n++)
                if (event.getHit(m, n).isValid()) {
                  o++;
                  if (&tp->getElementr(k) == &event.getHit(k, m))
                    Pix[l / 20][l % 20].elementNumber[k] = o;
                }
          }
          l++;
        }
      }
    }
  numberOfPixels = l;
  if (l > 0)
    for (int i = 0; i < numTrees; i++)
      pixTree[i]->Fill();
  if (l > 0)
    numPixTree->Fill();
}

void AGenerateOutput::write_Hits()
{
  eventNr = event.getEventNumber();
  int l = 0;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfHits(i); j++)
      if (event.getHit(i, j).isValid())
        l++;
  int numTrees = getNumTrees(l, 0);
  if (numTrees > numH)
    expandTree(0, l);
  l = 0;
  TCalibHit* th;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfHits(i); j++) {
      //	if(l<maxOfHits)
      {
        th = &event.getHit(i, j);
        if (th->isValid()) {
          Hits[l / 20][l % 20].qdcR = th->getRawADC();
          Hits[l / 20][l % 20].tdcR = th->getRawTDC();
          Hits[l / 20][l % 20].qdcC = th->getADC();
          Hits[l / 20][l % 20].tdcC = th->getTDC();
          Hits[l / 20][l % 20].el = th->getElement();
          Hits[l / 20][l % 20].det = th->getDetector();
          l++;
        }
      }
    }
  numberOfHits = l;
  if (l > 0)
    for (int i = 0; i < numTrees; i++)
      hitTree[i]->Fill();
  if (l > 0)
    numHitTree->Fill();
}
void AGenerateOutput::write_Clusters()
{
  eventNr = event.getEventNumber();
  int l = 0;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfClusters(i); j++)
      l++;
  int numTrees = getNumTrees(l, 2);
  if (numTrees > numC)
    expandTree(2, l);
  l = 0;
  int o = -1;
  TCluster* tc;
  for (int i = 0; i < event.getMaxNumber<TDetector>(); i++)
    for (int j = 0; j < event.getNumberOfClusters(i); j++) {
      //	if(l<maxOfCluster)
      {
        tc = &event.getCluster(i, j);
        if (tc->getNumberOfPixels() > 0) {
          Clus[l / 20][l % 20].numberOfPixels = tc->getNumberOfPixels();
          if (Clus[l / 20][l % 20].numberOfPixels > 10)
            Clus[l / 20][l % 20].numberOfPixels = 10;
          Clus[l / 20][l % 20].center[0] = tc->getCenter().X();
          Clus[l / 20][l % 20].center[1] = tc->getCenter().Y();
          Clus[l / 20][l % 20].center[2] = tc->getCenter().Z();
          Clus[l / 20][l % 20].ID = i;
          for (int k = 0; k < Clus[l / 20][l % 20].numberOfPixels; k++) {
            for (int m = 0; m < event.getMaxNumber<TDetector>(); m++)
              for (int n = 0; m < event.getNumberOfPixels(m); n++)
                if (event.getPixel(m, n).getShape()) {
                  o++;
                  if (&tc->getPixelr(k) == &event.getPixel(k, m))
                    Clus[l / 20][l % 20].pixelNumber[k] = o;
                }
          }
          l++;
        }
      }
    }
  numberOfCluster = l;
  if (l > 0)
    for (int i = 0; i < numTrees; i++)
      cluTree[i]->Fill();
  if (l > 0)
    numCluTree->Fill();
}

void AGenerateOutput::getMCTree(string mcFile)
{
  trackIn = new track[5];
  mcTree = new TTree("MCIn", "MCIn");
  for (int i = 0; i < 5; i++) {
    string s = "Track";
    s = s + string_number(i + 1);
    mcTree->Branch(s.data(), &trackIn[i],
                   "phi/D:theta:beta:ID/I:nEl:ss[17]/I:qdc[17]/F:tdc[17]/"
                   "F:vertex[3]/D:phiIn/D:thetaIn:betaIn:nElIn/I:ssIn[17]/"
                   "I:vertexIn[3]/D:momentum/D");
  }
  ifstream mcfile;
  mcfile.open(mcFile.data());
  int evtNr1, nTr1, ID1, IDfrom1, i, n;
  float v1, v2, v3, p1, p2, p3, e1, e2, e3, pe1, pe2, pe3;
  char li[100];
  momentum4D tmp;
  mcTree->Branch("eventNumber", &evtNr1, "eventNumber/I");
  while (mcfile) {
    mcfile >> evtNr1 >> nTr1;
    while (evtNr1 > mcTree->GetEntries()) {
      mcTree->Fill();
    }
    mcfile.getline(li, 100);
    for (int j = 0; j < nTr1; j++) {
      mcfile >> i >> ID1 >> v1 >> v2 >> v3 >> p1 >> p2 >> p3 >> e1 >> e2 >> e3 >> pe1 >> pe2 >>
          pe3 >> IDfrom1;
      n = -1;
      switch (IDfrom1) {
      case -1: {
        if (ID1 == 14)
          n = 0;
        else if (ID1 == 11)
          n = 1;
        else if (ID1 == 18)
          n = 2;
        break;
      }
      case 2: {
        if (ID1 == 9)
          n = 4;
        else if (ID1 == 14)
          n = 3;
        break;
      }
      }
      tmp.setPM(vector3D(p1, p2, p3), Eparticles::getMassG(ID1));
      trackIn[n].phi = tmp.Momentum().Phi();
      trackIn[n].theta = tmp.Momentum().Theta();
      trackIn[n].beta = tmp.Velocity().length();
      trackIn[n].ID = ID1;
      trackIn[n].vertex[0] = v1;
      trackIn[n].vertex[1] = v2;
      trackIn[n].vertex[2] = v3;
      trackIn[n].momentum = sqrt(p1 * p1 + p2 * p2 + p3 * p3);
    }
    mcTree->Fill();
  }
  mcfile.close();
  for (int i = 0; i < 5; i++) {
    string s = "Track";
    s = s + string_number(i + 1);
    mcTree->SetBranchStatus(s.data(), 1);
    mcTree->SetBranchAddress(s.data(), &trackIn[i]);
  }
  delete[] trackIn;
}
void AGenerateOutput::assignTrackTree()
{
  if (!(writeTracks))
    return;
}
void AGenerateOutput::assignPixelTree()
{
  if (!(writePixel))
    return;
}
void AGenerateOutput::assignClusterTree()
{
  if (!(writeCluster))
    return;
}
void AGenerateOutput::assignHitTree()
{
  if (!(writeHits))
    return;
}
#include "Eparticles.h"
void AGenerateOutput::write_Reactions()
{
  if (event.getNumberOfReactions() == 0)
    return;
  RbaseReaction* rea;
  rea = event.getReaction(0);
  if (rea == NULL)
    return;
  int type = -1;
  for (int i = 0; i < numReact; i++) {
    if (reactions[i] == NULL)
      continue;
    if (rea->name() == reactions[i]->name())
      type = i;
  }
  if (type == -1)
    return;
  int which = -1;
  int beg = 0;
  TTrack* tr = NULL;
  numTr = rea->getNumberOfSecondaries();
  TCalibHit* tmpHit;
  //  cout<<event.getEventNumber()<<endl;
  for (int i = 0; i < rea->getNumberOfSecondaries(); i++) {
    if (i == 5) {
      beg = 5;
      which++;
    }
    if (i == 10) {
      beg = 10;
      which++;
    }
    if ((i % 20) == 0) {
      beg = i;
      which++;
    }
    tr = rea->getTrack(i);
    //      cout<<*tr<<endl;
    if (tr == NULL)
      return;
    chisquared = rea->getChiSquared();
    Track[which][i - beg].phi = tr->getPath().Direction().Phi();
    Track[which][i - beg].theta = tr->getPath().Direction().Theta();
    Track[which][i - beg].phiIn = rea->getParticle(i).Momentum().Phi();
    Track[which][i - beg].thetaIn = rea->getParticle(i).Momentum().Theta();
    //      if(tr->getSpeed()<=0)
    Track[which][i - beg].betaIn = rea->getParticle(i).Beta();
    //      else
    Track[which][i - beg].beta = tr->getSpeed();
    if (Track[which][i - beg].beta <= 0)
      Track[which][i - beg].beta = rea->getParticle(i).Beta();
    //      cout<<rea->getParticle(i).Momentum().R()<<endl;
    Track[which][i - beg].momentum = rea->getParticle(i).Momentum().R();
    Track[which][i - beg].vertex[0] = tr->getPath().Foot().X();
    Track[which][i - beg].vertex[1] = tr->getPath().Foot().Y();
    Track[which][i - beg].vertex[2] = tr->getPath().Foot().Z();
    Track[which][i - beg].nEl = tr->getNumberOfCalibHits();
    //      cout<<i<<" "<<tr->getNumberOfCalibHits()<<endl;
    for (int j = 0; j < event.getMaxNumber<TDetector>(); j++)
      if ((j < 24 && j > 14) || (j < 8 && j >= 0)) {
        if (tr->getNumberOfCalibHits(j) > 0) {
          tmpHit = tr->getCalibSrcr(j, 0);
          if (tmpHit != NULL) {
            Track[which][i - beg].ss[(j <= 7 ? j : j - 7)] = tmpHit->getElement();
            Track[which][i - beg].qdc[(j <= 7 ? j : j - 7)] = tmpHit->getADC();
            Track[which][i - beg].tdc[(j <= 7 ? j : j - 7)] = tmpHit->getRawTDC();
          } else {
            Track[which][i - beg].ss[(j <= 7 ? j : j - 7)] = -1;
            Track[which][i - beg].qdc[(j <= 7 ? j : j - 7)] = -1;
            Track[which][i - beg].tdc[(j <= 7 ? j : j - 7)] = -1;
          }
        } else {
          Track[which][i - beg].ss[(j <= 7 ? j : j - 7)] = -1;
          Track[which][i - beg].qdc[(j <= 7 ? j : j - 7)] = -1;
          Track[which][i - beg].tdc[(j <= 7 ? j : j - 7)] = -1;
        }
      }
    Track[which][i - beg].ID = reactIDs[type][i];
  }
  reactTree[type]->Fill();
  return;
}
void AGenerateOutput::writeLambda() {}
void AGenerateOutput::expandTree(int which, int upto)
{
  //  cout<<"expandTree(which="<<which<<", upto="<<upto<<")"<<endl;
  int numtrees = getNumTrees(upto, which);
  switch (which) {
  case 0: {
    hitFile->cd();
    if (numtrees <= numH)
      return;
    numtrees = numtrees - numH;
    for (int i = 0; i < numtrees; i++) {
      TTree* tmpTree = new TTree((string("Hits") + string_number(numH + 1)).data(),
                                 (string("Hits") + string_number(numH + 1)).data());
      tmpTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
      tmpTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
      hit* tmpHit = new hit[20];
      for (int j = 0; j < 20; j++) {
        tmpTree->Branch((string("Hit") + string_number(numH * 20 + j + 1)).data(), &tmpHit[j],
                        "qdcR/I:tdcR/I:qdcC/D:tdcC/D:el/I:det/I");
      }
      hitTree[numH] = tmpTree;
      Hits[numH] = tmpHit;
      numH++;
    }
    break;
  }
  case 1: {
    pixFile->cd();
    numtrees = numtrees - numP;
    for (int i = 0; i < numtrees; i++) {
      TTree* tmpTree = new TTree((string("Pixels") + string_number(numP + 1)).data(),
                                 (string("Pixels") + string_number(numP + 1)).data());
      tmpTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
      tmpTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
      pixel* tmpPix = new pixel[20];
      for (int j = 0; j < 20; j++) {
        tmpTree->Branch((string("Pixel") + string_number(numP * 20 + j + 1)).data(), &tmpPix[j],
                        "ID/I:numberOfElements/I:elementNumber[10]/I:center[3]/D");
      }
      pixTree[numP] = tmpTree;
      Pix[numP] = tmpPix;
      numP++;
    }
    break;
  }
  case 2: {
    cluFile->cd();
    numtrees = numtrees - numC;
    for (int i = 0; i < numtrees; i++) {
      TTree* tmpTree = new TTree((string("Cluster") + string_number(numC + 1)).data(),
                                 (string("Cluster") + string_number(numC + 1)).data());
      tmpTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
      tmpTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
      cluster* tmpClu = new cluster[20];
      for (int j = 0; j < 20; j++) {
        tmpTree->Branch((string("Cluster") + string_number(numC * 20 + j + 1)).data(), &tmpClu[j],
                        "ID/I:numberOfPixels/I:pixelNumber[10]/I:center[3]/D");
      }
      cluTree[numC] = tmpTree;
      Clus[numC] = tmpClu;
      numC++;
    }
    break;
  }
  case 3: {
    if (trackFile != NULL)
      trackFile->cd();
    int beg = 0;
    int accnum = 20;
    if (numT == 0)
      accnum = 5;
    else if (numT == 1) {
      accnum = 5;
      beg = 5;
    } else if (numT == 2) {
      beg = 10;
      accnum = 10;
    } else if (numT == 3)
      beg = 20;
    else if (numT >= 4)
      beg = (numT - 2) * 20;

    while (numT < numtrees) {
      TTree* tmpTree = new TTree((string("Track") + string_number(numT + 1)).data(),
                                 (string("Track") + string_number(numT + 1)).data());
      tmpTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
      tmpTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
      track1* tmpTrack = new track1[accnum];
      Int_t* tmpF = new Int_t[accnum];
      Int_t** tmpHits = new Int_t*[accnum];
      tmpTree->Branch("fromID", tmpF, (string("fromID[") + string_number(accnum) + "]/I").data());
      for (int j = 0; j < accnum; j++) {
        tmpHits[j] = new Int_t[30];
        //		tmpTree->Branch((string("Track")+string_number(beg+j+1)).data(),&tmpTrack[j],"phi/D:theta:beta:ID/I:nEl:ss[17]/I:qdc[17]/F:tdc[17]/F:vertex[3]/D:phiIn/D:thetaIn:betaIn:nElIn/I:ssIn[17]/I:vertexIn[3]/D:momentum/D");
        /*		tmpTree->Branch((string("Track")+string_number(beg+j+1)).data(),&tmpTrack[j],"ID/I:nEl:phi/D:theta:beta:momentum:phiIn:thetaIn:betaIn:nElIn/I:ss[17]/I:ssIn[17]/I:qdc[17]/F:tdc[17]/F:vertex[3]/D:vertexIn[3]/D");*/
        tmpTree->Branch(
            (string("Track") + string_number(beg + j + 1)).data(), &tmpTrack[j],
            "qdc[17]/F:tdc[17]/F:ss[17]/I:ssIn[17]/I:vertex[3]/"
            "D:vertexIn[3]/D:phi/"
            "D:theta:beta:momentum:phiIn:thetaIn:betaIn:ID/"
            "I:nEl:nElIn"); // ID/I:nEl:phi/D:theta:beta:momentum:phiIn:thetaIn:betaIn:nElIn/I:ss[17]/I:ssIn[17]/I:qdc[17]/F:tdc[17]/F:vertex[3]/D:vertexIn[3]/D");
        tmpTree->Branch((string("hits") + string_number(beg + j + 1)).data(), tmpHits[j],
                        (string("hits") + string_number(beg + j + 1) + "[30]/I").data());
      }
      trackTree[numT] = tmpTree;
      fromTrack[numT] = tmpF;
      //	    cout<<tmpTrack<<" "<<flush;
      Track[numT] = tmpTrack;
      //	    cout<<Track.back()<<endl;
      hitsOfTrack[numT] = tmpHits;
      numT++;
      if (numT == 1)
        beg = 5;
      else if (numT == 2) {
        beg = 10;
        accnum = 10;
      } else if (numT == 3) {
        beg = 20;
        accnum = 20;
      } else
        beg += 20;
    }
    // 	for(unsigned int i=0;i<(fromTrack.size())&&i<50;i++)
    // 	  fromSave[i]=fromTrack[i];
    // 	for(unsigned int i=0;i<(trackTree.size())&&i<50;i++)
    // 	  treesave[i]=trackTree[i];
    // 	for(unsigned int i=0;i<(hitsOfTrack.size())&&i<50;i++)
    // 	  saver[i]=hitsOfTrack[i];
    break;
  }
  }
}

int AGenerateOutput::getNumTrees(int numEntr, int which)
{
  if (numEntr == 0)
    return 0;
  switch (which) {
  case 0:
  case 1:
  case 2: {
    return (numEntr / 20) + 1;
  }
  case 3: {
    if (numEntr <= 5)
      return 1;
    if (numEntr <= 10)
      return 2;
    return (numEntr / 20) + 3;
  }
  }
  return 0;
}
void AGenerateOutput::setReactions(vector<string> reacts, string outName)
{
  numReact = reacts.size();
  reactions = new RbaseReaction*[maxtr];
  reactTree = new TTree*[maxtr];
  if (server) {
    TFile* f = new TFile((outName + "R.root").data(), "RECREATE");
    f->cd();
  }
  reactIDs = new int*[numReact];
  int j = 0;
  for (unsigned int i = 0; i < reacts.size(); i++) {
    //      cout<<"\""<<reacts[i].data()<<"\""<<endl;
    vector<int> tmpi;
    if (reacts[i] == "pp-elastic" || reacts[i] == "ppelastic") {
      reactions[j] = new Rppelastic(NULL, momentum4D(vector3D(0, 0, 1), Eparticles::getMassG(14)));
      reactIDs[j] = new int[2];
      reactIDs[j][0] = 14;
      reactIDs[j][1] = 14;
      cout << "write pp-elastic" << endl;
    } else if (reacts[i] == "pp-pkl-pkppi") {
      reactions[j] =
          new Rpp_pkl_pkppi(NULL, momentum4D(vector3D(0, 0, 1), Eparticles::getMassG(14)),
                            momentum4D(vector3D(0, 0, 0), Eparticles::getMassG(14)));
      reactIDs[j] = new int[5];
      reactIDs[j][0] = 14;
      reactIDs[j][1] = Eparticles::getGeantID("kaon+");
      reactIDs[j][2] = Eparticles::getGeantID("lambda");
      reactIDs[j][3] = 14;
      reactIDs[j][4] = 9;
      cout << "write p-K-lambda" << endl;
    } else if (reacts[i] == "dpi+" || reacts[i] == "pp-dpi+") {
      reactions[j] = new Rpp_dpiPlus(NULL, momentum4D(vector3D(0, 0, 1), Eparticles::getMassG(14)));
      reactIDs[j] = new int[2];
      reactIDs[j][0] = 45;
      reactIDs[j][1] = 7;
      cout << "write pp-dpi+" << endl;
    } else if (reacts[i] == "") {
      reactions[j] = NULL;
      continue;
    } else {
      reactions[j] = NULL;
      continue;
    }
    int numtrees = getNumTrees(reactions[j]->getNumberOfSecondaries(), 3);
    if (numtrees < numT)
      expandTree(3, reactions[j]->getNumberOfSecondaries());
    //      f->cd();
    TTree* tmpTree = new TTree(reacts[i].data(), reacts[i].data());
    tmpTree->Branch("EventNumber", &event.getEventNumber(), "EventNumber/I");
    tmpTree->Branch("RunNumber", &event.getRunNumber(), "RunNumber/I");
    tmpTree->Branch("Trigger", &event.getTrigger(), "Trigger/I");
    tmpTree->Branch("NumberOfTracks", &numTr, "NumberOfTracks/I");
    tmpTree->Branch("chi", &chisquared, "chi/F");
    int k = -1, b = 0;
    for (int jj = 0; jj < reactions[j]->getNumberOfSecondaries(); jj++) {
      if (jj == 5) {
        k++;
        b = 5;
      }
      if (jj == 10) {
        k++;
        b = jj;
      }
      if ((jj % 20) == 0) {
        k++;
        b = jj;
      }
      tmpTree->Branch((string("Track") + string_number(jj + 1)).data(), &Track[k][jj - b],
                      "qdc[17]/F:tdc[17]/F:ss[17]/I:ssIn[17]/I:vertex[3]/D:vertexIn[3]/"
                      "D:phi/"
                      "D:theta:beta:momentum:phiIn:thetaIn:betaIn:ID/I:nEl:nElIn");
      // tmpTree->Branch((string("Track")+string_number(jj+1)).data(),&Track[k][jj-b],"ID/I:nEl:phi/D:theta:beta:momentum:phiIn:thetaIn:betaIn:nElIn/I:ss[17]/I:ssIn[17]/I:qdc[17]/F:tdc[17]/F:vertex[3]/D:vertexIn[3]/D");
      //	  tmpTree->Branch((string("Track")+string_number(jj+1)).data(),&Track[k][jj-b],"phi/D:theta:beta:ID/I:nEl:ss[17]/I:qdc[17]/F:tdc[17]/F:vertex[3]/D:phiIn/D:thetaIn:betaIn:nElIn/I:ssIn[17]/I:vertexIn[3]/D:momentum/D");
    }
    reactTree[j] = tmpTree;
    j++;
  }
  numReact = j;
}
void AGenerateOutput::OnFinalizeTracks(vector<TTree*>* tTree, TTree* nTTree)
{
  int common = numT;
  if (common > (int)tTree->size())
    common = tTree->size();
  for (int i = 0; i < common; i++)
    trackTree[i]->CopyEntries((*tTree)[i]);
  numTrackTree->CopyEntries(nTTree);
  if ((int)tTree->size() > numT) {
    for (unsigned int i = common; i < tTree->size(); i++) {
      trackTree[numT] = (*tTree)[i]->CloneTree();
      numT++;
    }
  }
}
void AGenerateOutput::OnFinalizeCluster(vector<TTree*>* cTree, TTree* nCTree)
{
  int common = numC;
  if (common > (int)cTree->size())
    common = cTree->size();
  for (int i = 0; i < common; i++)
    cluTree[i]->CopyEntries((*cTree)[i]);
  numCluTree->CopyEntries(nCTree);
  if ((int)cTree->size() > numC) {
    for (unsigned int i = common; i < cTree->size(); i++) {
      cluTree[numC] = (*cTree)[i]->CloneTree();
      numC++;
    }
  }
}
void AGenerateOutput::OnFinalizePixel(vector<TTree*>* pTree, TTree* nPTree)
{
  int common = numP;
  if (common > (int)pTree->size())
    common = pTree->size();
  for (int i = 0; i < common; i++)
    pixTree[i]->CopyEntries((*pTree)[i]);
  numPixTree->CopyEntries(nPTree);
  if ((int)pTree->size() > numP) {
    for (unsigned int i = common; i < pTree->size(); i++) {
      pixTree[numP] = (*pTree)[i]->CloneTree();
      numP++;
    }
  }
}
void AGenerateOutput::OnFinalizeHits(vector<TTree*>* hTree, TTree* nHTree)
{
  int common = numH;
  if (common > (int)hTree->size())
    common = hTree->size();
  for (int i = 0; i < common; i++)
    hitTree[i]->CopyEntries((*hTree)[i]);
  numHitTree->CopyEntries(nHTree);
  if ((int)hTree->size() > numH) {
    for (unsigned int i = common; i < hTree->size(); i++) {
      hitTree[numH] = (*hTree)[i]->CloneTree();
      numH++;
    }
  }
}
void AGenerateOutput::OnFinalizeReactions(vector<TTree*>* rTree)
{
  int num;
  for (unsigned int i = 0; i < rTree->size(); i++) {
    num = -1;
    for (int j = 0; j < numReact; i++)
      if (string((*rTree)[i]->GetName()) == string(reactTree[i]->GetName()))
        num = j;
    if (num >= 0)
      reactTree[num]->CopyEntries((*rTree)[i]);
    else {
      reactTree[numReact] = (*rTree)[i];
      numReact++;
    }
  }
}
void AGenerateOutput::OnNewRun(run_parameter& r)
{
  if (!addFile)
    return;
  vector<string> files;
  vector<int> filetypes;
  for (size_t i = 0; i < r.getNumberOfFiles(); i++) {
    files.push_back(r.getFile(i));
    filetypes.push_back(r.getFileType(i));
  }
  bool done = true;
  if (writeHits) {
    if (exchangeFile) {
      for (unsigned int i = 0; i < files.size(); i++)
        if (filetypes[i] == 6 && done)
          files[i] = outfileName + "H.root";
    } else
      r.addFile(outfileName + "H.root", 6);
  }
  if (writePixel) {
    if (exchangeFile) {
      for (unsigned int i = 0; i < files.size(); i++)
        if (filetypes[i] == 10 && done)
          files[i] = outfileName + "P.root";
    } else
      r.addFile(outfileName + "P.root", 10);
  }
  if (writeCluster) {
    if (exchangeFile) {
      for (unsigned int i = 0; i < files.size(); i++)
        if (filetypes[i] == 18 && done)
          files[i] = outfileName + "C.root";
    } else
      r.addFile(outfileName + "C.root", 18);
  }
  if (writeTracks) {
    if (exchangeFile) {
      for (unsigned int i = 0; i < files.size(); i++)
        if (filetypes[i] == 34 && done)
          files[i] = outfileName + "T.root";
    } else
      r.addFile(outfileName + "T.root", 34);
  }
  if (writeReactions) {
    if (exchangeFile) {
      for (unsigned int i = 0; i < files.size(); i++)
        if (filetypes[i] == 66 && done)
          files[i] = outfileName + "R.root";
    } else
      r.addFile(outfileName + "R.root", 66);
  }
  if (exchangeFile) {
    r.clearFiles();
    for (unsigned int i = 0; i < files.size(); i++)
      r.addFile(files[i], filetypes[i]);
  }
}
