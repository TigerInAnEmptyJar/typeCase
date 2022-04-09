tc_add_library(liparameterIO
  HEADERS
    include/parameterio.h
    include/baseparameterwriter.h
    include/baseparameterreader.h
    include/parameterreader0.h
    include/parameterreader1.h
    include/parameterreader2.h
    include/jsonparameterreader.h
    include/parameterwriter0.h
    include/parameterwriter1.h
    include/parameterwriter2.h
    include/jsonparameterwriter.h
    include/versions.h
  SOURCES
    src/parameterio.cpp
    src/parameterio_setup.cpp
    src/parameterreader0.cpp
    src/parameterreader1.cpp
    src/parameterreader2.cpp
    src/jsonparameterreader.cpp
    src/parameterwriter0.cpp
    src/parameterwriter1.cpp
    src/parameterwriter2.cpp
    src/jsonparameterwriter.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Parameter
    PRIVATE Qt5::Core
    PRIVATE stdc++fs
  ALIAS TypeCase::ParameterIO
)

add_subdirectory(tests)
