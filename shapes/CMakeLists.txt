project(shape)
add_library(lib${PROJECT_NAME}basic STATIC
    include/volumeShape.h
    include/planeShape.h
    include/baseShape.h
    src/baseShape.cpp
    src/planeShape.cpp
    src/volumeShape.cpp
)
target_include_directories(lib${PROJECT_NAME}basic
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib${PROJECT_NAME}basic
    PUBLIC libgeometry
    PUBLIC libparameter
)

add_library(lib${PROJECT_NAME} STATIC
    include/shapes.h
    include/ring.h
    include/hexPrism.h
    include/sphericRectangle.h
    include/shape_utilities.h
    include/planarShapes.h
    include/spiral.h
    include/circle.h
    include/fiber.h
    include/strawTube.h
    include/cylinder.h
    include/quadrangle.h
    include/wedge.h
    include/provideShapes.h
    src/provideShapes.cpp
    src/cylinder.cpp
    src/getShape.cpp
    src/hexPrism.cpp
    src/quadrangle.cpp
    src/ring.cpp
    src/sphericRectangle.cpp
    src/spiral.cpp
    src/triangle.cpp
    src/wedge.cpp
    src/circle.cpp
    src/fiber.cpp
    src/hexagon.cpp
    src/rectangle.cpp
    src/shape_utilities.cpp
    src/sphericTriangle.cpp
    src/strawTube.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libshapebasic
    PUBLIC libfactory
)

add_subdirectory(tests)
