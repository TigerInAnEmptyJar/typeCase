#pragma once

#include "baseparameterwriter.h"

#include <memory>
#include <ostream>
#include <vector>

namespace parameter {
namespace IO {

class ParameterWriter0 : public BaseParameterWriter
{
public:
  void writeParameter(std::vector<std::shared_ptr<base_parameter>> const& items,
                      std::filesystem::path path, FileType filetype) const override;

  virtual void writeAlgorithms(std::ostream& output,
                               std::vector<std::shared_ptr<base_parameter>> const& items) const;
  virtual void writeBeamtimes(std::ostream& output,
                              std::vector<std::shared_ptr<base_parameter>> const& items) const;
  virtual void writeRuns(std::ostream& output,
                         std::vector<std::shared_ptr<base_parameter>> const& items) const;
  virtual void writeDetectors(std::ostream& output,
                              std::vector<std::shared_ptr<base_parameter>> const& items) const;
  virtual void writeMaterials(std::ostream& output,
                              std::vector<std::shared_ptr<base_parameter>> const& items) const;
  virtual void writeShapes(std::ostream& output,
                           std::vector<std::shared_ptr<base_parameter>> const& items) const;

  // from write_algorithm_parameter_0
  virtual void writeSingleAlgorithm(std::ostream& output,
                                    std::shared_ptr<base_parameter> const& item) const;
  // from ostream operator << (...)
  virtual void writeSingleBeamtime(std::ostream& output,
                                   std::shared_ptr<base_parameter> const& item) const;
  // from write_run_parameter_0
  virtual void writeSingleRun(std::ostream& output,
                              std::shared_ptr<base_parameter> const& item) const;
  // from ostream operator << (...)
  virtual void writeSingleReaction(std::ostream& output,
                                   std::shared_ptr<base_parameter> const& item) const;
  // from write_detector_parameter_0
  virtual void writeSingleDetector(std::ostream& output,
                                   std::shared_ptr<base_parameter> const& item) const;
  // from ostream operator << (...)
  virtual void writeSingleMaterial(std::ostream& output,
                                   std::shared_ptr<base_parameter> const& item) const;
  // from write_shape_parameter_0
  virtual void writeSingleShape(std::ostream& output,
                                std::shared_ptr<base_parameter> const& item) const;

  bool isFiletypeAvailable(FileType check) const override;

  std::string getVersionString(FileType filetype) const override;
};

} // namespace IO
} // namespace parameter
