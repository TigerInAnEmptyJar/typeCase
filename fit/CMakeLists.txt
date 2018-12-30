project(fit)
add_library(lib${PROJECT_NAME} STATIC
    include/fittingRoutines.h
    src/kinfit.cpp
    src/lineFit.cpp
    src/vertexFit.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libgeometry
)
