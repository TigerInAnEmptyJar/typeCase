function(tc_add_library _NAME)
  set(options SHARED INTERFACE)
  set(single_option ALIAS)
  set(multi_option HEADERS SOURCES DEPENDS PUBLIC_INCLUDES PRIVATE_INCLUDES INTERFACE_INCLUDES DEFINES)
  cmake_parse_arguments(ARGS "${options}" "${single_option}"
    "${multi_option}" ${ARGN})

  if(ARGS_SHARED)
    add_library(${_NAME} SHARED ${ARGS_HEADERS} ${ARGS_SOURCES})
  elseif(ARGS_INTERFACE)
    add_library(${_NAME} INTERFACE ${ARGS_HEADERS})
  else()
    add_library(${_NAME} ${ARGS_HEADERS} ${ARGS_SOURCES})
  endif()
  message(STATUS "Added Library ${_NAME} ${ARGS_ALIAS}")
  if(ARGS_DEPENDS)
    target_link_libraries(${_NAME} ${ARGS_DEPENDS})
  endif()

  if(ARGS_PUBLIC_INCLUDES)
    target_include_directories(${_NAME} PUBLIC "$<BUILD_INTERFACE:${ARGS_PUBLIC_INCLUDES}>")
  endif()
  if(ARGS_PRIVATE_INCLUDES)
    target_include_directories(${_NAME} PRIVATE "$<BUILD_INTERFACE:${ARGS_PRIVATE_INCLUDES}>")
  endif()
  if (ARGS_INTERFACE_INCLUDES)
    target_include_directories(${_NAME} INTERFACE "$<BUILD_INTERFACE:${ARGS_INTERFACE_INCLUDES}>")
  endif()
  if(ARGS_DEFINES)
    target_compile_definitions(${_NAME} ${ARGS_DEFINES})
  endif()

  if(ARGS_ALIAS)
    add_library(${ARGS_ALIAS} ALIAS ${_NAME})
  endif()
endfunction()

function(tc_add_executable _NAME)
  set(options )
  set(single_option )
  set(multi_option HEADERS SOURCES DEPENDS INCLUDES DEFINES)
  cmake_parse_arguments(ARGS "${options}" "${single_option}"
    "${multi_option}" ${ARGN})

  add_executable(${_NAME} ${ARGS_HEADERS} ${ARGS_SOURCES})
  if(ARGS_DEPENDS)
    target_link_libraries(${_NAME} ${ARGS_DEPENDS})
  endif()

  if(ARGS_INCLUDES)
    target_include_directories(${_NAME} ${ARGS_INCLUDES})
  endif()
  if(ARGS_DEFINES)
    target_compile_definitions(${_NAME} ${ARGS_DEFINES})
  endif()
endfunction()

function(tc_add_test _NAME)
  set(options MOCK)
  set(single_option TIMEOUT)
  set(multi_option HEADERS SOURCES DEPENDS INCLUDES DEFINES)
  cmake_parse_arguments(ARGS "${options}" "${single_option}"
    "${multi_option}" ${ARGN})

  set(dependencies PRIVATE gtest PRIVATE gtest_main PRIVATE gmock)
  if(ARGS_MOCK)
    list(APPEND dependencies PRIVATE gmock)
  endif()
  if (ARGS_DEPENDS)
    list(APPEND dependencies ${ARGS_DEPENDS})
  endif()
  tc_add_executable(${_NAME} SOURCES ${ARGS_SOURCES} HEADERS ${ARGS_HEADERS} DEPENDS ${dependencies} INCLUDES ${ARGS_INCLUDES} DEFINES ${ARGS_DEFINES})

  message(STATUS "Added Test ${_NAME}")

  gtest_discover_tests(${_NAME})

endfunction()
