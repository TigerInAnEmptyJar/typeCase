#include "AVirtualMachine.h"
#include "Eparticles.h"
#include "algorithmparameter.h"
#include "eLossAlgorithm.h"
#include <stdlib.h>
AVirtualMachine::AVirtualMachine(TTrack** tracksIn, TCalibHit*** hitsIn, TSetup& setupIn,
                                 int& numberOfTracksIn, int** numberOfHitsIn,
                                 const algorithm_parameter& descr)
    : AAlgorithm("track particles through virtual detector"), numberOfTracks(numberOfTracksIn),
      setup(setupIn)
{
  tracks = tracksIn;
  hits = hitsIn;
  numberOfHits = numberOfHitsIn;
  numberOfDetectors = setup.getNumberOfDetectors();
  numberOfDetectors = descr.value(4).value<vector<int>>().size();
  detectorIDs = new int[numberOfDetectors];
  mode = new int[numberOfDetectors];
  maxHitPerTrack = new int[numberOfDetectors];
  promptThetaRange[0] = descr.value(2).value<float>();
  promptThetaRange[1] = descr.value(3).value<float>();
  // mode bits:
  // 1: do suspect on any
  // 2: do suspect on primary
  // 4: do suspect on secondary
  for (int i = 0; i < numberOfDetectors; i++) {
    detectorIDs[i] = descr.value(4).value<vector<int>>().at(i);
    if ((int)descr.value(5).value<vector<int>>().size() > i)
      mode[i] = descr.value(5).value<vector<int>>().at(i);
    if ((int)descr.value(6).value<vector<int>>().size() > i)
      maxHitPerTrack[i] = descr.value(6).value<vector<int>>().at(i);
  }
  for (unsigned int i = 0; i < descr.value(7).value<vector<int>>().size(); i++)
    particleID[i] = descr.value(7).value<vector<int>>().at(i);
  commonStart = false;
  commonStart = descr.value(0).value<bool>();
  startTDC = descr.value(1).value<int>();
}
AVirtualMachine::~AVirtualMachine()
{
  delete[] detectorIDs;
  delete[] mode;
  delete[] maxHitPerTrack;
}
vector3D AVirtualMachine::addHit(int detNum, int elNum, volumeShape* shape, const sLine3D& path,
                                 TTrack* source)
{
  // cout<<"AVirtualMachine::addHit"<<endl;
  // cout<<detectorIDs[detNum]<<" "<<elNum<<" : "<<shape<<endl;
  if (shape == NULL)
    return vector3D();
  Vector parameter = shape->Hitting(source->getPath());
  vector3D dist(parameter[3], parameter[4], parameter[5]);
  point3D enter(parameter[0], parameter[1], parameter[2]);
  vector3D distance = enter - source->getPath().Foot();
  int detID = detectorIDs[detNum];
  //    cout<<dist.toString(_spheric_).ascii()<<endl;
  if (dist.R() <= 0 && // shape->getMaxDistance()&&
      distance * path.Direction() > 0) {
    TCalibHit* tmpHit = hits[detID][*(numberOfHits[detID])];
    tmpHit->getDetectorr() = detID;
    tmpHit->getElementr() = elNum;
    tmpHit->setHitShape(shape);
    tmpHit->setValid();
    *(numberOfHits[detID]) += 1;
    source->addCalibSrc(tmpHit);
    // cout<<"AVirtualMachine::addHit :done"<<endl;
    return distance;
  }
  // cout<<"AVirtualMachine::addHit :done"<<endl;
  return vector3D();
}
#include "ABetheBloch.h"
#include "geometry.h"
float AVirtualMachine::calculateEloss(float pathlength, TMaterial* mat, int particleID, float beta)
{
  // cout<<"AVirtualMachine::calculateEloss()"<<endl;
  // boooha !! implement energyloss algorithm first!!!
  AELossAlgorithm* algo = mat->getAlgorithm();
  if (algo != NULL) {
    // cout<<"use own "<<algo->parameters()<<endl;
    //      cout<<pathlength<<" "<<particleID<<" "<<beta<<"
    //      "<<algo->energyLossB(particleID, beta,
    //      pathlength)<<endl;
    // cout<<"AVirtualMachine::calculateEloss() :done"<<endl;
    return algo->energyLossB(particleID, beta, pathlength);
  }
  ABetheBloch tmp;
  Vector parameter(4);
  //   param.setValue(0,mat->getDensity());
  //   param.setValue(1,mat->getRadiationLength());
  parameter.setValues(mat->getDensity(), mat->getRadiationLength(), mat->getMeanMass(),
                      mat->getMeanCharge());
  tmp.setParameters(parameter);
  //  cout<<parameter<<endl;
  // cout<<"AVirtualMachine::calculateEloss() :done"<<endl;
  return tmp.energyLossB(particleID, beta, pathlength);
}

