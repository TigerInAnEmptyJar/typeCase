tc_add_library(libreaction
  HEADERS
    include/RthreeParticleDecay.h
    include/RtwoParticleDecay.h
    include/Rppelastic.h
    include/reactions.h
    include/Rpp_dpi+.h
    include/Rpp_pkl_pkppi.h
  SOURCES
    src/Rpp_dpi+.cpp
    src/Rppelastic.cpp
    src/Rpp_pkl_pkppi.cpp
    src/RthreeParticleDecay.cpp
    src/RtwoParticleDecay.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    PUBLIC TypeCase::Basic
    PUBLIC TypeCase::Container
    PUBLIC TypeCase::Fit
  ALIAS TypeCase::Reaction
)

