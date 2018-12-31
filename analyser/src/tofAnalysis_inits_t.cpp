#include "ABetheBloch.h"
#include "Eparticles.h"
#include "logger.h"
#include "parameterManager.h"
#include "tofAnalysis_t.h"
#include <QtCore/QDateTime>
void tofAnalysis::createSetup(vector<detector_parameter>& dets, vector<material_parameter>& mats,
                              reaction_parameter col)
{
  if (!Eparticles::IsInit())
    Eparticles::init();
  anaLog << "init setup" << incD << endli;
  if (isInitS)
    killSetup();
  defineMaterials(mats, dets);
  defineDetectors(dets);
  defineReaction(col);
  isInitS = true;
  return;
  while (Setup.getNumberOfDetectors() > 0)
    Setup.popDetector();
  /***Beam und Target ***********************************************/
}

void tofAnalysis::defineMaterials(const string& setupFileName, const string& materialFileName)
{
  reaction_parameter col;
  vector<detector_parameter> dparams;
  ioLog << "read setup from file " << setupFileName << endli << incD;
  //     ifstream input;
  //     ioLog<<"open file"<<endli;
  anaLog << "read setup from file \"" << setupFileName << "\" "
         << (parameterManager::readDetectorParameter_ascii(setupFileName, dparams, col)
                 ? "successfull"
                 : "failed")
         << endli;
  //     input.open(setupFileName.data());
  //     int nDetectors;
  //     ioLog<<"read beam(s) and target"<<endli;
  //     input >> col;
  //     input >> nDetectors;
  //     ioLog<< "read detector parameter";
  //     for(int i=0;i<nDetectors;i++)
  //     {
  // 	ioLog<<"#";
  // 	detector_parameter d;
  // 	input >> d;
  // 	dparams.push_back(d);
  //     }
  //     ioLog<<endli;
  //     ioLog<<"close file"<<endli;
  //     input.close();
  //     ioLog<<decD;
  vector<material_parameter> mparams;
  anaLog << "read materials from file \"" << materialFileName << "\" "
         << (parameterManager::readMaterialParameter_ascii(materialFileName, mparams)
                 ? "successfull"
                 : "failed")
         << endli;
  //     int nMats;
  //     ioLog<<"read materials from file "<<materialFileName<<endli<<incD;
  //     ifstream input1;
  //     ioLog<<"open file"<<endli;
  //     input.open(materialFileName.data());
  //     input1 >> nMats;
  //     ioLog<< "read material parameter: ";
  //     for(int i=0;i<nMats;i++)
  //     {
  // 	ioLog<<"#";
  // 	material_parameter m;
  // 	input1 >> m;
  // 	mparams.push_back(m);
  //     }
  //     ioLog<<endli;
  //     ioLog<<"close file"<<endli;
  //     input1.close();
  //    ioLog<<decD;

  defineMaterials(mparams, dparams);
}

void tofAnalysis::defineMaterials(vector<material_parameter>& mats,
                                  vector<detector_parameter>& dets)
{
  anaLog << "define materials" << endli;
  vector<int> numM;
  bool isIn;
  int n;
  for (unsigned int i = 0; i < dets.size(); i++) {
    isIn = false;
    n = dets[i].getMaterial();
    for (int j = 0; j < (int)numM.size(); j++) {
      if (numM[j] == n) {
        isIn = true;
        break;
      }
    }
    if (isIn)
      continue;
    numM.push_back(n);
  }
  NumberOfMaterials = mats /*numM*/.size();
  Materials = new TMaterial*[mats.size() /*Setup.getMaxDetectors()*/];
  for (int i = 0; i < (int)mats /*numM*/.size(); i++) {
    // 	if(numM[i]<0||numM[i]>=(int)mats.size())
    // 	{
    // 	    Materials[i]=new TMaterial("none", 1,0,0,0);
    // 	    continue;
    // 	}
    Vector parameter(2 + mats[i].NumberOfElements() * 3);
    parameter.setValue(1, mats[i /*numM[i]*/].RadiationLength());
    parameter.setValue(0, mats[i /*numM[i]*/].Density());
    Materials[i] = new TMaterial(
        mats[i /*numM[i]*/].getName(), mats[i /*numM[i]*/].NumberOfElements(),
        mats[i /*numM[i]*/].Element(0).getMass(), mats[i /*numM[i]*/].Element(0).getCharge(),
        mats[i /*numM[i]*/].Element(0).getWeight());
    parameter.setValue(2, mats[i /*numM[i]*/].Element(0).getMass());
    parameter.setValue(3, mats[i /*numM[i]*/].Element(0).getCharge());
    parameter.setValue(4, Materials[i]->getWeight(0));
    for (int j = 1; j < mats[i /*numM[i]*/].NumberOfElements(); j++) {
      Materials[i]->addElement(mats[i /*numM[i]*/].Element(j).getMass(),
                               mats[i /*numM[i]*/].Element(j).getCharge(),
                               mats[i /*numM[i]*/].Element(j).getWeight());
      parameter.setValue(j * 3 + 2, mats[i /*numM[i]*/].Element(j).getMass());
      parameter.setValue(j * 3 + 3, mats[i /*numM[i]*/].Element(j).getCharge());
      parameter.setValue(j * 3 + 4, Materials[i]->getWeight(j));
    }
    Materials[i]->setRadiationLength(mats[i /*numM[i]*/].RadiationLength());
    Materials[i]->setDensity(mats[i /*numM[i]*/].Density());
    Materials[i]->setSpeedOfLight(mats[i /*numM[i]*/].Speed());
    ABetheBloch* elossAlgo = new ABetheBloch();
    elossAlgo->setParameters(parameter);
    Materials[i]->setAlgorithm(elossAlgo);
  }
}

