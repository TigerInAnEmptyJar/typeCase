project(code)

add_library(lib${PROJECT_NAME} STATIC
    include/algorithmwriter.h
    include/shapewriter.h
    src/algorithmwriter.cpp
    src/shapewriter.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libparameter
    PUBLIC libdocumentation
    PUBLIC libshape
)
