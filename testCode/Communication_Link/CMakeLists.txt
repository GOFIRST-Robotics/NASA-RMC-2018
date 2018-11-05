cmake_minimum_required(VERSION 2.8.3)
project(Communication_Link)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED roscpp geometry_msgs serial)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include
  ${catkin_INCLUDE_DIRS}
)

add_executable(Instructor_Node
    src/Instructor_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../testCode/Communication/formatter/Formatter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../testCode/Communication/formatter/Formatter.hpp
  )



add_dependencies(Instructor_Node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(Instructor_Node
  ${catkin_LIBRARIES}
)
