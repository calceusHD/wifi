cmake_minimum_required(VERSION 3.4)

project(wifi)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -O0 -g -Wall -Wextra")

add_executable(wifi src/main.cpp src/network_wifi.cpp src/byteorder.cpp src/crc32.cpp src/packet.cpp src/frame.cpp)
if(__WIN32)
target_link_libraries(wifi ws2_32)
endif()
target_include_directories(wifi PRIVATE src)
