cmake_minimum_required(VERSION 3.5)

add_subdirectory(speech)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CC_FLAGS "-Wall -Wextra")
add_executable(main main.cpp)
target_link_libraries(main speech)
include_directories(speech/src)
