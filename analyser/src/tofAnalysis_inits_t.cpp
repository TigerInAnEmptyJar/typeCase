#include "tofAnalysis_t.h"

#include "ABetheBloch.h"
#include "Eparticles.h"
#include "logger.h"
#include "parameterio.h"
#include "shapeFactory.h"
#include "versions.h"

#include <QtCore/QDateTime>
void tofAnalysis::createSetup(vector<std::shared_ptr<detector_parameter>>& dets,
                              vector<std::shared_ptr<material_parameter>>& mats,
                              std::shared_ptr<reaction_parameter> col)
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
  parameter::IO::ParameterIO io;
  io.setup();
  auto materials = io.readParameter(materialFileName, parameter::IO::version_2,
                                    parameter::IO::ParameterIO::FileType::MATERIAL);
  auto detectors = io.readParameter(setupFileName, parameter::IO::version_2,
                                    parameter::IO::ParameterIO::FileType::DETECTOR);

  anaLog << "read setup from file \"" << setupFileName << "\" " << detectors.size() << endli;
  anaLog << "read materials from file \"" << materialFileName << "\" " << materials.size() << endli;

  vector<std::shared_ptr<material_parameter>> mats;
  vector<std::shared_ptr<detector_parameter>> dets;
  std::transform(materials.begin(), materials.end(), std::back_inserter(mats), [](auto element) {
    return std::dynamic_pointer_cast<material_parameter>(element);
  });
  std::transform(
      detectors.begin(), detectors.end(), std::back_inserter(dets), [&materials](auto element) {
        auto detector = std::dynamic_pointer_cast<detector_parameter>(element);
        auto material = std::find_if(
            materials.begin(), materials.end(), [id = detector->getMaterialId()](auto item) {
              if (auto material = std::dynamic_pointer_cast<material_parameter>(item)) {
                return id == material->id();
              }
              return false;
            });
        if (material != materials.end()) {
          detector->setMaterial(std::dynamic_pointer_cast<material_parameter>(*material).get());
        }

        return detector;
      });

  defineMaterials(mats, dets);
}

void tofAnalysis::defineMaterials(vector<std::shared_ptr<material_parameter>>& mats,
                                  vector<std::shared_ptr<detector_parameter>>& dets)
{
  anaLog << "define materials" << endli;
  vector<int> numM;
  bool isIn;
  int n;
  for (unsigned int i = 0; i < dets.size(); i++) {
    isIn = false;
    n = dets[i]->getMaterial();
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
    Vector parameter(2 + mats[i]->NumberOfElements() * 3);
    parameter.setValue(1, mats[i /*numM[i]*/]->RadiationLength());
    parameter.setValue(0, mats[i /*numM[i]*/]->Density());
    Materials[i] = new TMaterial(
        mats[i /*numM[i]*/]->getName(), mats[i /*numM[i]*/]->NumberOfElements(),
        mats[i /*numM[i]*/]->Element(0).getMass(), mats[i /*numM[i]*/]->Element(0).getCharge(),
        mats[i /*numM[i]*/]->Element(0).getWeight());
    parameter.setValue(2, mats[i /*numM[i]*/]->Element(0).getMass());
    parameter.setValue(3, mats[i /*numM[i]*/]->Element(0).getCharge());
    parameter.setValue(4, Materials[i]->getWeight(0));
    for (int j = 1; j < mats[i /*numM[i]*/]->NumberOfElements(); j++) {
      Materials[i]->addElement(mats[i /*numM[i]*/]->Element(j).getMass(),
                               mats[i /*numM[i]*/]->Element(j).getCharge(),
                               mats[i /*numM[i]*/]->Element(j).getWeight());
      parameter.setValue(j * 3 + 2, mats[i /*numM[i]*/]->Element(j).getMass());
      parameter.setValue(j * 3 + 3, mats[i /*numM[i]*/]->Element(j).getCharge());
      parameter.setValue(j * 3 + 4, Materials[i]->getWeight(j));
    }
    Materials[i]->setRadiationLength(mats[i /*numM[i]*/]->RadiationLength());
    Materials[i]->setDensity(mats[i /*numM[i]*/]->Density());
    Materials[i]->setSpeedOfLight(mats[i /*numM[i]*/]->Speed());
    ABetheBloch* elossAlgo = new ABetheBloch();
    elossAlgo->setParameters(parameter);
    Materials[i]->setAlgorithm(elossAlgo);
  }
}

