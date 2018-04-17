cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(lidar_stand_visualizer)
# set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch)

include_directories(/usr/local)

find_package(OpenCV REQUIRED)
find_package(CUDA 9.1 REQUIRED)
find_package(PCL 1.2 REQUIRED)
find_package(Sweep REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (lidar_stand_visualizer lidarRealtimeViz.cpp Lidar.cpp)
target_link_libraries (lidar_stand_visualizer ${PCL_LIBRARIES} ${OpenCV_LIBS} sweep)
