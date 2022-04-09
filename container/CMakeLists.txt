tc_add_library(libcontainer
  HEADERS
    include/detector.h
    include/CommonCalibrationParser.h
    include/rawHit.h
    include/cluster.h
    include/material.h
    include/setup.h
    include/calibHit.h
    include/pixel.h
    include/container_utilities.h
    include/track.h
    include/event.h
    include/container.h
    include/TBase.h
    include/hitCluster.h
  SOURCES
    src/calibHit.cpp
    src/CommonCalibrationParser.cpp
    src/detector.cpp
    src/hitCluster.cpp
    src/pixel.cpp
    src/setup.cpp
    src/track.cpp
    src/cluster.cpp
    src/container_utilities.cpp
    src/event.cpp
    src/material.cpp
    src/rawHit.cpp
    src/TBase.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Shape
    PUBLIC TypeCase::Basic
  ALIAS TypeCase::Container
)

