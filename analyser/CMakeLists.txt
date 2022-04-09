tc_add_library(libanalyzer
  HEADERS
    include/tofAnalysis_t.h
  SOURCES
    src/tofAnalysis_algorithm_t.cpp
    src/tofAnalysis_consts_t.cpp
    src/tofAnalysis_data_t.cpp
    src/tofAnalysis_inits_t.cpp
    src/tofAnalysis_t.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Algorithm
    PUBLIC TypeCase::ParameterIO
  ALIAS TypeCase::Analyzer
)

