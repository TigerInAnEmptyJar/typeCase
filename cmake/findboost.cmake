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
if(NOT (${Boost_UUID_LIBRARY_DEBUG} OR ${Boost_UUID_LIBRARY_RELEASE}))
    add_library(LIBBOOSTUUID INTERFACE )
    target_link_libraries(LIBBOOSTUUID
        INTERFACE Boost::boost)
    add_library(Boost::uuid ALIAS LIBBOOSTUUID)
endif()
if(NOT (${Boost_SIGNALS2_LIBRARY_DEBUG} OR ${Boost_SIGNALS2_LIBRARY_RELEASE}))
    add_library(LIBBOOSTSIGNALS2 INTERFACE )
    target_link_libraries(LIBBOOSTSIGNALS2
        INTERFACE Boost::boost)
    add_library(Boost::signals2 ALIAS LIBBOOSTSIGNALS2)
endif()
