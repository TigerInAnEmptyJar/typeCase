tc_add_library(libcode
  HEADERS
    include/algorithmwriter.h
    include/shapewriter.h
  SOURCES
    src/algorithmwriter.cpp
    src/shapewriter.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Parameter
    PUBLIC TypeCase::Documentation
    PUBLIC TypeCase::Shape
  ALIAS TypeCase::Code
)

