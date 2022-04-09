tc_add_library(libfactory
  HEADERS
    include/shapeFactory.h
    include/algorithmFactory.h
  SOURCES
    src/shapeFactory.cpp
    src/algorithmFactory.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC libparameter
    PUBLIC libshapebasic
    PUBLIC libbasic
    PUBLIC Boost::headers
  ALIAS TypeCase::Factory
)

add_subdirectory(tests)
