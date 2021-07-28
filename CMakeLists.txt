cmake_minimum_required(VERSION 3.5)

add_subdirectory(World)
add_subdirectory(json)
include_directories(World/src/)
include_directories(World/tools/)
project(speech LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-mtune=native -Wall -Wextra")
set(CMAKE_CC_FLAGS "-mtune=native -Wall -Wextra")

add_executable(speech main.cpp)
target_link_libraries(speech world world_tool nlohmann_json pthread)
