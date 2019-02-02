find_package(GTest)

set(SHAPE_TEST_SOURCES
    providerTest.cpp
    circleTest.cpp
    hexagonTest.cpp
    quadrangleTest.cpp
    rectangleTest.cpp
    sphericRectangleTest.cpp
    sphericTriangleTest.cpp
    triangleTest.cpp
    cylinderTest.cpp
    fiberTest.cpp
    hexPrismTest.cpp
    ringTest.cpp
    spiralTest.cpp
    wedgeTest.cpp
)
add_executable(shapesTest
    ${SHAPE_TEST_SOURCES}
)

target_link_libraries(shapesTest
    PRIVATE libshape
    PRIVATE ${GTEST_BOTH_LIBRARIES}
    PRIVATE gmock
    PRIVATE pthread
)

gtest_add_tests(shapesTest
    SOURCES ${SHAPE_TEST_SOURCES}
)
