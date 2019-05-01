project(geometry)

add_library(lib${PROJECT_NAME} STATIC
    include/Eparticles.h
    include/definitions.h
    include/geometry.h
    include/string_utilities.h
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
)

target_include_directories(lib${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(newGeometry INTERFACE)
target_include_directories(newGeometry
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(tests)
