#include "AVirtualMachine.h"   
#include "eLossAlgorithm.h"
#include "Eparticles.h"
#include <stdlib.h>
#include "algorithmparameter.h"
AVirtualMachine::AVirtualMachine(TTrack **tracksIn, TCalibHit ***hitsIn, TSetup &setupIn, int &numberOfTracksIn, int **numberOfHitsIn, const algorithm_parameter &descr): AAlgorithm("track particles through virtual detector"),numberOfTracks(numberOfTracksIn),setup(setupIn)
{
    tracks=tracksIn;
    hits=hitsIn;
    numberOfHits=numberOfHitsIn;
    numberOfDetectors=setup.getNumberOfDetectors();
    if(descr.getNumberOfParam<vector<int> >()>=2)
	numberOfDetectors=descr.getParam<vector<int> >(0).getData().size();
    detectorIDs=new int[numberOfDetectors];
    mode=new int[numberOfDetectors];
    maxHitPerTrack=new int[numberOfDetectors];
    promptThetaRange[0]=descr.getParam<float>(0).getData();
    promptThetaRange[1]=descr.getParam<float>(1).getData();
//mode bits:
//1: do suspect on any
//2: do suspect on primary
//4: do suspect on secondary
    if(descr.getNumberOfParam<vector<int> >()>2)
    {
	for(int i=0;i<numberOfDetectors;i++)
	{
	    detectorIDs[i]=descr.getParam<vector<int> >(0).getData().at(i);
	    if((int)descr.getParam<vector<int> >(1).getData().size()>i)
		mode[i]=descr.getParam<vector<int> >(1).getData().at(i);
	    if((int)descr.getParam<vector<int> >(2).getData().size()>i)
		maxHitPerTrack[i]=descr.getParam<vector<int> >(2).getData().at(i);
	}
    }
    else if(descr.getNumberOfParam<vector<int> >()>=2)
    {
	for(int i=0;i<numberOfDetectors;i++)
	{
	    detectorIDs[i]=descr.getParam<vector<int> >(0).getData().at(i);
	    maxHitPerTrack[i]=2;
	    if((int)descr.getParam<vector<int> >(1).getData().size()>i)
		mode[i]=descr.getParam<vector<int> >(1).getData().at(i);
	}
    }
    else if(descr.getNumberOfParam<vector<int> >()==1)
    {
	for(int i=0;i<numberOfDetectors;i++)
	{
	    detectorIDs[i]=descr.getParam<vector<int> >(0).getData().at(i);
	    maxHitPerTrack[i]=2;
	    mode[i]=1;
	}
    }
    else
    {
	for(int i=0;i<numberOfDetectors;i++)
	{
	    detectorIDs[i]=i;
	    maxHitPerTrack[i]=2;
	    mode[i]=1;
	}
    }
    if(descr.getNumberOfParam<vector<int> >()>=3)
      {
	for(unsigned int i=0;i<descr.getParam<vector<int> >(3).getData().size();i++)
	  particleID[i]=descr.getParam<vector<int> >(3).getData().at(i);
      }
    commonStart=false;
    if(descr.getNumberOfParam<bool>()>0)
	commonStart=descr.getParam<bool>(0).getData();
    if(descr.getNumberOfParam<int>()>0)
	startTDC=descr.getParam<int>(0).getData();
}
AVirtualMachine::~AVirtualMachine()
{
    delete []detectorIDs;
    delete []mode;
    delete []maxHitPerTrack;
}
vector3D AVirtualMachine::addHit(int detNum, int elNum, volumeShape* shape, const sLine3D &path,TTrack *source) 
{
  //cout<<"AVirtualMachine::addHit"<<endl;
  //cout<<detectorIDs[detNum]<<" "<<elNum<<" : "<<shape<<endl;
  if(shape==NULL)return vector3D();
    Vector parameter=shape->Hitting(source->getPath());
    vector3D dist(parameter[3],parameter[4],parameter[5]);
    point3D enter(parameter[0],parameter[1],parameter[2]);
    vector3D distance=enter-source->getPath().Foot();
    int detID=detectorIDs[detNum];
    //    cout<<dist.toString(_spheric_).ascii()<<endl;
    if(dist.R()<=0&&//shape->getMaxDistance()&&
       distance*path.Direction()>0)
    {
	TCalibHit* tmpHit=hits[detID][*(numberOfHits[detID])];
	tmpHit->getDetectorr()=detID;
	tmpHit->getElementr()=elNum;
	tmpHit->setHitShape(shape);
	tmpHit->setValid();
	*(numberOfHits[detID])+=1;
	source->addCalibSrc(tmpHit);
	//cout<<"AVirtualMachine::addHit :done"<<endl;
	return distance;
    }
  //cout<<"AVirtualMachine::addHit :done"<<endl;
    return vector3D();
}
#include "ABetheBloch.h"
#include "geometry.h"
float AVirtualMachine::calculateEloss(float pathlength,TMaterial *mat,int particleID, float beta)
{
  //cout<<"AVirtualMachine::calculateEloss()"<<endl;
//boooha !! implement energyloss algorithm first!!!
  AELossAlgorithm *algo=mat->getAlgorithm();
  if(algo!=NULL)
    {
      //cout<<"use own "<<algo->parameters()<<endl;
      //      cout<<pathlength<<" "<<particleID<<" "<<beta<<" "<<algo->energyLossB(particleID, beta, pathlength)<<endl;
      //cout<<"AVirtualMachine::calculateEloss() :done"<<endl;
      return algo->energyLossB(particleID, beta, pathlength);
    }
  ABetheBloch tmp;
  Vector parameter(4);
//   param.setValue(0,mat->getDensity());
//   param.setValue(1,mat->getRadiationLength());
  parameter.setValues(mat->getDensity(),mat->getRadiationLength(),mat->getMeanMass(),mat->getMeanCharge());
  tmp.setParameters(parameter);
  //  cout<<parameter<<endl;
  //cout<<"AVirtualMachine::calculateEloss() :done"<<endl;
  return tmp.energyLossB(particleID, beta, pathlength);
}

