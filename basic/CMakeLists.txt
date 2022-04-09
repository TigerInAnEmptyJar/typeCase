tc_add_library(libbasic
  HEADERS
    include/algorithm.h
    include/AReactionRecognition.h
    include/eLossAlgorithm.h
    include/fitAlgorithm.h
    include/RbaseReaction.h
    include/logger.h
  SOURCES
    src/algorithm.cpp
    src/AReactionRecognition.cpp
    src/eLossAlgorithm.cpp
    src/fitAlgorithm.cpp
    src/getReaction.cpp
    src/logger.cpp
    src/RbaseReaction.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC Qt5::Core
    PUBLIC TypeCase::Parameter
    PUBLIC TypeCase::Container
    PUBLIC TypeCase::Reaction
  ALIAS TypeCase::Basic
)