void tofAnalysis::defineDetectors(const string& fileName)
{
  parameter::IO::ParameterIO io;
  io.setup();
  auto detectors = io.readParameter(fileName, parameter::IO::version_2,
                                    parameter::IO::ParameterIO::FileType::DETECTOR);

  anaLog << "read setup from file \"" << fileName << "\" " << detectors.size() << endli;

  vector<std::shared_ptr<detector_parameter>> dets;
  std::transform(detectors.begin(), detectors.end(), std::back_inserter(dets), [](auto element) {
    auto detector = std::dynamic_pointer_cast<detector_parameter>(element);
    return detector;
  });
  defineDetectors(dets);
}

void tofAnalysis::defineDetectors(vector<std::shared_ptr<detector_parameter>>& dets)
{
  int maxID = 0;
  for (unsigned int i = 0; i < dets.size(); i++) {
    if (dets[i]->getID() > maxID)
      maxID = dets[i]->getID();
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
  auto& shapeFactory = ShapeFactory::getInstance();
  for (int i = 0; i <= maxID; i++) {
    n = 0;
    id = -1;
    for (unsigned int j = 0; j < dets.size(); j++) {
      if (i == dets[j]->getID())
        id = j;
    }
    if (id == -1) {
      Detectors[i] = new TDetector(/**Materials[n]*/ *((TMaterial*)0), dets[id]->getID());
      Setup.addDetector(*Detectors[i]);
      continue;
    }
    // 	for(int j=0;j<NumberOfMaterials;j++)
    // 	{
    // 	    if(dets[id].getMaterial()==j)
    // 		n=j;
    // 	}
    n = dets[id]->getMaterial();
    if (n > NumberOfMaterials)
      n = -1;
    anaLog << "new Detector " << i;
    if (n == -1) {
      Detectors[i] = new TDetector(/**Materials[n]*/ *((TMaterial*)0), dets[id]->getID());
    } else {
      Detectors[i] = new TDetector(*Materials[n], dets[id]->getID());
    }
    Detectors[i]->setNumberOfElements(dets[id]->getNumberOfElements());
    Detectors[i]->setStackType(dets[id]->getStackType());
    anaLog << "define detector shape " << dets[id]->getShape()->getName();
    auto s = dets[id]->getShape();
    auto shape = shapeFactory.createVolume(*dets[id]->getShape());
    shape->setMaxDistance(dets[id]->getMaxDistance());
    anaLog << "... setShape";
    Detectors[i]->setShapeFirstElement(shape);
    anaLog << "... add detector to Setup" << endli;
    if (s->getName() == "wedge" || s->getName() == "spiral")
      Detectors[i]->setCircular();
    // Detectors[i]->setCircular(dets[id].isCircular());
    Setup.addDetector(*Detectors[i]);
  }
  anaLog << endli;
}

void tofAnalysis::defineReaction(std::shared_ptr<reaction_parameter> col)
{
  anaLog << "define beams and targets" << endli;
  TBeam b1;
  momentum4D mom;
  mom.setPM(vector3D(0, 0, col->getBeamMomentum(0)), Eparticles::getMass("proton"));
  b1.setParticle(mom);
  TBeam b2;
  mom.setPM(vector3D(0, 0, col->getBeamMomentum(1)), Eparticles::getMass("proton"));
  b2.setParticle(mom);
  shape_parameter s = col->getTargetShape();
  auto& shapeFactory = ShapeFactory::getInstance();
  auto sh1 = shapeFactory.createVolume(s);
  anaLog << "end define setup" << endli;
  isInitS = true;
  mom.setPM(vector3D(0, 0, 0), Eparticles::getMass("proton"));
  TTarget t(mom, sh1);
  Setup.setNumberOfBeams(((col->hasTwoBeams()) ? 2 : 1));
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