int AVirtualMachine::searchADetector(int detNum, int detID, vector3D &distance, sLine3D path,TTrack *source)
{
  //cout<<"AVirtualMachine::searchADetector()"<<endl;
    TDetector *detector=&setup.getDetectorr(detID);
    int ret,backside,begin,end;
//     int pos=i*numberOfDetectors+j;
    Vector parameter(10);
    vector3D dist;
    point3D enter;
    //cout<<source->getPath()<<endl;
//     TCalibHit*tmpHit;
    distance=vector3D();
    if(detector->getNumberOfElements()<=0)
	      {
		//cout<<"AVirtualMachine::searchADetector():done"<<endl;
		return -1;
	      }
    volumeShape *detectorShape=detector->getShape(0);
    if((mode[detNum]&1)==1||((mode[detNum]&2)==2&&!source->isSecondary())||((mode[detNum]&4)==4&&source->isSecondary()))
    {
      ret=detectorShape->suspect(path,detector->getStackType());
      //cout<<ret<<path<<endl;
	if(ret<=-1||ret>=detector->getNumberOfElements())
	      {
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
  return -1;}
	//cout<<1<<endl;
	distance=addHit(detNum, ret, detector->getShape(ret), path,source);
	backside=detectorShape->suspect(sLine3D(path.Foot(),path.Direction()*-1),detector->getStackType());
	if(backside>detector->getNumberOfElements())backside=detector->getNumberOfElements()-1;
	if(backside==ret||
	   backside>detector->getNumberOfElements()||
	   backside<0)
	      {
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
  return ret;}
	if(abs(backside-ret)>detector->getNumberOfElements()/2.
	   &&!detector->isCircular())
	      {
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
  return ret;}
	if(abs(backside-ret)>detector->getNumberOfElements()/2.)
	{
	    if(abs(abs(backside-ret)-detector->getNumberOfElements())>maxHitPerTrack[detNum])
	      {
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
  return ret;}
	    if(backside<ret)
	    {
		begin=ret;
		end=backside;
	    }
	    else
	    {
		begin=backside;
		end=ret+1;
	    }
	    //cout<<2<<endl;
	    for(int k=0;k<begin;k++)
		dist=addHit(detNum, k, detector->getShape(k), path,source);
	    //cout<<3<<endl;
	    for(int k=end;k<detector->getNumberOfElements();k++)
		dist=addHit(detNum, k, detector->getShape(k), path,source);
	}
	else
	{
	    if(abs(backside-ret)>maxHitPerTrack[detNum])
	      {
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
		return ret;
	      }
	    if(backside>ret)
	    {
		begin=ret+1;
		end=backside;
	    }
	    else
	    {
		begin=backside;
		end=ret-1;
	    }
	    //cout<<3<<endl;
	    for(int k=begin;k<=end;k++)
		dist=addHit(detNum, k, detector->getShape(k), path,source);
	}
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
	return ret;
    }
    else
    {
    }
  //cout<<"AVirtualMachine::searchADetector():done"<<endl;
    return -1;
}

