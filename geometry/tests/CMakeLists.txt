find_package(GTest)

add_executable(geometryTest
    geometryTest.cpp
    performanceTest.cpp
)

target_link_libraries(geometryTest
    PRIVATE libgeometry
    PRIVATE ${GTEST_BOTH_LIBRARIES}
    PRIVATE gmock
    PRIVATE pthread
)

gtest_add_tests(geometryTest
    SOURCES
        geometryTest.cpp
        performanceTest.cpp
)
