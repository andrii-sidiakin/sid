# Sid.Prelude

add_library(sid_prelude INTERFACE)
add_library(Sid::prelude ALIAS sid_prelude)

set(COMPILE_CXX_GNU         $<COMPILE_LANG_AND_ID:CXX,GNU>)
set(COMPILE_CXX_Clang       $<COMPILE_LANG_AND_ID:CXX,Clang>)
set(COMPILE_CXX_AppleClang  $<COMPILE_LANG_AND_ID:CXX,AppleClang>)
set(COMPILE_CXX_MSVC        $<COMPILE_LANG_AND_ID:CXX,MSVC>)

set(COMPILE_CXX_LIKE_GCC    $<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>)
set(COMPILE_CXX_LIKE_MSVC   $<COMPILE_LANG_AND_ID:CXX,MSVC>)

set(SANITIZER_FLAGS_GNU_OR_CLANG
    -fsanitize=address
    -fsanitize=leak
    -fsanitize=pointer-compare
    -fsanitize=pointer-subtract
    -fsanitize=undefined
    -fsanitize-undefined-trap-on-error
)

target_compile_features(sid_prelude INTERFACE
    cxx_std_23
)

target_compile_options(sid_prelude INTERFACE
    # enable more warnings
    $<${COMPILE_CXX_LIKE_GCC}:$<BUILD_INTERFACE:-Wall -Wextra -Wpedantic>>
    $<${COMPILE_CXX_LIKE_MSVC}:$<BUILD_INTERFACE:/Wall>>
    # and treat them as errors in Release builds
    $<$<CONFIG:Release>:$<${COMPILE_CXX_LIKE_GCC}:$<BUILD_INTERFACE:-Werror>>>
    $<$<CONFIG:Release>:$<${COMPILE_CXX_LIKE_MSVC}:$<BUILD_INTERFACE:/WX>>>
)

target_link_options(sid_prelude INTERFACE
    $<${COMPILE_CXX_GNU}:${SANITIZER_FLAGS_GNU_OR_CLANG}>
    $<${COMPILE_CXX_Clang}:${SANITIZER_FLAGS_GNU_OR_CLANG}>
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
