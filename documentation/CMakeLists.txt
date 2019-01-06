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

add_library(lib${PROJECT_NAME} STATIC
    include/tmyclass.h
    src/functionnametypedoc.cpp
    src/nametypedoc.cpp
    src/tmyclass.cpp
    src/utils.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libgeometry
    PUBLIC Qt5::Core
)

#add_exectuable(${PRIJECT_NAME} STATIC
#src/doc_classes.cpp
#src/doc_display.cpp
#    src/doc_selectclassesdialog.cpp
#    src/documentation_mw.cpp
#    src/doc_update_dialog.cpp
#    src/main.cpp
#    src/qmake_image_collection.cpp
#    src/doc_classes.ui.h
#    src/doc_display.ui.h
#    src/doc_selectclassesdialog.ui.h
#    src/documentation_mw.ui.h
#)
