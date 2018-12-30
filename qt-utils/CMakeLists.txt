project(qt-utils)
add_library(lib${PROJECT_NAME} STATIC
    src/utilities.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
