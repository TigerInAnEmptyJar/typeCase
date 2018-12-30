cmake_minimum_required (VERSION 3.4)

project (TypeCase)
# The version number.
set (TypeCase_VERSION_MAJOR 1)
set (TypeCase_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(findboost)
include(findQt)
include(findRoot)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O -Werror -Wall -fPIC ")
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

add_subdirectory(geometry)
add_subdirectory(parameter)
add_subdirectory(shapes)
add_subdirectory(reaction)
add_subdirectory(container)
add_subdirectory(basic)
add_subdirectory(fit)

add_subdirectory(algorithms)
add_subdirectory(analyser)
add_subdirectory(eventpainter)

add_subdirectory(cl-typeCase)

add_subdirectory(documentation)
add_subdirectory(code)
#add_subdirectory(gui)
#add_subdirectory(qt-utils)
