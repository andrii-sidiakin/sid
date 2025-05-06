# Sid

cmake_minimum_required(VERSION 3.31)
project(Sid VERSION 0.0.0)

option(SID_BUILD_TESTS "Build tests" YES)
option(SID_BUILD_EXAMPLES "Build examples" YES)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_EXTENSIONS OFF)

if(SID_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(libs/prelude)

if(SID_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
