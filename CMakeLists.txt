cmake_minimum_required(VERSION 3.5.1)
project (server C)

set (SERVER_SRCS
src/networking.c
src/queue.c
src/connection.c 
src/server.c)

add_executable(server ${SERVER_SRCS})
target_include_directories(server PUBLIC include)
target_link_libraries(server PUBLIC pthread)
