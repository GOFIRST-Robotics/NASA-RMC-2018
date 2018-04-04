cmake_minimum_required(VERSION 2.8.0)
project(deadReckoning)
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch)

set ( PROJECT_LINK_LIBS libahrs.so)
link_directories( ~/Documents/Workspace/workspace/NASA-RMC-2018/testCode/DeadReckoning/DeadReckoning/build )

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(deadReckoning ${SOURCES})
target_link_libraries(deadReckoning ${PROJECT_LINK_LIBS})