int AVirtualMachine::searchADetector(int detNum, int detID, vector3D& distance, sLine3D path,
                                     TTrack* source)
{
  // cout<<"AVirtualMachine::searchADetector()"<<endl;
  TDetector* detector = &setup.getDetectorr(detID);
  int ret, backside, begin, end;
  //     int pos=i*numberOfDetectors+j;
  Vector parameter(10);
  vector3D dist;
  point3D enter;
  // cout<<source->getPath()<<endl;
  //     TCalibHit*tmpHit;
  distance = vector3D();
  if (detector->getNumberOfElements() <= 0) {
    // cout<<"AVirtualMachine::searchADetector():done"<<endl;
    return -1;
  }
  volumeShape* detectorShape = detector->getShape(0);
  if ((mode[detNum] & 1) == 1 || ((mode[detNum] & 2) == 2 && !source->isSecondary()) ||
      ((mode[detNum] & 4) == 4 && source->isSecondary())) {
    ret = detectorShape->suspect(path, detector->getStackType());
    // cout<<ret<<path<<endl;
    if (ret <= -1 || ret >= static_cast<int>(detector->getNumberOfElements())) {
      // cout<<"AVirtualMachine::searchADetector():done"<<endl;
      return -1;
    }
    // cout<<1<<endl;
    distance = addHit(detNum, ret, detector->getShape(ret), path, source);
    backside = detectorShape->suspect(sLine3D(path.Foot(), path.Direction() * -1),
                                      detector->getStackType());
    if (backside > static_cast<int>(detector->getNumberOfElements()))
      backside = detector->getNumberOfElements() - 1;
    if (backside == ret || backside > static_cast<int>(detector->getNumberOfElements()) ||
        backside < 0) {
      // cout<<"AVirtualMachine::searchADetector():done"<<endl;
      return ret;
    }
    if (abs(backside - ret) > detector->getNumberOfElements() / 2. && !detector->isCircular()) {
      // cout<<"AVirtualMachine::searchADetector():done"<<endl;
      return ret;
    }
    if (abs(backside - ret) > detector->getNumberOfElements() / 2.) {
      if (abs(static_cast<int>(abs(backside - ret) - detector->getNumberOfElements()) >
              maxHitPerTrack[detNum])) {
        // cout<<"AVirtualMachine::searchADetector():done"<<endl;
        return ret;
      }
      if (backside < ret) {
        begin = ret;
        end = backside;
      } else {
        begin = backside;
        end = ret + 1;
      }
      // cout<<2<<endl;
      for (int k = 0; k < begin; k++)
        dist = addHit(detNum, k, detector->getShape(k), path, source);
      // cout<<3<<endl;
      for (int k = end; k < static_cast<int>(detector->getNumberOfElements()); k++)
        dist = addHit(detNum, k, detector->getShape(k), path, source);
    } else {
      if (abs(backside - ret) > maxHitPerTrack[detNum]) {
        // cout<<"AVirtualMachine::searchADetector():done"<<endl;
        return ret;
      }
      if (backside > ret) {
        begin = ret + 1;
        end = backside;
      } else {
        begin = backside;
        end = ret - 1;
      }
      // cout<<3<<endl;
      for (int k = begin; k <= end; k++)
        dist = addHit(detNum, k, detector->getShape(k), path, source);
    }
    // cout<<"AVirtualMachine::searchADetector():done"<<endl;
    return ret;
  } else {
  }
  // cout<<"AVirtualMachine::searchADetector():done"<<endl;
  return -1;
}

