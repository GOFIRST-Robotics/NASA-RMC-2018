cmake_minimum_required(VERSION 2.8.11)
project(VisionBeacons)
set (CMAKE_CXX_STANDARD 11)
include_directories(/usr/local)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
get_filename_component(testCode_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(testCode_DIR ${testCode_DIR} DIRECTORY)
include_directories(${testCode_DIR}/VisionBeacons)
include_directories(${testCode_DIR}/Camera)

add_executable(VisionBeaconsTest.out ${testCode_DIR}/VisionBeacons/VisionBeacons.cpp ./runVisionBeacons.cpp ${testCode_DIR}/Camera/Camera.cpp)
target_link_libraries(VisionBeaconsTest.out ${OpenCV_LIBS})
