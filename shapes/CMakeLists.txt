project(shape)
add_library(lib${PROJECT_NAME} STATIC
    include/volumeShape.h
    include/shapes.h
    include/ring.h
    include/hexPrism.h
    include/sphericRectangle.h
    include/shape_utilities.h
    include/spiral.h
    include/planeShape.h
    include/circle.h
    include/fiber.h
    include/strawTube.h
    include/baseShape.h
    include/cylinder.h
    include/quadrangle.h
    include/wedge.h
    src/baseShape.cpp
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
    src/planeShape.cpp
    src/rectangle.cpp
    src/shape_utilities.cpp
    src/sphericTriangle.cpp
    src/strawTube.cpp
    src/volumeShape.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libgeometry
    PUBLIC libparameter
)
