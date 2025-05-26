#
#
#

#
add_custom_target(build-tests COMMENT "Build all tests")

#
#
#
function(sid_testsuite_setup)
    if(TARGET Sid::testsuite)
        return() # already done
    endif()

    if(NOT SID_FETCH_EXTERNAL)
        find_package(doctest REQUIRED)
    else()
        include(FetchContent)
        FetchContent_Declare(doctest
            GIT_REPOSITORY https://github.com/doctest/doctest
            GIT_TAG        master
            OVERRIDE_FIND_PACKAGE
        )
        find_package(doctest REQUIRED)
    endif()

    add_library(Sid::testsuite ALIAS doctest)

endfunction()

#
#
#
function (sid_add_target_to_test_list target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "Given '${target}' is not a target.")
    endif()
    if (NOT TARGET build-tests)
        message(WARNING "Sid testing is not enabled")
    else()
        add_dependencies(build-tests ${target})
    endif()
endfunction()

#
#
#
function(sid_define_test NAME)
    message(STATUS "Add Test: ${NAME}; opts=[${ARGN}]")

    cmake_parse_arguments(SID_TEST "" "GROUP" "SOURCES;LIBRARIES" ${ARGN})

    if(NOT DEFINED SID_TEST_GROUP)
        set(SID_TEST_TARGET "test.${NAME}")
    else()
        set(SID_TEST_TARGET "test.${SID_TEST_GROUP}.${NAME}")
    endif()

    if(NOT DEFINED SID_TEST_SOURCES)
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.cpp")
            message(FATAL_ERROR "[${SID_TEST_TARGET}]: "
                "No expected source file found: '${NAME}.cpp'")
        endif()
        set(SID_TEST_SOURCES "${NAME}.cpp")
    endif()


    add_executable(${SID_TEST_TARGET} ${SID_TEST_SOURCES})
    sid_add_target_to_test_list(${SID_TEST_TARGET})

    add_test(NAME ${SID_TEST_TARGET} COMMAND ${SID_TEST_TARGET})

    if(DEFINED SID_TEST_LIBRARIES)
        target_link_libraries(${SID_TEST_TARGET} PRIVATE Sid::testsuite
            ${SID_TEST_LIBRARIES})
    else()
        target_link_libraries(${SID_TEST_TARGET} PRIVATE Sid::testsuite)
    endif()

endfunction()

#
#
#
macro(sid_define_test_set GROUP_TAG)
    cmake_parse_arguments(SID_TEST_GROUP "" "GROUP" "LIBRARIES" ${ARGN})

    if(NOT DEFINED SID_TEST_GROUP_GROUP)
        set(SID_TEST_GROUP_GROUP "sid.${GROUP_TAG}")
    endif()

    function(sid_define_${GROUP_TAG}_test NAME)
        set(call_ARGN ARGN)
        sid_define_test(${NAME}
            GROUP "${SID_TEST_GROUP_GROUP}"
            LIBRARIES ${SID_TEST_GROUP_LIBRARIES}
            ${${call_ARGN}}
        )
    endfunction()

endmacro()

