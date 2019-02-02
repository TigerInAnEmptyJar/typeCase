project(parameter)

add_library(lib${PROJECT_NAME} STATIC
    include/shapeparameter.h
    include/algorithmparameter.h
    include/beamtimeparameter.h
    include/paintparameter.h
    include/detectorparameter.h
    include/materialparameter.h
    include/baseparameter.h
    include/parameterManager.h
    src/versions3.h
    src/versions6.h
    src/versions2.h
    src/versions5.h
    src/versions0.h
    src/versions1.h
    src/versions4.h
    src/algorithmparameter.cpp
    src/beamtimeparameter.cpp
    src/materialparameter.cpp
    src/parameterManager.cpp
    src/baseparameter.cpp
    src/detectorparameter.cpp
    src/paintparameter.cpp
    src/shapeparameter.cpp
)

target_include_directories(lib${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libgeometry
    PUBLIC Qt5::Core
    PUBLIC Boost::uuid
)
