project(basic)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(lib${PROJECT_NAME} STATIC
    include/algorithm.h
    include/AReactionRecognition.h
    include/eLossAlgorithm.h
    include/fitAlgorithm.h
    include/RbaseReaction.h
    include/logger.h
    src/algorithm.cpp
    src/AReactionRecognition.cpp
    src/eLossAlgorithm.cpp
    src/fitAlgorithm.cpp
    src/getReaction.cpp
    src/logger.cpp
    src/RbaseReaction.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC Qt5::Core
    PUBLIC libparameter
    PUBLIC libcontainer
    PUBLIC libreaction
)
