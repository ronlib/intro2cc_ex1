cmake_minimum_required(VERSION 3.5)
project(ex1)

# first we add the executable that generates the table
add_executable(test_bit_serializer main_test.c bit_serializer.c)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(channel channel.c)
add_executable(sender sender.c connect_socket.c)
add_executable(receiver receiver.c connect_socket.c)