void tofAnalysis::defineDetectors(const string& fileName)
{
  reaction_parameter col;
  vector<detector_parameter> dparams;
  anaLog << "read setup from file " << fileName << "\":"
         << (parameterManager::readDetectorParameter_ascii(fileName, dparams, col) ? "successfull"
                                                                                   : "failed")
         << endli << incD;
  //     ifstream input;
  //     ioLog<<"open file"<<endli;
  //     input.open(fileName.data());
  //     int nDetectors;
  //     ioLog<<"read beam(s) and target"<<endli;
  //     input >> col;
  //     input >> nDetectors;
  //     ioLog<< "read detector parameter";
  //     for(int i=0;i<nDetectors;i++)
  //     {
  // 	ioLog<<"#";
  // 	detector_parameter d;
  // 	input >> d;
  // 	dparams.push_back(d);
  //     }
  //     ioLog<<endli;
  //     ioLog<<"close file"<<endli;
  //     input.close();
  //     ioLog<<decD;
  defineDetectors(dparams);
  //    defineReaction(col);
}

void tofAnalysis::defineDetectors(vector<detector_parameter>& dets)
{
  int maxID = 0;
  for (unsigned int i = 0; i < dets.size(); i++) {
    if (dets[i].getID() > maxID)
      maxID = dets[i].getID();
  }
  if (Setup.getMaxDetectors() < maxID)
    NumberOfDetectors = Setup.getMaxDetectors();
  else
    NumberOfDetectors = maxID + 1;
  anaLog << "define detectors " << NumberOfDetectors << endli;
  Detectors = new TDetector*[NumberOfDetectors];
  for (int i = 0; i < NumberOfDetectors; i++)
    Detectors[i] = 0;
  int id;
  int n;
  for (int i = 0; i <= maxID; i++) {
    n = 0;
    id = -1;
    for (unsigned int j = 0; j < dets.size(); j++) {
      if (i == dets[j].getID())
        id = j;
    }
    if (id == -1) {
      Detectors[i] = new TDetector(/**Materials[n]*/ *((TMaterial*)0), dets[id].getID());
      Setup.addDetector(*Detectors[i]);
      continue;
    }
    // 	for(int j=0;j<NumberOfMaterials;j++)
    // 	{
    // 	    if(dets[id].getMaterial()==j)
    // 		n=j;
    // 	}
    n = dets[id].getMaterial();
    if (n > NumberOfMaterials)
      n = -1;
    anaLog << "new Detector " << i;
    if (n == -1) {
      Detectors[i] = new TDetector(/**Materials[n]*/ *((TMaterial*)0), dets[id].getID());
    } else {
      Detectors[i] = new TDetector(*Materials[n], dets[id].getID());
    }
    Detectors[i]->setNumberOfElements(dets[id].getNumberOfElements());
    Detectors[i]->setStackType(dets[id].getStackType());
    anaLog << "define detector shape " << dets[id].getShape().getName();
    shape_parameter s = dets[id].getShape();
    volumeShape* shape = tofAnalysis::getVShape(s);
    shape->setMaxDistance(dets[id].getMaxDistance());
    anaLog << "... setShape";
    Detectors[i]->setShapeFirstElement(shape);
    anaLog << "... add detector to Setup" << endli;
    if (s.getName() == "wedge" || s.getName() == "spiral")
      Detectors[i]->setCircular();
    // Detectors[i]->setCircular(dets[id].isCircular());
    Setup.addDetector(*Detectors[i]);
  }
  anaLog << endli;
}

void tofAnalysis::defineReaction(reaction_parameter col)
{
  anaLog << "define beams and targets" << endli;
  TBeam b1;
  momentum4D mom;
  mom.setPM(vector3D(0, 0, col.getBeamMomentum(0)), Eparticles::getMass("proton"));
  b1.setParticle(mom);
  TBeam b2;
  mom.setPM(vector3D(0, 0, col.getBeamMomentum(1)), Eparticles::getMass("proton"));
  b2.setParticle(mom);
  shape_parameter s = col.getTargetShape();
  volumeShape* sh1 = tofAnalysis::getVShape(s);
  anaLog << "end define setup" << endli;
  isInitS = true;
  mom.setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
  TTarget t(mom, *sh1);
  //    t.setShape(*sh1);
  Setup.setNumberOfBeams(((col.hasTwoBeams()) ? 2 : 1));
  Setup.setBeam(b1, 0);
  Setup.setBeam(b2, 1);
  Setup.setTarget(t);
}

void tofAnalysis::createSetup(const string& setupFileName, const string& materialFileName)
{
  defineMaterials(materialFileName, setupFileName);
  defineDetectors(setupFileName);
  isInitS = true;
}

void tofAnalysis::killSetup()
{
  if (isInitS) {
    for (int i = 0; i < NumberOfDetectors; i++)
      delete Detectors[i];
    delete[] Detectors;
    for (int i = 0; i < NumberOfMaterials; i++) {
      AELossAlgorithm* tmp = Materials[i]->getAlgorithm();
      delete tmp;
      delete Materials[i];
    }
    delete[] Materials;
  }
  isInitS = false;
}
