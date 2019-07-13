#pragma once

#include <QString>

namespace Keys {
const QString beamtimes{"beamtimes"};
const QString runs{"runs"};
const QString detectors{"detectors"};
const QString materials{"materials"};
const QString algorithms{"algorithms"};
const QString shapes{"shapes"};
const QString reactions{"reactions"};

const QString beamtime{"BEAMTIME"};
const QString run{"RUN"};
const QString detector{"DETECTOR"};
const QString material{"MATERIAL"};
const QString algorithm{"ALGORITHM"};
const QString shape{"SHAPE"};
const QString reaction{"REACTION"};

const QString parameterType{"parameterType"};
const QString name{"base::name"};
const QString base_id{"base::id"};
const QString description{"base::description"};

const QString pType{"parameterType"};
const QString pName{"parameterName"};
const QString pValue{"parameterValue"};

const QString id_number{"id"};
// Algorithms
const QString level{"level"};
const QString category{"category"};
const QString parameter{"parameter"};
const QString use{"used"};
// Beamtimes
const QString runFile{"runFile"};
const QString setupFile{"setupFile"};
const QString calibrationFile{"calibrationFile"};
// Runs
const QString runType{"runType"};
const QString parent{"parent"};
const QString own{"ownCalibration"};
const QString runNumber{"runNumber"};
const QString run_files{"files"};
const QString run_fileType{"fileType"};
const QString run_events{"events"};
const QString run_name{"fileName"};
const QString start{"start"};
const QString stop{"stop"};
// Dates
const QString month{"month"};
const QString year{"year"};
const QString day{"day"};
const QString hour{"hour"};
const QString minute{"minute"};
const QString second{"second"};
// Detectors
const QString number_of_elements{"numberOfElements"};
const QString stack_type{"stackType"};
const QString detector_number{"detectorNumber"};
const QString is_circular{"isCircular"};
const QString max_distance{"maxDistance"};
const QString detector_shape{"shape"};
const QString material_id{"materialId"};
// Materials
const QString is_active{"isActive"};
const QString energyloss{"energyloss"};
const QString density{"density"};
const QString radiation_length{"radiationLength"};
const QString speed{"speedOfLight"};
const QString elements{"elements"};
const QString element_mass{"mass"};
const QString element_charge{"charge"};
const QString element_weight{"weight"};
// Reaction
const QString two_beams{"twoBeams"};
const QString target_material_id{"targetMaterialId"};
const QString beam_material_id{"beamMaterialId"};
const QString beam_momentum_1{"beamMomentum1"};
const QString beam_momentum_2{"beamMomentum2"};
const QString target_shape{"targetShape"};
// const QString {""};
} // namespace Keys
