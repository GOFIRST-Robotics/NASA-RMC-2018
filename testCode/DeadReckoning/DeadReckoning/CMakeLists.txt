cmake_minimum_required(VERSION 2.8.0)
project(DeadReckoning)
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch)

FIND_PACKAGE (Threads REQUIRED)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.h
)

add_library(ahrs SHARED ${SOURCES})
set_target_properties(ahrs PROPERTIES 
  PUBLIC_HEADER "AHRS.h"
  FRAMEWORK TRUE
)

add_executable(NavXDeadReckoning "${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.cpp"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/DeadReckoning.h")
target_link_libraries(NavXDeadReckoning ${CMAKE_THREAD_LIBS_INIT})
#target_compile_features(NavXTimeStamp PRIVATE cxx_range_for)

install(TARGETS ahrs 
        LIBRARY DESTINATION /usr/local/lib
        PUBLIC_HEADER DESTINATION /usr/local/include
)

install(TARGETS ahrs NavXDeadReckoning
        LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
        RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)
