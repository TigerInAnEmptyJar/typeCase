project(eventpainter)
add_library(lib${PROJECT_NAME} STATIC
    include/eventpainter.h
    src/eventpainter.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libcontainer
    PUBLIC Qt5::Core
    PUBLIC ROOT::Gpad
)
