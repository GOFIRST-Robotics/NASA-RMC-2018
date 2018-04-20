cmake_minimum_required(VERSION 2.8.11)
project(Lidar)
set (CMAKE_CXX_STANDARD 11)
include_directories(/usr/local)

find_package(OpenCV REQUIRED)
find_package(CUDA 9.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Sweep REQUIRED)

add_executable(lidarTest.out ./LIDARtest.cpp ./Lidar.cpp)
target_link_libraries(lidarTest.out ${OpenCV_LIBS} sweep)
