tc_add_library(libparameter
  HEADERS
    include/shapeparameter.h
    include/algorithmparameter.h
    include/beamtimeparameter.h
    include/paintparameter.h
    include/detectorparameter.h
    include/materialparameter.h
    include/baseparameter.h
    include/valuesparameter.h
  SOURCES
    src/algorithmparameter.cpp
    src/beamtimeparameter.cpp
    src/materialparameter.cpp
    src/baseparameter.cpp
    src/detectorparameter.cpp
    src/paintparameter.cpp
    src/shapeparameter.cpp
    src/valuesparameter.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC libgeometry
    PUBLIC Qt5::Core
    PUBLIC Boost::headers
  ALIAS TypeCase::Parameter
)
