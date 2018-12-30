find_path(BOOST_ROOT boost-build.jam
    HINTS ${CMAKE_SOURCE_DIR}/../boost)
set(BOOST_INCLUDEDIR ${BOOST_ROOT})
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib)
include(FindBoost)

if(NOT (${Boost_program_options_LIBRARY_DEBUG} OR ${Boost_program_options_LIBRARY_RELEASE}))
    add_library(LIBBOOSTPROGRAMOPTIONS INTERFACE )
    target_link_libraries(LIBBOOSTPROGRAMOPTIONS
        INTERFACE ${BOOST_LIBRARYDIR}/libboost_program_options.a
        INTERFACE Boost::boost)
    add_library(Boost::program_options ALIAS LIBBOOSTPROGRAMOPTIONS)
endif()
