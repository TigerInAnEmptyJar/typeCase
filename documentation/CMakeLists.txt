project(documentation)

find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM )
endif (DOXYGEN_FOUND)

tc_add_library(libdocumentation
  HEADERS
    include/tmyclass.h
  SOURCES
    src/functionnametypedoc.cpp
    src/nametypedoc.cpp
    src/tmyclass.cpp
    src/utils.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Geometry
    PUBLIC Qt5::Core
  ALIAS TypeCase::Documentation
)

