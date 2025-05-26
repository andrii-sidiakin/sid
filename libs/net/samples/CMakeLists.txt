
add_executable(sid.net.socket socket.cpp)
target_link_libraries(sid.net.socket PRIVATE Sid::net)
