#pragma once

#include "baseparameterreader.h"

namespace parameter {
namespace IO {

class ParameterReader0 : public BaseParameterReader
{
public:
  std::vector<std::shared_ptr<base_parameter>> readParameter(std::filesystem::path path,
                                                             FileType filetype) const override;
  bool isFiletypeAvailable(FileType check) const override;

  virtual std::vector<std::shared_ptr<base_parameter>> readAlgorithms(std::istream& input) const;
  virtual std::vector<std::shared_ptr<base_parameter>> readRuns(std::istream& input) const;
  virtual std::vector<std::shared_ptr<base_parameter>> readBeamtimes(std::istream& input) const;
  virtual std::vector<std::shared_ptr<base_parameter>> readSetup(std::istream& input) const;
  virtual std::vector<std::shared_ptr<base_parameter>> readMaterials(std::istream& input) const;
  virtual std::vector<std::shared_ptr<base_parameter>> readShapes(std::istream& input) const;

  virtual std::shared_ptr<base_parameter> readSingleAlgorithm(std::istream& input) const;
  virtual std::shared_ptr<base_parameter> readSingleRun(std::istream& input) const;
  // That's already final.
  virtual std::shared_ptr<base_parameter> readSingleBeamtime(std::istream& input) const;
  // That's already final.
  virtual std::shared_ptr<base_parameter> readSingleReaction(std::istream& input) const;
  virtual std::shared_ptr<base_parameter> readSingleDetector(std::istream& input) const;
  // That's already final.
  virtual std::shared_ptr<base_parameter> readSingleMaterial(std::istream& input) const;
  virtual std::shared_ptr<base_parameter> readSingleShape(std::istream& input) const;

  virtual void recoverShapeId(std::shared_ptr<base_parameter>& shape) const;
  virtual void recoverAlgorithmId(std::shared_ptr<base_parameter>& algorithm) const;
  void setShapeIdRecovery(std::function<void(std::shared_ptr<base_parameter>&)> function) override;
  void setAlgorithmIdRecovery(
      std::function<void(std::shared_ptr<base_parameter>&)> function) override;

  FileType checkVersionString(std::string const&) const override;

private:
  std::function<void(shared_ptr<base_parameter>&)> _shapeRecovery;
  std::function<void(shared_ptr<base_parameter>&)> _algorithmRecovery;
};

} // namespace IO
} // namespace parameter
