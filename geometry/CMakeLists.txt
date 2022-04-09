tc_add_library(libgeometry
  HEADERS
    include/Eparticles.h
    include/definitions.h
    include/geometry.h
    include/string_utilities.h
  SOURCES
    src/Eparticles.cpp
    src/lLine3D.cpp
    src/matrix_decomposition.c
    src/Momentum4D.cpp
    src/Point3D.cpp
    src/projector.cpp
    src/string_utilities.cpp
    src/Vector4D.cpp
    src/geomObj.cpp
    src/LU_Decomposition.cpp
    src/matrix_inversion.c
    src/plane3D.cpp
    src/Point4D.cpp
    src/sLine2D.cpp
    src/Vector2D.cpp
    src/Vector.cpp
    src/lLine2D.cpp
    src/matrix3D.cpp
    src/matrixNxM.cpp
    src/Point2D.cpp
    src/Point.cpp
    src/sLine3D.cpp
    src/Vector3D.cpp
  PUBLIC_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  ALIAS TypeCase::Geometry
)

tc_add_library(newGeometry INTERFACE
  HEADERS
    include/geometry_new.h
  INTERFACE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  ALIAS TypeCase::Geometry::New
)

add_subdirectory(tests)
