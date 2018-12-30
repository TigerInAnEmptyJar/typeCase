project(reaction)
add_library(lib${PROJECT_NAME} STATIC
    include/RthreeParticleDecay.h
    include/RtwoParticleDecay.h
    include/Rppelastic.h
    include/reactions.h
    include/Rpp_dpi+.h
    include/Rpp_pkl_pkppi.h
    src/Rpp_dpi+.cpp
    src/Rppelastic.cpp
    src/Rpp_pkl_pkppi.cpp
    src/RthreeParticleDecay.cpp
    src/RtwoParticleDecay.cpp
)
target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lib${PROJECT_NAME}
    PUBLIC libbasic
    PUBLIC libcontainer
    PUBLIC libfit
)
