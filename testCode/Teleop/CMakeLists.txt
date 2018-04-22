cmake_minimum_required(VERSION 2.8.0)
project(Teleop)
add_definitions(-std=c++11 -Wall -Wextra -Wno-switch)

FIND_PACKAGE (Threads REQUIRED)
FIND_PACKAGE(serial REQUIRED)

set(MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
include_directories( 
  "${MAIN_DIR}/src/util"
  "${MAIN_DIR}/include" 
  "${CMAKE_CURRENT_SOURCE_DIR}" )

if(MAKE-LAPTOP-JOYSTICK)
  add_executable(LaptopJoystick.out 
    "${CMAKE_CURRENT_SOURCE_DIR}/LaptopJoystickWifi.cpp"
    "${MAIN_DIR}/src/util/Telecomm.cpp"
    "${MAIN_DIR}/src/util/Formatter.cpp"
    "${MAIN_DIR}/src/util/joystick.cc")

elseif(MAKE-PI-RECIEVER AND (UDP OR SERIAL))
  if(UDP)
    add_definitions(-DUDP)
  elseif(SERIAL)
    add_definitions(-DSERIAL)
  endif()
  add_executable(PiReciever.out 
    "${CMAKE_CURRENT_SOURCE_DIR}/LaptopJoystickWifi.cpp"
    "${MAIN_DIR}/src/util/Telecomm.cpp"
    "${MAIN_DIR}/src/util/Formatter.cpp")

  target_link_libraries(PiReciever.out Serial ${CMAKE_THREAD_LIBS_INIT})
endif()
