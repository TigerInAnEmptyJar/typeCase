project(container)

add_library(lib${PROJECT_NAME} STATIC
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
)

target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libshape
    PUBLIC libbasic
)
