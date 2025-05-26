# sid.net

add_library(sid_net)
add_library(Sid::net ALIAS sid_net)

target_link_libraries(sid_net PUBLIC Sid::prelude Sid::bio)

target_include_directories(sid_net PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(sid_net PRIVATE
    "src/error.cpp"
    "src/bsd/socket_impl.cpp"
)

if(SID_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(samples)