void *AVirtualMachine::process(void *ptr)
{
  //cout<<"*AVirtualMachine::process()"<<endl;
  //  cout<<"AVirtualMachine::process(void *ptr)"<<endl;
    int hit[numberOfTracks*numberOfDetectors];
    vector3D distanceToVertex[numberOfTracks*numberOfDetectors];
    int sorting[numberOfDetectors];
//    int last[numberOfTracks];
//    float timeToTrigger[numberOfTracks];
    bool used[numberOfDetectors];
    sLine3D path;
    int n;
    //float binning=1;
    vector3D direction;
//     cout<<numberOfTracks<<" tracks"<<endl;
    for(int i=0;i<numberOfTracks;i++)
    {
	path=tracks[i]->getPath();
 	//cout<<" "<<i<<path<<endl;
	direction=path.Direction();
//	last[i]=-1;
//	timeToTrigger[i]=-1;
	if(!tracks[i]->isSecondary()&&(promptThetaRange[0]>direction.Theta()||promptThetaRange[1]<direction.Theta()))
	  {
// 	    cout<<"outside defined range"<<endl;
	    continue;
	  }
	if(tracks[i]->getCharge()==0)
	  {
// 	    cout<<"no neutral tracks"<<endl;
	    continue;
	  }
	for(int j=0;j<numberOfDetectors;j++)//search detector elements hit by the track
	{
	  //	  cout<<"detector: "<<detectorIDs[j]<<" "<<flush;
	  hit[i*numberOfDetectors+j]=searchADetector(j, detectorIDs[j], distanceToVertex[i*numberOfDetectors+j], path,tracks[i]);
	  //	  cout<<hit[i*numberOfDetectors+j]<<endl;
	  used[j]=(hit[i*numberOfDetectors+j]>=0);
	  sorting[j]=-1;
	}
	n=0;
	for(int j=0;j<numberOfDetectors;j++)//sort hits on flight path
	{
	  //cout<<detectorIDs[j]<<" "<<used[j]<<distanceToVertex[i*numberOfDetectors+j].toString(_spheric_).ascii()<<endl;
	    int num=j;
	    for(int k=0;k<numberOfDetectors;k++)
	    {
	      if((distanceToVertex[i*numberOfDetectors+num].R()>distanceToVertex[i*numberOfDetectors+k].R()&&used[k])||!used[num])
		    num=k;
	    }
	    //cout<<j<<" "<<num<<" "<<used[num]<<endl;
	    if(!used[num])break;
	    used[num]=false;
	    sorting[j]=num;
	    n++;
	}
	//cout<<" "<<n<<" hits found"<<endl;
	if(n==0)continue;
//tdc trigger
	if(commonStart)
	{
//	    last[i]=sorting[0];
//	    timeToTrigger[i]=distanceToVertex[i*numberOfDetectors+sorting[0]].R()/tracks[i]->getSpeed()*299.792;
	}
	//else
	    //last[i]=sorting[n-1];
	TCalibHit*tmpHit;
// 	cout<<n<<" "<<tracks[i]->getNumberOfCalibHits()<<endl;
	float tmpF,timing=0,dt;
	float beta=tracks[i]->getSpeed(),mass=Eparticles::getMassG(particleID[i]);
	float gamma=1/sqrt(1-beta*beta);
	float energy=mass*gamma;
	vector3D last=tracks[i]->getPath().Foot();
	for(int j=0;j<n;j++)//calculate energy loss in detector
	{
	  if(distanceToVertex[i*numberOfDetectors+sorting[j]].R()<=0)continue;
	  //cout<<detectorIDs[sorting[j]]<<" "<<distanceToVertex[i*numberOfDetectors+sorting[j]].toString(_spheric_).ascii()<<endl;
	  int nEl=tracks[i]->getNumberOfCalibHits(detectorIDs[sorting[j]]);
	  float paths[20];
	  tmpF=0;dt=0;
	  timing+=(last-distanceToVertex[i*numberOfDetectors+sorting[j]]).R()/beta/299.792;
	  for(int k=0;k<nEl;k++)
	    {
	      tmpHit=tracks[i]->getCalibSrcr(detectorIDs[sorting[j]],k);
	      paths[k]=tmpHit->getHitShape()->getFlightPathInShape(path);
	      //	      if(paths[k]<binning)
	      tmpHit->setADC(calculateEloss(paths[k],setup.getDetectorr(detectorIDs[sorting[j]]).getMaterial(),particleID[i]/*tracks[i]->getID()*/, beta));
	      tmpHit->setTDC(timing+dt);
	      tmpF+=tmpHit->getADC();
	      dt+=paths[k]/beta/299.72;
	      //	      cout<<tmpF<<" "<<paths[k]<<" "<<*tmpHit<<endl;
	    }
	  last=distanceToVertex[i*numberOfDetectors+sorting[j]];
	  gamma=(energy-tmpF)/mass;
	  beta=sqrt(-(1/(gamma*gamma)-1));
	  //	  cout<<"beta new "<<beta<<endl;
	}
    }
     //cout<<"hits found:"<<endl;
    //for(int j=0;j<numberOfDetectors;j++)
    //{
	 //cout<<detectorIDs[j]<<" : "<<*numberOfHits[detectorIDs[j]]<<flush;
	 //for(int k=0;k<(*numberOfHits[detectorIDs[j]]);k++)
	   //cout<<" "<<hits[detectorIDs[j]][k]->getElement()<<flush;
	 //cout<<endl;
    //}
  //cout<<"*AVirtualMachine::process():done"<<endl;
    return ptr;
}
//#include "algorithmparameter.h"
algorithm_parameter AVirtualMachine::getDescription()
{
    algorithm_parameter ret("Virtual detector tracking",0,0);
    vector<string> des;
//     des.push_back("This algorithm uses the definition of the setup to create");
//     des.push_back("a virtual detector and track the particles, generated ");
//     des.push_back("elsewhere, generating their signal in the detector.");
//     des.push_back("Hits are generated including timing and energy information.");
//     des.push_back("The TDC is the time, the signal leaves the detector.");
//     des.push_back("Energy loss will slow the particles down but it won\'t");
//     des,push_back("change the particles direction, since it is only calculated");
//     des.push_back("by simple energy-loss-calculations.");
//     des.push_back("Only straight tracks are assumed, no magnetic field will be");
//     des.push_back("taken into account. Therefor, the stepwidth outside a ");
//     des.push_back("detector area is from detector to detector element and ");
//     des.push_back("inside the material 1mm.");
//     des.push_back("Some detectors share the same volume, here the energy will");
//     des.push_back("be distributed according to the path to the read-out.");
//     des.push_back("Noise and smearing of the signals will be implemented.");
    ret.setDescription(des);
    ret.setCategory(1);
    ret.addParam<bool>(single_parameter<bool>("Use common Start",false));
    ret.addParam<int>(single_parameter<int>("TDC at trigger",200));
    vector<int>tmp;
    ret.addParam<vector<int> >(single_parameter<vector<int> >("use detectors",tmp));
    ret.addParam<vector<int> >(single_parameter<vector<int> >("detector mode",tmp));
    ret.addParam<vector<int> >(single_parameter<vector<int> >("max el. per det and passing particle",tmp));
    ret.addParam<vector<int> >(single_parameter<vector<int> >("particle IDs of tracks",tmp));
    ret.addParam<float>(single_parameter<float>("minimum theta of detector",0));
    ret.addParam<float>(single_parameter<float>("maximum theta of detector",M_PI));
    return ret;
}