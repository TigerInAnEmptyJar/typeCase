project(parameterIO)

add_library(lib${PROJECT_NAME} STATIC
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
)

target_include_directories(lib${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libparameter
    PRIVATE Qt5::Core
    PRIVATE stdc++fs
)

add_subdirectory(tests)
