#! @brief minimal build for tetri game 
#! @author Umar Ba <jUmarB@protonmail.com> 

cmake_minimum_required(VERSION 3.25)
project(Ttris VERSION 0.1 DESCRIPTION "Tertis Game") 

option(BUILD_RELEASE "Build stage  by default it's in Debug"  OFF) 
if(NOT BUILD_STAGE)
  set(CMAKE_BUILD_TYPE Debug)
endif() 

find_package(Curses REQUIRED) 
message(CHECK_START "looking curses lib ") 
if(NOT CURSES_FOUND) 
  message(FATAL "Require curses to build the project")
endif() 
message(CHECK_PASS "OK") 

FILE(GLOB_RECURSE src  src/*.c)  

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  
    ${CMAKE_SOURCE_DIR}/bin) 

add_executable(${PROJECT_NAME}  ${src}) 

target_link_libraries(${PROJECT_NAME}  ${CURSES_LIBRARY}) 
