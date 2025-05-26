# Sid

cmake_minimum_required(VERSION 3.31)
project(Sid VERSION 0.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(SID_BUILD_TESTS "Build tests" YES)
option(SID_BUILD_EXAMPLES "Build examples" YES)
option(SID_ENABLE_SANITIZER "Enable sanitizer" OFF)
option(SID_FETCH_EXTERNAL "Fetch external dependecies on demand"
    ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_EXTENSIONS OFF)

if(SID_BUILD_TESTS)
    enable_testing()
    include(sid_testsuite)
    sid_testsuite_setup()
endif()

add_subdirectory(libs/prelude)
add_subdirectory(libs/bio)
add_subdirectory(libs/net)

if(SID_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
