tc_add_library(libeventpainter
  HEADERS
    include/eventpainter.h
  SOURCES
    src/eventpainter.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Container
    PUBLIC Qt5::Core
    PUBLIC ROOT::Gpad
  ALIAS TypeCase::EventPainter
)

