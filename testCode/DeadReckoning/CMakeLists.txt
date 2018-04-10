cmake_minimum_required(VERSION 2.8.0)
project(DeadReckoning)
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch)

set ( PROJECT_LINK_LIBS libahrs.so )
link_directories( "${CMAKE_CURRENT_SOURCE_DIR}/navXTimeSync" )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/navXTimeSync"
                     "${CMAKE_CURRENT_SOURCE_DIR}" )

add_executable(deadreckoning "${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.cpp"
                             "${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.h"
                             "${CMAKE_CURRENT_SOURCE_DIR}/NavX.cpp")

target_link_libraries(deadreckoning AHRS)
#target_link_libraries( deadreckoning ${PROJECT_LINK_LIBS} )
