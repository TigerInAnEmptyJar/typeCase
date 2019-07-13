project(analyzer)

add_library(lib${PROJECT_NAME} STATIC
    include/tofAnalysis_t.h
    src/tofAnalysis_algorithm_t.cpp
    src/tofAnalysis_consts_t.cpp
    src/tofAnalysis_data_t.cpp
    src/tofAnalysis_inits_t.cpp
    src/tofAnalysis_t.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libalgorithm
    PUBLIC libparameterIO
)
