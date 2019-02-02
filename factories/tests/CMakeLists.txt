find_package(GTest)

add_executable(factoryTest
    shapeFactoryTest.cpp
)

target_link_libraries(factoryTest
    PRIVATE libfactory
    PRIVATE ${GTEST_BOTH_LIBRARIES}
    PRIVATE gmock
    PRIVATE pthread
)

gtest_add_tests(factoryTest
    SOURCES shapeFactoryTest.cpp
)
