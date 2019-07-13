#include "parameterio.h"

#include "jsonparameterreader.h"
#include "jsonparameterwriter.h"
#include "parameterreader0.h"
#include "parameterreader1.h"
#include "parameterreader2.h"
#include "parameterwriter0.h"
#include "parameterwriter1.h"
#include "parameterwriter2.h"
#include "versions.h"

namespace parameter {
namespace IO {

void ParameterIO::setup()
{
  installVersion(std::make_shared<ParameterReader0>(), std::make_shared<ParameterWriter0>(),
                 version_0);
  installVersion(std::make_shared<ParameterReader1>(), std::make_shared<ParameterWriter1>(),
                 version_1);
  installVersion(std::make_shared<ParameterReader2>(), std::make_shared<ParameterWriter2>(),
                 version_2);
  installVersion(std::make_shared<JsonParameterReader>(), std::make_shared<JsonParameterWriter>(),
                 version_json);
}

} // namespace IO
} // namespace parameter
