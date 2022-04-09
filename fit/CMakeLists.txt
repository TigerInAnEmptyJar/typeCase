tc_add_library(libfit
  HEADERS
    include/fittingRoutines.h
  SOURCES
    src/kinfit.cpp
    src/lineFit.cpp
    src/vertexFit.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    TypeCase::Geometry
  ALIAS TypeCase::Fit
)

