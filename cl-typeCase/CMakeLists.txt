project(cl-typecase)

add_executable(${PROJECT_NAME}
    include/loggingClass.h
    src/analysis.cpp
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC libanalyzer
    PUBLIC libeventpainter
    PUBLIC Boost::program_options
    PUBLIC ROOT::Core
    PUBLIC ROOT::RIO
    PUBLIC ROOT::Tree
    PUBLIC ROOT::Hist
    PUBLIC ROOT::Gpad
    PUBLIC ROOT::Physics
    PUBLIC ROOT::Postscript
    PUBLIC Qt5::Core
)
