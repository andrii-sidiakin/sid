add_executable(test_mt mt.cpp)
target_link_libraries(test_mt PRIVATE Sid::prelude)
add_test(NAME sid.mt COMMAND test_mt)
