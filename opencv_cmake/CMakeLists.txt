cmake_minimum_required(VERSION 3.5)

project(opencvCMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-no-pie")

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(opencvCMake main.cpp)
target_link_libraries( opencvCMake ${OpenCV_LIBS} )
