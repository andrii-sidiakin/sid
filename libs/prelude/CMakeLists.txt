# Sid.Prelude

add_library(sid_prelude INTERFACE)
add_library(Sid::prelude ALIAS sid_prelude)

target_compile_features(sid_prelude INTERFACE
    cxx_std_23
)

set(COMPILER_IS_CXX_LIKE_GCC  $<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>)
set(COMPILER_IS_CXX_LIKE_MSVC $<COMPILE_LANG_AND_ID:CXX,MSVC>)

target_compile_options(sid_prelude INTERFACE
    # enable more warnings
    $<${COMPILER_IS_CXX_LIKE_GCC}:$<BUILD_INTERFACE:-Wall -Wextra -Wpedantic>>
    $<${COMPILER_IS_CXX_LIKE_MSVC}:$<BUILD_INTERFACE:/Wall>>
    # and treat them as errors in Release builds
    $<$<CONFIG:Release>:$<${COMPILER_IS_CXX_LIKE_GCC}:$<BUILD_INTERFACE:-Werror>>>
    $<$<CONFIG:Release>:$<${COMPILER_IS_CXX_LIKE_MSVC}:$<BUILD_INTERFACE:/WX>>>
)

target_include_directories(sid_prelude INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/sid
    DESTINATION include)

if(SID_BUILD_TESTS)
    add_subdirectory(tests)
endif()
