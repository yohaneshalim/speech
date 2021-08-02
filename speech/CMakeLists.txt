cmake_minimum_required(VERSION 3.9)

project(speech LANGUAGES CXX)
set(MAIN_PROJECT OFF)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CC_FLAGS "-Wall -Wextra")
project(speech VERSION 1.0 DESCRIPTION "speech library")
add_library(speech SHARED
  src/speech.cpp
  include/speech.hpp
  include/jsonString.hpp
  include/audioio.h
  include/parameterio.h
  src/parameterio.cpp
  src/audioio.cpp
  )
#add_library(world STATIC IMPORTED)
#set_target_properties(world PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libworld.a)
target_include_directories(speech PUBLIC "include")
target_link_directories(speech PUBLIC "lib")
target_link_libraries(speech world world_tool pthread)