void AVirtualMachine::process()
{
  // cout<<"*AVirtualMachine::process()"<<endl;
  //  cout<<"AVirtualMachine::process(void *ptr)"<<endl;
  int hit[numberOfTracks * numberOfDetectors];
  vector3D distanceToVertex[numberOfTracks * numberOfDetectors];
  int sorting[numberOfDetectors];
  //    int last[numberOfTracks];
  //    float timeToTrigger[numberOfTracks];
  bool used[numberOfDetectors];
  sLine3D path;
  int n;
  // float binning=1;
  vector3D direction;
  //     cout<<numberOfTracks<<" tracks"<<endl;
  for (int i = 0; i < numberOfTracks; i++) {
    path = tracks[i]->getPath();
    // cout<<" "<<i<<path<<endl;
    direction = path.Direction();
    //	last[i]=-1;
    //	timeToTrigger[i]=-1;
    if (!tracks[i]->isSecondary() &&
        (promptThetaRange[0] > direction.Theta() || promptThetaRange[1] < direction.Theta())) {
      // 	    cout<<"outside defined range"<<endl;
      continue;
    }
    if (tracks[i]->getCharge() == 0) {
      // 	    cout<<"no neutral tracks"<<endl;
      continue;
    }
    for (int j = 0; j < numberOfDetectors; j++) // search detector elements hit by the track
    {
      //	  cout<<"detector: "<<detectorIDs[j]<<" "<<flush;
      hit[i * numberOfDetectors + j] = searchADetector(
          j, detectorIDs[j], distanceToVertex[i * numberOfDetectors + j], path, tracks[i]);
      //	  cout<<hit[i*numberOfDetectors+j]<<endl;
      used[j] = (hit[i * numberOfDetectors + j] >= 0);
      sorting[j] = -1;
    }
    n = 0;
    for (int j = 0; j < numberOfDetectors; j++) // sort hits on flight path
    {
      // cout<<detectorIDs[j]<<"
      // "<<used[j]<<distanceToVertex[i*numberOfDetectors+j].toString(_spheric_).ascii()<<endl;
      int num = j;
      for (int k = 0; k < numberOfDetectors; k++) {
        if ((distanceToVertex[i * numberOfDetectors + num].R() >
                 distanceToVertex[i * numberOfDetectors + k].R() &&
             used[k]) ||
            !used[num])
          num = k;
      }
      // cout<<j<<" "<<num<<" "<<used[num]<<endl;
      if (!used[num])
        break;
      used[num] = false;
      sorting[j] = num;
      n++;
    }
    // cout<<" "<<n<<" hits found"<<endl;
    if (n == 0)
      continue;
    // tdc trigger
    if (commonStart) {
      //	    last[i]=sorting[0];
      //	    timeToTrigger[i]=distanceToVertex[i*numberOfDetectors+sorting[0]].R()/tracks[i]->getSpeed()*299.792;
    }
    // else
    // last[i]=sorting[n-1];
    TCalibHit* tmpHit;
    // 	cout<<n<<" "<<tracks[i]->getNumberOfCalibHits()<<endl;
    float tmpF, timing = 0, dt;
    float beta = tracks[i]->getSpeed(), mass = Eparticles::getMassG(particleID[i]);
    float gamma = 1 / sqrt(1 - beta * beta);
    float energy = mass * gamma;
    vector3D last = tracks[i]->getPath().Foot();
    for (int j = 0; j < n; j++) // calculate energy loss in detector
    {
      if (distanceToVertex[i * numberOfDetectors + sorting[j]].R() <= 0)
        continue;
      // cout<<detectorIDs[sorting[j]]<<"
      // "<<distanceToVertex[i*numberOfDetectors+sorting[j]].toString(_spheric_).ascii()<<endl;
      int nEl = tracks[i]->getNumberOfCalibHits(detectorIDs[sorting[j]]);
      float paths[20];
      tmpF = 0;
      dt = 0;
      timing += (last - distanceToVertex[i * numberOfDetectors + sorting[j]]).R() / beta / 299.792;
      for (int k = 0; k < nEl; k++) {
        tmpHit = tracks[i]->getCalibSrcr(detectorIDs[sorting[j]], k);
        paths[k] = tmpHit->getHitShape()->getFlightPathInShape(path);
        //	      if(paths[k]<binning)
        tmpHit->setADC(calculateEloss(paths[k],
                                      setup.getDetectorr(detectorIDs[sorting[j]]).getMaterial(),
                                      particleID[i] /*tracks[i]->getID()*/, beta));
        tmpHit->setTDC(timing + dt);
        tmpF += tmpHit->getADC();
        dt += paths[k] / beta / 299.72;
        //	      cout<<tmpF<<" "<<paths[k]<<" "<<*tmpHit<<endl;
      }
      last = distanceToVertex[i * numberOfDetectors + sorting[j]];
      gamma = (energy - tmpF) / mass;
      beta = sqrt(-(1 / (gamma * gamma) - 1));
      //	  cout<<"beta new "<<beta<<endl;
    }
  }
  // cout<<"hits found:"<<endl;
  // for(int j=0;j<numberOfDetectors;j++)
  //{
  // cout<<detectorIDs[j]<<" : "<<*numberOfHits[detectorIDs[j]]<<flush;
  // for(int k=0;k<(*numberOfHits[detectorIDs[j]]);k++)
  // cout<<" "<<hits[detectorIDs[j]][k]->getElement()<<flush;
  // cout<<endl;
  //}
  // cout<<"*AVirtualMachine::process():done"<<endl;
}
//#include "algorithmparameter.h"
algorithm_parameter AVirtualMachine::getDescription()
{
  algorithm_parameter ret("Virtual detector tracking", algorithm_parameter::Category::INPUT, 0);
  std::string des = "This algorithm uses the definition of the setup to create"
                    "a virtual detector and track the particles, generated"
                    "elsewhere, generating their signal in the detector."
                    "Hits are generated including timing and energy information."
                    "The TDC is the time, the signal leaves the detector."
                    "Energy loss will slow the particles down but it won\'t"
                    "change the particles direction, since it is only calculated"
                    "by simple energy-loss-calculations."
                    "Only straight tracks are assumed, no magnetic field will be"
                    "taken into account. Therefor, the stepwidth outside a"
                    "detector area is from detector to detector element and"
                    "inside the material 1mm."
                    "Some detectors share the same volume, here the energy will"
                    "be distributed according to the path to the read-out."
                    "Noise and smearing of the signals will be implemented.";
  ret.setDescription(des);
  ret.setCategory(algorithm_parameter::Category::INPUT);
  ret.addValue("Use common Start", false);
  ret.addValue("TDC at trigger", static_cast<int>(200));
  ret.addValue("minimum theta of detector", 0.f);
  ret.addValue("maximum theta of detector", static_cast<float>(M_PI));
  vector<int> tmp;
  ret.addValue("use detectors", tmp);
  ret.addValue("detector mode", tmp);
  ret.addValue("max el. per det and passing particle", tmp);
  ret.addValue("particle IDs of tracks", tmp);
  return ret;
}
