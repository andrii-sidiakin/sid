# sid.bio

add_library(sid_bio)
add_library(Sid::bio ALIAS sid_bio)

target_link_libraries(sid_bio PUBLIC Sid::prelude)

target_include_directories(sid_bio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(sid_bio PRIVATE
    "src/bio.cpp"
)

if(SID_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(samples)

