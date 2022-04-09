cmake_minimum_required (VERSION 3.4)

project (TypeCase)
# The version number.
set (TypeCase_VERSION_MAJOR 1)
set (TypeCase_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
find_package(Qt5 REQUIRED COMPONENTS Core Test)
find_package(Boost REQUIRED COMPONENTS headers)
include(findRoot)
include(utilities)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O -Werror -Wall -fPIC ")
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

######################
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/af29db7ec28d6df1c7f0f745186884091e602e07.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Populate(googletest)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

include(GoogleTest)
#################################################

add_subdirectory(geometry)
add_subdirectory(parameter)
add_subdirectory(factories)
add_subdirectory(parameterIO)
add_subdirectory(shapes)
add_subdirectory(reaction)
add_subdirectory(container)
add_subdirectory(basic)
add_subdirectory(fit)

add_subdirectory(algorithms)
add_subdirectory(analyser)
add_subdirectory(eventpainter)

#add_subdirectory(cl-typeCase)

add_subdirectory(documentation)
add_subdirectory(code)
#add_subdirectory(gui)
#add_subdirectory(qt-utils)

add_subdirectory(UI)
