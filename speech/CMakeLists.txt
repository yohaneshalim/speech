cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(speech LANGUAGES CXX VERSION 1.0 DESCRIPTION "speech library")

add_subdirectory(world)

add_library(speech SHARED
  src/parameterio.cpp
  src/audioio.cpp
  src/speech.cpp
  include/speech.hpp
  include/jsonString.hpp
  include/audioio.h
  include/parameterio.h
  )
target_include_directories(speech PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(speech PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/world/src)
target_link_libraries(speech world world_tool pthread)

