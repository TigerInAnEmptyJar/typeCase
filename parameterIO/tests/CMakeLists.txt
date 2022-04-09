tc_add_test(parameterIoTest
  SOURCES
    algorithmIO_test.cpp
    beamtimeIO_test.cpp
    databaseIO_test.cpp
    detectorIO_test.cpp
    materialIO_test.cpp
    runIO_test.cpp
    shapeIO_test.cpp
    jsonIO_test.cpp
    fixture.cpp
#    testingUtilities.cpp
    #setupIO_test.cpp
  DEPENDS
    PRIVATE TypeCase::ParameterIO
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/algorithms_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/algorithms_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/algorithms_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/algorithms_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/algorithms_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/algorithms_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/beamtimes_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/beamtimes_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/beamtimes_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/beamtimes_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/beamtimes_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/beamtimes_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/detectors_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/detectors_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/detectors_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/detectors_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/detectors_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/detectors_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/materials_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/materials_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/materials_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/materials_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/materials_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/materials_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/runs_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/runs_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/runs_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/runs_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/runs_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/runs_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shapes_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/shapes_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shapes_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/shapes_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shapes_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/shapes_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs1_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs1_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs1_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs1_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs1_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs1_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs2_0.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs2_0.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs2_1.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs2_1.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_runs2_2.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_runs2_2.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/databases_0.data      ${CMAKE_CURRENT_BINARY_DIR}/data/databases_0.data      COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/databases_1.data      ${CMAKE_CURRENT_BINARY_DIR}/data/databases_1.data      COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/databases_2.data      ${CMAKE_CURRENT_BINARY_DIR}/data/databases_2.data      COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/algorithms_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/algorithms_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/all_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/all_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/beamtimes_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/beamtimes_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/database_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/database_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/detectors_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/detectors_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/materials_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/materials_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/process_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/process_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/runs_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/runs_json.data COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/shapes_json.data ${CMAKE_CURRENT_BINARY_DIR}/data/shapes_json.data COPYONLY)
