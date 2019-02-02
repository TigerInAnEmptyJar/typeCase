project(factory)

add_library(lib${PROJECT_NAME} STATIC
    include/shapeFactory.h
    include/algorithmFactory.h
    src/shapeFactory.cpp
    src/algorithmFactory.cpp
)

target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libparameter
    PUBLIC libshapebasic
    PUBLIC Boost::uuid
)

add_subdirectory(tests)
